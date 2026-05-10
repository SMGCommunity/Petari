#include "Game/Enemy/BombHeiLauncher.hpp"
#include "Game/MapObj/MapObjConnector.hpp"

namespace {
    static const f32 hThrowVel = 35.0f;
    static const f32 hThrowFrontVel = 1.0f;
    static const s32 hCountTimer = 60;
}  // namespace

namespace NrvBombHeiLauncher {
    NEW_NERVE(HostTypeNrvLauncherWait, BombHeiLauncher, LauncherWait);
    NEW_NERVE(HostTypeNrvStop, BombHeiLauncher, Stop);
}  // namespace NrvBombHeiLauncher

BombHeiLauncher::BombHeiLauncher(const char* pName) : LiveActor(pName), mBomb(nullptr), mCountdown(0), mConnector(nullptr), mIsActive(true) {
    mConnector = new MapObjConnector(this);
}

void BombHeiLauncher::init(const JMapInfoIter& rIter) {
    bool isBombHeiLauncher = true;
    if (MR::isValidInfo(rIter)) {
        MR::initDefaultPos(this, rIter);

        const char* objectName = nullptr;
        MR::getObjectName(&objectName, rIter);
        if (MR::isEqualString(objectName, "BombLauncher")) {
            isBombHeiLauncher = false;
        }

        if (MR::useStageSwitchReadA(this, rIter)) {
            mIsActive = false;
            MR::listenStageSwitchOnOffA(this, MR::Functor(this, &BombHeiLauncher::onSwitchA), MR::Functor(this, &BombHeiLauncher::offSwitchA));
        }

        MR::useStageSwitchSleep(this, rIter);
    }

    initModelManagerWithAnm("BombHeiLauncher", nullptr, false);
    MR::connectToSceneCollisionEnemy(this);
    MR::initLightCtrl(this);
    MR::onCalcGravity(this);

    initSound(6, false);
    initEffectKeeper(0, nullptr, false);

    initHitSensor(2);
    MR::addBodyMessageSensorEnemy(this);
    MR::initCollisionParts(this, "BombHeiLauncher", getSensor("body"), nullptr);
    MR::onCalcGravity(this);

    initNerve(&NrvBombHeiLauncher::HostTypeNrvStop::sInstance);
    mCountdown = -1;

    MR::validateClipping(this);
    MR::setGroupClipping(this, rIter, 32);
    makeActorDead();

    if (isBombHeiLauncher) {
        mBomb = new BombHei("ボム兵");
        mBomb->mType = BombHei::BOMB_HEI;
    } else {
        mBomb = new BombHei("ボム");
        mBomb->mType = BombHei::BOMB;
    }

    mBomb->mPosition.set(mPosition);
    mBomb->initWithoutIter();
    mBomb->setGroupClipping(rIter);

    s32 ignoreCollisionArg = -1;
    MR::getJMapInfoArg2NoInit(rIter, &ignoreCollisionArg);
    if (ignoreCollisionArg != -1) {
        mBomb->mExplosionIgnoreCollision = true;
    }

    mBomb->kill();
}

void BombHeiLauncher::initAfterPlacement() {
    TVec3f up;
    MR::calcUpVec(&up, this);
    MR::resetPosition(mBomb, mPosition.addOperatorInLine(up * 30.0f));
    mBomb->mGravity.set(up.invertOperatorInternal());
    mBomb->initAfterPlacement();
    mConnector->attachToUnder();
    makeActorAppeared();
}

void BombHeiLauncher::startCountdown() {
    if (mCountdown != -1) {
        return;
    }

    mCountdown = hCountTimer;
}

void BombHeiLauncher::onSwitchA() {
    mIsActive = true;
    mCountdown = 0;
    setNerve(&NrvBombHeiLauncher::HostTypeNrvLauncherWait::sInstance);
}

void BombHeiLauncher::offSwitchA() {
    mIsActive = false;
    mCountdown = -1;
    setNerve(&NrvBombHeiLauncher::HostTypeNrvStop::sInstance);

    if (mBomb != nullptr && !MR::isDead(mBomb)) {
        mBomb->kill();
    }
}

void BombHeiLauncher::exeLauncherWait() {
    // FIXME: what
    if (MR::isFirstStep(this)) {
    }

    if (!mIsActive) {
        return;
    }

    if (mCountdown == 0) {
        if (MR::isOnPlayerShadow(this) || MR::isOnPlayer(this)) {
            return;
        }

        MR::startBck(this, "Open", nullptr);
        MR::startSound(this, "SE_EM_BOMBLAUN_LAUNCH", -1, -1);

        mBomb->appear();
        MR::resetPosition(mBomb, mPosition);

        TVec3f up, front;
        MR::calcUpVec(&up, this);
        MR::calcFrontVec(&front, this);
        getBombVelocity()->set(up * hThrowVel);
        getBombVelocity()->add(front * hThrowFrontVel);
        setNerve(&NrvBombHeiLauncher::HostTypeNrvStop::sInstance);
        return;
    }

    if (MR::isDead(mBomb)) {
        startCountdown();
    }
}

void BombHeiLauncher::exeStop() {
    if (MR::isFirstStep(this)) {
        mCountdown = -1;
    }

    if (mIsActive && MR::isGreaterStep(this, 120)) {
        setNerve(&NrvBombHeiLauncher::HostTypeNrvLauncherWait::sInstance);
    }
}

void BombHeiLauncher::calcAndSetBaseMtx() {
    LiveActor::calcAndSetBaseMtx();
    mConnector->connect();
}

void BombHeiLauncher::control() {
    if (mCountdown <= 0) {
        return;
    }

    mCountdown--;
}
