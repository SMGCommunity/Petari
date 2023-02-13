#include "Game/MapObj/MechaKoopaPartsArm.h"
#include "Game/Util.h"

MechaKoopaPartsArm::MechaKoopaPartsArm(const char *pName) : MapObjActor(pName) {
    mIsRightArm = false;
}

void MechaKoopaPartsArm::init(const JMapInfoIter &rIter) {
    MapObjActor::init(rIter);
    mIsRightArm = isObjectName("MechaKoopaPartsArmRight");
    MapObjActorInitInfo info;
    info.setupHioNode("地形オブジェ");
    info.setupDefaultPos();
    info.setupConnectToScene();
    info.setupEffect(nullptr);
    info.setupSound(6);
    info.setupNerve(&NrvMechaKoopaPartsArm::MechaKoopaPartsArmNrvWaitRear::sInstance);
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
        if (isNerve(&NrvMechaKoopaPartsArm::MechaKoopaPartsArmNrvWaitFront::sInstance)) {
            setNerve(&NrvMechaKoopaPartsArm::MechaKoopaPartsArmNrvMoveStartRear::sInstance);
        }
        else {
            setNerve(&NrvMechaKoopaPartsArm::MechaKoopaPartsArmNrvMoveStartFront::sInstance);
        }
    }
}

void MechaKoopaPartsArm::exeMoveStart() {
    if (MR::isFirstStep(this)) {
        MapObjActorUtil::pauseRotator(this);

        if (isNerve(&NrvMechaKoopaPartsArm::MechaKoopaPartsArmNrvMoveStartFront::sInstance)) {
            MR::startBck(this, "Front", nullptr);
        }
        else {
            MR::startBck(this, "Rear", nullptr);
        }

        MR::startSound(this, "SE_OJ_M_KOOPA_ARM_START", -1, -1);
        if (MR::isOnPlayer(this)) {
            MR::shakeCameraNormal();
            MR::tryRumblePadMiddle(this, 0);
        }
    }

    if (MR::isBckStopped(this)) {
        if (isNerve(&NrvMechaKoopaPartsArm::MechaKoopaPartsArmNrvMoveStartFront::sInstance)) {
            setNerve(&NrvMechaKoopaPartsArm::MechaKoopaPartsArmNrvMoveFront::sInstance);
        }
        else {
            setNerve(&NrvMechaKoopaPartsArm::MechaKoopaPartsArmNrvMoveRear::sInstance);
        }
    }
}

void MechaKoopaPartsArm::exeMove() {
    MR::startLevelSound(this, "SE_OJ_LV_M_KOOPA_ARM_MOVE", -1, -1, -1);

    if (!MapObjActorUtil::isRotatorMoving(this)) {
        MR::startSound(this, "SE_OJ_M_KOOPA_ARM_END", -1, -1);

        if (isNerve(&NrvMechaKoopaPartsArm::MechaKoopaPartsArmNrvMoveFront::sInstance)) {
            setNerve(&NrvMechaKoopaPartsArm::MechaKoopaPartsArmNrvWaitFront::sInstance);
        }
        else {
            setNerve(&NrvMechaKoopaPartsArm::MechaKoopaPartsArmNrvWaitRear::sInstance);
        }
    }
}

void MechaKoopaPartsArm::exeBreakStart() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "BreakStart", nullptr);
        MR::startSound(this, "SE_OJ_M_KOOPA_ARM_FALL_ST", -1, -1);
        MapObjActorUtil::pauseRotator(this);
    }

    if (MR::isBckStopped(this)) {
        setNerve(&NrvMechaKoopaPartsArm::MechaKoopaPartsArmNrvBreak::sInstance);
    }
}

void MechaKoopaPartsArm::exeBreak() {
    if (MR::isFirstStep(this)) {
        MR::emitEffect(this, "Spark");
        MR::emitEffect(this, "BlackSmoke");
        MapObjActorUtil::startRailMover(this);
    }

    MR::startLevelSound(this, "SE_OJ_LV_M_KOOPA_ARM_FALL", -1, -1, -1);
    if (MapObjActorUtil::isRailMoverReachedEnd(this)) {
        MR::startSound(this, "SE_OJ_M_KOOPA_ARM_BREAK", -1, -1);
        kill();
    }
}

void MechaKoopaPartsArm::initCaseUseSwitchB(const MapObjActorInitInfo &rInfo) {
    if (mIsRightArm) {
        MR::FunctorV0M<MechaKoopaPartsArm *, void (MechaKoopaPartsArm::*)()> breakFunc = MR::Functor<MechaKoopaPartsArm>(this, &MechaKoopaPartsArm::startBreak);
        MR::listenStageSwitchOnB(this, breakFunc);
    }
}

void MechaKoopaPartsArm::initCaseNoUseSwitchB(const MapObjActorInitInfo &) {

}

void MechaKoopaPartsArm::startBreak() {
    setNerve(&NrvMechaKoopaPartsArm::MechaKoopaPartsArmNrvBreakStart::sInstance);
}

namespace NrvMechaKoopaPartsArm {
    INIT_NERVE(MechaKoopaPartsArmNrvWaitFront);
    INIT_NERVE(MechaKoopaPartsArmNrvWaitRear);
    INIT_NERVE(MechaKoopaPartsArmNrvMoveStartFront);
    INIT_NERVE(MechaKoopaPartsArmNrvMoveStartRear);
    INIT_NERVE(MechaKoopaPartsArmNrvMoveFront);
    INIT_NERVE(MechaKoopaPartsArmNrvMoveRear);
    INIT_NERVE(MechaKoopaPartsArmNrvBreakStart);
    INIT_NERVE(MechaKoopaPartsArmNrvBreak);

    void MechaKoopaPartsArmNrvBreak::execute(Spine *pSpine) const {
        MechaKoopaPartsArm* arm = reinterpret_cast<MechaKoopaPartsArm*>(pSpine->mExecutor);
        arm->exeBreak();
    }

    void MechaKoopaPartsArmNrvBreakStart::execute(Spine *pSpine) const {
        MechaKoopaPartsArm* arm = reinterpret_cast<MechaKoopaPartsArm*>(pSpine->mExecutor);
        arm->exeBreakStart();
    }

    void MechaKoopaPartsArmNrvMoveRear::execute(Spine *pSpine) const {
        MechaKoopaPartsArm* arm = reinterpret_cast<MechaKoopaPartsArm*>(pSpine->mExecutor);
        arm->exeMove();
    }

    void MechaKoopaPartsArmNrvMoveFront::execute(Spine *pSpine) const {
        MechaKoopaPartsArm* arm = reinterpret_cast<MechaKoopaPartsArm*>(pSpine->mExecutor);
        arm->exeMove();
    }

    void MechaKoopaPartsArmNrvMoveStartRear::executeOnEnd(Spine *pSpine) const {
        MechaKoopaPartsArm* arm = reinterpret_cast<MechaKoopaPartsArm*>(pSpine->mExecutor);
        MapObjActorUtil::resumeAllMapPartsFunctions(arm);
    }

    void MechaKoopaPartsArmNrvMoveStartRear::execute(Spine *pSpine) const {
        MechaKoopaPartsArm* arm = reinterpret_cast<MechaKoopaPartsArm*>(pSpine->mExecutor);
        arm->exeMoveStart();
    }

    void MechaKoopaPartsArmNrvMoveStartFront::executeOnEnd(Spine *pSpine) const {
        MechaKoopaPartsArm* arm = reinterpret_cast<MechaKoopaPartsArm*>(pSpine->mExecutor);
        MapObjActorUtil::resumeAllMapPartsFunctions(arm);
    }

    void MechaKoopaPartsArmNrvMoveStartFront::execute(Spine *pSpine) const {
        MechaKoopaPartsArm* arm = reinterpret_cast<MechaKoopaPartsArm*>(pSpine->mExecutor);
        arm->exeMoveStart();
    }

    void MechaKoopaPartsArmNrvWaitRear::execute(Spine *pSpine) const {
        MechaKoopaPartsArm* arm = reinterpret_cast<MechaKoopaPartsArm*>(pSpine->mExecutor);
        arm->exeWait();
    }

    void MechaKoopaPartsArmNrvWaitFront::execute(Spine *pSpine) const {
        MechaKoopaPartsArm* arm = reinterpret_cast<MechaKoopaPartsArm*>(pSpine->mExecutor);
        arm->exeWait();
    }
};

MechaKoopaPartsArm::~MechaKoopaPartsArm() {

}