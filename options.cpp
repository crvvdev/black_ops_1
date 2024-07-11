#include "options.hpp"

namespace Options
{
namespace Aimbot
{
bool Enabled = false;
bool AutoAim = false;
bool AutoFire = false;
bool AutoWall = false;
bool Prediction = false;
bool SilentAim = false;
bool UseAimKey = true;
bool VisibleCheck = false;
bool AimOnFriendly = false;
bool DrawLocked = false;
int Key = 0x1;
int Style = 0;
int Bone = 0;
float FOV = 180.0f;
} // namespace Aimbot

namespace Visual
{
bool Enabled = false;
bool ESPLines = false;
bool ESPNames = false;
bool ESPBones = false;
bool ESPBarrel = false;
bool ESPWeapon = false;
bool ESPDrawFriend = false;
bool ESPDrawEnemy = true;
int ESPBoxes = 0;
} // namespace Visual

namespace Misc
{
bool NoSpread = false;
bool NoRecoil = false;
bool UnlockDvar = false;
bool DrawFPS = false;
bool UsePanic = false;
int PanicKey = 0;
int CHair = 0;
float CHairSize = 15.f;
} // namespace Misc
} // namespace Options