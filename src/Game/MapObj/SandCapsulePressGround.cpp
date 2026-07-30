#include "Game/MapObj/SandCapsulePressGround.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/Util.hpp"
#include "Game/Util/ObjUtil.hpp"

namespace NrvSandCapsulePressGround {
    NEW_NERVE(SandCapsulePressGroundNrvWaitBottom, SandCapsulePressGround, Wait);
    NEW_NERVE(SandCapsulePressGroundNrvWaitTop, SandCapsulePressGround, Wait);
    NEW_NERVE(SandCapsulePressGroundNrvMoveStartUp, SandCapsulePressGround, MoveStart);
    NEW_NERVE(SandCapsulePressGroundNrvMoveStartDown, SandCapsulePressGround, MoveStart);
    NEW_NERVE(SandCapsulePressGroundNrvMoveUp, SandCapsulePressGround, Move);
    NEW_NERVE(SandCapsulePressGroundNrvMoveDown, SandCapsulePressGround, Move);
    NEW_NERVE(SandCapsulePressGroundNrvMoveUpEnd, SandCapsulePressGround, MoveEnd);
    NEW_NERVE(SandCapsulePressGroundNrvMoveDownEnd, SandCapsulePressGround, MoveEnd);
    NEW_NERVE(SandCapsulePressGroundNrvDisappearDemo, SandCapsulePressGround, DisappearDemo);
    NEW_NERVE(SandCapsulePressGroundNrvStop, SandCapsulePressGround, Stop);
};  // namespace NrvSandCapsulePressGround

SandCapsulePressGround::SandCapsulePressGround(const char* pName) : MapObjActor(pName) {
    _DC.identity();
}

void SandCapsulePressGround::init(const JMapInfoIter& rIter) {
    MapObjActor::init(rIter);
    MapObjActorInitInfo info;
    MapObjActorUtil::setupInitInfoSimpleMapObj(&info);
    info.setupNerve(GET_NERVE(SandCapsulePressGround, SandCapsulePressGroundNrvWaitBottom));
    initialize(rIter, info);
    MR::setBodySensorTypePress(this);
    _DC.set(getBaseMtx());
    MR::setEffectHostMtx(this, "FallSand", _DC);

    if (MR::isDemoCast(this, nullptr)) {
        MR::tryRegisterDemoActionNerve(this, GET_NERVE(SandCapsulePressGround, SandCapsulePressGroundNrvDisappearDemo), nullptr);
    }

    _C4.set< f32 >(mPosition);
    TVec3f up;
    MR::calcUpVec(&up, this);
    _D0.scaleAdd(3600.0f, up, _C4);
}

void SandCapsulePressGround::exeWait() {
    if (MR::isOnSwitchB(this)) {
        if (isNerve(GET_NERVE(SandCapsulePressGround, SandCapsulePressGroundNrvWaitBottom))) {
            setNerve(GET_NERVE(SandCapsulePressGround, SandCapsulePressGroundNrvMoveStartUp));
        } else {
            setNerve(GET_NERVE(SandCapsulePressGround, SandCapsulePressGroundNrvMoveStartDown));
        }
    }
}

void SandCapsulePressGround::exeMoveStart() {
    if (MR::isFirstStep(this)) {
        MR::onSwitchA(this);
        MR::shakeCameraInfinity(this, 0.15f, 1.5f);
        MR::makeMtxUpNoSupport(&_DC, -mGravity);
        MR::emitEffect(this, "FallSand");
    }

    if (MR::isLessStep(this, 120)) {
        MR::tryRumblePadWeak(this, 0);
        MR::startLevelSound(this, "SE_OJ_LV_DESERT_LAND_SIGN");
    } else if (MR::isStep(this, 120)) {
        MR::stopShakingCamera(this);

        if (isNerve(GET_NERVE(SandCapsulePressGround, SandCapsulePressGroundNrvMoveStartUp))) {
            setNerve(GET_NERVE(SandCapsulePressGround, SandCapsulePressGroundNrvMoveUp));
        } else {
            setNerve(GET_NERVE(SandCapsulePressGround, SandCapsulePressGroundNrvMoveDown));
        }
    }
}

void SandCapsulePressGround::exeMove() {
    if (MR::isFirstStep(this)) {
        MR::startSound(this, "SE_OJ_DESERT_LAND_MOVE_ST");
        MR::tryRumblePadStrong(this, 0);
        calcMoveVelocity();
    }

    if (!tryStop()) {
        MR::startLevelSound(this, "SE_OJ_LV_DESERT_LAND_MOVE");
        _DC.setTrans(mPosition);

        if (MR::isStep(this, 1200)) {
            mVelocity.zero();
            MR::deleteEffect(this, "FallSand");
            MR::startSound(this, "SE_OJ_DESERT_LAND_MOVE_ED");

            if (isNerve(GET_NERVE(SandCapsulePressGround, SandCapsulePressGroundNrvMoveUp))) {
                mPosition.set< f32 >(_D0);
                setNerve(GET_NERVE(SandCapsulePressGround, SandCapsulePressGroundNrvMoveUpEnd));
            } else {
                mPosition.set< f32 >(_C4);
                setNerve(GET_NERVE(SandCapsulePressGround, SandCapsulePressGroundNrvMoveDownEnd));
            }
        }
    }
}

void SandCapsulePressGround::exeMoveEnd() {
    if (MR::isFirstStep(this)) {
        MR::offSwitchA(this);
    }

    if (MR::isStep(this, 180)) {
        if (isNerve(GET_NERVE(SandCapsulePressGround, SandCapsulePressGroundNrvMoveUpEnd))) {
            setNerve(GET_NERVE(SandCapsulePressGround, SandCapsulePressGroundNrvWaitTop));
        } else {
            setNerve(GET_NERVE(SandCapsulePressGround, SandCapsulePressGroundNrvWaitBottom));
        }
    }
}

void SandCapsulePressGround::exeDisappearDemo() {
    if (MR::isFirstStep(this)) {
        MR::overlayWithPreviousScreen(5);
        MR::offSwitchA(this);

        TVec3f v8 = _C4 + _D0;
        v8.x /= 2.0f;
        v8.y /= 2.0f;
        v8.z /= 2.0f;
        MR::resetPosition(this, v8);
        MR::startBck(this, "MoveAway", nullptr);
        MR::stopShakingCamera(this);
        MR::shakeCameraNormal();
        MR::startSound(this, "SE_OJ_DESERT_LAND_MOVE_ST");
    }

    MR::tryRumblePadWeak(this, 0);
    MR::startLevelSound(this, "SE_OJ_LV_DESERT_LAND_MOVE");

    if (MR::isBckStopped(this)) {
        MR::startSound(this, "SE_OJ_DESERT_LAND_MOVE_END");
        kill();
    }
}

void SandCapsulePressGround::exeStop() {
    if (MR::isFirstStep(this)) {
        MR::invalidateHitSensors(this);
    }

    if (MR::isStep(this, 30)) {
        mVelocity.zero();
    }
}

void SandCapsulePressGround::calcMoveVelocity() {
    TVec3f up;
    MR::calcUpVec(&up, this);

    mVelocity.scale(3.0f, isNerve(GET_NERVE(SandCapsulePressGround, SandCapsulePressGroundNrvMoveDown)) ? -up : up);
}

bool SandCapsulePressGround::tryStop() {
    if (!MR::isPlayerOnPress()) {
        return false;
    }

    setNerve(GET_NERVE(SandCapsulePressGround, SandCapsulePressGroundNrvStop));
    return true;
}

SandCapsulePressGround::~SandCapsulePressGround() {
}
