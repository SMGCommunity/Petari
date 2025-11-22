#include "Game/Demo/DemoExecutorFunction.hpp"
#include "Game/Demo/DemoActionKeeper.hpp"
#include "Game/Demo/DemoTalkAnimCtrl.hpp"
#include "Game/Demo/DemoTimeKeeper.hpp"

namespace DemoExecutorFunction {
    void registerDemoActionFunction(const DemoExecutor* pExecutor, const LiveActor* pActor, const MR::FunctorBase& rFunctor, const char* a4) {
        pExecutor->mActionKeeper->registerFunctor(pActor, rFunctor, a4);
    }

    void registerDemoActionNerve(const DemoExecutor* pExecutor, const LiveActor* pActor, const Nerve* pNerve, const char* a4) {
        pExecutor->mActionKeeper->registerNerve(pActor, pNerve, a4);
    }

    bool isExistDemoPart(const DemoExecutor* pExecutor, const char* pPartName) {
        if (isExistDemoPartMain(pExecutor, pPartName)) {
            return true;
        }

        if (isExistDemoPartSub(pExecutor, pPartName)) {
            return true;
        }

        return false;
    }

    bool isExistDemoPartMain(const DemoExecutor* pExecutor, const char* pPartName) {
        DemoTimeKeeper* pTimeKeeper = pExecutor->mTimeKeeper;

        for (s32 i = 0; i < pTimeKeeper->mNumPartInfos; i++) {
            if (MR::isEqualString(pTimeKeeper->mMainPartInfos[i].mName, pPartName)) {
                return true;
            }
        }

        return false;
    }

    bool isExistDemoPartSub(const DemoExecutor* pExecutor, const char* pPartName) {
        DemoSubPartKeeper* pSubPartKeeper = pExecutor->mSubPartKeeper;

        for (s32 i = 0; i < pSubPartKeeper->mNumSubPartInfos; i++) {
            if (MR::isEqualString(pSubPartKeeper->mSubPartInfos[i].mSubPartName, pPartName)) {
                return true;
            }
        }

        return false;
    }

    bool isRegisteredDemoCast(const DemoExecutor* pExecutor, const LiveActor* pActor) {
        LiveActorGroup* pGroup = pExecutor->mGroup;

        for (s32 i = 0; i < pGroup->mObjectCount; i++) {
            if (MR::isSame(pGroup->getActor(i), pActor)) {
                return true;
            }
        }

        return false;
    }

    bool isRegisteredDemoActionAppear(const DemoExecutor* pExecutor, const LiveActor* pActor) {
        return pExecutor->mActionKeeper->isRegisteredDemoActionAppear(pActor);
    }

    bool isRegisteredDemoActionFunctor(const DemoExecutor* pExecutor, const LiveActor* pActor) {
        return pExecutor->mActionKeeper->isRegisteredDemoActionFunctor(pActor);
    }

    bool isRegisteredDemoActionNerve(const DemoExecutor* pExecutor, const LiveActor* pActor) {
        return pExecutor->mActionKeeper->isRegisteredDemoActionNerve(pActor);
    }

    bool tryCreateDemoTalkAnimCtrlForActor(DemoExecutor* pExecutor, LiveActor* pActor, const char* a3, const char* a4) {
        DemoTalkAnimCtrl* pTalkAnimCtrl = new DemoTalkAnimCtrl(pActor, pExecutor->mName, a4);

        pTalkAnimCtrl->initForActor(a3);
        pExecutor->addTalkAnimCtrl(pTalkAnimCtrl);

        return true;
    }

    bool tryCreateDemoTalkAnimCtrlForScene(DemoExecutor* pExecutor, LiveActor* pActor, const JMapInfoIter& rIter, const char* a4, const char* a5,
                                           s32 a6, s32 a7) {
        DemoTalkAnimCtrl* pTalkAnimCtrl = new DemoTalkAnimCtrl(pActor, pExecutor->mName, a5);

        pTalkAnimCtrl->initForScene(a4, a4, rIter);
        pTalkAnimCtrl->_40 = a6;
        pTalkAnimCtrl->_44 = a7;
        pExecutor->addTalkAnimCtrl(pTalkAnimCtrl);

        return true;
    }

    void registerDemoTalkMessageCtrl(DemoExecutor* pExecutor, LiveActor* pActor, TalkMessageCtrl* pTalkCtrl) {
        pExecutor->addTalkMessageCtrl(pActor, pTalkCtrl);
    }

    void setDemoTalkMessageCtrl(DemoExecutor* pExecutor, const LiveActor* pActor, TalkMessageCtrl* pTalkCtrl) {
        pExecutor->setTalkMessageCtrl(pActor, pTalkCtrl);
    }

    void pauseTimeKeepDemo(DemoExecutor* pExecutor) { return pExecutor->pause(); }

    void resumeTimeKeepDemo(DemoExecutor* pExecutor) { return pExecutor->resume(); }

    bool isPauseTimeKeepDemo(const DemoExecutor* pExecutor) { return pExecutor->mTimeKeeper->mIsPaused; }
};  // namespace DemoExecutorFunction
