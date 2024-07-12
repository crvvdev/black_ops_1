#pragma once

namespace Cheats
{
namespace Aimbot
{
static bool g_isAimLocked = false;
static float g_deltaX = 0.f, g_deltaY = 0.f;

void Run();
void AutoFire(usercmd_t *cmd);
void SilentAim(usercmd_t *cmd);
void Render();
} // namespace Aimbot
} // namespace Cheats