#include "Game/Scene/StopSceneController.hpp"
#include "Game/Util.hpp"

StopSceneDelayRequest::StopSceneDelayRequest() : NameObj("シーン硬直遅延発行") {
    _C = 0;
    _10 = 0;
    MR::connectToScene(this, 1, -1, -1, -1);
}

StopSceneController::StopSceneController() : NameObj("StopSceneController") {
    mDelayRequests = nullptr;
    _10 = 0;
    
    mDelayRequests = new NameObjGroup("シーン硬直遅延発行者の管理", 16);

    for (s32 i = 0; i < 16; i++) {
        StopSceneDelayRequest* req = new StopSceneDelayRequest();
        req->initWithoutIter();
        mDelayRequests->registerObj(req);
    }
}

void StopSceneDelayRequest::movement() {
    if (_10 != 0 && --_10 == 0) {
        MR::stopScene(_C);
    }
}

#ifdef NON_MATCHING
void StopSceneController::requestStopScene(s32 a1) {
    if (_10 > 0) {
        if (_10 < a1) {
            return;
        }

        _10 = a1;
    }
    else {
        _10 = a1;
    }
}
#endif

// void StopSceneController::requestStopSceneDelay(s32 a1, s32 a2)

void StopSceneController::movement() {
    if (_10 > 0) {
        _10--;
    }
}

bool StopSceneController::isSceneStopped() const {
    return _10 > 0;
}

StopSceneDelayRequest::~StopSceneDelayRequest() {

}

StopSceneController::~StopSceneController() {

}
