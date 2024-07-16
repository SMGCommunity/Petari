#pragma once

#include "Game/LiveActor/Nerve.hpp"
#include "Game/Screen/LayoutActor.hpp"
#include "Game/Screen/YesNoController.hpp"
#include "Game/Screen/IconAButton.hpp"

typedef void (*MessageChangeFuncPtr)(LayoutActor*, const char*, const char*);

struct MessageChangeFuncTable {
    MessageChangeFuncPtr mFuncPtr;
};

class SysInfoWindow : public LayoutActor {
    public:
    enum SysInfoWindowType {
        WINDOWTYPE_0,
        WINDOWTYPE_1
    };

    enum SysInfoExecuteType {
        EXECUTETYPE_0,
        EXECUTETYPE_1
    };

    enum SysInfoType {
        INFOTYPE_0,
        INFOTYPE_1,
        INFOTYPE_2
    };

    enum SysInfoTextPos {
        TEXTPOS_0,
        TEXTPOS_1
    };

    enum SysInfoMessageType {
        MESSAGETYPE_0,
        MESSAGETYPE_1
    };

    SysInfoWindow(SysInfoWindowType, SysInfoExecuteType);
    virtual ~SysInfoWindow();
    virtual void init(const JMapInfoIter& rIter);
    virtual void movement();
    virtual void calcAnim();
    virtual void draw() const;
    virtual void appear();
    void appear(const char*, SysInfoType, SysInfoTextPos, SysInfoMessageType);
    virtual void disappear();
    virtual void kill();
    void forceKill();
    bool isWait() const;
    bool isSelectedYes() const;
    bool isDisappear() const;
    void control();
    const char* getLayoutName() const;
    void exeWait();
    void exeDisappear();
    void setYesNoSelectorSE(const char*,const char*,const char*);
    void resetYesNoSelectorSE();
    void setTextBoxArgNumber(s32, s32);
    void setTextBoxArgString(const wchar_t*, s32);
    
    s32 _20;
    s32 _24;
    YesNoController* _28;
    IconAButton* _2C;
    const char* _30;
    const char* _34;
    bool _38;
};

namespace NrvSysInfoWindow {
    NERVE(SysInfoWindowNrvAppear);
    NERVE_DECL(SysInfoWindowNrvWait, SysInfoWindow, SysInfoWindow::exeWait);
    NERVE_DECL(SysInfoWindowNrvDisappear, SysInfoWindow, SysInfoWindow::exeDisappear);
};

namespace MR {
    SysInfoWindow* createSysInfoWindow();
    SysInfoWindow* createSysInfoWindowExecuteWithChildren();
    SysInfoWindow* createSysInfoWindowMiniExecuteWithChildren();
};