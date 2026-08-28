#pragma once

#include "Game/NameObj/NameObj.hpp"

class WaterPressureBullet;

class WaterPressureBulletHolder : public NameObj {
public:
    /// @brief Creates a new `WaterPressureBulletHolder`.
    /// @param pName A pointer to the null-terminated name of the object.
    WaterPressureBulletHolder(const char* pName);

    /* 0x08 */ virtual ~WaterPressureBulletHolder();
    /* 0x0C */ virtual void init(const JMapInfoIter& rIter);

    WaterPressureBullet* callEmptyBullet();

    /* 0x0C */ WaterPressureBullet* mBullet[16];
};
