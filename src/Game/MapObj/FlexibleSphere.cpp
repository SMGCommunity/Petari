#include "Game/MapObj/FlexibleSphere.hpp"
#include "Game/Player/J3DModelX.hpp"
#include "Game/Scene/SceneFunction.hpp"
#include "Game/Util/ActorSensorUtil.hpp"
#include "Game/Util/ActorSwitchUtil.hpp"
#include "Game/Util/DemoUtil.hpp"
#include "Game/Util/EffectUtil.hpp"
#include "Game/Util/Functor.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/MapPartsUtil.hpp"
#include "Game/Util/ModelUtil.hpp"
#include "Game/Util/ObjUtil.hpp"
#include "Game/Util/PlayerUtil.hpp"
#include "Game/Util/SoundUtil.hpp"
#include <JSystem/J3DGraphBase/J3DShape.hpp>
#include <JSystem/JGeometry/TMatrix.hpp>
#include <revolution/gx/GXGeometry.h>
#include <revolution/mtx.h>

#define GROW true
#define SHRINK false

f32 scale = 1.0f;
void shapeCallBack(J3DShape* pShape) {
    Mtx mtx;
    PSMTXScale(mtx, scale, scale, 0.0f);
    GXLoadTexMtxImm(mtx, 30, GX_MTX2x4);
    GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, 30, GX_FALSE, 125);
    GXSetTexCoordGen2(GX_TEXCOORD1, GX_TG_MTX2x4, GX_TG_TEX0, 30, GX_FALSE, 125);
    GXSetTexCoordGen2(GX_TEXCOORD2, GX_TG_MTX2x4, GX_TG_TEX0, 30, GX_FALSE, 125);
}

FlexibleSphere::FlexibleSphere(const char* pName) : LiveActor(pName), mTimer(0) {}

void FlexibleSphere::init(const JMapInfoIter& rIter) {
    MR::initDefaultPos(this, rIter);

    char arcName[256];
    MR::getMapPartsObjectNameIfExistShapeID(arcName, 256, rIter);
    initModelManagerWithAnm(arcName, nullptr, false);

    initHitSensor(1);
    MR::addHitSensorMapObj(this, "body", 8, 1000.0f, TVec3f(0.0f, 0.0f, 0.0f));
    MR::invalidateHitSensors(this);

    initEffectKeeper(0, nullptr, false);

    initSound(4, false);

    MR::initCollisionParts(this, arcName, getSensor("body"), nullptr);

    mGrowDirection = GROW;

    MR::connectToScene(this, MR::MovementType_Planet, MR::CalcAnimType_Planet, -1, MR::DrawType_FlexibleSphere);

    MR::setGroupClipping(this, rIter, 32);
    MR::setClippingTypeSphere(this, 3000.0f);
    MR::setClippingFarMax(this);

    J3DModelX* model = reinterpret_cast< J3DModelX* >(MR::getJ3DModel(this));
    model->mShapeCallback = shapeCallBack;

    if (MR::isValidInfo(rIter)) {
        MR::useStageSwitchReadA(this, rIter);
        MR::useStageSwitchWriteDead(this, rIter);
    }

    makeActorAppeared();

    mMaxScale = 1.0f;
    mMinScale = 0.1f;
    mScaleRate = 0.002f;
    mVanishOnShrinkEnd = false;
    _90 = true;

    if (MR::isValidInfo(rIter)) {
        s32 arg0 = -1;
        s32 arg1 = -1;
        s32 arg2 = -1;
        s32 arg3 = -1;
        s32 arg4 = -1;

        MR::getJMapInfoArg0NoInit(rIter, &arg0);
        MR::getJMapInfoArg1NoInit(rIter, &arg1);
        MR::getJMapInfoArg2NoInit(rIter, &arg2);
        MR::getJMapInfoArg3NoInit(rIter, &arg3);
        MR::getJMapInfoArg4NoInit(rIter, &arg4);

        if (arg0 != -1) {
            mMaxScale = arg0 / 10000.0f;
        }

        if (arg1 != -1) {
            mMinScale = arg1 / 10000.0f;
        }

        if (arg2 != -1) {
            mScaleRate = arg2 / 10000.0f;
        }

        if (arg3 == 1) {
            mVanishOnShrinkEnd = true;
        }

        if (arg4 == 1) {
            _90 = false;
        }
    }

    mShakeTimer = 0;
    mEnabled = false;

    if (mVanishOnShrinkEnd) {
        mGrowDirection = SHRINK;
        mTimer = (mScale.x - mMinScale) / mScaleRate;
    }
}

void FlexibleSphere::kill() {
    if (MR::isValidSwitchDead(this)) {
        MR::onSwitchDead(this);
    }

    if (MR::isRegisteredEffect(this, "Vanish")) {
        MR::emitEffect(this, "Vanish");
    }

    LiveActor::kill();
}

void FlexibleSphere::control() {
    if (mShakeTimer != 0) {
        if (MR::calcDistanceToPlayer(mPosition) < 4000.0f) {
            MR::startLevelSound(this, "SE_AT_LV_EARTHQUAKE", -1, -1, -1);
        }

        if (--mShakeTimer == 0) {
            MR::stopShakingCamera(this);
            MR::validateClipping(this);
        }
        return;
    }

    if (!mEnabled) {
        if (MR::isValidSwitchA(this)) {
            if (MR::isOnSwitchA(this)) {
                mEnabled = true;
                if (MR::isRegisteredEffect(this, "Wind")) {
                    MR::emitEffect(this, "Wind");
                }
            } else {
                return;
            }
        } else {
            if (MR::isActorOnPlayer(this)) {
                mEnabled = true;
            } else {
                return;
            }
        }
    }

    MR::startLevelSound(this, "SE_OJ_LV_FLEX_SPHERE_MOVE", ((mScale.x - mMinScale) / (mMaxScale - mMinScale)) * 100.0f, -1, -1);

    bool quake = false;
    if (mGrowDirection != SHRINK) {
        f32 scale = mScale.x;
        if (scale < mMaxScale) {
            scale = scale + mScaleRate;
            mScale.x = scale;
            mScale.y = scale;
            mScale.z = scale;
        } else {
            mGrowDirection = SHRINK;
            quake = true;
        }
    } else {
        // mGrowDirection == SHRINK
        f32 scale = mScale.x;
        if (scale > mMinScale) {
            bool vanishOnEnd = mVanishOnShrinkEnd;
            scale = scale - mScaleRate;
            mScale.x = scale;
            mScale.y = scale;
            mScale.z = scale;

            if (vanishOnEnd && mTimer > 0) {
                if (!MR::isPlayerDead()) {
                    if (!MR::isPowerStarGetDemoActive() && mTimer % 60 == 0) {
                        if (mTimer <= 120) {
                            MR::startSystemSE("SE_SY_TIMER_A_QUASI_0", -1, -1);
                        } else if (mTimer <= 600) {
                            MR::startSystemSE("SE_SY_TIMER_A_1", -1, -1);
                        } else {
                            MR::startSystemSE("SE_SY_TIMER_A_2", -1, -1);
                        }
                    }
                }
                mTimer--;
            }

        } else {
            if (mVanishOnShrinkEnd) {
                MR::startSystemSE("SE_SY_TIMER_A_0", -1, -1);
                MR::startSound(this, "SE_OJ_FLEX_SPHERE_VANISH", -1, -1);
                kill();
            } else {
                mGrowDirection = GROW;
                quake = true;
            }
        }
    }

    if (quake && mShakeTimer == 0) {
        mShakeTimer = 60;
        if (MR::calcDistanceToPlayer(mPosition) < 4000.0f) {
            MR::shakeCameraInfinity(this, 0.2f, 3.0f);
            MR::invalidateClipping(this);
        }

        if (MR::isActorOnPlayer(this)) {
            MR::tryRumblePad(this, "マリオ[しびれ]", WPAD_CHAN0);
        }
    }
}

void FlexibleSphere::draw() const {
    if (!MR::isDead(this)) {
        J3DModelX* model = reinterpret_cast< J3DModelX* >(MR::getJ3DModel(this));
        model->viewCalc2();
        GXInvalidateVtxCache();
        scale = 0.05f / mScale.x + 0.95f;
        model->directDraw(nullptr);
    }
}

bool FlexibleSphere::receiveMsgPlayerAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    return false;
}
