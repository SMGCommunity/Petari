#pragma once

#include "Game/Screen/LayoutActor.hpp"

struct HBMControllerData;
struct HBMDataInfo;

class HomeButtonMenuContext {
public:
    /// @brief Creates a new `HomeButtonMenuContext`.
    HomeButtonMenuContext();

    void initControllerData();
    void initHomeButtonInfo();

private:
    /* 0x00 */ HBMDataInfo* mHomeButtonInfo;
    /* 0x04 */ HBMControllerData* mControllerData;
};

class HomeButtonLayout : public LayoutActor {
public:
    /// @brief Creates a new `HomeButtonLayout`.
    HomeButtonLayout();

    virtual void init(const JMapInfoIter& rIter);
    virtual void movement();
    virtual void draw() const;

    void forceToDeactive();
    bool isActive() const;
    void updateController();
    bool tryCorePadTriggerHome();
    void exeDeactive();
    void exeActive();

private:
    /* 0x20 */ HomeButtonMenuContext* mMenuContext;
    /* 0x24 */ bool _24;
    /* 0x25 */ bool _25;
};
