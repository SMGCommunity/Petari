#pragma once

#include "Game/MapObj/PressureBase.hpp"

class FirePressure : public PressureBase {
public:
    FirePressure(const char*);

    /* 0x08 */ virtual ~FirePressure();
    /* 0x0C */ virtual void init(const JMapInfoIter&);
    /* 0x78 */ virtual void initBullet(const JMapInfoIter&);
    /* 0x7C */ virtual bool shotBullet(f32);

    void calcGunPointFromCannon(TPos3f*);

    bool mDisableCollsion;  // 0x8D
};