#pragma once

#define SceneObj_DemoDirector           0x3
#define SceneObj_LightDirector          0x6
#define SceneObj_StageDataHolder        0x8
#define SceneObj_AreaObjContainer       0xD
#define SceneObj_MarioHolder            0x14
#define SceneObj_PlacementStateChecker  0x27
#define SceneObj_AllLiveActorGroup      0x30
#define SceneObj_NameObjExecuteHolder   0x58
#define SceneObj_PlanetMapCreator       0x74

class NameObj;

class SceneObjHolder {
public:
    NameObj* getObj(int) const;
};

namespace MR {
    SceneObjHolder* getSceneObjHolder();

    bool isExistSceneObj(int);
};
