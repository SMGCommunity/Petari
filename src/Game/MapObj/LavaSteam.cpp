#include "Game/MapObj/LavaSteam.hpp"
#include "Game/Util/ActorSensorUtil.hpp"
#include "JSystem/JGeometry/TUtil.hpp"
#include "JSystem/JGeometry/TVec.hpp"
#include "JSystem/JMath/JMath.hpp"
#include "math_types.hpp"
#include "revolution/types.h"

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
    TVec3f vec = mRotation*PI_180;

    f32 vecz = vec.z;
    f32 vecy = vec.y;
    f32 vecx = vec.x;

    f32 cosz = cos(vecz);
    f32 cosy = cos(vecy);
    f32 cosx = cos(vecx);
    f32 siny = sin(vecy);
    f32 sinz = sin(vecz);
    f32 sinx = sin(vecx); 


    mtx[0][0] = (cosy*cosz);
    f32 v2 = (sinx*cosy);
    mtx[2][1] = v2;
    mtx[1][0] = (cosy*sinz);
    f32 v1 = ((cosx * cosz) + ((sinx * siny) * sinz));
    mtx[1][1] = v1;
    f32 v0 = (((sinx * siny) * cosz) - (cosx * sinz));
    mtx[0][1] = v0;
    mtx[0][2] = ((cosx * cosz) * siny) + (sinx * sinz);
    mtx[2][2] = (cosx * cosy);
    mtx[1][2] = ((cosx * sinz) * siny) - (sinx * cosz);
    mtx[2][0] = -siny;

    _8C.set(v0, v1, v2);
    MR::normalize(&_8C);
}

void LavaSteam::attackSensor(HitSensor* pSender, HitSensor* pReceiver) {

    if (!isNerve(&NrvLavaSteam::HostTypeWait::sInstance) && !isNerve(&NrvLavaSteam::HostTypeWaitForSwitchOn::sInstance)) {
        if (MR::isSensorPlayerOrRide(pReceiver)) {
            f32 rad = pReceiver->mRadius;
            f32 f31 = rad*0.69999999f;
            
            if (isNerve(&NrvLavaSteam::HostTypeSteam::sInstance)) {
                TVec3f stack_30 =_8C*400.0f;
                TVec3f stack_3C = TVec3f(mPosition);
            
                JMathInlineVEC::PSVECAdd2(&stack_3C, &stack_30, &stack_3C);
                
                TVec3f stack_48;
                stack_48.set(stack_3C);
                stack_48.z = 70.0f;
                TVec3f stack_20;
                stack_20.set(pReceiver->mPosition);

                f32 dist = JMathInlineVEC::PSVECSquareDistance((Vec*)&stack_48, (Vec*)&stack_20);
                f32 s = (stack_48.z+f31);
                stack_48.z= f31;

                if ((dist <= s*s) & 3 && MR::sendMsgEnemyAttackFire(pReceiver, pSender))
                    return;
            }
        
            if (isNerve(&NrvLavaSteam::HostTypeSteam::sInstance)) {
                TVec3f stack_64;
                TVec3f stack_58;
                TVec3f stack_14 = _8C*330.0f;

                stack_58.set(mPosition);
                stack_64.set(stack_14);

                TVec3f stack_8;
                JMathInlineVEC::PSVECSubtract(&pReceiver->mPosition, &stack_58, &stack_8);
                f32 sq1 = stack_8.squared();
                f32 sq2 = stack_64.squared();
                f32 _64dot8 = stack_64.dot(stack_8);
                f32 ff1 = sq2*sq1;
                f32 ff2 = _64dot8/sq2;
                f32 squ3 = JGeometry::TUtil<f32>::sqrt((ff1-(_64dot8*_64dot8))/sq2);

                if (0.0f <= ff2 && ff2 <= 1.0f && squ3 < (10.0f+f31) && MR::sendMsgEnemyAttackFire(pReceiver, pSender))
                    return;
            }
        }
    }
}

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

void LavaSteam::exeWait() {
    if (MR::isFirstStep(this)) {
        MR::invalidateHitSensors(this);
        MR::emitEffect(this, "Sign");
        _98.x = 1.0f;
        _98.y = 1.0f;
        _98.z = 1.0f;
    }

    if (MR::isGreaterStep(this, 82)) {
        f32 c = 90-getNerveStep();
        _98.setAll<f32>(MR::getEaseInValue(c*=0.125f, 0.001, 1.0, 1.0));
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
