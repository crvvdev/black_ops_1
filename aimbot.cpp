#include "includes.hpp"

namespace Cheats
{
namespace Aimbot
{
int g_bestTargetId = -1;

void GetSelectedBone(centity_s *pEnt, vec3_t aimPos)
{
    switch (Options::Aimbot::Bone)
    {
    default:
        Engine::GetTagPos(pEnt, Engine::Tags::Helmet, aimPos);
        break;

    case 1:
        Engine::GetTagPos(pEnt, Engine::Tags::Chest, aimPos);
        break;

    case 2:
        Engine::GetTagPos(pEnt, Engine::Tags::Stomach, aimPos);
        break;
    }
}

int FindBestTarget()
{
    int bestTargetId = -1;
    float distMin = FLT_MAX;
    float fovMin = Options::Aimbot::FOV;

    clientInfo_t *localClient = GetLocalClient();

    for (int i = 0; i < ENTITYNUM_WORLD; ++i)
    {
        clientInfo_t *client = GetClientByIndex(i);
        centity_s *entity = GetEntityByIndex(i);

        if (!entity || !client || !entity->Alive || !entity->Valid || client->clientNum == localClient->clientNum ||
            entity->type != TYPE_HUMAN)
        {
            continue;
        }

        bool aimOnFriendly = (client->Team == localClient->Team && Engine::IsTeamBasedGametype(cgs->gametype));

        if (Options::Aimbot::AimOnFriendly)
        {
            aimOnFriendly = false;
        }

        if (aimOnFriendly)
        {
            continue;
        }

        vec3_t bonePos{};
        GetSelectedBone(entity, bonePos);

        const bool isVisible = Engine::IsVisible(refdef->ViewOrigin, bonePos, ps->clientNum);

        if (!Options::Aimbot::AutoWall)
        {
            if (Options::Aimbot::VisibleCheck && !isVisible)
            {
                continue;
            }
        }
        else
        {
            // check if the shot can penetatre walls
            float multiplier = Engine::GetAutoWallDamageMultiplier(refdef->ViewOrigin, bonePos);

            if (multiplier == 0.0 && !isVisible)
            {
                continue;
            }
        }

        if (Options::Aimbot::Style == 0)
        {
            // find best target based on fov
            const vec3_t viewAngle(refdefViewAngles->angles[1], refdefViewAngles->angles[0],
                                   refdefViewAngles->angles[2]);

            vec3_t aimAngle{};
            Math::CalcAngle(refdef->ViewOrigin, bonePos, aimAngle);

            const float fov = Math::GetFov(viewAngle, aimAngle);
            if (fov < fovMin)
            {
                fovMin = fov;
                bestTargetId = i;
            }
        }
        else if (Options::Aimbot::Style == 1)
        {
            // find best target based on distance
            const float distance = Math::GetDistance3D(bonePos, refdef->ViewOrigin);
            if (distance < distMin)
            {
                distMin = distance;
                bestTargetId = i;
            }
        }
    }

    return bestTargetId;
}

void Run()
{
    if (!Options::Aimbot::Enabled)
    {
        return;
    }

    if (!Options::Aimbot::AutoAim && !Options::Aimbot::UseAimKey)
    {
        Options::Aimbot::AutoAim = true;
    }

    g_isAimLocked = false;
    g_bestTargetId = FindBestTarget();

    if (g_bestTargetId == -1)
    {
        return;
    }

    centity_s *entity = GetEntityByIndex(g_bestTargetId);
    if (!entity)
    {
        return;
    }

    vec3_t aimBone{};
    GetSelectedBone(entity, aimBone);

    if (Options::Aimbot::Prediction)
    {
        float ping = cg->snap->ping / 50.f;

        // apply a little prediction on target
        vec3_t vecVelocity{};
        VectorSubtract(entity->vOrigin, entity->vOldOrigin, vecVelocity);

        Math::VectorMA(aimBone, ping, vecVelocity, aimBone);
    }

    // we're currently locked into a target
    g_isAimLocked = true;

    vec3_t aimAngle = {};
    Math::CalcAngle(aimBone, refdef->ViewOrigin, aimAngle);

    g_deltaX = aimAngle[0] - refdefViewAngles->angles[YAW];
    g_deltaY = aimAngle[1] - refdefViewAngles->angles[PITCH];

    bool shouldAim = false;

    if (Options::Aimbot::AutoAim)
    {
        shouldAim = true;
    }
    else
    {
        if (!Options::Aimbot::UseAimKey)
        {
            shouldAim = true;
        }
        else if (::GetAsyncKeyState(Options::Aimbot::Key) & (1 << 16))
        {
            shouldAim = true;
        }
    }

    if (shouldAim && !Options::Aimbot::SilentAim)
    {
        float *UnclampedViewAngleY = (float *)(VIEWANGLES);
        float *UnclampedViewAngleX = (float *)(VIEWANGLES + 4);

        *UnclampedViewAngleX += g_deltaX;
        *UnclampedViewAngleY += g_deltaY;
    }
}

void AutoFire(usercmd_t *cmd)
{
    if (Options::Aimbot::AutoFire && g_isAimLocked)
    {
        cmd->buttons[0] &= ~0x80000000;
        cmd->buttons[0] |= 0x80000000;

        if (Engine::IsAkimbo())
        {
            cmd->buttons[0] |= 0x100080;
        }
    }
}

void SilentAim(usercmd_t *cmd)
{
    if (Options::Aimbot::SilentAim && g_isAimLocked)
    {
        cmd->viewAngles[0] += ANGLE2SHORT(g_deltaY);
        cmd->viewAngles[1] += ANGLE2SHORT(g_deltaX);
    }
}

void Render()
{
    if (!Options::Aimbot::Enabled)
    {
        return;
    }

    if (Options::Aimbot::Style == 0)
    {
        const float scale = tanf(DEG2RAD(Options::Aimbot::FOV) / 2.f) / tanf(refdef->fov[0]);
        const float radius = scale * static_cast<float>(refdef->width / 2);

        g_renderer->AddCircle(g_drawList,
                              Vec2(static_cast<float>(refdef->width / 2), static_cast<float>(refdef->height / 2)),
                              radius, D3DCOLOR_XRGB(255, 255, 255));
    }

    if (Options::Aimbot::DrawLocked && g_bestTargetId != -1)
    {
        centity_s *entity = GetEntityByIndex(g_bestTargetId);
        if (!entity)
        {
            return;
        }

        vec3_t headPos{};
        Engine::GetTagPos(entity, Engine::Tags::Head, headPos);

        vec2_t screenPos = {};
        if (Math::WorldToScreen(headPos, screenPos))
        {
            g_renderer->AddCircle(g_drawList, Vec2(screenPos[0], screenPos[1]), 10.f, D3DCOLOR_XRGB(255, 255, 255));
        }
    }
}
} // namespace Aimbot
} // namespace Cheats