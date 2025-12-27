#include "Game/Util/ActorSwitchUtil.hpp"
#include "Game/LiveActor/LiveActor.hpp"
#include "Game/Map/ActorAppearSwitchListener.hpp"
#include "Game/Map/SleepControllerHolder.hpp"
#include "Game/Map/SwitchWatcherHolder.hpp"
#include "Game/Util/JMapUtil.hpp"
#include "Game/Util/ObjUtil.hpp"

namespace MR {
    bool useStageSwitchReadA(LiveActor* pActor, const JMapInfoIter& rIter) {
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

    bool useStageSwitchReadB(LiveActor* pActor, const JMapInfoIter& rIter) {
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

    bool useStageSwitchReadAppear(LiveActor* pActor, const JMapInfoIter& rIter) {
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

    void useStageSwitchSleep(LiveActor* pActor, const JMapInfoIter& rIter) {
        SleepControlFunc::addSleepControl(pActor, rIter);
    }

    // Same as useStageSwitchReadA
    bool useStageSwitchWriteA(LiveActor* pActor, const JMapInfoIter& rIter) {
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
    bool useStageSwitchWriteB(LiveActor* pActor, const JMapInfoIter& rIter) {
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

    bool useStageSwitchWriteDead(LiveActor* pActor, const JMapInfoIter& rIter) {
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

    bool needStageSwitchReadA(LiveActor* pActor, const JMapInfoIter& rIter) {
        return useStageSwitchReadA(pActor, rIter);
    }

    bool needStageSwitchReadB(LiveActor* pActor, const JMapInfoIter& rIter) {
        return useStageSwitchReadB(pActor, rIter);
    }

    bool needStageSwitchReadAppear(LiveActor* pActor, const JMapInfoIter& rIter) {
        return useStageSwitchReadAppear(pActor, rIter);
    }

    bool needStageSwitchWriteA(LiveActor* pActor, const JMapInfoIter& rIter) {
        return useStageSwitchWriteA(pActor, rIter);
    }

    bool needStageSwitchWriteB(LiveActor* pActor, const JMapInfoIter& rIter) {
        return useStageSwitchWriteB(pActor, rIter);
    }

    bool needStageSwitchWriteDead(LiveActor* pActor, const JMapInfoIter& rIter) {
        return useStageSwitchWriteDead(pActor, rIter);
    }

    bool isValidSwitchA(const LiveActor* pActor) {
        return pActor->mStageSwitchCtrl != nullptr && pActor->mStageSwitchCtrl->isValidSwitchA();
    }

    bool isValidSwitchB(const LiveActor* pActor) {
        return pActor->mStageSwitchCtrl != nullptr && pActor->mStageSwitchCtrl->isValidSwitchB();
    }

    bool isValidSwitchAppear(const LiveActor* pActor) {
        return pActor->mStageSwitchCtrl != nullptr && pActor->mStageSwitchCtrl->isValidSwitchAppear();
    }

    bool isValidSwitchDead(const LiveActor* pActor) {
        return pActor->mStageSwitchCtrl != nullptr && pActor->mStageSwitchCtrl->isValidSwitchDead();
    }

    bool isOnSwitchA(const LiveActor* pActor) {
        return pActor->mStageSwitchCtrl->isOnSwitchA();
    }

    bool isOnSwitchB(const LiveActor* pActor) {
        return pActor->mStageSwitchCtrl->isOnSwitchB();
    }

    bool isOnSwitchAppear(const LiveActor* pActor) {
        return pActor->mStageSwitchCtrl->isOnSwitchAppear();
    }

    void onSwitchA(LiveActor* pActor) {
        pActor->mStageSwitchCtrl->onSwitchA();
    }

    void onSwitchB(LiveActor* pActor) {
        pActor->mStageSwitchCtrl->onSwitchB();
    }

    void onSwitchDead(LiveActor* pActor) {
        pActor->mStageSwitchCtrl->onSwitchDead();
    }

    void offSwitchA(LiveActor* pActor) {
        pActor->mStageSwitchCtrl->offSwitchA();
    }

    void offSwitchB(LiveActor* pActor) {
        pActor->mStageSwitchCtrl->offSwitchB();
    }

    void offSwitchDead(LiveActor* pActor) {
        pActor->mStageSwitchCtrl->offSwitchDead();
    }

    void syncStageSwitchAppear(LiveActor* pActor) {
        ActorAppearSwitchListener* pSwitchListener = new ActorAppearSwitchListener(pActor, true, true);
        getSwitchWatcherHolder()->joinSwitchEventListenerAppear(pActor->mStageSwitchCtrl, pSwitchListener);
    }

    void listenStageSwitchOnAppear(LiveActor* pActor, const MR::FunctorBase& rFunctor) {
        listenNameObjStageSwitchOnAppear(pActor, pActor->mStageSwitchCtrl, rFunctor);
    }

    void listenStageSwitchOnOffAppear(LiveActor* pActor, const MR::FunctorBase& rFunctor1, const MR::FunctorBase& rFunctor2) {
        listenNameObjStageSwitchOnOffAppear(pActor, pActor->mStageSwitchCtrl, rFunctor1, rFunctor2);
    }

    void listenStageSwitchOnA(LiveActor* pActor, const MR::FunctorBase& rFunctor) {
        listenNameObjStageSwitchOnA(pActor, pActor->mStageSwitchCtrl, rFunctor);
    }

    void listenStageSwitchOnOffA(LiveActor* pActor, const MR::FunctorBase& rFunctor1, const MR::FunctorBase& rFunctor2) {
        listenNameObjStageSwitchOnOffA(pActor, pActor->mStageSwitchCtrl, rFunctor1, rFunctor2);
    }

    void listenStageSwitchOnB(LiveActor* pActor, const MR::FunctorBase& rFunctor) {
        listenNameObjStageSwitchOnB(pActor, pActor->mStageSwitchCtrl, rFunctor);
    }

    void listenStageSwitchOffB(LiveActor* pActor, const MR::FunctorBase& rFunctor) {
        listenNameObjStageSwitchOffB(pActor, pActor->mStageSwitchCtrl, rFunctor);
    }

    void listenStageSwitchOnOffB(LiveActor* pActor, const MR::FunctorBase& rFunctor1, const MR::FunctorBase& rFunctor2) {
        listenNameObjStageSwitchOnOffB(pActor, pActor->mStageSwitchCtrl, rFunctor1, rFunctor2);
    }
};  // namespace MR
