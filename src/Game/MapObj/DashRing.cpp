#include "Game/MapObj/DashRing.hpp"
#include "Game/Scene/SceneFunction.hpp"
#include "Game/Util/ActorSensorUtil.hpp"
#include "Game/Util/DirectDraw.hpp"
#include "Game/Util/JMapUtil.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/MathUtil.hpp"
#include "Game/Util/MtxUtil.hpp"
#include "Game/Util/ObjUtil.hpp"
#include "Game/Util/PlayerUtil.hpp"
#include "Game/Util/SoundUtil.hpp"

void DashRing_FORCE_MATCH_SDATA2() {
    (void)1.0f;
    (void)0.0f;
    (void)0.5f;
    (void)1000.0f;
    (void)2.5f;
    (void)100.0f;
    (void)0.001f;
    (void)20.0f;
    (void)200.0f;
    (void)300.0f;
    (void)0.03f;
    (void)2000.0f;
    (void)15.0f;
    (void)0.1f;
    (void)TWO_PI;
    (void)(1.0f/64.0f);
    (void)(TWO_PI/64.0f);
    (void)30.0f;
}

DashRing::~DashRing() {
}

void DashRing::initCommon(const JMapInfoIter& rIter, const char* pName) {
    MR::initDefaultPos(this, rIter);

    mActiveTime = 0;
    mRevivalTime = 0;
    mDoLoop = false;

    if (pName != nullptr) {
        initModelManagerWithAnm(pName, nullptr, false);

        MR::connectToSceneMapObj(this);

        mDoLoop = true;
    } else {
        MR::connectToScene(this, MR::MovementType_MapObj, -1, -1, MR::DrawType_DashRing);
    }

    initHitSensor(1);
    MR::addHitSensorMapObj(this, "body", 4, 1000.0f, TVec3f(0.0f, 0.0f, 0.0f));
    MR::validateClipping(this);
    calcAxis();

    mBoostTime = 300;
    mMaxDuration = 120;
    mSpeedScale = 2.5f;
    mDoTurn = true;

    if (MR::checkJMapDataEntries(rIter)) {
        s32 arg0 = -1;
        s32 arg1 = -1;
        s32 arg2 = -1;

        MR::getJMapInfoArg0NoInit(rIter, &arg0);
        MR::getJMapInfoArg1NoInit(rIter, &arg1);

        if (arg0 != -1) {
            mBoostTime = arg0;
        }

        if (arg1 != -1) {
            mMaxDuration = arg1;
        }

        if (MR::getJMapInfoArg2NoInit(rIter, &arg2) && arg2 != -1) {
            mSpeedScale = arg2 / 100.0f;
        }

        s32 arg3 = -1;
        MR::getJMapInfoArg3NoInit(rIter, &arg3);

        if (arg3 != -1) {
            mDoTurn = false;
        }
    }

    initSound(3, false);
    initEffectKeeper(3, nullptr, false);

    appear();

    if (mDoLoop) {
        const char* animName = "Loop";
        MR::startBck(this, animName, 0);
        MR::startBrk(this, animName);
    }

    mInitialPosition = mPosition;
}

void DashRing::attackSensor(HitSensor* pSender, HitSensor* pReceiver) {
    if (mRevivalTime != 0) {
        return;
    }

    if (!MR::isSensorPlayer(pReceiver)) {
        return;
    }

    TVec3f dir(MR::getSensorPos(pReceiver) - mPosition);
    TVec3f vec40;
    f32 val = MR::vecKillElement(dir, mAxis, &vec40);
    if (MR::abs(val) < 20.0f && vec40.length() < 200.0f) {
        MR::sendArbitraryMsg(ACTMES_PASS_RING, pReceiver, getSensor("body"));

        if (mActiveTime != 0) {
            return;
        }

        if (mRevivalTime != 0) {
            return;
        }

        mActiveTime = 15;

        MR::invalidateClipping(this);

        MR::startSystemSE("SE_SY_GET_DASH_RING");

        if (mDoLoop) {
            const char* pName = "Start";
            MR::startBck(this, pName, nullptr);
            MR::startBrk(this, pName);
        }

        return;
    }

    if (mActiveTime != 0) {
        return;
    }

    if (!mDoTurn) {
        return;
    }

    if (!(MR::abs(val) > 300.0f || vec40.length() > 300.0f)) {
        return;
    }

    if (MR::normalizeOrZero(&dir)) {
        return;
    }

    f32 f1 = MR::diffAngleAbs(mAxis, dir);
    if (f1 > MR::diffAngleAbs(mAxis, -dir)) {
        MR::vecBlendSphere(mAxis, -dir, &mAxis, 0.03f);
    } else {
        MR::vecBlendSphere(mAxis, dir, &mAxis, 0.03f);
    }

    calcSubAxis();
}

void DashRing::control() {
    if (mActiveTime != 0) {
        mActiveTime--;

        if (mActiveTime == 0) {
            mRevivalTime = 180;
            MR::invalidateHitSensors(this);

            const char* pName = "End";
            MR::startBck(this, pName, nullptr);
            MR::startBrk(this, pName);

            return;
        }

        mPosition = *MR::getPlayerPos() + *MR::getPlayerVelocity();

        if ((mPosition - mInitialPosition).length() > 2000.0f) {
            mPosition = mInitialPosition;
            mActiveTime = 0;
        }

        return;
    }

    if (mRevivalTime != 0) {
        mRevivalTime--;

        if (mRevivalTime != 0) {
            return;
        }

        if (!mDoLoop) {
            return;
        }

        mPosition = mInitialPosition;

        const char* pName = "Revival";
        MR::startBck(this, pName, nullptr);
        MR::startBrk(this, pName);

        MR::validateHitSensors(this);

        return;
    }

    if (MR::isBckOneTimeAndStopped(this)) {
        const char* pName = "Loop";
        MR::startBck(this, pName, nullptr);
        MR::startBrk(this, pName);

        MR::validateClipping(this);
    }
}

void DashRing::calcAndSetBaseMtx() {
    if (mDoLoop) {
        TPos3f pos;
        MR::makeMtxFrontSidePos(&pos, mAxis, mSubAxis, mPosition);
        PSMTXCopy(pos, getBaseMtx());
    }
}

void DashRing::calcAxis() {
    TVec3f axis(0.0f, 1.0f, 0.0f);

    Mtx mtxTR;
    MR::makeMtxTR(mtxTR, 0.0f, 0.0f, 0.0f, mRotation.x, mRotation.y, mRotation.z);

    PSMTXMultVec(mtxTR, &axis, &axis);
    mAxis = axis;

    calcSubAxis();
}

void DashRing::calcSubAxis() {
    TVec3f axis(mAxis);
    TVec3f subAxis;

    if (!MR::isNearZero(axis.y)) {
        subAxis.x = axis.y;
        subAxis.y = -axis.x;
        subAxis.z = 0.0f;
    } else {
        subAxis.x = axis.z;
        subAxis.y = 0.0f;
        subAxis.z = -axis.x;
    }

    MR::normalizeOrZero(&subAxis);
    mSubAxis = subAxis;
}

void DashRing::draw() const {
    TVec3f axis = mAxis;
    TVec3f subAxis = mSubAxis;

    f32 f1 = 1.0f;
    if (mActiveTime > 105) {
        f1 = static_cast< f32 >(mActiveTime - 105) / 15.0f;

        if (f1 > 1.0f) {
            f1 = 1.0f;
        }

        if (f1 < 0.1f) {
            f1 = 0.1f;
        }
    } else if (mActiveTime != 0) {
        f1 = 0.1f;
    }

    f32 f2 = mScale.x * 200.0f * f1;
    f32 f3 = f2 * TWO_PI / 64;

    TPos3f rotMtx;
    PSMTXRotAxisRad(rotMtx, axis, TWO_PI / 64);

    TDDraw::setup(0, 1, 0);

    GXSetZMode(GX_TRUE, GX_LEQUAL, GX_FALSE);

    for (u32 idx = 0; idx <= 64; idx++) {
        TVec3f vecB0 = mPosition + subAxis * f2;

        TVec3f crossVec;
        crossVec.cross(subAxis, axis);

        TDDraw::drawCylinder(vecB0 - crossVec * f3 * 0.5f, crossVec * f3, mScale.y * f1 * 30.0f, 0x00808060, 0x00808080, 0x10);

        PSMTXMultVec(rotMtx, subAxis, subAxis);
    } 
}
