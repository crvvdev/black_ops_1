#pragma once

// class CRageBotTab : public CTab
//{
// public:
//	void Setup();
//
//	CLabel		ActiveLabel;
//	CCheckBox	Active;
//
//	CCheckBox	Item [ 99 ];
//	CComboBox	CItem[ 99 ];
//	CSlider		SItem[ 99 ];
//
//	CGroupBox	Aimbot[ 2 ];
// };
//
// class CVisualTab : public CTab
//{
// public:
//	void Setup();
//
//	CCheckBox Dummy[ 99 ];
//	CSlider SDummy[ 99 ];
// };
//
// class CMiscTab : public CTab
//{
// public:
//	void Setup();
//
//	CCheckBox Dummy[ 99 ];
//	CSlider SDummy[ 99 ];
//	CKeyBind KDummy[ 99 ];
// };
//
// class CColorTab : public CTab
//{
// public:
//	void Setup();
//
//	// Groups
//	CGroupBox TNVisGroup;
//	CGroupBox TVisGroup;
//
//	// T Not Visible
//	CSlider  TNotVis[ 3 ];
//
//	// T Visible
//	CSlider  TVisible[ 3 ];
// };
//
// class CSettingsTab : public CTab
//{
// public:
//	void Setup();
//
//	CGroupBox ButtonGroup;
//	CButton SaveButton;
//	CButton LoadButton;
// };

class CMenuWindow : public CWindow
{
  private:
    CTab *AddTab(std::string szCaption);
    CGroupBox *AddGroup(std::string szCaption, CTab *Tab, int x = 0, int y = 0, int w = 260, int h = 265);
    CLabel *AddLabel(std::string szCaption, CTab *Tab, int x = 0, int y = 0);

    CCheckBox *AddCheckbox(std::string szCaption, bool *bVar, CGroupBox *Group, CTab *Tab, std::string szFileId = {},
                           int x = 0, int y = 0);
    CSlider *AddSlider(std::string szCaption, float *flVar, float flMin, float flMax, CGroupBox *Group, CTab *Tab,
                       std::string szFileId = {});
    CComboBox *AddCombo(std::string szCaption, int *iVar, std::vector<std::string> vItems, CGroupBox *Group, CTab *Tab,
                        std::string szFileId = {});
    CKeyBind *AddKeybind(std::string szCaption, int *iVar, CGroupBox *Group, CTab *Tab, std::string szFileId = {});
    CButton *AddButton(std::string szCaption, CGroupBox *Group, CTab *Tab, void (*Callback)());

  public:
    CMenuWindow() = default;
    CMenuWindow(bool showMenu = false)
    {
        if (showMenu)
        {
            this->Open();
        }
    }

    void Setup();
};

namespace Menu
{
void SetupMenu(HWND hWnd);
void DoUIFrame();

extern bool MenuCreated;
extern CMenuWindow Window;
}; // namespace Menu
