#pragma once

namespace Hooks
{
using Reset_t = HRESULT(WINAPI *)(LPDIRECT3DDEVICE9, D3DPRESENT_PARAMETERS *);
inline Reset_t oReset = nullptr;

extern HRESULT WINAPI hkReset(LPDIRECT3DDEVICE9 device, D3DPRESENT_PARAMETERS *presentParams);

using EndScene_t = HRESULT(WINAPI *)(LPDIRECT3DDEVICE9);
inline EndScene_t oEndScene = nullptr;

extern HRESULT WINAPI hkEndScene(LPDIRECT3DDEVICE9 device);

using R_EndFrame_t = void(WINAPIV *)();
inline R_EndFrame_t oR_EndFrame = nullptr;

extern void WINAPIV hkR_EndFrame();

using CG_PredictPlayerState_t = void(WINAPIV *)(int localClientNum);
inline CG_PredictPlayerState_t oCG_PredictPlayerState = nullptr;

extern void WINAPIV hkCG_PredictPlayerState(int localClientNum);

using CL_WritePacket_t = void(WINAPIV *)(int localClientNum);
inline CL_WritePacket_t oCL_WritePacket = nullptr;

extern void WINAPIV hkCL_WritePacket(int localClientNum);

} // namespace Hooks