#pragma once

#include "Game/NameObj/NameObj.h"
#include "Game/MapObj/WaterPressureBullet.h"

class WaterPressureBulletHolder : public NameObj {
public:
    WaterPressureBulletHolder(const char *);

    virtual ~WaterPressureBulletHolder();
    virtual void init(const JMapInfoIter &);

    WaterPressureBullet* callEmptyBullet();

    WaterPressureBullet* mBullets[0x10];    // _C
};