#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class GroundChecker : public LiveActor {
public:
    GroundChecker(const char*, f32, f32);

    void bringFriction(f32, f32);

    /* 0x8C */ f32 _8C;
    /* 0x90 */ f32 _90;
};
