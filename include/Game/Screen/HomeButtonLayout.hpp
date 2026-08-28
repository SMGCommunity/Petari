#pragma once

#include "Game/Screen/LayoutActor.hpp"
#include <revolution/hbm.h>

class HomeButtonMenuContext {
public:
    /// @brief Creates a new `HomeButtonMenuContext`.
    HomeButtonMenuContext();

    void initControllerData();
    void initHomeButtonInfo();

    /* 0x00 */ HBMDataInfo* mHomeButtonInfo;
    /* 0x04 */ HBMControllerData* mControllerData;
};

class HomeButtonLayout : public LayoutActor {
public:
    /// @brief Creates a new `HomeButtonLayout`.
    HomeButtonLayout();

    /* 0x0C */ virtual void init(const JMapInfoIter& rIter);
    /* 0x14 */ virtual void movement();
    /* 0x18 */ virtual void draw() const;

    void forceToDeactive();
    bool isActive() const;
    void updateController();
    bool tryCorePadTriggerHome();

    void exeDeactive();
    void exeActive();

    /* 0x20 */ HomeButtonMenuContext* mMenuContext;
    /* 0x24 */ bool _24;
    /* 0x25 */ bool _25;
};
