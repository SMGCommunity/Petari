#pragma once

class PowerStarHolder;

class EventFunction {
public:
    static PowerStarHolder* getPowerStarHolder();
};

namespace MR {
    void initEventSystemAfterPlacement();
};
