#pragma once

#include "Game/NameObj/NameObj.hpp"

class StopSceneStateController;

class SceneNameObjMovementController : public NameObj {
public:
    SceneNameObjMovementController();

    virtual ~SceneNameObjMovementController();
    virtual void movement();

    // requestStopSceneFor
    void requestStopSceneOverwrite(const NameObj *);


    u8 _C;
    StopSceneStateController* mStateController;     // 0x10
};

namespace MR {
    SceneNameObjMovementController* getSceneNameObjMovementController();
};
