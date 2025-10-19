#pragma once

#include "Game/Screen/LayoutActor.hpp"

class HomeButtonLayout : public LayoutActor {
public:
    HomeButtonLayout();

    bool isActive() const;

    u32 _20;
    u8 _24;
    u8 _25;
};
