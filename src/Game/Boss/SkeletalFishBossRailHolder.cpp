#include "Game/Boss/SkeletalFishBossRailHolder.hpp"
#include "Game/Boss/SkeletalFishBossRail.hpp"

SkeletalFishBossRailHolder::SkeletalFishBossRailHolder(const char *pName) : NameObj(pName) {
    mNumRails = 0;

    for (s32 i = 0; i < 3; i++) {
        mRails[i] = nullptr;
    }
}

void SkeletalFishBossRailHolder::add(SkeletalFishBossRail *pRail) {
    mRails[mNumRails++] = pRail;
}

SkeletalFishBossRail* SkeletalFishBossRailHolder::getInOrder(s32 idx) const {
    return mRails[idx];
}

SkeletalFishBossRail* SkeletalFishBossRailHolder::getByID(s32 id) const {
    for (u32 i = 0; i < mNumRails; i++) {
        SkeletalFishBossRail* rail = getInOrder(i);

        if (id == rail->_8C) {
            return rail;
        }
    }

    return mRails[0];
}

namespace MR {
    SkeletalFishBossRailHolder* getSkeletalFishBossRailHolder() {
        return MR::getSceneObj<SkeletalFishBossRailHolder*>(SceneObj_SkeletalFishRailHolder);
    }

    void createSkeletalFishBossRailHolder() {
        MR::createSceneObj(SceneObj_SkeletalFishRailHolder);
    }
};

SkeletalFishBossRailHolder::~SkeletalFishBossRailHolder() {

}
