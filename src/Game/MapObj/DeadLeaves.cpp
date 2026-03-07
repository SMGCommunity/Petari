#include "Game/MapObj/DeadLeaves.hpp"
#include "Game/MapObj/MapObjActorInitInfo.hpp"

namespace NrvDeadLeaves {
    NEW_NERVE(DeadLeavesNrvWait, DeadLeaves, Wait);
    NEW_NERVE(DeadLeavesNrvSpin, DeadLeaves, Spin);
};  // namespace NrvDeadLeaves

DeadLeaves::DeadLeaves(const char* pName) : MapObjActor(pName) {
    mItemType = -1;
}

void DeadLeaves::init(const JMapInfoIter& rIter) {
    MapObjActor::init(rIter);
    MapObjActorInitInfo info;
    MapObjActorUtil::setupInitInfoSimpleMapObj(&info);
    info.setupHitSensor();
    TVec3f sensor;
    sensor.x = 0.0f;
    sensor.y = 30.0f;
    sensor.z = 0.0f;
    info.setupHitSensorParam(8, 70.0f, sensor);
    info.setupNerve(&NrvDeadLeaves::DeadLeavesNrvWait::sInstance);
    initialize(rIter, info);
    MR::getJMapInfoArg0NoInit(rIter, &mItemType);

    if (mItemType == 0) {
        MR::declareCoin(this, 1);
    } else if (mItemType == 1) {
        MR::declareStarPiece(this, 3);
    }
}

void DeadLeaves::exeWait() {}

void DeadLeaves::exeSpin() {
    if (MR::isFirstStep(this)) {
        MR::startAllAnim(this, "Spin");
        MR::startSound(this, "SE_OJ_LEAVES_SWING", -1, -1);

        if (mItemType == 0) {
            MR::appearCoinPop(this, mPosition, 1);
        } else if (mItemType == 1) {
            bool appear = MR::appearStarPiece(this, mPosition, 3, 10.0f, 40.0f, false);
            if (appear) {
                MR::startSound(this, "SE_OJ_STAR_PIECE_BURST", -1, -1);
            }
        }
    }

    if (MR::isBckStopped(this)) {
        setNerve(&NrvDeadLeaves::DeadLeavesNrvWait::sInstance);
    }
}

bool DeadLeaves::receiveMsgPlayerAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    if (MR::isMsgPlayerSpinAttack(msg)) {
        setNerve(&NrvDeadLeaves::DeadLeavesNrvSpin::sInstance);
        return true;
    }

    return false;
}

DeadLeaves::~DeadLeaves() {}
