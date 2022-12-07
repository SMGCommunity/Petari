#pragma once

#include "Game/NameObj/NameObj.h"
#include "Game/MapObj/FirePressureBullet.h"

class FirePressureBulletHolder : public NameObj {
public:
    FirePressureBulletHolder(const char *);

    virtual ~FirePressureBulletHolder();
    virtual void init(const JMapInfoIter &);

    FirePressureBullet* callEmptyBullet();

    FirePressureBullet* mBullets[0x8];  // _C
};