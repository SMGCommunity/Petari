
#include "Game/Enemy/BegomanLauncher.hpp"
#include "Game/Enemy/BegomanBaby.hpp"
#include "Game/Enemy/BegomanSpike.hpp"
#include "Game/Enemy/BegomanSpring.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/Util/ActorShadowUtil.hpp"
#include "Game/Util/ActorSwitchUtil.hpp"
#include "Game/Util/EffectUtil.hpp"
#include "Game/Util/GravityUtil.hpp"
#include "Game/Util/JMapUtil.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/ObjUtil.hpp"
#include "Game/Util/SoundUtil.hpp"
#include "Game/Util/StringUtil.hpp"

namespace NrvBegomanLauncher {
    NEW_NERVE(HostTypeNrvWaitAfterSwitchOn, BegomanLauncher, WaitAfterSwitchOn);
    NEW_NERVE(HostTypeNrvWait, BegomanLauncher, Wait);
    NEW_NERVE(HostTypeNrvWaitForLaunch, BegomanLauncher, WaitForLaunch);
    NEW_NERVE(HostTypeNrvLaunch, BegomanLauncher, Launch);
    NEW_NERVE(HostTypeNrvAppear, BegomanLauncher, Appear);
    NEW_NERVE(HostTypeNrvDisappear, BegomanLauncher, Disappear);
}  // namespace NrvBegomanLauncher

namespace {
    const s32 sLaunchTime = 0;
}

BegomanLauncher::BegomanLauncher(const char* pName) : LiveActor(pName), mBegomanCount(), mAppearDelay(), mBegomanArray(), mLaunchType(3) {
}

void BegomanLauncher::init(const JMapInfoIter& rIter) {
    MR::initDefaultPos(this, rIter);
    s32 count = 1;
    MR::getJMapInfoArg3NoInit(rIter, &count);
    mBegomanCount = count;
    MR::getJMapInfoArg2NoInit(rIter, &mAppearDelay);
    const char* pObjectName = nullptr;
    MR::getObjectName(&pObjectName, rIter);
    mBegomanArray = new BegomanBase*[mBegomanCount];
    if (MR::isEqualString(pObjectName, "TogeBegomanLauncher")) {
        setName("トゲベーゴマンランチャー");
        for (s32 i = 0; i < mBegomanCount; i++) {
            mBegomanArray[i] = new BegomanSpike("トゲベーゴマン");
            mBegomanArray[i]->mPosition.set(mPosition);
            mBegomanArray[i]->init(rIter);
            mBegomanArray[i]->makeActorDead();
        }
    } else if (MR::isEqualString(pObjectName, "BegomanBabyLauncher")) {
        setName("ベビーベーゴマンランチャー");
        mLaunchType = 2;
        MR::declareCoin(this, mBegomanCount);
        for (s32 i = 0; i < mBegomanCount; i++) {
            mBegomanArray[i] = new BegomanBaby(this, "ランチャーベビー");
            mBegomanArray[i]->mPosition.set(mPosition);
            mBegomanArray[i]->init(rIter);
            mBegomanArray[i]->makeActorDead();
        }
    } else {
        for (s32 i = 0; i < mBegomanCount; i++) {
            mBegomanArray[i] = new BegomanSpring("バネベーゴマン");
            mBegomanArray[i]->mPosition.set(mPosition);
            mBegomanArray[i]->init(rIter);
            mBegomanArray[i]->makeActorDead();
        }
    }

    initModelManagerWithAnm("BegomanLauncher", nullptr, false);
    MR::connectToSceneEnemy(this);
    initNerve(GET_NERVE(BegomanLauncher, HostTypeNrvWait));
    initEffectKeeper(0, "BegomanLauncher", false);
    initSound(8, false);
    MR::initShadowVolumeSphere(this, 100.0f);
    if (MR::useStageSwitchReadAppear(this, rIter)) {
        MR::listenStageSwitchOnAppear(this, MR::Functor(this, &BegomanLauncher::onSwitchAppear));
        makeActorDead();
    } else {
        appear();
    }

    if (MR::useStageSwitchReadB(this, rIter)) {
        MR::listenStageSwitchOnB(this, MR::Functor(this, &BegomanLauncher::kill));
    }
}

void BegomanLauncher::initAfterPlacement() {
    MR::calcGravity(this);
}

void BegomanLauncher::kill() {
    LiveActor::kill();
    for (s32 i = 0; i < mBegomanCount; i++) {
        if (!MR::isDead(mBegomanArray[i])) {
            mBegomanArray[i]->kill();
        }
    }

    MR::deleteEffect(this, "Launcher");
}

void BegomanLauncher::onSwitchAppear() {
    if (MR::isDead(this)) {
        setNerve(GET_NERVE(BegomanLauncher, HostTypeNrvWaitAfterSwitchOn));
        appear();
    }
}

void BegomanLauncher::exeWaitAfterSwitchOn() {
    if (MR::isGreaterStep(this, mAppearDelay)) {
        setNerve(GET_NERVE(BegomanLauncher, HostTypeNrvLaunch));
    }
}

void BegomanLauncher::exeWait() {
    if (MR::isFirstStep(this)) {
        MR::hideModel(this);
    }

    if (mLaunchType == 2) {
        for (s32 i = 0; i < mBegomanCount; i++) {
            if (!MR::isDead(mBegomanArray[i])) {
                return;
            }
        }

        setNerve(GET_NERVE(BegomanLauncher, HostTypeNrvWaitForLaunch));
    } else {
        for (s32 i = 0; i < mBegomanCount; i++) {
            if (MR::isDead(mBegomanArray[i])) {
                setNerve(GET_NERVE(BegomanLauncher, HostTypeNrvWaitForLaunch));
                return;
            }
        }
    }
}

void BegomanLauncher::exeWaitForLaunch() {
    if (MR::isGreaterStep(this, 180)) {
        setNerve(GET_NERVE(BegomanLauncher, HostTypeNrvAppear));
    }
}

void BegomanLauncher::exeLaunch() {
    if (MR::isFirstStep(this)) {
        MR::showModel(this);
        MR::emitEffect(this, "Launcher");
        MR::startSound(this, "SE_EM_BEGOMANLAUN_LAUNCH");
    }

    if (MR::isGreaterStep(this, sLaunchTime)) {
        if (mLaunchType == 2) {
            BegomanBase::launchBegomanBabyLauncher(this, reinterpret_cast< BegomanBaby** >(mBegomanArray), mBegomanCount, 100.0f, 10.0f, 30.0f,
                                                   nullptr);
            setNerve(GET_NERVE(BegomanLauncher, HostTypeNrvDisappear));
        } else {
            for (s32 i = 0; i < mBegomanCount; i++) {
                if (MR::isDead(mBegomanArray[i])) {
                    MR::calcGravity(this);
                    mBegomanArray[i]->mPosition.set(mPosition);
                    mBegomanArray[i]->mVelocity.set(mGravity * -30.0f);
                    mBegomanArray[i]->appear();
                    setNerve(GET_NERVE(BegomanLauncher, HostTypeNrvDisappear));
                    return;
                }
            }
        }
    }
}

void BegomanLauncher::exeAppear() {
    if (MR::isFirstStep(this)) {
        MR::showModel(this);
        MR::emitEffect(this, "Launcher");
        MR::startBck(this, "BegomanLauncher");
    }

    MR::startLevelSound(this, "SE_EM_LV_BEGOMANLAUN_APPEAR");
    if (MR::isGreaterStep(this, 90)) {
        MR::stopBck(this);
        setNerve(GET_NERVE(BegomanLauncher, HostTypeNrvLaunch));
    }
}

void BegomanLauncher::exeDisappear() {
    if (MR::isFirstStep(this)) {
        MR::showModel(this);
        MR::startBck(this, "BegomanLauncher");
        MR::setBckFrame(this, 90.0f);
        MR::deleteEffect(this, "Launcher");
    }

    if (MR::isGreaterStep(this, 90)) {
        setNerve(GET_NERVE(BegomanLauncher, HostTypeNrvWait));
    }
}
