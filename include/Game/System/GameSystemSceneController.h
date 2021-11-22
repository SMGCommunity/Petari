#pragma once

#include "Game/Util.h"

class NameObjListExecutor;

class SceneControlInfo {
public:
    SceneControlInfo();

    void setScene(const char *);
    void setStage(const char *);
    void setStartIdInfo(const JMapIdInfo &);

    char mScene[0x20];  // _0
    char mStage[0x20];  // _20
};

class GameSystemSceneController {
public:
    NameObjListExecutor* getNameObjListExecutor() const;
};