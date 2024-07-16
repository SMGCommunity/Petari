#pragma once

#include "Game/Screen/LayoutActor.hpp"
#include "Game/Screen/ButtonPaneController.hpp"
#include "Inline.hpp"
class SysInfoWindow;
class LuigiLetter;

namespace {
    bool isInvalidBackAstroDome() NO_INLINE;
};

class PauseMenu : public LayoutActor {
public:
    PauseMenu();
    virtual ~PauseMenu();
    virtual void init(const JMapInfoIter& rIter);
    virtual void appear();
    virtual void draw() const;
    virtual void kill();
    virtual void control();
    void updateStarPane();
    void startPaneAnimWithoutButton(const char*);
    bool isPaneAnimStoppedWithoutButton() const;
    void forceToWaitAllButton();
    void exeSelecting();
    void exeDecided();
    void exeDisappear();
    void exeConfirm();
    void exeGameDataSave();
    void exeLuigiLetter();

    ButtonPaneController* _20;
    ButtonPaneController* _24;
    TVec2f _28;
    SysInfoWindow* _30;
    LuigiLetter* _34;
    ButtonPaneController* _38;
    TVec2f _3C;
    TVec2f _44;
    TVec2f _4C;
    bool _54;
};