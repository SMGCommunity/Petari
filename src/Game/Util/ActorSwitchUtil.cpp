#include "Game/Util/ActorSwitchUtil.hpp"
#include "Game/LiveActor/LiveActor.hpp"
#include "Game/Map/ActorAppearSwitchListener.hpp"
#include "Game/Map/SleepControllerHolder.hpp"
#include "Game/Map/StageSwitch.hpp"
#include "Game/Map/SwitchWatcherHolder.hpp"
#include "Game/Util/JMapUtil.hpp"
#include "Game/Util/ObjUtil.hpp"

bool MR::useStageSwitchReadA(LiveActor* pActor, const JMapInfoIter& rIter) {
    if (!isValidInfo(rIter)) {
        return false;
    }

    if (!isExistStageSwitchA(rIter)) {
        return false;
    }

    if (pActor->mStageSwitchCtrl == nullptr) {
        pActor->initStageSwitch(rIter);
    }

    return true;
}

bool MR::useStageSwitchReadB(LiveActor* pActor, const JMapInfoIter& rIter) {
    if (!isValidInfo(rIter)) {
        return false;
    }

    if (!isExistStageSwitchB(rIter)) {
        return false;
    }

    if (pActor->mStageSwitchCtrl == nullptr) {
        pActor->initStageSwitch(rIter);
    }

    return true;
}

bool MR::useStageSwitchReadAppear(LiveActor* pActor, const JMapInfoIter& rIter) {
    if (!isValidInfo(rIter)) {
        return false;
    }

    if (!isExistStageSwitchAppear(rIter)) {
        return false;
    }

    if (pActor->mStageSwitchCtrl == nullptr) {
        pActor->initStageSwitch(rIter);
    }

    return true;
}

void MR::useStageSwitchSleep(LiveActor* pActor, const JMapInfoIter& rIter) {
    SleepControlFunc::addSleepControl(pActor, rIter);
}

// Same as useStageSwitchReadA
bool MR::useStageSwitchWriteA(LiveActor* pActor, const JMapInfoIter& rIter) {
    if (!isValidInfo(rIter)) {
        return false;
    }

    if (!isExistStageSwitchA(rIter)) {
        return false;
    }

    if (pActor->mStageSwitchCtrl == nullptr) {
        pActor->initStageSwitch(rIter);
    }

    return true;
}

// Same as useStageSwitchReadB
bool MR::useStageSwitchWriteB(LiveActor* pActor, const JMapInfoIter& rIter) {
    if (!isValidInfo(rIter)) {
        return false;
    }

    if (!isExistStageSwitchB(rIter)) {
        return false;
    }

    if (pActor->mStageSwitchCtrl == nullptr) {
        pActor->initStageSwitch(rIter);
    }

    return true;
}

bool MR::useStageSwitchWriteDead(LiveActor* pActor, const JMapInfoIter& rIter) {
    if (!isValidInfo(rIter)) {
        return false;
    }

    if (!isExistStageSwitchDead(rIter)) {
        return false;
    }

    if (pActor->mStageSwitchCtrl == nullptr) {
        pActor->initStageSwitch(rIter);
    }

    return true;
}

bool MR::needStageSwitchReadA(LiveActor* pActor, const JMapInfoIter& rIter) {
    return useStageSwitchReadA(pActor, rIter);
}

bool MR::needStageSwitchReadB(LiveActor* pActor, const JMapInfoIter& rIter) {
    return useStageSwitchReadB(pActor, rIter);
}

bool MR::needStageSwitchReadAppear(LiveActor* pActor, const JMapInfoIter& rIter) {
    return useStageSwitchReadAppear(pActor, rIter);
}

bool MR::needStageSwitchWriteA(LiveActor* pActor, const JMapInfoIter& rIter) {
    return useStageSwitchWriteA(pActor, rIter);
}

bool MR::needStageSwitchWriteB(LiveActor* pActor, const JMapInfoIter& rIter) {
    return useStageSwitchWriteB(pActor, rIter);
}

bool MR::needStageSwitchWriteDead(LiveActor* pActor, const JMapInfoIter& rIter) {
    return useStageSwitchWriteDead(pActor, rIter);
}

bool MR::isValidSwitchA(const LiveActor* pActor) {
    return pActor->mStageSwitchCtrl != nullptr && pActor->mStageSwitchCtrl->isValidSwitchA();
}

bool MR::isValidSwitchB(const LiveActor* pActor) {
    return pActor->mStageSwitchCtrl != nullptr && pActor->mStageSwitchCtrl->isValidSwitchB();
}

bool MR::isValidSwitchAppear(const LiveActor* pActor) {
    return pActor->mStageSwitchCtrl != nullptr && pActor->mStageSwitchCtrl->isValidSwitchAppear();
}

bool MR::isValidSwitchDead(const LiveActor* pActor) {
    return pActor->mStageSwitchCtrl != nullptr && pActor->mStageSwitchCtrl->isValidSwitchDead();
}

bool MR::isOnSwitchA(const LiveActor* pActor) {
    return pActor->mStageSwitchCtrl->isOnSwitchA();
}

bool MR::isOnSwitchB(const LiveActor* pActor) {
    return pActor->mStageSwitchCtrl->isOnSwitchB();
}

bool MR::isOnSwitchAppear(const LiveActor* pActor) {
    return pActor->mStageSwitchCtrl->isOnSwitchAppear();
}

void MR::onSwitchA(LiveActor* pActor) {
    pActor->mStageSwitchCtrl->onSwitchA();
}

void MR::onSwitchB(LiveActor* pActor) {
    pActor->mStageSwitchCtrl->onSwitchB();
}

void MR::onSwitchDead(LiveActor* pActor) {
    pActor->mStageSwitchCtrl->onSwitchDead();
}

void MR::offSwitchA(LiveActor* pActor) {
    pActor->mStageSwitchCtrl->offSwitchA();
}

void MR::offSwitchB(LiveActor* pActor) {
    pActor->mStageSwitchCtrl->offSwitchB();
}

void MR::offSwitchDead(LiveActor* pActor) {
    pActor->mStageSwitchCtrl->offSwitchDead();
}

void MR::syncStageSwitchAppear(LiveActor* pActor) {
    ActorAppearSwitchListener* pSwitchListener = new ActorAppearSwitchListener(pActor, true, true);
    getSwitchWatcherHolder()->joinSwitchEventListenerAppear(pActor->mStageSwitchCtrl, pSwitchListener);
}

void MR::listenStageSwitchOnAppear(LiveActor* pActor, const MR::FunctorBase& rFunctor) {
    listenNameObjStageSwitchOnAppear(pActor, pActor->mStageSwitchCtrl, rFunctor);
}

void MR::listenStageSwitchOnOffAppear(LiveActor* pActor, const MR::FunctorBase& rFunctor1, const MR::FunctorBase& rFunctor2) {
    listenNameObjStageSwitchOnOffAppear(pActor, pActor->mStageSwitchCtrl, rFunctor1, rFunctor2);
}

void MR::listenStageSwitchOnA(LiveActor* pActor, const MR::FunctorBase& rFunctor) {
    listenNameObjStageSwitchOnA(pActor, pActor->mStageSwitchCtrl, rFunctor);
}

void MR::listenStageSwitchOnOffA(LiveActor* pActor, const MR::FunctorBase& rFunctor1, const MR::FunctorBase& rFunctor2) {
    listenNameObjStageSwitchOnOffA(pActor, pActor->mStageSwitchCtrl, rFunctor1, rFunctor2);
}

void MR::listenStageSwitchOnB(LiveActor* pActor, const MR::FunctorBase& rFunctor) {
    listenNameObjStageSwitchOnB(pActor, pActor->mStageSwitchCtrl, rFunctor);
}

void MR::listenStageSwitchOffB(LiveActor* pActor, const MR::FunctorBase& rFunctor) {
    listenNameObjStageSwitchOffB(pActor, pActor->mStageSwitchCtrl, rFunctor);
}

void MR::listenStageSwitchOnOffB(LiveActor* pActor, const MR::FunctorBase& rFunctor1, const MR::FunctorBase& rFunctor2) {
    listenNameObjStageSwitchOnOffB(pActor, pActor->mStageSwitchCtrl, rFunctor1, rFunctor2);
}
