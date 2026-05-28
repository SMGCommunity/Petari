#include "Game/MapObj/WaterPressureBulletHolder.hpp"

WaterPressureBulletHolder::WaterPressureBulletHolder(const char* pName) : NameObj(pName) {
}

WaterPressureBullet* WaterPressureBulletHolder::callEmptyBullet() {
    for (u32 i = 0; i < ARRAY_SIZE(mBullets); i++) {
        if (MR::isDead(mBullets[i])) {
            return mBullets[i];
        }
    }

    return nullptr;
}

WaterPressureBulletHolder::~WaterPressureBulletHolder() {
}

void WaterPressureBulletHolder::init(const JMapInfoIter& rIter) {
    for (u32 i = 0; i < ARRAY_SIZE(mBullets); i++) {
        mBullets[i] = new WaterPressureBullet("ウォータープレッシャーの弾");
        mBullets[i]->initWithoutIter();
    }
}