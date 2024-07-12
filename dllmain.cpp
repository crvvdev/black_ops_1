#include "includes.hpp"
#include <MinHook.h>

using namespace Hooks;
bool g_minHookInitialized = false;

bool InitializeD3D(DWORD_PTR **d3dDeviceVTable)
{
    assert(d3dDeviceVTable);

    IDirect3D9 *d3d9 = Direct3DCreate9(D3D_SDK_VERSION);
    if (!d3d9)
    {
        return false;
    }

    D3DPRESENT_PARAMETERS d3dpp{};
    d3dpp.Windowed = TRUE;
    d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
    d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;

    IDirect3DDevice9 *d3dDevice = nullptr;
    HRESULT hr = d3d9->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, GetDesktopWindow(),
                                    D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &d3dDevice);
    if (FAILED(hr) || !d3dDevice)
    {
        d3d9->Release();
        return false;
    }

    *d3dDeviceVTable = *(DWORD_PTR **)(d3dDevice);

    return true;
}

void OnDetach()
{
    if (!g_minHookInitialized)
    {
        return;
    }

    MH_DisableHook(MH_ALL_HOOKS);
    MH_Uninitialize();
}

DWORD WINAPI StartRoutine(LPVOID)
{
    DWORD_PTR *d3dDeviceVTable = nullptr;

    if (!InitializeD3D(&d3dDeviceVTable))
    {
        return EXIT_FAILURE;
    }

    /*
        Aim 360 Patch
    */
    Tools::Patch((void *)AIM_360, "\xEB", 1);

    MH_Initialize();

    MH_CreateHook(reinterpret_cast<PVOID>(d3dDeviceVTable[16]), &hkReset, reinterpret_cast<LPVOID *>(&oReset));
    MH_CreateHook(reinterpret_cast<PVOID>(d3dDeviceVTable[42]), &hkEndScene, reinterpret_cast<LPVOID *>(&oEndScene));

    // R_EndFrame is the procedure which the game renders the current frame/scene
    MH_CreateHook(reinterpret_cast<PVOID>(R_ENDFRAME), &hkR_EndFrame, reinterpret_cast<LPVOID *>(&oR_EndFrame));

    // CL_WritePacket is one of the procedures which the game process player input
    MH_CreateHook(reinterpret_cast<PVOID>(CL_WRITEPACKET), &hkCL_WritePacket,
                  reinterpret_cast<LPVOID *>(&oCL_WritePacket));

    // CG_PredictPlayerState is one of the procedures which the game process player input as well
    /* MH_CreateHook(reinterpret_cast<PVOID>(CG_PREDICTPLAYERSTATE), &hkCG_PredictPlayerState,
                   reinterpret_cast<LPVOID *>(&oCG_PredictPlayerState));*/

    MH_EnableHook(MH_ALL_HOOKS);

    g_minHookInitialized = true;

    return EXIT_SUCCESS;
}

BOOL WINAPI DllMain(HMODULE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
{
    UNREFERENCED_PARAMETER(lpvReserved);

    switch (fdwReason)
    {
    case DLL_PROCESS_ATTACH: {
        Globals::g_thisModule = hinstDLL;

        // Create a new thread to bootstrap our hooks
        CreateThread(nullptr, 0, StartRoutine, nullptr, 0, nullptr);
        break;
    }
    case DLL_PROCESS_DETACH: {
        OnDetach();
        break;
    }
    }
    return true;
}
