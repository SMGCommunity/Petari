#pragma once

#include "Game/NameObj/NameObjHolder.hpp"
#include "Game/Util.hpp"

class NameObjListExecutor;

class SceneControlInfo {
public:
    SceneControlInfo();

    void setScene(const char *);
    void setStage(const char *);
    void setStartIdInfo(const JMapIdInfo &);

    char mScene[0x20];  // 0x0
    char mStage[0x20];  // 0x20
    u32 _40;
    u32 _44;
    JMapIdInfo* _48;
};

class GameSystemSceneController {
public:
    NameObjListExecutor* getNameObjListExecutor() const;

    u8 _0[0xA8];
    NameObjHolder* mObjHolder;     // 0xA8
};