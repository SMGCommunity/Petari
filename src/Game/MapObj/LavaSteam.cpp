#include "Game/MapObj/LavaSteam.hpp"

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
        MR::listenStageSwitchOnA(this, MR::Functor(this, &startSteam));
    }
    else if (MR::tryRegisterDemoCast(this, rIter)) {
        setNerve(&NrvLavaSteam::HostTypeWaitForSwitchOn::sInstance);
        MR::registerDemoActionFunctor(this, MR::Functor(this, &startSteam), 0);
    }

    MR::useStageSwitchSleep(this, rIter);
    initSound(4, 0);
    MR::connectToSceneNoSilhouettedMapObj(this);
    makeActorAppeared();
}

#ifdef NON_MATCHING
// A lot is wrong here
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
    mtx.mMtx[0][2] = calc1;
    mtx.mMtx[0][3] = ((cosx * cosz) * siny) + (sinx2 * sinz);
    mtx.mMtx[2][0] = -siny;
    mtx.mMtx[2][2] = (cosx * cosy);
    mtx.mMtx[1][2] = ((cosx * sinz) * siny) - (sinx2 * cosz);
    
    _8C.set(calc1, calc2, calc3);
    //_98.set(mtx.mMtx[0][1], mtx.mMtx[1][1], (sinz*cosy));
    MR::normalize(&_8C);
}
#endif

#ifdef NON_MATCHING
// Incomplete
void LavaSteam::attackSensor(HitSensor* pSender, HitSensor* pReceiver) {

    if (!isNerve(&NrvLavaSteam::HostTypeWait::sInstance) && !isNerve(&NrvLavaSteam::HostTypeWaitForSwitchOn::sInstance)) {
        if (MR::isSensorPlayerOrRide(pReceiver)) {
            f32 f = 0.69999999f*pReceiver->mRadius;

            if (isNerve(&NrvLavaSteam::HostTypeSteam::sInstance)) {
                TVec3f stack_30 = _8C*400.0f;
                TVec3f stack_3C = TVec3f(mPosition);
                TVec3f stack_48; 
            }
        }
    } // END OF FUNC
}
#endif

void LavaSteam::startClipped() {
    LiveActor::startClipped();
    MR::deleteEffectAll(this);
}

#ifdef NON_MATCHING
// Instruction swap
void LavaSteam::endClipped() {
    LiveActor::endClipped();
    isNerve(&NrvLavaSteam::HostTypeWaitForSwitchOn::sInstance);
}

void LavaSteam::startSteam() {
    setNerve(&NrvLavaSteam::HostTypeSteam::sInstance);
}
#endif

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
        _98.setAll(MR::getEaseInValue((90-getNerveStep())*0.125f, 0.001f, 1.0f, 1.0f));
    }

    if (MR::isGreaterStep(this, 90))
        MR::forceDeleteEffect(this, "Sign");

    if (MR::isGreaterStep(this, 120))
        setNerve(&NrvLavaSteam::HostTypeSteam::sInstance);
}
#endif

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

    HostTypeSteamEnd(HostTypeSteamEnd::sInstance);
    HostTypeSteam(HostTypeSteam::sInstance);
    HostTypeWait(HostTypeWait::sInstance);
    HostTypeWaitForSwitchOn(HostTypeWaitForSwitchOn::sInstance);
}