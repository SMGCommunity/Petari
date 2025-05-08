#pragma once

#include "Game/Screen/LayoutActor.hpp"

class IconAButton;

class InformationMessage : LayoutActor {
public:
    InformationMessage();

    virtual ~InformationMessage();
    virtual void init(const JMapInfoIter& rIter);
    virtual void appear();

    void appearWithButtonLayout();
    void disappear();
    void setMessage(const char* pMessage);
    void setMessage(const wchar_t* pMessage);
    void setReplaceString(const wchar_t* pString, s32 param2);
    void exeAppear();
    void exeWait();
    void exeDisappear();

private:
    /* 0x20 */ IconAButton* mAButtonIcon;
    /* 0x24 */ bool mIsCenterPane;
};
