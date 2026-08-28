#pragma once

#include "Game/Screen/LayoutActor.hpp"

class SaveIcon : public LayoutActor {
public:
    SaveIcon(const LayoutActor* pActor);

    /* 0x1C */ virtual void calcAnim();
    /* 0x24 */ virtual void appear();

private:
    /* 0x20 */ const LayoutActor* mActor;
};
