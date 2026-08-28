#pragma once

#include "Game/LiveActor/Nerve.hpp"
#include "Game/Screen/LayoutActor.hpp"
#include "JSystem/JGeometry.hpp"

class CollectCounter : public LayoutActor {
public:
    CollectCounter(const char*);

    /* 0x08 */ virtual ~CollectCounter();
    /* 0x0C */ virtual void init(const JMapInfoIter&);
    /* 0x2C */ virtual void control();

    void exeHide();
    void exeShow();

    void setCount(s32);
    bool tryEndShow();
    void setPosition();

    TVec2f mFollowPosition;  // 0x20
};
