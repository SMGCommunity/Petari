#include "Game/MapObj/CoinGroup.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/MapObj/Coin.hpp"
#include "Game/Util.hpp"

namespace {
    static const s32 sAppearCameraTime = 30;
    static const s32 sAppearDelayTime = 3;
    static const s32 sDemoAppearTime = 90;
};  // namespace

namespace NrvCoinGroup {
    NEW_NERVE(CoinGroupNrvAppear, CoinGroup, Appear);
    NEW_NERVE(CoinGroupNrvTryStartDemo, CoinGroup, TryStartDemo);
    NEW_NERVE(CoinGroupNrvDemoAppear, CoinGroup, DemoAppear);
    NEW_NERVE(CoinGroupNrvKill, CoinGroup, Kill);
};  // namespace NrvCoinGroup

CoinGroup::CoinGroup(const char* pName) : LiveActor(pName), mCoinArray(), mCameraInfo(), mCoinCount(), mTimeLimit(-1), mIsPurpleCoinGroup() {
}

void CoinGroup::init(const JMapInfoIter& rIter) {
    s32 count = 0;
    MR::getJMapInfoArg0NoInit(rIter, &count);
    mCoinCount = count;
    mTimeLimit = -1;
    MR::getJMapInfoArg1NoInit(rIter, &mTimeLimit);
    mCoinArray = new Coin*[mCoinCount];

    for (u32 i = 0; i < mCoinCount; i++) {
        if (mIsPurpleCoinGroup) {
            mCoinArray[i] = static_cast< Coin* >(MR::createPurpleCoin(this, getCoinName()));
        } else {
            mCoinArray[i] = static_cast< Coin* >(MR::createCoin(this, getCoinName()));
        }

        mCoinArray[i]->setShadowAndPoseModeFromJMapIter(rIter);

        if (mCoinArray[i]->_BA) {
            MR::initDefaultPos(mCoinArray[i], rIter);
        }

        Coin* coin = mCoinArray[i];
        coin->mScale.set(1.0f);
        mCoinArray[i]->initWithoutIter();
    }

    initCoinArray(rIter);
    placementCoin();

    if (MR::tryRegisterDemoCast(this, rIter)) {
        MR::registerDemoActionFunctor(this, MR::Functor(this, &CoinGroup::appearCoinAll), "コイン出現");
    } else if (MR::useStageSwitchReadAppear(this, rIter)) {
        MR::connectToSceneMapObjMovement(this);
        MR::syncStageSwitchAppear(this);
        MR::initActorCamera(this, rIter, &mCameraInfo);

        if (MR::isExistActorCamera(mCameraInfo)) {
            initNerve(GET_NERVE(CoinGroup, CoinGroupNrvDemoAppear));
        } else {
            initNerve(GET_NERVE(CoinGroup, CoinGroupNrvAppear));
        }
    } else {
        appearCoinFix();
    }

    if (MR::useStageSwitchReadB(this, rIter)) {
        MR::listenStageSwitchOnB(this, MR::Functor(this, &CoinGroup::killCoinAll));
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
    if (mTimeLimit < 0) {
        appearCoinFix();
    } else {
        appearCoinAllTimer();
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

void CoinGroup::setCoinTrans(s32 coinIndex, const TVec3f& rPos) {
    Coin* coin = mCoinArray[coinIndex];
    coin->mPosition.x = rPos.x;
    coin->mPosition.y = rPos.y;
    coin->mPosition.z = rPos.z;
}

void CoinGroup::appear() {
    LiveActor::appear();

    if (isNerve(GET_NERVE(CoinGroup, CoinGroupNrvDemoAppear))) {
        MR::requestStartDemo(this, "出現", GET_NERVE(CoinGroup, CoinGroupNrvDemoAppear), GET_NERVE(CoinGroup, CoinGroupNrvTryStartDemo));
    }
}

void CoinGroup::exeAppear() {
    if (MR::isStep(this, ::sAppearDelayTime)) {
        if (mIsPurpleCoinGroup) {
            MR::startSystemSE("SE_SY_PURPLE_COIN_APPEAR");
        } else {
            MR::startSystemSE("SE_SY_COIN_APPEAR");
        }

        appearCoinAll();
        setNerve(GET_NERVE(CoinGroup, CoinGroupNrvKill));
        kill();
    }
}

void CoinGroup::exeTryStartDemo() {
}

void CoinGroup::exeDemoAppear() {
    if (MR::isFirstStep(this)) {
        MR::startActorCameraTargetSelf(this, mCameraInfo, ::sAppearCameraTime);
        MR::startSystemSE("SE_SY_COIN_APPEAR");
        appearCoinAll();
    }

    if (MR::isGreaterStep(this, ::sDemoAppearTime)) {
        MR::endDemo(this, "出現");
        MR::endActorCamera(this, mCameraInfo, false, -1);
        setNerve(GET_NERVE(CoinGroup, CoinGroupNrvKill));
        kill();
    }
}

void CoinGroup::exeKill() {
    kill();
}
