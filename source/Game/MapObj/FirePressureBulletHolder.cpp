#include "Game/MapObj/FirePressureBulletHolder.h"

FirePressureBulletHolder::FirePressureBulletHolder(const char *pName) : NameObj(pName) {

}

FirePressureBullet* FirePressureBulletHolder::callEmptyBullet() {
    for (u32 i = 0; i < 8; i++) {
        if (MR::isDead(mBullets[i])) {
            return mBullets[i];
        }
    }

    return nullptr;
}

FirePressureBulletHolder::~FirePressureBulletHolder() {

}

void FirePressureBulletHolder::init(const JMapInfoIter &rIter) {
    for (u32 i = 0; i < 8; i++) {
        mBullets[i] = new FirePressureBullet("ファイアプレッシャーの弾");
        mBullets[i]->initWithoutIter();
    }
}