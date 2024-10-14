#include "Game/Demo/DemoFunction.hpp"
#include <cstdio>
#include <cstring>
#include "Game/Demo/DemoCastGroup.hpp"
#include "Game/Demo/DemoCastGroupHolder.hpp"
#include "Game/Demo/DemoDirector.hpp"
#include "Game/Demo/DemoExecutorFunction.hpp"
#include "Game/Demo/DemoSubPartKeeper.hpp"
#include "Game/Demo/DemoTimeKeeper.hpp"
#include "Game/Effect/EffectSystemUtil.hpp"


namespace {
    DemoTimeKeeper* getCurrentTimeKeeper() NO_INLINE { return DemoFunction::getDemoDirector()->mExecutor->mTimeKeeper; }
    DemoSubPartKeeper* getCurrentSubPartKeeper() NO_INLINE { return DemoFunction::getDemoDirector()->mExecutor->mSubPartKeeper; }
    bool isCurrentMainPart(const char* pPartName) NO_INLINE {
        DemoTimeKeeper* timekeeper = getCurrentTimeKeeper();
        return MR::isEqualString(pPartName, timekeeper->mSubPartInfos[0].mName);
    }
};  // namespace

namespace DemoFunction {
    DemoDirector* getDemoDirector() { return reinterpret_cast<DemoDirector*>(MR::getSceneObjHolder()->getObj(SceneObj_DemoDirector)); }

    DemoCastGroupHolder* getDemoCastSubGroupHolder() { return getDemoDirector()->_1C; }

    void registerDemoSimpleCastAllFunction(LiveActor* pActor) { return getDemoDirector()->registerDemoSimpleCast(pActor); }

    void registerDemoSimpleCastAllFunction(LayoutActor* pActor) { return getDemoDirector()->registerDemoSimpleCast(pActor); }

    void registerDemoSimpleCastAllFunction(NameObj* pObj) { return getDemoDirector()->registerDemoSimpleCast(pObj); }

    void registerDemoActionFunctorFunction(const LiveActor* pActor, const MR::FunctorBase& rFunctor, const char* pName) {
        DemoExecutorFunction::registerDemoActionFunction(findDemoExecutor(pActor), pActor, rFunctor, pName);
    }

    void registerDemoActionFunctorFunction(const LiveActor* pActor, const MR::FunctorBase& rFunctor, const char* pDemoName, const char* pName) {
        DemoExecutorFunction::registerDemoActionFunction(findDemoExecutor(pDemoName), pActor, rFunctor, pName);
    }

    void registerDemoActionNerveFunction(const LiveActor* pActor, const Nerve* pNerve, const char* pName) {
        DemoExecutorFunction::registerDemoActionNerve(findDemoExecutor(pActor), pActor, pNerve, pName);
    }

    bool tryStartDemoRegistered(LiveActor* pActor, const char* pPartName) {
        DemoExecutor* executor = findDemoExecutor(pActor);
        if (executor == nullptr) {
            return false;
        }
        if (pPartName != nullptr) {
            return executor->tryStartProperDemoSystemPart(pPartName);
        } else {
            return executor->tryStartProperDemoSystem();
        }
    }

    bool tryStartDemoRegisteredMarioPuppetable(LiveActor* pActor, const char* pPartName) {
        DemoExecutor* executor = findDemoExecutor(pActor);
        if (executor == nullptr) {
            return false;
        }
        if (pPartName != nullptr) {
            return executor->tryStartDemoSystemPart(pPartName, 2);
        } else {
            return MR::tryStartTimeKeepDemoMarioPuppetable(executor, executor->mName, nullptr);
        }
    }

    void registerDemoExecutor(DemoExecutor* pExecutor) { getDemoDirector()->_18->registerObj(pExecutor); }

    DemoExecutor* findDemoExecutor(const char* pDemoName) {
        DemoCastGroup* group = getDemoDirector()->_18->findCastGroup(pDemoName);
        if (group == nullptr) {
            return nullptr;
        }
        return reinterpret_cast<DemoExecutor*>(group);
    }

    DemoExecutor* findDemoExecutor(const LiveActor* pActor) {
        DemoExecutor* executor;
        for (s32 i = 0; i < getDemoDirector()->_18->mObjectCount; i++) {
            executor = reinterpret_cast<DemoExecutor*>(getDemoDirector()->_18->getCastGroup(i));
            if (DemoExecutorFunction::isRegisteredDemoCast(executor, pActor)) {
                return executor;
            }
        }
        return NULL;
    }

    DemoExecutor* findDemoExecutorActive(const LiveActor* pActor) {
        DemoExecutor* executor;
        for (s32 i = 0; i < getDemoDirector()->_18->mObjectCount; i++) {
            executor = reinterpret_cast<DemoExecutor*>(getDemoDirector()->_18->getCastGroup(i));
            if (DemoExecutorFunction::isRegisteredDemoCast(executor, pActor) && MR::isDemoActive(executor->mName)) {
                return executor;
            }
        }
        return NULL;
    }

    bool isDemoCast(const DemoExecutor* pExecutor, const LiveActor* pActor) { return DemoExecutorFunction::isRegisteredDemoCast(pExecutor, pActor); }

    bool isExistDemoPart(const DemoExecutor* pExecutor, const char* pPartName) { return DemoExecutorFunction::isExistDemoPart(pExecutor, pPartName); }

    bool isDemoPartActiveFunction(const char* pPartName) {
        if (!MR::isTimeKeepDemoActive()) {
            return false;
        }
        if (isCurrentMainPart(pPartName)) {
            return true;
        }
        return getCurrentSubPartKeeper()->isDemoPartActive(pPartName);
    }

    s32 getDemoPartStepFunction(const char* pPartName) {
        if (isCurrentMainPart(pPartName)) {
            return getCurrentTimeKeeper()->mCurrentStep;
        } else {
            return getCurrentSubPartKeeper()->getDemoPartStep(pPartName);
        }
    }

    s32 getDemoPartTotalStepFunction(const char* pPartName) {
        if (isCurrentMainPart(pPartName)) {
            return getCurrentTimeKeeper()->mSubPartInfos[0].mTotalSteps;
        } else {
            return getCurrentSubPartKeeper()->getDemoPartTotalStep(pPartName);
        }
    }

    bool isDemoLastPartLastStep() {
        if (!MR::isTimeKeepDemoActive()) {
            return false;
        }
        s32 total = getCurrentTimeKeeper()->mSubPartInfos[0].mTotalSteps;
        s32 current = getCurrentTimeKeeper()->mCurrentStep;
        if (current != total - 1) {
            return false;
        } else {
            return getCurrentTimeKeeper()->isPartLast();
        }
    }

    bool isDemoSuspendOrLastPartLastStep() {
        if (!MR::isTimeKeepDemoActive()) {
            return false;
        }
        DemoDirector* director = getDemoDirector();
        if (director->mExecutor == nullptr) {
            return false;
        }
        DemoTimeKeeper* timekeeper = director->mExecutor->mTimeKeeper;
        if (timekeeper->isExistSuspendFlagCurrentPart()) {
            return timekeeper->isExistSuspendFlagCurrentPart();
        }
        return isDemoLastPartLastStep();
    }

    const char* getCurrentDemoPartNameMain(const char* pDemoName) {
        DemoExecutor* executor = findDemoExecutor(pDemoName);
        if (executor == nullptr) {
            return nullptr;
        }
        DemoTimePartInfo* part = &executor->mTimeKeeper->mSubPartInfos[0];
        if (part != nullptr) {
            return part->mName;
        }
        return nullptr;
    }

    bool isTargetDemoCast(LiveActor* pActor, const JMapInfoIter& rIter, const char* pActorName, s32 id) {
        if (pActorName == nullptr) {
            return false;
        }
        if (!MR::isName(pActor, pActorName)) {
            return false;
        }
        if (id >= 0 && MR::getDemoCastID(rIter) != id) {
            return false;
        }
        return true;
    }

    bool isRegisteredDemoActionAppear(const LiveActor* pActor) {
        return DemoExecutorFunction::isRegisteredDemoActionAppear(DemoFunction::findDemoExecutor(pActor), pActor);
    }

    bool isRegisteredDemoActionFunctor(const LiveActor* pActor) {
        return DemoExecutorFunction::isRegisteredDemoActionFunctor(DemoFunction::findDemoExecutor(pActor), pActor);
    }

    bool isRegisteredDemoActionFunctor(const LiveActor* pActor, const char* pDemoName) {
        return DemoExecutorFunction::isRegisteredDemoActionFunctor(DemoFunction::findDemoExecutor(pDemoName), pActor);
    }

    bool isRegisteredDemoActionNerve(const LiveActor* pActor) {
        return DemoExecutorFunction::isRegisteredDemoActionNerve(DemoFunction::findDemoExecutor(pActor), pActor);
    }

    void requestDemoCastMovementOn(LiveActor* pActor) {
        MR::requestMovementOn(pActor);
        if (pActor->mEffectKeeper != nullptr) {
            MR::Effect::requestMovementOn(pActor->mEffectKeeper);
        }
    }

    bool isRegisteredDemoCast(const LiveActor* pActor, const char* pDemoName) {
        DemoExecutor* executor = findDemoExecutor(pDemoName);
        if (executor != nullptr) {
            return DemoExecutorFunction::isRegisteredDemoCast(executor, pActor);
        }
        return false;
    }

    void pauseTimeKeepDemo(LiveActor* pActor) { return DemoExecutorFunction::pauseTimeKeepDemo(DemoFunction::findDemoExecutorActive(pActor)); }

    void resumeTimeKeepDemo(LiveActor* pActor) { return DemoExecutorFunction::resumeTimeKeepDemo(DemoFunction::findDemoExecutorActive(pActor)); }

    bool isPauseTimeKeepDemo(LiveActor* pActor) { return DemoExecutorFunction::isPauseTimeKeepDemo(DemoFunction::findDemoExecutorActive(pActor)); }

    void pauseTimeKeepDemo(const char* pDemoName) { return DemoExecutorFunction::pauseTimeKeepDemo(DemoFunction::findDemoExecutor(pDemoName)); }

    void resumeTimeKeepDemo(const char* pDemoName) { return DemoExecutorFunction::resumeTimeKeepDemo(DemoFunction::findDemoExecutor(pDemoName)); }

    bool isPauseTimeKeepDemo(const char* pDemoName) { return DemoExecutorFunction::isPauseTimeKeepDemo(DemoFunction::findDemoExecutor(pDemoName)); }

    bool tryCreateDemoTalkAnimCtrlForActor(LiveActor* pActor, const char* pFileBaseName, const char* a3) {
        DemoExecutor* executor = findDemoExecutor(pActor);
        return DemoExecutorFunction::tryCreateDemoTalkAnimCtrlForActor(executor, pActor, pFileBaseName, a3);
    }

    bool tryCreateDemoTalkAnimCtrlForActorDirect(LiveActor* pActor, const char* pDemoName, const char* pFileBaseName, const char* a4) {
        DemoExecutor* executor = findDemoExecutor(pDemoName);
        return DemoExecutorFunction::tryCreateDemoTalkAnimCtrlForActor(executor, pActor, pFileBaseName, a4);
    }

    bool tryCreateDemoTalkAnimCtrlForScene(LiveActor* pActor, const JMapInfoIter& rIter, const char* pFileBaseName, const char* a4, s32 a5, s32 a6) {
        DemoExecutor* executor = findDemoExecutor(pActor);
        return DemoExecutorFunction::tryCreateDemoTalkAnimCtrlForScene(executor, pActor, rIter, pFileBaseName, a4, a5, a6);
    }

    bool tryCreateDemoTalkAnimCtrlForSceneDirect(LiveActor* pActor, const char* pDemoName, const JMapInfoIter& rIter, const char* pFileBaseName,
                                                 const char* a5, s32 a6, s32 a7) {
        DemoExecutor* executor = findDemoExecutor(pDemoName);
        return DemoExecutorFunction::tryCreateDemoTalkAnimCtrlForScene(executor, pActor, rIter, pFileBaseName, a5, a6, a7);
    }

    void registerDemoTalkMessageCtrl(LiveActor* pActor, TalkMessageCtrl* pTalkCtrl) {
        DemoExecutor* executor = findDemoExecutor(pActor);
        executor->addTalkMessageCtrl(pActor, pTalkCtrl);
    }

    void registerDemoTalkMessageCtrlDirect(LiveActor* pActor, TalkMessageCtrl* pTalkCtrl, const char* pDemoName) {
        DemoExecutor* executor = findDemoExecutor(pDemoName);
        DemoExecutorFunction::registerDemoTalkMessageCtrl(executor, pActor, pTalkCtrl);
    }

    void setDemoTalkMessageCtrlDirect(const LiveActor* pActor, TalkMessageCtrl* pTalkCtrl, const char* pDemoName) {
        DemoExecutor* executor = findDemoExecutor(pDemoName);
        DemoExecutorFunction::setDemoTalkMessageCtrl(executor, pActor, pTalkCtrl);
    }

    ResourceHolder* loadDemoArchive() {
        char buf[0x100];
        snprintf(buf, 0x100, "DemoSheet.arc");
        return MR::createAndAddResourceHolder(buf);
    }

    s32 createSheetParser(const DemoExecutor* pExecutor, const char* pNameSuffix, JMapInfo** pMap) {
        const char* sheet_name = pExecutor->mSheetName;
        JMapInfo* map = MR::tryCreateCsvParser(getDemoDirector()->mResourceHolder, "Demo%s%s.bcsv", sheet_name, pNameSuffix);
        *pMap = map;
        if (map != nullptr) {
            const JMapData* data = map->mData;
            if (data != nullptr) {
                return data->mNumEntries;
            }
            return 0;
        }
        return 0;
    }

    bool isDemoPartTalk(const char* pDemoName) { return MR::isEqualSubString(pDemoName, "会話"); }
};  // namespace DemoFunction
