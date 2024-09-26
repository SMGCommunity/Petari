#pragma once

#include "Game/NameObj/NameObj.hpp"
#include "Game/MapObj/WaterPressureBullet.hpp"

class WaterPressureBulletHolder : public NameObj {
public:
    WaterPressureBulletHolder(const char *);

    virtual ~WaterPressureBulletHolder();
    virtual void init(const JMapInfoIter &);

    WaterPressureBullet* callEmptyBullet();

    WaterPressureBullet* mBullets[0x10];    // 0xC
};