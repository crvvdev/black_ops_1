#include "includes.hpp"

static constexpr char g_configFile[] = "C:\\ProgramData\\blackops_1.cfg";
static constexpr char g_menuTitle[] = "T5MP BOT";
static constexpr int g_windowWidth = 575;
static constexpr int g_windowHeight = 385;

CMenuWindow Menu::Window(true);
bool Menu::MenuCreated = false;

CCheckBox *CMenuWindow::AddCheckbox(std::string szCaption, bool *bVar, CGroupBox *Group, CTab *Tab,
                                    std::string szFileId, int x, int y)
{
    if (!Tab)
        return nullptr;

    CCheckBox *chk = new CCheckBox;
    if (!szFileId.empty())
        chk->SetFileId(szFileId);

    if (x > 0 && y > 0)
        chk->SetPosition(x, y);

    if (bVar)
        chk->SetVarCb(bVar);

    if (!szCaption.empty() && Group)
        Group->PlaceLabledControl(szCaption, Tab, chk);
    else
        Tab->RegisterControl(chk);

    return chk;
}

CTab *CMenuWindow::AddTab(std::string szCaption)
{
    if (szCaption.empty())
        return nullptr;

    CTab *tab = new CTab;
    tab->SetTitle(szCaption);
    this->RegisterTab(tab);
    return tab;
}

CLabel *CMenuWindow::AddLabel(std::string szCaption, CTab *Tab, int x, int y)
{
    if (szCaption.empty() || !Tab)
        return nullptr;

    CLabel *label = new CLabel;
    if (x > 0 && y > 0)
        label->SetPosition(x, y);

    label->SetText(szCaption);
    Tab->RegisterControl(label);
    return label;
}

CGroupBox *CMenuWindow::AddGroup(std::string szCaption, CTab *Tab, int x, int y, int w, int h)
{
    if (szCaption.empty() || !Tab)
        return nullptr;

    CGroupBox *group = new CGroupBox;
    if (x > 0 && y > 0)
        group->SetPosition(x, y);

    group->SetText(szCaption);
    group->SetSize(w, h);
    Tab->RegisterControl(group);
    return group;
}

CSlider *CMenuWindow::AddSlider(std::string szCaption, float *flVar, float flMin, float flMax, CGroupBox *Group,
                                CTab *Tab, std::string szFileId)
{
    if (szCaption.empty() || !Tab || !Group)
        return nullptr;

    CSlider *slider = new CSlider;
    slider->SetVarCb(flVar);
    slider->SetBoundaries(flMin, flMax);
    slider->SetFileId(szFileId);
    Group->PlaceLabledControl(szCaption, Tab, slider);
    return slider;
}

CComboBox *CMenuWindow::AddCombo(std::string szCaption, int *iVar, std::vector<std::string> vItems, CGroupBox *Group,
                                 CTab *Tab, std::string szFileId)
{
    if (szCaption.empty() || !Tab || !Group)
        return nullptr;

    CComboBox *combo = new CComboBox;
    combo->SetFileId(szFileId);
    combo->SetVarCb(iVar);

    for (auto &item : vItems)
        combo->AddItem(item);

    Group->PlaceLabledControl(szCaption, Tab, combo);
    return combo;
}

CKeyBind *CMenuWindow::AddKeybind(std::string szCaption, int *iVar, CGroupBox *Group, CTab *Tab, std::string szFileId)
{
    if (szCaption.empty() || !Tab || !Group)
        return nullptr;

    CKeyBind *key = new CKeyBind;
    key->SetFileId(szFileId);
    key->SetVarCb(iVar);
    Group->PlaceLabledControl(szCaption, Tab, key);
    return key;
}

CButton *CMenuWindow::AddButton(std::string szCaption, CGroupBox *Group, CTab *Tab, void (*Callback)())
{
    if (szCaption.empty() || !Tab || !Group)
        return nullptr;

    CButton *btn = new CButton;
    btn->SetText(szCaption);
    btn->SetCallback(Callback);
    Group->PlaceLabledControl(szCaption, Tab, btn);
    return btn;
}

using namespace Options;

void CMenuWindow::Setup()
{
    SetPosition(50, 50);
    SetSize(g_windowWidth, g_windowHeight);
    SetTitle(g_menuTitle);

    //
    // Aimbot
    //
    {
        auto AimbotTab = this->AddTab("Aimbot");
        if (AimbotTab)
        {
            AddLabel("Enabled", AimbotTab, 16, 16);
            AddCheckbox({}, &Aimbot::Enabled, nullptr, AimbotTab, "active", 76, 16);

            auto Group = AddGroup("Aimbot", AimbotTab, 16, 43);
            if (Group)
            {
                AddCheckbox("Auto-Aim", &Aimbot::AutoAim, Group, AimbotTab, "aim_autoaim");
                AddCheckbox("Auto-Fire", &Aimbot::AutoFire, Group, AimbotTab, "aim_autofire");
                AddCheckbox("Auto-Wall", &Aimbot::AutoWall, Group, AimbotTab, "aim_autowall");
                AddCheckbox("Prediction", &Aimbot::Prediction, Group, AimbotTab, "aim_prediction");
                AddCheckbox("Silent-Aim", &Aimbot::SilentAim, Group, AimbotTab, "aim_silent");
                AddCheckbox("Visible Check", &Aimbot::VisibleCheck, Group, AimbotTab, "aim_vischeck");
                AddSlider("FOV Threshold", &Aimbot::FOV, 0.0f, 180.f, Group, AimbotTab, "aim_fov");
                AddCombo("Aim Style", &Aimbot::Style, {"FOV", "Distance"}, Group, AimbotTab, "aim_style");
                AddCombo("Aim Bone", &Aimbot::Bone, {"Head", "Chest", "Stomach"}, Group, AimbotTab, "aim_bone");
            }

            auto Group2 = AddGroup("Others", AimbotTab, 280, 43);
            if (Group2)
            {
                AddCheckbox("Aim on Friendly", &Aimbot::AimOnFriendly, Group2, AimbotTab, "aim_friendly");
                AddCheckbox("Draw Locked", &Aimbot::DrawLocked, Group2, AimbotTab, "aim_drawlocked");
                AddCheckbox("Use Aim Key", &Aimbot::UseAimKey, Group2, AimbotTab, "aim_use_aimkey");
                AddKeybind("Aim Key", &Aimbot::Key, Group2, AimbotTab, "aim_aimkey");
            }
        }
    }

    //
    // Visuals
    //
    {
        auto VisualTab = this->AddTab("Visuals");
        if (VisualTab)
        {
            AddLabel("Enabled", VisualTab, 16, 16);
            AddCheckbox({}, &Visual::Enabled, nullptr, VisualTab, "active", 76, 16);

            auto Group = AddGroup("ESP", VisualTab, 16, 43);
            if (Group)
            {
                AddCombo("Box", &Visual::ESPBoxes, {"None", "2D", "Edge", "3D"}, Group, VisualTab, "esp_box");
                AddCheckbox("Draw Lines", &Visual::ESPLines, Group, VisualTab, "esp_lines");
                AddCheckbox("Draw Names", &Visual::ESPNames, Group, VisualTab, "esp_names");
                AddCheckbox("Draw Bones", &Visual::ESPBones, Group, VisualTab, "esp_bones");
                AddCheckbox("Draw Barrel", &Visual::ESPBarrel, Group, VisualTab, "esp_barrel");
                AddCheckbox("Draw Weapon", &Visual::ESPWeapon, Group, VisualTab, "esp_weapon");
            }

            auto Group2 = AddGroup("Others", VisualTab, 280, 43);
            if (Group)
            {
                AddCheckbox("Draw Friendly", &Visual::ESPDrawFriend, Group2, VisualTab, "esp_draw_friendly");
                AddCheckbox("Draw Enemy", &Visual::ESPDrawEnemy, Group2, VisualTab, "esp_draw_enemy");
            }
        }
    }

    //
    // Misc
    //
    {
        auto MiscTab = this->AddTab("Misc");
        if (MiscTab)
        {
            auto Group = AddGroup("Other", MiscTab, 16, 16, 530, 293);
            if (Group)
            {
                AddCombo("2D CrossHair", &Misc::CHair, {"None", "Cross", "Dot", "T-Cross"}, Group, MiscTab,
                         "misc_2dchair");
                AddSlider("CrossHair Size", &Misc::CHairSize, 0.0f, 150.f, Group, MiscTab, "misc_2dchair_size");
                AddCheckbox("Draw FPS", &Misc::DrawFPS, Group, MiscTab, "misc_drawfps");
                AddCheckbox("No Spread", &Misc::NoSpread, Group, MiscTab, "misc_nospread");
                AddCheckbox("No Recoil", &Misc::NoRecoil, Group, MiscTab, "misc_norecoil");
                AddCheckbox("Unlock Dvars", &Misc::UnlockDvar, Group, MiscTab, "misc_unlockdvar");
                AddCheckbox("Use Panic Key", &Misc::UsePanic, Group, MiscTab, "misc_usepanic");
                AddKeybind("Panic Key", &Misc::PanicKey, Group, MiscTab, "misc_panickey");
            }
        }
    }

    auto Save = []() { GUI.SaveWindowState(&Menu::Window, g_configFile); };
    auto Load = []() { GUI.LoadWindowState(&Menu::Window, g_configFile); };

    //
    // Settings
    //
    {
        auto SettingsTab = this->AddTab("Settings");
        if (SettingsTab)
        {
            auto Group = AddGroup("Configuration", SettingsTab, 16, 16, 530, 293);
            if (SettingsTab)
            {
                AddButton("Save Configuration", Group, SettingsTab, Save);
                AddButton("Load Configuration", Group, SettingsTab, Load);
            }
        }
    }
}

void Menu::SetupMenu(HWND hWnd)
{
    Window.Setup();

    GUI.SetHwnd(hWnd);
    GUI.RegisterWindow(&Window);
    GUI.BindWindow(VK_INSERT, &Window);
}

void Menu::DoUIFrame()
{
    GUI.Update();
    GUI.Draw();
}
