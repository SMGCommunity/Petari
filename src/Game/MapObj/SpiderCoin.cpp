#include "Game/MapObj/SpiderCoin.hpp"
#include "Game/LiveActor/PartsModel.hpp"
#include "Game/MapObj/SpiderThread.hpp"
#include "Game/Scene/SceneFunction.hpp"
#include "Game/Util/DemoUtil.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/MathUtil.hpp"
#include "Game/Util/MtxUtil.hpp"
#include "Game/Util/ObjUtil.hpp"
#include "Game/Util/StarPointerUtil.hpp"
#include <JSystem/JGeometry/TMatrix.hpp>
#include <JSystem/JGeometry/TVec.hpp>
#include <revolution/mtx.h>
#include <revolution/types.h>

inline f32 modRotateFromBase(f32 angle, f32 base) {
    return static_cast< f32 >(fmod((angle - base) + 360.0f, 360.0f)) + base;
}

namespace NrvSpiderCoin {
    NEW_NERVE(SpiderCoinNrvWait, SpiderCoin, Wait);
    NEW_NERVE(SpiderCoinNrvTouch, SpiderCoin, Touch);
    NEW_NERVE(SpiderCoinNrvTouchAndApart, SpiderCoin, TouchAndApart);
}  // namespace NrvSpiderCoin

SpiderCoin::SpiderCoin(const char* pName)
    : LiveActor(pName), mPointPos(nullptr), mNeutralPos(nullptr), mUp(nullptr), mThreadZ(0.0f), mPad0Velocity(0.0f, 0.0f), mPad1Velocity(0.0f, 0.0f),
      mCocoonModel(nullptr) {
    mBaseMtx.identity();
}

void SpiderCoin::init(const JMapInfoIter& rIter) {
    MR::initDefaultPos(this, rIter);
    mThreadZ = mPosition.z;
    initModelManagerWithAnm("Coin", nullptr, false);
    MR::connectToSceneItemStrongLight(this);

    initHitSensor(1);
    MR::addHitSensorCallbackMapObj(this, "body", 8, 50.0f);

    initEffectKeeper(8, nullptr, false);
    initNerve(&NrvSpiderCoin::SpiderCoinNrvWait::sInstance);
    MR::initStarPointerTarget(this, 150.0f, TVec3f(0.0f, 80.0f, 0.0f));

    MR::declareCoin(this, 1);
    mCocoonModel = new PartsModel(this, "まゆ", "SpaceCocoon", mBaseMtx, MR::DrawBufferType_NoSilhouettedMapObj, false);
    mCocoonModel->mScale.set(0.3f);
    mCocoonModel->initWithoutIter();

    MR::registerDemoSimpleCastAll(this);
    MR::registerDemoSimpleCastAll(mCocoonModel);
    makeActorAppeared();
}

void SpiderCoin::kill() {
    LiveActor::kill();
    TVec3f vel(-mUp->x, -mUp->y, 0.0f);
    MR::normalizeOrZero(&vel);
    vel.scale(20.0f);
    TVec3f pos(mPosition.x, mPosition.y, mThreadZ);

    MR::appearCoinToVelocity(this, pos, vel, 1);
    MR::emitEffect(mCocoonModel, "Stick");
}

void SpiderCoin::exeWait() {
    if (MR::isFirstStep(this)) {
        if (mPointPos == nullptr) {
            MR::startActorBindToSpiderThread(this, &mPointPos, &mNeutralPos, &mUp, mPosition, TVec3f(0.0f, 0.0f, 0.0f));
        }
        mRotation.y = 0.0f;
    }
    mPosition.set(*mPointPos);

    if (MR::tryStarPointerCheck(this, true, "弱")) {
        setNerve(&NrvSpiderCoin::SpiderCoinNrvTouch::sInstance);
    }
}

void SpiderCoin::exeTouch() {
    if (MR::isFirstStep(this)) {
        mPad0Velocity.y = 0.0f;
        mPad0Velocity.x = 0.0f;
        mPad1Velocity.y = 0.0f;
        mPad1Velocity.x = 0.0f;
    }

    mPosition.set(*mPointPos);
    mRotation.y += 30.0f;
    mRotation.y = modRotateFromBase(mRotation.y, 0.0f);
    if (MR::testCorePadTriggerA(WPAD_CHAN0) || MR::testCorePadTriggerA(WPAD_CHAN1)) {
        kill();
        return;
    }

    if (!tryRub(WPAD_CHAN0, &mPad0Velocity) && !tryRub(WPAD_CHAN1, &mPad1Velocity) && !MR::isStarPointerPointing(this, WPAD_CHAN0, true, "弱") &&
        !MR::isStarPointerPointing(this, WPAD_CHAN1, true, "弱")) {
        setNerve(&NrvSpiderCoin::SpiderCoinNrvTouchAndApart::sInstance);
    }
}

void SpiderCoin::exeTouchAndApart() {
    mPosition.set(*mPointPos);

    mRotation.y += MR::getEaseOutValue((30 - getNerveStep()) / 30.0f, 0.0f, 1.0f, 1.0f) * 30.0f;
    mRotation.y = modRotateFromBase(mRotation.y, 0.0f);

    if (MR::isGreaterStep(this, 5) && MR::isStarPointerPointing(this, WPAD_CHAN0, true, "弱")) {
        kill();
        return;
    }

    if (MR::isStep(this, 30)) {
        setNerve(&NrvSpiderCoin::SpiderCoinNrvWait::sInstance);
    }
}

void SpiderCoin::calcAndSetBaseMtx() {
    if (mUp != nullptr) {
        TVec3f up(*mUp);
        up.scale(-1.0f);
        MR::normalize(&up);

        TVec3f front(0.0f, 0.0f, 1.0f);
        TVec3f side;
        PSVECCrossProduct(&up, &front, &side);
        MR::normalize(&side);
        PSVECCrossProduct(&side, &up, &front);
        MR::normalize(&front);

        TPos3f mtx;
        mtx.identity();
        mtx.setVec(side, up, front);

        TRot3f rotMtx;
        rotMtx.identity();
        MR::makeMtxRotateY(rotMtx.toMtxPtr(), mRotation.y);
        PSMTXConcat(mtx.toMtxPtr(), rotMtx.toMtxPtr(), mtx.toMtxPtr());

        TVec3f pos(up);
        pos.scale(-50.0f);
        pos.add(mPosition);
        mtx.setTrans(pos);
        mBaseMtx.setInline(mtx.toMtxPtr());

        pos.set(up);
        pos.scale(-20.0f);
        pos.add(mPosition);
        TVec3f forward(front);
        forward.scale(20.0f);
        pos.add(forward);
        mBaseMtx.setTrans(pos);
        MR::setBaseTRMtx(this, mtx);
    }
}

bool SpiderCoin::tryRub(s32 padChannel, TVec2f* pVel) {
    if (!MR::isStarPointerPointing(this, padChannel, true, "弱") || MR::getStarPointerScreenSpeed(padChannel) < 5.0f) {
        return false;
    }

    TVec2f* vel = MR::getStarPointerScreenVelocity(padChannel);
    if (vel->x * pVel->x + vel->y * pVel->y < -20.0f) {
        kill();
        return true;
    }

    TVec2f* vel2 = MR::getStarPointerScreenVelocity(padChannel);
    pVel->x = vel2->x;
    pVel->y = vel2->y;
    return false;
}
