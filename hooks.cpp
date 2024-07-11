#include "includes.hpp"

namespace Hooks
{
HRESULT WINAPI hkReset(LPDIRECT3DDEVICE9 device, D3DPRESENT_PARAMETERS *presentParams)
{
    const HRESULT hr = oReset(device, presentParams);

    if (Globals::g_rendererInitialized)
    {
        g_renderer->OnLostDevice();

        if (SUCCEEDED(hr))
        {
            g_renderer->OnResetDevice();
        }
    }

    return hr;
}

HRESULT WINAPI hkEndScene(LPDIRECT3DDEVICE9 device)
{
    while (Globals::g_d3dDevice != device)
    {
        Globals::g_d3dDevice = device;
    }

    if (!Options::Misc::UsePanic)
    {
        if (!Menu::MenuCreated)
        {
            Menu::MenuCreated = true;

            D3DDEVICE_CREATION_PARAMETERS params{};
            device->GetCreationParameters(&params);

            Menu::SetupMenu(params.hFocusWindow);
        }

        // initialize renderer if needed
        if (!Globals::g_rendererInitialized)
        {
            Globals::g_rendererInitialized = true;

            g_renderer = std::make_shared<Renderer>(device, 4096 * 4);

            g_drawList = g_renderer->CreateRenderList();
            g_drawListAct = g_renderer->CreateRenderList();
            g_drawListRendering = g_renderer->CreateRenderList();

            g_fontESP = g_renderer->AddFont(L"Tahoma", 7, FONT_FLAG_CLEAR_TYPE);
            g_fontGUI = g_renderer->AddFont(L"Verdana", 9, FONT_FLAG_CLEAR_TYPE);
            g_fontGUIBold = g_renderer->AddFont(L"Verdana", 9, FONT_FLAG_CLEAR_TYPE | FONT_FLAG_BOLD);
        }

        g_renderer->BeginFrame();

        Menu::DoUIFrame();

        Cheats::Misc::Run();

        // tell renderer to render our custom draw list
        g_renderer->Render(GetRenderList());
        g_renderer->Render();

        g_renderer->EndFrame();
    }

    return oEndScene(device);
}

void WINAPIV hkR_EndFrame()
{
    static bool once = true;
    if (once)
    {
        once = false;
        Engine::Initialize();
    }

    if (Globals::g_rendererInitialized)
    {
        BeginDraw();

        Cheats::ESP::Run();
        Cheats::Aimbot::Run();
        Cheats::Aimbot::Render();

        EndDraw();
    }

    return oR_EndFrame();
}

void OnPlayerCommand(usercmd_t *oldCmd, usercmd_t *currentCmd)
{
    /*
        Aimbot
    */
    Cheats::Aimbot::SilentAim(oldCmd);
    Cheats::Aimbot::AutoFire(currentCmd);

    /*
        Remove Spread
    */
    if (Options::Misc::NoSpread)
    {
        Engine::RemoveSpread(oldCmd);
    }
}

void WINAPIV hkCL_WritePacket(int localClientNum)
{
    usercmd_t *oldCmd = clientActive->GetUserCmd(clientActive->commandNumber - 1);
    usercmd_t *currentCmd = clientActive->GetUserCmd(clientActive->commandNumber);

    oldCmd->servertime += 2;

    OnPlayerCommand(oldCmd, currentCmd);

    return oCL_WritePacket(localClientNum);
}

void WINAPIV hkCG_PredictPlayerState(int localClientNum)
{
    static int backupAngles[3]{};

    usercmd_t *oldCmd = clientActive->GetUserCmd(clientActive->commandNumber - 1);
    usercmd_t *currentCmd = clientActive->GetUserCmd(clientActive->commandNumber);
    usercmd_t *newCmd = clientActive->GetUserCmd(clientActive->commandNumber + 1);

    memcpy(newCmd, currentCmd, sizeof(*newCmd));
    ++clientActive->commandNumber;

    oldCmd->viewAngles[0] = backupAngles[0];
    oldCmd->viewAngles[1] = backupAngles[1];
    oldCmd->viewAngles[2] = backupAngles[2];

    backupAngles[0] = currentCmd->viewAngles[0];
    backupAngles[1] = currentCmd->viewAngles[1];
    backupAngles[2] = currentCmd->viewAngles[2];

    ++oldCmd->servertime;
    --currentCmd->servertime;

    OnPlayerCommand(oldCmd, currentCmd);

    return oCG_PredictPlayerState(localClientNum);
}
} // namespace Hooks