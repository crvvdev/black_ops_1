#include "includes.hpp"

namespace Cheats
{
namespace Misc
{
void DrawFPSCounter()
{
    if (!Options::Misc::DrawFPS)
    {
        return;
    }

    static float framesPerSecond = 0.0f;
    static int fps;
    static float lastTime = 0.0f;
    float currentTime = ::GetTickCount() * 0.001f;
    ++framesPerSecond;

    std::string fpsStr = "FPS: ";
    fpsStr += std::to_string(fps);

    g_renderer->AddText(g_fontGUI, fpsStr, Vec2(1, 1), D3DCOLOR_XRGB(255, 255, 255), TEXT_FLAG_DROPSHADOW);

    if (currentTime - lastTime > 1.0f)
    {
        lastTime = currentTime;
        fps = (int)framesPerSecond;
        framesPerSecond = 0;
    }
}

void Run()
{
    DrawFPSCounter();

    /*
        Remove Recoil
    */
    /*static bool recoilEnabled = false;

    if (Options::Misc::NoRecoil && !recoilEnabled)
    {
        recoilEnabled = true;
        Tools::Patch((void *)RECOIL_1, "\xC3\x90", 2);
        Tools::Patch((void *)RECOIL_2, "\x00", 1);
        Tools::Patch((void *)RECOIL_3, "\x75", 1);
        Tools::Patch((void *)RECOIL_4, "\x74", 1);
    }
    else if (!Options::Misc::NoRecoil && recoilEnabled)
    {
        recoilEnabled = false;
        Tools::Patch((void *)RECOIL_1, "\x83\xEC\x10", 3);
        Tools::Patch((void *)RECOIL_2, "\x06", 1);
        Tools::Patch((void *)RECOIL_3, "\x74", 1);
        Tools::Patch((void *)RECOIL_4, "\x75", 1);
    }*/

    if (Options::Misc::NoRecoil)
    {
        for (int i = 0; i < 3; i++)
        {
            viewMatrix->Recoil[i] = 0.f;
        }
    }

    /*
     *	Unlock Dvars
     */
    static bool dvarsUnlocked = false;

    if (Options::Misc::UnlockDvar && !dvarsUnlocked)
    {
        dvarsUnlocked = true;
        Tools::Patch((void *)UNLOCK_DVAR1, "\x90\x90", 2);
        Tools::Patch((void *)UNLOCK_DVAR2, "\x90\x90", 2);
        Tools::Patch((void *)UNLOCK_DVAR3, "\x90\x90\x90", 3);
        Tools::Patch((void *)UNLOCK_DVAR4, "\x90\x90", 2);
    }
    else if (!Options::Misc::UnlockDvar && dvarsUnlocked)
    {
        dvarsUnlocked = false;
        Tools::Patch((void *)UNLOCK_DVAR1, "\x74\x0A", 2);
        Tools::Patch((void *)UNLOCK_DVAR2, "\x74\x0A", 2);
        Tools::Patch((void *)UNLOCK_DVAR3, "\x83\xFF\x02", 3);
        Tools::Patch((void *)UNLOCK_DVAR4, "\x74\x03", 2);
    }
}
} // namespace Misc
} // namespace Cheats