#include "Game/Demo/DemoStartRequestUtil.hpp"
#include "Game/Demo/DemoDirector.hpp"
#include "Game/Demo/DemoExecutor.hpp"
#include "Game/Demo/DemoFunction.hpp"
#include "Game/Demo/DemoStartRequestHolder.hpp"
#include "Game/Effect/EffectSystemUtil.hpp"
#include "Game/Player/MarioAccess.hpp"
#include "Game/Scene/SceneNameObjMovementController.hpp"
#include "Game/Screen/LayoutActor.hpp"
#include "Game/Util.hpp"


namespace {
    inline void setDemoStartInfoCommon(DemoStartInfo* pStartInfo, const char* demoName, const char* pChar2, s32 a4, DemoStartInfo::DemoType demoType,
                                       DemoStartInfo::CinemaFrameType frameType, DemoStartInfo::StarPointerType pointerType,
                                       DemoStartInfo::DeleteEffectType deleteEffectType) {
        pStartInfo->mDemoName = demoName;
        pStartInfo->mDemoType = demoType;
        pStartInfo->_24 = a4;
        pStartInfo->mFrameType = frameType;
        pStartInfo->mPointerType = pointerType;
        pStartInfo->mDeleteEffectType = deleteEffectType;
        pStartInfo->mDemoExecutor = DemoFunction::findDemoExecutor(demoName);
        pStartInfo->_1C = pChar2;
    }
    
};  // namespace

namespace DemoStartRequestUtil {
    void startDemoSystem(NameObj* pObj, const char* demoName, s32 a3, DemoStartInfo::DemoType demoType, DemoStartInfo::CinemaFrameType frameType,
                         DemoStartInfo::StarPointerType pointerType, DemoStartInfo::DeleteEffectType deleteEffectType, const char* pChar2) {
        MR::getSceneNameObjMovementController()->requestStopSceneFor(MR::MovementControlType(a3), pObj);
        bool isCinemaFrame = frameType == DemoStartInfo::CinemaFrameType_0;
        if (demoType == DemoStartInfo::DemoType_TimeKeep) {
            DemoFunction::getDemoDirector()->startDemoTimeKeep(pObj, demoName, a3, isCinemaFrame, pChar2);
        } else {
            DemoFunction::getDemoDirector()->startDemoProgrammable(pObj, demoName, isCinemaFrame, a3);
        }

        MR::deactivateDefaultGameLayout();

        if (isCinemaFrame) {
            MR::tryScreenToFrameCinemaFrame();
        }

        switch (pointerType) {
        case DemoStartInfo::StarPointerType_1:
            MR::startStarPointerModeDemoWithStarPointer(pObj);
            break;
        case DemoStartInfo::StarPointerType_2:
            MR::startStarPointerModeDemoWithHandPointerFinger(pObj);
            break;
        default:
            MR::startStarPointerModeDemo(pObj);
            break;
        }

        MR::requestMovementOn(pObj);
        MR::pauseOffCameraDirector();
        MR::pauseOffLensFlare();

        if (deleteEffectType == DemoStartInfo::DeleteEffectType_1) {
            MR::Effect::forceDeleteAllOneTimeEmitter();
        }

        // inline?
        bool remoteDemo = false;
        if (MR::MovementControlType(a3) == MR::MovementControlType_2 || MR::MovementControlType(a3) == MR::MovementControlType_3) {
            remoteDemo = true;
        }

        if (remoteDemo) {
            MarioAccess::readyRemoteDemo();
        }

        MR::sendMsgToAllLiveActor(ACTMES_START_DEMO, nullptr);
    }

    void startDemoSystem(LiveActor* pActor, const char* demoName, s32 a3, DemoStartInfo::DemoType demoType, DemoStartInfo::CinemaFrameType frameType,
                         DemoStartInfo::StarPointerType pointerType, DemoStartInfo::DeleteEffectType deleteEffectType, const char* pChar2) {
        startDemoSystem(static_cast< NameObj* >(pActor), demoName, a3, demoType, frameType, pointerType, deleteEffectType, pChar2);
        MR::requestMovementOn(pActor);
    }

    void startDemoSystem(LayoutActor* pActor, const char* demoName, s32 a3, DemoStartInfo::DemoType demoType,
                         DemoStartInfo::CinemaFrameType frameType, DemoStartInfo::StarPointerType pointerType,
                         DemoStartInfo::DeleteEffectType deleteEffectType, const char* pChar2) {
        startDemoSystem(static_cast< NameObj* >(pActor), demoName, a3, demoType, frameType, pointerType, deleteEffectType, pChar2);
        MR::requestMovementOn(pActor);
    }

    bool requestStartDemo(LiveActor* pActor, const char* demoName, const Nerve* pDemoNerve, const Nerve* pNoDemoNerve, s32 a5,
                          DemoStartInfo::DemoType demoType, DemoStartInfo::CinemaFrameType frameType, DemoStartInfo::StarPointerType pointerType,
                          DemoStartInfo::DeleteEffectType deleteEffectType) {
        if (MR::canStartDemo()) {
            if (pDemoNerve != nullptr) {
                pActor->setNerve(pDemoNerve);
            }

            startDemoSystem(pActor, demoName, a5, demoType, frameType, pointerType, deleteEffectType, nullptr);
            if (MR::isRegisteredEffect(pActor, nullptr)) {
                MR::pauseOffEffectAll(pActor);
            }
            return true;
        }

        if (pNoDemoNerve != nullptr) {
            pActor->setNerve(pNoDemoNerve);
        }
        DemoStartRequestHolder* requestHolder = DemoFunction::getDemoDirector()->mStartRequestHolder;

        DemoStartInfo startInfo;
        startInfo._0 = pActor;
        startInfo._20 = pDemoNerve;
        ::setDemoStartInfoCommon(&startInfo, demoName, nullptr, a5, demoType, frameType, pointerType, deleteEffectType);

        requestHolder->registerStartDemoInfo(startInfo);

        requestHolder->pushRequest(pActor, demoName);

        return false;
    }

    bool requestStartDemo(LayoutActor* pActor, const char* demoName, const Nerve* pDemoNerve, const Nerve* pNoDemoNerve, s32 a5,
                          DemoStartInfo::DemoType demoType, DemoStartInfo::CinemaFrameType frameType, DemoStartInfo::StarPointerType pointerType,
                          DemoStartInfo::DeleteEffectType deleteEffectType) {
        if (MR::canStartDemo()) {
            if (pDemoNerve != nullptr) {
                pActor->setNerve(pDemoNerve);
            }

            startDemoSystem(pActor, demoName, a5, demoType, frameType, pointerType, deleteEffectType, nullptr);
            if (MR::isRegisteredEffect(pActor, nullptr)) {
                MR::pauseOffEffectAll(pActor);
            }
            return true;
        }

        if (pNoDemoNerve != nullptr) {
            pActor->setNerve(pNoDemoNerve);
        }
        DemoStartRequestHolder* requestHolder = DemoFunction::getDemoDirector()->mStartRequestHolder;

        DemoStartInfo startInfo;
        startInfo._4 = pActor;
        startInfo._20 = pDemoNerve;
        ::setDemoStartInfoCommon(&startInfo, demoName, nullptr, a5, demoType, frameType, pointerType, deleteEffectType);

        requestHolder->registerStartDemoInfo(startInfo);

        requestHolder->pushRequest(pActor, demoName);

        return false;
    }

    bool requestStartDemo(NerveExecutor* pExecutor, LiveActor* pActor, const char* demoName, const Nerve* pDemoNerve, const Nerve* pNoDemoNerve,
                          s32 a6, DemoStartInfo::DemoType demoType, DemoStartInfo::CinemaFrameType frameType,
                          DemoStartInfo::StarPointerType pointerType, DemoStartInfo::DeleteEffectType deleteEffectType) {
        if (MR::canStartDemo()) {
            if (pDemoNerve != nullptr) {
                pExecutor->setNerve(pDemoNerve);
            }

            startDemoSystem(pActor, demoName, a6, demoType, frameType, pointerType, deleteEffectType, nullptr);

            return true;
        }

        if (pNoDemoNerve != nullptr) {
            pExecutor->setNerve(pNoDemoNerve);
        }
        DemoStartRequestHolder* requestHolder = DemoFunction::getDemoDirector()->mStartRequestHolder;

        DemoStartInfo startInfo;
        startInfo._8 = pExecutor;
        startInfo._20 = pDemoNerve;
        startInfo._10 = pActor;
        ::setDemoStartInfoCommon(&startInfo, demoName, nullptr, a6, demoType, frameType, pointerType, deleteEffectType);

        requestHolder->registerStartDemoInfo(startInfo);

        requestHolder->pushRequest(pExecutor, demoName);

        return false;
    }

    bool requestStartTimeKeepDemo(LiveActor* pActor, const char* demoName, const char* pChar, const Nerve* pDemoNerve, const Nerve* pNoDemoNerve,
                                  s32 a5, DemoStartInfo::DemoType demoType, DemoStartInfo::CinemaFrameType frameType,
                                  DemoStartInfo::StarPointerType pointerType, DemoStartInfo::DeleteEffectType deleteEffectType) {
        if (MR::canStartDemo()) {
            if (pDemoNerve != nullptr) {
                pActor->setNerve(pDemoNerve);
            }

            startDemoSystem(pActor, demoName, a5, demoType, frameType, pointerType, deleteEffectType, pChar);

            return true;
        }

        if (pNoDemoNerve != nullptr) {
            pActor->setNerve(pNoDemoNerve);
        }
        DemoStartRequestHolder* requestHolder = DemoFunction::getDemoDirector()->mStartRequestHolder;

        DemoStartInfo startInfo;
        startInfo._0 = pActor;
        startInfo._20 = pDemoNerve;
        ::setDemoStartInfoCommon(&startInfo, demoName, pChar, a5, demoType, frameType, pointerType, deleteEffectType);

        requestHolder->registerStartDemoInfo(startInfo);

        requestHolder->pushRequest(pActor, demoName);

        return false;
    }

    bool requestStartTimeKeepDemo(NerveExecutor* pExecutor, LiveActor* pActor, const char* demoName, const char* pChar, const Nerve* pDemoNerve,
                                  const Nerve* pNoDemoNerve, s32 a5, DemoStartInfo::DemoType demoType, DemoStartInfo::CinemaFrameType frameType,
                                  DemoStartInfo::StarPointerType pointerType, DemoStartInfo::DeleteEffectType deleteEffectType) {
        if (MR::canStartDemo()) {
            if (pDemoNerve != nullptr) {
                pExecutor->setNerve(pDemoNerve);
            }

            startDemoSystem(pActor, demoName, a5, demoType, frameType, pointerType, deleteEffectType, pChar);

            return true;
        }

        if (pNoDemoNerve != nullptr) {
            pExecutor->setNerve(pNoDemoNerve);
        }
        DemoStartRequestHolder* requestHolder = DemoFunction::getDemoDirector()->mStartRequestHolder;

        DemoStartInfo startInfo;
        startInfo._8 = pExecutor;
        startInfo._20 = pDemoNerve;
        startInfo._10 = pActor;
        ::setDemoStartInfoCommon(&startInfo, demoName, pChar, a5, demoType, frameType, pointerType, deleteEffectType);

        requestHolder->registerStartDemoInfo(startInfo);

        requestHolder->pushRequest(pExecutor, demoName);

        return false;
    }

    bool requestStartTimeKeepDemo(NameObj* pObj, const char* demoName, const char* pChar, s32 a5, DemoStartInfo::DemoType demoType,
                                  DemoStartInfo::CinemaFrameType frameType, DemoStartInfo::StarPointerType pointerType,
                                  DemoStartInfo::DeleteEffectType deleteEffectType) {
        if (MR::canStartDemo()) {
            startDemoSystem(pObj, demoName, a5, demoType, frameType, pointerType, deleteEffectType, pChar);

            return true;
        }

        DemoStartRequestHolder* requestHolder = DemoFunction::getDemoDirector()->mStartRequestHolder;

        DemoStartInfo startInfo;
        startInfo._C = pObj;
        ::setDemoStartInfoCommon(&startInfo, demoName, pChar, a5, demoType, frameType, pointerType, deleteEffectType);

        requestHolder->registerStartDemoInfo(startInfo);

        requestHolder->pushRequest(pObj, demoName);

        return false;
    }

    void startDemo(const DemoStartInfo* pStartInfo) {
        NameObj* demoStarter = getDemoStarter(*pStartInfo);
        MR::getSceneNameObjMovementController()->requestStopSceneFor(MR::MovementControlType(pStartInfo->_24), demoStarter);
        MR::deactivateDefaultGameLayout();
        if (pStartInfo->mFrameType == DemoStartInfo::CinemaFrameType_0) {
            MR::tryScreenToFrameCinemaFrame();
        }

        switch (pStartInfo->mPointerType) {
        case DemoStartInfo::StarPointerType_1:
            MR::startStarPointerModeDemoWithStarPointer(demoStarter);
            break;
        case DemoStartInfo::StarPointerType_2:
            MR::startStarPointerModeDemoWithHandPointerFinger(demoStarter);
            break;
        default:
            MR::startStarPointerModeDemo(demoStarter);
            break;
        }

        requestMovementOn(pStartInfo);

        if (pStartInfo->_20 != nullptr) {
            setNerveToStarter(pStartInfo);
        }

        MR::pauseOffCameraDirector();
        MR::pauseOffLensFlare();

        if (pStartInfo->mDeleteEffectType == DemoStartInfo::DeleteEffectType_1) {
            MR::Effect::forceDeleteAllOneTimeEmitter();
        }

        if (pStartInfo->_0 != nullptr && MR::isRegisteredEffect(pStartInfo->_0, nullptr)) {
            MR::pauseOffEffectAll(pStartInfo->_0);
        }

        if (pStartInfo->_4 != nullptr && MR::isRegisteredEffect(pStartInfo->_4, nullptr)) {
            MR::pauseOffEffectAll(pStartInfo->_4);
        }

        bool remoteDemo = true;
        if (MR::MovementControlType(pStartInfo->_24) != MR::MovementControlType_2 &&
            MR::MovementControlType(pStartInfo->_24) != MR::MovementControlType_3) {
            remoteDemo = false;
        }

        if (remoteDemo) {
            MarioAccess::readyRemoteDemo();
        }

        if (pStartInfo->mDemoType == DemoStartInfo::DemoType_TimeKeep) {
            NameObj* obj = pStartInfo->_0;
            if (obj == nullptr) {
                obj = DemoFunction::getDemoDirector()->mStartRequestHolder->mProxyObj;
            }
            DemoFunction::getDemoDirector()->startDemoExecutor(obj, pStartInfo->mDemoName, pStartInfo->_24, nullptr);
        }

        startDemoExecutorIfExist(*pStartInfo);
        MR::sendMsgToAllLiveActor(ACTMES_START_DEMO, nullptr);
    }

    void startDemo(DemoStartRequestHolder* pStartRequestHolder) {
        if (pStartRequestHolder->getCurrentInfo() != nullptr) {
            startDemo(pStartRequestHolder->getCurrentInfo());
        }
    }

    bool popStartDemoRequest(DemoStartRequestHolder* pStartRequestHolder) {
        if (pStartRequestHolder->getCurrentInfo() == nullptr) {
            return false;
        }

        pStartRequestHolder->popRequest();
        return true;
    }

    bool isExistStartDemoRequest(const DemoStartRequestHolder* pStartRequestHolder) {
        return pStartRequestHolder->isExistRequest();
    }

    NameObj* getDemoStarter(const DemoStartInfo& rStartInfo) {
        if (rStartInfo._0 != nullptr) {
            return rStartInfo._0;
        }

        if (rStartInfo._4 != nullptr) {
            return rStartInfo._4;
        }

        if (rStartInfo._C != nullptr) {
            return rStartInfo._C;
        }

        if (rStartInfo._8 != nullptr) {
            return rStartInfo._10;
        }

        return nullptr;
    }

    void setNerveToStarter(const DemoStartInfo* pStartInfo) {
        if (pStartInfo->_0 != nullptr) {
            pStartInfo->_0->setNerve(pStartInfo->_20);
            return;
        }

        if (pStartInfo->_4 != nullptr) {
            pStartInfo->_4->setNerve(pStartInfo->_20);
            return;
        }

        if (pStartInfo->_8 != nullptr) {
            pStartInfo->_8->setNerve(pStartInfo->_20);
        }
    }

    void requestMovementOn(const DemoStartInfo* pStartInfo) {
        if (pStartInfo->_0 != nullptr) {
            MR::requestMovementOn(pStartInfo->_0);
        }

        if (pStartInfo->_4 != nullptr) {
            MR::requestMovementOn(pStartInfo->_4);
        }

        if (pStartInfo->_C != nullptr) {
            MR::requestMovementOn(pStartInfo->_C);
        }

        if (pStartInfo->_10 != nullptr) {
            MR::requestMovementOn(pStartInfo->_10);
        }
    }

    bool isEmpty(const DemoStartInfo* pStartInfo) {
        if (pStartInfo->_0 != nullptr) {
            return false;
        }

        if (pStartInfo->_4 != nullptr) {
            return false;
        }

        if (pStartInfo->_8 != nullptr) {
            return false;
        }

        if (pStartInfo->_C != nullptr) {
            return false;
        }

        return true;
    }

    bool startDemoExecutorIfExist(const DemoStartInfo& rStartInfo) {
        if (rStartInfo.mDemoExecutor == nullptr) {
            return false;
        }

        if (rStartInfo._1C != nullptr) {
            rStartInfo.mDemoExecutor->startPart(getDemoStarter(rStartInfo), rStartInfo.mDemoName, rStartInfo._1C, rStartInfo._24);
        } else {
            rStartInfo.mDemoExecutor->start(getDemoStarter(rStartInfo), rStartInfo.mDemoName, rStartInfo._24);
        }
        return true;
    }
};  // namespace DemoStartRequestUtil
