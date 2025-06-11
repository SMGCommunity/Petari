#include "Game/Util/ActorSwitchUtil.hpp"
#include "Game/LiveActor/LiveActor.hpp"
#include "Game/Map/ActorAppearSwitchListener.hpp"
#include "Game/Map/SleepControllerHolder.hpp"
#include "Game/Map/SwitchWatcherHolder.hpp"
#include "Game/Util/JMapUtil.hpp"
#include "Game/Util/ObjUtil.hpp"

namespace MR {
    bool useStageSwitchReadA(LiveActor *pActor, const JMapInfoIter &rInfo) {
        if (!isValidInfo(rInfo)) {
            return false;
        }
        if (!isExistStageSwitchA(rInfo)) {
            return false;
        }
        if (!pActor->mStageSwitchCtrl) {
            pActor->initStageSwitch(rInfo);
        }
        return true;
    }

    bool useStageSwitchReadB(LiveActor *pActor, const JMapInfoIter &rInfo) {
        if (!isValidInfo(rInfo)) {
            return false;
        }
        if (!isExistStageSwitchB(rInfo)) {
            return false;
        }
        if (!pActor->mStageSwitchCtrl) {
            pActor->initStageSwitch(rInfo);
        }
        return true;
    }

    bool useStageSwitchReadAppear(LiveActor *pActor, const JMapInfoIter &rInfo) {
        if (!isValidInfo(rInfo)) {
            return false;
        }
        if (!isExistStageSwitchAppear(rInfo)) {
            return false;
        }
        if (!pActor->mStageSwitchCtrl) {
            pActor->initStageSwitch(rInfo);
        }
        return true;
    }

    void useStageSwitchSleep(LiveActor *pActor, const JMapInfoIter &rInfo) {
        SleepControlFunc::addSleepControl(pActor, rInfo);
    }

    // Same as useStageSwitchReadA
    bool useStageSwitchWriteA(LiveActor *pActor, const JMapInfoIter &rInfo) {
        if (!isValidInfo(rInfo)) {
            return false;
        }
        if (!isExistStageSwitchA(rInfo)) {
            return false;
        }
        if (!pActor->mStageSwitchCtrl) {
            pActor->initStageSwitch(rInfo);
        }
        return true;
    }

    // Same as useStageSwitchReadB
    bool useStageSwitchWriteB(LiveActor *pActor, const JMapInfoIter &rInfo) {
        if (!isValidInfo(rInfo)) {
            return false;
        }
        if (!isExistStageSwitchB(rInfo)) {
            return false;
        }
        if (!pActor->mStageSwitchCtrl) {
            pActor->initStageSwitch(rInfo);
        }
        return true;
    }

    bool useStageSwitchWriteDead(LiveActor *pActor, const JMapInfoIter &rInfo) {
        if (!isValidInfo(rInfo)) {
            return false;
        }
        if (!isExistStageSwitchDead(rInfo)) {
            return false;
        }
        if (!pActor->mStageSwitchCtrl) {
            pActor->initStageSwitch(rInfo);
        }
        return true;
    }

    bool needStageSwitchReadA(LiveActor *pActor, const JMapInfoIter &rInfo) {
        return useStageSwitchReadA(pActor, rInfo);
    }

    bool needStageSwitchReadB(LiveActor *pActor, const JMapInfoIter &rInfo) {
        return useStageSwitchReadB(pActor, rInfo);
    }

    bool needStageSwitchReadAppear(LiveActor *pActor, const JMapInfoIter &rInfo) {
        return useStageSwitchReadAppear(pActor, rInfo);
    }

    bool needStageSwitchWriteA(LiveActor *pActor, const JMapInfoIter &rInfo) {
        return useStageSwitchWriteA(pActor, rInfo);
    }

    bool needStageSwitchWriteB(LiveActor *pActor, const JMapInfoIter &rInfo) {
        return useStageSwitchWriteB(pActor, rInfo);
    }

    bool needStageSwitchWriteDead(LiveActor *pActor, const JMapInfoIter &rInfo) {
        return useStageSwitchWriteDead(pActor, rInfo);
    }

    bool isValidSwitchA(const LiveActor *pActor) {
        bool ret = false;
        if (pActor->mStageSwitchCtrl && pActor->mStageSwitchCtrl->isValidSwitchA()) {
            ret = true;
        }
        return ret;
    }

    bool isValidSwitchB(const LiveActor *pActor) {
        bool ret = false;
        if (pActor->mStageSwitchCtrl && pActor->mStageSwitchCtrl->isValidSwitchB()) {
            ret = true;
        }
        return ret;
    }

    bool isValidSwitchAppear(const LiveActor *pActor) {
        bool ret = false;
        if (pActor->mStageSwitchCtrl && pActor->mStageSwitchCtrl->isValidSwitchAppear()) {
            ret = true;
        }
        return ret;
    }

    bool isValidSwitchDead(const LiveActor *pActor) {
        bool ret = false;
        if (pActor->mStageSwitchCtrl && pActor->mStageSwitchCtrl->isValidSwitchDead()) {
            ret = true;
        }
        return ret;
    }

    bool isOnSwitchA(const LiveActor *pActor) {
        return pActor->mStageSwitchCtrl->isOnSwitchA();
    }

    bool isOnSwitchB(const LiveActor *pActor) {
        return pActor->mStageSwitchCtrl->isOnSwitchB();
    }

    bool isOnSwitchAppear(const LiveActor *pActor) {
        return pActor->mStageSwitchCtrl->isOnSwitchAppear();
    }

    void onSwitchA(LiveActor *pActor) {
        pActor->mStageSwitchCtrl->onSwitchA();
    }

    void onSwitchB(LiveActor *pActor) {
        pActor->mStageSwitchCtrl->onSwitchB();
    }

    void onSwitchDead(LiveActor *pActor) {
        pActor->mStageSwitchCtrl->onSwitchDead();
    }

    void offSwitchA(LiveActor *pActor) {
        pActor->mStageSwitchCtrl->offSwitchA();
    }

    void offSwitchB(LiveActor *pActor) {
        pActor->mStageSwitchCtrl->offSwitchB();
    }

    void offSwitchDead(LiveActor *pActor) {
        pActor->mStageSwitchCtrl->offSwitchDead();
    }

    void syncStageSwitchAppear(LiveActor *pActor) {
        ActorAppearSwitchListener* pSwitchListener = new ActorAppearSwitchListener(pActor, true, true);
        getSwitchWatcherHolder()->joinSwitchEventListenerAppear(pActor->mStageSwitchCtrl, pSwitchListener);
    }

    void listenStageSwitchOnAppear(LiveActor *pActor, const MR::FunctorBase &rFunctor) {
        listenNameObjStageSwitchOnAppear(pActor, pActor->mStageSwitchCtrl, rFunctor);
    }

	void listenStageSwitchOnOffAppear(LiveActor *pActor, const MR::FunctorBase &rFunctor1, const MR::FunctorBase &rFunctor2) {
        listenNameObjStageSwitchOnOffAppear(pActor, pActor->mStageSwitchCtrl, rFunctor1, rFunctor2);
    }

	void listenStageSwitchOnA(LiveActor *pActor, const MR::FunctorBase &rFunctor) {
        listenNameObjStageSwitchOnA(pActor, pActor->mStageSwitchCtrl, rFunctor);
    }

	void listenStageSwitchOnOffA(LiveActor *pActor, const MR::FunctorBase &rFunctor1, const MR::FunctorBase &rFunctor2) {
        listenNameObjStageSwitchOnOffA(pActor, pActor->mStageSwitchCtrl, rFunctor1, rFunctor2);
    }

	void listenStageSwitchOnB(LiveActor *pActor, const MR::FunctorBase &rFunctor) {
        listenNameObjStageSwitchOnB(pActor, pActor->mStageSwitchCtrl, rFunctor);
    }

	void listenStageSwitchOffB(LiveActor *pActor, const MR::FunctorBase &rFunctor) {
        listenNameObjStageSwitchOffB(pActor, pActor->mStageSwitchCtrl, rFunctor);
    }

	void listenStageSwitchOnOffB(LiveActor *pActor, const MR::FunctorBase &rFunctor1, const MR::FunctorBase &rFunctor2) {
        listenNameObjStageSwitchOnOffB(pActor, pActor->mStageSwitchCtrl, rFunctor1, rFunctor2);
    }

};
