#include "Game/MapObj/DesertMovingLand.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/MapObj/MapObjActorInitInfo.hpp"
#include "Game/Util.hpp"
#include "Game/Util/ActorSensorUtil.hpp"
#include "Game/Util/ActorSwitchUtil.hpp"
#include "Game/Util/DemoUtil.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/ObjUtil.hpp"
#include "Game/Util/SoundUtil.hpp"
#include <revolution/mtx.h>

namespace {
    static const char* cDemoName = "砂漠ＵＦＯ上昇";
};  // namespace

namespace NrvDesertMovingLand {
    NEW_NERVE(HostTypeWaitTop, DesertMovingLand, Wait);
    NEW_NERVE(HostTypeWaitBottom, DesertMovingLand, Wait);
    NEW_NERVE(HostTypeMoveDown, DesertMovingLand, MoveDown);
    NEW_NERVE(HostTypeMoveUp, DesertMovingLand, MoveUp);
    NEW_NERVE(HostTypeMoveDownSign, DesertMovingLand, MoveSign);
    NEW_NERVE(HostTypeMoveUpSign, DesertMovingLand, MoveSign);
    NEW_NERVE(HostTypeStop, DesertMovingLand, Stop);
};  // namespace NrvDesertMovingLand

DesertMovingLand::DesertMovingLand(const char* pName) : MapObjActor(pName), _C4(gZeroVec), _D0(gZeroVec) {
    _DC = 720;
    _E0 = 720;
    _E4.identity();
}

void DesertMovingLand::init(const JMapInfoIter& rIter) {
    MapObjActor::init(rIter);
    MapObjActorInitInfo info = MapObjActorInitInfo();
    MapObjActorUtil::setupInitInfoSimpleMapObj(&info);
    info.setupHitSensor();
    info.setupNerve(GET_NERVE(DesertMovingLand, HostTypeWaitTop));
    info.setupBaseMtxFollowTarget();
    info.setupFarClipping(-1.0f);
    info.mIsAppearRiddleSE = false;
    initialize(rIter, info);

    if (!MR::isValidSwitchA(this))
        setNerve(GET_NERVE(DesertMovingLand, HostTypeWaitBottom));

    MtxPtr jointMtx = MR::getJointMtx(this, mObjectName);
    HitSensor* pSensorMove = getSensor(0);
    MR::initCollisionParts(this, "Move", pSensorMove, jointMtx);

    if (MR::isDead(this))
        MR::invalidateCollisionParts(this);
    else
        MR::validateCollisionParts(this);

    f32 arg = 2000.0f;
    MR::getJMapInfoArg0NoInit(rIter, &arg);
    MR::getJMapInfoArg1NoInit(rIter, &_DC);
    MR::getJMapInfoArg2NoInit(rIter, &_E0);
    TVec3f stack_3C;
    MR::calcUpVec(&stack_3C, this);

    if (MR::isValidSwitchA(this)) {
        _C4.set(mPosition);
        _D0.set(_C4 - stack_3C * arg);
    } else {
        _D0.set(mPosition);
        _C4.set(_D0 + stack_3C * arg);
    }
    MR::setBodySensorTypePress(this);
}

void DesertMovingLand::control() {
    MapObjActor::control();
    if (MR::isDemoActive(::cDemoName))
        updateDemoPlayerPos();

    MR::startLevelSound(this, "SE_AT_LV_WIND_MOVING_DESERT");
    if (!isNerve(GET_NERVE(DesertMovingLand, HostTypeStop)) && MR::isPlayerOnPress())
        setNerve(GET_NERVE(DesertMovingLand, HostTypeStop));
}

void DesertMovingLand::startDemo() {
    MtxPtr playerBaseMtx = MR::getPlayerDemoActor()->getBaseMtx();
    TMtx34f stack_38;
    stack_38.set(playerBaseMtx);
    MtxPtr jointMtx = MR::getJointMtx(this, mObjectName);
    TMtx34f stack_8;
    stack_8.set(jointMtx);
    stack_8.invert(stack_8);
    _E4.concat(stack_8, stack_38);
}

void DesertMovingLand::updateDemoPlayerPos() {
    MtxPtr jointMtx = MR::getJointMtx(this, mObjectName);
    TPos3f stack_8;
    stack_8.set(jointMtx);
    stack_8.concat(stack_8, _E4);
    stack_8.getTrans(MR::getPlayerDemoActor()->mPosition);
    MR::setPlayerBaseMtx(stack_8);
}

void DesertMovingLand::exeWait() {
    if (MR::isValidSwitchA(this) && MR::isOnSwitchA(this) && isNerve(GET_NERVE(DesertMovingLand, HostTypeWaitTop))) {
        startDemo();
        setNerve(GET_NERVE(DesertMovingLand, HostTypeMoveDownSign));
    } else if (MR::isOnSwitchB(this) && isNerve(GET_NERVE(DesertMovingLand, HostTypeWaitBottom))) {
        startDemo();
        setNerve(GET_NERVE(DesertMovingLand, HostTypeMoveUpSign));
    }
}

void DesertMovingLand::exeMoveDown() {
    if (MR::isFirstStep(this)) {
        TVec3f stack_8(_D0 - _C4);
        stack_8.setLength(stack_8.length() / _DC);
        mVelocity.set(stack_8);
        MR::startAtmosphereSE("SE_OJ_DESERT_LAND_MOVE_ST");
    }
    MR::startAtmosphereLevelSE("SE_OJ_LV_DESERT_LAND_MOVE");
    if (MR::isLessStep(this, 90)) {
        MR::tryRumblePadWeak(this, WPAD_CHAN0);
    }

    if (MR::isStep(this, 90)) {
        MR::stopShakingCamera(this);
    }

    if (MR::isStep(this, _DC)) {
        MR::offSwitchB(this);
        mVelocity.zero();
        mPosition.set(_D0);
        MR::startAtmosphereSE("SE_OJ_DESERT_LAND_MOVE_ED");
        setNerve(GET_NERVE(DesertMovingLand, HostTypeWaitBottom));
    }
}

void DesertMovingLand::exeMoveUp() {
    if (MR::isFirstStep(this)) {
        TVec3f stack_8(_C4 - _D0);
        stack_8.setLength(stack_8.length() / _E0);
        mVelocity.set(stack_8);
        MR::startAtmosphereSE("SE_OJ_DESERT_LAND_MOVE_ST");
    }
    MR::startAtmosphereLevelSE("SE_OJ_LV_DESERT_LAND_MOVE");

    if (MR::isLessStep(this, 90)) {
        MR::tryRumblePadWeak(this, WPAD_CHAN0);
    }

    if (MR::isStep(this, 90)) {
        MR::stopShakingCamera(this);
    }

    if (MR::isStep(this, _E0)) {
        if (MR::isValidSwitchA(this))
            MR::offSwitchA(this);

        mVelocity.zero();
        mPosition.set(_C4);
        MR::startAtmosphereSE("SE_OJ_DESERT_LAND_MOVE_ED");
        setNerve(GET_NERVE(DesertMovingLand, HostTypeWaitTop));
    }
}

void DesertMovingLand::exeMoveSign() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "Vibration", 0);

        if (MR::isDemoActive("砂ＵＦＯ下降"))
            MR::stopStageBGM(60);

        MR::shakeCameraInfinity(this, 0.15f, 1.2f);
    }

    MR::startLevelSound(this, "SE_OJ_LV_DESERT_LAND_SIGN");
    MR::tryRumblePadMiddle(this, WPAD_CHAN0);
    if (MR::isBckStopped(this)) {
        if (isNerve(GET_NERVE(DesertMovingLand, HostTypeMoveUpSign)))
            setNerve(GET_NERVE(DesertMovingLand, HostTypeMoveUp));
        else {
            if (isNerve(GET_NERVE(DesertMovingLand, HostTypeMoveDownSign))) {
                if (MR::isDemoActive("砂ＵＦＯ下降"))
                    MR::startEventBGM(MR::EventBgmID_Hurry);

                setNerve(GET_NERVE(DesertMovingLand, HostTypeMoveDown));
            }
        }
    }
}

void DesertMovingLand::exeStop() {
    if (MR::isFirstStep(this))
        MR::invalidateHitSensors(this);

    if (MR::isStep(this, 30))
        mVelocity.zero();
}

void DesertMovingLand::connectToScene(const MapObjActorInitInfo& rInfo) {
    MR::connectToSceneCollisionMapObj(this);
}

void DesertMovingLand::initCaseNoUseSwitchB(const MapObjActorInitInfo& rInfo) {
}

void DesertMovingLand::initCaseUseSwitchB(const MapObjActorInitInfo& rInfo) {
}

void DesertMovingLand::initCaseUseSwitchA(const MapObjActorInitInfo& rInfo) {
}

DesertMovingLand::~DesertMovingLand(){};
