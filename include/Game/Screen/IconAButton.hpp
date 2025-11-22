#pragma once

#include "Game/Screen/LayoutActor.hpp"

class IconAButton : public LayoutActor {
public:
    IconAButton(bool, bool);

    virtual void init(const JMapInfoIter&);
    virtual void control();

    void setFollowActorPane(LayoutActor*, const char*);
    bool isOpen();
    bool isWait();
    void openWithTalk();
    void openWithRead();
    void openWithTurn();
    void openWithoutMessage();
    void term();
    void exeOpen();
    void exeWait();
    void exeTerm();
    void updateFollowPos();

    /* 0x20 */ TVec2f mFollowPos;
    /* 0x28 */ LayoutActor* mFollowActor;
    /* 0x2C */ char mFollowPaneName[24];
};
