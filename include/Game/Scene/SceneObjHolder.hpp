#pragma once

class NameObj;

enum SceneObj {
    SceneObj_SensorHitChecker = 0x00,
    SceneObj_CollisionDirector = 0x01,
    SceneObj_ClippingDirector = 0x02,
    SceneObj_DemoDirector = 0x03,
    SceneObj_EventDirector = 0x04,
    SceneObj_EffectSystem = 0x05,
    SceneObj_LightDirector = 0x06,
    SceneObj_SceneDataInitializer = 0x07,
    SceneObj_StageDataHolder = 0x08,
    SceneObj_MessageSensorHolder = 0x09,
    SceneObj_StageSwitchContainer = 0x0A,
    SceneObj_SwitchWatcherHolder = 0x0B,
    SceneObj_SleepControllerHolder = 0x0C,
    SceneObj_AreaObjContainer = 0x0D,
    SceneObj_LiveActorGroupArray = 0x0E,
    SceneObj_MovementOnOffGroupHolder = 0x0F,
    SceneObj_CaptureScreenActor = 0x10,
    SceneObj_AudCameraWatcher = 0x11,
    SceneObj_AudEffectDirector = 0x12,
    SceneObj_AudBgmConductor = 0x13,
    SceneObj_MarioHolder = 0x14,
    SceneObj_15 = 0x15,
    SceneObj_MirrorCamera = 0x16,
    SceneObj_CameraContext = 0x17,
    SceneObj_NameObjGroup = 0x18,
    SceneObj_TalkDirector = 0x19,
    SceneObj_EventSequencer = 0x1A,
    SceneObj_StopSceneController = 0x1B,
    SceneObj_SceneNameObjMovementController = 0x1C,
    SceneObj_ImageEffectSystemHolder = 0x1D,
    SceneObj_BloomEffect = 0x1E,
    SceneObj_BloomEffectSimple = 0x1F,
    SceneObj_ScreenBlurEffect = 0x20,
    SceneObj_DepthOfFieldBlur = 0x21,
    SceneObj_SceneWipeHolder = 0x22,
    SceneObj_PlayerActionGuidance = 0x23,
    SceneObj_ScenePlayingResult = 0x24,
    SceneObj_LensFlareDirector = 0x25,
    SceneObj_FurDrawManager = 0x26,
    SceneObj_PlacementStateChecker = 0x27,
    SceneObj_NamePosHolder = 0x28,
    SceneObj_NPCDirector = 0x29,
    SceneObj_ResourceShare = 0x2A,
    SceneObj_MoviePlayerSimple = 0x2B,
    SceneObj_WarpPodMgr = 0x2C,
    SceneObj_CenterScreenBlur = 0x2D,
    SceneObj_OdhConverter = 0x2E,
    SceneObj_CometRetryButton = 0x2F,
    SceneObj_AllLiveActorGroup = 0x30,
    SceneObj_CameraDirector = 0x31,
    SceneObj_PlanetGravityManager = 0x32,
    SceneObj_BaseMatrixFollowTargetHolder = 0x33,
    SceneObj_GameSceneLayoutHolder = 0x34,
    SceneObj_35 = 0x35,
    SceneObj_CoinHolder = 0x36,
    SceneObj_PurpleCoinHolder = 0x37,
    SceneObj_CoinRotater = 0x38,
    SceneObj_AirBubbleHolder = 0x39,
    SceneObj_BigFanHolder = 0x3A,
    SceneObj_KarikariDirector = 0x3B,
    SceneObj_StarPieceDirector = 0x3C,
    SceneObj_BegomanAttackPermitter = 0x3D,
    SceneObj_TripodBossAccesser = 0x3E,
    SceneObj_KameckBeamHolder = 0x3F,
    SceneObj_KameckFireBallHolder = 0x40,
    SceneObj_KameckBeamTurtleHolder = 0x41,
    SceneObj_KabokuriFireHolder = 0x42,
    SceneObj_TakoHeiInkHolder = 0x43,
    SceneObj_ShadowControllerHolder = 0x44,
    SceneObj_ShadowVolumeDrawInit = 0x45,
    SceneObj_ShadowSurfaceDrawInit = 0x46,
    SceneObj_SwingRopeGroup = 0x47,
    SceneObj_PlantStalkDrawInit = 0x48,
    SceneObj_PlantLeafDrawInit = 0x49,
    SceneObj_TrapezeRopeDrawInit = 0x4A,
    SceneObj_4B = 0x4B,
    SceneObj_VolumeModelDrawInit = 0x4C,
    SceneObj_SpinDriverPathDrawInit = 0x4D,
    SceneObj_NoteGroup = 0x4E,
    SceneObj_ClipAreaDropHolder = 0x4F,
    SceneObj_FallOutFieldDraw = 0x50,
    SceneObj_ClipFieldFillDraw = 0x51,
    SceneObj_52 = 0x52,
    SceneObj_ClipAreaHolder = 0x53,
    SceneObj_ArrowSwitchMultiHolder = 0x54,
    SceneObj_ScreenAlphaCapture = 0x55,
    SceneObj_MapPartsRailGuideHolder = 0x56,
    SceneObj_GCapture = 0x57,
    SceneObj_NameObjExecuteHolder = 0x58,
    SceneObj_ElectricRailHolder = 0x59,
    SceneObj_SpiderThread = 0x5A,
    SceneObj_QuakeEffectGenerator = 0x5B,
    SceneObj_5C = 0x5C,
    SceneObj_HeatHazeDirector = 0x5D,
    SceneObj_BlueChipHolder = 0x5E,
    SceneObj_YellowChipHolder = 0x5F,
    SceneObj_BigBubbleHolder = 0x60,
    SceneObj_EarthenPipeMediator = 0x61,
    SceneObj_WaterAreaHolder = 0x62,
    SceneObj_WaterPlantDrawInit = 0x63,
    SceneObj_OceanHomeMapCtrl = 0x64,
    SceneObj_RaceManager = 0x65,
    SceneObj_GroupCheckManager = 0x66,
    SceneObj_SkeletalFishBabyRailHolder = 0x67,
    SceneObj_SkeletalFishBossRailHolder = 0x68,
    SceneObj_WaterPressureBulletHolder = 0x69,
    SceneObj_FirePressureBulletHolder = 0x6A,
    SceneObj_SunshadeMapHolder = 0x6B,
    SceneObj_MiiFacePartsHolder = 0x6C,
    SceneObj_MiiFaceIconHolder = 0x6D,
    SceneObj_FluffWindHolder = 0x6E,
    SceneObj_SphereSelector = 0x6F,
    SceneObj_GalaxyNamePlateDrawer = 0x70,
    SceneObj_CinemaFrame = 0x71,
    SceneObj_BossAccessor = 0x72,
    SceneObj_MiniatureGalaxyHolder = 0x73,
    SceneObj_PlanetMapCreator = 0x74,
    SceneObj_PriorDrawAirHolder = 0x75,
    SceneObj_InformationObserver = 0x76,
    SceneObj_GalaxyMapController = 0x77,
    SceneObj_MoviePlayingSequenceHolder = 0x78,
    SceneObj_PrologueHolder = 0x79,
    SceneObj_StaffRoll = 0x7A,

    SceneObj_NumMax,
};

/// @brief The container for global scene objects.
class SceneObjHolder {
public:
    /// @brief Creates a new `SceneObjHolder`.
    SceneObjHolder();

    /// @brief Creates the requested object, returning the existing instance if it has already been created.
    /// @param id The index of the object.
    /// @return A pointer to the object.
    NameObj* create(int id);

    /// @brief Returns the requested object.
    /// @param id The index of the object.
    /// @return A pointer to the object.
    NameObj* getObj(int id) const;

    /// @brief Determines if the requested object has been created.
    /// @param id The index of the object.
    /// @return `true` if the object has been created, `false` otherwise.
    bool isExist(int id) const NO_INLINE;

    /// @brief Creates the requested object.
    /// @param id The index of the object.
    /// @return A pointer to the object.
    NameObj* newEachObj(int id);

private:
    /// @brief The array of objects.
    /* 0x00 */ NameObj* mObj[SceneObj_NumMax];
};

namespace MR {
    /// @brief Creates the requested global scene object, returning the existing instance if it has already been created.
    /// @param id The index of the object.
    /// @return A pointer to the object.
    NameObj* createSceneObj(int id);

    /// @brief Returns the container for global scene objects.
    /// @return A pointer to the container for global scene objects.
    SceneObjHolder* getSceneObjHolder() NO_INLINE;

    /// @brief Determines if the requested global scene object has been created.
    /// @param id The index of the object.
    /// @return `true` if the object has been created, `false` otherwise.
    bool isExistSceneObj(int id);

    /// @brief Returns the requested global scene object.
    /// @param id The index of the object.
    /// @return A pointer to the object.
    template < class T >
    T* getSceneObj(int id) {
        return static_cast< T* >(getSceneObjHolder()->getObj(id));
    }
};  // namespace MR
