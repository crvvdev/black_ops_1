#pragma once

namespace Options
{
namespace Aimbot
{
extern bool Enabled;
extern bool AutoAim;
extern bool AutoFire;
extern bool AutoWall;
extern bool Prediction;
extern bool UseAimKey;
extern bool SilentAim;
extern bool VisibleCheck;
extern bool AimOnFriendly;
extern bool DrawLocked;
extern int Style;
extern int Bone;
extern int Key;
extern float FOV;
} // namespace Aimbot

namespace Visual
{
extern bool Enabled;
extern bool ESPLines;
extern bool ESPNames;
extern bool ESPBones;
extern bool ESPBarrel;
extern bool ESPWeapon;
extern bool ESPDrawFriend;
extern bool ESPDrawEnemy;
extern int ESPBoxes;
} // namespace Visual

namespace Misc
{
extern bool DrawFPS;
extern bool NoSpread;
extern bool NoRecoil;
extern bool UnlockDvar;
extern bool UsePanic;
extern int PanicKey;
extern int CHair;
extern float CHairSize;
} // namespace Misc
} // namespace Options