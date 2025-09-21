#include "Game/Util/DemoUtil.hpp"
#include "Game/Demo/DemoDirector.hpp"
#include "Game/Demo/DemoFunction.hpp"
#include "Game/NPC/TalkDirector.hpp"
#include "Game/Scene/GameSceneFunction.hpp"

namespace MR {
    bool tryRegisterDemoCast(LiveActor *pActor, const JMapInfoIter &rIter) {
        return DemoFunction::getDemoDirector()->registerDemoCast(pActor, rIter);
    }

    void registerDemoActionFunctor(const LiveActor *pActor, const MR::FunctorBase &rFunctor, const char *pName) {
        DemoFunction::registerDemoActionFunctorFunction(pActor, rFunctor, pName);
    }

    void registerDemoActionNerve(const LiveActor *pActor, const Nerve *pNerve, const char *pName) {
        DemoFunction::registerDemoActionNerveFunction(pActor, pNerve, pName);
    }

    bool tryRegisterDemoActionFunctor(const LiveActor *pActor, const MR::FunctorBase &rFunctor, const char *pName) {
        if (!DemoFunction::isRegisteredDemoActionFunctor(pActor)) {
            return false;
        }
        else {
            DemoFunction::registerDemoActionFunctorFunction(pActor, rFunctor, pName);
            return true;
        }
    }

    bool tryRegisterDemoActionNerve(const LiveActor *pActor, const Nerve *pNerve, const char *pName) {
        if (!DemoFunction::isRegisteredDemoActionNerve(pActor)) {
            return false;
        }
        else {
            DemoFunction::registerDemoActionNerveFunction(pActor, pNerve, pName);
            return true;
        }
    }

    bool tryRegisterDemoCast(LiveActor *pActor, const char *pName, const JMapInfoIter &rIter) {
        return DemoFunction::getDemoDirector()->registerDemoCast(pActor, pName, rIter);
    }

    void registerDemoCast(LiveActor *pActor, const char *pName, const JMapInfoIter &rIter) {
        DemoFunction::getDemoDirector()->registerDemoCast(pActor, pName, rIter);
    }

    void initTalkAnimDemoCast(LiveActor *pActor, const JMapInfoIter &rIter, const char *pName1, const char *pName2) {
        DemoFunction::getDemoDirector()->registerDemoCast(pActor, pName1, rIter);
        DemoFunction::tryCreateDemoTalkAnimCtrlForActorDirect(pActor, pName1, pName2, nullptr);
    }

    void registerDemoActionFunctorDirect(const LiveActor *pActor, const MR::FunctorBase &rFunctor, const char *pName1, const char *pName2) {
        DemoFunction::registerDemoActionFunctorFunction(pActor, rFunctor, pName1, pName2);
    }

    bool tryRegisterDemoActionFunctorDirect(const LiveActor *pActor, const MR::FunctorBase &rFunctor, const char *pName1, const char *pName2) {
        if (!DemoFunction::isRegisteredDemoActionFunctor(pActor, pName1)) {
            return false;
        }
        else {
            DemoFunction::registerDemoActionFunctorFunction(pActor, rFunctor, pName1, pName2);
            return true;
        }
    }

    bool tryStartDemoRegistered(LiveActor *pActor, const char *pName) {
        return DemoFunction::tryStartDemoRegistered(pActor, pName);
    }

    bool tryStartDemoRegisteredMarioPuppetable(LiveActor *pActor, const char *pName) {
        return DemoFunction::tryStartDemoRegisteredMarioPuppetable(pActor, pName);
    }

    void registerDemoSimpleCastAll(LiveActor *pActor) {
        DemoFunction::registerDemoSimpleCastAllFunction(pActor);
    }

    void registerDemoSimpleCastAll(LayoutActor *pLayoutActor) {
        DemoFunction::registerDemoSimpleCastAllFunction(pLayoutActor);
    }

    void registerDemoSimpleCastAll(NameObj *pObj) {
        DemoFunction::registerDemoSimpleCastAllFunction(pObj);
    }

    bool isDemoCast(const LiveActor *pActor, const char *pName) {
        DemoExecutor* pExecutor = DemoFunction::findDemoExecutor(pActor);
        if (!pExecutor) {
            return false;
        }
        if (pName) {
            return DemoFunction::isRegisteredDemoCast(pActor, pName);
        }
        else {
            return DemoFunction::isDemoCast(pExecutor, pActor);
        }
    }

    bool isRegisteredDemoActionAppear(const LiveActor *pActor) {
        return DemoFunction::isRegisteredDemoActionAppear(pActor);
    }

    bool isRegisteredDemoActionNerve(const LiveActor *pActor) {
        return DemoFunction::isRegisteredDemoActionNerve(pActor);
    }

    void endDemo(NameObj *pObj, const char *pName) {
        DemoFunction::getDemoDirector()->endDemo(pObj, pName, false);
    }

    void endDemoWaitCameraInterpolating(NameObj *pObj, const char *pName) {
        DemoFunction::getDemoDirector()->endDemo(pObj, pName, true);
    }

    void initDemoSheetTalkAnim(LiveActor *pActor, const JMapInfoIter &rIter, const char *pName1, const char *pName2, TalkMessageCtrl *pTalkCtrl) {
        tryInitDemoSheetTalkAnim(pActor, rIter, pName1, pName2, pTalkCtrl);
    }

    void initDemoSheetTalkAnimFunctor(LiveActor *pActor, const JMapInfoIter &rIter, const char *pName1, 
        const char *pName2, TalkMessageCtrl *pTalkCtrl, const MR::FunctorBase &rFunctor) {
            if (tryInitDemoSheetTalkAnim(pActor, rIter, pName1, pName2, pTalkCtrl)) {
                DemoFunction::registerDemoActionFunctorFunction(pActor, rFunctor, pName1, nullptr);
            }
    }

    bool isDemoExist(const char *pName) {
        return DemoFunction::getDemoDirector()->isExistTimeKeepDemo(pName);
    }

    bool isDemoActive() {
        return DemoFunction::getDemoDirector()->mIsActive;
    }

    bool isDemoActive(const char *pName) {
        DemoExecutor* pExecutor = DemoFunction::getDemoDirector()->mExecutor;
        if (pExecutor) {
            if (isName(pExecutor, pName)) {
                return true;
            }
        }
        char* pDemoName = DemoFunction::getDemoDirector()->getCurrentDemoName();
        if (!pDemoName) {
            return false;
        }
        else {
            return isEqualString(pDemoName, pName);
        }
    }

    bool canStartDemo() {
        if (DemoFunction::getDemoDirector()->mIsActive) {
            return false;
        }
        if (isPlayerDead()) {
            return false;
        }
        if (GameSceneFunction::isExecStageClearDemo()) {
            return false;
        }
        else {
            return !isPlayerConfrontDeath();
        }
    }

    bool isTimeKeepDemoActive() {
        if (DemoFunction::getDemoDirector()->mIsActive == false) {
            return false;
        }
        return DemoFunction::getDemoDirector()->mExecutor != nullptr;
    }

    bool isDemoActiveRegistered(const LiveActor *pActor) {
        DemoExecutor* pExecutor = DemoFunction::findDemoExecutor(pActor);
        if (pExecutor == false) {
            return false;
        }
        else {
            return DemoFunction::getDemoDirector()->mExecutor == pExecutor;
        }
    }

    bool isDemoPartExist(const LiveActor *pActor, const char *pName) {
        DemoExecutor* pExecutor = DemoFunction::findDemoExecutor(pActor);
        if (pExecutor) {
            return DemoFunction::isExistDemoPart(pExecutor, pName);
        }
        else {
            return false;
        }
    }

    bool isDemoLastStep() {
        return DemoFunction::isDemoLastPartLastStep();
    }

    bool isDemoPartActive(const char *pName) {
        return DemoFunction::isDemoPartActiveFunction(pName);
    }

    bool isDemoPartStep(const char *pName, s32 a2) {
        if (DemoFunction::isDemoPartActiveFunction(pName) == false) {
            return false;
        }
        else {
            return DemoFunction::getDemoPartStepFunction(pName) == a2;
        }
    }

    bool isDemoPartFirstStep(const char *pName) {
        if (DemoFunction::isDemoPartActiveFunction(pName) == false) {
            return false;
        }
        else {
            return DemoFunction::getDemoPartStepFunction(pName) == 0;
        }
    }

    // Minor mismatch: regswap
    /* bool isDemoPartLastStep(const char *pName) {
        if (DemoFunction::isDemoPartActiveFunction(pName) == false) {
            return false;
        }
        else {
            s32 totalSteps = DemoFunction::getDemoPartTotalStepFunction(pName);
            return totalSteps - 1 == DemoFunction::getDemoPartStepFunction(pName);
        }
    } */

    bool isDemoPartLessEqualStep(const char *pName, s32 a2) {
        if (DemoFunction::isDemoPartActiveFunction(pName) == false) {
            return false;
        }
        else {
            return DemoFunction::getDemoPartStepFunction(pName) <= a2;
        }
    }

    bool isDemoPartGreaterStep(const char *pName, s32 a2) {
        if (DemoFunction::isDemoPartActiveFunction(pName) == false) {
            return false;
        }
        else {
            return DemoFunction::getDemoPartStepFunction(pName) > a2;
        }
    }

    s32 getDemoPartTotalStep(const char *pName) {
        return DemoFunction::getDemoPartTotalStepFunction(pName);
    }

    s32 getDemoPartStep(const char *pName) {
        return DemoFunction::getDemoPartStepFunction(pName);
    }

    void pauseTimeKeepDemo(LiveActor *pActor) {
        DemoFunction::pauseTimeKeepDemo(pActor);
    }

    void resumeTimeKeepDemo(LiveActor *pActor) {
        DemoFunction::resumeTimeKeepDemo(pActor);
    }

    bool isPowerStarGetDemoActive() {
        return GameSceneFunction::isExecStageClearDemo();
    }

    const char* getCurrentDemoPartNameMain(const char *pName) {
        return DemoFunction::getCurrentDemoPartNameMain(pName);
    }
    
    void endTalkingSequence(NameObj *pObj) {
        const char* pName = "会話";
        DemoFunction::getDemoDirector()->endDemo(pObj, pName, false);
    }

    bool isSystemTalking() {
        if (isExistSceneObj(0x19) == false) {
            return false;
        }
        else {
            return getSceneObj<TalkDirector*>(SceneObj_TalkDirector)->isSystemTalking();
        }
    }

    bool isNormalTalking() {
        if (isExistSceneObj(0x19) == false) {
            return false;
        }
        else {
            return getSceneObj<TalkDirector*>(SceneObj_TalkDirector)->isNormalTalking();
        }
    }

    LiveActor* getTalkingActor() {
        if (isExistSceneObj(0x19) == false) {
            return false;
        }
        else {
            return getSceneObj<TalkDirector*>(SceneObj_TalkDirector)->getTalkingActor();
        }
    }

    bool isDemoPartTalk(const char *pName) {
        return DemoFunction::isDemoPartTalk(pName);
    }
};
