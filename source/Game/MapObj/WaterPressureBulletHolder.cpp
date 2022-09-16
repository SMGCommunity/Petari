#include "Game/MapObj/WaterPressureBulletHolder.h"

WaterPressureBulletHolder::WaterPressureBulletHolder(const char *pName) : NameObj(pName) {
    
}

WaterPressureBullet* WaterPressureBulletHolder::callEmptyBullet() {
    for (u32 i = 0; i < 0x10; i++) {
        if (MR::isDead(mBullets[i])) {
            return mBullets[i];
        }
    }

    return NULL;
}

WaterPressureBulletHolder::~WaterPressureBulletHolder() {
    
}

void WaterPressureBulletHolder::init(const JMapInfoIter &rIter) {
    for (u32 i = 0; i < 0x10; i++) {
        mBullets[i] = new WaterPressureBullet("ウォータープレッシャーの弾");
        mBullets[i]->initWithoutIter();
    }
}