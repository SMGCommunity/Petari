#include "Game/MapObj/LavaSteam.hpp"
#include "Game/LiveActor/HitSensor.hpp"
#include "Game/Util/ActorSensorUtil.hpp"
#include "JSystem/JGeometry/TUtil.hpp"
#include "JSystem/JGeometry/TVec.hpp"
#include "JSystem/JMath/JMath.hpp"
#include "math_types.hpp"
#include "revolution/types.h"

namespace NrvLavaSteam {
    NEW_NERVE(HostTypeWait, LavaSteam, Wait);
    NEW_NERVE(HostTypeWaitForSwitchOn, LavaSteam, WaitForSwitchOn);
    NEW_NERVE(HostTypeSteam, LavaSteam, Steam);
    NEW_NERVE(HostTypeSteamEnd, LavaSteam, SteamEnd);
};  // namespace NrvLavaSteam

LavaSteam::LavaSteam(const char* pName) : LiveActor(pName) {
    f32 f = 1.0f;
    f32 g = 0.0f;
    _8C.x = g;
    _8C.y = f;
    _8C.z = g;
    _98.x = f;
    _98.y = f;
    _98.z = f;
}

void LavaSteam::init(const JMapInfoIter& rIter) {
    MR::initDefaultPos(this, rIter);
    initModelManagerWithAnm("LavaSteam", 0, 0);
    initEffectKeeper(0, 0, false);
    MR::setEffectHostSRT(this, "Sign", &mPosition, &mRotation, &_98);
    initHitSensor(1);
    TVec3f vec;
    vec.x = 0.0f;
    vec.y = 250.0f;
    vec.z = 0.0f;
    HitSensor* pSensor = MR::addHitSensorMapObj(this, "body", 8, 250.0f, vec);

    MR::setClippingTypeSphere(this, 250.0f, &pSensor->mPosition);
    MR::setGroupClipping(this, rIter, 0x10);
    initNerve(&NrvLavaSteam::HostTypeWait::sInstance);

    if (MR::useStageSwitchReadA(this, rIter)) {
        setNerve(&NrvLavaSteam::HostTypeWaitForSwitchOn::sInstance);
        MR::listenStageSwitchOnA(this, MR::Functor(this, &LavaSteam::startSteam));
    } else if (MR::tryRegisterDemoCast(this, rIter)) {
        setNerve(&NrvLavaSteam::HostTypeWaitForSwitchOn::sInstance);
        MR::registerDemoActionFunctor(this, MR::Functor(this, &LavaSteam::startSteam), 0);
    }
    MR::useStageSwitchSleep(this, rIter);
    initSound(4, 0);
    MR::connectToSceneNoSilhouettedMapObj(this);
    makeActorAppeared();
}

#ifdef NON_MATCHING
void LavaSteam::initAfterPlacement() {
    TRot3f mtx;
    mtx.identity();
    TVec3f vec = mRotation * PI_180;

    f32 vecz = vec.z;
    f32 vecy = vec.y;
    f32 vecx = vec.x;

    f32 v11 = cos(vecz);
    f32 v12 = cos(vecy);
    f32 v13 = cos(vecx);
    f32 v15 = sin(vecy);
    f32 v14 = sin(vecz);
    f32 v16 = sin(vecx);

    mtx.mMtx[0][0] = v12 * v11;
    mtx.mMtx[2][1] = v16 * v12;
    mtx.mMtx[1][0] = v12 * v14;
    mtx.mMtx[1][1] = (v13 * v11) + (v16 * v15 * v14);
    mtx.mMtx[0][1] = (v16 * v15 * v11) - (v13 * v14);
    mtx.mMtx[0][2] = (v13 * v11 * v15) + (v16 * v14);
    mtx.mMtx[2][0] = -v15;
    mtx.mMtx[2][2] = v12 * v13;
    mtx.mMtx[1][2] = (v13 * v14 * v15) - (v16 * v11);

    _8C.set(mtx.mMtx[0][1], mtx.mMtx[1][1], mtx.mMtx[2][1]);
    MR::normalize(&_8C);
}
#endif

#ifdef NON_MATCHING
void LavaSteam::attackSensor(HitSensor* pSender, HitSensor* pReceiver) {
    TVec3f stack_64;
    TVec3f stack_58;
    f32 stack_2C, stack_54;

    if (!isNerve(&NrvLavaSteam::HostTypeWait::sInstance) && !isNerve(&NrvLavaSteam::HostTypeWaitForSwitchOn::sInstance)) {
        if (MR::isSensorPlayerOrRide(pReceiver)) {
            f32 rad = pReceiver->mRadius;
            f32 f31 = rad * 0.7f;

            if (isNerve(&NrvLavaSteam::HostTypeSteam::sInstance)) {
                TVec3f stack_30 = _8C * 400.0f;
                TVec3f stack_3C = TVec3f(mPosition);

                JMathInlineVEC::PSVECAdd2(&stack_3C, &stack_30, &stack_3C);

                TVec3f stack_48;
                stack_48.set(stack_3C);

                stack_54 = 70.0;

                TVec3f stack_20;
                stack_20.set(pReceiver->mPosition);

                f32 dist = stack_48.squared(stack_20);
                f32 s = (stack_54 + f31);
                stack_2C = f31;

                if ((dist <= s * s) & 3 && MR::sendMsgEnemyAttackFire(pReceiver, pSender))
                    return;
            }

            if (isNerve(&NrvLavaSteam::HostTypeSteam::sInstance)) {
                TVec3f stack_14 = _8C * 330.0f;

                stack_58.set(mPosition);
                stack_64.set(stack_14);

                TVec3f stack_8;
                JMathInlineVEC::PSVECSubtract(&pReceiver->mPosition, &stack_58, &stack_8);
                f32 sq1 = stack_8.squared();
                f32 sq2 = stack_64.squared();
                f32 _64dot8 = stack_64.dot(stack_8);
                f32 ff1 = sq2 * sq1;
                f32 ff2 = _64dot8 / sq2;
                f32 squ3 = JGeometry::TUtil< f32 >::sqrt((ff1 - (_64dot8 * _64dot8)) / sq2);

                if (0.0f <= ff2 && ff2 <= 1.0f && squ3 < (10.0f + f31) && MR::sendMsgEnemyAttackFire(pReceiver, pSender))
                    return;
            }
        }
    }
}
#endif

void LavaSteam::startClipped() {
    LiveActor::startClipped();
    MR::deleteEffectAll(this);
}

void LavaSteam::endClipped() {
    LiveActor::endClipped();

    if (isNerve(&NrvLavaSteam::HostTypeWaitForSwitchOn::sInstance))
        return;
}

void LavaSteam::startSteam() {
    setNerve(&NrvLavaSteam::HostTypeSteam::sInstance);
}

#ifdef NON_MATCHING
void LavaSteam::exeWait() {
    if (MR::isFirstStep(this)) {
        MR::invalidateHitSensors(this);
        MR::emitEffect(this, "Sign");
        _98.x = 1.0f;
        _98.y = 1.0f;
        _98.z = 1.0f;
    }

    if (MR::isGreaterStep(this, 82)) {
        f32 c = 90 - getNerveStep();
        _98.setAll< f32 >(MR::getEaseInValue(c *= 0.125f, 0.001, 1.0, 1.0));
    }

    if (MR::isStep(this, 90))
        MR::forceDeleteEffect(this, "Sign");

    if (MR::isStep(this, 120))
        setNerve(&NrvLavaSteam::HostTypeSteam::sInstance);
}
#endif

void LavaSteam::exeWaitForSwitchOn() {}

void LavaSteam::exeSteam() {
    if (MR::isFirstStep(this)) {
        MR::emitEffect(this, "Steam");
        MR::validateHitSensors(this);
    }

    MR::startLevelSound(this, "SE_OJ_LV_LAVA_STEAM_OUT", -1, -1, -1);

    if (MR::isStep(this, 90)) {
        MR::deleteEffect(this, "Steam");
        setNerve(&NrvLavaSteam::HostTypeSteamEnd::sInstance);
    }
}

void LavaSteam::exeSteamEnd() {
    if (MR::isStep(this, 90)) {
        setNerve(&NrvLavaSteam::HostTypeWait::sInstance);
    }
}

LavaSteam::~LavaSteam() {}
