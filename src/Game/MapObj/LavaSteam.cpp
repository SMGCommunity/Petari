#include "Game/MapObj/LavaSteam.hpp"
#include "Game/Util/ActorSensorUtil.hpp"
#include "JSystem/JGeometry/TUtil.hpp"
#include "JSystem/JMath/JMath.hpp"
#include "revolution/mtx.h"
#include "revolution/types.h"

LavaSteam::LavaSteam(const char* pName) : LiveActor(pName) {
    _8C.x = 0.0f;
    _8C.y = 1.0f;
    _8C.z = 0.0f;
    _98.x = 1.0f;
    _98.y = 1.0f;
    _98.z = 1.0f;
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
    }
    else if (MR::tryRegisterDemoCast(this, rIter)) {
        setNerve(&NrvLavaSteam::HostTypeWaitForSwitchOn::sInstance);
        MR::registerDemoActionFunctor(this, MR::Functor(this, &LavaSteam::startSteam), 0);
    }
    MR::useStageSwitchSleep(this, rIter);
    initSound(4, 0);
    MR::connectToSceneNoSilhouettedMapObj(this);
    makeActorAppeared();
}

void LavaSteam::initAfterPlacement() {
    TMtx34f mtx;
    mtx.identity();
    TVec3f vec = mRotation*0.017453292;
    f32 vecz = vec.z;
    f32 vecy = vec.y;
    f32 vecx = vec.x;

    f32 cosz = cos(vecz);
    f32 cosy = cos(vecy);
    f32 cosx = cos(vecx);
    f32 sinz = sin(vecz);
    f32 siny = sin(vecy);
    f32 sinx = sin(vecx);
    f32 sinx2 = sinx;


    f32 calc3 = (sinx*cosy);
    mtx.mMtx[0][0] = (cosy*cosz);
    mtx.mMtx[2][1] = (sinx*cosy);
    mtx.mMtx[1][0] = (cosy*sinz);
    f32 calc2 = ((cosx * cosz) + ((sinx * siny) * sinz));
    f32 calc1 = (((sinx * siny) * cosz) - (cosx * sinz));
    mtx.mMtx[1][1] = calc2;
    mtx.mMtx[0][1] = calc1;
    mtx.mMtx[0][2] = ((cosx * cosz) * siny) + (sinx2 * sinz);
    mtx.mMtx[2][0] = -siny;
    mtx.mMtx[2][2] = (cosx * cosy);
    mtx.mMtx[1][2] = ((cosx * sinz) * siny) - (sinx2 * cosz);
    
    _8C.set(calc1, calc2, calc3);
    MR::normalize(&_8C);
}


void LavaSteam::attackSensor(HitSensor* pSender, HitSensor* pReceiver) {
    TVec3f stack_64;

    if (!isNerve(&NrvLavaSteam::HostTypeWait::sInstance) && !isNerve(&NrvLavaSteam::HostTypeWaitForSwitchOn::sInstance)) {
        if (MR::isSensorPlayerOrRide(pReceiver)) {
            f32 f = 0.69999999f*pReceiver->mRadius;

            if (isNerve(&NrvLavaSteam::HostTypeSteam::sInstance)) {
                TVec3f stack_30 = _8C*400.0f;
                TVec3f stack_3C = mPosition;
                JMathInlineVEC::PSVECAdd(&stack_30, &stack_3C, &stack_3C);

                TVec3f stack_48 = stack_3C;
                f32 v50 = 70.0f;
                TVec3f stack_20 = TVec3f(pReceiver->mPosition);

                __asm volatile {
                   psq_l f2, 0x4C(r1), 0, 0
                   psq_l f1, 0x24(r1), 0, 0
                   ps_sub f2, f2, f1
                   psq_l f3, 0x48(r1), 0, 0
                   psq_l f1, 0x20(r1), 0, 0
                };

                __asm volatile {
                    ps_mul f2, f2, f2
                    ps_sub f1, f3, f1
                    ps_madd f1, f1, f1, f2
                    ps_sum0 f1, f1, f2, f2
                };

                if ((v50 + f)*(v50+ f) || !MR::sendMsgEnemyAttackFire(pReceiver, pSender) || isNerve(&NrvLavaSteam::HostTypeSteam::sInstance)) {
                    TVec3f stack_8;
                    TVec3f stack_14 = _8C*330.0f;
                    TVec3f stack_58 = mPosition;
                    stack_64 = stack_14;

                    JMathInlineVEC::PSVECSubtract(&pReceiver->mPosition, &stack_58, &stack_8);
                    f32 squ1 = stack_8.squared();
                    f32 squ2 = stack_64.squared();
                    f32 _64dot8 = stack_64.dot(stack_8);

                    f32 ff1 = squ2*squ1;
                    f32 ff2 = _64dot8/squ2;
                    f32 squ3 = JGeometry::TUtil<f32>::sqrt((ff1-(_64dot8*_64dot8)) /squ2);

                    if (0.0f <= ff2 && ff2 <= 1.0 && squ3 < (10+f)) {
                        MR::sendMsgEnemyAttackFire(pReceiver, pSender);
                    }
                }
            }
        }
    }
}

void LavaSteam::startClipped() {
    LiveActor::startClipped();
    MR::deleteEffectAll(this);
}

void LavaSteam::endClipped() {
    LiveActor::endClipped(),
    isNerve(&NrvLavaSteam::HostTypeWaitForSwitchOn::sInstance);
}

void LavaSteam::startSteam() {
    setNerve(&NrvLavaSteam::HostTypeSteam::sInstance);
}

void LavaSteam::exeWait() {
    if (MR::isFirstStep(this)) {
        MR::invalidateHitSensors(this);
        MR::emitEffect(this, "Sign");
        _98.x = 1.0f;
        _98.y = 1.0f;
        _98.z = 1.0f;
    }

    if (MR::isGreaterStep(this, 82)) {
        int step = getNerveStep();
        f32 f = MR::getEaseInValue((90-step)*0.125f, 0.001f, 1.0f, 1.0f);
        _98.setAll<f32>(f);
    }

    if (MR::isStep(this, 90))
        MR::forceDeleteEffect(this, "Sign");

    if (MR::isStep(this, 120))
        setNerve(&NrvLavaSteam::HostTypeSteam::sInstance);
}

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

LavaSteam::~LavaSteam() {

}
namespace NrvLavaSteam {
    void HostTypeSteamEnd::execute(Spine* pSpine) const {
        LavaSteam* pActor = (LavaSteam*)pSpine->mExecutor;

        if (MR::isStep(pActor, 0x5A))
            pActor->setNerve(&NrvLavaSteam::HostTypeWait::sInstance);
    }

    HostTypeWait(HostTypeWait::sInstance);
    HostTypeWaitForSwitchOn(HostTypeWaitForSwitchOn::sInstance);
    HostTypeSteam(HostTypeSteam::sInstance);
    HostTypeSteamEnd(HostTypeSteamEnd::sInstance);
}
