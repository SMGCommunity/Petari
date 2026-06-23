#include "Game/Util/DemoUtil.hpp"
#include "Game/Demo/DemoDirector.hpp"
#include "Game/Demo/DemoExecutor.hpp"
#include "Game/Demo/DemoFunction.hpp"
#include "Game/NPC/TalkDirector.hpp"
#include "Game/Scene/GameSceneFunction.hpp"
#include "Game/Scene/SceneObjHolder.hpp"
#include "Game/Util/ObjUtil.hpp"
#include "Game/Util/PlayerUtil.hpp"
#include "Game/Util/StringUtil.hpp"

namespace {
    TalkDirector* getTalkDirector() {
        return MR::getSceneObj< TalkDirector >(SceneObj_TalkDirector);
    }
};  // namespace

namespace MR {
    bool tryRegisterDemoCast(LiveActor* pActor, const JMapInfoIter& rIter) {
        return DemoFunction::getDemoDirector()->registerDemoCast(pActor, rIter);
    }

    void registerDemoActionFunctor(const LiveActor* pActor, const MR::FunctorBase& rFunctor, const char* pName) {
        DemoFunction::registerDemoActionFunctorFunction(pActor, rFunctor, pName);
    }

    void registerDemoActionNerve(const LiveActor* pActor, const Nerve* pNerve, const char* pName) {
        DemoFunction::registerDemoActionNerveFunction(pActor, pNerve, pName);
    }

    bool tryRegisterDemoActionFunctor(const LiveActor* pActor, const MR::FunctorBase& rFunctor, const char* pName) {
        if (!DemoFunction::isRegisteredDemoActionFunctor(pActor)) {
            return false;
        }

        registerDemoActionFunctor(pActor, rFunctor, pName);

        return true;
    }

    bool tryRegisterDemoActionNerve(const LiveActor* pActor, const Nerve* pNerve, const char* pName) {
        if (!isRegisteredDemoActionNerve(pActor)) {
            return false;
        }

        registerDemoActionNerve(pActor, pNerve, pName);

        return true;
    }

    bool tryRegisterDemoCast(LiveActor* pActor, const char* pName, const JMapInfoIter& rIter) {
        return DemoFunction::getDemoDirector()->registerDemoCast(pActor, pName, rIter);
    }

    void registerDemoCast(LiveActor* pActor, const char* pName, const JMapInfoIter& rIter) {
        DemoFunction::getDemoDirector()->registerDemoCast(pActor, pName, rIter);
    }

    void initTalkAnimDemoCast(LiveActor* pActor, const JMapInfoIter& rIter, const char* pName1, const char* pName2) {
        DemoFunction::getDemoDirector()->registerDemoCast(pActor, pName1, rIter);
        DemoFunction::tryCreateDemoTalkAnimCtrlForActorDirect(pActor, pName1, pName2, nullptr);
    }

    void registerDemoActionFunctorDirect(const LiveActor* pActor, const MR::FunctorBase& rFunctor, const char* pName1, const char* pName2) {
        DemoFunction::registerDemoActionFunctorFunction(pActor, rFunctor, pName1, pName2);
    }

    bool tryRegisterDemoActionFunctorDirect(const LiveActor* pActor, const MR::FunctorBase& rFunctor, const char* pName1, const char* pName2) {
        if (!DemoFunction::isRegisteredDemoActionFunctor(pActor, pName1)) {
            return false;
        }

        DemoFunction::registerDemoActionFunctorFunction(pActor, rFunctor, pName1, pName2);

        return true;
    }

    bool tryStartDemoRegistered(LiveActor* pActor, const char* pName) {
        return DemoFunction::tryStartDemoRegistered(pActor, pName);
    }

    bool tryStartDemoRegisteredMarioPuppetable(LiveActor* pActor, const char* pName) {
        return DemoFunction::tryStartDemoRegisteredMarioPuppetable(pActor, pName);
    }

    void registerDemoSimpleCastAll(LiveActor* pActor) {
        DemoFunction::registerDemoSimpleCastAllFunction(pActor);
    }

    void registerDemoSimpleCastAll(LayoutActor* pLayoutActor) {
        DemoFunction::registerDemoSimpleCastAllFunction(pLayoutActor);
    }

    void registerDemoSimpleCastAll(NameObj* pObj) {
        DemoFunction::registerDemoSimpleCastAllFunction(pObj);
    }

    bool isDemoCast(const LiveActor* pActor, const char* pName) {
        DemoExecutor* demoExecutor = DemoFunction::findDemoExecutor(pActor);

        if (demoExecutor == nullptr) {
            return false;
        }

        if (pName != nullptr) {
            return DemoFunction::isRegisteredDemoCast(pActor, pName);
        }

        return DemoFunction::isDemoCast(demoExecutor, pActor);
    }

    bool isRegisteredDemoActionAppear(const LiveActor* pActor) {
        return DemoFunction::isRegisteredDemoActionAppear(pActor);
    }

    bool isRegisteredDemoActionNerve(const LiveActor* pActor) {
        return DemoFunction::isRegisteredDemoActionNerve(pActor);
    }

    void endDemo(NameObj* pObj, const char* pName) {
        DemoFunction::getDemoDirector()->endDemo(pObj, pName, false);
    }

    void endDemoWaitCameraInterpolating(NameObj* pObj, const char* pName) {
        DemoFunction::getDemoDirector()->endDemo(pObj, pName, true);
    }

    void initDemoSheetTalkAnim(LiveActor* pActor, const JMapInfoIter& rIter, const char* pName1, const char* pName2, TalkMessageCtrl* pTalkCtrl) {
        tryInitDemoSheetTalkAnim(pActor, rIter, pName1, pName2, pTalkCtrl);
    }

    void initDemoSheetTalkAnimFunctor(LiveActor* pActor, const JMapInfoIter& rIter, const char* pName1, const char* pName2,
                                      TalkMessageCtrl* pTalkCtrl, const MR::FunctorBase& rFunctor) {
        if (tryInitDemoSheetTalkAnim(pActor, rIter, pName1, pName2, pTalkCtrl)) {
            DemoFunction::registerDemoActionFunctorFunction(pActor, rFunctor, pName1, nullptr);
        }
    }

    bool isDemoExist(const char* pName) {
        return DemoFunction::getDemoDirector()->isExistTimeKeepDemo(pName);
    }

    bool isDemoActive() {
        return DemoFunction::getDemoDirector()->mIsActive;
    }

    bool isDemoActive(const char* pName) {
        DemoExecutor* pExecutor = DemoFunction::getDemoDirector()->mExecutor;

        if (pExecutor != nullptr) {
            if (isName(pExecutor, pName)) {
                return true;
            }
        }

        const char* pDemoName = DemoFunction::getDemoDirector()->getCurrentDemoName();

        if (pDemoName == nullptr) {
            return false;
        }

        return isEqualString(pDemoName, pName);
    }

    bool canStartDemo() {
        if (isDemoActive()) {
            return false;
        }

        if (isPlayerDead()) {
            return false;
        }

        if (GameSceneFunction::isExecStageClearDemo()) {
            return false;
        }

        return !isPlayerConfrontDeath();
    }

    bool isTimeKeepDemoActive() {
        if (DemoFunction::getDemoDirector()->mIsActive == false) {
            return false;
        }

        return DemoFunction::getDemoDirector()->mExecutor != nullptr;
    }

    bool isDemoActiveRegistered(const LiveActor* pActor) {
        DemoExecutor* demoExecutor = DemoFunction::findDemoExecutor(pActor);

        if (demoExecutor == nullptr) {
            return false;
        }

        return DemoFunction::getDemoDirector()->mExecutor == demoExecutor;
    }

    bool isDemoPartExist(const LiveActor* pActor, const char* pName) {
        DemoExecutor* demoExecutor = DemoFunction::findDemoExecutor(pActor);

        if (demoExecutor != nullptr) {
            return DemoFunction::isExistDemoPart(demoExecutor, pName);
        }

        return false;
    }

    bool isDemoLastStep() {
        return DemoFunction::isDemoLastPartLastStep();
    }

    bool isDemoPartActive(const char* pName) {
        return DemoFunction::isDemoPartActiveFunction(pName);
    }

    bool isDemoPartStep(const char* pName, s32 step) {
        if (isDemoPartActive(pName) == false) {
            return false;
        }

        return getDemoPartStep(pName) == step;
    }

    bool isDemoPartFirstStep(const char* pName) {
        if (isDemoPartActive(pName) == false) {
            return false;
        }

        return getDemoPartStep(pName) == 0;
    }

    bool isDemoPartLastStep(const char *pName) {
        if (isDemoPartActive(pName) == false) {
            return false;
        }

        return getDemoPartStep(pName) == getDemoPartTotalStep(pName) - 1;
    }

    bool isDemoPartLessEqualStep(const char* pName, s32 step) {
        if (isDemoPartActive(pName) == false) {
            return false;
        }

        return getDemoPartStep(pName) <= step;
    }

    bool isDemoPartGreaterStep(const char* pName, s32 step) {
        if (isDemoPartActive(pName) == false) {
            return false;
        }

        return getDemoPartStep(pName) > step;
    }

    s32 getDemoPartTotalStep(const char* pName) {
        return DemoFunction::getDemoPartTotalStepFunction(pName);
    }

    f32 calcDemoPartStepRate(const char* pName) {
        return static_cast< f32 >(getDemoPartStep(pName)) / getDemoPartTotalStep(pName);
    }

    s32 getDemoPartStep(const char* pName) {
        return DemoFunction::getDemoPartStepFunction(pName);
    }

    void pauseTimeKeepDemo(LiveActor* pActor) {
        DemoFunction::pauseTimeKeepDemo(pActor);
    }

    void resumeTimeKeepDemo(LiveActor* pActor) {
        DemoFunction::resumeTimeKeepDemo(pActor);
    }

    bool isPowerStarGetDemoActive() {
        return GameSceneFunction::isExecStageClearDemo();
    }

    const char* getCurrentDemoPartNameMain(const char* pName) {
        return DemoFunction::getCurrentDemoPartNameMain(pName);
    }

    void endTalkingSequence(NameObj* pObj) {
        endDemo(pObj, "会話");
    }

    bool isSystemTalking() {
        if (isExistSceneObj(SceneObj_TalkDirector) == false) {
            return false;
        }

        return getTalkDirector()->isSystemTalking();
    }

    bool isNormalTalking() {
        if (isExistSceneObj(SceneObj_TalkDirector) == false) {
            return false;
        }

        return getTalkDirector()->isNormalTalking();
    }

    LiveActor* getTalkingActor() {
        if (isExistSceneObj(SceneObj_TalkDirector) == false) {
            return false;
        }

        return getTalkDirector()->getTalkingActor();
    }

    bool isDemoPartTalk(const char* pName) {
        return DemoFunction::isDemoPartTalk(pName);
    }
};  // namespace MR
