#include "Game/MapObj/FireBar.hpp"
#include "Game/Util.hpp"
#include "JSystem/JMath/JMath.hpp"

FireBarBall::~FireBarBall() {

}

FireBar::~FireBar() {

}

FireBarBall::FireBarBall(LiveActor *pParent) : ModelObj("ファイアバー玉", "FireBarBall", 0, 0xB, -2, -2, false) {
    mFireBarParent = pParent;
} 

void FireBarBall::init(const JMapInfoIter &rIter) {
    f32 shadowDropDist = 100.0f;
    MR::getJMapInfoArg6NoInit(rIter, &shadowDropDist);

    if (0.0f != shadowDropDist) {
        MR::initShadowVolumeCylinder(this, 55.0f);
        MR::setShadowDropLength(this, 0, shadowDropDist);
        s32 shadowDropRotate = -1;
        MR::getJMapInfoArg4NoInit(rIter, &shadowDropRotate);

        if (shadowDropRotate == 0) {
            TVec3f upVec;
            MR::calcUpVec(&upVec, mFireBarParent);
            upVec.negate();
            MR::setShadowDropDirection(this, 0, upVec);
        }

        s32 cutShadows = -1;
        MR::getJMapInfoArg2NoInit(rIter, &cutShadows);

        if (cutShadows == 0) {
            MR::setShadowVolumeEndDropOffset(this, 0, 20.0f);
            MR::onShadowVolumeCutDropLength(this, 0);
            MR::onCalcShadow(this, 0);
        }
    }

    MR::setClippingTypeSphereContainsModelBoundingBox(this, 100.0f);
    makeActorAppeared();
}

void FireBarBall::initAfterPlacement() {
    MR::calcGravity(this);
}

void FireBarBall::control() {
    controlEmitEffect();
}

void FireBarBall::startClipped() {
    MR::deleteEffect(this, "FireBarBall");
    LiveActor::startClipped();
}

void FireBarBall::endClipped() {
    LiveActor::endClipped();
}

void FireBarBall::controlEmitEffect() {
    if (MR::isNearPlayerAnyTime(this, 5000.0f)) {
        MR::emitEffect(this, "FireBarBall");
    }
    else {
        MR::deleteEffect(this, "FireBarBall");
    }
}

FireBar::FireBar(const char *pName) : LiveActor(pName) {
    mFireBalls = 0;
    mFireBallCount = 5;
    _94.x = 0.0f;
    _94.y = 0.0f;
    _94.z = 0.0f;
    mFireBarSpeed = -1.0f;
    mStickCount = 1;
    mStickDistance = 140.0f;
}

void FireBar::init(const JMapInfoIter &rIter) {
    MR::initDefaultPos(this, rIter);
    initModelManagerWithAnm("FireBarCore", 0, false);
    MR::connectToSceneMapObj(this);
    initHitSensor(2);
    MR::addBodyMessageSensorMapObj(this);
    MR::addHitSensorCallbackEnemyAttack(this, "fire", 8, 30.0f);
    MR::initCollisionParts(this, "FireBarCore", getSensor("body"), 0);
    initEffectKeeper(0, 0, false);
    initSound(3, false);
    MR::calcGravity(this);

    f32 shadowDropDist = 100.0f;
    MR::getJMapInfoArg6NoInit(rIter, &shadowDropDist);
    MR::getJMapInfoArg3NoInit(rIter, &shadowDropDist);
    shadowDropDist -= 55.0f;

    if (shadowDropDist > 0.0f) {
        TVec3f up_vec;
        TVec3f scaled_vec;
        TVec3f drop_direction;
        TVec3f shadow_box;
        shadow_box.x = 100.0f;
        shadow_box.y = 100.0f;
        shadow_box.z = 100.0f;
        MR::initShadowVolumeBox(this, shadow_box);
        MR::calcUpVec(&up_vec, this);
        JMAVECScaleAdd((const Vec*)&up_vec, (const Vec*)&mPosition, (Vec*)&scaled_vec, 50.0f);
        MR::setShadowDropPosition(this, 0, scaled_vec);
        MR::setShadowVolumeStartDropOffset(this, 0, 100.0f);
        MR::setShadowDropLength(this, 0, shadowDropDist);

        s32 rotateShadow = -1;
        MR::getJMapInfoArg4NoInit(rIter, &rotateShadow);
        if (rotateShadow == 0) {
            MR::calcUpVec(&drop_direction, this);
            drop_direction.negate();
            MR::setShadowDropDirection(this, 0, drop_direction);
        }
    }

    s32 numFireBalls = 5;
    f32 fireBallSpeed = -1.0f;
    MR::getJMapInfoArg0NoInit(rIter, &numFireBalls);
    MR::getJMapInfoArg1NoInit(rIter, &fireBallSpeed);
    MR::getJMapInfoArg5NoInit(rIter, &mStickDistance);
    MR::getJMapInfoArg7NoInit(rIter, &mStickCount);

    mFireBarSpeed = 0.1f * fireBallSpeed;
    mFireBallCount = mStickCount * numFireBalls;
    MR::calcFrontVec(&_94, this);
    initFireBarBall(rIter);
    MR::setGroupClipping(this, rIter, 0x20);
    MR::setClippingTypeSphere(this, (mFireBallCount + 1) * 100.0f);
    MR::useStageSwitchSleep(this, rIter);
    initNerve(&NrvFireBar::FireBarNrvWait::sInstance);
    makeActorAppeared();
}

void FireBar::exeWait() {
    TVec3f up_vec;
    MR::calcUpVec(&up_vec, this);
    MR::rotateVecDegree(&up_vec, _94, mFireBarSpeed);
    MR::startLevelSound(this, "SE_OJ_LV_FIRE_BAR_CENTER", -1, -1, -1);
    fixFireBarBall();
}

void FireBar::makeActorAppeared() {
    for (s32 i = 0; i < mFireBallCount; i++) {
        mFireBalls[i]->makeActorAppeared();
    }

    LiveActor::makeActorAppeared();
}

void FireBar::makeActorDead() {
    for (s32 i = 0; i < mFireBallCount; i++) {
        mFireBalls[i]->makeActorDead();
    }

    LiveActor::makeActorDead();
}

void FireBar::attackSensor(HitSensor *a1, HitSensor *a2) {
    if (MR::isSensorPlayerOrRide(a2) && MR::isSensorEnemyAttack(a1)) {
        MR::sendMsgEnemyAttackFire(a2, a1);
    }
}

// updateHitSensor__7FireBarFP9HitSensor

#ifdef NON_MATCHING
// meh
void FireBar::initFireBarBall(const JMapInfoIter &rIter) {
    mFireBalls = new FireBarBall*[mFireBallCount];
    s32 totalNum = mFireBallCount;
    totalNum /= mStickCount;
    f32 startVal = 25.0f;

    for (s32 i = 0; i < mFireBallCount; i++) {
        mFireBalls[i] = new FireBarBall(this);
        mFireBalls[i]->init(rIter);
        MR::startBtk(mFireBalls[i], "FireBarBall");
        s32 div = i / totalNum;
        div *= totalNum;

        startVal = !(i - div) ? 0.0f : startVal + 25.0f;
        s32 btkFrame = MR::getBtkFrameMax(mFireBalls[i]);
        s32 derp = startVal;
        startVal = (derp - (derp / (btkFrame * btkFrame)));
        MR::setBtkFrame(mFireBalls[i], startVal);
    }

    fixFireBarBall();
}
#endif

void FireBar::fixFireBarBall() {
    TVec3f scaled;
    scaled.scale(100.0f, _94);

    TVec3f final_pos;
    final_pos.z = 0.0f;
    final_pos.y = 0.0f;
    final_pos.x = 0.0f;

    s32 totalNum = mFireBallCount;
    totalNum /= mStickCount;
    for (s32 i = 0; i < mFireBallCount; i++) {
        s32 div = i / totalNum;
        div *= totalNum;
        if (!(i - div)) {
            TVec3f up_vec;
            MR::calcUpVec(&up_vec, this);
            MR::rotateVecDegree(&scaled, up_vec, 360.0f / mStickCount);
            TVec3f norm;
            MR::normalize(scaled, &norm);
            JMAVECScaleAdd((const Vec*)&norm, (const Vec*)&mPosition, (Vec*)&final_pos, mStickDistance);
            JMAVECScaleAdd((const Vec*)&up_vec, (const Vec*)&final_pos, (Vec*)&final_pos, 50.0f);
        }
        else {
            JMathInlineVEC::PSVECAdd((const Vec*)&final_pos, (const Vec*)&scaled, (Vec*)&final_pos);
        }

        mFireBalls[i]->mPosition.set<f32>(scaled);
    }
}

namespace NrvFireBar {
    FireBarNrvWait FireBarNrvWait::sInstance;

    void FireBarNrvWait::execute(Spine *pSpine) const {
        FireBar* bar = reinterpret_cast<FireBar*>(pSpine->mExecutor);
        bar->exeWait();
    }
};
