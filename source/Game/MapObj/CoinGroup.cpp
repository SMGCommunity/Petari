#include "Game/MapObj/CoinGroup.hpp"
#include "Game/Util.hpp"

CoinGroup::CoinGroup(const char *pName) : LiveActor(pName) {
    mCoinArray = nullptr;
    mCameraInfo = nullptr;
    mCoinCount = 0;
    mTimeLimit = -1;
    mIsPurpleCoinGroup = false;
}

void CoinGroup::init(const JMapInfoIter &rIter) {
    s32 count = 0;
    MR::getJMapInfoArg0NoInit(rIter, &count);
    mCoinCount = count;
    mTimeLimit = -1;
    MR::getJMapInfoArg1NoInit(rIter, &mTimeLimit);
    mCoinArray = new Coin*[mCoinCount];

    for (u32 i = 0; i < mCoinCount; i++) {
        if (mIsPurpleCoinGroup) {
            mCoinArray[i] = reinterpret_cast<Coin*>(MR::createPurpleCoin(this, getCoinName()));
        }
        else {
            mCoinArray[i] = reinterpret_cast<Coin*>(MR::createCoin(this, getCoinName()));
        }

        mCoinArray[i]->setShadowAndPoseModeFromJMapIter(rIter);

        if (mCoinArray[i]->_BA) {
            MR::initDefaultPos(mCoinArray[i], rIter);
        }

        Coin* coin = mCoinArray[i];

        coin->mScale.x = 1.0f;
        coin->mScale.y = 1.0f;
        coin->mScale.z = 1.0f;
        mCoinArray[i]->initWithoutIter();
    }

    initCoinArray(rIter);
    placementCoin();

    if (MR::tryRegisterDemoCast(this, rIter)) {
        MR::FunctorV0M<CoinGroup *, void (CoinGroup::*)()> demoFunc = MR::Functor<CoinGroup>(this, &CoinGroup::appearCoinAll);
        MR::registerDemoActionFunctor(this, demoFunc, "コイン出現");
    }
    else if (MR::useStageSwitchReadAppear(this, rIter)) {
        MR::connectToSceneMapObjMovement(this);
        MR::syncStageSwitchAppear(this);
        MR::initActorCamera(this, rIter, &mCameraInfo);

        if (MR::isExistActorCamera(mCameraInfo)) {
            initNerve(&NrvCoinGroup::CoinGroupNrvDemoAppear::sInstance);
        }
        else {
            initNerve(&NrvCoinGroup::CoinGroupNrvAppear::sInstance);
        }
    }
    else {
        appearCoinFix();
    }

    if (MR::useStageSwitchReadB(this, rIter)) {
        MR::FunctorV0M<CoinGroup *, void (CoinGroup::*)()> killFunc = MR::Functor<CoinGroup>(this, &CoinGroup::killCoinAll);
        MR::listenStageSwitchOnB(this, killFunc);
    }

    MR::invalidateClipping(this);
    makeActorDead();
}

void CoinGroup::killCoinAll() {
    for (u32 i = 0; i < mCoinCount; i++) {
        mCoinArray[i]->kill();
    }
}

void CoinGroup::appearCoinAll() {
    if (mTimeLimit >= 0) {
        appearCoinAllTimer();
    }
    else {
        appearCoinFix();
    }
}

void CoinGroup::appearCoinFix() {
    for (u32 i = 0; i < mCoinCount; i++) {
        mCoinArray[i]->appear();
    }
}

void CoinGroup::appearCoinAllTimer() {
    for (u32 i = 0; i < mCoinCount; i++) {
        mCoinArray[i]->appearFixTimer(mTimeLimit, -1);
    }
}

void CoinGroup::setCoinTrans(s32 coinIndex, const TVec3f &rPos) {
    Coin* coin = mCoinArray[coinIndex];
    coin->mPosition.x = rPos.x;
    coin->mPosition.y = rPos.y;
    coin->mPosition.z = rPos.z;
}

void CoinGroup::appear() {
    LiveActor::appear();
    
    if (isNerve(&NrvCoinGroup::CoinGroupNrvDemoAppear::sInstance)) {
        MR::requestStartDemo(this, "出現", &NrvCoinGroup::CoinGroupNrvDemoAppear::sInstance, &NrvCoinGroup::CoinGroupNrvTryStartDemo::sInstance);
    }
}

void CoinGroup::exeAppear() {
    if (MR::isStep(this, 3)) {
        if (mIsPurpleCoinGroup) {
            MR::startSystemSE("SE_SY_PURPLE_COIN_APPEAR", -1, -1);
        }
        else {
            MR::startSystemSE("SE_SY_COIN_APPEAR", -1, -1);
        }

        appearCoinAll();
        setNerve(&NrvCoinGroup::CoinGroupNrvKill::sInstance);
        kill();
    }
}

void CoinGroup::exeDemoAppear() {
    if (MR::isFirstStep(this)) {
        MR::startActorCameraTargetSelf(this, mCameraInfo, 30);
        MR::startSystemSE("SE_SY_COIN_APPEAR", -1, -1);
        appearCoinAll();
    }

    if (MR::isGreaterStep(this, 90)) {
        MR::endDemo(this, "出現");
        MR::endActorCamera(this, mCameraInfo, false, -1);
        setNerve(&NrvCoinGroup::CoinGroupNrvKill::sInstance);
        kill();
    }
}

namespace NrvCoinGroup {
    INIT_NERVE(CoinGroupNrvAppear);
    INIT_NERVE(CoinGroupNrvTryStartDemo);
    INIT_NERVE(CoinGroupNrvDemoAppear);
    INIT_NERVE(CoinGroupNrvKill);

    void CoinGroupNrvKill::execute(Spine *pSpine) const {
        CoinGroup* coin = reinterpret_cast<CoinGroup*>(pSpine->mExecutor);
        coin->kill();
    }

    void CoinGroupNrvDemoAppear::execute(Spine *pSpine) const {
        CoinGroup* coin = reinterpret_cast<CoinGroup*>(pSpine->mExecutor);
        coin->exeDemoAppear();
    }

    void CoinGroupNrvTryStartDemo::execute(Spine *pSpine) const {

    }

    void CoinGroupNrvAppear::execute(Spine *pSpine) const {
        CoinGroup* coin = reinterpret_cast<CoinGroup*>(pSpine->mExecutor);
        coin->exeAppear();
    }
};

const char* CoinGroup::getCoinName() const {
    return "コイン(グループ配置)";
}

void CoinGroup::placementCoin() {

}