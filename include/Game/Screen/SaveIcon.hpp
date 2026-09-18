#pragma once

#include "Game/Screen/LayoutActor.hpp"

class SaveIcon : public LayoutActor {
public:
    SaveIcon(const LayoutActor* pActor);

    virtual void calcAnim();
    virtual void appear();

public:
    /* 0x20 */ const LayoutActor* mActor;
};
