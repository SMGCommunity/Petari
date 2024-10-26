#include "Game/MapObj/KeySwitch.hpp"
#include "JSystem/JMath/JMath.hpp"

KeySwitch::~KeySwitch() {

}

KeySwitch::KeySwitch(const char *pName) : LiveActor(pName) {
    mCameraInfo = 0;
    mCurDemoFrame = -1;
}

void KeySwitch::init(const JMapInfoIter &rIter) {
    MR::initDefaultPos(this, rIter);
    initModelManagerWithAnm("KeySwitch", 0, false);
    MR::connectToSceneMapObjDecoration(this);
    initHitSensor(1);
    TVec3f box;
    box.x = 0.0f;
    box.y = 105.0f;
    box.z = 0.0f;
    MR::addHitSensorMapObjSimple(this, "body", 8, 105.0f, box);
    initBinder(105.0f, 105.0f, 0);
    initEffectKeeper(0, 0, false);
    initSound(4, false);
    MR::initShadowVolumeCylinder(this, 90.0f);

    if (MR::isValidInfo(rIter)) {
        MR::initActorCamera(this, rIter, &mCameraInfo);
    }

    bool usesSwitch = false;

    if (MR::isValidInfo(rIter)) {
        bool uses = MR::useStageSwitchReadAppear(this, rIter);
        if (uses) {
            usesSwitch = true;
            MR::syncStageSwitchAppear(this);
        }

        MR::needStageSwitchWriteA(this, rIter);
    }

    if (usesSwitch || !MR::isValidInfo(rIter)) {
        initNerve(&NrvKeySwitch::KeySwitchNrvDemoStart::sInstance);
        
    }
    else {
        initNerve(&NrvKeySwitch::KeySwitchNrvWait::sInstance);
    }

    if (usesSwitch || !MR::isValidInfo(rIter)) {
        makeActorDead();
    }
    else {
        makeActorAppeared();
    }
}

void KeySwitch::initKeySwitchByOwner(const JMapInfoIter &rIter) {
    initWithoutIter();
    MR::needStageSwitchWriteA(this, rIter);
    MR::initActorCamera(this, rIter, &mCameraInfo);
}

void KeySwitch::appearKeySwitch(const TVec3f &rVec) {
    MR::resetPosition(this, rVec);
    MR::onCalcGravity(this);
    MR::invalidateClipping(this);
    appear();
}

void KeySwitch::exeDemoStart() {
    if (!mCameraInfo) {
        setNerve(&NrvKeySwitch::KeySwitchNrvAppear::sInstance);
        return;
    }

    if (MR::tryStartDemoWithoutCinemaFrame(this, cDemoName)) {
        MR::startActorCameraTargetSelf(this, mCameraInfo, -1);
        mCurDemoFrame = 0;
        setNerve(&NrvKeySwitch::KeySwitchNrvAppear::sInstance);
    }
}

void KeySwitch::exeAppear() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "Rotation", 0);
        mVelocity.scale(-40.0f, mGravity);
        MR::invalidateClipping(this);
        MR::startSound(this, "SE_OJ_KEY_SWITCH_APPEAR", -1, -1);
    }

    MR::setBckRate(this, MR::calcNerveValue(this, 0xB4, 3.0f, 1.5f));
    MR::addVelocityToGravity(this, 2.0f);

    bool val = false;

    if (MR::isBindedGround(this)) {
        if (PSVECMag(mVelocity.toCVec()) >= 10.0f) {
            s32 mag = PSVECMag(mVelocity.toCVec());
            mag *= 2;
            if (mag > 0x64) {
                mag = 0x64;
            }

            MR::startSound(this, "SE_OJ_KEY_SWITCH_BOUND", mag, -1);
            TVec3f neg;
            neg.negateInline_2(mGravity);
            MR::calcReboundVelocity(&mVelocity, neg, 0.60000002f, 0.69999999f);
        }
        else {
            val = true;
        }
    }

    if (!tryAvoid()) {
        if (val) {
            MR::validateClipping(this);
            setNerve(&NrvKeySwitch::KeySwitchNrvWait::sInstance);
        }
    }
}

void KeySwitch::exeWait() {
    if (MR::isFirstStep(this)) {
        MR::tryStartBck(this, "Rotation", 0);
        MR::setBckRate(this, 1.0f);
        mVelocity.x = 0.0f;
        mVelocity.y = 0.0f;
        mVelocity.z = 0.0f;
        MR::offCalcGravity(this);
    }
}

void KeySwitch::appear() {
    LiveActor::appear();
}

void KeySwitch::kill() {
    MR::onSwitchA(this);
    MR::startSystemSE("SE_SY_READ_RIDDLE_S", -1, -1);
    MR::emitEffect(this, "Get");
    LiveActor::kill();
}

void KeySwitch::calcAndSetBaseMtx() {
    TVec3f front;
    MR::calcFrontVec(&front, this);
    TPos3f mtx;
    MR::calcMtxFromGravityAndZAxis(&mtx, this, mGravity, front);
    MR::setBaseTRMtx(this, mtx);
}

void KeySwitch::control() {
    if (mCurDemoFrame != -1 && mCameraInfo) {
        if (mCurDemoFrame >= 0x28) {
            MR::endActorCamera(this, mCameraInfo, false, -1);
            MR::endDemo(this, cDemoName);
            mCameraInfo = 0;
            mCurDemoFrame = -1;
        }
        else {
            mCurDemoFrame = mCurDemoFrame + 1;
        }
    }
}

bool KeySwitch::receiveMsgPlayerAttack(u32 msg, HitSensor *a1, HitSensor *a2) {
    return MR::isMsgStarPieceReflect(msg);
}

bool KeySwitch::receiveOtherMsg(u32 msg, HitSensor *a1, HitSensor *a2) {
    if (MR::isMsgStarPieceReflect(msg)) {
        return 1;
    }

    if (isNerve(&NrvKeySwitch::KeySwitchNrvAppear::sInstance) && MR::isLessEqualStep(this, 0x3C)) {
        return 0;
    }

    if (MR::isMsgItemGet(msg)) {
        MR::startSound(this, "SE_OJ_KEY_SWITCH_GET", -1, -1);
        MR::tryRumblePadMiddle(this, 0);
        kill();
        return 1;
    }

    return 0;
}

bool KeySwitch::tryAvoid() {
    LiveActor* sensorActor;
    HitSensor* sensor = 0;

    if (MR::isBindedGround(this)) {
        sensor = MR::getGroundSensor(this);
    }
    else if (MR::isBindedWall(this)) {
        sensor = MR::getWallSensor(this);
    }

    if (!sensor) {
        return false;
    }

    if (sensor->mSensorType != 0x58) {
        return false;
    }
    
    sensorActor = sensor->mActor;
    TVec3f up;
    MR::calcUpVec(&up, sensorActor);
    TVec3f thing;
    thing.subInline2(mPosition, sensorActor->mPosition);
    TVec3f stack_8;
    JMAVECScaleAdd(up.toCVec(), thing.toCVec(), stack_8.toVec(), -up.dot(thing));

    if (MR::normalizeOrZero(&stack_8)) {
        MR::calcFrontVec(&stack_8, sensorActor);
    }

    mVelocity.scale(10.0f, stack_8);
    return true;
}

namespace NrvKeySwitch {
    INIT_NERVE(KeySwitchNrvDemoStart);
    INIT_NERVE(KeySwitchNrvAppear);
    INIT_NERVE(KeySwitchNrvWait);

    void KeySwitchNrvWait::execute(Spine *pSpine) const {
        KeySwitch* key = reinterpret_cast<KeySwitch*>(pSpine->mExecutor);
        key->exeWait();
    }

    void KeySwitchNrvAppear::execute(Spine *pSpine) const {
        KeySwitch* key = reinterpret_cast<KeySwitch*>(pSpine->mExecutor);
        key->exeAppear();
    }

    void KeySwitchNrvDemoStart::execute(Spine *pSpine) const {
        KeySwitch* key = reinterpret_cast<KeySwitch*>(pSpine->mExecutor);
        key->exeDemoStart();
    }
};