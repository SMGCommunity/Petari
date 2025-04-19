#pragma once

#include "Inline.hpp"

#define SceneObj_SensorHitChecker 0x0
#define SceneObj_CollisionDirector 0x1
#define SceneObj_ClippingDirector 0x2
#define SceneObj_DemoDirector 0x3
#define SceneObj_EventDirector 0x4
#define SceneObj_EffectSystem 0x5
#define SceneObj_LightDirector 0x6
#define SceneObj_SceneDataInitializer 0x7
#define SceneObj_StageDataHolder 0x8
#define SceneObj_MessageSensorHolder 0x9
#define SceneObj_StageSwitchContainer 0xA
#define SceneObj_SwitchWatcherHolder 0xB
#define SceneObj_SleepControllerHolder 0xC
#define SceneObj_AreaObjContainer 0xD
#define SceneObj_LiveActorGroupArray 0xE
#define SceneObj_MovementOnOffGroupHolder 0xF
#define SceneObj_CaptureScreenActor 0x10
#define SceneObj_AudCameraWatcher 0x11
#define SceneObj_AudEffectDirector 0x12
#define SceneObj_AudBgmConductor 0x13
#define SceneObj_MarioHolder 0x14
// #define SceneObj_15 0x15
#define SceneObj_MirrorCamera 0x16
#define SceneObj_CameraContext 0x17
#define SceneObj_NameObjGroup 0x18
#define SceneObj_TalkDirector 0x19
#define SceneObj_EventSequencer 0x1A
#define SceneObj_StopSceneController 0x1B
#define SceneObj_SceneNameObjMovementController 0x1C
#define SceneObj_ImageEffectSystemHolder 0x1D
#define SceneObj_BloomEffect 0x1E
#define SceneObj_BloomEffectSimple 0x1F
#define SceneObj_ScreenBlurEffect 0x20
#define SceneObj_DepthOfFieldBlur 0x21
#define SceneObj_SceneWipeHolder 0x22
#define SceneObj_PlayerActionGuidance 0x23
#define SceneObj_ScenePlayingResult 0x24
#define SceneObj_LensFlareDirector 0x25
#define SceneObj_FurDrawManager 0x26
#define SceneObj_PlacementStateChecker 0x27
#define SceneObj_NamePosHolder 0x28
#define SceneObj_NPCDirector 0x29
#define SceneObj_ResourceShare 0x2A
#define SceneObj_MoviePlayerSimple 0x2B
#define SceneObj_WarpPodMgr 0x2C
#define SceneObj_CenterScreenBlur 0x2D
#define SceneObj_OdhConverter 0x2E
#define SceneObj_CometRetryButton 0x2F
#define SceneObj_AllLiveActorGroup 0x30
#define SceneObj_CameraDirector 0x31
#define SceneObj_PlanetGravityManager 0x32
#define SceneObj_BaseMatrixFollowTargetHolder 0x33
#define SceneObj_GameSceneLayoutHolder 0x34
// #define SceneObj_35 0x35
#define SceneObj_CoinHolder 0x36
#define SceneObj_PurpleCoinHolder 0x37
#define SceneObj_CoinRotater 0x38
#define SceneObj_AirBubbleHolder 0x39
#define SceneObj_BigFanHolder 0x3A
#define SceneObj_KarikariDirector 0x3B
#define SceneObj_StarPieceDirector 0x3C
#define SceneObj_BegomanAttackPermitter 0x3D
#define SceneObj_TripodBossAccesser 0x3E
#define SceneObj_KameckBeamHolder 0x3F
#define SceneObj_KameckFireBallHolder 0x40
#define SceneObj_KameckBeamTurtleHolder 0x41
#define SceneObj_KabokuriFireHolder 0x42
#define SceneObj_TakoHeiInkHolder 0x43
#define SceneObj_ShadowControllerHolder 0x44
#define SceneObj_ShadowVolumeDrawInit 0x45
#define SceneObj_ShadowSurfaceDrawInit 0x46
#define SceneObj_SwingRopeGroup 0x47
#define SceneObj_PlantStalkDrawInit 0x48
#define SceneObj_PlantLeafDrawInit 0x49
#define SceneObj_TrapezeRopeDrawInit 0x4A
// #define SceneObj_4B 0x4B
#define SceneObj_VolumeModelDrawInit 0x4C
#define SceneObj_SpinDriverPathDrawInit 0x4D
#define SceneObj_NoteGroup 0x4E
#define SceneObj_ClipAreaDropHolder 0x4F
#define SceneObj_FallOutFieldDraw 0x50
#define SceneObj_ClipFieldFillDraw 0x51
// #define SceneObj_52 0x52
#define SceneObj_ClipAreaHolder 0x53
#define SceneObj_ArrowSwitchMultiHolder 0x54
#define SceneObj_ScreenAlphaCapture 0x55
#define SceneObj_MapPartsRailGuideHolder 0x56
#define SceneObj_GCapture 0x57
#define SceneObj_NameObjExecuteHolder 0x58
#define SceneObj_ElectricRailHolder 0x59
#define SceneObj_SpiderThread 0x5A
#define SceneObj_QuakeEffectGenerator 0x5B
// #define SceneObj_5C 0x5C
#define SceneObj_HeatHazeDirector 0x5D
#define SceneObj_BlueChipHolder 0x5E
#define SceneObj_YellowChipHolder 0x5F
#define SceneObj_BigBubbleHolder 0x60
#define SceneObj_EarthenPipeMediator 0x61
#define SceneObj_WaterAreaHolder 0x62
#define SceneObj_WaterPlantDrawInit 0x63
#define SceneObj_OceanHomeMapCtrl 0x64
#define SceneObj_RaceManager 0x65
#define SceneObj_GroupCheckManager 0x66
#define SceneObj_SkeletalFishBabyRailHolder 0x67
#define SceneObj_SkeletalFishBossRailHolder 0x68
#define SceneObj_WaterPressureBulletHolder 0x69
#define SceneObj_FirePressureBulletHolder 0x6A
#define SceneObj_SunshadeMapHolder 0x6B
#define SceneObj_MiiFacePartsHolder 0x6C
#define SceneObj_MiiFaceIconHolder 0x6D
#define SceneObj_FluffWindHolder 0x6E
#define SceneObj_SphereSelector 0x6F
#define SceneObj_GalaxyNamePlateDrawer 0x70
#define SceneObj_CinemaFrame 0x71
#define SceneObj_BossAccessor 0x72
#define SceneObj_MiniatureGalaxyHolder 0x73
#define SceneObj_PlanetMapCreator 0x74
#define SceneObj_PriorDrawAirHolder 0x75
#define SceneObj_InformationObserver 0x76
#define SceneObj_GalaxyMapController 0x77
#define SceneObj_MoviePlayingSequenceHolder 0x78
#define SceneObj_PrologueHolder 0x79
#define SceneObj_StaffRoll 0x7A
#define SceneObj_COUNT 0x7B

class NameObj;

/// @brief The container for global scene objects.
class SceneObjHolder {
public:
    /// @brief Creates a new `SceneObjHolder`.
    SceneObjHolder();

    /// @brief Creates the requested object, returning the existing instance if it has already been created.
    /// @param id The index of the object.
    /// @return The pointer to the object.
    NameObj* create(int id);

    /// @brief Returns the requested object.
    /// @param id The index of the object.
    /// @return The pointer to the object.
    NameObj* getObj(int id) const;

    /// @brief Determines if the requested object has been created.
    /// @param id The index of the object.
    /// @return `true` if the object has been created, `false` otherwise.
    bool isExist(int id) const NO_INLINE;

    /// @brief Creates the requested object.
    /// @param id The index of the object.
    /// @return The pointer to the object.
    NameObj* newEachObj(int id);

private:
    /// @brief The array of objects.
    /* 0x00 */ NameObj* mObj[SceneObj_COUNT];
};

namespace MR {
    /// @brief Creates the requested global scene object, returning the existing instance if it has already been created.
    /// @param id The index of the object.
    /// @return The pointer to the object.
    NameObj* createSceneObj(int id);

    /// @brief Returns the container for global scene objects.
    /// @return The pointer to the container for global scene objects.
    SceneObjHolder* getSceneObjHolder() NO_INLINE;

    /// @brief Determines if the requested global scene object has been created.
    /// @param id The index of the object.
    /// @return `true` if the object has been created, `false` otherwise.
    bool isExistSceneObj(int id);

    /// @brief Returns the requested global scene object.
    /// @param id The index of the object.
    /// @return The pointer to the object.
    template <class T>
    inline T getSceneObj(int id) {
        return static_cast<T>(MR::getSceneObjHolder()->getObj(id));
    }
};
