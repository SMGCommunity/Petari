#include "Game/Demo/DemoPositionController.hpp"
#include "Game/LiveActor/LiveActor.hpp"
#include "Game/Util/ActorCameraUtil.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/LiveActor/ActorCameraInfo.hpp"

DemoPositionController::DemoPositionController(const char *pName, const JMapInfoIter &rIter) : LiveActor("OSS_09_B"), pCameraInfo(nullptr) {
    MR::initDefaultPos(this, rIter);
    initModelManagerWithAnm(pName, nullptr, false);
    pCameraInfo = new ActorCameraInfo(rIter);
    MR::invalidateClipping(this);
    makeActorDead();
}

void DemoPositionController::control() {
    calcAnim();
}

void DemoPositionController::initAnimCamera(const char *pCameraName) {
    MR::initAnimCamera(this, this->pCameraInfo, pCameraName);
}

void DemoPositionController::startDemo(const char *pBckName) {
    appear();
    MR::startBck(this, pBckName, nullptr);
    MR::startAnimCameraTargetSelf(this, pCameraInfo, pBckName, 0, 1.0f);
}

void DemoPositionController::endDemo(const char *pCameraName) {
    MR::endAnimCamera(this, pCameraInfo, pCameraName, -1, true);
    kill();
}

DemoPositionController::~DemoPositionController() {}
