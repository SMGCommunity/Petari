#include "Game/Demo/DemoDirector.hpp"
#include "Game/Demo/DemoCastGroupHolder.hpp"
#include "Game/Demo/DemoExecutor.hpp"
#include "Game/Demo/DemoFunction.hpp"
#include "Game/Demo/DemoSimpleCastHolder.hpp"
#include "Game/Demo/DemoStartRequestHolder.hpp"
#include "Game/Demo/DemoStartRequestUtil.hpp"
#include "Game/Player/MarioAccess.hpp"
#include "Game/Scene/SceneNameObjMovementController.hpp"
#include "Game/Scene/SceneObjHolder.hpp"
#include "Game/System/ResourceHolder.hpp"
#include "Game/Util/ActorSensorUtil.hpp"
#include "Game/Util/CameraUtil.hpp"
#include "Game/Util/DemoUtil.hpp"
#include "Game/Util/JMapIdInfo.hpp"
#include "Game/Util/JMapUtil.hpp"
#include "Game/Util/ObjUtil.hpp"
#include "Game/Util/ScreenUtil.hpp"
#include "Game/Util/StarPointerUtil.hpp"

DemoDirector::DemoDirector(const char* pName)
    : NameObj(pName), mIsActive(), mExecutor(), _14(), _18(), _20(new DemoSimpleCastHolder(512, 64, 128)), mResourceHolder(),
      mStartRequestHolder(new DemoStartRequestHolder()), _2C(), _30(), _34(-1), _38(true) {
    MR::connectToScene(this, 11, -1, -1, -1);

    _18 = new DemoCastGroupHolder();
    _18->initWithoutIter();

    mCastSubGroupHolder = new DemoCastGroupHolder();
    mCastSubGroupHolder->initWithoutIter();

    mResourceHolder = DemoFunction::loadDemoArchive();
}

void DemoDirector::movement() {
    if (mExecutor != nullptr && !_14) {
        mExecutor->movement();
    }

    if (tryStartDemoRequested()) {
        return;
    }

    if (!_14) {
        return;
    }

    if (!MR::isCameraInterpolatingNearlyEnd()) {
        return;
    }

    _14 = false;

    doDemoEndRequest();
}

void DemoDirector::startDemoProgrammable(NameObj* pParam1, const char* pParam2, bool param3, s32 param4) {
    startDemo(pParam1, pParam2, param3, param4);
}

void DemoDirector::startDemoTimeKeep(NameObj* pParam1, const char* pParam2, s32 param3, bool param4, const char* pParam5) {
    startDemo(pParam1, pParam2, param4, param3);
    startDemoExecutor(pParam1, pParam2, param3, pParam5);
}

void DemoDirector::startDemoExecutor(NameObj* pParam1, const char* pParam2, s32 param3, const char* pParam4) {
    mExecutor = DemoFunction::findDemoExecutor(pParam2);

    if (pParam4 != nullptr) {
        mExecutor->startPart(pParam1, pParam2, pParam4, param3);
    } else {
        mExecutor->start(pParam1, pParam2, param3);
    }
}

const char* DemoDirector::getCurrentDemoName() const {
    if (mIsActive) {
        return _30;
    }

    return nullptr;
}

void DemoDirector::endDemo(NameObj* pParam1, const char* pParam2, bool param3) {
    if (DemoStartRequestUtil::isExistStartDemoRequest(mStartRequestHolder)) {
        MR::sendMsgToAllLiveActor(ACTMES_END_DEMO, nullptr);

        mExecutor = nullptr;

        doDemoEndRequest();
        startDemoRequested();
    } else if (param3 && !MR::isCameraInterpolatingNearlyEnd() && _34 != 3) {
        _14 = true;

        MR::sendMsgToAllLiveActor(ACTMES_END_DEMO, nullptr);

        mExecutor = nullptr;

        MR::getSceneObj< SceneNameObjMovementController >(SceneObj_SceneNameObjMovementController)->requestStopSceneOverwrite(pParam1);
    } else {
        MR::sendMsgToAllLiveActor(ACTMES_END_DEMO, nullptr);

        mExecutor = nullptr;

        doDemoEndRequest();
    }
}

bool DemoDirector::isExistTimeKeepDemo(const char* pParam1) const {
    for (int i = 0; i < _18->getObjectCount(); i++) {
        if (MR::isName(_18->getCastGroup(i), pParam1)) {
            return true;
        }
    }

    return false;
}

bool DemoDirector::registerDemoCast(LiveActor* pActor, const JMapInfoIter& rIter) {
    if (!MR::isValidInfo(rIter)) {
        return false;
    }

    s32 demoGroupId = -1;

    if (!MR::getJMapInfoDemoGroupID(rIter, &demoGroupId)) {
        return false;
    }

    JMapIdInfo idInfo = JMapIdInfo(MR::getDemoGroupID(rIter), rIter);

    if (_18->tryRegisterDemoActor(pActor, rIter, idInfo)) {
        return true;
    }

    return mCastSubGroupHolder->tryRegisterDemoActor(pActor, rIter, idInfo);
}

bool DemoDirector::registerDemoCast(LiveActor* pActor, const char* pParam2, const JMapInfoIter& rIter) {
    if (_18->tryRegisterDemoActor(pActor, pParam2, rIter)) {
        return true;
    }

    return mCastSubGroupHolder->tryRegisterDemoActor(pActor, pParam2, rIter);
}

void DemoDirector::registerDemoSimpleCast(LiveActor* pActor) {
    _20->registerActor(pActor);
}

void DemoDirector::registerDemoSimpleCast(LayoutActor* pActor) {
    _20->registerActor(pActor);
}

void DemoDirector::registerDemoSimpleCast(NameObj* pObj) {
    _20->registerNameObj(pObj);
}

bool DemoDirector::tryStartDemoRequested() {
    if (!mStartRequestHolder->isExistRequest()) {
        return false;
    }

    if (!MR::canStartDemo()) {
        return false;
    }

    startDemoRequested();

    return true;
}

void DemoDirector::startDemo(NameObj* pObj, const char* pParam2, bool param3, s32 param4) {
    _2C = pObj;
    mIsActive = true;
    _30 = pParam2;
    _34 = param4;
    _38 = param3;

    _20->movementOnAllCasts();
    MR::initStarPieceGetCSSound();
}

void DemoDirector::startDemoRequested() {
    DemoStartRequestUtil::startDemo(mStartRequestHolder);

    const DemoStartInfo& info = *mStartRequestHolder->getCurrentInfo();

    startDemo(DemoStartRequestUtil::getDemoStarter(info), info.mDemoName, info._2C == nullptr, info._24);
    DemoStartRequestUtil::popStartDemoRequest(mStartRequestHolder);
}

void DemoDirector::doDemoEndRequest() {
    MR::getSceneNameObjMovementController()->requestPlaySceneFor(MR::MovementControlType(_34), _2C);
    MR::activateDefaultGameLayout();
    MR::endStarPointerMode(_2C);

    if (_38) {
        MR::tryFrameToScreenCinemaFrame();
    }

    if (_34 == MR::MovementControlType_2 || _34 == MR::MovementControlType_3) {
        MarioAccess::endRemoteDemo(nullptr);
    }

    mIsActive = false;
}
