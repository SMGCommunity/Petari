#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class GroundChecker : public LiveActor {
public:
    GroundChecker(const char*, f32, f32);

    virtual ~GroundChecker();

    void bringFriction(f32, f32);

    f32 _8C;
    f32 _90;
};