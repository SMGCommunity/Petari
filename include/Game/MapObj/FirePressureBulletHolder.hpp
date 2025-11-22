#pragma once

#include "Game/MapObj/FirePressureBullet.hpp"
#include "Game/NameObj/NameObj.hpp"

class FirePressureBulletHolder : public NameObj {
public:
    FirePressureBulletHolder(const char*);

    virtual ~FirePressureBulletHolder();
    virtual void init(const JMapInfoIter&);

    FirePressureBullet* callEmptyBullet();

    FirePressureBullet* mBullets[0x8];  // 0xC
};