#pragma once

#include "Game/Screen/LayoutActor.hpp"

class GameSystemResetAndPowerProcess : public LayoutActor {
public:
    GameSystemResetAndPowerProcess();
    ~GameSystemResetAndPowerProcess();

    bool isActive() const;

    u32 _20;
    u32 _24;
    u8 _28[0x30]; // placeholder
    u32 _58;
    u8 _5C;
    u8 _5D;
    u8 _5E;
};
