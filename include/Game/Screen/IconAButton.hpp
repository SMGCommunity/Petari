#pragma once

#include "Game/LiveActor/Nerve.hpp"
#include "Game/Screen/LayoutActor.hpp"
#include "Game/Util/LayoutUtil.hpp"

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

    TVec2f mFollowPos;              // 0x20
    LayoutActor* mFollowActor;      // 0x28
    char mFollowPane[0x18];         // 0x2C
};

namespace NrvIconAButton {
    NERVE_DECL(IconAButtonNrvOpen, IconAButton, IconAButton::exeOpen);
    NERVE_DECL(IconAButtonNrvWait, IconAButton, IconAButton::exeWait);
    NERVE_DECL(IconAButtonNrvTerm, IconAButton, IconAButton::exeTerm);
};