#include "Game/Demo/DemoExecutorFunction.hpp"

namespace DemoExecutorFunction {
    void registerDemoActionFunction(const DemoExecutor *pExecutor, const LiveActor *pActor, const MR::FunctorBase &rFunctor, const char *a4) {
        pExecutor->mActionKeeper->registerFunctor(pActor, rFunctor, a4);
    }

    void registerDemoActionNerve(const DemoExecutor *pExecutor, const LiveActor *pActor, const Nerve *pNerve, const char *a4) {
        pExecutor->mActionKeeper->registerNerve(pActor, pNerve, a4);
    }

    bool isExistDemoPart(const DemoExecutor *pExecutor, const char *pPartName) {
        if (isExistDemoPartMain(pExecutor, pPartName)) return true;
        if (isExistDemoPartSub(pExecutor, pPartName)) return true;
        return false;
    }

    bool isExistDemoPartMain(const DemoExecutor *pExecutor, const char *pPartName) {
        DemoTimeKeeper *timekeeper = pExecutor->mTimeKeeper;
        for (s32 i = 0; i < timekeeper->mNumPartInfos; i++) {
            if (MR::isEqualString(timekeeper->mMainPartInfos[i].mName, pPartName)) {
                return true;
            }
        }
        return false;
    }

    bool isExistDemoPartSub(const DemoExecutor *pExecutor, const char *pPartName) {
        DemoTimeKeeper *timekeeper = pExecutor->mTimeKeeper;
        for (s32 i = 0; i < timekeeper->mNumPartInfos; i++) {
            if (MR::isEqualString(timekeeper->mSubPartInfos[i].mName, pPartName)) {
                return true;
            }
        }
        return false;
    }

    bool isRegisteredDemoCast(const DemoExecutor *pExecutor, const LiveActor *pActor) {
        LiveActorGroup *group = reinterpret_cast<LiveActorGroup *>(pExecutor->mGroup);
        for (s32 i = 0; i < group->mObjectCount; i++) {
            if (MR::isSame(group->getActor(i), pActor)) {
                return true;
            }
        }
        return false;
    }

    bool isRegisteredDemoActionAppear(const DemoExecutor *pExecutor, const LiveActor *pActor) {
        return pExecutor->mActionKeeper->isRegisteredDemoActionAppear(pActor);
    }

    bool isRegisteredDemoActionFunctor(const DemoExecutor *pExecutor, const LiveActor *pActor) {
        return pExecutor->mActionKeeper->isRegisteredDemoActionFunctor(pActor);
    }

    bool isRegisteredDemoActionNerve(const DemoExecutor *pExecutor, const LiveActor *pActor) {
        return pExecutor->mActionKeeper->isRegisteredDemoActionNerve(pActor);
    }

    // bool tryCreateDemoTalkAnimCtrlForActor(DemoExecutor *, LiveActor *, const char *, const char *);
    // bool tryCreateDemoTalkAnimCtrlForScene(DemoExecutor *, LiveActor *, const JMapInfoIter &, const char *, const char *, s32, s32);

    void registerDemoTalkMessageCtrl(DemoExecutor *pExecutor, LiveActor *pActor, TalkMessageCtrl *pTalkCtrl) {
        pExecutor->addTalkMessageCtrl(pActor, pTalkCtrl);
    }

    void setDemoTalkMessageCtrl(DemoExecutor *pExecutor, const LiveActor *pActor, TalkMessageCtrl *pTalkCtrl) {
        pExecutor->setTalkMessageCtrl(pActor, pTalkCtrl);
    }

    void pauseTimeKeepDemo(DemoExecutor *pExecutor) {
        return pExecutor->pause();
    }

    void resumeTimeKeepDemo(DemoExecutor *pExecutor) {
        return pExecutor->resume();
    }

    bool isPauseTimeKeepDemo(const DemoExecutor *pExecutor) {
        return pExecutor->mTimeKeeper->mIsPaused;
    }
}
