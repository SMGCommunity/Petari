#include "Game/Scene/SceneNameObjMovementController.hpp"
#include "Game/Camera/CameraContext.hpp"
#include "Game/Camera/CameraDirector.hpp"
#include "Game/LiveActor/ClippingDirector.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/LiveActor/SensorHitChecker.hpp"
#include "Game/Map/CollisionDirector.hpp"
#include "Game/Map/LightDirector.hpp"
#include "Game/Map/SleepControllerHolder.hpp"
#include "Game/Map/SwitchWatcherHolder.hpp"
#include "Game/Map/WaterAreaHolder.hpp"
#include "Game/NPC/TalkDirector.hpp"
#include "Game/NameObj/NameObjGroup.hpp"
#include "Game/Scene/SceneObjHolder.hpp"
#include "Game/Screen/CinemaFrame.hpp"
#include "Game/Screen/InformationObserver.hpp"
#include "Game/Util/EffectUtil.hpp"
#include "Game/Util/NerveUtil.hpp"
#include "Game/Util/ObjUtil.hpp"
#include "Game/Util/ScreenUtil.hpp"
#include "Game/Util/SystemUtil.hpp"

namespace {
    void executeStopCategoriesDemoCommon() {
        MR::suspendAllSceneNameObj();
        MR::getSceneObj< NameObjGroup >(SceneObj_NameObjGroup)->pauseOffAll();
        MR::notifyRequestNameObjMovementOnOff();
        MR::requestEffectStopSceneStart();
        CategoryList::requestMovementOn(MR::MovementType_UNK_0x14);
        CategoryList::requestMovementOn(MR::MovementType_ShadowControllerHolder);
        CategoryList::requestMovementOn(MR::MovementType_LayoutOnPause);
        MR::pauseOffTalkDirector();
        MR::requestMovementOn(MR::getSceneObj< NameObj >(SceneObj_SensorHitChecker));
        MR::requestMovementOn(MR::getSceneObj< NameObj >(SceneObj_CameraContext));
        MR::requestMovementOn(MR::getSceneObj< NameObj >(SceneObj_ClippingDirector));
        MR::requestMovementOn(MR::getSceneObj< NameObj >(SceneObj_LightDirector));
        WaterAreaFunction::pauseOffWaterArea();

        if (MR::isExistSceneObj(SceneObj_CollisionDirector)) {
            MR::requestMovementOn(MR::getSceneObj< CollisionDirector >(SceneObj_CollisionDirector));
        }

        CategoryList::requestMovementOn(MR::MovementType_DemoDirector);
        MR::requestMovementOn(MR::getSceneObj< NameObj >(SceneObj_CinemaFrame));
        MR::requestMovementOn(MR::getSceneObj< NameObj >(SceneObj_InformationObserver));
        MR::requestMovementOn(MR::getSceneObj< NameObj >(SceneObj_SleepControllerHolder));
        CategoryList::requestMovementOn(MR::MovementType_AudCameraWatcher);
        CategoryList::requestMovementOn(MR::MovementType_AudEffectDirector);
    }
};  // namespace

namespace {
    NEW_NERVE(StopSceneStateControlNone, StopSceneStateControl, None);
    NEW_NERVE(StopSceneStateControlStopped, StopSceneStateControl, Stopped);
};  // namespace

StopSceneStateControl::StopSceneStateControl() : NerveExecutor("StopSceneStateControl") {
    initNerve(&::StopSceneStateControlNone::sInstance);

    _8 = MR::MovementControlType_0;
    _C = nullptr;
}

void StopSceneStateControl::requestStopSceneFor(MR::MovementControlType param1, const NameObj* pParam2) {
    if (MR::isNewNerve(this)) {
        if (isNerve(&::StopSceneStateControlNone::sInstance)) {
            _8 = param1;
            _C = pParam2;

            setNerve(&::StopSceneStateControlStopped::sInstance);
            executeStopCategories(param1);
        } else if (isNerve(&::StopSceneStateControlStopped::sInstance)) {
            return;
        }
    } else if (isNerve(&::StopSceneStateControlNone::sInstance)) {
        _8 = param1;
        _C = pParam2;

        setNerve(&::StopSceneStateControlStopped::sInstance);
        executeStopCategories(param1);
    } else if (!isNerve(&::StopSceneStateControlStopped::sInstance)) {
        return;
    }
}

void StopSceneStateControl::requestStopSceneOverwrite(const NameObj* pParam1) {
    if (MR::isNewNerve(this)) {
        if (isNerve(&::StopSceneStateControlNone::sInstance)) {
            return;
        }

        if (!isNerve(&::StopSceneStateControlStopped::sInstance)) {
            return;
        }

        _C = pParam1;
        _8 = MR::MovementControlType_5;

        setNerve(&::StopSceneStateControlStopped::sInstance);
        executeStopCategories(MR::MovementControlType_5);
    } else {
        if (isNerve(&::StopSceneStateControlNone::sInstance)) {
            return;
        }

        if (!isNerve(&::StopSceneStateControlStopped::sInstance)) {
            return;
        }

        _C = pParam1;
        _8 = MR::MovementControlType_5;

        setNerve(&::StopSceneStateControlStopped::sInstance);
        executeStopCategories(MR::MovementControlType_5);
    }
}

void StopSceneStateControl::requestPlaySceneFor(MR::MovementControlType param1, const NameObj* pParam2) {
    if (MR::isNewNerve(this)) {
        if (isNerve(&::StopSceneStateControlNone::sInstance)) {
            return;
        }

        if (isNerve(&::StopSceneStateControlStopped::sInstance)) {
            return;
        }
    } else {
        if (isNerve(&::StopSceneStateControlNone::sInstance)) {
            return;
        }

        if (!isNerve(&::StopSceneStateControlStopped::sInstance)) {
            return;
        }

        _8 = param1;
        _C = pParam2;

        setNerve(&::StopSceneStateControlNone::sInstance);
        executeStopCategories(MR::MovementControlType_0);
    }
}

void StopSceneStateControl::executeStopCategories(MR::MovementControlType param1) {
    switch (param1) {
    case MR::MovementControlType_0:
        MR::resumeAllSceneNameObj();
        MR::notifyRequestNameObjMovementOnOff();
        MR::requestEffectStopSceneEnd();
        break;
    case MR::MovementControlType_1:
        ::executeStopCategoriesDemoCommon();
        MR::requestMovementOnSwitchWatcher();
        break;
    case MR::MovementControlType_2:
        ::executeStopCategoriesDemoCommon();
        MR::requestMovementOnSwitchWatcher();
        CategoryList::requestMovementOn(MR::MovementType_Player);
        CategoryList::requestMovementOn(MR::MovementType_PlayerDecoration);
        MR::requestMovementOnImageEffect();
        break;
    case MR::MovementControlType_3:
        CategoryList::requestMovementOff(MR::MovementType_Enemy);
        CategoryList::requestMovementOff(MR::MovementType_EnemyDecoration);
        CategoryList::requestMovementOff(MR::MovementType_CollisionEnemy);
        break;
    case MR::MovementControlType_4:
        CategoryList::requestMovementOff(MR::MovementType_Player);
        CategoryList::requestMovementOff(MR::MovementType_PlayerDecoration);
        break;
    case MR::MovementControlType_5:
        ::executeStopCategoriesDemoCommon();
        MR::requestMovementOn(MR::getSceneObj< CameraDirector >(SceneObj_CameraDirector));
        break;
    }
}

void StopSceneStateControl::exeNone() {}

void StopSceneStateControl::exeStopped() {}

SceneNameObjMovementController::SceneNameObjMovementController()
    : NameObj("SceneNameObjMovementController"), _C(false), mStopSceneStateControl(nullptr) {
    mStopSceneStateControl = new StopSceneStateControl();
}

void SceneNameObjMovementController::requestStopSceneFor(MR::MovementControlType param1, const NameObj* pParam2) {
    mStopSceneStateControl->requestStopSceneFor(param1, pParam2);
}

void SceneNameObjMovementController::requestStopSceneOverwrite(const NameObj* pParam1) {
    mStopSceneStateControl->requestStopSceneOverwrite(pParam1);
}

void SceneNameObjMovementController::requestPlaySceneFor(MR::MovementControlType param1, const NameObj* pParam2) {
    mStopSceneStateControl->requestPlaySceneFor(param1, pParam2);
}

void SceneNameObjMovementController::movement() {
    if (_C) {
        MR::syncWithFlagsAllSceneNameObj();

        _C = false;
    }

    mStopSceneStateControl->updateNerve();
}

void SceneNameObjMovementController::notifyRequestNameObjMovementOnOff(bool param1) {
    _C = param1;
}

namespace MR {
    SceneNameObjMovementController* getSceneNameObjMovementController() {
        return getSceneObj< SceneNameObjMovementController >(SceneObj_SceneNameObjMovementController);
    }

    void notifyRequestNameObjMovementOnOff() { getSceneNameObjMovementController()->notifyRequestNameObjMovementOnOff(true); }
};  // namespace MR
