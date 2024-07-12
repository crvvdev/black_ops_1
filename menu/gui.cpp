#include "..\includes.hpp"
#include <algorithm>

CGUI GUI;

// Draws all windows
void CGUI::Draw()
{
    bool ShouldDrawCursor = false;

    for (auto window : Windows)
    {
        if (window->m_bIsOpen)
        {
            *(char *)ENABLE_CURSOR = 0;
            ShouldDrawCursor = true;
            DrawWindow(window);
        }
    }

    if (ShouldDrawCursor)
    {
        Vec2 tmp{float(Mouse.x), float(Mouse.y)};

        g_renderer->AddRectFilled(Vec4(tmp.x + 1, tmp.y, 1.f, 17.f), D3DCOLOR_RGBA(3, 6, 26, 255));
        for (int i = 0; i < 11; i++)
            g_renderer->AddRectFilled(Vec4(tmp.x + 2 + i, tmp.y + 1 + i, 1l, 1l), D3DCOLOR_RGBA(3, 6, 26, 255));
        g_renderer->AddRectFilled(Vec4(tmp.x + 8, tmp.y + 12, 5l, 1l), D3DCOLOR_RGBA(3, 6, 26, 255));
        g_renderer->AddRectFilled(Vec4(tmp.x + 8, tmp.y + 13, 1l, 1l), D3DCOLOR_RGBA(3, 6, 26, 255));
        g_renderer->AddRectFilled(Vec4(tmp.x + 9, tmp.y + 14, 1l, 2l), D3DCOLOR_RGBA(3, 6, 26, 255));
        g_renderer->AddRectFilled(Vec4(tmp.x + 10, tmp.y + 16, 1l, 2l), D3DCOLOR_RGBA(3, 6, 26, 255));
        g_renderer->AddRectFilled(Vec4(tmp.x + 8, tmp.y + 18, 2l, 1l), D3DCOLOR_RGBA(3, 6, 26, 255));
        g_renderer->AddRectFilled(Vec4(tmp.x + 7, tmp.y + 16, 1l, 2l), D3DCOLOR_RGBA(3, 6, 26, 255));
        g_renderer->AddRectFilled(Vec4(tmp.x + 6, tmp.y + 14, 1l, 2l), D3DCOLOR_RGBA(3, 6, 26, 255));
        g_renderer->AddRectFilled(Vec4(tmp.x + 5, tmp.y + 13, 1l, 1l), D3DCOLOR_RGBA(3, 6, 26, 255));
        g_renderer->AddRectFilled(Vec4(tmp.x + 4, tmp.y + 14, 1l, 1l), D3DCOLOR_RGBA(3, 6, 26, 255));
        g_renderer->AddRectFilled(Vec4(tmp.x + 3, tmp.y + 15, 1l, 1l), D3DCOLOR_RGBA(3, 6, 26, 255));
        g_renderer->AddRectFilled(Vec4(tmp.x + 2, tmp.y + 16, 1l, 1l), D3DCOLOR_RGBA(3, 6, 26, 255));
        for (int i = 0; i < 4; i++)
            g_renderer->AddRectFilled(Vec4(tmp.x + 2 + i, tmp.y + 2 + i, 1.f, float(14 - (i * 2))),
                                      D3DCOLOR_RGBA(37 - (i * 4), 137 - (i * 4), 255 - (i * 4), 255));
        g_renderer->AddRectFilled(Vec4(tmp.x + 6, tmp.y + 6, 1.f, 8l), D3DCOLOR_RGBA(0, 150, 255, 255));
        g_renderer->AddRectFilled(Vec4(tmp.x + 7, tmp.y + 7, 1.f, 9l), D3DCOLOR_RGBA(0, 150, 255, 255));
        for (int i = 0; i < 4; i++)
            g_renderer->AddRectFilled(Vec4(tmp.x + 8 + i, tmp.y + 8 + i, 1.f, float(4 - i)),
                                      D3DCOLOR_RGBA(37 - (i * 4), 137 - (i * 4), 255 - (i * 4), 255));
        g_renderer->AddRectFilled(Vec4(tmp.x + 8, tmp.y + 14, 1.f, 4l), D3DCOLOR_RGBA(0, 150, 255, 255));
        g_renderer->AddRectFilled(Vec4(tmp.x + 9, tmp.y + 16, 1.f, 2l), D3DCOLOR_RGBA(0, 150, 255, 255));
    }
    else
    {
        *(char *)ENABLE_CURSOR = 1;
    }
}

// Handle all input etc
void CGUI::Update()
{
    bool IsDraggingWindow = true;

    // Key Array
    std::copy(keys, keys + 255, oldKeys);

    for (int x = 1; x < 255; x++)
        keys[x] = ::GetAsyncKeyState(x);

    // Mouse Location
    GetCursorPos(&Mouse);
    ScreenToClient(hWnd, &Mouse);

    // Window Binds
    for (auto &bind : WindowBinds)
    {
        if (GetKeyPress(bind.first))
        {
            bind.second->Toggle();
        }
    }

    // Stop dragging
    if (IsDraggingWindow && !GetKeyState(VK_LBUTTON))
    {
        IsDraggingWindow = false;
        DraggingWindow = nullptr;
    }

    // If we are in the proccess of dragging a window
    if (IsDraggingWindow && GetKeyState(VK_LBUTTON) && !GetKeyPress(VK_LBUTTON))
    {
        if (DraggingWindow)
        {
            DraggingWindow->m_x = Mouse.x - DragOffsetX;
            DraggingWindow->m_y = Mouse.y - DragOffsetY;
        }
    }

    // Process some windows
    for (auto window : Windows)
    {
        if (window->m_bIsOpen)
        {
            // Used to tell the widget processing that there could be a click
            bool bCheckWidgetClicks = false;

            // If the user clicks inside the window
            if (GetKeyPress(VK_LBUTTON))
            {
                // if (IsMouseInRegion(window->m_x, window->m_y, window->m_x + window->m_iWidth, window->m_y +
                // window->m_iHeight))
                //{
                //  Is it inside the client area?
                if (IsMouseInRegion(window->GetClientArea1()))
                {
                    // User is selecting a new tab
                    if (IsMouseInRegion(window->GetTabArea()))
                    {
                        // Loose focus on the control
                        window->IsFocusingControl = false;
                        window->FocusedControl = nullptr;

                        int iTab = 0;
                        int TabCount = window->Tabs.size();
                        if (TabCount) // If there are some tabs
                        {
                            int TabSize = (window->m_iWidth - 4 - 12) / TabCount;
                            int Dist = Mouse.x - (window->m_x + 8);
                            while (Dist > TabSize)
                            {
                                if (Dist > TabSize)
                                {
                                    iTab++;
                                    Dist -= TabSize;
                                }
                            }
                            window->SelectedTab = window->Tabs[iTab];
                        }
                    }
                    else
                        bCheckWidgetClicks = true;
                }
                else if (IsMouseInRegion(window->m_x, window->m_y, window->m_x + window->m_iWidth,
                                         window->m_y + window->m_iHeight))
                {
                    // Must be in the around the title or side of the window
                    // So we assume the user is trying to drag the window
                    IsDraggingWindow = true;
                    DraggingWindow = window;
                    DragOffsetX = Mouse.x - window->m_x;
                    DragOffsetY = Mouse.y - window->m_y;

                    // Loose focus on the control
                    window->IsFocusingControl = false;
                    window->FocusedControl = nullptr;
                }

                // else
                //{
                //  Loose focus on the control
                // window->IsFocusingControl = false;
                // window->FocusedControl = nullptr;
                //}
            }

            // Controls
            if (window->SelectedTab != nullptr)
            {
                // Focused widget
                bool SkipWidget = false;
                CControl *SkipMe = nullptr;

                // this window is focusing on a widget??
                if (window->IsFocusingControl)
                {
                    if (window->FocusedControl != nullptr)
                    {
                        // We've processed it once, skip it later
                        SkipWidget = true;
                        SkipMe = window->FocusedControl;

                        POINT cAbs = window->FocusedControl->GetAbsolutePos();
                        RECT controlRect = {cAbs.x, cAbs.y, window->FocusedControl->m_iWidth,
                                            window->FocusedControl->m_iHeight};
                        window->FocusedControl->OnUpdate();

                        if (window->FocusedControl->Flag(UIFlags::UI_Clickable) && IsMouseInRegion(controlRect) &&
                            bCheckWidgetClicks)
                        {
                            window->FocusedControl->OnClick();

                            // If it gets clicked we loose focus
                            window->IsFocusingControl = false;
                            window->FocusedControl = nullptr;
                            bCheckWidgetClicks = false;
                        }
                    }
                }

                // Itterate over the rest of the control
                for (auto control : window->SelectedTab->Controls)
                {
                    if (control != nullptr)
                    {
                        if (SkipWidget && SkipMe == control)
                            continue;

                        POINT cAbs = control->GetAbsolutePos();
                        RECT controlRect = {cAbs.x, cAbs.y, control->m_iWidth, control->m_iHeight};
                        control->OnUpdate();

                        if (control->Flag(UIFlags::UI_Clickable) && IsMouseInRegion(controlRect) && bCheckWidgetClicks)
                        {
                            control->OnClick();
                            bCheckWidgetClicks = false;

                            // Change of focus
                            if (control->Flag(UIFlags::UI_Focusable))
                            {
                                window->IsFocusingControl = true;
                                window->FocusedControl = control;
                            }
                            else
                            {
                                window->IsFocusingControl = false;
                                window->FocusedControl = nullptr;
                            }
                        }
                    }
                }

                // We must have clicked whitespace
                if (bCheckWidgetClicks)
                {
                    // Loose focus on the control
                    window->IsFocusingControl = false;
                    window->FocusedControl = nullptr;
                }
            }
        }
    }
}

// Returns
bool CGUI::GetKeyPress(unsigned int key)
{
    if (keys[key] == true && oldKeys[key] == false)
        return true;
    else
        return false;
}

bool CGUI::GetKeyState(unsigned int key)
{
    return keys[key];
}

bool CGUI::IsMouseInRegion(int x, int y, int x2, int y2)
{
    if (Mouse.x > x && Mouse.y > y && Mouse.x < x2 && Mouse.y < y2)
        return true;
    else
        return false;
}

bool CGUI::IsMouseInRegion(RECT region)
{
    return IsMouseInRegion(region.left, region.top, region.left + region.right, region.top + region.bottom);
}

POINT CGUI::GetMouse()
{
    return Mouse;
}

bool CGUI::DrawWindow(CWindow *window)
{
    g_renderer->AddRect(Vec4(float(window->m_x), float(window->m_y), float(window->m_iWidth), float(window->m_iHeight)),
                        D3DCOLOR_RGBA(21, 21, 21, 80));

    g_renderer->AddGradientRect(Vec4(float(window->m_x + 2), float(window->m_y + 2), float(window->m_iWidth - 4), 26.f),
                                D3DCOLOR_ARGB(200, 13, 5, 255), D3DCOLOR_ARGB(200, 10, 10, 10),
                                GradientDirection::Horizontal);

    g_renderer->AddRectFilled(Vec4(float(window->m_x + 2), float(window->m_y + 2 + 26), float(window->m_iWidth - 4),
                                   float(window->m_iHeight - 4 - 26)),
                              D3DCOLOR_ARGB(200, 10, 10, 10));

    g_renderer->AddRect(
        Vec4(float(window->m_x + 1), float(window->m_y + 1), float(window->m_iWidth - 2), float(window->m_iHeight - 2)),
        D3DCOLOR_RGBA(0, 0, 0, 255));

    g_renderer->AddText(g_fontGUIBold, window->Title,
                        Vec2(float(window->m_x + 8 + window->m_iWidth / 2), float(window->m_y + 8)),
                        D3DCOLOR_XRGB(255, 255, 255), TEXT_FLAG_CENTERED_X);

    g_renderer->AddRectFilled(Vec4(float(window->m_x + 8), float(window->m_y + 1 + 27),
                                   float(window->m_iWidth - 4 - 12), float(window->m_iHeight - 2 - 8 - 26)),
                              D3DCOLOR_RGBA(30, 30, 30, 255));

    g_renderer->AddGradientRect(
        Vec4(float(window->m_x + 8), float(window->m_y + 1 + 27), float(window->m_iWidth - 4 - 12), 29.f),
        D3DCOLOR_RGBA(49, 42, 42, 255), D3DCOLOR_RGBA(49, 42, 42, 255), GradientDirection::Horizontal);

    int TabCount = static_cast<int>(window->Tabs.size());
    if (TabCount) // If there are some tabs
    {
        int TabSize = (window->m_iWidth - 4 - 12) / TabCount;
        for (int i = 0; i < TabCount; i++)
        {
            RECT TabArea = {window->m_x + 8 + (i * TabSize), window->m_y + 1 + 27, TabSize, 29};
            CTab *tab = window->Tabs[i];
            if (window->SelectedTab == tab)
            {
                g_renderer->AddGradientRect(
                    Vec4(float(window->m_x + 8 + (i * TabSize)), float(window->m_y + 1 + 27), float(TabSize), 29.f),
                    D3DCOLOR_RGBA(106, 106, 106, 255), D3DCOLOR_RGBA(49, 42, 42, 255), GradientDirection::Horizontal);
            }
            else if (IsMouseInRegion(TabArea))
            {
                g_renderer->AddGradientRect(
                    Vec4(float(window->m_x + 8 + (i * TabSize)), float(window->m_y + 1 + 27), float(TabSize), 29.f),
                    D3DCOLOR_RGBA(106, 106, 106, 255), D3DCOLOR_RGBA(80, 80, 80, 255), GradientDirection::Horizontal);
            }

            g_renderer->AddRectFilled(
                Vec4(float(window->m_x + 8), float(window->m_y + 1 + 27), float(window->m_iWidth - 4 - 12), 2.f),
                D3DCOLOR_ARGB(200, 10, 10, 10));

            g_renderer->AddText(g_fontGUIBold, tab->Title,
                                Vec2(float(TabArea.left + (TabSize / 2)), float(TabArea.top + 8)),
                                D3DCOLOR_XRGB(255, 255, 255), TEXT_FLAG_CENTERED_X);
        }
    }

    // Controls
    if (window->SelectedTab != nullptr)
    {
        // Focused widget
        bool SkipWidget = false;
        CControl *SkipMe = nullptr;

        // this window is focusing on a widget??
        if (window->IsFocusingControl)
        {
            if (window->FocusedControl != nullptr)
            {
                // We need to draw it last, so skip it in the regular loop
                SkipWidget = true;
                SkipMe = window->FocusedControl;
            }
        }

        // Itterate over all the other controls
        for (auto control : window->SelectedTab->Controls)
        {
            if (SkipWidget && SkipMe == control)
                continue;

            if (control != nullptr && control->Flag(UIFlags::UI_Drawable))
            {
                POINT cAbs = control->GetAbsolutePos();
                RECT controlRect = {cAbs.x, cAbs.y, control->m_iWidth, control->m_iHeight};
                bool hover = false;
                if (IsMouseInRegion(controlRect))
                {
                    hover = true;
                }
                control->Draw(hover);
            }
        }

        // Draw the skipped widget last
        if (SkipWidget)
        {
            auto control = window->FocusedControl;

            if (control != nullptr && control->Flag(UIFlags::UI_Drawable))
            {
                POINT cAbs = control->GetAbsolutePos();
                RECT controlRect = {cAbs.x, cAbs.y, control->m_iWidth, control->m_iHeight};
                bool hover = false;
                if (IsMouseInRegion(controlRect))
                {
                    hover = true;
                }
                control->Draw(hover);
            }
        }
    }

    return true;
}

void CGUI::RegisterWindow(CWindow *window)
{
    Windows.push_back(window);

    // Resorting to put groupboxes at the start
    for (auto tab : window->Tabs)
    {
        for (auto control : tab->Controls)
        {
            if (control->Flag(UIFlags::UI_RenderFirst))
            {
                tab->Controls.erase(std::remove(tab->Controls.begin(), tab->Controls.end(), control),
                                    tab->Controls.end());
                tab->Controls.insert(tab->Controls.begin(), control);
            }
        }
    }
}

void CGUI::BindWindow(unsigned char Key, CWindow *window)
{
    if (window)
        WindowBinds.insert({Key, window});
    else
        WindowBinds.erase(Key);
}

BOOL WritePrivateProfileInt(LPCSTR lpAppName, LPCSTR lpKeyName, int iValue, LPCSTR lpFileName)
{
    char szTemp[127]{};
    wsprintfA(szTemp, "%d", iValue);
    return WritePrivateProfileStringA(lpAppName, lpKeyName, szTemp, lpFileName);
}

void CGUI::SaveWindowState(CWindow *window, std::string Filename)
{
    if (!window->Tabs.empty())
    {
        for (auto &tabs : window->Tabs)
        {
            WritePrivateProfileSectionA(tabs->Title.c_str(), 0, Filename.c_str());

            if (!tabs->Controls.empty())
            {
                for (auto &control : tabs->Controls)
                {
                    if (control && control->Flag(UIFlags::UI_SaveFile) && control->FileIdentifier.length() > 1 &&
                        control->FileControlType)
                    {
                        CCheckBox *cbx = nullptr;
                        CComboBox *cbo = nullptr;
                        CKeyBind *key = nullptr;
                        CSlider *sld = nullptr;

                        switch (control->FileControlType)
                        {
                        case UIControlTypes::UIC_CheckBox:
                            cbx = (CCheckBox *)control;
                            WritePrivateProfileInt(tabs->Title.c_str(), control->FileIdentifier.c_str(),
                                                   static_cast<int>(cbx->GetState()), Filename.c_str());
                            break;
                        case UIControlTypes::UIC_ComboBox:
                            cbo = (CComboBox *)control;
                            WritePrivateProfileInt(tabs->Title.c_str(), control->FileIdentifier.c_str(),
                                                   cbo->GetIndex(), Filename.c_str());
                            break;
                        case UIControlTypes::UIC_KeyBind:
                            key = (CKeyBind *)control;
                            WritePrivateProfileInt(tabs->Title.c_str(), control->FileIdentifier.c_str(), key->GetKey(),
                                                   Filename.c_str());
                            break;
                        case UIControlTypes::UIC_Slider:
                            sld = (CSlider *)control;
                            WritePrivateProfileInt(tabs->Title.c_str(), control->FileIdentifier.c_str(),
                                                   static_cast<int>(sld->GetValue()), Filename.c_str());
                            break;
                        }
                    }
                }
            }
        }
    }
}

void CGUI::LoadWindowState(CWindow *window, std::string Filename)
{
    if (!window->Tabs.empty())
    {
        for (auto &tabs : window->Tabs)
        {
            if (!tabs->Controls.empty())
            {
                for (auto &control : tabs->Controls)
                {
                    if (control && control->Flag(UIFlags::UI_SaveFile) && control->FileIdentifier.length() > 1 &&
                        control->FileControlType)
                    {
                        switch (control->FileControlType)
                        {
                        case UIControlTypes::UIC_CheckBox: {
                            auto cbx = (CCheckBox *)control;
                            auto val = GetPrivateProfileIntA(tabs->Title.c_str(), control->FileIdentifier.c_str(),
                                                             static_cast<int>(cbx->GetState()), Filename.c_str());
                            cbx->SetState(static_cast<bool>(val));
                            break;
                        }

                        case UIControlTypes::UIC_ComboBox: {
                            auto cbo = (CComboBox *)control;
                            auto val = GetPrivateProfileIntA(tabs->Title.c_str(), control->FileIdentifier.c_str(),
                                                             cbo->GetIndex(), Filename.c_str());
                            cbo->SelectIndex(val);
                            break;
                        }

                        case UIControlTypes::UIC_KeyBind: {
                            auto key = (CKeyBind *)control;
                            auto val = GetPrivateProfileIntA(tabs->Title.c_str(), control->FileIdentifier.c_str(),
                                                             key->GetKey(), Filename.c_str());
                            key->SetKey(val);
                            break;
                        }

                        case UIControlTypes::UIC_Slider: {
                            auto sld = (CSlider *)control;
                            auto val = GetPrivateProfileIntA(tabs->Title.c_str(), control->FileIdentifier.c_str(),
                                                             static_cast<int>(sld->GetValue()), Filename.c_str());
                            sld->SetValue(static_cast<float>(val));
                            break;
                        }
                        }
                    }
                }
            }
        }
    }
}
