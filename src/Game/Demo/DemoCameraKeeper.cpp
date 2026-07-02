#include "Game/Demo/DemoCameraKeeper.hpp"
#include "Game/Camera/CameraTargetArg.hpp"
#include "Game/Demo/DemoCameraFunction.hpp"
#include "Game/Demo/DemoExecutor.hpp"
#include "Game/Demo/DemoFunction.hpp"
#include "Game/LiveActor/ActorCameraInfo.hpp"
#include "Game/Util/ActorCameraUtil.hpp"
#include "Game/Util/CameraUtil.hpp"
#include "Game/Util/DemoUtil.hpp"
#include "Game/Util/SceneUtil.hpp"
#include "Game/Util/StringUtil.hpp"
#include <cstdio>

DemoCameraInfo::DemoCameraInfo()
    : mPartName(), mCameraTargetName(), mCameraTargetCastID(-1), mAnimCameraName(), mAnimCameraStartFrame(-1), mAnimCameraEndFrame(-1),
      mIsContinuous(), _1C(), _20(), _24() {
}

void DemoCameraKeeper::initCast(LiveActor* pActor, const JMapInfoIter& rIter) {
    s32 j = 0;
    for (s32 i = 0, j = 0; i < _4; j++, i++) {
        DemoCameraInfo* curInfo = &_8[j];
        if (!DemoFunction::isTargetDemoCast(pActor, rIter, curInfo->mCameraTargetName, curInfo->mCameraTargetCastID)) {
            continue;
        }
        curInfo->_24 = pActor;
        initActorCamera(curInfo, rIter);
    }
}

void DemoCameraKeeper::start() {
    _C = 0;
}

// https://decomp.me/scratch/eRSVw
void DemoCameraKeeper::update() {
    if (_C < 0)
        return;
    if (_4 > _C)
        return;

    s32 j = 0;
    for (s32 i = 0, j = 0; i < _4; j++, i++) {
        DemoCameraInfo* curInfo = &_8[j];
        executeType(curInfo);
    }
}

void DemoCameraKeeper::end() {
    endCurrentCamera();
    _C = -1;
}

void DemoCameraKeeper::initActorCamera(DemoCameraInfo* pInfo, const JMapInfoIter& rIter) {
    ActorCameraInfo* cameraInfo = new ActorCameraInfo(-1, MR::getPlacedZoneId(rIter));
    pInfo->_20 = cameraInfo;
    MR::declareEventCamera(cameraInfo, pInfo->_1C);
    if (pInfo->mAnimCameraName == nullptr)
        return;

    char animCameraName[256];
    DemoCameraFunction::makeAnimCameraName(animCameraName, 256, pInfo);

    MR::initAnimCamera(pInfo->_24, pInfo->_20, animCameraName);
}

void DemoCameraKeeper::executeType(const DemoCameraInfo* pInfo) {
    if (MR::isDemoPartFirstStep(pInfo->mPartName)) {
        endCurrentCamera();
        _10 = pInfo;
        executeFirst(_10);
        return;
    }
    if (!MR::isDemoPartLastStep(pInfo->mPartName))
        return;

    executeLast(_10);
    _C++;
}

void DemoCameraKeeper::executeFirst(const DemoCameraInfo* pInfo) {
    char animCameraName[256];

    if (pInfo->_24 != nullptr) {
        if (pInfo->mAnimCameraName != nullptr) {
            MR::removeExtensionString(animCameraName, 256, pInfo->mAnimCameraName);
            MR::startAnimCameraTargetSelf(pInfo->_24, pInfo->_20, animCameraName, pInfo->mAnimCameraStartFrame, 1.0f);
        } else {
            MR::startEventCamera(pInfo->_20, pInfo->_1C, CameraTargetArg(pInfo->_24), -1);
        }
    } else if (DemoCameraFunction::isCameraTargetMario(pInfo)) {
        MR::startEventCameraTargetPlayer(pInfo->_20, pInfo->_1C, -1);
    } else {
        MR::startEventCameraNoTarget(pInfo->_20, pInfo->_1C, -1);
    }
}

void DemoCameraKeeper::executeLast(const DemoCameraInfo* pInfo) {
    char animCameraName[256];
    if (pInfo->mAnimCameraName == nullptr)
        return;

    MR::removeExtensionString(animCameraName, 256, pInfo->mAnimCameraName);
    MR::endAnimCamera(pInfo->_24, pInfo->_20, animCameraName, pInfo->mAnimCameraEndFrame, true);
}

void DemoCameraKeeper::endCurrentCamera() {
    if (_10 != nullptr) {
        MR::endEventCamera(_10->_20, _10->_1C, _10->mIsContinuous != false, -1);
    }

    _10 = nullptr;
}

// https://decomp.me/scratch/2zGrk
DemoCameraKeeper::DemoCameraKeeper(DemoExecutor* pExecutor, const JMapInfoIter& rIter)
    : mExecutor(pExecutor), _4(-1), _8(nullptr), _C(-1), _10(nullptr) {
    JMapInfo* map = nullptr;
    _4 = DemoFunction::createSheetParser(mExecutor, "Camera", &map);

    _8 = new DemoCameraInfo[_4];

    s32 executorNameLen = strlen(mExecutor->mName);

    s32 j = 0;
    for (s32 i = 0; i < _4; i++, ++j) {
        DemoCameraInfo* curInfo = &_8[j];
        map->getValue< const char* >(i, "PartName", &curInfo->mPartName);
        map->getValue< const char* >(i, "CameraTargetName", &curInfo->mCameraTargetName);
        map->getValue< s32 >(i, "CameraTargetCastID", &curInfo->mCameraTargetCastID);
        map->getValue< const char* >(i, "AnimCameraName", &curInfo->mAnimCameraName);
        map->getValue< s32 >(i, "AnimCameraStartFrame", &curInfo->mAnimCameraStartFrame);
        map->getValue< s32 >(i, "AnimCameraEndFrame", &curInfo->mAnimCameraEndFrame);
        s32 continious = -1;
        map->getValue< s32 >(i, "IsContinuous", &continious);

        curInfo->mIsContinuous = (continious == true);
        DemoCameraFunction::setStringNullIfEmpty(&curInfo->mCameraTargetName);
        DemoCameraFunction::setStringNullIfEmpty(&curInfo->mAnimCameraName);

        s32 animNameLen = executorNameLen + strlen(curInfo->mPartName) + 3;

        char* aninName = new char[animNameLen];

        snprintf(aninName, animNameLen, "%s[%s]", mExecutor->mName, curInfo->mPartName);

        curInfo->_1C = aninName;
        if (curInfo->mCameraTargetName == nullptr || DemoCameraFunction::isCameraTargetMario(curInfo)) {
            initActorCamera(curInfo, rIter);
        }
    }
}
