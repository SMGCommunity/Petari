#pragma once

#include "Game/Screen/LayoutActor.hpp"

class SaveIcon : LayoutActor {
public:
    SaveIcon(const LayoutActor* pActor);

    virtual ~SaveIcon();
    virtual void calcAnim();
    virtual void appear();

private:
    /* 0x20 */ const LayoutActor* mActor;
};
