#include "..\includes.hpp"

#pragma region Base Control
void CControl::SetPosition(int x, int y)
{
    m_x = x;
    m_y = y;
}

void CControl::SetSize(int w, int h)
{
    m_iWidth = w;
    m_iHeight = h;
}

void CControl::GetSize(int &w, int &h)
{
    w = m_iWidth;
    h = m_iHeight;
}

bool CControl::Flag(int f)
{
    if (m_Flags & f)
        return true;
    else
        return false;
}

POINT CControl::GetAbsolutePos()
{
    POINT p;
    RECT client = parent->GetClientArea();
    if (parent)
    {
        p.x = m_x + client.left;
        p.y = m_y + client.top + 29;
    }

    return p;
}

void CControl::SetFileId(std::string fid)
{
    FileIdentifier = fid;
}

CCheckBox::CCheckBox()
{
    // bVar = nullptr;
    Checked = new bool;

    m_Flags = UIFlags::UI_Clickable | UIFlags::UI_Drawable | UIFlags::UI_SaveFile;
    m_iWidth = 13;
    m_iHeight = 13;

    FileControlType = UIControlTypes::UIC_CheckBox;
}

void CCheckBox::SetState(bool s)
{
    *Checked = s;
}

bool CCheckBox::GetState()
{
    return *Checked;
}

void CCheckBox::SetVarCb(bool *bVar)
{
    if (Checked)
        delete Checked;

    Checked = bVar;
}

void CCheckBox::Draw(bool hover)
{
    POINT tmp = GetAbsolutePos();
    Vec2 a{float(tmp.x), float(tmp.y)};
    g_renderer->AddRect(Vec4(a.x, a.y, 13.f, 13.f), D3DCOLOR_RGBA(255, 255, 255, 255));

    if (hover)
    {
        if (*Checked)
        {
            g_renderer->AddRectFilled(Vec4(a.x + 2, a.y + 2, 9.f, 9.f), D3DCOLOR_XRGB(255, 255, 255));
        }
        else
        {
            g_renderer->AddRectFilled(Vec4(a.x + 2, a.y + 2, 9.f, 9.f), D3DCOLOR_RGBA(129, 129, 129, 255));
        }
        g_renderer->AddRect(Vec4(a.x + 2, a.y + 2, 9.f, 9.f), D3DCOLOR_RGBA(20, 20, 20, 80));
    }
    else if (*Checked)
    {
        g_renderer->AddRectFilled(Vec4(a.x + 2, a.y + 2, 9.f, 9.f), D3DCOLOR_XRGB(255, 255, 255));
        g_renderer->AddRect(Vec4(a.x + 2, a.y + 2, 9.f, 9.f), D3DCOLOR_RGBA(20, 20, 20, 80));
    }
}

void CCheckBox::OnUpdate()
{
    m_iWidth = 13;
    m_iHeight = 13;
}

void CCheckBox::OnClick()
{
    *Checked = !*Checked;
}

CLabel::CLabel()
{
    m_Flags = UIFlags::UI_Drawable;
    Text = "Default";
    FileIdentifier = "Default";
}

void CLabel::Draw(bool hover)
{
    auto col = D3DCOLOR_RGBA(245, 245, 245, 255);

    if (hover)
        col = D3DCOLOR_RGBA(205, 205, 205, 255);

    POINT tmp = GetAbsolutePos();
    Vec2 a{float(tmp.x), float(tmp.y)};
    g_renderer->AddText(g_fontGUIBold, Text, Vec2(a.x, a.y), col);
}

void CLabel::SetText(std::string text)
{
    Text = text;
}

void CLabel::OnUpdate()
{
}
void CLabel::OnClick()
{
}
#pragma endregion Implementations of the Label functions

#pragma region GroupBox
CGroupBox::CGroupBox()
{
    Items = 1;
    m_Flags = UIFlags::UI_Drawable | UIFlags::UI_RenderFirst;
    Text = "Default";
    FileIdentifier = "Default";
}

void CGroupBox::Draw(bool hover)
{
    UNREFERENCED_PARAMETER(hover);

    POINT tmp = GetAbsolutePos();
    Vec2 a{float(tmp.x), float(tmp.y)};

    auto txtSize = g_renderer->CalculateTextExtent(g_fontGUIBold, Text);

    g_renderer->AddRectFilled(Vec4(a.x + 2, a.y + 2, float(m_iWidth - 4), float(m_iHeight - 4)),
                              D3DCOLOR_RGBA(8, 8, 8, 10)); // menu mini box color
    g_renderer->AddText(g_fontGUIBold, Text, Vec2(a.x + 15, a.y - (txtSize.y / 2)), D3DCOLOR_RGBA(255, 255, 255, 255));

    g_renderer->AddLine(Vec2(a.x, a.y), Vec2(a.x + 12, a.y), D3DCOLOR_RGBA(129, 129, 129, 255));
    g_renderer->AddLine(Vec2(a.x + 15 + txtSize.x + 5, a.y), Vec2(a.x + m_iWidth, a.y),
                        D3DCOLOR_RGBA(129, 129, 129, 255));
    g_renderer->AddLine(Vec2(a.x, a.y), Vec2(a.x, a.y + m_iHeight), D3DCOLOR_RGBA(129, 129, 129, 255));
    g_renderer->AddLine(Vec2(a.x, a.y + m_iHeight), Vec2(a.x + m_iWidth, a.y + m_iHeight),
                        D3DCOLOR_RGBA(129, 129, 129, 255));
    g_renderer->AddLine(Vec2(a.x + m_iWidth, a.y), Vec2(a.x + m_iWidth, a.y + m_iHeight),
                        D3DCOLOR_RGBA(129, 129, 129, 255));
}

void CGroupBox::SetText(std::string text)
{
    Text = text;
}

void CGroupBox::PlaceLabledControl(std::string Label, CTab *Tab, CControl *control)
{
    int x = m_x + 16;
    int y = m_y + Items * 24;

    CLabel *label = new CLabel;
    label->SetPosition(x, y);
    label->SetText(Label);
    Tab->RegisterControl(label);

    x += m_iWidth / 2;

    int cw, ch;
    control->SetPosition(x, y);
    control->GetSize(cw, ch);
    control->SetSize((m_iWidth / 2) - 32, ch);
    Tab->RegisterControl(control);
    Items++;
}

void CGroupBox::OnUpdate()
{
}
void CGroupBox::OnClick()
{
}
#pragma endregion Implementations of the Group Box functions

#pragma region Sliders
CSlider::CSlider()
{
    Value = new float;
    m_Flags = UIFlags::UI_Drawable | UIFlags::UI_Clickable | UIFlags::UI_SaveFile;

    FileControlType = UIControlTypes::UIC_Slider;
}

void CSlider::Draw(bool hover)
{
    UNREFERENCED_PARAMETER(hover);

    POINT tmp = GetAbsolutePos();
    Vec2 a{float(tmp.x), float(tmp.y)};

    g_renderer->AddRectFilled(Vec4(a.x, a.y + 5.f, float(m_iWidth), 2.f), D3DCOLOR_XRGB(255, 255, 255));

    float Ratio = *Value / (Max - Min);
    auto Location = Ratio * m_iWidth;

    g_renderer->AddRectFilled(Vec4(a.x + Location, a.y + 1, 4.f, 9.f), D3DCOLOR_XRGB(255, 255, 255));
    g_renderer->AddRect(Vec4(a.x + Location, a.y + 1, 4.f, 9.f), D3DCOLOR_RGBA(20, 20, 20, 80));

    char fmt[24]{};
    sprintf_s(fmt, "%.f", *Value);

    auto txtSize = g_renderer->CalculateTextExtent(g_fontGUIBold, fmt);

    g_renderer->AddText(g_fontGUIBold, fmt, Vec2(a.x + (m_iWidth / 2) - txtSize.y / 2, a.y + 10),
                        D3DCOLOR_RGBA(255, 255, 255, 255));
}

void CSlider::OnUpdate()
{
    POINT a = GetAbsolutePos();
    m_iHeight = 11;

    if (DoDrag)
    {
        if (GUI.GetKeyState(VK_LBUTTON))
        {
            POINT m = GUI.GetMouse();
            float NewX;
            float Ratio;
            NewX = static_cast<float>(m.x - a.x - 1);
            if (NewX < 0)
                NewX = 0;
            if (NewX > m_iWidth)
                NewX = float(m_iWidth);
            Ratio = NewX / float(m_iWidth);
            *Value = Min + (Max - Min) * Ratio;
        }
        else
        {
            DoDrag = false;
        }
    }
}

void CSlider::OnClick()
{
    POINT a = GetAbsolutePos();
    RECT SliderRegion = {a.x, a.y, m_iWidth, 11};

    if (GUI.IsMouseInRegion(SliderRegion))
    {
        DoDrag = true;
    }
}

float CSlider::GetValue()
{
    return *Value;
}

void CSlider::SetValue(float v)
{
    *Value = v;
}

void CSlider::SetVarCb(float *var)
{
    Value = var;
}

void CSlider::SetBoundaries(float min, float max)
{
    Min = min;
    Max = max;
}

CKeyBind::CKeyBind()
{
    Key = new int;
    m_Flags = UIFlags::UI_Drawable | UIFlags::UI_Clickable | UIFlags::UI_SaveFile;
    FileControlType = UIControlTypes::UIC_KeyBind;
}

void CKeyBind::SetVarCb(int *var)
{
    Key = var;
}

void CKeyBind::Draw(bool hover)
{
    POINT tmp = GetAbsolutePos();
    Vec2 a{float(tmp.x), float(tmp.y)};

    g_renderer->AddRect(Vec4(a.x, a.y, float(m_iWidth), float(m_iHeight)), D3DCOLOR_RGBA(129, 129, 129, 255));
    if (hover)
    {
        g_renderer->AddRectFilled(Vec4(a.x + 2.f, a.y + 2.f, float(m_iWidth - 4), float(m_iHeight - 4)),
                                  D3DCOLOR_RGBA(255, 255, 255, 80));
    }

    char KeyName[128]{};
    strcpy_s(KeyName, "< Unbound >");

    if (IsGettingKey)
    {
        strcpy_s(KeyName, "...");
    }
    else
    {
        if (*Key > 0)
        {
            if (*Key <= VK_RBUTTON)
            {
                sprintf_s(KeyName, "Mouse %d", *Key);
            }
            else if (*Key > VK_CANCEL && *Key <= VK_XBUTTON2)
            {
                sprintf_s(KeyName, "Mouse %d", *Key - 1);
            }
            else
            {
                auto scanCode = MapVirtualKey(*Key, 0) << 16;

                switch (*Key)
                {
                case VK_HOME:
                case VK_END:
                case VK_LEFT:
                case VK_UP:
                case VK_RIGHT:
                case VK_DOWN:
                case VK_PRIOR:
                case VK_NEXT:
                case VK_INSERT:
                case VK_DELETE:
                case VK_NUMLOCK:
                    scanCode |= 0x01000000;
                }

                char NameBuffer[128]{};
                if (::GetKeyNameTextA(scanCode, NameBuffer, sizeof(NameBuffer)))
                {
                    strcpy_s(KeyName, NameBuffer);
                }
                else
                {
                    strcpy_s(KeyName, "< Invalid >");
                }
            }
        }
        else
        {
            strcpy_s(KeyName, "< Unbound >");
        }
    }

    g_renderer->AddText(g_fontGUIBold, KeyName, Vec2((a.x + 2) + m_iWidth / 2, a.y), D3DCOLOR_RGBA(255, 255, 255, 255),
                        TEXT_FLAG_CENTERED_X);
}

void CKeyBind::OnUpdate()
{
    m_iHeight = 16;
    POINT a = GetAbsolutePos();
    if (IsGettingKey)
    {
        for (int i = 0; i < 255; i++)
        {
            if (GUI.GetKeyPress(i))
            {
                if (i == VK_ESCAPE)
                {
                    IsGettingKey = false;
                    *Key = -1;
                    return;
                }

                *Key = i;
                IsGettingKey = false;
                return;
            }
        }
    }
}

void CKeyBind::OnClick()
{
    POINT a = GetAbsolutePos();
    if (!IsGettingKey)
    {
        IsGettingKey = true;
    }
}

int CKeyBind::GetKey()
{
    return *Key;
}

void CKeyBind::SetKey(int key)
{
    *Key = key;
}

CButton::CButton()
{
    m_iWidth = 177;
    m_Flags = UIFlags::UI_Drawable | UIFlags::UI_Clickable;
    Text = "Default";
    CallBack = nullptr;
    FileIdentifier = "Default";
}

void CButton::Draw(bool hover)
{
    POINT tmp = GetAbsolutePos();
    Vec2 a{float(tmp.x), float(tmp.y)};
    g_renderer->AddRect(Vec4(a.x, a.y, float(m_iWidth), float(m_iHeight)), D3DCOLOR_RGBA(129, 129, 129, 255));

    if (hover)
    {
        g_renderer->AddGradientRect(Vec4(a.x + 2.f, a.y + 2.f, float(m_iWidth - 4), float(m_iHeight - 4)),
                                    D3DCOLOR_RGBA(60, 60, 60, 255), D3DCOLOR_RGBA(80, 80, 80, 255),
                                    GradientDirection::Horizontal);
    }
    else
    {
        g_renderer->AddGradientRect(Vec4(a.x + 2.f, a.y + 2.f, float(m_iWidth - 4), float(m_iHeight - 4)),
                                    D3DCOLOR_RGBA(8, 8, 8, 255), D3DCOLOR_RGBA(8, 8, 8, 255),
                                    GradientDirection::Horizontal);
    }

    g_renderer->AddText(g_fontGUIBold, Text, Vec2(a.x + float(m_iWidth / 2), a.y + float(m_iHeight / 2)),
                        D3DCOLOR_RGBA(255, 255, 255, 255), TEXT_FLAG_CENTERED);
}

void CButton::SetText(std::string text)
{
    Text = text;
}

void CButton::SetCallback(CButton::ButtonCallback_t callback)
{
    CallBack = callback;
}

void CButton::OnUpdate()
{
    m_iHeight = 26;
}

void CButton::OnClick()
{
    if (CallBack)
        CallBack();
}

CComboBox::CComboBox()
{
    IsOpen = false;
    SelectedIndex = new int;
    m_Flags = UIFlags::UI_Drawable | UIFlags::UI_Clickable | UIFlags::UI_Focusable | UIFlags::UI_SaveFile;
    FileControlType = UIControlTypes::UIC_ComboBox;
}

void CComboBox::SetVarCb(int *var)
{
    SelectedIndex = var;
}

void CComboBox::Draw(bool hover)
{
    UNREFERENCED_PARAMETER(hover);

    POINT tmp = GetAbsolutePos();
    Vec2 a{float(tmp.x), float(tmp.y)};
    RECT Region = {tmp.x, tmp.y, m_iWidth, 16};
    g_renderer->AddRect(Vec4(a.x, a.y, float(m_iWidth), 16.f), D3DCOLOR_RGBA(129, 129, 129, 255));

    // Hover for the Top Box
    if (GUI.IsMouseInRegion(Region))
    {
        g_renderer->AddRectFilled(Vec4(a.x + 2.f, a.y + 2.f, float(m_iWidth - 4), 12.f),
                                  D3DCOLOR_RGBA(80, 80, 80, 255));
    }

    // If we have some items
    if (Items.size() > 0)
    {
        // The current item
        g_renderer->AddText(g_fontGUIBold, GetItem(), Vec2(a.x + 2.f, a.y), D3DCOLOR_RGBA(255, 255, 255, 255));

        // If the drop down part is open
        if (IsOpen)
        {
            g_renderer->AddRectFilled(Vec4(a.x, a.y + 17.f, float(m_iWidth), float(Items.size() * 16)),
                                      D3DCOLOR_RGBA(80, 80, 80, 255));

            // Draw the items
            for (auto i = 0ul; i < Items.size(); i++)
            {
                RECT ItemRegion = {tmp.x, tmp.y + 17L + long(i * 16), long(m_iWidth), 16L};

                // Hover
                if (GUI.IsMouseInRegion(ItemRegion))
                {
                    g_renderer->AddRectFilled(Vec4(a.x, a.y + 17.f + float(i * 16), float(m_iWidth), 16.f),
                                              D3DCOLOR_RGBA(207, 207, 207, 255));
                }

                g_renderer->AddText(g_fontGUIBold, Items[i], Vec2(a.x + 2.f, a.y + 19.f + float(i * 16)),
                                    D3DCOLOR_RGBA(255, 255, 255, 255));
            }
        }
    }
}

void CComboBox::AddItem(std::string text)
{
    Items.push_back(text);
    *SelectedIndex = 0;
}

void CComboBox::OnUpdate()
{
    if (IsOpen)
    {
        m_iHeight = 16 + 16 * Items.size();

        if (parent->GetFocus() != this)
        {
            IsOpen = false;
        }
    }
    else
    {
        m_iHeight = 16;
    }
}

void CComboBox::OnClick()
{
    POINT a = GetAbsolutePos();
    RECT Region = {a.x, a.y, m_iWidth, 16};

    if (IsOpen)
    {
        // If we clicked one of the items(Not in the top bar)
        if (!GUI.IsMouseInRegion(Region))
        {
            // Draw the items
            for (auto i = 0ul; i < Items.size(); i++)
            {
                RECT ItemRegion = {a.x, a.y + 16 + long(i) * 16, m_iWidth, 16};

                // Hover
                if (GUI.IsMouseInRegion(ItemRegion))
                {
                    *SelectedIndex = i;
                }
            }
        }

        // Close the drop down
        IsOpen = false;
    }
    else
    {
        IsOpen = true;
    }
}

int CComboBox::GetIndex()
{
    return *SelectedIndex;
}

std::string CComboBox::GetItem()
{
    if (*SelectedIndex >= 0 && *SelectedIndex < int(Items.size()))
    {
        return Items[*SelectedIndex];
    }
    return "Error";
}

void CComboBox::SelectIndex(int idx)
{
    if (idx >= 0 && idx < int(Items.size()))
    {
        *SelectedIndex = idx;
    }
}