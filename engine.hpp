#pragma once

namespace Engine
{
using GScr_AllocString_ = unsigned int (*)(const char *tagName);
inline GScr_AllocString_ GScr_AllocString = (GScr_AllocString_)G_SRCALLOCSTR;

using BG_GetSpreadForWeapon_ = void (*)(void *, void *, float *, float *);
inline BG_GetSpreadForWeapon_ BG_GetSpreadForWeapon = (BG_GetSpreadForWeapon_)BG_GETSPREADFORWEAPON;

using BG_GetWeapondef_ = WeaponDef *(*)(short);
inline BG_GetWeapondef_ BG_GetWeaponDef = (BG_GetWeapondef_)BG_GETWEAPONDEF;

using RandomBulletDir_ = void (*)(int, float *, float *);
inline RandomBulletDir_ RandomBulletDir = (RandomBulletDir_)RANDOM_BULLET_DIR;

using CBuf_AddText_ = void (*)(int, const char *);
inline CBuf_AddText_ CBuf_AddText = (CBuf_AddText_)CBUF_ADDTEXT;

using CG_CanSeeFriendlyHead_ = bool (*)(int, centity_s *);
inline CG_CanSeeFriendlyHead_ CG_CanSeeFriendlyHead = (CG_CanSeeFriendlyHead_)CG_CANSEEFRIENDLYHEAD;

using BG_GetSurfacePenetrationDepth_ = float (*)(WeaponDef *, int);
inline BG_GetSurfacePenetrationDepth_ BG_GetSurfacePenetrationDepth =
    (BG_GetSurfacePenetrationDepth_)BG_GETSURFACEPENETRATIONDEPTH;

using BG_AdvanceTrace_ = bool (*)(BulletFireParams *, BulletTraceResults *, float);
inline BG_AdvanceTrace_ BG_AdvanceTrace = (BG_AdvanceTrace_)BG_ADVANCEDTRACE;

using Trace_GetEntityHitId_ = short (*)(trace_t *);
inline Trace_GetEntityHitId_ Trace_GetEntityHitId = (Trace_GetEntityHitId_)TRACE_GETENTITYHITID;

using CG_LocationalTrace_ = void (*)(trace_t *, vec3_t, vec3_t, int, int, bool, void *);
inline CG_LocationalTrace_ CG_LocationalTrace = (CG_LocationalTrace_)CG_LOCATIONALTRACE;

using BG_GetWeaponVariantDef_ = WeaponVariantDef *(*)(unsigned);
inline BG_GetWeaponVariantDef_ BG_GetWeaponVariantDef = (BG_GetWeaponVariantDef_)BG_GETWEAPONVARIANTDEF;

using CG_GetViewFov_ = float (*)();
inline CG_GetViewFov_ CG_GetViewFov = (CG_GetViewFov_)CG_GETVIEWFOV;

using Com_GetClientDObj_t = void *(__cdecl *)(unsigned int handle, int localClientNum);
inline Com_GetClientDObj_t Com_GetClientDObj = reinterpret_cast<Com_GetClientDObj_t>(COM_GETCLIENTDOBJ);

using CG_DObjGetWorldTagPos_t = int(__cdecl *)(centity_s *entity, void *obj, unsigned int tagName, vec3_t pos);
inline CG_DObjGetWorldTagPos_t CG_DObjGetWorldTagPos = reinterpret_cast<CG_DObjGetWorldTagPos_t>(CG_DOBJGETWORLDTAGPOS);

using CG_GetPlayerViewOrigin_t = bool(__cdecl *)(int localClientNum, playerState_s *ps, float *origin);
inline CG_GetPlayerViewOrigin_t CG_GetPlayerViewOrigin = reinterpret_cast<CG_GetPlayerViewOrigin_t>(0x679940);

namespace Tags
{
inline unsigned int Helmet = 0, Head = 0, Chest = 0, Stomach = 0;
}

inline void ExecuteCommand(const char *cmd)
{
    CBuf_AddText(0, cmd);
}

inline void Initialize()
{
    Tags::Helmet = GScr_AllocString("j_helmet");
    Tags::Head = GScr_AllocString("j_head");
    Tags::Chest = GScr_AllocString("j_spineupper");
    Tags::Stomach = GScr_AllocString("j_spinelower");
}

inline std::string GetWeaponNameClean(std::string szWeaponName)
{
    szWeaponName = szWeaponName.substr(szWeaponName.find_first_of('_') + 1);
    szWeaponName = std::regex_replace(szWeaponName, std::regex("_"), " ");
    return szWeaponName;
}

inline int GetTagPos(centity_s *entity, unsigned int tagName, vec3_t pos)
{
    void *obj = Com_GetClientDObj(entity->clientNum, ps->clientNum);
    if (obj)
    {
        return CG_DObjGetWorldTagPos(entity, obj, tagName, pos);
    }
    return 0;
}

inline bool BulletTrace(BulletFireParams *bfp, BulletTraceResults *trace, int localclientnum, centity_s *entity,
                        int unk1)
{
    void *addr = reinterpret_cast<void *>(BULLET_TRACE);
    int retval;

    __asm
    {
        pushad
        mov edi, bfp
        mov esi, trace
        push unk1
        push entity
        push localclientnum
        call addr
        add esp, 0xC
        mov retval, eax
        popad
    }

    return retval;
}

inline bool IsVisible(centity_s *entity)
{
    return CG_CanSeeFriendlyHead(ps->clientNum, entity);
}

inline bool IsVisible(vec3_t src, vec3_t dst, int skipNumber)
{
    trace_t tr{};
    CG_LocationalTrace(&tr, src, dst, skipNumber, 0x803003, 0, 0);

    return (tr.fraction > 0.97f);
}

inline bool IsTeamBasedGametype(const char *gameType)
{
    if (!strcmp(gameType, "dm") || !strcmp(gameType, "gun") || !strcmp(gameType, "HLND") || !strcmp(gameType, "oic") ||
        !strcmp(gameType, "shrp"))
    {
        return false;
    }
    return true;
}

__forceinline short GetCurrentWeapon()
{
    return ps->weapon;
}

__forceinline bool IsAkimbo()
{
    const auto weaponDefPtr = reinterpret_cast<uint8_t *>(BG_GetWeaponDef(GetCurrentWeapon()));
    if (weaponDefPtr)
    {
        return (*(unsigned int *)(weaponDefPtr + 0x594) > 0);
    }
    return false;
}

inline void RemoveSpread(usercmd_t *usercmd, int seed)
{
    float spreadVar = 0.f;
    float minSpread = 0.f, maxSpread = 0.f;
    float spreadX = 0.f, spreadY = 0.f;

    const auto weaponDefPtr = reinterpret_cast<uint8_t *>(BG_GetWeaponDef(GetCurrentWeapon()));
    if (weaponDefPtr)
    {
        BG_GetSpreadForWeapon(ps, weaponDefPtr, &minSpread, &maxSpread);
        RandomBulletDir(seed, &spreadX, &spreadY);

        const auto fAdsSpread = *(float *)(weaponDefPtr + 0x6C8);

        if (ps->fWeaponPosFrac > 0.9f)
        {
            spreadVar = ((maxSpread - fAdsSpread) * (ps->aimSpreadScale / 255.f)) + fAdsSpread;
        }
        else
        {
            spreadVar = (maxSpread - minSpread) * (ps->aimSpreadScale / 255.f) + minSpread;
        }

        spreadX *= spreadVar;
        spreadY *= spreadVar;

        usercmd->viewAngles[0] += ANGLE2SHORT(spreadY);
        usercmd->viewAngles[1] += ANGLE2SHORT(spreadX);
    }
}

inline void CG_BulletEndpos(int seed, const float spread, const vec3_t &start, vec3_t &end, vec3_t &dir,
                            const vec3_t &forwardDir, const vec3_t &rightDir, const vec3_t &upDir, const float maxRange)
{
    float x, y;
    RandomBulletDir(seed, &x, &y);

    float spreadAngle = tanf((spread * 0.0174532925f)) * maxRange;

    x *= spreadAngle;
    y *= spreadAngle;

    end[0] = start[0] + (forwardDir[0] * maxRange) + (rightDir[0] * x) + (upDir[0] * y);
    end[1] = start[1] + (forwardDir[1] * maxRange) + (rightDir[1] * x) + (upDir[1] * y);
    end[2] = start[2] + (forwardDir[2] * maxRange) + (rightDir[2] * x) + (upDir[2] * y);

    if (dir)
    {
        dir[0] = end[0] - start[0];
        dir[1] = end[1] - start[1];
        dir[2] = end[2] - start[2];
        Math::VectorNormalize(dir);
    }
}

inline void(__cdecl *angleVectors)(const float *angles, float *forward, float *right,
                                   float *up) = (void(__cdecl *)(const float *, float *, float *, float *))0x657D30;
inline void(__cdecl *vectoangles)(const float *vec, float *angles) = (void(__cdecl *)(const float *, float *))0x4D5230;

inline void RemoveSpread(usercmd_t *usercmd)
{
    const auto weaponDefPtr = reinterpret_cast<uint8_t *>(BG_GetWeaponDef(GetCurrentWeapon()));
    if (!weaponDefPtr)
    {
        return;
    }

    float minSpread, maxSpread, finalSpread, range;
    const float fAdsSpread = *(float *)(weaponDefPtr + 0x6C8);
    float cgSpread = ps->aimSpreadScale / 255.f;

    BG_GetSpreadForWeapon(ps, weaponDefPtr, &minSpread, &maxSpread);
    finalSpread = ps->fWeaponPosFrac == 1.0f ? fAdsSpread : minSpread;
    finalSpread = ((maxSpread - finalSpread) * cgSpread) + finalSpread;

    vec3_t viewOrigin{};
    if (!CG_GetPlayerViewOrigin(0, ps, viewOrigin))
    {
        return;
    }

    vec3_t viewAxis[3]{};
    // TODO: add check for thirdperson
    {
        vec3_t gunAngles = {cg->gunPitch, cg->gunYaw, 0.f};

        angleVectors(gunAngles, viewAxis[0], viewAxis[1], viewAxis[2]);
    }

    const int weapType = *(int *)(weaponDefPtr + 0x1C);
    const float fMinDamageRange = *(float *)(weaponDefPtr + 0x7AC);

    // check if weapon has spread (WEAPCLASS_SPREAD)
    range = weapType == 3 ? fMinDamageRange : 8192.0f;

    vec3_t spreadEnd{};
    vec3_t spreadDirection{};
    CG_BulletEndpos(ps->commandTime, finalSpread, viewOrigin, spreadEnd, spreadDirection, viewAxis[0], viewAxis[1],
                    viewAxis[2], range);

    vec3_t spreadFix{};
    vectoangles(spreadDirection, spreadFix);

    float gunPitch = cg->gunPitch;
    float gunYaw = cg->gunYaw;

    usercmd->viewAngles[0] -= ANGLE2SHORT(-(gunPitch - spreadFix[0]));
    usercmd->viewAngles[1] -= ANGLE2SHORT(-(gunYaw - spreadFix[1]));
}

inline float GetAutoWallDamageMultiplier(const vec3_t &start, const vec3_t &end)
{
    // get our weapon
    WeaponDef *weapDef = BG_GetWeaponDef(GetCurrentWeapon());
    uint8_t *weapDefPtr = reinterpret_cast<uint8_t *>(weapDef);
    uint8_t *localEntityPtr = reinterpret_cast<uint8_t *>(GetLocalEntity());

    // set up our BulletFireParams struct
    BulletFireParams bp{};
    bp.weaponEntIndex = 1022;
    bp.ignoreEntIndex = ps->clientNum;
    bp.damageMultiplier = 1.0f;
    bp.methodOfDeath = (*(BYTE *)(weapDefPtr + 1357) != 0) + 1; // bRifleBullet

    VectorCopy(start, bp.origStart);
    VectorCopy(start, bp.start);
    VectorCopy(end, bp.end);

    vec3_t dir{};
    VectorSubtract(end, start, dir);
    Math::VectorNormalize(dir);

    VectorCopy(dir, bp.dir);

    // call BulletTrace for the first time and get our BulletTraceResults struct
    BulletTraceResults bulletTraceResults = {};
    bool hit = BulletTrace(&bp, &bulletTraceResults, 0, GetLocalEntity(), 0);

    // if the result of the bullet trace was TRACE_HITTYPE_NONE, we return the full damage multiplier
    if (!hit)
    {
        return bp.damageMultiplier;
    }

    // return if the hit type was TRACE_HITTYPE_ENTITY
    if (Trace_GetEntityHitId(&bulletTraceResults.trace) != 1022)
    {
        return 0.f;
    }

    if (bulletTraceResults.trace.partGroup == 2)
    {
        return 0.f;
    }

    if (!*(DWORD *)(weapDefPtr + 36) // if (weaponDefPtr->penetrateType == PENETRATE_TYPE_NONE)
        || bulletTraceResults.trace.startsolid || *(BYTE *)(weapDefPtr + 1581)) // if ( weapDef->bBulletImpactExplode )
    {
        return 0.f;
    }

    // we can penetrate a maximum of 5 walls, we loop each wall
    int penetrationCount = 0;
    float perk_bulletPenetrationMultiplier = 2.0f;
    float penetrationDepth;
    BulletFireParams bulletFireParams_2;
    BulletTraceResults bulletTraceResults_2;
    vec3_t hitPosBackup;
    bool hit_2;
    bool v26;

    while (true)
    {
        // get the penetration depth of the hit surface, and apply the perk_bulletPenetrationMultiplier perk if we have
        // it need to test if perk detection works
        penetrationDepth = BG_GetSurfacePenetrationDepth(weapDef, bulletTraceResults.hitSurfaceType);

        // if ( attacker->type == TYPE_HUMAN && (attacker->client->ps.perks[0] & 0x20) != 0 )
        if (*(WORD *)(localEntityPtr + 678) == 1 && (*(DWORD *)(localEntityPtr + 389756) & 32))
        {
            penetrationDepth *= perk_bulletPenetrationMultiplier;
        }

        // we return if our penetration depth is zero or negative
        if (penetrationDepth <= 0.0f)
        {
            return 0.f;
        }

        // backup our latest hit position and advance the trace (return if we can't advance the trace)
        VectorCopy(bulletTraceResults.hitPos, hitPosBackup);

        if (!BG_AdvanceTrace(&bp, &bulletTraceResults, 0.13500000536441803f))
        {
            return 0.f;
        }

        // get the wall depth by hitting the face of the surface, then the back of the surface, and saving those hit
        // positions
        hit = BulletTrace(&bp, &bulletTraceResults, 0, GetLocalEntity(), bulletTraceResults.hitSurfaceType);

        memcpy(&bulletFireParams_2, &bp, sizeof(BulletFireParams));

        VectorScale(bp.dir, -1.0f,
                    bulletFireParams_2.dir); // inverse the direction when at the back of the wall
        VectorCopy(bp.end, bulletFireParams_2.start);
        Math::VectorMA(hitPosBackup, 0.009999999776482582f, bulletFireParams_2.dir, bulletFireParams_2.end);

        memcpy(&bulletTraceResults_2, &bulletTraceResults, sizeof(BulletTraceResults));

        VectorScale(bulletTraceResults_2.trace.normal, -1.0f, bulletTraceResults_2.trace.normal);

        if (hit)
        {
            BG_AdvanceTrace(&bulletFireParams_2, &bulletTraceResults_2, 0.009999999776482582f);
        }

        hit_2 = BulletTrace(&bulletFireParams_2, &bulletTraceResults_2, 0, GetLocalEntity(),
                            bulletTraceResults_2.hitSurfaceType);

        v26 = (hit_2 != 0) && bulletTraceResults_2.trace.allsolid || bulletTraceResults.trace.startsolid ||
              bulletTraceResults_2.trace.startsolid;

        if (hit_2 || v26)
        {
            break;
        }

        if (!hit)
        {
            return bp.damageMultiplier;
        }
        goto continueLoop;

    continueLoop:
        if (hit)
        {
            penetrationCount++;
            if (penetrationCount < 5) // max number of surfaces penetrated is 5 (from penetrationCount dvar)
                continue;
        }

        return 0.f;
    }

    // calculate the surface depth
    float surfaceDepth = 0.0f;
    if (v26)
    {
        surfaceDepth = Math::GetDistance3D(bulletFireParams_2.start, bulletFireParams_2.end);
    }
    else
    {
        surfaceDepth = Math::GetDistance3D(bulletTraceResults_2.hitPos, hitPosBackup);
    }

    // minimum surface depth is 1.0f
    if (surfaceDepth < 1.0f)
        surfaceDepth = 1.0f;

    // if we hit the back side of the wall, set the minimum penetration depth of both surfaces hit as our final
    // penetration depth
    if (hit_2)
    {
        float penetrationDepth_2 = BG_GetSurfacePenetrationDepth(weapDef, bulletTraceResults_2.hitSurfaceType);

        // if ( attacker->type == TYPE_HUMAN && (attacker->client->ps.perks[0] & 0x20) != 0 )
        if (*(WORD *)(localEntityPtr + 678) == 1 && (*(DWORD *)(localEntityPtr + 389756) & 32))
        {
            penetrationDepth_2 *= perk_bulletPenetrationMultiplier;
        }

        penetrationDepth = std::min(penetrationDepth, penetrationDepth_2);

        // we return if our penetration depth is zero or negative
        if (penetrationDepth <= 0.0f)
        {
            return 0.f;
        }
    }

    // if ( attacker->type == TYPE_HUMAN && (attacker->client->ps.perks[0] & 0x20) != 0 )
    if (*(WORD *)(localEntityPtr + 678) == 1)
    {
        if (*(DWORD *)(localEntityPtr + 389756) & 32)
        {
        LABEL_84:
            if (bp.damageMultiplier <= 0.0f)
            {
                return 0.f;
            }

            goto continueLoop;
        }
    }

    // decrease our damage multiplier (the fraction)
    bp.damageMultiplier -= surfaceDepth / penetrationDepth;
    goto LABEL_84;
}
} // namespace Engine