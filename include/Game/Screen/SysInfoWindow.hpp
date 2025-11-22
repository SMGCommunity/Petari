#pragma once

#include "Game/Screen/LayoutActor.hpp"

typedef void(MessageChangeFunc)(LayoutActor*, const char*, const char*);

class IconAButton;
class YesNoController;

struct MessageChangeFuncTableEntry {
    MessageChangeFunc* mFuncPtr;
};

class SysInfoWindow : public LayoutActor {
public:
    enum SysInfoWindowType {
        WindowType_Normal,
        WindowType_Mini,
    };

    enum SysInfoExecuteType {
        ExecuteType_Normal,
        ExecuteType_Children,
    };

    enum SysInfoType {
        Type_Key,
        Type_Blocking,
        Type_YesNo,
    };

    enum SysInfoTextPos {
        TextPos_Center,
        TextPos_Bottom,
    };

    enum SysInfoMessageType {
        MessageType_Game,
        MessageType_System,
    };

    SysInfoWindow(SysInfoWindowType, SysInfoExecuteType);

    virtual void init(const JMapInfoIter& rIter);
    virtual void movement();
    virtual void draw() const;
    virtual void calcAnim();
    virtual void appear();
    virtual void kill();
    virtual void control();

    void appear(const char*, SysInfoType, SysInfoTextPos, SysInfoMessageType);
    void disappear();
    void forceKill();
    bool isWait() const;
    bool isSelectedYes() const;
    bool isDisappear() const;
    const char* getLayoutName() const;
    void exeAppear();
    void exeWait();
    void exeDisappear();
    void setYesNoSelectorSE(const char*, const char*, const char*);
    void resetYesNoSelectorSE();
    void setTextBoxArgNumber(s32, s32);
    void setTextBoxArgString(const wchar_t*, s32);

    /* 0x20 */ SysInfoWindowType mWindowType;
    /* 0x24 */ SysInfoType mType;
    /* 0x28 */ YesNoController* mYesNoSelector;
    /* 0x2C */ IconAButton* mIconAButton;
    /* 0x30 */ const char* mTextParentPaneName;
    /* 0x34 */ const char* mWindowParentPaneName;
    /* 0x38 */ bool _38;
};

namespace MR {
    SysInfoWindow* createSysInfoWindow();
    SysInfoWindow* createSysInfoWindowExecuteWithChildren();
    SysInfoWindow* createSysInfoWindowMiniExecuteWithChildren();
};  // namespace MR
