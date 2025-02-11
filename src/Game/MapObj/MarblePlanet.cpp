#include "Game/MapObj/MarblePlanet.hpp"
#include "Game/LiveActor/ModelObj.hpp"
#include "JSystem/JMath.hpp"

MarblePlanet::MarblePlanet(const char *pName) : LiveActor(pName) {
    mCorePlanetModel = 0;
    mPlanetElectrons = 0;
    mWatermelonCollision = 0;
    mNumElectrons = 3;
    mRemainingElectrons = 3;
}

void MarblePlanet::init(const JMapInfoIter &rIter) {
    MR::initDefaultPos(this, rIter);
    initModelManagerWithAnm("MarblePlanet", 0, false);
    MR::connectToScenePlanet(this);
    initHitSensor(1);
    MR::addHitSensorEnemy(this, "core", 8, 50.0f, TVec3f(0.0f, 0.0f, 0.0f));
    MR::initCollisionParts(this, "MarblePlanet", getSensor(0), 0);
    mWatermelonCollision = MR::createCollisionPartsFromLiveActor(this, "WaterMelon", getSensor(0), (MR::CollisionScaleType)2);
    initEffectKeeper(0, 0, false);
    initSound(4, false);
    MR::setClippingTypeSphereContainsModelBoundingBox(this, 100.0f);
    MR::setClippingFar(this, -1.0f);
    MR::needStageSwitchWriteDead(this, rIter);
    MR::getJMapInfoArg0NoInit(rIter, &mNumElectrons);
    mRemainingElectrons = mNumElectrons;
    initCoreAndElectron();
    MR::declarePowerStar(this);
    initNerve(&NrvMarblePlanet::MarblePlanetNrvWait::sInstance);
    makeActorAppeared();
}

void MarblePlanet::exeScaleUpCore() {
    if (MR::isFirstStep(this)) {
        MR::tryRumblePadMiddle(this, 0);
        MR::shakeCameraNormal();
        mRemainingElectrons = mRemainingElectrons - 1;
        switch (mRemainingElectrons) {
            case 0:
                MR::emitEffect(this, "Break");
                MR::startSound(this, "SE_OJ_MARBLE_HIT_CORE_3", -1, -1);
                MR::startSystemSE("SE_SY_MARBLE_HIT_CORE_3", -1, -1);
                break;
            case 1:
                MR::emitEffect(this, "Smoke6f");
                MR::startSound(this, "SE_OJ_MARBLE_HIT_CORE_2", -1, -1);
                MR::startSystemSE("SE_SY_MARBLE_HIT_CORE_2", -1, -1);
                break;
            case 2:
            default:
                MR::emitEffect(this, "Smoke3f");
                MR::startSound(this, "SE_OJ_MARBLE_HIT_CORE_1", -1, -1);
                MR::startSystemSE("SE_SY_MARBLE_HIT_CORE_1", -1, -1);
                break;
        }

        if (mRemainingElectrons <= 0) {
            setNerve(&NrvMarblePlanet::MarblePlanetNrvBreakCore::sInstance);
            return;
        }
        else {
            s32 electronCount = mNumElectrons;
            f32 frameMax = MR::getBckFrameMax(mCorePlanetModel);
            f32 frame = ((electronCount - mRemainingElectrons) * frameMax) / electronCount;
            MR::setBckFrameAndStop(mCorePlanetModel, frame);
            MR::setBtkFrameAndStop(mCorePlanetModel, frame);
        } 
    }

    f32 nerveRate = MR::calcNerveRate(this, 0x1E);
    f32 scale = MR::getScaleWithReactionValueZeroToOne(nerveRate, 0.5f, -0.5f);
    mCorePlanetModel->mScale.setAll<f32>(MR::getLinerValue(scale, 1.3f, 1.0f, 1.0f));
    
    if (MR::isStep(this, 0x1E)) {
        setNerve(&NrvMarblePlanet::MarblePlanetNrvWait::sInstance);
    }
}

void MarblePlanet::exeBreakCore() {
    if (MR::isFirstStep(this)) {
        MR::setBckFrameAndStop(mCorePlanetModel, MR::getBckFrameMax(mCorePlanetModel));
        MR::setBtkFrameAndStop(mCorePlanetModel, MR::getBtkFrameMax(mCorePlanetModel));
    }

    if (MR::isStep(this, 1)) {
        MR::tryRumblePadStrong(this, 0);
        MR::shakeCameraStrong();
        MR::startAfterBossBGM();
        MR::requestAppearPowerStar(this, mPosition);
        MR::hideModel(this);
        MR::invalidateCollisionParts(this);
        MR::validateCollisionParts(mWatermelonCollision);
        MR::onSwitchDead(this);
    }
}

void MarblePlanet::startClipped() {
    mCorePlanetModel->startClipped();

    for (int i = 0; i < mNumElectrons; i++) {
        mPlanetElectrons[i]->startClipped();
    }

    LiveActor::startClipped();
}

void MarblePlanet::endClipped() {
    mCorePlanetModel->endClipped();

    for (int i = 0; i < mNumElectrons; i++) {
        mPlanetElectrons[i]->endClipped();
    }

    LiveActor::endClipped();
}

void MarblePlanet::kill() {
    MR::onSwitchDead(this);
    mCorePlanetModel->kill();
    LiveActor::kill();
}

bool MarblePlanet::receiveMsgEnemyAttack(u32 msg, HitSensor *a1, HitSensor *a2) {
    if (isNerve(&NrvMarblePlanet::MarblePlanetNrvScaleUpCore::sInstance)) {
        return 0;
    }

    if (isNerve(&NrvMarblePlanet::MarblePlanetNrvBreakCore::sInstance)) {
        return 0;
    }

    setNerve(&NrvMarblePlanet::MarblePlanetNrvScaleUpCore::sInstance);
    return 1;
}

void MarblePlanet::initCoreAndElectron() {
    mCorePlanetModel = MR::createModelObjMapObj("ビー玉惑星コア", "MarblePlanetCore", getBaseMtx());
    MR::invalidateClipping(mCorePlanetModel);
    MR::startBck(mCorePlanetModel, "MarblePlanetCore", 0);
    MR::startBtk(mCorePlanetModel, "MarblePlanetCore");
    MR::setBckFrameAndStop(mCorePlanetModel, 0.0f);
    MR::setBtkFrameAndStop(mCorePlanetModel, 0.0f);
    mPlanetElectrons = new MarblePlanetElectron*[mNumElectrons];

    TVec3f front;
    MR::calcFrontVec(&front, this);
    
    TVec3f* pos;
    int i = 0;

    if (mNumElectrons > 0) {
        pos = &mPosition;
        for (; i < mNumElectrons; i++) {
                if (i != 0) {
                    TVec3f up;
                    MR::calcUpVec(&up, this);
                    MR::rotateVecDegree(&front, up, (360.0f / mNumElectrons));
                }

                TVec3f position;
                JMAVECScaleAdd(&front, pos, &position, 1000.0f);
                TVec3f rotation;
                rotation.setAll<f32>((360.0f * i) / mNumElectrons);
                mPlanetElectrons[i] = new MarblePlanetElectron(this, position, rotation, "ビー玉惑星電子");
                mPlanetElectrons[i]->initWithoutIter();
            }
    }
}

MarblePlanetElectron::MarblePlanetElectron(LiveActor *pPlanet, const TVec3f &rPosition, const TVec3f &rRotation, const char *pName) : LiveActor(pName) {
    mParentPlanet = static_cast<MarblePlanet*>(pPlanet);
    mElectronShadow = 0;
    _94.x = 0.0f;
    _94.y = 0.0f;
    _94.z = 1.0f;
    mPosition.set<f32>(rPosition);
    mRotation.set<f32>(rRotation);
}

void MarblePlanetElectron::init(const JMapInfoIter &rIter) {
    initModelManagerWithAnm("MarblePlanetElectron", 0, false);
    MR::connectToScenePlanet(this);
    initHitSensor(1);
    MR::addHitSensorEnemy(this, "body", 8, 500.0f, TVec3f(0.0f, 0.0f, 0.0f));
    initBinder(200.0f, 0.0f, 0);
    initEffectKeeper(1, 0, false);
    MR::addEffectHitNormal(this, 0);
    initSound(4, false);
    MR::invalidateClipping(this);
    mElectronShadow = new MarblePlanetElectronShadow(this, mParentPlanet->mPosition, "電子影");
    mElectronShadow->initWithoutIter();
    MR::calcGravity(this);
    mGravity.negate();

    TPos3f mtx;
    MR::makeMtxUpNoSupportPos(&mtx, mGravity, mPosition);
    MR::setBaseTRMtx(this, mtx);
    MR::calcFrontVec(&_94, this);
    MR::startBck(this, "MarblePlanetElectron", 0);
    initNerve(&NrvMarblePlanetElectron::MarblePlanetElectronNrvMove::sInstance);
    makeActorAppeared();
}

void MarblePlanetElectron::exeMove() {
    MR::turnDirectionToGround(this, &_94);
    MR::attenuateVelocity(this, 0.99000001f);
    f32 mag = PSVECMag(&mVelocity);
    f32 scale = (mag >= 13.0f ? mag : 13.0f);
    mVelocity.scale(scale, _94);
    MR::startLevelSound(this, "SE_OJ_LV_MARBLE_ROTATE", -1, -1, -1);
}

/*
void MarblePlanetElectron::exeAttack() {
    if (MR::isFirstStep(this)) {
        MR::startSound(this, "SE_OJ_MARBLE_FLIP", -1, -1);
    }

    TVec3f velocity;
    velocity.sub(mParentPlanet->mPosition, mPosition);
    MR::normalize(&velocity);
    mVelocity.scale(40.0f, velocity);
}
*/

void MarblePlanetElectron::control() {
    MR::calcGravity(this);
    mGravity.negate();

    if (isNerve(&NrvMarblePlanetElectron::MarblePlanetElectronNrvMove::sInstance)) {
        MR::restrictVelocity(this, 30.0f);
    }
}

void MarblePlanetElectron::attackSensor(HitSensor *a1, HitSensor *a2) {
    if (MR::isSensorEnemy(a2)) {
        if (MR::sendMsgEnemyAttack(a2, a1)) {
            mElectronShadow->kill();
            kill();
        }
        else {
            bool isNear = !MR::isNear(this, a2->mActor->mPosition, 440.0f);

            if (!isNear) {
                if (MR::sendMsgPush(a2, a1)) {
                    MR::tryRumblePadVeryWeak(this, 0);

                    if (!MR::isEffectValid(this, "HitMarkNormal")) {
                        MR::emitEffectHitBetweenSensors(this, a1, a2, 0.0f, 0);
                    }

                    MR::killVelocityToTarget(this, a2->mActor->mPosition);
                }
            }
        }
    }
}

bool MarblePlanetElectron::receiveMsgPlayerAttack(u32 msg, HitSensor *a2, HitSensor *a3) {
    if (isNerve(&NrvMarblePlanetElectron::MarblePlanetElectronNrvAttack::sInstance)) {
        return 0;
    }

    if (MR::isMsgPlayerHipDrop(msg)) {
        setNerve(&NrvMarblePlanetElectron::MarblePlanetElectronNrvAttack::sInstance);
        return 1;
    }

    return 0;
}

bool MarblePlanetElectron::receiveMsgPush(HitSensor *a1, HitSensor *a2) {
    if (!MR::isSensorEnemy(a1)) {
        return 0;
    }

    crashElectron(a1);
    return 1;
}

/*
void MarblePlanetElectron::crashElectron(HitSensor *pSensor) {
    TVec3f stack_8;
    stack_8.sub(pSensor->mActor->mPosition, mPosition);
    MR::normalize(&stack_8);
    JMAVECScaleAdd(&stack_8, &mVelocity, &mVelocity, -5.0f);
    MR::normalize(mVelocity, &_94);
    mVelocity.x *= 1.2f;
    mVelocity.y *= 1.2f;
    mVelocity.z *= 1.2f;
    MR::startSound(this, "SE_OJ_MARBLE_HIT_EACH", -1, -1);
}
*/

MarblePlanetElectronShadow::MarblePlanetElectronShadow(LiveActor *pElectronPtr, const TVec3f &rVec, const char *pName) : LiveActor(pName) {
    mParentElectron = static_cast<MarblePlanetElectron*>(pElectronPtr);
    _90 = &rVec;
}

void MarblePlanetElectronShadow::init(const JMapInfoIter &rIter) {
    initModelManagerWithAnm("MarblePlanetElectronShadow", 0, false);
    MR::connectToScenePlanet(this);
    MR::invalidateClipping(this);
    makeActorAppeared();
}

/*
void MarblePlanetElectronShadow::calcAndSetBaseMtx() {
    mPosition.set<f32>(*_90);
    TVec3f stack_8;
    stack_8.sub(mParentElectron->mPosition, *_90);
    MR::normalize(&stack_8);
    TPos3f up_mtx;
    MR::makeMtxUpNoSupportPos(&up_mtx, stack_8, *_90);
    MR::setBaseTRMtx(this, up_mtx);
}
*/

MarblePlanet::~MarblePlanet() {

}

MarblePlanetElectron::~MarblePlanetElectron() {

}

MarblePlanetElectronShadow::~MarblePlanetElectronShadow() {

}

namespace NrvMarblePlanet {
    INIT_NERVE(MarblePlanetNrvWait);
    INIT_NERVE(MarblePlanetNrvScaleUpCore);
    INIT_NERVE(MarblePlanetNrvBreakCore);
};

namespace NrvMarblePlanetElectron {
    INIT_NERVE(MarblePlanetElectronNrvMove);
    INIT_NERVE(MarblePlanetElectronNrvAttack);

    void MarblePlanetElectronNrvAttack::execute(Spine *pSpine) const {
        MarblePlanetElectron* electron = reinterpret_cast<MarblePlanetElectron*>(pSpine->mExecutor);
        electron->exeAttack();
    }

    void MarblePlanetElectronNrvMove::execute(Spine *pSpine) const {
        MarblePlanetElectron* electron = reinterpret_cast<MarblePlanetElectron*>(pSpine->mExecutor);
        electron->exeMove();
    }
};

namespace NrvMarblePlanet {
    void MarblePlanetNrvBreakCore::execute(Spine *pSpine) const {
        MarblePlanet* marble = reinterpret_cast<MarblePlanet*>(pSpine->mExecutor);
        marble->exeBreakCore();
    }

    void MarblePlanetNrvScaleUpCore::execute(Spine *pSpine) const {
        MarblePlanet* marble = reinterpret_cast<MarblePlanet*>(pSpine->mExecutor);
        marble->exeScaleUpCore();
    }

    void MarblePlanetNrvWait::execute(Spine *pSpine) const {

    }
};
