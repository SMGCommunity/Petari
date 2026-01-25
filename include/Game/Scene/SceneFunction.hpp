#pragma once

#include <revolution.h>

namespace MR {
    /* thanks to https://github.com/magcius/noclip.website/blob/master/src/SuperMarioGalaxy/NameObj.ts */
    enum MovementType {
        MovementType_StopSceneDelayRequest = 0x01,
        MovementType_Camera = 0x02,
        MovementType_ScreenEffect = 0x03,
        MovementType_ClippingDirector = 0x04,
        MovementType_SensorHitChecker = 0x05,
        MovementType_MsgSharedGroup = 0x06,
        MovementType_UNK_0x07 = 0x07,
        MovementType_AudCameraWatcher = 0x09,
        MovementType_TalkDirector = 0x0A,
        MovementType_DemoDirector = 0x0B,
        MovementType_UNK_0x0C = 0x0C,
        MovementType_AreaObj = 0x0D,
        MovementType_Layout = 0x0E,
        MovementType_LayoutDecoration = 0x0F,
        MovementType_Movie = 0x10,
        MovementType_WipeLayout = 0x11,
        MovementType_LayoutOnPause = 0x12,
        MovementType_MovieSubtitles = 0x13,
        MovementType_UNK_0x14 = 0x14,
        MovementType_UNK_0x15 = 0x15,
        MovementType_MirrorCamera = 0x16,
        MovementType_ImageEffect = 0x17,
        MovementType_AudEffectDirector = 0x18,
        MovementType_AudBgmConductor = 0x19,
        MovementType_CameraCover = 0x1A,
        MovementType_SwitchWatcherHolder = 0x1B,
        MovementType_ClippedMapParts = 0x1C,
        MovementType_Planet = 0x1D,
        MovementType_CollisionMapObj = 0x1E,
        MovementType_CollisionEnemy = 0x1F,
        MovementType_CollisionDirector = 0x20,
        MovementType_Environment = 0x21,
        MovementType_MapObj = 0x22,
        MovementType_MapObjDecoration = 0x23,
        MovementType_Sky = 0x24,
        MovementType_Player = 0x25,
        MovementType_PlayerDecoration = 0x26,
        MovementType_PlayerMessenger = 0x27,
        MovementType_NPC = 0x28,
        MovementType_Ride = 0x29,
        MovementType_Enemy = 0x2A,
        MovementType_EnemyDecoration = 0x2B,
        MovementType_Item = 0x2C,
        MovementType_ShadowControllerHolder = 0x2D
    };

    enum CalcAnimType {
        CalcAnimType_ClippedMapParts = 0x00,
        CalcAnimType_Planet = 0x01,
        CalcAnimType_CollisionMapObj = 0x02,
        CalcAnimType_CollisionEnemy = 0x03,
        CalcAnimType_Environment = 0x04,
        CalcAnimType_MapObj = 0x05,
        CalcAnimType_NPC = 0x06,
        CalcAnimType_Ride = 0x07,
        CalcAnimType_Enemy = 0x08,
        CalcAnimType_Player = 0x09,
        CalcAnimType_PlayerDecoration = 0x0A,
        CalcAnimType_MapObjDecoration = 0x0B,
        CalcAnimType_MirrorMapObj = 0x0C,
        CalcAnimType_Layout = 0x0D,
        CalcAnimType_LayoutDecoration = 0x0E,
        CalcAnimType_MovieSubtitles = 0x0F,
        CalcAnimType_Item = 0x10,
        CalcAnimType_UNK_0x12 = 0x12,
        CalcAnimType_AnimParticle = 0x13,
        CalcAnimType_AnimParticleIgnorePause = 0x14
    };

    enum DrawBufferType {
        DrawBufferType_ClippedMapParts = 0x00,
        DrawBufferType_Sky = 0x01,
        DrawBufferType_Air = 0x02,
        DrawBufferType_Sun = 0x03,
        DrawBufferType_Planet = 0x04,
        DrawBufferType_PlanetLow = 0x05,
        DrawBufferType_Environment = 0x06,
        DrawBufferType_EnvironmentStrongLight = 0x07,
        DrawBufferType_MapObj = 0x08,
        DrawBufferType_MapObjWeakLight = 0x09,
        DrawBufferType_MapObjStrongLight = 0x0A,
        DrawBufferType_NoShadowedMapObj = 0x0B,
        DrawBufferType_NoShadowedMapObjStrongLight = 0x0C,
        DrawBufferType_NoSilhouettedMapObj = 0x0D,
        DrawBufferType_NoSilhouettedMapObjWeakLight = 0x0E,
        DrawBufferType_NoSilhouettedMapObjStrongLight = 0x0F,
        DrawBufferType_NPC = 0x10,
        DrawBufferType_Ride = 0x11,
        DrawBufferType_Enemy = 0x12,
        DrawBufferType_EnemyDecoration = 0x13,
        DrawBufferType_Player = 0x14,
        DrawBufferType_PlayerDecoration = 0x15,
        DrawBufferType_CrystalBox = 0x16,
        DrawBufferType_UNK_0x17 = 0x17,
        DrawBufferType_UNK_0x18 = 0x18,
        DrawBufferType_IndirectMapObj = 0x19,
        DrawBufferType_IndirectMapObjStrongLight = 0x1A,
        DrawBufferType_IndirectNpc = 0x1B,
        DrawBufferType_IndirectEnemy = 0x1C,
        DrawBufferType_IndirectPlanet = 0x1D,
        DrawBufferType_BloomModel = 0x1E,
        DrawBufferType_TripodBoss = 0x1F,
        DrawBufferType_Crystal = 0x20,
        DrawBufferType_CrystalItem = 0x21,
        DrawBufferType_GlaringLight = 0x22,
        DrawBufferType_AstroDomeSky = 0x23,
        DrawBufferType_Model3DFor2D = 0x24,
        DrawBufferType_0x25 = 0x25,
        DrawBufferType_0x26 = 0x26,
        DrawBufferType_MirrorMapObj = 0x27,
    };

    enum DrawType {
        DrawType_SwingRope = 0x00,
        DrawType_0x1 = 0x01,
        DrawType_Creeper = 0x02,
        DrawType_0x3 = 0x03,
        DrawType_PlantStalk = 0x04,
        DrawType_Plant = 0x05,
        DrawType_Trapeze = 0x06,
        DrawType_OceanBowl = 0x07,
        DrawType_OceanRing = 0x08,
        DrawType_OceanRingPipeInside = 0x09,
        DrawType_OceanRingPipeOutside = 0x0A,
        DrawType_OceanSphere = 0x0B,
        DrawType_WaterRoad = 0x0C,
        DrawType_WhirlPoolAccelerator = 0x0D,
        DrawType_ElectricRailHolder = 0x0E,
        DrawType_DrawType_BigBubble = 0x0F,
        DrawType_BigBubbleGoal = 0x10,
        DrawType_0x11 = 0x11,
        DrawType_SpinDriverPathDrawer = 0x12,
        DrawType_GCapture = 0x13,
        DrawType_ClipAreaDropLaser = 0x14,
        DrawType_SpiderThread = 0x15,
        DrawType_SpaceCocoon = 0x16,
        DrawType_0x17 = 0x17,
        DrawType_WarpPodPath = 0x18,     // TODO: DrawType_NormalMap?
        DrawType_FlexibleSphere = 0x19,  // TODO: DrawType_DynamicCollisionObj?
        DrawType_KirairaChain = 0x1A,
        DrawType_WaterPlant = 0x1B,
        DrawType_VolumeModel = 0x1C,
        DrawType_Flag = 0x1D,
        DrawType_AstroDomeSkyClear = 0x1E,
        DrawType_AstroDomeOrbit = 0x1F,
        DrawType_0x20 = 0x20,
        DrawType_OceanBowlBloomDrawer = 0x21,
        DrawType_Player = 0x22,
        DrawType_0x23 = 0x23,
        DrawType_0x24 = 0x24,
        DrawType_MiiFacePartsHolder = 0x25,
        DrawType_ShadowSurface = 0x26,
        DrawType_ShadowVolume = 0x27,
        DrawType_0x28 = 0x28,
        DrawType_AlphaShadow = 0x29,
        DrawType_ClipArea = 0x2A,
        DrawType_FallOutFieldDraw = 0x2B,
        DrawType_ClipFieldFillDraw = 0x2C,
        DrawType_CaptureScreenIndirect = 0x2D,
        DrawType_CaptureScreenCamera = 0x2E,
        DrawType_CenterScreenBlur = 0x2F,
        DrawType_0x30 = 0x30,
        DrawType_Fur = 0x31,
        DrawType_0x32 = 0x32,
        DrawType_0x33 = 0x33,
        DrawType_BloomEffectPreDraw = 0x34,
        DrawType_BloomEffectPostDraw = 0x35,
        DrawType_BloomModel = 0x36,
        DrawType_ImageEffect = 0x37,
        DrawType_DashRing = 0x38,
        DrawType_BrightSun = 0x39,
        DrawType_WaterCameraFilter = 0x3A,
        DrawType_0x3B = 0x3B,
        DrawType_Layout = 0x3C,
        DrawType_LayoutDecoration = 0x3D,
        DrawType_LayoutOnPause = 0x3E,
        DrawType_Movie = 0x3F,
        DrawType_MovieSubtitles = 0x40,
        DrawType_CometScreenFilter = 0x41,
        DrawType_WipeLayout = 0x42,
        DrawType_TalkLayout = 0x43,
        DrawType_0x44 = 0x44,
        DrawType_CinemaFrame = 0x45,
        DrawType_GalaxyNamePlate = 0x46,
        DrawType_EffectDraw3D = 0x47,
        DrawType_EffectDrawIndirect = 0x48,
        DrawType_EffectDrawAfterIndirect = 0x49,
        DrawType_EffectDraw2D = 0x4A,
        DrawType_EffectDrawFor2DModel = 0x4B,
        DrawType_EffectDrawForBloomEffect = 0x4C,
        DrawType_EffectDrawAfterImageEffect = 0x4D,
        DrawType_MessageBoardCapture = 0x4E,
        DrawType_MiiFaceIcon = 0x4F,
        DrawType_MiiFaceNew = 0x50,
        DrawType_CameraCover = 0x51,
        DrawType_0x52 = 0x52,
    };
};  // namespace MR

class Scene;

class SceneFunction {
public:
    static void startStageFileLoad();
    static void waitDoneStageFileLoad();
    static void startActorFileLoadCommon();
    static void startActorFileLoadScenario();
    static void startActorPlacement();
    static void initAfterScenarioSelected();
    static void initForNameObj();
    static void initForLiveActor();
    static void initEffectSystem(u32, u32);
    static void allocateDrawBufferActorList();
    static void createHioBasicNode(Scene*);

    static void movementStopSceneController();
    static void executeMovementList();

    static void executeDrawList2DNormal();
    static void executeDrawList2DMovie();

    static void executeDrawBufferListNormalOpaBeforeVolumeShadow();
    static void executeDrawSilhouetteAndFillShadow();
    static void executeDrawAlphaShadow();
    static void executeDrawBufferListNormalOpa();
    static void executeDrawListOpa();
    static void executeDrawBufferListNormalXlu();
    static void executeDrawListXlu();
    static void executeDrawAfterIndirect();
    static void executeDrawImageEffect();
    static void executeDrawBufferListNormalOpaBeforeSilhouette();

    static void executeCalcAnimListOnPlayingMovie();

    static void executeCalcAnimList();

    static void executeCalcViewAndEntryList();

    static void executeMovementListOnPlayingMovie();

    static void executeCalcViewAndEntryList2D();
};

class CategoryList {
public:
    static void execute(MR::MovementType);
    static void execute(MR::CalcAnimType);
    static void execute(MR::DrawType);
    static void entryDrawBuffer2D();
    static void entryDrawBuffer3D();
    static void entryDrawBufferMirror();
    static void drawOpa(MR::DrawBufferType);
    static void drawXlu(MR::DrawBufferType);
    static void requestMovementOn(MR::MovementType);
    static void requestMovementOff(MR::MovementType);
};
