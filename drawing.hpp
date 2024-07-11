#pragma once

#include <renderer_dx9.hpp>
using namespace CheatRenderFramework;

namespace Drawing
{
inline RendererPtr g_renderer = nullptr;
inline RenderListPtr g_drawList = nullptr;
inline RenderListPtr g_drawListAct = nullptr;
inline RenderListPtr g_drawListRendering = nullptr;
inline std::mutex g_renderMutex{};

inline FontHandle g_fontESP = 0;
inline FontHandle g_fontGUI = 0;
inline FontHandle g_fontGUIBold = 0;

inline RenderListPtr &GetRenderList()
{
    if (g_renderMutex.try_lock())
    {
        *g_drawListRendering = *g_drawListAct;
        g_renderMutex.unlock();
    }
    return g_drawListRendering;
}

inline void BeginDraw()
{
    g_drawList->Clear();
}

inline void EndDraw()
{
    g_renderMutex.lock();
    *g_drawListAct = *g_drawList;
    g_renderMutex.unlock();
}
} // namespace Drawing