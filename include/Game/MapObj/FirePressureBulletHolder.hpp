#pragma once

#include "Game/NameObj/NameObj.hpp"

class FirePressureBullet;

class FirePressureBulletHolder : public NameObj {
public:
    /// @brief Creates a new `FirePressureBulletHolder`.
    /// @param pName A pointer to the null-terminated name of the object.
    FirePressureBulletHolder(const char* pName);

    virtual ~FirePressureBulletHolder();
    virtual void init(const JMapInfoIter& rIter);

    FirePressureBullet* callEmptyBullet();

    /* 0x0C */ FirePressureBullet* mBullet[8];
};
