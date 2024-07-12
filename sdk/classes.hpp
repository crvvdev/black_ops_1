#pragma once

#include "pointers.hpp"

#define BOX_RADIUS 16.0f

#define FLAGS_STANDING 0x02           // standing, still
#define FLAGS_STANDING_FORWARD 0x400  // standing, moving forwards
#define FLAGS_STANDING_BACKWARD 0x800 // standing, moving backwards

#define FLAGS_CROUCH 0x04            // crouch, still
#define FLAGS_CROUCH_FORWARD 0x1000  // crouched, moving forwards
#define FLAGS_CROUCH_BACKWARD 0x2000 // crouched, moving backwards

#define FLAGS_PRONE 0x08           // prone, still
#define FLAGS_PRONE_FORWARD 0x100  // prone, moving forwards
#define FLAGS_PRONE_BACKWARD 0x200 // prone, moving backwards

#define FLAGS_CLIMBING 0x40000   // climbing a ladder
#define FLAGS_SPRINTING 0x100000 // sprinting
#define FLAGS_SCALING 0x800000   // the player is using the object to hop over/onto it

#define CONTENTS_SOLID 0x0000001
#define CONTENTS_UNKNOWN1 0x000B000
#define CONTENTS_UNKNOWN2 0x0800000
#define CONTENTS_BODY 0x2000000
#define VISIBLE_TRACE (CONTENTS_SOLID | CONTENTS_UNKNOWN1 | CONTENTS_UNKNOWN2)

#define GENTITYNUM_BITS 10
#define MAX_GENTITIES (1 << GENTITYNUM_BITS)
#define ENTITYNUM_WORLD (MAX_GENTITIES - 2)

typedef float vec_t;
typedef float vec2_t[2];
typedef float vec3_t[3];
typedef float vec4_t[4];

enum EntType
{
    TYPE_SMOKE = 0,
    TYPE_HUMAN = 1,
    TYPE_DEAD = 2,
    TYPE_WEAPON = 3,
    TYPE_EXPLOSIVE = 4,
    TYPE_VEHICLE = 6,
    TYPE_CLAYMORELASER = 8,
    TYPE_TURRET = 11,
    TYPE_HELICOPTER = 12,
    TYPE_PLANE = 13,
    TYPE_ZOMBIE = 20,
    TYPE_OLDEXPLOSIVE = 78,
};

enum team_t
{
    TEAM_FFA,
    TEAM_ALLIES,
    TEAM_ENEMIES,
    TEAM_SPECTATOR,
    TEAM_TEAMS
};

enum TraceHitType
{
    TRACE_HITTYPE_NONE = 0,
    TRACE_HITTYPE_ENTITY = 1,
    TRACE_HITTYPE_DYNENT_MODEL = 2,
    TRACE_HITTYPE_DYNENT_BRUSH = 3,
    TRACE_HITTYPE_GLASS = 4,
};

struct trace_t
{
    vec3_t normal;                      // 0x0000
    char unk1[0x04];                    // 0x000C
    float fraction;                     // 0x0010
    int sflags;                         // 0x0014
    int cflags;                         // 0x0018
    enum TraceHitType hitType;          // 0x001c
    unsigned short hitId;               // 0x0020
    unsigned short modelIndex;          // 0x0022
    unsigned short partName;            // 0x0024
    unsigned short boneIndex;           // 0x0026
    unsigned short partGroup;           // 0x0028
    bool allsolid;                      // 0x002a
    bool startsolid;                    // 0x002b
    bool walkable;                      // 0x002c
    char Padding_455[3];                // 0x002d
    struct cStaticModel_s *staticModel; // 0x0030
    int hitPartition;                   // 0x0034
};

struct BulletFireParams
{
    int weaponEntIndex;     // 0x00
    int ignoreEntIndex;     // 0x04
    float damageMultiplier; // 0x08
    int methodOfDeath;      // 0x0C
    vec3_t origStart;       // 0x10
    vec3_t start;           // 0x1C
    vec3_t end;             // 0x28
    vec3_t dir;             // 0x34

}; // 0x40

struct BulletTraceResults
{
    trace_t trace;      // 0x00
    int *hitEnt;        // 0x38, gentity_s*
    vec3_t hitPos;      // 0x3C
    int ignoreHitEnt;   // 0x48
    int hitSurfaceType; // 0x4C

}; // 0x50

struct refdef_s
{
    int width;          // 0x0
    int height;         // 0x4
    vec2_t fov;         // 0x8
    float totalfov;     // 0x10
    char pad1[4];       // 0x14
    vec3_t ViewOrigin;  // 0x18
    char pad2[8];       // 0x1C
    vec3_t ViewAxis[3]; // 0x24

}; // 0x30; rawr

struct centity_s
{
    char pad1[8];        // 0x0
    DWORD SpectatorType; // 0x8
    char pad2[36];       // 0xC
    vec3_t vOrigin;      // 0x30
    vec3_t vAngles;      // 0x3C
    char pad3[320];      // 0x48
    vec3_t vNewOrigin;   // 0x188
    char pad4[24];       // 0x194
    vec3_t vNewAngles;   // 0x1AC
    char pad5[48];       // 0x1B8
    int clientNum;       // 0x1E8
    int Pose;            // 0x1EC
    char pad6[16];       // 0x1F0
    vec3_t vOldOrigin;   // 0x200
    char pad7[24];       // 0x20C
    vec3_t vOldAngles;   // 0x224
    char pad8[20];       // 0x230
    int Flags;           // 0x244
    int WeaponID;        // 0x248
    char pad9[90];       // 0x24C
    __int16 type;        // 0x2A6
    char pad10[10];      // 0x2A8
    __int16 Weapon;      // 0x2B2
    char pad11[112];     // 0x2B4
    bool Alive;          // 0x324
    bool Valid;          // 0x325
    char pad12[2];       // 0x326
};                       // 0x328; sph4ck

struct clientInfo_t
{
    BYTE Padding0[0x10];    // 0x0000
    DWORD Valid;            // 0x0010
    DWORD Padding1;         // 0x0014 Seems to always be 1
    DWORD clientNum;        // 0x0018
    char szName[32];        // 0x001C
    int Team;               // 0x003C
    int TeamBackup;         // 0x0040
    BYTE Padding2[0x2C];    // 0x0044
    DWORD CurrentFPS;       // 0x0070
    DWORD Alive;            // 0x0074 ||Dead: 0||Alive: 1||
    BYTE Padding3[0x10];    // 0x0078
    DWORD DeathCount;       // 0x0088
    BYTE Padding4[0x8];     // 0x008C
    DWORD DeathCountBackup; // 0x0094
    BYTE Padding5[0x18];    // 0x0098
    char ModelOne[32];      // 0x00B0
    BYTE Padding6[0x20];    // 0x00D0
    char ModelTwo[32];      // 0x00F0
    BYTE Padding7[0x20];    // 0x0110
    char ModelThree[32];    // 0x0130
    BYTE Padding8[0x160];   // 0x0150
    char ModelFour[32];     // 0x02B0
    BYTE Padding9[0x188];   // 0x02D0
    float Prediction;       // 0x0458
    BYTE Padding10[0x4];    // 0x045C
    float ViewAngleX;       // 0x0460
    float ViewAngleY;       // 0x0464
    BYTE Padding11[0x78];   // 0x0468
    DWORD MoveState;        // 0x04E0
    BYTE Padding12[0x30];   // 0x04E4
    DWORD CrouchState;      // 0x0514 ||Standing: 0||Crouching: 1||Proning: 2||
    DWORD WeaponUnknown;    // 0x0518 Something to do with weapon (switches 1/0)
    DWORD Shooting;         // 0x051C ||Shooting: 1||Not Shooting: 0||
    DWORD Zooming;          // 0x0520 ||Zooming: 1||No Zooming: 0||
    BYTE Padding13[0xAC];   // 0x0524
};                          // 0x5D0; Forza

typedef struct
{
    int number;       // 0x0
    int eType;        // 0x4
    int eFlags;       // 0x8
    char pad1[176];   // 0xC
    __int16 mod;      // 0xBC
    char pad2[6];     // 0xBE
    __int16 victim;   // 0xC4
    __int16 attacker; // 0xC6

} entityState_s; // rawr/king-orgy (for the CoD4 version)

struct playerState_s
{
    int commandTime;                    // 0x0000
    int pm_type;                        // 0x0004
    int bobCycle;                       // 0x0008
    int pm_flags;                       // 0x000C
    char pad00[0x18];                   // 0x0010
    float origin[3];                    // 0x0028
    float velocity[3];                  // 0x0034
    char pad01[0x44];                   // 0x0040
    float delta_angles[3];              // 0x0084
    char pad02[0xA0];                   // 0x0090
    char clientNum;                     // 0x0130
    char pad03[0x13];                   // 0x0131
    unsigned short weapon;              // 0x0144
    unsigned short lastStandPrevWeapon; // 0x0146
    char pad04[0x20];                   // 0x0148
    float fWeaponPosFrac;               // 0x0168
    int adsDelayTime;                   // 0x016C
    int spreadOverride;                 // 0x0170
    int spreadOverrideState;            // 0x0174
    float weaponSpinLerp;               // 0x0178
    int viewmodelIndex;                 // 0x017C
    float viewangles[3];                // 0x0180
    char pad05[0x3D4];                  // 0x018C
    float aimSpreadScale;               // 0x0560
    char pad06[0x2184];                 // 0x0564
};

struct cgs_t
{
    char pad1[32];    // 0x0
    char gametype[4]; // 0x20
    char pad2[28];    // 0x24
    char server[16];  // 0x40
    char pad3[244];   // 0x50
    char mapname[32]; // 0x144
};

struct snapshot_s
{
    int snapFlags;               // 0x00000
    int ping;                    // 0x00004
    int serverTime;              // 0x00008
    int physicsTime;             // 0x0000C
    playerState_s ps;            // 0x00010
    int numEntities;             // 0x026F8
    int numClients;              // 0x026FC
    entityState_s entities[512]; // 0x02700
};

struct cg_s
{
    int clientNum;                       // 0x00000
    int localClientNum;                  // 0x00004
    char pad00[0x20];                    // 0x00008
    snapshot_s *snap;                    // 0x00028
    snapshot_s *nextSnap;                // 0x0002C
    char pad01[0x40658];                 // 0x00030
    int time;                            // 0x40688
    int oldTime;                         // 0x4068C
    int physicsTime;                     // 0x40690
    int mapRestart;                      // 0x40694
    int renderingThirdPerson;            // 0x40698
    playerState_s predictedPlayerState;  // 0x4069C
    char pad02[0x37C];                   // 0x42D84
    refdef_s refdef;                     // 0x43100
    char pad03[0x184BC];                 // 0x43164
    float refdefViewAngles[3];           // 0x5B620
    char pad04[0x3824];                  // 0x5B62C
    int weaponSelect;                    // 0x5EE50
    char pad05[0x140];                   // 0x5EE54
    float gunPitch;                      // 0x5EF94
    float gunYaw;                        // 0x5EF98
    char pad06[0x50];                    // 0x5EF9C
    float compassNorthYaw;               // 0x5EFEC
    float compassNorth[2];               // 0x5EFF0
    struct Material *compassMapMaterial; // 0x5EFF8
    float compassMapUpperLeft[2];        // 0x5EFFC
    float compassMapWorldSize[2];        // 0x5F004
    char pad07[0x40];                    // 0x5F00C
    float zoomSensitivity;               // 0x5F04C
    char pad08[0x88];                    // 0x5F050
    int inKillCam;                       // 0x5F0D8
    char pad09[0x15C];                   // 0x5F0DC
    clientInfo_t clients[0x12];          // 0x5F238
    char pad10[0xA038];                  // 0x65AD8
    float aimSpreadScale;                // 0x6FB10
    char pad11[0x1CB0];                  // 0x6FB14
    int globalScramblerActive;           // 0x717C4
};

struct Material
{
    char *name; // 0x0
};              // 0x4

class WeaponName_t
{
  public:
    char szName[32];   // 0x0000
    char pad_0020[64]; // 0x0020
};                     // Size: 0x0060

struct WeaponDef
{
    char pad0[9];             // 0x0
    int SurfacePenetrateType; // 0x9
    char pad1[787];           // 0xD
    Material *hudIcon;        // 0x320
    char pad2[553];           // 0x324
    bool LargePenetrate;      // 0x54D
    char pad3[378];           // 0x54E
    float spreadmultiplier;   // 0x6C8

}; // 0x324; rawr/king-orgy

struct WeaponVariantDef
{
    char *szInternalName; // 0x0
    DWORD iVariantCount;  // 0x4
    WeaponDef *weapDef;   // 0x8
    char *szDisplayName;  // 0xC
};                        // 0x10; rawr

struct usercmd_t
{
    int servertime;    // 0x0000
    int buttons[2];    // 0x0004
    int viewAngles[3]; // 0x000C
    char _0x0018[4];
    char N0653ACD0[2]; // 0x001C
    char forwardmove;  // 0x001E
    char sidemove;     // 0x001F
    char _0x0020[20];
};

struct clientActive_t
{
    usercmd_t cmds[128];
    int commandNumber;

    usercmd_t *GetUserCmd(int cmdNum)
    {
        int id = cmdNum & 0x7F;
        return &cmds[id];
    }
}; // 0x1A04; x22/rawr

enum MethodOfDeath
{
    M_KNIFE = 7,
    M_HEADSHOT = 9,
    M_CRUSHED = 10,
    M_FALL = 12,
    M_SUICIDE = 14,
    M_GRENADE = 16
}; // rawr

class Dogs_s
{
  public:
    __int32 entityNum; // 0000
    __int32 snapshot;  // 0004
    vec3_t origin;     // 0008
    __int32 team;      // 0014
    __int32 owner;     // 0018
};

struct refdefViewAngles_t
{
    vec3_t angles; // Backwards , YX instead of XY
};

class ViewMatrix
{
  public:
    char _0x0000[8];
    float Recoil[3]; // 0x0008
    char _0x000C[8];
    float Origin[3]; // 0x0014
    char _0x0018[100];
    float ViewAngleX; // 0x007C
    float ViewAngleY; // 0x0080
    float ViewAngleZ; // 0x0084
    char _0x0088[948];

}; // Size=0x043C

inline ViewMatrix *viewMatrix = (ViewMatrix *)VIEWMATRIX;
inline refdef_s *refdef = (refdef_s *)REFDEF;
inline refdefViewAngles_t *refdefViewAngles = (refdefViewAngles_t *)RDVIEWANGLES;
inline clientActive_t *clientActive = (clientActive_t *)USERCMDS;
inline cgs_t *cgs = (cgs_t *)CGS;
inline cg_s *cg = (cg_s *)CG;
inline playerState_s *ps = (playerState_s *)CGS_PLAYERSTATE;

__forceinline centity_s *const GetEntityByIndex(int i)
{
    return ((centity_s *)(CGENTITY + (ENTITY_SIZE * i)));
}

__forceinline clientInfo_t *const GetClientByIndex(int i)
{
    return ((clientInfo_t *)(CLIENTINFO + (CLIENTINFO_SIZE * i)));
}

__forceinline clientInfo_t *const GetLocalClient()
{
    return GetClientByIndex(ps->clientNum);
}

__forceinline centity_s *const GetLocalEntity()
{
    return GetEntityByIndex(ps->clientNum);
}