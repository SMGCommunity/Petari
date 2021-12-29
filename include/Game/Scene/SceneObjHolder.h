#pragma once

#define SceneObj_ClippingDirector       0x2
#define SceneObj_DemoDirector           0x3
#define SceneObj_LightDirector          0x6
#define SceneObj_StageDataHolder        0x8
#define SceneObj_StageSwitchContainer   0xA
#define SceneObj_AreaObjContainer       0xD
#define SceneObj_MarioHolder            0x14
#define SceneObj_MirrorCamera           0x16
#define SceneObj_PlacementStateChecker  0x27
#define SceneObj_NamePosHolder          0x28
#define SceneObj_AllLiveActorGroup      0x30
#define SCENEOBJ_PLANETGRAVITYMANAGER   0x32
#define SceneObj_VolumeModelDrawInit    0x4C
#define SceneObj_NameObjExecuteHolder   0x58
#define SceneObj_PlanetMapCreator       0x74

class NameObj;

class SceneObjHolder {
public:
    SceneObjHolder();

    NameObj* create(int);
    NameObj* getObj(int) const;
    bool isExist(int) const;
    NameObj* newEachObj(int);

    NameObj* mSceneObjs[0x7B];      // _0
};

namespace MR {
    NameObj* createSceneObj(int);
    SceneObjHolder* getSceneObjHolder();
    bool isExistSceneObj(int);
};
