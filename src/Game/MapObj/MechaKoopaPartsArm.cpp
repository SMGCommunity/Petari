#include "Game/MapObj/MechaKoopaPartsArm.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/MapObj/MapObjActorInitInfo.hpp"
#include "Game/Util.hpp"

namespace NrvMechaKoopaPartsArm {
    NEW_NERVE(MechaKoopaPartsArmNrvWaitFront, MechaKoopaPartsArm, Wait);
    NEW_NERVE(MechaKoopaPartsArmNrvWaitRear, MechaKoopaPartsArm, Wait);
    NEW_NERVE_ONEND(MechaKoopaPartsArmNrvMoveStartFront, MechaKoopaPartsArm, MoveStart, MoveStart);
    NEW_NERVE_ONEND(MechaKoopaPartsArmNrvMoveStartRear, MechaKoopaPartsArm, MoveStart, MoveStart);
    NEW_NERVE(MechaKoopaPartsArmNrvMoveFront, MechaKoopaPartsArm, Move);
    NEW_NERVE(MechaKoopaPartsArmNrvMoveRear, MechaKoopaPartsArm, Move);
    NEW_NERVE(MechaKoopaPartsArmNrvBreakStart, MechaKoopaPartsArm, BreakStart);
    NEW_NERVE(MechaKoopaPartsArmNrvBreak, MechaKoopaPartsArm, Break);
};  // namespace NrvMechaKoopaPartsArm

MechaKoopaPartsArm::MechaKoopaPartsArm(const char* pName) : MapObjActor(pName), mIsRightArm() {
}

void MechaKoopaPartsArm::init(const JMapInfoIter& rIter) {
    MapObjActor::init(rIter);
    mIsRightArm = isObjectName("MechaKoopaPartsArmRight");
    MapObjActorInitInfo info;
    info.setupHioNode("地形オブジェ");
    info.setupDefaultPos();
    info.setupConnectToScene();
    info.setupEffect(nullptr);
    info.setupSound(6);
    info.setupNerve(GET_NERVE(MechaKoopaPartsArm, MechaKoopaPartsArmNrvWaitRear));
    info.setupRotator();

    if (mIsRightArm) {
        info.setupRailMover();
        info.setupBaseMtxFollowTarget();
    }

    initialize(rIter, info);
    MapObjActorUtil::startRotator(this);
}

void MechaKoopaPartsArm::exeWait() {
    if (MapObjActorUtil::isRotatorMoving(this)) {
        if (isNerve(GET_NERVE(MechaKoopaPartsArm, MechaKoopaPartsArmNrvWaitFront))) {
            setNerve(GET_NERVE(MechaKoopaPartsArm, MechaKoopaPartsArmNrvMoveStartRear));
        } else {
            setNerve(GET_NERVE(MechaKoopaPartsArm, MechaKoopaPartsArmNrvMoveStartFront));
        }
    }
}

void MechaKoopaPartsArm::exeMoveStart() {
    if (MR::isFirstStep(this)) {
        MapObjActorUtil::pauseRotator(this);

        if (isNerve(GET_NERVE(MechaKoopaPartsArm, MechaKoopaPartsArmNrvMoveStartFront))) {
            MR::startBck(this, "Front");
        } else {
            MR::startBck(this, "Rear");
        }

        MR::startSound(this, "SE_OJ_M_KOOPA_ARM_START");

        if (MR::isOnPlayer(this)) {
            MR::shakeCameraNormal();
            MR::tryRumblePadMiddle(this, WPAD_CHAN0);
        }
    }

    if (MR::isBckStopped(this)) {
        if (isNerve(GET_NERVE(MechaKoopaPartsArm, MechaKoopaPartsArmNrvMoveStartFront))) {
            setNerve(GET_NERVE(MechaKoopaPartsArm, MechaKoopaPartsArmNrvMoveFront));
        } else {
            setNerve(GET_NERVE(MechaKoopaPartsArm, MechaKoopaPartsArmNrvMoveRear));
        }
    }
}

void MechaKoopaPartsArm::endMoveStart() {
    MapObjActorUtil::resumeAllMapPartsFunctions(this);
}

void MechaKoopaPartsArm::exeMove() {
    MR::startLevelSound(this, "SE_OJ_LV_M_KOOPA_ARM_MOVE");

    if (MapObjActorUtil::isRotatorMoving(this)) {
        return;
    }

    MR::startSound(this, "SE_OJ_M_KOOPA_ARM_END");

    if (isNerve(GET_NERVE(MechaKoopaPartsArm, MechaKoopaPartsArmNrvMoveFront))) {
        setNerve(GET_NERVE(MechaKoopaPartsArm, MechaKoopaPartsArmNrvWaitFront));
    } else {
        setNerve(GET_NERVE(MechaKoopaPartsArm, MechaKoopaPartsArmNrvWaitRear));
    }
}

void MechaKoopaPartsArm::exeBreakStart() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "BreakStart");
        MR::startSound(this, "SE_OJ_M_KOOPA_ARM_FALL_ST");
        MapObjActorUtil::pauseRotator(this);
    }

    if (MR::isBckStopped(this)) {
        setNerve(GET_NERVE(MechaKoopaPartsArm, MechaKoopaPartsArmNrvBreak));
    }
}

void MechaKoopaPartsArm::exeBreak() {
    if (MR::isFirstStep(this)) {
        MR::emitEffect(this, "Spark");
        MR::emitEffect(this, "BlackSmoke");
        MapObjActorUtil::startRailMover(this);
    }

    MR::startLevelSound(this, "SE_OJ_LV_M_KOOPA_ARM_FALL");

    if (MapObjActorUtil::isRailMoverReachedEnd(this)) {
        MR::startSound(this, "SE_OJ_M_KOOPA_ARM_BREAK");
        kill();
    }
}

void MechaKoopaPartsArm::initCaseUseSwitchB(const MapObjActorInitInfo& rInfo) {
    if (mIsRightArm) {
        MR::listenStageSwitchOnB(this, MR::Functor(this, &MechaKoopaPartsArm::startBreak));
    }
}

void MechaKoopaPartsArm::initCaseNoUseSwitchB(const MapObjActorInitInfo&) {
}

void MechaKoopaPartsArm::startBreak() {
    setNerve(GET_NERVE(MechaKoopaPartsArm, MechaKoopaPartsArmNrvBreakStart));
}
