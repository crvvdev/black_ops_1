#pragma once

class CCheckBox : public CControl
{
  private:
    // bool* bVar;
  public:
    CCheckBox();
    void SetVarCb(bool *var);
    void SetState(bool s);
    bool GetState();

  protected:
    bool *Checked;
    void Draw(bool hover);
    void OnUpdate();
    void OnClick();
};

class CLabel : public CControl
{
  public:
    CLabel();
    void SetText(std::string text);

  protected:
    std::string Text;
    void Draw(bool hover);
    void OnUpdate();
    void OnClick();
};

class CGroupBox : public CControl
{
  public:
    CGroupBox();
    void SetText(std::string text);
    void PlaceLabledControl(std::string Label, CTab *Tab, CControl *control);

  protected:
    int Items;
    std::string Text;
    void Draw(bool hover);
    void OnUpdate();
    void OnClick();
};

class CSlider : public CControl
{
  public:
    CSlider();
    float GetValue();
    void SetVarCb(float *var);
    void SetValue(float v);
    void SetBoundaries(float min, float max);

  protected:
    float *Value;
    float Min;
    float Max;
    bool DoDrag;
    void Draw(bool hover);
    void OnUpdate();
    void OnClick();
};

class CKeyBind : public CControl
{
  public:
    CKeyBind();
    void SetVarCb(int *var);
    int GetKey();
    void SetKey(int key);

  protected:
    int *Key;
    bool IsGettingKey;
    void Draw(bool hover);
    void OnUpdate();
    void OnClick();
};

class CButton : public CControl
{
  public:
    typedef void (*ButtonCallback_t)(void);
    CButton();
    void SetCallback(ButtonCallback_t callback);
    void SetText(std::string text);

  protected:
    ButtonCallback_t CallBack;
    std::string Text;
    void Draw(bool hover);
    void OnUpdate();
    void OnClick();
};

class CComboBox : public CControl
{
  public:
    CComboBox();
    void AddItem(std::string text);
    void SetVarCb(int *var);
    void SelectIndex(int idx);
    int GetIndex();
    std::string GetItem();

  protected:
    std::vector<std::string> Items;
    int *SelectedIndex;
    bool IsOpen;
    void Draw(bool hover);
    void OnUpdate();
    void OnClick();
};
