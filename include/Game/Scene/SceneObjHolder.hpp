#pragma once

#define SceneObj_SensorHitChecker           0x0
#define SceneObj_CollisionDirector          0x1
#define SceneObj_ClippingDirector           0x2
#define SceneObj_DemoDirector               0x3
#define SceneObj_EventDirector              0x4
#define SceneObj_EffectSystem               0x5
#define SceneObj_LightDirector              0x6
#define SceneObj_SceneDataInitializer       0x7
#define SceneObj_StageDataHolder            0x8
#define SceneObj_MessageSensorHolder        0x9
#define SceneObj_StageSwitchContainer       0xA
#define SceneObj_SwitchWatcherHolder        0xB
#define SceneObj_SleepControllerHolder      0xC
#define SceneObj_AreaObjContainer           0xD
#define SceneObj_MarioHolder                0x14
#define SceneObj_MirrorCamera               0x16
#define SceneObj_CameraContext              0x17
#define SceneObj_PlayerActionGuidance       0x23
#define SceneObj_PlacementStateChecker      0x27
#define SceneObj_NamePosHolder              0x28
#define SceneObj_AllLiveActorGroup          0x30
#define SceneObj_PlanetGravityManager       0x32
#define SceneObj_PurpleCoinHolder           0x37
#define SceneObj_CoinRotater                0x38
#define SceneObj_AirBubbleHolder            0x39
#define SceneObj_BigFanHolder               0x3A
#define SceneObj_ShadowControllerHolder     0x44
#define SceneObj_ShadowVolumeDrawer         0x45
#define SceneObj_ShadowSurfaceDrawerInit    0x46
#define SceneObj_VolumeModelDrawInit        0x4C
#define SceneObj_SpinDrawerPathDrawInit     0x4D
#define SceneObj_NoteGroup                  0x4E
#define SceneObj_ArrowSwitchMultiHolder     0x54
#define SceneObj_NameObjExecuteHolder       0x58
#define SceneObj_QuakeEffectGenerator       0x5B
#define SceneObj_HeatHazeDirector           0x5D
#define SceneObj_FirePressureBulletHolder   0x6A
#define SceneObj_GalaxyNamePlateDrawer      0x70
#define SceneObj_PlanetMapCreator           0x74
#define SceneObj_PriorDrawAirHolder         0x75

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

    template<class T>
    inline T getSceneObj(int objID) {
        return reinterpret_cast<T>(MR::getSceneObjHolder()->getObj(objID));
    }
};
