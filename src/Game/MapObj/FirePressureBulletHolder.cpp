#include "Game/MapObj/FirePressureBulletHolder.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/MapObj/FirePressureBullet.hpp"
#include "Game/Util.hpp"
#include "Game/Util/LiveActorUtil.hpp"

FirePressureBulletHolder::FirePressureBulletHolder(const char* pName) : NameObj(pName) {
}

FirePressureBullet* FirePressureBulletHolder::callEmptyBullet() {
    for (u32 i = 0; i < ARRAY_SIZE(mBullet); i++) {
        if (MR::isDead(mBullet[i])) {
            return mBullet[i];
        }
    }

    return nullptr;
}

FirePressureBulletHolder::~FirePressureBulletHolder() {
}

void FirePressureBulletHolder::init(const JMapInfoIter& rIter) {
    for (u32 i = 0; i < ARRAY_SIZE(mBullet); i++) {
        mBullet[i] = new FirePressureBullet("ファイアプレッシャーの弾");
        mBullet[i]->initWithoutIter();
    }
}
