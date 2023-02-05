#pragma once

#include "Game/Screen/LayoutActor.h"

class IconAButton : public LayoutActor {
public:
    IconAButton(bool, bool);

    virtual ~IconAButton();
    virtual void init(const JMapInfoIter &);
    virtual void control();

    void setFollowActorPane(LayoutActor *, const char *);
    bool isOpen();
    bool isWait();
    void openWithTalk();
    void openWithRead();
    void openWithTurn();
    void openWithoutMessage();
    void term();
    void exeOpen();
    void exeTerm();
    
    inline void exeWait() {
        if (MR::isFirstStep(this)) {
            MR::startAnim(this, "Press", 0);
        }
    }

    void updateFollowPos();

    TVec2f mFollowPos;              // _20
    LayoutActor* mFollowActor;      // _28
    char mFollowPane[0x18];         // _2C
};

namespace NrvIconAButton {
    NERVE_DECL(IconAButtonNrvOpen, IconAButton, IconAButton::exeOpen);
    NERVE_DECL(IconAButtonNrvWait, IconAButton, IconAButton::exeWait);
    NERVE_DECL(IconAButtonNrvTerm, IconAButton, IconAButton::exeTerm);
};