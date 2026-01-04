#pragma once

#include "Game/LiveActor/Nerve.hpp"
#include "Game/Screen/LayoutActor.hpp"
#include "JSystem/JGeometry.hpp"

class CollectCounter : public LayoutActor {
public:
    CollectCounter(const char*);

    virtual ~CollectCounter();
    virtual void init(const JMapInfoIter&);
    virtual void control();

    void exeHide();
    void exeShow();

    void setCount(s32);
    bool tryEndShow();
    void setPosition();

    TVec2f mFollowPosition;  // 0x20
};
