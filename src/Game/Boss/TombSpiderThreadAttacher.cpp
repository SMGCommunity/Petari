#include "Game/Boss/TombSpiderThreadAttacher.hpp"
#include "Game/MapObj/SpiderThread.hpp"
#include "Game/Util/JMapInfo.hpp"
#include "Game/Util/JointUtil.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/ObjUtil.hpp"

namespace NrvTombSpiderThreadAttacher {
    NEW_NERVE(TombSpiderThreadAttacherNrvFree, TombSpiderThreadAttacher, Free);
    NEW_NERVE(TombSpiderThreadAttacherNrvAttach, TombSpiderThreadAttacher, Attach);
}  // namespace NrvTombSpiderThreadAttacher

TombSpiderThreadAttacher::TombSpiderThreadAttacher(LiveActor* pActor, const char* pJointName, f32 radius, f32 offset)
    : LiveActor("巣の付着点"), mJointMtx(nullptr), mRadius(radius), mZOffset(offset) {
    mJointMtx = MR::getJointMtx(pActor, pJointName);
}

void TombSpiderThreadAttacher::init(const JMapInfoIter& rIter) {
    MR::connectToSceneEnemyMovement(this);
    initNerve(&NrvTombSpiderThreadAttacher::TombSpiderThreadAttacherNrvFree::sInstance);
    MR::invalidateClipping(this);
    makeActorDead();
}

void TombSpiderThreadAttacher::exeFree() {
    TVec3f pos(mJointMtx[0][3], mJointMtx[1][3], mJointMtx[2][3] - mZOffset);
    MR::tryPushSpiderThread(pos, mRadius);
}

void TombSpiderThreadAttacher::exeAttach() {}
