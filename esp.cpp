#include "includes.hpp"

namespace Cheats
{
namespace ESP
{
unsigned int Bone_Middle[5], Bone_Right_Arm[5], Bone_Left_Arm[5], Bone_Right_Leg[5], Bone_Left_Leg[5];

enum EAlignSide : uint8_t
{
    SIDE_LEFT = 0U,
    SIDE_TOP,
    SIDE_RIGHT,
    SIDE_BOTTOM,
    SIDE_MAX
};

struct CBox
{
    vec4_t bb;
    Vec2 points[8];
    float centerX, centerY;
};

void GetEntityBoundingBox(centity_s *entity, vec3_t &vMin, vec3_t &vMax)
{
    vec3_t pos;
    bool firstBone = true;

    auto updateBoundingBox = [&](const vec3_t &bonePos) {
        if (firstBone)
        {
            VectorCopy(bonePos, vMin);
            VectorCopy(bonePos, vMax);

            firstBone = false;
        }
        else
        {
            vMin[0] = std::min(vMin[0], bonePos[0]);
            vMin[1] = std::min(vMin[1], bonePos[1]);
            vMin[2] = std::min(vMin[2], bonePos[2]);

            vMax[0] = std::max(vMax[0], bonePos[0]);
            vMax[1] = std::max(vMax[1], bonePos[1]);
            vMax[2] = std::max(vMax[2], bonePos[2]);
        }
    };

    for (int i = 0; i < 5; ++i)
    {
        Engine::GetTagPos(entity, Bone_Middle[i], pos);
        updateBoundingBox(pos);

        Engine::GetTagPos(entity, Bone_Right_Arm[i], pos);
        updateBoundingBox(pos);

        Engine::GetTagPos(entity, Bone_Left_Arm[i], pos);
        updateBoundingBox(pos);

        Engine::GetTagPos(entity, Bone_Right_Leg[i], pos);
        updateBoundingBox(pos);

        Engine::GetTagPos(entity, Bone_Left_Leg[i], pos);
        updateBoundingBox(pos);
    }
}

bool GetEntityBox(centity_s *entity, CBox &bbox)
{
    vec3_t min, max;
    GetEntityBoundingBox(entity, min, max);

    vec3_t origin;
    VectorCopy(entity->vOrigin, origin);

    vec3_t corners[8];
    VectorCopy(min, corners[0]);
    VectorSet(corners[1], max[0], min[1], min[2]);
    VectorSet(corners[2], min[0], max[1], min[2]);
    VectorSet(corners[3], max[0], max[1], min[2]);
    VectorSet(corners[4], min[0], min[1], max[2]);
    VectorSet(corners[5], max[0], min[1], max[2]);
    VectorSet(corners[6], min[0], max[1], max[2]);
    VectorCopy(max, corners[7]);

    // Rotate the corners based on entity angles
    for (int i = 0; i < 8; ++i)
    {
        // We have to obtain the relative pos since GetEntityBoundingBox returns absolute positions.
        vec3_t relativePos;
        relativePos[0] = corners[i][0] - origin[0];
        relativePos[1] = corners[i][1] - origin[1];
        relativePos[2] = corners[i][2] - origin[2];

        Math::Rotate2D(relativePos, entity->vAngles[YAW]);

        corners[i][0] = relativePos[0] + origin[0];
        corners[i][1] = relativePos[1] + origin[1];
        corners[i][2] = relativePos[2] + origin[2];
    }

    vec2_t screenCorners[8];
    for (int i = 0; i < 8; ++i)
    {
        if (!Math::WorldToScreen(corners[i], screenCorners[i]))
        {
            return false;
        }
    }

    bbox.bb[SIDE_LEFT] = FLT_MAX;
    bbox.bb[SIDE_TOP] = FLT_MAX;
    bbox.bb[SIDE_RIGHT] = -FLT_MAX;
    bbox.bb[SIDE_BOTTOM] = -FLT_MAX;

    for (int i = 0; i < 8; ++i)
    {
        bbox.bb[SIDE_LEFT] = std::min(bbox.bb[SIDE_LEFT], screenCorners[i][0]);
        bbox.bb[SIDE_TOP] = std::min(bbox.bb[SIDE_TOP], screenCorners[i][1]);
        bbox.bb[SIDE_RIGHT] = std::max(bbox.bb[SIDE_RIGHT], screenCorners[i][0]);
        bbox.bb[SIDE_BOTTOM] = std::max(bbox.bb[SIDE_BOTTOM], screenCorners[i][1]);
    }

    bbox.centerX = bbox.bb[SIDE_LEFT] + ((bbox.bb[SIDE_RIGHT] - bbox.bb[SIDE_LEFT]) / 2.f);
    bbox.centerY = bbox.bb[SIDE_TOP] + ((bbox.bb[SIDE_BOTTOM] - bbox.bb[SIDE_TOP]) / 2.f);

    memcpy(&bbox.points, screenCorners, sizeof(screenCorners));

    return true;
}

void DrawBoundingBox3D(CBox &bbox, const Color &color)
{
    // Draw bottom face
    g_renderer->AddLine(g_drawList, Vec2(bbox.points[0].x, bbox.points[0].y), Vec2(bbox.points[1].x, bbox.points[1].y),
                        color, 2.f);
    g_renderer->AddLine(g_drawList, Vec2(bbox.points[1].x, bbox.points[1].y), Vec2(bbox.points[3].x, bbox.points[3].y),
                        color, 2.f);
    g_renderer->AddLine(g_drawList, Vec2(bbox.points[3].x, bbox.points[3].y), Vec2(bbox.points[2].x, bbox.points[2].y),
                        color, 2.f);
    g_renderer->AddLine(g_drawList, Vec2(bbox.points[2].x, bbox.points[2].y), Vec2(bbox.points[0].x, bbox.points[0].y),
                        color, 2.f);

    // Draw top face
    g_renderer->AddLine(g_drawList, Vec2(bbox.points[4].x, bbox.points[4].y), Vec2(bbox.points[5].x, bbox.points[5].y),
                        color, 2.f);
    g_renderer->AddLine(g_drawList, Vec2(bbox.points[5].x, bbox.points[5].y), Vec2(bbox.points[7].x, bbox.points[7].y),
                        color, 2.f);
    g_renderer->AddLine(g_drawList, Vec2(bbox.points[7].x, bbox.points[7].y), Vec2(bbox.points[6].x, bbox.points[6].y),
                        color, 2.f);
    g_renderer->AddLine(g_drawList, Vec2(bbox.points[6].x, bbox.points[6].y), Vec2(bbox.points[4].x, bbox.points[4].y),
                        color, 2.f);

    // Draw vertical lines
    g_renderer->AddLine(g_drawList, Vec2(bbox.points[0].x, bbox.points[0].y), Vec2(bbox.points[4].x, bbox.points[4].y),
                        color, 2.f);
    g_renderer->AddLine(g_drawList, Vec2(bbox.points[1].x, bbox.points[1].y), Vec2(bbox.points[5].x, bbox.points[5].y),
                        color, 2.f);
    g_renderer->AddLine(g_drawList, Vec2(bbox.points[2].x, bbox.points[2].y), Vec2(bbox.points[6].x, bbox.points[6].y),
                        color, 2.f);
    g_renderer->AddLine(g_drawList, Vec2(bbox.points[3].x, bbox.points[3].y), Vec2(bbox.points[7].x, bbox.points[7].y),
                        color, 2.f);

    // Draw primary lines without thickness (optional)
    g_renderer->AddLine(g_drawList, Vec2(bbox.points[0].x, bbox.points[0].y), Vec2(bbox.points[1].x, bbox.points[1].y),
                        color);
    g_renderer->AddLine(g_drawList, Vec2(bbox.points[1].x, bbox.points[1].y), Vec2(bbox.points[3].x, bbox.points[3].y),
                        color);
    g_renderer->AddLine(g_drawList, Vec2(bbox.points[3].x, bbox.points[3].y), Vec2(bbox.points[2].x, bbox.points[2].y),
                        color);
    g_renderer->AddLine(g_drawList, Vec2(bbox.points[2].x, bbox.points[2].y), Vec2(bbox.points[0].x, bbox.points[0].y),
                        color);

    g_renderer->AddLine(g_drawList, Vec2(bbox.points[4].x, bbox.points[4].y), Vec2(bbox.points[5].x, bbox.points[5].y),
                        color);
    g_renderer->AddLine(g_drawList, Vec2(bbox.points[5].x, bbox.points[5].y), Vec2(bbox.points[7].x, bbox.points[7].y),
                        color);
    g_renderer->AddLine(g_drawList, Vec2(bbox.points[7].x, bbox.points[7].y), Vec2(bbox.points[6].x, bbox.points[6].y),
                        color);
    g_renderer->AddLine(g_drawList, Vec2(bbox.points[6].x, bbox.points[6].y), Vec2(bbox.points[4].x, bbox.points[4].y),
                        color);

    g_renderer->AddLine(g_drawList, Vec2(bbox.points[0].x, bbox.points[0].y), Vec2(bbox.points[4].x, bbox.points[4].y),
                        color);
    g_renderer->AddLine(g_drawList, Vec2(bbox.points[1].x, bbox.points[1].y), Vec2(bbox.points[5].x, bbox.points[5].y),
                        color);
    g_renderer->AddLine(g_drawList, Vec2(bbox.points[2].x, bbox.points[2].y), Vec2(bbox.points[6].x, bbox.points[6].y),
                        color);
    g_renderer->AddLine(g_drawList, Vec2(bbox.points[3].x, bbox.points[3].y), Vec2(bbox.points[7].x, bbox.points[7].y),
                        color);
}

void RectangleEdge(float x, float y, float w, float h, float thickness, const Color &col)
{
    float lineW = (w / 5);
    float lineH = (h / 6);
    float lineT = thickness;

    g_renderer->AddLine(g_drawList, Vec2(x - lineT, y - lineT), Vec2(x + lineW, y - lineT), D3DCOLOR_XRGB(0, 0, 0));
    g_renderer->AddLine(g_drawList, Vec2(x - lineT, y - lineT), Vec2(x - lineT, y + lineH), D3DCOLOR_XRGB(0, 0, 0));
    g_renderer->AddLine(g_drawList, Vec2(x - lineT, y + h - lineH), Vec2(x - lineT, y + h + lineT),
                        D3DCOLOR_XRGB(0, 0, 0));
    g_renderer->AddLine(g_drawList, Vec2(x - lineT, y + h + lineT), Vec2(x + lineW, y + h + lineT),
                        D3DCOLOR_XRGB(0, 0, 0));
    g_renderer->AddLine(g_drawList, Vec2(x + w - lineW, y - lineT), Vec2(x + w + lineT, y - lineT),
                        D3DCOLOR_XRGB(0, 0, 0));
    g_renderer->AddLine(g_drawList, Vec2(x + w + lineT, y - lineT), Vec2(x + w + lineT, y + lineH),
                        D3DCOLOR_XRGB(0, 0, 0));
    g_renderer->AddLine(g_drawList, Vec2(x + w + lineT, y + h - lineH), Vec2(x + w + lineT, y + h + lineT),
                        D3DCOLOR_XRGB(0, 0, 0));
    g_renderer->AddLine(g_drawList, Vec2(x + w - lineW, y + h + lineT), Vec2(x + w + lineT, y + h + lineT),
                        D3DCOLOR_XRGB(0, 0, 0));

    g_renderer->AddLine(g_drawList, Vec2(x, y), Vec2(x, y + lineH), col);
    g_renderer->AddLine(g_drawList, Vec2(x, y), Vec2(x + lineW, y), col);
    g_renderer->AddLine(g_drawList, Vec2(x + w - lineW, y), Vec2(x + w, y), col);
    g_renderer->AddLine(g_drawList, Vec2(x + w, y), Vec2(x + w, y + lineH), col);
    g_renderer->AddLine(g_drawList, Vec2(x, y + h - lineH), Vec2(x, y + h), col);
    g_renderer->AddLine(g_drawList, Vec2(x, y + h), Vec2(x + lineW, y + h), col);
    g_renderer->AddLine(g_drawList, Vec2(x + w - lineW, y + h), Vec2(x + w, y + h), col);
    g_renderer->AddLine(g_drawList, Vec2(x + w, y + h - lineH), Vec2(x + w, y + h), col);
}

void DrawBoneEsp(centity_s *Ent, const Color& col)
{
    auto DrawBone = [&](centity_s *Ent, unsigned int BoneName1, unsigned int BoneName2) {
        vec3_t bonePos1{};
        Engine::GetTagPos(Ent, BoneName1, bonePos1);

        if (!Math::WorldToScreen(bonePos1, bonePos1))
        {
            return;
        }

        vec3_t bonePos2{};
        Engine::GetTagPos(Ent, BoneName2, bonePos2);

        if (!Math::WorldToScreen(bonePos2, bonePos2))
        {
            return;
        }

        g_renderer->AddLine(g_drawList, Vec2(bonePos1[0], bonePos1[1]), Vec2(bonePos2[0], bonePos2[1]), col);
    };

    for (int i = 0; i < 4; i++)
    {
        DrawBone(Ent, Bone_Middle[i], Bone_Middle[i + 1]);
        DrawBone(Ent, Bone_Right_Arm[i], Bone_Right_Arm[i + 1]);
        DrawBone(Ent, Bone_Left_Arm[i], Bone_Left_Arm[i + 1]);
        DrawBone(Ent, Bone_Right_Leg[i], Bone_Right_Leg[i + 1]);
        DrawBone(Ent, Bone_Left_Leg[i], Bone_Left_Leg[i + 1]);
    }
}

void Initialize()
{
    static bool once = true;
    if (once)
    {
        once = false;

        Bone_Middle[0] = Engine::GScr_AllocString("j_helmet");
        Bone_Middle[1] = Engine::GScr_AllocString("neck");
        Bone_Middle[2] = Engine::GScr_AllocString("j_spineupper");
        Bone_Middle[3] = Engine::GScr_AllocString("j_spinelower");
        Bone_Middle[4] = Engine::GScr_AllocString("pelvis");

        Bone_Right_Arm[0] = Engine::GScr_AllocString("neck");
        Bone_Right_Arm[1] = Engine::GScr_AllocString("j_clavicle_ri");
        Bone_Right_Arm[2] = Engine::GScr_AllocString("j_shoulder_ri");
        Bone_Right_Arm[3] = Engine::GScr_AllocString("j_elbow_ri");
        Bone_Right_Arm[4] = Engine::GScr_AllocString("j_wrist_ri");

        Bone_Left_Arm[0] = Engine::GScr_AllocString("neck");
        Bone_Left_Arm[1] = Engine::GScr_AllocString("j_clavicle_le");
        Bone_Left_Arm[2] = Engine::GScr_AllocString("j_shoulder_le");
        Bone_Left_Arm[3] = Engine::GScr_AllocString("j_elbow_le");
        Bone_Left_Arm[4] = Engine::GScr_AllocString("j_wrist_le");

        Bone_Right_Leg[0] = Engine::GScr_AllocString("pelvis");
        Bone_Right_Leg[1] = Engine::GScr_AllocString("j_knee_ri");
        Bone_Right_Leg[2] = Engine::GScr_AllocString("j_ankle_ri");
        Bone_Right_Leg[3] = Engine::GScr_AllocString("j_ankle_ri");
        Bone_Right_Leg[4] = Engine::GScr_AllocString("j_ankle_ri");

        Bone_Left_Leg[0] = Engine::GScr_AllocString("pelvis");
        Bone_Left_Leg[1] = Engine::GScr_AllocString("j_knee_le");
        Bone_Left_Leg[2] = Engine::GScr_AllocString("j_ankle_le");
        Bone_Left_Leg[3] = Engine::GScr_AllocString("j_ankle_le");
        Bone_Left_Leg[4] = Engine::GScr_AllocString("j_ankle_le");
    }
}

void ESP::Run()
{
    Initialize();

    if (!Options::Visual::Enabled)
    {
        return;
    }

    clientInfo_t *localClient = GetLocalClient();
    if (!localClient)
    {
        return;
    }

    for (int i = 0; i < 8; ++i)
    {
        auto *dogEntity = (Dogs_s *)(DOGS + (i * DOGS_SIZE));

        if (!dogEntity || !dogEntity->entityNum)
        {
            continue;
        }

        centity_s *entity = GetEntityByIndex(dogEntity->entityNum);

        vec2_t screenPos = {};
        if (Math::WorldToScreen(entity->vOrigin, screenPos))
        {
            if (dogEntity->team != localClient->Team)
            {
                g_renderer->AddText(g_drawList, g_fontESP, "Dog", Vec2(screenPos[0], screenPos[1]),
                                    D3DCOLOR_XRGB(240, 240, 4), TEXT_FLAG_DROPSHADOW);
            }
        }
    }

    for (int i = 0; i < ENTITYNUM_WORLD; ++i)
    {
        clientInfo_t *client = GetClientByIndex(i);
        centity_s *entity = GetEntityByIndex(i);

        if (!entity || !entity->Alive || !entity->Valid)
        {
            continue;
        }

        switch (entity->type)
        {
        default:
            break;

        case TYPE_TURRET: {
            vec2_t screenPos = {};
            if (Math::WorldToScreen(entity->vOrigin, screenPos))
            {
                g_renderer->AddText(g_drawList, g_fontESP, "Sentry", Vec2(screenPos[0], screenPos[1]),
                                    D3DCOLOR_XRGB(133, 8, 33), TEXT_FLAG_CENTERED | TEXT_FLAG_DROPSHADOW);
            }

            break;
        }

        case TYPE_PLANE:
        case TYPE_HELICOPTER: {

            vec2_t screenPos = {};
            if (Math::WorldToScreen(entity->vOrigin, screenPos))
            {
                g_renderer->AddText(g_drawList, g_fontESP, "Killstreak", Vec2(screenPos[0], screenPos[1]),
                                    D3DCOLOR_XRGB(143, 199, 14), TEXT_FLAG_CENTERED | TEXT_FLAG_DROPSHADOW);
            }

            break;
        }

        case TYPE_EXPLOSIVE: {

            vec2_t screenPos = {};
            if (Math::WorldToScreen(entity->vOrigin, screenPos))
            {
                WeaponVariantDef *wd = Engine::BG_GetWeaponVariantDef(entity->Weapon);
                if (wd)
                {
                    g_renderer->AddText(g_drawList, g_fontESP, Engine::GetWeaponNameClean(wd->szDisplayName),
                                        Vec2(screenPos[0], screenPos[1]), D3DCOLOR_XRGB(184, 130, 4),
                                        TEXT_FLAG_CENTERED | TEXT_FLAG_DROPSHADOW);
                }
                else
                {
                    g_renderer->AddText(g_drawList, g_fontESP, "Explosive", Vec2(screenPos[0], screenPos[1]),
                                        D3DCOLOR_XRGB(184, 130, 4), TEXT_FLAG_CENTERED | TEXT_FLAG_DROPSHADOW);
                }
            }
            break;
        }

        case TYPE_HUMAN: {

            if (!client || client->clientNum == localClient->clientNum)
            {
                continue;
            }

            bool sameTeam = (client->Team == localClient->Team && Engine::IsTeamBasedGametype(cgs->gametype));
            Color color = sameTeam ? Color(0, 240, 40) : Color(240, 10, 40);

            if (Options::Visual::ESPDrawEnemy && !Options::Visual::ESPDrawFriend)
            {
                if (sameTeam)
                {
                    continue;
                }
            }
            else if (!Options::Visual::ESPDrawEnemy && Options::Visual::ESPDrawFriend)
            {
                if (!sameTeam)
                {
                    continue;
                }
            }
            else if (!Options::Visual::ESPDrawEnemy && !Options::Visual::ESPDrawFriend)
            {
                continue;
            }

            if (Engine::IsVisible(entity) && !sameTeam)
            {
                color = Color(230, 230, 10);
            }

            CBox bbox = {};
            if (!GetEntityBox(entity, bbox))
            {
                continue;
            }

            const Vec2 vecBoxMin = {bbox.bb[SIDE_LEFT], bbox.bb[SIDE_TOP]};
            const Vec2 vecBoxMax = {bbox.bb[SIDE_RIGHT], bbox.bb[SIDE_BOTTOM]};
            float bw = vecBoxMax.x - vecBoxMin.x;
            float bh = vecBoxMax.y - vecBoxMin.y;

            switch (Options::Visual::ESPBoxes)
            {
            default:
                break;

            case 1:
                g_renderer->AddRect(g_drawList, vecBoxMin, vecBoxMax, color);
                break;
            case 2:
                RectangleEdge(vecBoxMin.x, vecBoxMin.y, bw, bh, 1.f, color);
                break;
            case 3:
                DrawBoundingBox3D(bbox, color);
                break;
            }

            if (Options::Visual::ESPNames)
            {
                Vec2 size = g_renderer->CalculateTextExtent(g_fontESP, client->szName);

                g_renderer->AddText(g_drawList, g_fontESP, client->szName,
                                    Vec2(bbox.centerX, bbox.bb[SIDE_TOP] - 2.f - size.y), color,
                                    TEXT_FLAG_CENTERED_X | TEXT_FLAG_DROPSHADOW);
            }

            if (Options::Visual::ESPLines)
            {
                g_renderer->AddLine(g_drawList, Vec2(float(refdef->width / 2), float(refdef->height)),
                                    Vec2(bbox.centerX, bbox.bb[SIDE_BOTTOM]), color);
            }

            if (Options::Visual::ESPWeapon)
            {
                auto wd = Engine::BG_GetWeaponVariantDef(entity->Weapon);
                if (wd)
                {
                    g_renderer->AddText(g_drawList, g_fontESP, Engine::GetWeaponNameClean(wd->szDisplayName),
                                        Vec2(bbox.centerX, bbox.bb[SIDE_BOTTOM] + 2.f), color,
                                        TEXT_FLAG_CENTERED_X | TEXT_FLAG_DROPSHADOW);
                }
            }

            if (Options::Visual::ESPBones)
            {
                DrawBoneEsp(entity, color);
            }

            if (Options::Visual::ESPBarrel)
            {
                vec3_t vecHead{};
                Engine::GetTagPos(entity, Engine::Tags::Head, vecHead);

                vec3_t vecViewPoint{};
                Math::PlayerViewPoint(entity->vAngles, vecHead, vecViewPoint);

                vec2_t outPos[2]{};

                if (Math::WorldToScreen(vecHead, outPos[0]) && Math::WorldToScreen(vecViewPoint, outPos[1]))
                {
                    g_renderer->AddRectFilled(g_drawList, Vec4(outPos[0][0] - 2, outPos[0][1] - 2, 5, 5),
                                              D3DCOLOR_ARGB(202, 48, 48, 200));

                    g_renderer->AddLine(g_drawList, Vec2(outPos[0][0], outPos[0][1]), Vec2(outPos[1][0], outPos[1][1]),
                                        D3DCOLOR_ARGB(255, 255, 255, 200));
                }
            }

            break;
        }
        }
    }
}
} // namespace ESP
} // namespace Cheats