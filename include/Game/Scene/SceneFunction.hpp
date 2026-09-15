#pragma once

#include <revolution/types.h>

namespace MR {
    /* thanks to https://github.com/magcius/noclip.website/blob/master/src/SuperMarioGalaxy/NameObj.ts */
    enum MovementType {
        MovementType_None = -1,
        /* 0x00 */ MovementType_UNK_0x00,
        /* 0x01 */ MovementType_StopSceneDelayRequest,
        /* 0x02 */ MovementType_Camera,
        /* 0x03 */ MovementType_ScreenEffect,
        /* 0x04 */ MovementType_ClippingDirector,
        /* 0x05 */ MovementType_SensorHitChecker,
        /* 0x06 */ MovementType_MsgSharedGroup,
        /* 0x07 */ MovementType_UNK_0x07,
        /* 0x08 */ MovementType_UNK_0x08,
        /* 0x09 */ MovementType_AudCameraWatcher,
        /* 0x0A */ MovementType_TalkDirector,
        /* 0x0B */ MovementType_DemoDirector,
        /* 0x0C */ MovementType_UNK_0x0C,
        /* 0x0D */ MovementType_AreaObj,
        /* 0x0E */ MovementType_Layout,
        /* 0x0F */ MovementType_LayoutDecoration,
        /* 0x10 */ MovementType_Movie,
        /* 0x11 */ MovementType_WipeLayout,
        /* 0x12 */ MovementType_LayoutOnPause,
        /* 0x13 */ MovementType_MovieSubtitles,
        /* 0x14 */ MovementType_UNK_0x14,
        /* 0x15 */ MovementType_UNK_0x15,
        /* 0x16 */ MovementType_MirrorCamera,
        /* 0x17 */ MovementType_ImageEffect,
        /* 0x18 */ MovementType_AudEffectDirector,
        /* 0x19 */ MovementType_AudBgmConductor,
        /* 0x1A */ MovementType_CameraCover,
        /* 0x1B */ MovementType_SwitchWatcherHolder,
        /* 0x1C */ MovementType_ClippedMapParts,
        /* 0x1D */ MovementType_Planet,
        /* 0x1E */ MovementType_CollisionMapObj,
        /* 0x1F */ MovementType_CollisionEnemy,
        /* 0x20 */ MovementType_CollisionDirector,
        /* 0x21 */ MovementType_Environment,
        /* 0x22 */ MovementType_MapObj,
        /* 0x23 */ MovementType_MapObjDecoration,
        /* 0x24 */ MovementType_Sky,
        /* 0x25 */ MovementType_Player,
        /* 0x26 */ MovementType_PlayerDecoration,
        /* 0x27 */ MovementType_PlayerMessenger,
        /* 0x28 */ MovementType_NPC,
        /* 0x29 */ MovementType_Ride,
        /* 0x2A */ MovementType_Enemy,
        /* 0x2B */ MovementType_EnemyDecoration,
        /* 0x2C */ MovementType_Item,
        /* 0x2D */ MovementType_ShadowControllerHolder,
    };

    enum CalcAnimType {
        CalcAnimType_None = -1,
        /* 0x00 */ CalcAnimType_ClippedMapParts,
        /* 0x01 */ CalcAnimType_Planet,
        /* 0x02 */ CalcAnimType_CollisionMapObj,
        /* 0x03 */ CalcAnimType_CollisionEnemy,
        /* 0x04 */ CalcAnimType_Environment,
        /* 0x05 */ CalcAnimType_MapObj,
        /* 0x06 */ CalcAnimType_NPC,
        /* 0x07 */ CalcAnimType_Ride,
        /* 0x08 */ CalcAnimType_Enemy,
        /* 0x09 */ CalcAnimType_Player,
        /* 0x0A */ CalcAnimType_PlayerDecoration,
        /* 0x0B */ CalcAnimType_MapObjDecoration,
        /* 0x0C */ CalcAnimType_MirrorMapObj,
        /* 0x0D */ CalcAnimType_Layout,
        /* 0x0E */ CalcAnimType_LayoutDecoration,
        /* 0x0F */ CalcAnimType_MovieSubtitles,
        /* 0x10 */ CalcAnimType_Item,
        /* 0x11 */ CalcAnimType_UNK_0x11,
        /* 0x12 */ CalcAnimType_UNK_0x12,
        /* 0x13 */ CalcAnimType_AnimParticle,
        /* 0x14 */ CalcAnimType_AnimParticleIgnorePause,
        /* 0x15 */ CalcAnimType_UNK_0x15,
    };

    enum DrawBufferType {
        DrawBufferType_None = -1,
        /* 0x00 */ DrawBufferType_ClippedMapParts,
        /* 0x01 */ DrawBufferType_Sky,
        /* 0x02 */ DrawBufferType_Air,
        /* 0x03 */ DrawBufferType_Sun,
        /* 0x04 */ DrawBufferType_Planet,
        /* 0x05 */ DrawBufferType_PlanetLow,
        /* 0x06 */ DrawBufferType_Environment,
        /* 0x07 */ DrawBufferType_EnvironmentStrongLight,
        /* 0x08 */ DrawBufferType_MapObj,
        /* 0x09 */ DrawBufferType_MapObjWeakLight,
        /* 0x0A */ DrawBufferType_MapObjStrongLight,
        /* 0x0B */ DrawBufferType_NoShadowedMapObj,
        /* 0x0C */ DrawBufferType_NoShadowedMapObjStrongLight,
        /* 0x0D */ DrawBufferType_NoSilhouettedMapObj,
        /* 0x0E */ DrawBufferType_NoSilhouettedMapObjWeakLight,
        /* 0x0F */ DrawBufferType_NoSilhouettedMapObjStrongLight,
        /* 0x10 */ DrawBufferType_NPC,
        /* 0x11 */ DrawBufferType_Ride,
        /* 0x12 */ DrawBufferType_Enemy,
        /* 0x13 */ DrawBufferType_EnemyDecoration,
        /* 0x14 */ DrawBufferType_Player,
        /* 0x15 */ DrawBufferType_PlayerDecoration,
        /* 0x16 */ DrawBufferType_CrystalBox,
        /* 0x17 */ DrawBufferType_UNK_0x17,
        /* 0x18 */ DrawBufferType_UNK_0x18,
        /* 0x19 */ DrawBufferType_IndirectMapObj,
        /* 0x1A */ DrawBufferType_IndirectMapObjStrongLight,
        /* 0x1B */ DrawBufferType_IndirectNpc,
        /* 0x1C */ DrawBufferType_IndirectEnemy,
        /* 0x1D */ DrawBufferType_IndirectPlanet,
        /* 0x1E */ DrawBufferType_BloomModel,
        /* 0x1F */ DrawBufferType_TripodBoss,
        /* 0x20 */ DrawBufferType_Crystal,
        /* 0x21 */ DrawBufferType_CrystalItem,
        /* 0x22 */ DrawBufferType_GlaringLight,
        /* 0x23 */ DrawBufferType_AstroDomeSky,
        /* 0x24 */ DrawBufferType_Model3DFor2D,
        /* 0x25 */ DrawBufferType_0x25,
        /* 0x26 */ DrawBufferType_0x26,
        /* 0x27 */ DrawBufferType_MirrorMapObj,
        /* 0x28 */ DrawBufferType_0x28,
    };

    enum DrawType {
        DrawType_None = -1,
        /* 0x00 */ DrawType_SwingRope,
        /* 0x01 */ DrawType_0x1,
        /* 0x02 */ DrawType_Creeper,
        /* 0x03 */ DrawType_0x3,
        /* 0x04 */ DrawType_PlantStalk,
        /* 0x05 */ DrawType_Plant,
        /* 0x06 */ DrawType_Trapeze,
        /* 0x07 */ DrawType_OceanBowl,
        /* 0x08 */ DrawType_OceanRing,
        /* 0x09 */ DrawType_OceanRingPipeInside,
        /* 0x0A */ DrawType_OceanRingPipeOutside,
        /* 0x0B */ DrawType_OceanSphere,
        /* 0x0C */ DrawType_WaterRoad,
        /* 0x0D */ DrawType_WhirlPoolAccelerator,
        /* 0x0E */ DrawType_ElectricRailHolder,
        /* 0x0F */ DrawType_BigBubble,
        /* 0x10 */ DrawType_BigBubbleGoal,
        /* 0x11 */ DrawType_0x11,
        /* 0x12 */ DrawType_SpinDriverPathDrawer,
        /* 0x13 */ DrawType_GCapture,
        /* 0x14 */ DrawType_ClipAreaDropLaser,
        /* 0x15 */ DrawType_SpiderThread,
        /* 0x16 */ DrawType_SpaceCocoon,
        /* 0x17 */ DrawType_0x17,
        /* 0x18 */ DrawType_WarpPodPath,     // TODO: DrawType_NormalMap?
        /* 0x19 */ DrawType_FlexibleSphere,  // TODO: DrawType_DynamicCollisionObj?
        /* 0x1A */ DrawType_KirairaChain,
        /* 0x1B */ DrawType_WaterPlant,
        /* 0x1C */ DrawType_VolumeModel,
        /* 0x1D */ DrawType_Flag,
        /* 0x1E */ DrawType_AstroDomeSkyClear,
        /* 0x1F */ DrawType_AstroDomeOrbit,
        /* 0x20 */ DrawType_0x20,
        /* 0x21 */ DrawType_OceanBowlBloomDrawer,
        /* 0x22 */ DrawType_Player,
        /* 0x23 */ DrawType_0x23,
        /* 0x24 */ DrawType_0x24,
        /* 0x25 */ DrawType_MiiFacePartsHolder,
        /* 0x26 */ DrawType_ShadowSurface,
        /* 0x27 */ DrawType_ShadowVolume,
        /* 0x28 */ DrawType_0x28,
        /* 0x29 */ DrawType_AlphaShadow,
        /* 0x2A */ DrawType_ClipArea,
        /* 0x2B */ DrawType_FallOutFieldDraw,
        /* 0x2C */ DrawType_ClipFieldFillDraw,
        /* 0x2D */ DrawType_CaptureScreenIndirect,
        /* 0x2E */ DrawType_CaptureScreenCamera,
        /* 0x2F */ DrawType_CenterScreenBlur,
        /* 0x30 */ DrawType_0x30,
        /* 0x31 */ DrawType_Fur,
        /* 0x32 */ DrawType_0x32,
        /* 0x33 */ DrawType_0x33,
        /* 0x34 */ DrawType_BloomEffectPreDraw,
        /* 0x35 */ DrawType_BloomEffectPostDraw,
        /* 0x36 */ DrawType_BloomModel,
        /* 0x37 */ DrawType_ImageEffect,
        /* 0x38 */ DrawType_DashRing,
        /* 0x39 */ DrawType_BrightSun,
        /* 0x3A */ DrawType_WaterCameraFilter,
        /* 0x3B */ DrawType_0x3B,
        /* 0x3C */ DrawType_Layout,
        /* 0x3D */ DrawType_LayoutDecoration,
        /* 0x3E */ DrawType_LayoutOnPause,
        /* 0x3F */ DrawType_Movie,
        /* 0x40 */ DrawType_MovieSubtitles,
        /* 0x41 */ DrawType_CometScreenFilter,
        /* 0x42 */ DrawType_WipeLayout,
        /* 0x43 */ DrawType_TalkLayout,
        /* 0x44 */ DrawType_0x44,
        /* 0x45 */ DrawType_CinemaFrame,
        /* 0x46 */ DrawType_GalaxyNamePlate,
        /* 0x47 */ DrawType_EffectDraw3D,
        /* 0x48 */ DrawType_EffectDrawIndirect,
        /* 0x49 */ DrawType_EffectDrawAfterIndirect,
        /* 0x4A */ DrawType_EffectDraw2D,
        /* 0x4B */ DrawType_EffectDrawFor2DModel,
        /* 0x4C */ DrawType_EffectDrawForBloomEffect,
        /* 0x4D */ DrawType_EffectDrawAfterImageEffect,
        /* 0x4E */ DrawType_MessageBoardCapture,
        /* 0x4F */ DrawType_MiiFaceIcon,
        /* 0x50 */ DrawType_MiiFaceNew,
        /* 0x51 */ DrawType_CameraCover,
        /* 0x52 */ DrawType_0x52,
    };

    enum LightType {
        LightType_None = -1,  // TODO: better name?
        /* 0x00 */ LightType_Player,
        /* 0x01 */ LightType_Strong,
        /* 0x02 */ LightType_Weak,
        /* 0x03 */ LightType_Planet,
        /* 0x04 */ LightType_Coin,
    };

    enum CameraType {
        /* 0x00 */ CameraType_3D,
        /* 0x01 */ CameraType_2D,
        /* 0x02 */ CameraType_Mirror,
    };
};  // namespace MR

class Scene;

class SceneFunction {
public:
    static void movementStopSceneController();
    static void executeMovementList();
    static void executeMovementListOnPlayingMovie();
    static void executeCalcAnimList();
    static void executeCalcAnimListOnPlayingMovie();
    static void executeCalcViewAndEntryList();
    static void executeCalcViewAndEntryList2D();
    static void executeDrawBufferListNormalOpaBeforeVolumeShadow();
    static void executeDrawBufferListNormalOpaBeforeSilhouette();
    static void executeDrawBufferListNormalOpa();
    static void executeDrawBufferListNormalXlu();
    static void executeDrawListOpa();
    static void executeDrawListXlu();
    static void executeDrawSilhouetteAndFillShadow();
    static void executeDrawAlphaShadow();
    static void executeDrawAfterIndirect();
    static void executeDrawImageEffect();
    static void executeDrawList2DNormal();
    static void executeDrawList2DMovie();
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
