#include "Game/MapObj/FallDownBridge.hpp"
#include "Game/LiveActor/HitSensor.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/Util/ActorSensorUtil.hpp"
#include "Game/Util/JMapInfo.hpp"
#include "Game/Util/JMapUtil.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/MathUtil.hpp"
#include "Game/Util/MtxUtil.hpp"
#include "Game/Util/ObjUtil.hpp"
#include "Game/Util/SoundUtil.hpp"

namespace {
    const TVec3f sBaseClipCentor = TVec3f(0.0f, 1250.0f, 0.0f);
    // const f32 sBlockDepth = 0.0f;
    // const f32 sFallAccel = 0.0f;
    // const f32 sFallDegreeFreq = 0.0f;
    // const f32 sFixFallDegree = 0.0f;
    // const f32 sFixDegreeVelocity = 0.0f;
    // const f32 sReboundRate = 0.0f;
    // const f32 sNormalPushPower = 0.0f;
    // const f32 sDashPushPower = 0.0f;
    // const f32 sTestPushPower = 0.0f;
    const s32 sQuakeTime = 80;
    const f32 sShakeCameraNormalVelocity = 1.0f;
    const f32 sShakeCameraWeakVelocity = 0.5f;
    // const f32 sLimitFallDownDgree = 0.0f;
};  // namespace

namespace NrvFallDownBridge {
    NEW_NERVE(FallDownBridgeNrvWait, FallDownBridge, Wait);
    NEW_NERVE(FallDownBridgeNrvQuake, FallDownBridge, Quake);
};  // namespace NrvFallDownBridge

FallDownBridge::FallDownBridge(const char* pName) : LiveActor(pName), _8C(0.0f, 1250.0f, 0.0f), _98(), _9C(), _A0(0.0f), _A4(90.0f), _A8() {
}

void FallDownBridge::init(const JMapInfoIter& rIter) {
    MR::initDefaultPos(this, rIter);

    const char* pName = nullptr;
    MR::getObjectName(&pName, rIter);
    initModelManagerWithAnm(pName, nullptr, false);

    MR::connectToSceneMapObj(this);

    initHitSensor(1);
    MR::addHitSensorMapObj(this, "body", 16, 0.0f, TVec3f(0.0f, 0.0f, 0.0f));

    MR::initCollisionParts(this, pName, getSensor("body"), nullptr);
    initSound(4, false);

    MR::setClippingTypeSphere(this, 2600.0f, &_8C);

    initNerve(GET_NERVE(FallDownBridge, FallDownBridgeNrvWait));

    appear();
}

void FallDownBridge::control() {
    f32 val = MR::sin(MR::toRadian(MR::abs(_98) - 1.0f / 0.11f));

    if (_98 < 0.0f) {
        val = -val;
    }

    _9C += val * 0.06f;
    _98 += _9C;

    if (!_A8 && MR::abs(_9C) >= 0.06f) {
        MR::startLevelSound(this, "SE_OJ_LV_CHAL_FD_BRIDGE_FL");
    }

    bool ret = false;
    if (_98 > _A4) {
        _98 = _A4;
        ret = true;
    }

    if (_98 < _A0) {
        _98 = _A0;
        ret = true;
    }

    if (ret) {
        if (MR::abs(_9C) > ::sShakeCameraNormalVelocity) {
            MR::startSound(this, "SE_OJ_CHAL_FD_BRIDGE_END");
            MR::shakeCameraNormal();
        } else {
            if (MR::abs(_9C) > ::sShakeCameraWeakVelocity) {
                MR::shakeCameraWeak();
            }
        }

        _9C *= -0.5f;

        if (MR::abs(_9C) < 0.06f) {
            _9C = 0.0f;

            if (MR::abs(_98) > 1.0f / 0.11f) {
                _A8 = true;
            }
        }
    }

    if (MR::abs(_98) < 0.1f && MR::abs(_9C) < 0.06f) {
        _98 = 0.0f;
        _9C = 0.0f;
    }

    _9C *= 0.99f;
}

bool FallDownBridge::receiveOtherMsg(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    if (!_A8 && isNerve(GET_NERVE(FallDownBridge, FallDownBridgeNrvWait)) && (msg == ACTMES_BALL_DASH_WALL || msg == ACTMES_BALL_TOUCH_WALL)) {
        LiveActor* pHost = pSender->mHost;
        if (!MR::isBindedWall(pHost)) {
            return false;
        }

        TVec3f vec6C(*MR::getWallNormal(pHost));
        MR::normalizeOrZero(&vec6C);

        TPos3f mtx60;
        mtx60.set(getBaseMtx());

        TVec3f vec78;
        mtx60.getXDir(vec78);

        f32 dot = vec78.dot(vec6C);
        if (MR::abs(dot) < 0.1f) {
            return false;
        }

        f32 val;
        if (msg == ACTMES_BALL_DASH_WALL) {
            val = 0.45f;
            MR::startSound(this, "SE_OJ_CHAL_FD_BRIDGE_HIT");
        } else {
            val = 0.3f;
        }

        if (dot < 0.0f) {
            val = -val;
        }

        _9C += val;

        setNerve(GET_NERVE(FallDownBridge, FallDownBridgeNrvQuake));

        return true;
    }

    return false;
}

void FallDownBridge::calcAndSetBaseMtx() {
    LiveActor::calcAndSetBaseMtx();

    TPos3f mtx58;
    mtx58.set(getBaseMtx());

    TPos3f mtx88;
    mtx88.identity();

    TVec3f vecC4;
    if (_98 >= 0.0f) {
        vecC4.set< f32 >(0.0f, 0.0f, 0.0f);
    } else {
        vecC4.set(mScale.x * 400.0f, 0.0f, 0.0f);
    }

    mtx88.setTrans(-vecC4);

    // FIXME: inline TVec3f ctor
    TPos3f baseMtx;
    baseMtx.identity();
    baseMtx.makeRotate(TVec3f(0.0f, 0.0f, 1.0f), MR::toRadian(_98));

    baseMtx.concat(baseMtx, mtx88);

    MR::addTransMtx(baseMtx, vecC4);

    baseMtx.concat(mtx58, baseMtx);

    MR::setBaseTRMtx(this, baseMtx);

    baseMtx.mult(::sBaseClipCentor, _8C);
}

void FallDownBridge::exeWait() {
}

void FallDownBridge::exeQuake() {
    if (getNerveStep() > ::sQuakeTime) {
        setNerve(GET_NERVE(FallDownBridge, FallDownBridgeNrvWait));
    }
}
