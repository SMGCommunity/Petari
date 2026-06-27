#include "Game/Util/DemoUtil.hpp"
#include "Game/Demo/DemoDirector.hpp"
#include "Game/Demo/DemoExecutor.hpp"
#include "Game/Demo/DemoFunction.hpp"
#include "Game/Demo/DemoPlayerKeeper.hpp"
#include "Game/Demo/DemoStartRequestUtil.hpp"
#include "Game/NPC/TalkDirector.hpp"
#include "Game/Scene/GameSceneFunction.hpp"
#include "Game/Scene/SceneObjHolder.hpp"
#include "Game/Util/ObjUtil.hpp"
#include "Game/Util/PlayerUtil.hpp"
#include "Game/Util/StringUtil.hpp"
#include "Game/Util/TalkUtil.hpp"

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
        DemoExecutor* executor = DemoFunction::findDemoExecutor(pActor);

        if (executor == nullptr) {
            return false;
        }

        if (pName != nullptr) {
            return DemoFunction::isRegisteredDemoCast(pActor, pName);
        }

        return DemoFunction::isDemoCast(executor, pActor);
    }

    bool isRegisteredDemoActionAppear(const LiveActor* pActor) {
        return DemoFunction::isRegisteredDemoActionAppear(pActor);
    }

    bool isRegisteredDemoActionNerve(const LiveActor* pActor) {
        return DemoFunction::isRegisteredDemoActionNerve(pActor);
    }

    bool tryStartDemo(LiveActor* pActor, const char* pName) {
        if (!canStartDemo()) {
            return false;
        }

        DemoStartRequestUtil::startDemoSystem(pActor, pName, 1, DemoStartInfo::DemoType(0), DemoStartInfo::CinemaFrameType(0),
                                              DemoStartInfo::StarPointerType(0), DemoStartInfo::DeleteEffectType(0), nullptr);

        return true;
    }

    bool tryStartDemo(LayoutActor* pActor, const char* pName) {
        if (!canStartDemo()) {
            return false;
        }

        DemoStartRequestUtil::startDemoSystem(pActor, pName, 1, DemoStartInfo::DemoType(0), DemoStartInfo::CinemaFrameType(0),
                                              DemoStartInfo::StarPointerType(0), DemoStartInfo::DeleteEffectType(0), nullptr);

        return true;
    }

    bool tryStartDemoWithoutCinemaFrame(LiveActor* pActor, const char* pName) {
        if (!canStartDemo()) {
            return false;
        }

        DemoStartRequestUtil::startDemoSystem(pActor, pName, 1, DemoStartInfo::DemoType(0), DemoStartInfo::CinemaFrameType(1),
                                              DemoStartInfo::StarPointerType(0), DemoStartInfo::DeleteEffectType(0), nullptr);

        return true;
    }

    bool tryStartDemoWithoutCinemaFrameValidStarPointer(LiveActor* pActor, const char* pName) {
        if (!canStartDemo()) {
            return false;
        }

        DemoStartRequestUtil::startDemoSystem(pActor, pName, 1, DemoStartInfo::DemoType(0), DemoStartInfo::CinemaFrameType(1),
                                              DemoStartInfo::StarPointerType(1), DemoStartInfo::DeleteEffectType(0), nullptr);

        return true;
    }

    bool tryStartDemoWithoutCinemaFrameValidHandPointerFinger(NameObj* pObj, const char* pName) {
        if (!canStartDemo()) {
            return false;
        }

        DemoStartRequestUtil::startDemoSystem(pObj, pName, 1, DemoStartInfo::DemoType(0), DemoStartInfo::CinemaFrameType(1),
                                              DemoStartInfo::StarPointerType(2), DemoStartInfo::DeleteEffectType(0), nullptr);

        return true;
    }

    bool tryStartDemoWithoutCinemaFrameValidHandPointerFinger(LayoutActor* pActor, const char* pName) {
        if (!canStartDemo()) {
            return false;
        }

        DemoStartRequestUtil::startDemoSystem(pActor, pName, 1, DemoStartInfo::DemoType(0), DemoStartInfo::CinemaFrameType(1),
                                              DemoStartInfo::StarPointerType(2), DemoStartInfo::DeleteEffectType(0), nullptr);

        return true;
    }

    bool tryStartDemoMarioPuppetable(LiveActor* pActor, const char* pName) {
        if (!canStartDemo()) {
            return false;
        }

        DemoStartRequestUtil::startDemoSystem(pActor, pName, 2, DemoStartInfo::DemoType(0), DemoStartInfo::CinemaFrameType(0),
                                              DemoStartInfo::StarPointerType(0), DemoStartInfo::DeleteEffectType(0), nullptr);

        return true;
    }

    bool tryStartDemoMarioPuppetableWithoutCinemaFrame(LiveActor* pActor, const char* pName) {
        if (!canStartDemo()) {
            return false;
        }

        DemoStartRequestUtil::startDemoSystem(pActor, pName, 2, DemoStartInfo::DemoType(0), DemoStartInfo::CinemaFrameType(1),
                                              DemoStartInfo::StarPointerType(0), DemoStartInfo::DeleteEffectType(0), nullptr);

        return true;
    }

    bool tryStartTimeKeepDemo(NameObj* pObj, const char* pName, const char* pPartName) {
        if (!canStartDemo()) {
            return false;
        }

        startTimeKeepDemo(pObj, pName, pPartName);

        return true;
    }

    bool tryStartTimeKeepDemoMarioPuppetable(NameObj* pObj, const char* pName, const char* pPartName) {
        if (!canStartDemo()) {
            return false;
        }

        DemoExecutor* executor = DemoFunction::findDemoExecutor(pName);

        if (pPartName != nullptr) {
            executor->startDemoSystemPart(pPartName, 2);
        } else {
            bool isExistPosName = executor->mPlayerKeeper != nullptr && executor->mPlayerKeeper->isExistPosName();

            DemoStartRequestUtil::startDemoSystem(pObj, pName, 2, DemoStartInfo::DemoType(1), DemoStartInfo::CinemaFrameType(0),
                                                  DemoStartInfo::StarPointerType(0), DemoStartInfo::DeleteEffectType(isExistPosName), nullptr);
        }

        return true;
    }

    bool tryStartTimeKeepDemoMarioPuppetable(LiveActor* pActor, const char* pName, const char* pPartName) {
        if (!canStartDemo()) {
            return false;
        }

        DemoExecutor* executor = DemoFunction::findDemoExecutor(pName);

        if (pPartName != nullptr) {
            executor->startDemoSystemPart(pPartName, 2);
        } else {
            bool isExistPosName = executor->mPlayerKeeper != nullptr && executor->mPlayerKeeper->isExistPosName();

            DemoStartRequestUtil::startDemoSystem(pActor, pName, 2, DemoStartInfo::DemoType(1), DemoStartInfo::CinemaFrameType(0),
                                                  DemoStartInfo::StarPointerType(0), DemoStartInfo::DeleteEffectType(isExistPosName), nullptr);
        }

        return true;
    }

    bool requestStartDemo(LiveActor* pActor, const char* pName, const Nerve* pCanStartDemoNerve, const Nerve* pCannotStartDemoNerve) {
        return DemoStartRequestUtil::requestStartDemo(pActor, pName, pCanStartDemoNerve, pCannotStartDemoNerve, 1, DemoStartInfo::DemoType(0),
                                                      DemoStartInfo::CinemaFrameType(0), DemoStartInfo::StarPointerType(0),
                                                      DemoStartInfo::DeleteEffectType(0));
    }

    bool requestStartDemoWithoutCinemaFrame(LiveActor* pActor, const char* pName, const Nerve* pCanStartDemoNerve,
                                            const Nerve* pCannotStartDemoNerve) {
        return DemoStartRequestUtil::requestStartDemo(pActor, pName, pCanStartDemoNerve, pCannotStartDemoNerve, 1, DemoStartInfo::DemoType(0),
                                                      DemoStartInfo::CinemaFrameType(1), DemoStartInfo::StarPointerType(0),
                                                      DemoStartInfo::DeleteEffectType(0));
    }

    bool requestStartDemoWithoutCinemaFrame(LayoutActor* pActor, const char* pName, const Nerve* pCanStartDemoNerve,
                                            const Nerve* pCannotStartDemoNerve) {
        return DemoStartRequestUtil::requestStartDemo(pActor, pName, pCanStartDemoNerve, pCannotStartDemoNerve, 1, DemoStartInfo::DemoType(0),
                                                      DemoStartInfo::CinemaFrameType(1), DemoStartInfo::StarPointerType(0),
                                                      DemoStartInfo::DeleteEffectType(0));
    }

    bool requestStartDemoMarioPuppetable(LiveActor* pActor, const char* pName, const Nerve* pCanStartDemoNerve, const Nerve* pCannotStartDemoNerve) {
        return DemoStartRequestUtil::requestStartDemo(pActor, pName, pCanStartDemoNerve, pCannotStartDemoNerve, 2, DemoStartInfo::DemoType(0),
                                                      DemoStartInfo::CinemaFrameType(0), DemoStartInfo::StarPointerType(0),
                                                      DemoStartInfo::DeleteEffectType(0));
    }

    bool requestStartDemoMarioPuppetable(NerveExecutor* pExecutor, LiveActor* pActor, const char* pName, const Nerve* pCanStartDemoNerve,
                                         const Nerve* pCannotStartDemoNerve) {
        return DemoStartRequestUtil::requestStartDemo(pExecutor, pActor, pName, pCanStartDemoNerve, pCannotStartDemoNerve, 2,
                                                      DemoStartInfo::DemoType(0), DemoStartInfo::CinemaFrameType(0),
                                                      DemoStartInfo::StarPointerType(0), DemoStartInfo::DeleteEffectType(0));
    }

    bool requestStartDemoMarioPuppetableWithoutCinemaFrame(LiveActor* pActor, const char* pName, const Nerve* pCanStartDemoNerve,
                                                           const Nerve* pCannotStartDemoNerve) {
        return DemoStartRequestUtil::requestStartDemo(pActor, pName, pCanStartDemoNerve, pCannotStartDemoNerve, 2, DemoStartInfo::DemoType(0),
                                                      DemoStartInfo::CinemaFrameType(1), DemoStartInfo::StarPointerType(0),
                                                      DemoStartInfo::DeleteEffectType(0));
    }

    bool requestStartTimeKeepDemo(LiveActor* pActor, const char* pName, const Nerve* pCanStartDemoNerve, const Nerve* pCannotStartDemoNerve,
                                  const char* pParam5) {
        return DemoStartRequestUtil::requestStartTimeKeepDemo(pActor, pName, pParam5, pCanStartDemoNerve, pCannotStartDemoNerve, 1,
                                                              DemoStartInfo::DemoType(1), DemoStartInfo::CinemaFrameType(0),
                                                              DemoStartInfo::StarPointerType(0), DemoStartInfo::DeleteEffectType(0));
    }

    bool requestStartTimeKeepDemo(NameObj* pObj, const char* pName, const char* pParam3) {
        return DemoStartRequestUtil::requestStartTimeKeepDemo(pObj, pName, pParam3, 1, DemoStartInfo::DemoType(1), DemoStartInfo::CinemaFrameType(0),
                                                              DemoStartInfo::StarPointerType(0), DemoStartInfo::DeleteEffectType(0));
    }

    bool requestStartTimeKeepDemoMarioPuppetable(LiveActor* pActor, const char* pName, const Nerve* pCanStartDemoNerve,
                                                 const Nerve* pCannotStartDemoNerve, const char* pParam5) {
        return DemoStartRequestUtil::requestStartTimeKeepDemo(pActor, pName, pParam5, pCanStartDemoNerve, pCannotStartDemoNerve, 2,
                                                              DemoStartInfo::DemoType(1), DemoStartInfo::CinemaFrameType(0),
                                                              DemoStartInfo::StarPointerType(0), DemoStartInfo::DeleteEffectType(0));
    }

    bool requestStartTimeKeepDemoMarioPuppetable(NerveExecutor* pExecutor, LiveActor* pActor, const char* pName, const Nerve* pCanStartDemoNerve,
                                                 const Nerve* pCannotStartDemoNerve, const char* pParam6) {
        return DemoStartRequestUtil::requestStartTimeKeepDemo(pExecutor, pActor, pName, pParam6, pCanStartDemoNerve, pCannotStartDemoNerve, 2,
                                                              DemoStartInfo::DemoType(1), DemoStartInfo::CinemaFrameType(0),
                                                              DemoStartInfo::StarPointerType(0), DemoStartInfo::DeleteEffectType(0));
    }

    bool requestStartTimeKeepDemoMarioPuppetable(NameObj* pObj, const char* pName, const char* pParam3) {
        return DemoStartRequestUtil::requestStartTimeKeepDemo(pObj, pName, pParam3, 2, DemoStartInfo::DemoType(1), DemoStartInfo::CinemaFrameType(0),
                                                              DemoStartInfo::StarPointerType(0), DemoStartInfo::DeleteEffectType(0));
    }

    bool requestStartTimeKeepDemoWithoutCinemaFrame(LiveActor* pActor, const char* pName, const Nerve* pCanStartDemoNerve,
                                                    const Nerve* pCannotStartDemoNerve, const char* pParam4) {
        return DemoStartRequestUtil::requestStartTimeKeepDemo(pActor, pName, pParam4, pCanStartDemoNerve, pCannotStartDemoNerve, 1,
                                                              DemoStartInfo::DemoType(1), DemoStartInfo::CinemaFrameType(1),
                                                              DemoStartInfo::StarPointerType(0), DemoStartInfo::DeleteEffectType(0));
    }

    bool requestStartDemoRegistered(LiveActor* pActor, const Nerve* pCanStartDemoNerve, const Nerve* pCannotStartDemoNerve, const char* pParam4) {
        DemoExecutor* executor = DemoFunction::findDemoExecutor(pActor);

        if (executor == nullptr) {
            return false;
        }

        return DemoStartRequestUtil::requestStartTimeKeepDemo(pActor, executor->mName, pParam4, pCanStartDemoNerve, pCannotStartDemoNerve, 1,
                                                              DemoStartInfo::DemoType(1), DemoStartInfo::CinemaFrameType(0),
                                                              DemoStartInfo::StarPointerType(0), DemoStartInfo::DeleteEffectType(0));
    }

    bool requestStartDemoRegisteredMarioPuppetable(LiveActor* pActor, const Nerve* pCanStartDemoNerve, const Nerve* pCannotStartDemoNerve,
                                                   const char* pParam4) {
        DemoExecutor* executor = DemoFunction::findDemoExecutor(pActor);

        if (executor == nullptr) {
            return false;
        }

        return DemoStartRequestUtil::requestStartTimeKeepDemo(pActor, executor->mName, pParam4, pCanStartDemoNerve, pCannotStartDemoNerve, 2,
                                                              DemoStartInfo::DemoType(1), DemoStartInfo::CinemaFrameType(0),
                                                              DemoStartInfo::StarPointerType(0), DemoStartInfo::DeleteEffectType(0));
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

    void initDemoSheetTalkAnimWithMessageName(LiveActor* pActor, const JMapInfoIter& rIter, const char* pParam3, const char* pParam4,
                                              const char* pParam5) {
        TalkMessageCtrl* talkCtrl = MR::createTalkCtrl(pActor, rIter, pParam5, TVec3f(0.0f, 0.0f, 0.0f), nullptr);

        MR::onRootNodeAutomatic(talkCtrl);
        MR::tryInitDemoSheetTalkAnim(pActor, rIter, pParam3, pParam4, talkCtrl);
    }

    void initDemoSheetTalkAnimFunctor(LiveActor* pActor, const JMapInfoIter& rIter, const char* pName1, const char* pName2,
                                      TalkMessageCtrl* pTalkCtrl, const MR::FunctorBase& rFunctor) {
        if (tryInitDemoSheetTalkAnim(pActor, rIter, pName1, pName2, pTalkCtrl)) {
            DemoFunction::registerDemoActionFunctorFunction(pActor, rFunctor, pName1, nullptr);
        }
    }

    bool tryInitDemoSheetTalkAnim(LiveActor* pActor, const JMapInfoIter& rIter, const char* pParam3, const char* pParam4,
                                  TalkMessageCtrl* pTalkCtrl) {
        if (tryRegisterDemoCast(pActor, pParam3, rIter)) {
            if (pTalkCtrl == nullptr) {
                pTalkCtrl = MR::createTalkCtrl(pActor, rIter, pParam4, TVec3f(0.0f, 0.0f, 0.0f), nullptr);

                MR::onRootNodeAutomatic(pTalkCtrl);
            }

            DemoFunction::registerDemoTalkMessageCtrlDirect(pActor, pTalkCtrl, pParam3);
            DemoFunction::tryCreateDemoTalkAnimCtrlForSceneDirect(pActor, pParam3, rIter, pParam4, nullptr, 0, 0);

            return true;
        }

        return false;
    }

    void startTimeKeepDemo(NameObj* pObj, const char* pName, const char* pPartName) {
        if (pPartName != nullptr) {
            DemoFunction::findDemoExecutor(pName)->startDemoSystemPart(pPartName, 1);
        } else {
            DemoStartRequestUtil::startDemoSystem(pObj, pName, 1, DemoStartInfo::DemoType(1), DemoStartInfo::CinemaFrameType(0),
                                                  DemoStartInfo::StarPointerType(0), DemoStartInfo::DeleteEffectType(0), nullptr);
        }
    }

    void startTimeKeepDemoMarioPuppetable(NameObj* pObj, const char* pName, const char* pPartName) {
        DemoExecutor* executor = DemoFunction::findDemoExecutor(pName);

        if (pPartName != nullptr) {
            executor->startDemoSystemPart(pPartName, 2);
        } else {
            bool isExistPosName = executor->mPlayerKeeper != nullptr && executor->mPlayerKeeper->isExistPosName();

            DemoStartRequestUtil::startDemoSystem(pObj, pName, 2, DemoStartInfo::DemoType(1), DemoStartInfo::CinemaFrameType(0),
                                                  DemoStartInfo::StarPointerType(0), DemoStartInfo::DeleteEffectType(isExistPosName), nullptr);
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
        DemoExecutor* executor = DemoFunction::findDemoExecutor(pActor);

        if (executor == nullptr) {
            return false;
        }

        return DemoFunction::getDemoDirector()->mExecutor == executor;
    }

    bool isDemoPartExist(const LiveActor* pActor, const char* pName) {
        DemoExecutor* executor = DemoFunction::findDemoExecutor(pActor);

        if (executor != nullptr) {
            return DemoFunction::isExistDemoPart(executor, pName);
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

    bool isDemoPartLastStep(const char* pName) {
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

    void startTalkingSequenceWithoutCinemaFrame(NameObj* pObj) {
        DemoStartRequestUtil::startDemoSystem(pObj, "会話", 3, DemoStartInfo::DemoType(0), DemoStartInfo::CinemaFrameType(1),
                                              DemoStartInfo::StarPointerType(0), DemoStartInfo::DeleteEffectType(0), nullptr);
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

    void startTalkingSequence(NameObj* pObj) {
        DemoStartRequestUtil::startDemoSystem(pObj, "会話", 3, DemoStartInfo::DemoType(0), DemoStartInfo::CinemaFrameType(0),
                                              DemoStartInfo::StarPointerType(0), DemoStartInfo::DeleteEffectType(0), nullptr);
    }
};  // namespace MR
