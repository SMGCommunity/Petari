#include "Game/NameObj/NameObjFactory.hpp"
#include "Game/NameObj/NameObjFactoryStubs.hpp"
#include "Game/AreaObj.hpp"
#include "Game/Boss.hpp"
#include "Game/Demo.hpp"
#include "Game/Effect.hpp"
#include "Game/Enemy.hpp"
#include "Game/Gravity.hpp"
#include "Game/Map.hpp"
#include "Game/Map/OceanBowl.hpp"
#include "Game/MapObj.hpp"
#include "Game/NPC.hpp"
#include "Game/Ride.hpp"
#include "Game/Player/MarioActor.hpp"

namespace NrvMorphItemObjNeo {
    NEW_NERVE(MorphItemObjNeoNrvWait, MorphItemObjNeo, Wait);
    NEW_NERVE(MorphItemObjNeoNrvAppear, MorphItemObjNeo, Appear);
    NEW_NERVE(MorphItemObjNeoNrvSwitchAppear, MorphItemObjNeo, SwitchAppear);
    NEW_NERVE(MorphItemObjNeoNrvWait2, MorphItemObjNeo, Wait2);
    NEW_NERVE(MorphItemObjNeoNrvFly, MorphItemObjNeo, Fly);
    NEW_NERVE(MorphItemObjNeoNrvDemo, MorphItemObjNeo, Demo);
}

namespace NrvBenefitItemObj {
    INIT_NERVE(HostTypeNrvWait);
    INIT_NERVE(HostTypeNrvShoot);
    INIT_NERVE(HostTypeNrvCatch);
    INIT_NERVE(HostTypeNrvAppearGround);
    INIT_NERVE(HostTypeNrvPreEscape);
    INIT_NERVE(HostTypeNrvEscape);
}

namespace NrvMarioActor {
    INIT_NERVE(MarioActorNrvWait);
    INIT_NERVE(MarioActorNrvGameOver);
    INIT_NERVE(MarioActorNrvGameOverAbyss);
    INIT_NERVE(MarioActorNrvGameOverAbyss2);
    INIT_NERVE(MarioActorNrvGameOverFire);
    INIT_NERVE(MarioActorNrvGameOverBlackHole);
    INIT_NERVE(MarioActorNrvGameOverNonStop);
    INIT_NERVE(MarioActorNrvGameOverSink);
    INIT_NERVE(MarioActorNrvTimeWait);
    INIT_NERVE(MarioActorNrvNoRush);
}

volatile const Nerve* cNerveInitKeep[] = {
    &NrvBenefitItemObj::HostTypeNrvWait::sInstance,
    &NrvBenefitItemObj::HostTypeNrvShoot::sInstance,
    &NrvBenefitItemObj::HostTypeNrvCatch::sInstance,
    &NrvBenefitItemObj::HostTypeNrvAppearGround::sInstance,
    &NrvBenefitItemObj::HostTypeNrvPreEscape::sInstance,
    &NrvBenefitItemObj::HostTypeNrvEscape::sInstance,
    &NrvMarioActor::MarioActorNrvWait::sInstance,
    &NrvMarioActor::MarioActorNrvGameOver::sInstance,
    &NrvMarioActor::MarioActorNrvGameOverAbyss::sInstance,
    &NrvMarioActor::MarioActorNrvGameOverAbyss2::sInstance,
    &NrvMarioActor::MarioActorNrvGameOverFire::sInstance,
    &NrvMarioActor::MarioActorNrvGameOverBlackHole::sInstance,
    &NrvMarioActor::MarioActorNrvGameOverNonStop::sInstance,
    &NrvMarioActor::MarioActorNrvGameOverSink::sInstance,
    &NrvMarioActor::MarioActorNrvTimeWait::sInstance,
    &NrvMarioActor::MarioActorNrvNoRush::sInstance,
    &NrvMorphItemObjNeo::MorphItemObjNeoNrvWait::sInstance,
    &NrvMorphItemObjNeo::MorphItemObjNeoNrvAppear::sInstance,
    &NrvMorphItemObjNeo::MorphItemObjNeoNrvSwitchAppear::sInstance,
    &NrvMorphItemObjNeo::MorphItemObjNeoNrvWait2::sInstance,
    &NrvMorphItemObjNeo::MorphItemObjNeoNrvFly::sInstance,
    &NrvMorphItemObjNeo::MorphItemObjNeoNrvDemo::sInstance,
};


namespace MR {
    NameObj* createBigBubbleMoveLimitterCylinder(const char*);
    NameObj* createBigBubbleMoveLimitterPlane(const char*);
    NameObj* createBigBubbleGenerator(const char*);
    NameObj* createBigObstructBubbleGenerator(const char*);
    NameObj* createDinoPackunVs1(const char*);
    NameObj* createDinoPackunVs2(const char*);
    NameObj* createFallOutFieldDraw(const char*);
    NameObj* createClipFieldFillDraw(const char*);
    NameObj* createVolumeDrawSpotLight(const char*);
    NameObj* createFireBallBeamKameck(const char*);
    NameObj* createTurtleBeamKameck(const char*);
};

namespace KoopaFunction {
    NameObj* createKoopaVs1(const char*);
    NameObj* createKoopaVs2(const char*);
    NameObj* createKoopaVs3(const char*);
};

namespace {
    // FIXME: Polta is being worked on in #1867
    NameObj* createPoltaStub(const char* pName) {
        return new NameObj(pName);
    }

    template < typename T >
    NameObj* createCenterOriginCube(const char* pName) {
        return new T(0, pName);
    }

    template < typename T >
    NameObj* createBaseOriginCube(const char* pName) {
        return new T(1, pName);
    }

    template < typename T >
    NameObj* createSphere(const char* pName) {
        return new T(2, pName);
    }

    template < typename T >
    NameObj* createBaseOriginCylinder(const char* pName) {
        return new T(3, pName);
    }

    template < typename T >
    NameObj* createBowl(const char* pName) {
        return new T(4, pName);
    }

    const char* cPlayerArchiveLoaderObjTable[] = {
        "Hopper",          "BenefitItemInvincible", "MorphItemNeoBee", "MorphItemNeoFire",
        "MorphItemNeoFoo", "MorphItemNeoHopper",    "MorphItemNeoIce", "MorphItemNeoTeresa",
    };

    const NameObjFactory::Name2CreateFunc cCreateTable[] = {
        {
            "SwitchCube",
            createBaseOriginCube< SwitchArea >,
            nullptr,
        },
        {
            "SwitchSphere",
            createSphere< SwitchArea >,
            nullptr,
        },
        {
            "SwitchCylinder",
            createBaseOriginCylinder< SwitchArea >,
            nullptr,
        },
        {
            "CubeCameraBox",
            createCenterOriginCube< CubeCameraArea >,
            nullptr,
        },
        {
            "CubeCameraCylinder",
            createBaseOriginCylinder< CubeCameraArea >,
            nullptr,
        },
        {
            "CubeCameraSphere",
            createSphere< CubeCameraArea >,
            nullptr,
        },
        {
            "CubeCameraBowl",
            createBowl< CubeCameraArea >,
            nullptr,
        },
        {
            "EffectCylinder",
            createBaseOriginCylinder< AreaObj >,
            nullptr,
        },
        {
            "BindEndCube",
            createCenterOriginCube< AreaObj >,
            nullptr,
        },
        {
            "DeathCube",
            createBaseOriginCube< DeathArea >,
            nullptr,
        },
        {
            "DeathSphere",
            createSphere< DeathArea >,
            nullptr,
        },
        {
            "DeathCylinder",
            createBaseOriginCylinder< DeathArea >,
            nullptr,
        },
        {
            "PullBackCube",
            createBaseOriginCube< AreaObj >,
            nullptr,
        },
        {
            "PullBackCylinder",
            createBaseOriginCylinder< AreaObj >,
            nullptr,
        },
        {
            "RestartCube",
            createBaseOriginCube< RestartCube >,
            nullptr,
        },
        {
            "WarpCube",
            createBaseOriginCube< WarpCube >,
            nullptr,
        },
        {
            "TripodBossStepStartArea",
            createSphere< TripodBossStepStartArea >,
            nullptr,
        },
        {
            "ChangeBgmCube",
            createBaseOriginCube< ChangeBgmCube >,
            nullptr,
        },
        {
            "BgmProhibitArea",
            createSphere< BgmProhibitArea >,
            nullptr,
        },
        {
            "SoundEmitterCube",
            createBaseOriginCube< SoundEmitterCube >,
            nullptr,
        },
        {
            "SoundEmitterSphere",
            createSphere< SoundEmitterSphere >,
            nullptr,
        },
        {
            "WaterCube",
            createBaseOriginCube< WaterArea >,
            nullptr,
        },
        {
            "WaterSphere",
            createSphere< WaterArea >,
            nullptr,
        },
        {
            "WaterCylinder",
            createBaseOriginCylinder< WaterArea >,
            nullptr,
        },
        {
            "FallsCube",
            createBaseOriginCube< AreaObj >,
            nullptr,
        },
        {
            "PlaneModeCube",
            createBaseOriginCube< AreaObj >,
            nullptr,
        },
        {
            "PlaneCircularModeCube",
            createBaseOriginCube< AreaObj >,
            nullptr,
        },
        {
            "PipeModeCube",
            createBaseOriginCube< AreaObj >,
            nullptr,
        },
        {
            "TowerModeCylinder",
            createBaseOriginCylinder< AreaObj >,
            nullptr,
        },
        {
            "BloomCube",
            createCenterOriginCube< BloomArea >,
            nullptr,
        },
        {
            "BloomSphere",
            createSphere< BloomArea >,
            nullptr,
        },
        {
            "BloomCylinder",
            createBaseOriginCylinder< BloomArea >,
            nullptr,
        },
        {
            "SimpleBloomCube",
            createCenterOriginCube< SimpleBloomArea >,
            nullptr,
        },
        {
            "SimpleBloomSphere",
            createSphere< SimpleBloomArea >,
            nullptr,
        },
        {
            "SimpleBloomCylinder",
            createBaseOriginCylinder< SimpleBloomArea >,
            nullptr,
        },
        {
            "ScreenBlurCube",
            createCenterOriginCube< ScreenBlurArea >,
            nullptr,
        },
        {
            "ScreenBlurSphere",
            createSphere< ScreenBlurArea >,
            nullptr,
        },
        {
            "ScreenBlurCylinder",
            createBaseOriginCylinder< ScreenBlurArea >,
            nullptr,
        },
        {
            "DepthOfFieldCube",
            createCenterOriginCube< DepthOfFieldArea >,
            nullptr,
        },
        {
            "DepthOfFieldSphere",
            createSphere< DepthOfFieldArea >,
            nullptr,
        },
        {
            "DepthOfFieldCylinder",
            createBaseOriginCylinder< DepthOfFieldArea >,
            nullptr,
        },
        {
            "ClipAreaCenterBox",
            MR::createClipAreaCenterBox,
            "ClipVolumeBox",
        },
        {
            "ClipAreaBottomBox",
            MR::createClipAreaBottomBox,
            "ClipVolumeBox",
        },
        {
            "ClipAreaTopCone",
            MR::createClipAreaTopCone,
            "ClipVolumeSphere",
        },
        {
            "ClipAreaBottomCone",
            MR::createClipAreaBottomCone,
            "ClipVolumeSphere",
        },
        {
            "ClipAreaSphere",
            MR::createClipAreaSphere,
            "ClipVolumeSphere",
        },
        {
            "LightCtrlCube",
            createBaseOriginCube< LightArea >,
            nullptr,
        },
        {
            "LightCtrlCylinder",
            createBaseOriginCylinder< LightArea >,
            nullptr,
        },
        {
            "PlaneCollisionCube",
            createBaseOriginCube< AreaObj >,
            nullptr,
        },
        {
            "ForbidTriangleJumpCube",
            createBaseOriginCube< AreaObj >,
            nullptr,
        },
        {
            "ForbidWaterSearchCube",
            createBaseOriginCube< AreaObj >,
            nullptr,
        },
        {
            "QuakeEffectAreaCube",
            createBaseOriginCube< QuakeEffectArea >,
            nullptr,
        },
        {
            "HazeCube",
            createBaseOriginCube< HazeCube >,
            nullptr,
        },
        {
            "LensFlareArea",
            createBaseOriginCube< AreaObj >,
            nullptr,
        },
        {
            "CameraRepulsiveSphere",
            createNameObj< CameraRepulsiveSphere >,
            nullptr,
        },
        {
            "CameraRepulsiveCylinder",
            createNameObj< CameraRepulsiveCylinder >,
            nullptr,
        },
        {
            "SunLightAreaBox",
            createBaseOriginCube< SunLightArea >,
            nullptr,
        },
        {
            "GlaringLightAreaCylinder",
            createBaseOriginCylinder< GlaringLightArea >,
            nullptr,
        },
        {
            "BigBubbleGoalAreaBox",
            createBaseOriginCube< BigBubbleGoalArea >,
            nullptr,
        },
        {
            "BigBubbleGoalAreaCylinder",
            createBaseOriginCylinder< BigBubbleGoalArea >,
            nullptr,
        },
        {
            "BigBubbleGoalAreaSphere",
            createSphere< BigBubbleGoalArea >,
            nullptr,
        },
        {
            "BigBubbleCameraBox",
            createCenterOriginCube< BigBubbleCameraArea >,
            nullptr,
        },
        {
            "BigBubbleCameraCylinder",
            createBaseOriginCylinder< BigBubbleCameraArea >,
            nullptr,
        },
        {
            "BigBubbleCameraSphere",
            createSphere< BigBubbleCameraArea >,
            nullptr,
        },
        {
            "BigBubbleSwitchBox",
            createBaseOriginCube< BigBubbleSwitchArea >,
            nullptr,
        },
        {
            "BigBubbleSwitchCylinder",
            createBaseOriginCylinder< BigBubbleSwitchArea >,
            nullptr,
        },
        {
            "BigBubbleSwitchSphere",
            createSphere< BigBubbleSwitchArea >,
            nullptr,
        },
        {
            "ViewGroupCtrlCube",
            createBaseOriginCube< AreaObj >,
            nullptr,
        },
        {
            "AudioEffectCube",
            createBaseOriginCube< AudioEffectArea >,
            nullptr,
        },
        {
            "AudioEffectSphere",
            createSphere< AudioEffectArea >,
            nullptr,
        },
        {
            "AudioEffectCylinder",
            createBaseOriginCylinder< AudioEffectArea >,
            nullptr,
        },
        {
            "PlayerSeCube",
            createBaseOriginCube< PlayerSeArea >,
            nullptr,
        },
        {
            "PlayerSeSphere",
            createSphere< PlayerSeArea >,
            nullptr,
        },
        {
            "PlayerSeCylinder",
            createBaseOriginCylinder< PlayerSeArea >,
            nullptr,
        },
        {
            "MercatorCube",
            createBaseOriginCube< MercatorTransformCube >,
            nullptr,
        },
        {
            "AstroChangeStageCube",
            createBaseOriginCube< AstroChangeStageCube >,
            nullptr,
        },
        {
            "BlueStarGuidanceCube",
            createBaseOriginCube< AreaObj >,
            nullptr,
        },
        {
            "TicoSeedGuidanceCube",
            createBaseOriginCube< AreaObj >,
            nullptr,
        },
        {
            "MessageAreaCube",
            createBaseOriginCube< MessageArea >,
            nullptr,
        },
        {
            "MessageAreaCylinder",
            createBaseOriginCylinder< MessageArea >,
            nullptr,
        },
        {
            "SmokeEffectColorAreaCube",
            createBaseOriginCube< AreaObj >,
            nullptr,
        },
        {
            "BeeWallShortDistAreaCube",
            createBaseOriginCube< AreaObj >,
            nullptr,
        },
        {
            "ExtraWallCheckArea",
            createBaseOriginCube< AreaObj >,
            nullptr,
        },
        {
            "ExtraWallCheckCylinder",
            createBaseOriginCylinder< AreaObj >,
            nullptr,
        },
        {
            "MirrorAreaCube",
            createBaseOriginCube< AreaObj >,
            nullptr,
        },
        {
            "SpinGuidanceCube",
            createBaseOriginCube< SpinGuidanceArea >,
            "SpinGuidance",
        },
        {
            "TamakoroMoveGuidanceCube",
            createBaseOriginCube< SpinGuidanceArea >,
            "BallGuidance",
        },
        {
            "TamakoroJumpGuidanceCube",
            createBaseOriginCube< SpinGuidanceArea >,
            "BallGuidance",
        },
        {
            "HeavySteeringCube",
            createBaseOriginCube< AreaObj >,
            nullptr,
        },
        {
            "NonSleepCube",
            createBaseOriginCube< AreaObj >,
            nullptr,
        },
        {
            "AreaMoveSphere",
            createSphere< AreaObj >,
            nullptr,
        },
        {
            "DodoryuClosedCylinder",
            createBaseOriginCylinder< AreaObj >,
            nullptr,
        },
        {
            "DashChargeCylinder",
            createBaseOriginCylinder< AreaObj >,
            nullptr,
        },
        {
            "RasterScrollCube",
            createBaseOriginCube< AreaObj >,
            nullptr,
        },
        {
            "OnimasuCube",
            createCenterOriginCube< AreaObj >,
            nullptr,
        },
        {
            "ForbidJumpCube",
            createBaseOriginCube< AreaObj >,
            nullptr,
        },
        {
            "CollisionArea",
            createCenterOriginCube< CollisionArea >,
            nullptr,
        },
        {
            "AstroOverlookAreaCylinder",
            createBaseOriginCylinder< AreaObj >,
            nullptr,
        },
        {
            "CelestrialSphere",
            createSphere< AreaObj >,
            nullptr,
        },
        {
            "DarkMatterCube",
            createBaseOriginCube< AreaObj >,
            nullptr,
        },
        {
            "DarkMatterCylinder",
            createBaseOriginCylinder< AreaObj >,
            nullptr,
        },
        {
            "Mario",
            createNameObj< MarioActor >,
            nullptr,
        },
        {
            "MarioActor",
            createNameObj< MarioActor >,
            nullptr,
        },
        {
            "Caretaker",
            createNameObj< Caretaker >,
            "Caretaker",
        },
        {
            "Rabbit",
            createNameObj< Rabbit >,
            "MoonRabbit",
        },
        {
            "TalkSyati",
            createNameObj< Syati >,
            "Syati",
        },
        {
            "CollectTico",
            createNameObj< CollectTico >,
            "StrayTico",
        },
        {
            "TrickRabbit",
            createNameObj< TrickRabbit >,
            "TrickRabbit",
        },
        {
            "TrickRabbitFreeRun",
            createNameObj< TrickRabbitFreeRun >,
            "TrickRabbit",
        },
        {
            "TrickRabbitFreeRunCollect",
            createNameObj< TrickRabbitSnowCollect >,
            "TrickRabbit",
        },
        {
            "RunawayRabbitCollect",
            createNameObj< RunawayRabbitCollect >,
            "TrickRabbit",
        },
        {
            "TrickRabbitGhost",
            createNameObj< TrickRabbit >,
            "TrickRabbit",
        },
        {
            "Kinopio",
            createNameObj< Kinopio >,
            "Kinopio",
        },
        {
            "KinopioAstro",
            createNameObj< KinopioAstro >,
            "Kinopio",
        },
        {
            "HoneyBee",
            createNameObj< HoneyBee >,
            "HoneyBee",
        },
        {
            "HoneyQueen",
            createNameObj< HoneyQueen >,
            "HoneyQueen",
        },
        {
            "Peach",
            createNameObj< Peach >,
            "Peach",
        },
        {
            "Penguin",
            createNameObj< Penguin >,
            "Penguin",
        },
        {
            "PenguinCoach",
            createNameObj< PenguinCoach >,
            "PenguinCoach",
        },
        {
            "PenguinMaster",
            createNameObj< PenguinMaster >,
            "PenguinMaster",
        },
        {
            "PenguinRacer",
            createNameObj< PenguinRacer >,
            "Penguin",
        },
        {
            "PenguinRacerLeader",
            createNameObj< PenguinRacerLeader >,
            "Penguin",
        },
        {
            "PenguinSkater",
            createNameObj< PenguinSkater >,
            "Penguin",
        },
        {
            "PenguinStudent",
            createNameObj< PenguinStudent >,
            "Penguin",
        },
        {
            "SignBoard",
            createNameObj< SignBoard >,
            "SignBoard",
        },
        {
            "Syati",
            createNameObj< Syati >,
            "Syati",
        },
        {
            "Tico",
            createNameObj< Tico >,
            nullptr,
        },
        {
            "TicoComet",
            createNameObj< TicoComet >,
            "TicoComet",
        },
        {
            "TicoShop",
            createNameObj< TicoShop >,
            "TicoShop",
        },
        {
            "TicoStarRing",
            createNameObj< TicoStarRing >,
            "Tico",
        },
        {
            "TicoAstro",
            createNameObj< TicoAstro >,
            nullptr,
        },
        {
            "TicoBaby",
            createNameObj< Tico >,
            nullptr,
        },
        {
            "DemoRabbit",
            createNameObj< DemoRabbit >,
            nullptr,
        },
        {
            "TicoFat",
            createNameObj< TicoFat >,
            "TicoFat",
        },
        {
            "TicoGalaxy",
            createNameObj< TicoGalaxy >,
            "TicoFat",
        },
        {
            "TicoRail",
            createNameObj< TicoRail >,
            "Tico",
        },
        {
            "TicoReading",
            createNameObj< TicoReading >,
            "Tico",
        },
        {
            "TicoDomeLecture",
            createNameObj< TicoDomeLecture >,
            "Tico",
        },
        {
            "Rosetta",
            createNameObj< Rosetta >,
            nullptr,
        },
        {
            "RosettaReading",
            createNameObj< RosettaReading >,
            "RosettaReading",
        },
        {
            "RosettaPictureBook",
            createNameObj< RosettaPictureBook >,
            "RosettaPictureBook",
        },
        {
            "LuigiNPC",
            createNameObj< LuigiNPC >,
            nullptr,
        },
        {
            "LuigiEvent",
            createNameObj< LuigiNPC >,
            nullptr,
        },
        {
            "TeresaRacer",
            createNameObj< TeresaRacer >,
            "TeresaRacer",
        },
        {
            "NPCSupportRail",
            createNameObj< NPCSupportRail >,
            nullptr,
        },
        {
            "GhostPlayer",
            createNameObj< GhostPlayer >,
            nullptr,
        },
        {
            "Butler",
            createNameObj< Butler >,
            "Butler",
        },
        {
            "ButlerMap",
            createNameObj< ButlerMap >,
            "Butler",
        },
        {
            "ButlerExplain",
            createNameObj< ButlerExplain >,
            "Butler",
        },
        {
            "DemoKoopaJrShip",
            createNameObj< DemoKoopaJrShip >,
            "KoopaJrShip",
        },
        {
            "Creeper",
            createNameObj< Creeper >,
            "Creeper",
        },
        {
            "JetTurtle",
            createNameObj< JetTurtle >,
            "Koura",
        },
        {
            "Fluff",
            createNameObj< Fluff >,
            "Fluff",
        },
        {
            "FluffWind",
            createNameObj< FluffWind >,
            nullptr,
        },
        {
            "Tamakoro",
            createNameObj< Tamakoro >,
            "Tamakoro",
        },
        {
            "TamakoroWithTutorial",
            MR::createTamakoroWithTutorial,
            "Tamakoro",
        },
        {
            "GoldenTurtle",
            createNameObj< GoldenTurtle >,
            "KouraShine",
        },
        {
            "JumpBranch",
            createNameObj< JumpBranch >,
            "JumpBranch",
        },
        {
            "Plant",
            createNameObj< Plant >,
            "Plant",
        },
        {
            "Pole",
            createNameObj< Pole >,
            "Pole",
        },
        {
            "PoleNoModel",
            createNameObj< Pole >,
            "Pole",
        },
        {
            "PoleSquare",
            createNameObj< Pole >,
            "PoleSquare",
        },
        {
            "PoleSquareNoModel",
            createNameObj< Pole >,
            "PoleSquare",
        },
        {
            "SurfRay",
            createNameObj< SurfRay >,
            "SurfRay",
        },
        {
            "SwingRope",
            createNameObj< SwingRope >,
            "SwingRope",
        },
        {
            "SpaceCocoon",
            createNameObj< SpaceCocoon >,
            "SpaceCocoon",
        },
        {
            "Trapeze",
            createNameObj< Trapeze >,
            "Trapeze",
        },
        {
            "TreeCube",
            createNameObj< Pole >,
            "TreeCube",
        },
        {
            "WaterRoad",
            createNameObj< WaterRoad >,
            "WaterWave",
        },
        {
            "WarpPod",
            createNameObj< WarpPod >,
            "WarpPod",
        },
        {
            "CoinBox",
            createNameObj< CoinBox >,
            nullptr,
        },
        {
            "PunchBox",
            createNameObj< PunchBox >,
            "PunchBox",
        },
        {
            "SwitchBox",
            createNameObj< SwitchBox >,
            "SwitchBox",
        },
        {
            "NormalMapTestObj",
            createNameObj< NormalMapTestObj >,
            "NormalMapTestObj",
        },
        {
            "Takobo",
            createNameObj< Takobo >,
            "Takobo",
        },
        {
            "Karikari",
            createNameObj< Karikari >,
            "Karipon",
        },
        {
            "KoteBug",
            createNameObj< KoteBug >,
            "KoteBug",
        },
        {
            "Balloonfish",
            createNameObj< Balloonfish >,
            "Balloonfish",
        },
        {
            "BombHei",
            createNameObj< BombHei >,
            "BombHei",
        },
        {
            "Bomb",
            createNameObj< BombHei >,
            "BombHei",
        },
        {
            "Jiraira",
            createNameObj< Jiraira >,
            "Jiraira",
        },
        {
            "BombHeiLauncher",
            createNameObj< BombHeiLauncher >,
            "BombHeiLauncher",
        },
        {
            "BombLauncher",
            createNameObj< BombHeiLauncher >,
            "BombHeiLauncher",
        },
        {
            "SpinDriver",
            createNameObj< SpinDriver >,
            "SpinDriver",
        },
        {
            "SuperSpinDriver",
            MR::createSuperSpinDriverYellow,
            "SuperSpinDriver",
        },
        {
            "SuperSpinDriverGreen",
            MR::createSuperSpinDriverGreen,
            "SuperSpinDriver",
        },
        {
            "SuperSpinDriverPink",
            MR::createSuperSpinDriverPink,
            "SuperSpinDriver",
        },
        {
            "Kuribo",
            createNameObj< Kuribo >,
            "Kuribo",
        },
        {
            "KuriboChief",
            createNameObj< KuriboChief >,
            "KuriboChief",
        },
        {
            "KuriboMini",
            createNameObj< KuriboMini >,
            "KuriboMini",
        },
        {
            "Kabokuri",
            createNameObj< Kabokuri >,
            "Kabokuri",
        },
        {
            "TakoHei",
            createNameObj< TakoHei >,
            "TakoHei",
        },
        {
            "OtaRock",
            createNameObj< OtaRock >,
            "OtaRock",
        },
        {
            "Meramera",
            createNameObj< Meramera >,
            "Meramera",
        },
        {
            "IceMeramera",
            createNameObj< Meramera >,
            "IceMeramera",
        },
        {
            "EyeBeamer",
            createNameObj< EyeBeamer >,
            "EyeBeamer",
        },
        {
            "BigBirikyu",
            createNameObj< Birikyu >,
            "BigBirikyu",
        },
        {
            "Birikyu",
            createNameObj< BirikyuWithFace >,
            "Birikyu",
        },
        {
            "HomingKiller",
            createNameObj< HomingKillerLauncher >,
            "HomingKiller",
        },
        {
            "CocoSambo",
            createNameObj< CocoSambo >,
            "CocoSamboBody",
        },
        {
            "SamboHead",
            createNameObj< SamboHead >,
            "SamboHead",
        },
        {
            "BegomanBaby",
            createNameObj< BegomanBaby >,
            "BegomanBaby",
        },
        {
            "BegomanSpring",
            createNameObj< BegomanSpring >,
            "BegomanSpring",
        },
        {
            "BegomanSpringHide",
            createNameObj< BegomanSpring >,
            "BegomanSpring",
        },
        {
            "BegomanSpike",
            createNameObj< BegomanSpike >,
            "BegomanSpike",
        },
        {
            "BegomanLauncher",
            createNameObj< BegomanLauncher >,
            "BegomanLauncher",
        },
        {
            "TogeBegomanLauncher",
            createNameObj< BegomanLauncher >,
            "BegomanLauncher",
        },
        {
            "BegomanBabyLauncher",
            createNameObj< BegomanLauncher >,
            "BegomanLauncher",
        },
        {
            "FireBubble",
            createNameObj< FireBubble >,
            "FireBubble",
        },
        {
            "BallBeamer",
            createNameObj< BallBeamer >,
            "BallBeamer",
        },
        {
            "JumpBeamer",
            createNameObj< JumpBeamer >,
            "JumpBeamerBody",
        },
        {
            "JumpGuarder",
            createNameObj< JumpGuarder >,
            "JumpGuarder",
        },
        {
            "RingBeamer",
            createNameObj< RingBeamer >,
            "RingBeamer",
        },
        {
            "Pukupuku",
            createNameObj< Pukupuku >,
            "Pukupuku",
        },
        {
            "BombTeresa",
            createNameObj< BombTeresa >,
            "BombTeresa",
        },
        {
            "Teresa",
            createNameObj< Teresa >,
            "Teresa",
        },
        {
            "TeresaChief",
            createNameObj< Teresa >,
            "Teresa",
        },
        {
            "TeresaWater",
            createNameObj< TeresaWater >,
            "TeresaWater",
        },
        {
            "Torpedo",
            createNameObj< HomingKillerLauncher >,
            "Torpedo",
        },
        {
            "Dossun",
            createNameObj< Dossun >,
            "Dossun",
        },
        {
            "Onimasu",
            createNameObj< OnimasuJump >,
            "Onimasu",
        },
        {
            "OnimasuPivot",
            createNameObj< OnimasuPivot >,
            "Onimasu",
        },
        {
            "Unizo",
            createNameObj< Unizo >,
            "Unizo",
        },
        {
            "UnizoLand",
            createNameObj< Unizo >,
            "UnizoLand",
        },
        {
            "UnizoShoal",
            createNameObj< Unizo >,
            "UnizoShoal",
        },
        {
            "UnizoLauncher",
            createNameObj< UnizoLauncher >,
            "UnizoLand",
        },
        {
            "Kiraira",
            createNameObj< Kiraira >,
            "Kiraira",
        },
        {
            "KirairaRail",
            createNameObj< Kiraira >,
            "Kiraira",
        },
        {
            "NokonokoLand",
            createNameObj< NokonokoLand >,
            "NokonokoLand",
        },
        {
            "Gesso",
            createNameObj< Gesso >,
            "Gesso",
        },
        {
            "Poihana",
            createNameObj< Poihana >,
            "Poihana",
        },
        {
            "Snakehead",
            createNameObj< Snakehead >,
            "Snakehead",
        },
        {
            "SnakeheadSmall",
            createNameObj< Snakehead >,
            "SnakeheadSmall",
        },
        {
            "Jellyfish",
            createNameObj< Jellyfish >,
            "Jellyfish",
        },
        {
            "JellyfishElectric",
            createNameObj< JellyfishElectric >,
            "JellyfishElectric",
        },
        {
            "Kanina",
            createNameObj< Kanina >,
            "Kanina",
        },
        {
            "KaninaRed",
            createNameObj< Kanina >,
            "KaninaRed",
        },
        {
            "Mogu",
            createNameObj< Mogu >,
            "Mogu",
        },
        {
            "Mogucchi",
            createNameObj< Mogucchi >,
            "Mogucchi",
        },
        {
            "MogucchiRefuseTerritory",
            createNameObj< MogucchiRefuseTerritory >,
            nullptr,
        },
        {
            "KoopaJrShip",
            createNameObj< KoopaJrShip >,
            "KoopaJrShip",
        },
        {
            "Hanachan",
            createNameObj< Hanachan >,
            "HanachanHead",
        },
        {
            "JumpSpider",
            createNameObj< JumpSpider >,
            "JumpSpider",
        },
        {
            "StringSpider",
            createNameObj< StringSpider >,
            "StringSpider",
        },
        {
            "PackunPetit",
            createNameObj< PackunPetit >,
            "PackunPetit",
        },
        {
            "StinkBugSmall",
            createNameObj< StinkBugSmall >,
            "StinkBugSmall",
        },
        {
            "StinkBugParent",
            createNameObj< StinkBugParent >,
            "StinkBugParent",
        },
        {
            "BasaBasa",
            createNameObj< BasaBasa >,
            "BasaBasa",
        },
        {
            "BasaBasaIce",
            createNameObj< BasaBasa >,
            "BasaBasaIce",
        },
        {
            "Metbo",
            createNameObj< Metbo >,
            "Metbo",
        },
        {
            "BombBird",
            createNameObj< BombBird >,
            "BombBird",
        },
        {
            "MechanicKoopaMini",
            createNameObj< MechanicKoopaMini >,
            "MechanicKoopaMini",
        },
        {
            "Karon",
            createNameObj< Karon >,
            "Karon",
        },
        {
            "HammerHeadPackun",
            createNameObj< HammerHeadPackun >,
            "PackunFlower",
        },
        {
            "DharmaSambo",
            createNameObj< DharmaSambo >,
            "DharmaSambo",
        },
        {
            "SearchBeamer",
            createNameObj< SearchBeamer >,
            "SearchBeamer",
        },
        {
            "Petari",
            createNameObj< Petari >,
            "Petari",
        },
        {
            "MagnumKiller",
            createNameObj< HomingKillerLauncher >,
            "MagnumKiller",
        },
        {
            "BossBegoman",
            createNameObj< BossBegoman >,
            "BossBegoman",
        },
        {
            "BossStinkBug",
            createNameObj< BossStinkBug >,
            "BossStinkBug",
        },
        {
            "DinoPackun",
            MR::createDinoPackunVs1,
            "DinoPackun",
        },
        {
            "DinoPackunVs2",
            MR::createDinoPackunVs2,
            "DinoPackun2",
        },
        {
            "Dodoryu",
            createNameObj< Dodoryu >,
            "Dodoryu",
        },
        {
            "KoopaVs1",
            KoopaFunction::createKoopaVs1,
            "Koopa",
        },
        {
            "KoopaVs2",
            KoopaFunction::createKoopaVs2,
            "Koopa",
        },
        {
            "KoopaVs3",
            KoopaFunction::createKoopaVs3,
            "Koopa",
        },
        {
            "OtaKing",
            createNameObj< OtaKing >,
            nullptr,
        },
        // FIXME: Polta is being worked on in #1867
        {
            "Polta",
            createNameObj< Polta >,
            "Polta",
        },
        {
            "SkeletalFishBoss",
            createNameObj< SkeletalFishBoss >,
            "SkeletalFishBoss",
        },
        {
            "SkeletalFishBaby",
            createNameObj< SkeletalFishBaby >,
            "SnakeFish",
        },
        {
            "SkeletalFishBabyRail",
            createNameObj< SkeletalFishBabyRail >,
            nullptr,
        },
        {
            "SkeletalFishBossRail",
            createNameObj< SkeletalFishBossRail >,
            nullptr,
        },
        {
            "TombSpider",
            createNameObj< TombSpider >,
            "TombSpider",
        },
        {
            "BossKameck",
            MR::createBossKameck1,
            "BossKameck",
        },
        {
            "BossKameck2",
            MR::createBossKameck2,
            "BossKameck",
        },
        {
            "FireBallBeamKameck",
            MR::createFireBallBeamKameck,
            "Kameck",
        },
        {
            "TurtleBeamKameck",
            MR::createTurtleBeamKameck,
            "Kameck",
        },
        {
            "TripodBoss",
            MR::createTripodBoss,
            "TripodBoss",
        },
        {
            "Tripod2Boss",
            MR::createTripod2Boss,
            "Tripod2Boss",
        },
        {
            "TripodBossBaseJointPosition",
            createNameObj< TripodBossBaseJointPosition >,
            nullptr,
        },
        {
            "TripodBossKillerGenerater",
            createNameObj< TripodBossKillerGenerater >,
            "TripodBossKillerCannon",
        },
        {
            "TripodBossStepPoint",
            createNameObj< TripodBossStepPoint >,
            nullptr,
        },
        {
            "TripodBossCore",
            createNameObj< TripodBossCore >,
            "TripodBossCore",
        },
        {
            "TripodBossCoin",
            createNameObj< TripodBossCoin >,
            nullptr,
        },
        {
            "TripodBossKinokoOneUp",
            createNameObj< TripodBossKinokoOneUp >,
            "KinokoOneUp",
        },
        {
            "TripodBossShell",
            createNameObj< TripodBossShell >,
            "TripodBossShell",
        },
        {
            "TripodBossGuardWall",
            createNameObj< TripodBossGuardWall >,
            "TripodBossGuardWall",
        },
        {
            "TriPodBossBodyA",
            createNameObj< TripodBossFixParts >,
            "TriPodBossBodyA",
        },
        {
            "TripodBossBodyABloom",
            createNameObj< TripodBossFixParts >,
            "TripodBossBodyABloom",
        },
        {
            "TriPodBossLeg1A",
            createNameObj< TripodBossFixParts >,
            "TriPodBossLeg1A",
        },
        {
            "TriPodBossLeg1B",
            createNameObj< TripodBossFixParts >,
            "TriPodBossLeg1B",
        },
        {
            "TriPodBossLeg1C",
            createNameObj< TripodBossFixParts >,
            "TriPodBossLeg1C",
        },
        {
            "TriPodBossLeg2A",
            createNameObj< TripodBossFixParts >,
            "TriPodBossLeg2A",
        },
        {
            "TriPodBossLeg2ABloom",
            createNameObj< TripodBossFixParts >,
            "TriPodBossLeg2ABloom",
        },
        {
            "TriPodBossLeg3A",
            createNameObj< TripodBossFixParts >,
            "TriPodBossLeg3A",
        },
        {
            "TriPodBossLeg3B",
            createNameObj< TripodBossFixParts >,
            "TriPodBossLeg3B",
        },
        {
            "TriPodBossLeg3C",
            createNameObj< TripodBossFixParts >,
            "TriPodBossLeg3C",
        },
        {
            "TripodBossFoot",
            createNameObj< TripodBossFixParts >,
            "TripodBossFoot",
        },
        {
            "TripodBossFootBloom",
            createNameObj< TripodBossFixParts >,
            "TripodBossFootBloom",
        },
        {
            "TriPodBossLegBottom",
            createNameObj< TripodBossFixParts >,
            "TriPodBossLegBottom",
        },
        {
            "TripodBossEyeLight",
            createNameObj< TripodBossFixParts >,
            "TripodBossEyeLight",
        },
        {
            "TripodBossEyeLightBloom",
            createNameObj< TripodBossFixParts >,
            "TripodBossEyeLightBloom",
        },
        {
            "TriPodBossGearA",
            createNameObj< TripodBossRotateParts >,
            "TriPodBossGearA",
        },
        {
            "TriPodBossGearB",
            createNameObj< TripodBossRotateParts >,
            "TriPodBossGearB",
        },
        {
            "TriPodBossGearC",
            createNameObj< TripodBossRotateParts >,
            "TriPodBossGearC",
        },
        {
            "TriPodBossGearSmallA",
            createNameObj< TripodBossRotateParts >,
            "TriPodBossGearSmallA",
        },
        {
            "TriPodBossGearSmallB",
            createNameObj< TripodBossRotateParts >,
            "TriPodBossGearSmallB",
        },
        {
            "TriPodBossGearSmallC",
            createNameObj< TripodBossRotateParts >,
            "TriPodBossGearSmallC",
        },
        {
            "TriPodBossGearBody",
            createNameObj< TripodBossRotateParts >,
            "TriPodBossGearBody",
        },
        {
            "TripodBossUpperKillerCannon",
            createNameObj< TripodBossKillerGeneraterCircle >,
            "TripodBossKillerCannon",
        },
        {
            "TripodBossUnderKillerCannon",
            createNameObj< TripodBossKillerGeneraterCircle >,
            "TripodBossKillerCannon",
        },
        {
            "TripodBossBottomKillerCannon",
            createNameObj< TripodBossKillerGeneraterCircle >,
            "TripodBossKillerCannon",
        },
        {
            "SpiderAttachPoint",
            createNameObj< SlingShooter >,
            "SpiderThreadAttachPoint",
        },
        {
            "SpiderCoin",
            createNameObj< SpiderCoin >,
            "Coin",
        },
        {
            "SpiderMapBlock",
            createNameObj< SpiderMapBlock >,
            "SpiderMapBlock",
        },
        {
            "ArrowSwitch",
            createNameObj< ArrowSwitch >,
            "ArrowSwitch",
        },
        {
            "ArrowSwitchMulti",
            createNameObj< ArrowSwitchMulti >,
            "ArrowSwitch",
        },
        {
            "ArrowSwitchTarget",
            createNameObj< ArrowSwitchTarget >,
            nullptr,
        },
        {
            "AstroOverlookObj",
            createNameObj< AstroOverlookObj >,
            nullptr,
        },
        {
            "BlueStarCupsulePlanet",
            createNameObj< BlueStarCupsulePlanet >,
            "BlueStarCupsulePlanet",
        },
        {
            "BigFan",
            createNameObj< BigFan >,
            "BigFan",
        },
        {
            "BigFanNoLeg",
            createNameObj< BigFan >,
            "BigFanNoLeg",
        },
        {
            "ClipAreaDropLaser",
            createNameObj< ClipAreaDropLaser >,
            "ClipVolumeSphere",
        },
        {
            "RevolvingWay",
            createNameObj< RevolvingWay >,
            "RevolvingWay",
        },
        {
            "SoundEmitter",
            createNameObj< SoundEmitter >,
            nullptr,
        },
        {
            "JumpHole",
            createNameObj< JumpHole >,
            "JumpHole",
        },
        {
            "BallOpener",
            createNameObj< BallOpener >,
            "BallOpener",
        },
        {
            "DragonHeadFlower",
            createNameObj< DragonHeadFlower >,
            "DragonHeadFlower",
        },
        {
            "ConcreteBlock",
            createNameObj< SimpleBreakableObj >,
            "ConcreteBlock",
        },
        {
            "SphereRailDash",
            createNameObj< SphereRailDash >,
            nullptr,
        },
        {
            "SnowplowSwitch",
            createNameObj< SnowplowSwitch >,
            nullptr,
        },
        {
            "BallRail",
            createNameObj< BallRail >,
            nullptr,
        },
        {
            "FallDownBridge",
            createNameObj< FallDownBridge >,
            "FallDownBridge",
        },
        {
            "RockCreator",
            createNameObj< RockCreator >,
            "Rock",
        },
        {
            "MeteoContainer",
            createNameObj< MeteoContainer >,
            "MeteoContainer",
        },
        {
            "MagicBell",
            createNameObj< MagicBell >,
            "Bell",
        },
        {
            "FlowerGroup",
            createNameObj< PlantGroup >,
            "Flower",
        },
        {
            "FlowerBlueGroup",
            createNameObj< PlantGroup >,
            "FlowerBlue",
        },
        {
            "CutBushGroup",
            createNameObj< PlantGroup >,
            "CutBush",
        },
        {
            "SmallStone",
            createNameObj< SmallStone >,
            "SmallStone",
        },
        {
            "CircleShell",
            createNameObj< SmallStone >,
            "CircleShell",
        },
        {
            "CircleStrawberry",
            createNameObj< SmallStone >,
            "CircleStrawberry",
        },
        {
            "Tsukidashikun",
            createNameObj< Tsukidashikun >,
            "Tsukidashikun",
        },
        {
            "TsukidashikunBig",
            createNameObj< RailBlock >,
            "TsukidashikunBig",
        },
        {
            "TimerCoinBlock",
            createNameObj< ItemBlock >,
            "CoinBlock",
        },
        {
            "SplashCoinBlock",
            createNameObj< ItemBlock >,
            "CoinBlock",
        },
        {
            "TimerPieceBlock",
            createNameObj< ItemBlock >,
            "CoinBlock",
        },
        {
            "SplashPieceBlock",
            createNameObj< ItemBlock >,
            "CoinBlock",
        },
        {
            "ItemBlockSwitch",
            createNameObj< ItemBlock >,
            "CoinBlock",
        },
        {
            "ExterminationCheckerKuribo",
            MR::createExterminationPowerStar,
            "Kuribo",
        },
        {
            "ExterminationKuriboKeySwitch",
            MR::createExterminationKeySwitch,
            "Kuribo",
        },
        {
            "ExterminationSkeletalFishBaby",
            MR::createExterminationPowerStar,
            "SnakeFish",
        },
        {
            "ExterminationMeramera",
            MR::createExterminationPowerStar,
            "Meramera",
        },
        {
            "ShootingStar",
            createNameObj< ShootingStar >,
            "ShootingStar",
        },
        {
            "FlameGun",
            createNameObj< FlameGun >,
            "FlameGun",
        },
        {
            "BenefitItemOneUp",
            createNameObj< BenefitItemOneUp >,
            "KinokoOneUp",
        },
        {
            "BenefitItemLifeUp",
            createNameObj< BenefitItemLifeUp >,
            "KinokoLifeUp",
        },
        {
            "BenefitItemInvincible",
            createNameObj< BenefitItemInvincible >,
            "PowerupInvincible",
        },
        {
            "CoinSpot",
            createNameObj< CoinSpot >,
            nullptr,
        },
        {
            "CollisionBlocker",
            createNameObj< CollisionBlocker >,
            nullptr,
        },
        {
            "LotusLeaf",
            createNameObj< LotusLeaf >,
            "LotusLeaf",
        },
        {
            "PunchingKinoko",
            createNameObj< PunchingKinoko >,
            "PunchingKinoko",
        },
        {
            "DesertMovingLand",
            createNameObj< DesertMovingLand >,
            "DesertMovingLand",
        },
        {
            "DesertMovingLandSmall",
            createNameObj< DesertMovingLand >,
            "DesertMovingLandSmall",
        },
        {
            "NoteFairy",
            createNameObj< NoteFairy >,
            nullptr,
        },
        {
            "EarthenPipe",
            createNameObj< EarthenPipe >,
            "EarthenPipe",
        },
        {
            "EarthenPipeInWater",
            createNameObj< EarthenPipe >,
            "EarthenPipe",
        },
        {
            "ElectricRail",
            createNameObj< ElectricRail >,
            "ElectricRail",
        },
        {
            "ElectricRailMoving",
            createNameObj< ElectricRailMoving >,
            "ElectricRailMoving",
        },
        {
            "WanwanRolling",
            createNameObj< RockCreator >,
            "WanwanRolling",
        },
        {
            "WanwanRollingMini",
            createNameObj< RockCreator >,
            "WanwanRollingMini",
        },
        {
            "WanwanRollingGold",
            createNameObj< RockCreator >,
            "WanwanRollingGold",
        },
        {
            "BreakableCage",
            createNameObj< BreakableCage >,
            "BreakableCage",
        },
        {
            "BreakableCageRotate",
            createNameObj< BreakableCage >,
            "BreakableCage",
        },
        {
            "BreakableCageL",
            createNameObj< BreakableCage >,
            "BreakableCageL",
        },
        {
            "BreakableFixation",
            createNameObj< BreakableCage >,
            "BreakableFixation",
        },
        {
            "BreakableTrash",
            createNameObj< BreakableCage >,
            "BreakableTrash",
        },
        {
            "BigBubble",
            createNameObj< BigBubble >,
            "BigBubble",
        },
        {
            "BigBubbleMoveLimitterCylinder",
            MR::createBigBubbleMoveLimitterCylinder,
            nullptr,
        },
        {
            "BigBubbleMoveLimitterPlane",
            MR::createBigBubbleMoveLimitterPlane,
            nullptr,
        },
        {
            "BigBubbleGenerator",
            MR::createBigBubbleGenerator,
            "AirBubbleGenerator",
        },
        {
            "BigObstructBubbleGenerator",
            MR::createBigObstructBubbleGenerator,
            "AirBubbleGenerator",
        },
        {
            "GCaptureTarget",
            createNameObj< GCaptureTarget >,
            "GCaptureTarget",
        },
        {
            "AsteroidA",
            createNameObj< SimpleEnvironmentObj >,
            "AsteroidA",
        },
        {
            "AsteroidB",
            createNameObj< SimpleEnvironmentObj >,
            "AsteroidB",
        },
        {
            "AsteroidC",
            createNameObj< SimpleEnvironmentObj >,
            "AsteroidC",
        },
        {
            "AsteroidD",
            createNameObj< SimpleEnvironmentObj >,
            "AsteroidD",
        },
        {
            "SpaceStickA",
            createNameObj< SimpleEnvironmentObj >,
            "SpaceStickA",
        },
        {
            "SpaceStickB",
            createNameObj< SimpleEnvironmentObj >,
            "SpaceStickB",
        },
        {
            "BlackHole",
            createNameObj< BlackHole >,
            "BlackHole",
        },
        {
            "BlackHoleCube",
            createNameObj< BlackHole >,
            "BlackHole",
        },
        {
            "KillerGunnerSingle",
            createNameObj< KillerGunnerSingle >,
            "KillerGunnerSingle",
        },
        {
            "KillerGunnerDouble",
            createNameObj< SimpleEnvironmentObj >,
            "KillerGunnerDouble",
        },
        {
            "KillerGunnerTriple",
            createNameObj< SimpleEnvironmentObj >,
            "KillerGunnerTriple",
        },
        {
            "PowerStar",
            createNameObj< PowerStar >,
            "PowerStar",
        },
        {
            "GrandStar",
            createNameObj< PowerStar >,
            "GrandStar",
        },
        {
            "PowerStarAppearPoint",
            createNameObj< PowerStarAppearPoint >,
            nullptr,
        },
        {
            "CocoNut",
            createNameObj< CocoNut >,
            nullptr,
        },
        {
            "CoconutTree",
            createNameObj< CoconutTree >,
            "CoconutTree",
        },
        {
            "CoconutTreeLeaf",
            createNameObj< CoconutTreeLeafGroup >,
            "CoconutTreeLeaf",
        },
        {
            "CollapsePlane",
            createNameObj< CollapsePlane >,
            "CollapsePlane",
        },
        {
            "WoodBox",
            createNameObj< WoodBox >,
            "WoodBox",
        },
        {
            "ItemBubble",
            createNameObj< ItemBubble >,
            "ItemBubble",
        },
        {
            "SpinningBox",
            createNameObj< SpinningBox >,
            "SpinningBox",
        },
        {
            "ElectricBall",
            createNameObj< ElectricBall >,
            "ElectricBall",
        },
        {
            "SpaceMine",
            createNameObj< SpaceMine >,
            "SpaceMine",
        },
        {
            "MorphItemNeoHopper",
            createNameObj< MorphItemNeoHopper >,
            "PowerupHopper",
        },
        {
            "MorphItemNeoBee",
            createNameObj< MorphItemNeoBee >,
            "PowerUpBee",
        },
        {
            "MorphItemNeoTeresa",
            createNameObj< MorphItemNeoTeresa >,
            "PowerupTeresa",
        },
        {
            "MorphItemNeoIce",
            createNameObj< MorphItemNeoIce >,
            "PowerupIce",
        },
        {
            "MorphItemNeoFire",
            createNameObj< MorphItemNeoFire >,
            "PowerupFire",
        },
        {
            "MorphItemNeoFoo",
            createNameObj< MorphItemNeoFoo >,
            "PowerupFoo",
        },
        {
            "AirBubble",
            createNameObj< AirBubble >,
            "AirBubble",
        },
        {
            "AirBubbleGenerator",
            createNameObj< AirBubbleGenerator >,
            "AirBubbleGenerator",
        },
        {
            "Butterfly",
            createNameObj< Butterfly >,
            "Butterfly",
        },
        {
            "Pompon2Plant",
            createNameObj< PomponPlant >,
            "Pompon2Plant",
        },
        {
            "Pompon4Plant",
            createNameObj< PomponPlant >,
            "Pompon4Plant",
        },
        {
            "LavaGeyser",
            createNameObj< LavaGeyser >,
            "LavaGeyser",
        },
        {
            "LavaProminence",
            createNameObj< LavaProminence >,
            "LavaProminence",
        },
        {
            "LavaProminenceTriple",
            createNameObj< LavaProminenceTriple >,
            "LavaProminenceTriple",
        },
        {
            "LavaProminenceEnvironment",
            createNameObj< LavaProminenceTriple >,
            "LavaProminenceEnvironment",
        },
        {
            "LavaProminenceWithoutShadow",
            createNameObj< LavaProminence >,
            "LavaProminence",
        },
        {
            "LavaObstacleRockShell",
            createNameObj< HipDropMoveObj >,
            "LavaObstacleRockShell",
        },
        {
            "LavaObstacleRockHomeA",
            createNameObj< HipDropDemoMoveObj >,
            "LavaObstacleRockHomeA",
        },
        {
            "LavaObstacleRockHomeB",
            createNameObj< AnmModelSwitchMove >,
            "LavaObstacleRockHomeB",
        },
        {
            "LavaHomeFloaterB",
            createNameObj< LavaFloater >,
            "LavaHomeFloaterB",
        },
        {
            "LavaFloatingStepA",
            createNameObj< LavaFloater >,
            "LavaFloatingStepA",
        },
        {
            "LavaFloatingStepB",
            createNameObj< LavaFloater >,
            "LavaFloatingStepB",
        },
        {
            "LavaFloatingStepC",
            createNameObj< LavaFloater >,
            "LavaFloatingStepC",
        },
        {
            "LavaFloatingStepD",
            createNameObj< LavaFloater >,
            "LavaFloatingStepD",
        },
        {
            "LavaHomeSeesaw",
            createNameObj< LavaHomeSeesaw >,
            "LavaHomeSeesaw",
        },
        {
            "LavaHomeVolcanoFlow",
            createNameObj< LavaHomeVolcanoFlow >,
            "LavaHomeVolcanoFlow",
        },
        {
            "LavaHomeVolcanoInnerFlow",
            createNameObj< RailMoveObj >,
            "LavaHomeVolcanoInnerFlow",
        },
        {
            "LavaRotatePlanetStartStep",
            createNameObj< RailMoveObj >,
            "LavaRotatePlanetStartStep",
        },
        {
            "LavaBreakBridge",
            createNameObj< AnmModelGroundOnMove >,
            "LavaBreakBridge",
        },
        {
            "LavaBreakRockA",
            createNameObj< AnmModelSwitchMoveEndKill >,
            "LavaBreakRockA",
        },
        {
            "LavaBreakRockB",
            createNameObj< AnmModelGroundOnMove >,
            "LavaBreakRockB",
        },
        {
            "LavaBreakRockC",
            createNameObj< AnmModelGroundOnMove >,
            "LavaBreakRockC",
        },
        {
            "LavaBreakRockD",
            createNameObj< AnmModelGroundOnMove >,
            "LavaBreakRockD",
        },
        {
            "LavaBreakColumn",
            createNameObj< LavaBreakColumn >,
            "LavaBreakColumn",
        },
        {
            "LavaShellTower",
            createNameObj< LavaShellTower >,
            "LavaShellTower",
        },
        {
            "LavaFallDownColumnA",
            createNameObj< AnmModelSwitchMove >,
            "LavaFallDownColumnA",
        },
        {
            "LavaFallDownColumnB",
            createNameObj< AnmModelSwitchMove >,
            "LavaFallDownColumnB",
        },
        {
            "LavaShellTowerStep",
            createNameObj< LavaGalaxyParts >,
            "LavaShellTowerStep",
        },
        {
            "LavaStepA",
            createNameObj< LavaGalaxyParts >,
            "LavaStepA",
        },
        {
            "LavaStepB",
            createNameObj< LavaGalaxyParts >,
            "LavaStepB",
        },
        {
            "LavaStepC",
            createNameObj< LavaGalaxyParts >,
            "LavaStepC",
        },
        {
            "LavaSteam",
            createNameObj< LavaSteam >,
            "LavaSteam",
        },
        {
            "LavaStrangeRock",
            createNameObj< LavaStrangeRock >,
            "LavaStrangeRock",
        },
        {
            "LavaStrangeRockL",
            createNameObj< LavaStrangeRock >,
            "LavaStrangeRockL",
        },
        {
            "LavaStrangeRockTable",
            createNameObj< LavaStrangeRock >,
            "LavaStrangeRockTable",
        },
        {
            "LavaSpaceStickA",
            createNameObj< SimpleEnvironmentObj >,
            "LavaSpaceStickA",
        },
        {
            "LavaSpaceStickB",
            createNameObj< SimpleEnvironmentObj >,
            "LavaSpaceStickB",
        },
        {
            "LavaBlackUFO",
            createNameObj< SimpleEnvironmentObj >,
            "LavaBlackUFO",
        },
        {
            "CrystalCageS",
            createNameObj< CrystalCage >,
            "CrystalCageS",
        },
        {
            "CrystalCageM",
            createNameObj< CrystalCage >,
            "CrystalCageM",
        },
        {
            "CrystalCageL",
            createNameObj< CrystalCage >,
            "CrystalCageL",
        },
        {
            "CrystalCageMoving",
            createNameObj< CrystalCageMoving >,
            "CrystalCageMoving",
        },
        {
            "ShellfishCoin",
            createNameObj< Shellfish >,
            "Shellfish",
        },
        {
            "ShellfishYellowChip",
            createNameObj< Shellfish >,
            "Shellfish",
        },
        {
            "SeaBottomBigDoorA",
            createNameObj< RailDemoMoveObj >,
            "SeaBottomBigDoorA",
        },
        {
            "SeaBottomBigDoorB",
            createNameObj< RailDemoMoveObj >,
            "SeaBottomBigDoorB",
        },
        {
            "SeaBottomBigDoorC",
            createNameObj< RailDemoMoveObj >,
            "SeaBottomBigDoorC",
        },
        {
            "FloaterLandPartsFrame",
            createNameObj< SimpleMapObj >,
            "FloaterLandPartsFrame",
        },
        {
            "OceanFloaterTypeU",
            createNameObj< OceanFloaterLandParts >,
            "OceanFloaterTypeU",
        },
        {
            "DashRing",
            createNameObj< DashRing >,
            "DashRing",
        },
        {
            "MeteorStrike",
            createNameObj< MeteorStrikeLauncher >,
            "MeteorStrike",
        },
        {
            "MeteorStrikeEnvironment",
            createNameObj< MeteorStrikeLauncher >,
            "MeteorStrike",
        },
        {
            "MeteorCannon",
            createNameObj< MeteorStrikeLauncher >,
            "MeteorStrike",
        },
        {
            "BrightObj",
            createNameObj< BrightObj >,
            "LensFlare",
        },
        {
            "BrightSun",
            createNameObj< BrightSun >,
            "LensFlare",
        },
        {
            "PhantomTorch",
            createNameObj< PhantomTorch >,
            nullptr,
        },
        {
            "PhantomBonfire",
            createNameObj< PhantomTorch >,
            nullptr,
        },
        {
            "PhantomCandlestand",
            createNameObj< Candlestand >,
            "PhantomCandlestand",
        },
        {
            "CandlestandIceVolcano",
            createNameObj< Candlestand >,
            "CandlestandIceVolcano",
        },
        {
            "MarioLauncher",
            createNameObj< MarioLauncher >,
            "MarioLauncher",
        },
        {
            "MarioLauncherAttractor",
            createNameObj< MarioLauncherAttractor >,
            "MarioLauncherAttractor",
        },
        {
            "WaterBazooka",
            createNameObj< WaterBazooka >,
            "WaterBazooka",
        },
        {
            "ElectricBazooka",
            createNameObj< WaterBazooka >,
            "ElectricBazooka",
        },
        {
            "WaterPressure",
            createNameObj< WaterPressure >,
            "WaterPressure",
        },
        {
            "FirePressure",
            createNameObj< FirePressure >,
            "FirePressure",
        },
        {
            "IronCannonLauncher",
            createNameObj< IronCannonLauncher >,
            "IronCannonLauncher",
        },
        {
            "IronCannonLauncherPoint",
            createNameObj< IronCannonLauncherPoint >,
            nullptr,
        },
        {
            "FirePressureRadiate",
            createNameObj< FirePressureRadiate >,
            "FirePressure",
        },
        {
            "TemplateStageGeometry",
            createNameObj< SimpleMapObj >,
            "TemplateStageGeometry",
        },
        {
            "SunkenShip",
            createNameObj< SimpleBreakableObj >,
            "SunkenShip",
        },
        {
            "BeeFlowerHover",
            createNameObj< BeeFlowerHover >,
            "BeeFlowerHover",
        },
        {
            "WaterfallCaveCover",
            createNameObj< WaterfallCaveCover >,
            "WaterfallCaveCover",
        },
        {
            "WaterfallCaveNoBreakCover",
            createNameObj< SimpleMapObj >,
            "WaterfallCaveNoBreakCover",
        },
        {
            "PalmIsland",
            createNameObj< PalmIsland >,
            "PalmIsland",
        },
        {
            "IceMerameraKing",
            createNameObj< IceMerameraKing >,
            "IceMerameraKing",
        },
        {
            "OceanPierFloaterA",
            createNameObj< OceanWaveFloater >,
            "OceanPierFloaterA",
        },
        {
            "HeavenlyBeachTopRock",
            createNameObj< AnmModelSwitchMoveEndKill >,
            "HeavenlyBeachTopRock",
        },
        {
            "HeavenlyBeachUnderRock",
            createNameObj< AnmModelSwitchMove >,
            "HeavenlyBeachUnderRock",
        },
        {
            "OceanSmallTurtle",
            createNameObj< OceanSmallTurtle >,
            "OceanSmallTurtle",
        },
        {
            "OceanHexagonFloater",
            createNameObj< OceanWaveFloater >,
            "OceanHexagonFloater",
        },
        {
            "SpringWaterFloaterA",
            createNameObj< SpringWaterFloaterSpot >,
            "SpringWaterFloaterA",
        },
        {
            "SpringWaterFloaterB",
            createNameObj< SpringWaterFloaterSpot >,
            "SpringWaterFloaterB",
        },
        {
            "ShutterDoorB",
            createNameObj< RailMoveObj >,
            "ShutterDoorB",
        },
        {
            "ShutterDoorC",
            createNameObj< AnmModelSwitchMove >,
            "ShutterDoorC",
        },
        {
            "ShutterDoorD",
            createNameObj< AnmModelSwitchMove >,
            "ShutterDoorD",
        },
        {
            "ShutterDoorE",
            createNameObj< AnmModelSwitchMove >,
            "ShutterDoorE",
        },
        {
            "SeaBottomTriplePropellerStand",
            createNameObj< SimpleMapObj >,
            "SeaBottomTriplePropellerStand",
        },
        {
            "SeaBottomTriplePropeller",
            createNameObj< SeaBottomTriplePropeller >,
            "SeaBottomTriplePropeller",
        },
        {
            "OceanFloaterTowerRotateStepA",
            createNameObj< RotateMoveObj >,
            "OceanFloaterTowerRotateStepA",
        },
        {
            "OceanFloaterTowerRotateStepB",
            createNameObj< RotateMoveObj >,
            "OceanFloaterTowerRotateStepB",
        },
        {
            "OceanFloaterTowerRotateStepC",
            createNameObj< RotateMoveObj >,
            "OceanFloaterTowerRotateStepC",
        },
        {
            "OceanFloaterTowerRotateStepD",
            createNameObj< RotateMoveObj >,
            "OceanFloaterTowerRotateStepD",
        },
        {
            "LargeChain",
            createNameObj< LargeChain >,
            "LargeChain",
        },
        {
            "IcicleRock",
            createNameObj< SimpleBreakableObj >,
            "IcicleRock",
        },
        {
            "ItemAppearStoneSpinDriver",
            createNameObj< ItemAppearStone >,
            "ItemAppearStoneSpinDriver",
        },
        {
            "SnowFloor",
            createNameObj< SnowFloor >,
            nullptr,
        },
        {
            "SnowFloorTile",
            createNameObj< SnowFloorTile >,
            nullptr,
        },
        {
            "FlipPanel",
            createNameObj< FlipPanel >,
            "FlipPanel",
        },
        {
            "FlipPanelObserver",
            createNameObj< FlipPanelObserver >,
            nullptr,
        },
        {
            "FlipPanelReverse",
            createNameObj< FlipPanel >,
            "FlipPanelReverse",
        },
        {
            "FlipPanelFrame",
            createNameObj< SimpleMapObj >,
            "FlipPanelFrame",
        },
        {
            "PhantomTowerMoveStepA",
            createNameObj< RailMoveObj >,
            "PhantomTowerMoveStepA",
        },
        {
            "SideSpikeMoveStepA",
            createNameObj< SideSpikeMoveStep >,
            "SideSpikeMoveStepA",
        },
        {
            "SpaceMineRailA",
            createNameObj< SimpleMapObj >,
            "SpaceMineRailA",
        },
        {
            "SpaceMineRail5m",
            createNameObj< SimpleMapObj >,
            "SpaceMineRail5m",
        },
        {
            "SandUpDownTowerTop",
            createNameObj< SimpleMapObjFarMax >,
            "SandUpDownTowerTop",
        },
        {
            "SandUpDownTowerBottom",
            createNameObj< SimpleMapObjFarMax >,
            "SandUpDownTowerBottom",
        },
        {
            "SandUpDownKillerGunnerBase",
            createNameObj< SimpleMapObj >,
            "SandUpDownKillerGunnerBase",
        },
        {
            "SandUpDownTriRock",
            createNameObj< SandUpDownTriRock >,
            "SandUpDownTriRock",
        },
        {
            "JumpStand",
            createNameObj< JumpStand >,
            "JumpStand",
        },
        {
            "CaretakerGarbage",
            createNameObj< SimpleMapObj >,
            "CaretakerGarbage",
        },
        {
            "GlassBottleTall",
            createNameObj< SimpleMapObj >,
            "GlassBottleTall",
        },
        {
            "HopperBeltConveyerRotatePartsA",
            createNameObj< RotateMoveObj >,
            "HopperBeltConveyerRotatePartsA",
        },
        {
            "HopperBeltConveyerMovePartsA",
            createNameObj< RailMoveObj >,
            "HopperBeltConveyerMovePartsA",
        },
        {
            "ChooChooTrain",
            createNameObj< ChooChooTrain >,
            "ChooChooTrain",
        },
        {
            "SandRiverLightA",
            createNameObj< SwingLight >,
            "SandRiverLightA",
        },
        {
            "SnowMan",
            createNameObj< SnowMan >,
            "SnowMan",
        },
        {
            "SnowBlockA",
            createNameObj< SimpleBreakableObj >,
            "SnowBlockA",
        },
        {
            "ShockWaveGenerator",
            createNameObj< ShockWaveGenerator >,
            "ShockWaveGenerator",
        },
        {
            "DangerSignBoard",
            createNameObj< SimpleMapObjPush >,
            "DangerSignBoard",
        },
        {
            "Banekiti",
            createNameObj< Banekiti >,
            "Banekiti",
        },
        {
            "MirrorReflectionTest",
            createNameObj< SimpleMirrorObj >,
            "MirrorReflectionTest",
        },
        {
            "MirrorReflectionPeachCastle",
            createNameObj< SimpleMirrorObj >,
            "MirrorReflectionPeachCastle",
        },
        {
            "MirrorReflectionTwinFallLake",
            createNameObj< SimpleMirrorObj >,
            "MirrorReflectionTwinFallLake",
        },
        {
            "MirrorModelTest",
            createNameObj< SimpleMirrorReflectionObj >,
            "MirrorModelTest",
        },
        {
            "MirrorModelPeachCastle",
            createNameObj< SimpleMirrorReflectionObj >,
            "MirrorModelPeachCastle",
        },
        {
            "MirrorModelTwinFallLake",
            createNameObj< SimpleMirrorReflectionObj >,
            "MirrorModelTwinFallLake",
        },
        {
            "PhantomFirewood",
            createNameObj< SimpleMapObj >,
            "PhantomFirewood",
        },
        {
            "ArrowBoard",
            createNameObj< SimpleMapObj >,
            "ArrowBoard",
        },
        {
            "TeresaRoomGlaringLight",
            createNameObj< GlaringLight >,
            "TeresaRoomGlaringLight",
        },
        {
            "SpaceDustWoodA",
            createNameObj< SimpleEnvironmentObj >,
            "SpaceDustWoodA",
        },
        {
            "SpaceDustWoodB",
            createNameObj< SimpleEnvironmentObj >,
            "SpaceDustWoodB",
        },
        {
            "TeresaRoomDoor",
            createNameObj< AnmModelSwitchMoveEventCamera >,
            "TeresaRoomDoor",
        },
        {
            "TrampleStar",
            createNameObj< TrampleStar >,
            "TrampleStar",
        },
        {
            "ReverseGravityTowerInside",
            createNameObj< SimpleMapObj >,
            "ReverseGravityTowerInside",
        },
        {
            "HipDropRock",
            createNameObj< HipDropRock >,
            "HipDropRock",
        },
        {
            "DropOfWaterCore",
            createNameObj< SimpleMapObj >,
            "DropOfWaterCore",
        },
        {
            "WatchTowerRotateStep",
            createNameObj< WatchTowerRotateStep >,
            "WatchTowerRotateStep",
        },
        {
            "ForestAppearStepA",
            createNameObj< SimpleMapObj >,
            "ForestAppearStepA",
        },
        {
            "ForestObstacleWoodA",
            createNameObj< HipDropDemoMoveObj >,
            "ForestObstacleWoodA",
        },
        {
            "AstroDomeAsteroid",
            createNameObj< AstroDomeAsteroid >,
            "AstroDomeAsteroid",
        },
        {
            "AstroDomeBlueStar",
            createNameObj< AstroDomeBlueStar >,
            "GCaptureTarget",
        },
        {
            "AstroDomeComet",
            createNameObj< AstroDomeComet >,
            "AstroDomeComet",
        },
        {
            "AstroDomeDemoAstroGalaxy",
            createNameObj< AstroDomeDemoAstroGalaxy >,
            "AstroDomeDemoAstroGalaxy",
        },
        {
            "SphereSelectorHandle",
            createNameObj< SphereSelectorHandle >,
            nullptr,
        },
        {
            "ForestWoodCover",
            createNameObj< SimpleMapObj >,
            "ForestWoodCover",
        },
        {
            "KoopaShipManholeCover",
            createNameObj< ManholeCover >,
            "KoopaShipManholeCover",
        },
        {
            "StarDustStepA",
            createNameObj< SimpleMapObj >,
            "StarDustStepA",
        },
        {
            "StarDustStepB",
            createNameObj< SimpleMapObj >,
            "StarDustStepB",
        },
        {
            "TreasureSpot",
            createNameObj< TreasureSpot >,
            "TreasureSpot",
        },
        {
            "CoinFlower",
            createNameObj< TreasureSpot >,
            "CoinFlower",
        },
        {
            "StarDustRollingStepA",
            createNameObj< RotateMoveObj >,
            "StarDustRollingStepA",
        },
        {
            "AsteroidMoveA",
            createNameObj< RailMoveObj >,
            "AsteroidMoveA",
        },
        {
            "StarPieceCluster",
            createNameObj< SimpleMapObj >,
            "StarPieceCluster",
        },
        {
            "SpaceSeparatorA",
            createNameObj< SimpleMapObj >,
            "SpaceSeparatorA",
        },
        {
            "SpaceSeparatorB",
            createNameObj< SimpleMapObj >,
            "SpaceSeparatorB",
        },
        {
            "ForestNarrowStepA",
            createNameObj< SimpleMapObj >,
            "ForestNarrowStepA",
        },
        {
            "ForestHomeGate",
            createNameObj< SimpleMapObj >,
            "ForestHomeGate",
        },
        {
            "RGTowerPressGroundA",
            createNameObj< RailMoveObjPress >,
            "RGTowerPressGroundA",
        },
        {
            "WeatherVane",
            createNameObj< SimpleMapObj >,
            "WeatherVane",
        },
        {
            "RainCloud",
            createNameObj< RainCloud >,
            "RainCloud",
        },
        {
            "PowerStarKeeperA",
            createNameObj< RotateMoveObj >,
            "PowerStarKeeperA",
        },
        {
            "PowerStarKeeperB",
            createNameObj< RotateMoveObj >,
            "PowerStarKeeperB",
        },
        {
            "PowerStarKeeperC",
            createNameObj< RotateMoveObj >,
            "PowerStarKeeperC",
        },
        {
            "ForestPoihanaFenceA",
            createNameObj< SimpleMapObj >,
            "ForestPoihanaFenceA",
        },
        {
            "ForestPoihanaFenceB",
            createNameObj< SimpleMapObj >,
            "ForestPoihanaFenceB",
        },
        {
            "TeresaMansionBridgeA",
            createNameObj< SimpleMapObj >,
            "TeresaMansionBridgeA",
        },
        {
            "TeresaMansionBridgeB",
            createNameObj< SimpleMapObj >,
            "TeresaMansionBridgeB",
        },
        {
            "DarknessRoomDoorA",
            createNameObj< DarknessRoomDoor >,
            "DarknessRoomDoorA",
        },
        {
            "TeresaMansionDoorA",
            createNameObj< TypicalDoor >,
            "TeresaMansionDoorA",
        },
        {
            "TeresaMansionDoorB",
            createNameObj< TypicalDoor >,
            "TeresaMansionDoorB",
        },
        {
            "TeresaMansionEntranceDoor",
            createNameObj< TypicalDoorOpen >,
            "TeresaMansionEntranceDoor",
        },
        {
            "WaterBazookaTowerMoveStepA",
            createNameObj< RotateMoveObj >,
            "WaterBazookaTowerMoveStepA",
        },
        {
            "WaterBazookaTowerMoveStepB",
            createNameObj< RailMoveObj >,
            "WaterBazookaTowerMoveStepB",
        },
        {
            "WaterBazookaTowerMoveStepC",
            createNameObj< RailMoveObj >,
            "WaterBazookaTowerMoveStepC",
        },
        {
            "QuestionBoxMoveStepA",
            createNameObj< QuestionBoxGalleryObj >,
            "QuestionBoxMoveStepA",
        },
        {
            "QuestionBoxMoveStepB",
            createNameObj< QuestionBoxGalleryObj >,
            "QuestionBoxMoveStepB",
        },
        {
            "QuestionBoxMoveStepC",
            createNameObj< QuestionBoxGalleryObj >,
            "QuestionBoxMoveStepC",
        },
        {
            "ForestHomeBridge",
            createNameObj< SimpleMapObj >,
            "ForestHomeBridge",
        },
        {
            "ForestBarricadeRockA",
            createNameObj< SimpleMapObj >,
            "ForestBarricadeRockA",
        },
        {
            "ReverseGravityRoomPressGroundA",
            createNameObj< RailMoveObjPress >,
            "ReverseGravityRoomPressGroundA",
        },
        {
            "ReverseGravityRoomPressGroundB",
            createNameObj< RailMoveObjPress >,
            "ReverseGravityRoomPressGroundB",
        },
        {
            "ReverseGravityRoomPressGroundC",
            createNameObj< RailMoveObjPress >,
            "ReverseGravityRoomPressGroundC",
        },
        {
            "ReverseGravityRoomPressGroundD",
            createNameObj< RailMoveObjPress >,
            "ReverseGravityRoomPressGroundD",
        },
        {
            "ReverseGravityRoomPressGroundE",
            createNameObj< RailMoveObjPress >,
            "ReverseGravityRoomPressGroundE",
        },
        {
            "RollingOvalPlanetParts",
            createNameObj< RotateMoveObj >,
            "RollingOvalPlanetParts",
        },
        {
            "TeresaMansionLightA",
            createNameObj< SwingLight >,
            "TeresaMansionLightA",
        },
        {
            "TeresaMansionLightB",
            createNameObj< SwingLight >,
            "TeresaMansionLightB",
        },
        {
            "TeresaMansionCandlestand",
            createNameObj< Candlestand >,
            "TeresaMansionCandlestand",
        },
        {
            "TeresaMansionHoleCover",
            createNameObj< SimpleBreakableObj >,
            "TeresaMansionHoleCover",
        },
        {
            "KoopaStatue",
            createNameObj< SimpleBreakableObj >,
            "KoopaStatue",
        },
        {
            "TeresaPicture",
            createNameObj< SimpleTextureSwitchChangeObj >,
            "TeresaPicture",
        },
        {
            "KinokoOneUpPicture",
            createNameObj< SimpleTextureSwitchChangeObj >,
            "KinokoOneUpPicture",
        },
        {
            "FireBar",
            createNameObj< FireBar >,
            "FireBarCore",
        },
        {
            "BattleShipMovePartsA",
            createNameObj< RotateMoveObj >,
            "BattleShipMovePartsA",
        },
        {
            "BattleShipMovePartsB",
            createNameObj< RotateMoveObj >,
            "BattleShipMovePartsB",
        },
        {
            "BattleShipElevator",
            createNameObj< BattleShipElevator >,
            "BattleShipElevator",
        },
        {
            "BattleShipElevatorCover",
            createNameObj< SimpleMapObj >,
            "BattleShipElevatorCover",
        },
        {
            "TeresaRaceSpaceStickA",
            createNameObj< SimpleMapObj >,
            "TeresaRaceSpaceStickA",
        },
        {
            "TeresaRaceSpaceStickB",
            createNameObj< SimpleMapObj >,
            "TeresaRaceSpaceStickB",
        },
        {
            "TeresaRaceSpaceStickC",
            createNameObj< SimpleMapObj >,
            "TeresaRaceSpaceStickC",
        },
        {
            "TeresaRacePartsA",
            createNameObj< RotateMoveObj >,
            "TeresaRacePartsA",
        },
        {
            "TeresaRacePartsB",
            createNameObj< AnmModelBindMove >,
            "TeresaRacePartsB",
        },
        {
            "NeedlePlant",
            createNameObj< NeedlePlant >,
            "NeedlePlant",
        },
        {
            "NeedlePlantFly",
            createNameObj< NeedlePlant >,
            "NeedlePlantFly",
        },
        {
            "BeeWallClimbPartsA",
            createNameObj< RailMoveObj >,
            "BeeWallClimbPartsA",
        },
        {
            "PeachCastleTownAfterAttack",
            createNameObj< SimpleMapObj >,
            "PeachCastleTownAfterAttack",
        },
        {
            "PeachCastleTownBeforeAttack",
            createNameObj< SimpleMapObj >,
            "PeachCastleTownBeforeAttack",
        },
        {
            "PeachCastleTownGate",
            createNameObj< SimpleMapObj >,
            "PeachCastleTownGate",
        },
        {
            "PhantomDecoratePartsA",
            createNameObj< SimpleEnvironmentObj >,
            "PhantomDecoratePartsA",
        },
        {
            "PhantomDecoratePartsB",
            createNameObj< SimpleEnvironmentObj >,
            "PhantomDecoratePartsB",
        },
        {
            "PhantomDecoratePartsHole",
            createNameObj< SimpleEnvironmentObj >,
            "PhantomDecoratePartsHole",
        },
        {
            "CocoonStepA",
            createNameObj< SimpleMapObj >,
            "CocoonStepA",
        },
        {
            "CocoonStepB",
            createNameObj< SimpleMapObj >,
            "CocoonStepB",
        },
        {
            "SpaceCannonLauncher",
            createNameObj< SimpleMapObj >,
            "SpaceCannonLauncher",
        },
        {
            "TrapBaseA",
            createNameObj< SimpleMapObj >,
            "TrapBaseA",
        },
        {
            "SweetsDecoratePartsSpoon",
            createNameObj< RotateMoveObj >,
            "SweetsDecoratePartsSpoon",
        },
        {
            "SweetsDecoratePartsFork",
            createNameObj< RotateMoveObj >,
            "SweetsDecoratePartsFork",
        },
        {
            "SeesawMoveNutA",
            createNameObj< SeesawMoveNut >,
            "SeesawMoveNutA",
        },
        {
            "SeesawMoveNutB",
            createNameObj< SeesawMoveNut >,
            "SeesawMoveNutB",
        },
        {
            "SeesawMoveNutC",
            createNameObj< SeesawMoveNut >,
            "SeesawMoveNutC",
        },
        {
            "TwisterSan",
            createNameObj< Sandstorm >,
            "TwisterSan",
        },
        {
            "TwisterSanSlim",
            createNameObj< Sandstorm >,
            "TwisterSanSlim",
        },
        {
            "SunakazeKun",
            createNameObj< Sandstorm >,
            "SunakazeKun",
        },
        {
            "SandStreamMoveStepsA",
            createNameObj< RotateMoveObj >,
            "SandStreamMoveStepsA",
        },
        {
            "SandStreamMoveStepsB",
            createNameObj< RotateMoveObj >,
            "SandStreamMoveStepsB",
        },
        {
            "WaterLeakPipe",
            createNameObj< WaterLeakPipe >,
            "WaterLeakPipe",
        },
        {
            "BroadBeanMoveStepA",
            createNameObj< RailMoveObj >,
            "BroadBeanMoveStepA",
        },
        {
            "BroadBeanMoveStepB",
            createNameObj< RailMoveObj >,
            "BroadBeanMoveStepB",
        },
        {
            "ColorPencil",
            createNameObj< SimpleMapObj >,
            "ColorPencil",
        },
        {
            "TeresaRacePartsBallA",
            createNameObj< SimpleMapObj >,
            "TeresaRacePartsBallA",
        },
        {
            "SandCapsulePressGround",
            createNameObj< SandCapsulePressGround >,
            "SandCapsulePressGround",
        },
        {
            "RayGunPlanetPartsScrew",
            createNameObj< RailRotateMoveObj >,
            "RayGunPlanetPartsScrew",
        },
        {
            "RayGunPlanetPartsGear",
            createNameObj< RotateMoveObj >,
            "RayGunPlanetPartsGear",
        },
        {
            "BreakDownFixStepA",
            createNameObj< SimpleMapObj >,
            "BreakDownFixStepA",
        },
        {
            "SandStreamHighTowerMoveStepA",
            createNameObj< RailMoveObj >,
            "SandStreamHighTowerMoveStepA",
        },
        {
            "DriftWood",
            createNameObj< DriftWood >,
            "DriftWood",
        },
        {
            "ToyFactoryDecoratePartsGearA",
            createNameObj< RotateMoveObj >,
            "ToyFactoryDecoratePartsGearA",
        },
        {
            "MiniMechaKoopaPartsMoveStepA",
            createNameObj< RailMoveObj >,
            "MiniMechaKoopaPartsMoveStepA",
        },
        {
            "MiniMechaKoopaPartsGear",
            createNameObj< RotateMoveObj >,
            "MiniMechaKoopaPartsGear",
        },
        {
            "MiniMechaKoopaPartsCage",
            createNameObj< RotateMoveObj >,
            "MiniMechaKoopaPartsCage",
        },
        {
            "AsteroidBlockRotateStepA",
            createNameObj< RotateMoveObj >,
            "AsteroidBlockRotateStepA",
        },
        {
            "SandUpDownTowerBreakableWallA",
            createNameObj< SimpleBreakableObj >,
            "SandUpDownTowerBreakableWallA",
        },
        {
            "SandUpDownTowerBreakableWallB",
            createNameObj< SimpleBreakableObj >,
            "SandUpDownTowerBreakableWallB",
        },
        {
            "HoleDeathSandMoveStepA",
            createNameObj< RailMoveObj >,
            "HoleDeathSandMoveStepA",
        },
        {
            "SandUpDownTowerMoveStepA",
            createNameObj< RailMoveObj >,
            "SandUpDownTowerMoveStepA",
        },
        {
            "DeathSandLandPartsA",
            createNameObj< SimpleMapObj >,
            "DeathSandLandPartsA",
        },
        {
            "DeathSandLandPartsB",
            createNameObj< SimpleMapObj >,
            "DeathSandLandPartsB",
        },
        {
            "DeathSandLandPlatformStepA",
            createNameObj< SimpleMapObj >,
            "DeathSandLandPlatformStepA",
        },
        {
            "DeathSandEnvironmentSapotenA",
            createNameObj< SimpleEnvironmentObj >,
            "DeathSandEnvironmentSapotenA",
        },
        {
            "DeathSandEnvironmentSapotenB",
            createNameObj< SimpleEnvironmentObj >,
            "DeathSandEnvironmentSapotenB",
        },
        {
            "DeathSandEnvironmentRock",
            createNameObj< SimpleEnvironmentObj >,
            "DeathSandEnvironmentRock",
        },
        {
            "DeathSandEnvironmentPyramid",
            createNameObj< SimpleEnvironmentObj >,
            "DeathSandEnvironmentPyramid",
        },
        {
            "UFOSandObstacleA",
            createNameObj< SimpleMapObj >,
            "UFOSandObstacleA",
        },
        {
            "UFOSandObstacleB",
            createNameObj< SimpleMapObj >,
            "UFOSandObstacleB",
        },
        {
            "UFOSandObstacleC",
            createNameObj< SimpleMapObj >,
            "UFOSandObstacleC",
        },
        {
            "SweetDecoratePartsOrange",
            createNameObj< SimpleEnvironmentObj >,
            "SweetDecoratePartsOrange",
        },
        {
            "CapsuleCage",
            createNameObj< CapsuleCage >,
            "CapsuleCage",
        },
        {
            "KameckShipLv1",
            createNameObj< SimpleMapObj >,
            "KameckShipLv1",
        },
        {
            "LavaBallRisingPlanetLava",
            createNameObj< LavaBallRisingPlanetLava >,
            "LavaBallRisingPlanetLava",
        },
        {
            "StrongBlock",
            createNameObj< SimpleMapObj >,
            "StrongBlock",
        },
        {
            "ChoConveyorChocoA",
            createNameObj< SimpleMapObj >,
            "ChoConveyorChocoA",
        },
        {
            "ChoConveyorMoveChocoA",
            createNameObj< RailMoveObj >,
            "ChoConveyorMoveChocoA",
        },
        {
            "HoneyQueenLeafA",
            createNameObj< SimpleSeesawObj >,
            "HoneyQueenLeafA",
        },
        {
            "ForestHomePartsTree",
            createNameObj< SimpleMapObj >,
            "ForestHomePartsTree",
        },
        {
            "ForestHomePartsTreeTower",
            createNameObj< SimpleMapObj >,
            "ForestHomePartsTreeTower",
        },
        {
            "RosettaChair",
            createNameObj< RosettaChair >,
            "RosettaChair",
        },
        {
            "GravityLightA",
            createNameObj< GravityLight >,
            "GravityLightA",
        },
        {
            "GravityLightRoad",
            createNameObj< GravityLightRoad >,
            "GravityLightRoad",
        },
        {
            "PoltaBattlePlanetPartsA",
            createNameObj< SimpleMapObj >,
            "PoltaBattlePlanetPartsA",
        },
        {
            "HeavenlyBeachTimerStepA",
            createNameObj< SimpleTimerObj >,
            "HeavenlyBeachTimerStepA",
        },
        {
            "HeavenlyBeachTimerStepB",
            createNameObj< SimpleTimerObj >,
            "HeavenlyBeachTimerStepB",
        },
        {
            "ReverseKingdomTreeA",
            createNameObj< SimpleMapObj >,
            "ReverseKingdomTreeA",
        },
        {
            "FloaterOtakingFloaterA",
            createNameObj< LavaFloater >,
            "FloaterOtakingFloaterA",
        },
        {
            "WindMillPropeller",
            createNameObj< RotateMoveObj >,
            "WindMillPropeller",
        },
        {
            "WindMillPropellerMini",
            createNameObj< RotateMoveObj >,
            "WindMillPropellerMini",
        },
        {
            "LavaRotateStepsRotatePartsA",
            createNameObj< RotateMoveObj >,
            "LavaRotateStepsRotatePartsA",
        },
        {
            "LavaRotateStepsRotatePartsB",
            createNameObj< RotateMoveObj >,
            "LavaRotateStepsRotatePartsB",
        },
        {
            "LavaRotateStepsRotatePartsC",
            createNameObj< RotateMoveObj >,
            "LavaRotateStepsRotatePartsC",
        },
        {
            "LavaRotateStepsRotatePartsD",
            createNameObj< RotateMoveObj >,
            "LavaRotateStepsRotatePartsD",
        },
        {
            "QuickSand2DMovePartsA",
            createNameObj< RotateMoveObj >,
            "QuickSand2DMovePartsA",
        },
        {
            "BiriBiriBegomanSpikePistonA",
            createNameObj< RailMoveObj >,
            "BiriBiriBegomanSpikePistonA",
        },
        {
            "HugeBattleShipPlanetEntrance",
            createNameObj< SimpleMapObj >,
            "HugeBattleShipPlanetEntrance",
        },
        {
            "MysteryGravityRoomBridgeA",
            createNameObj< SimpleMapObj >,
            "MysteryGravityRoomBridgeA",
        },
        {
            "DeadLeaves",
            createNameObj< DeadLeaves >,
            "DeadLeaves",
        },
        {
            "PrologueDirector",
            createNameObj< PrologueDirector >,
            "DemoLetter",
        },
        {
            "DeathPromenadeMovePartsSpuareA",
            createNameObj< RailMoveObj >,
            "DeathPromenadeMovePartsSpuareA",
        },
        {
            "DeathPromenadeRotateCircleL",
            createNameObj< RotateMoveObj >,
            "DeathPromenadeRotateCircleL",
        },
        {
            "DeathPromenadeRotateCircleS",
            createNameObj< RotateMoveObj >,
            "DeathPromenadeRotateCircleS",
        },
        {
            "FlexibleSandPlanetPartsA",
            createNameObj< FlexibleSphere >,
            "FlexibleSandPlanetPartsA",
        },
        {
            "DarkHopperRotateStepA",
            createNameObj< SimpleClipPartsObj >,
            "DarkHopperRotateStepA",
        },
        {
            "DarkHopperPlanetPartsA",
            createNameObj< SimpleMapObj >,
            "DarkHopperPlanetPartsA",
        },
        {
            "DarkHopperPlanetPartsC",
            createNameObj< SimpleMapObj >,
            "DarkHopperPlanetPartsC",
        },
        {
            "DarkHopperPlanetPartsD",
            createNameObj< SimpleMapObj >,
            "DarkHopperPlanetPartsD",
        },
        {
            "ClockworkHandle",
            createNameObj< SimpleBreakableObj >,
            "ClockworkHandle",
        },
        {
            "MiniMechaKoopaPartsFan",
            createNameObj< SimpleMapObj >,
            "MiniMechaKoopaPartsFan",
        },
        {
            "RockRoadCircleA",
            createNameObj< SimpleMapObj >,
            "RockRoadCircleA",
        },
        {
            "AssemblyBlockPartsA",
            createNameObj< AssemblyBlock >,
            "AssemblyBlockPartsA",
        },
        {
            "AssemblyBlockPartsB",
            createNameObj< AssemblyBlock >,
            "AssemblyBlockPartsB",
        },
        {
            "AssemblyBlockPartsC",
            createNameObj< AssemblyBlock >,
            "AssemblyBlockPartsC",
        },
        {
            "AssemblyBlockPartsD",
            createNameObj< AssemblyBlock >,
            "AssemblyBlockPartsD",
        },
        {
            "AssemblyBlockPartsE",
            createNameObj< AssemblyBlock >,
            "AssemblyBlockPartsE",
        },
        {
            "AssemblyBlockPartsIceB",
            createNameObj< AssemblyBlock >,
            "AssemblyBlockPartsIceB",
        },
        {
            "AssemblyBlockPartsIceC",
            createNameObj< AssemblyBlock >,
            "AssemblyBlockPartsIceC",
        },
        {
            "AssemblyBlockPartsIceD",
            createNameObj< AssemblyBlock >,
            "AssemblyBlockPartsIceD",
        },
        {
            "AssemblyBlockPartsIceE",
            createNameObj< AssemblyBlock >,
            "AssemblyBlockPartsIceE",
        },
        {
            "AssemblyBlockPartsTimerA",
            createNameObj< AssemblyBlock >,
            "AssemblyBlockPartsTimerA",
        },
        {
            "HellBallGuidePartsA",
            createNameObj< SimpleMapObj >,
            "HellBallGuidePartsA",
        },
        {
            "HellBallRotatePartsA",
            createNameObj< RotateMoveObj >,
            "HellBallRotatePartsA",
        },
        {
            "HellBallRotatePartsB",
            createNameObj< RotateMoveObj >,
            "HellBallRotatePartsB",
        },
        {
            "HellBallRotatePartsC",
            createNameObj< RotateMoveObj >,
            "HellBallRotatePartsC",
        },
        {
            "HellBallRotatePartsD",
            createNameObj< RotateMoveObj >,
            "HellBallRotatePartsD",
        },
        {
            "HellBallRotatePartsE",
            createNameObj< RotateMoveObj >,
            "HellBallRotatePartsE",
        },
        {
            "HellBallRotatePartsF",
            createNameObj< RotateMoveObj >,
            "HellBallRotatePartsF",
        },
        {
            "HellBallRotatePartsG",
            createNameObj< RotateMoveObj >,
            "HellBallRotatePartsG",
        },
        {
            "IceSlipRoad",
            createNameObj< SimpleMapObj >,
            "IceSlipRoad",
        },
        {
            "CandyLiftA",
            createNameObj< RotateMoveObj >,
            "CandyLiftA",
        },
        {
            "CandyLiftB",
            createNameObj< RotateMoveObj >,
            "CandyLiftB",
        },
        {
            "SurfingRaceTutorialParts",
            createNameObj< SimpleMapObj >,
            "SurfingRaceTutorialParts",
        },
        {
            "SurfingRaceMainGate",
            createNameObj< SimpleMapObj >,
            "SurfingRaceMainGate",
        },
        {
            "SurfingRaceSubGate",
            createNameObj< SimpleMapObj >,
            "SurfingRaceSubGate",
        },
        {
            "SurfingRaceStep",
            createNameObj< SimpleMapObj >,
            "SurfingRaceStep",
        },
        {
            "SurfingRaceSignBoard",
            createNameObj< SimpleMapObj >,
            "SurfingRaceSignBoard",
        },
        {
            "SurfingRaceVictoryStand",
            createNameObj< SimpleMapObj >,
            "SurfingRaceVictoryStand",
        },
        {
            "HeavensDoorFlowerA",
            createNameObj< SimpleMapObjNoSilhouetted >,
            "HeavensDoorFlowerA",
        },
        {
            "HeavensDoorHouseDoor",
            createNameObj< SimpleMapObj >,
            "HeavensDoorHouseDoor",
        },
        {
            "HeavensDoorAppearStepAAfter",
            createNameObj< SimpleMapObj >,
            "HeavensDoorAppearStepAAfter",
        },
        {
            "HeavensDoorAppearStepA",
            createNameObj< HeavensDoorDemoObj >,
            "HeavensDoorAppearStepA",
        },
        {
            "HeavensDoorMiddleRotatePartsA",
            createNameObj< RotateMoveObj >,
            "HeavensDoorMiddleRotatePartsA",
        },
        {
            "HeavensDoorMiddleRotatePartsB",
            createNameObj< RotateMoveObj >,
            "HeavensDoorMiddleRotatePartsB",
        },
        {
            "HeavensDoorInsideCage",
            createNameObj< HeavensDoorDemoObj >,
            "HeavensDoorInsideCage",
        },
        {
            "HeavensDoorInsidePlanetPartsA",
            createNameObj< HeavensDoorDemoObj >,
            "HeavensDoorInsidePlanetPartsA",
        },
        {
            "HeavensDoorInsideRotatePartsA",
            createNameObj< RotateMoveObj >,
            "HeavensDoorInsideRotatePartsA",
        },
        {
            "HeavensDoorInsideRotatePartsB",
            createNameObj< RotateMoveObj >,
            "HeavensDoorInsideRotatePartsB",
        },
        {
            "HeavensDoorInsideRotatePartsC",
            createNameObj< RotateMoveObj >,
            "HeavensDoorInsideRotatePartsC",
        },
        {
            "MechaKoopaPartsHead",
            createNameObj< MechaKoopaPartsHead >,
            "MechaKoopaPartsHead",
        },
        {
            "MechaKoopaPartsCollar",
            createNameObj< RotateMoveObj >,
            "MechaKoopaPartsCollar",
        },
        {
            "MechaKoopaPartsBreast",
            createNameObj< RailMoveObjBreakAtEnd >,
            "MechaKoopaPartsBreast",
        },
        {
            "MechaKoopaPartsArmRight",
            createNameObj< MechaKoopaPartsArm >,
            "MechaKoopaPartsArmRight",
        },
        {
            "MechaKoopaPartsArmLeft",
            createNameObj< MechaKoopaPartsArm >,
            "MechaKoopaPartsArmLeft",
        },
        {
            "MechaKoopaPartsBody",
            createNameObj< SimpleMapObj >,
            "MechaKoopaPartsBody",
        },
        {
            "MechaKoopaPartsCaterpillar",
            createNameObj< AnmModelSwitchMove >,
            "MechaKoopaPartsCaterpillar",
        },
        {
            "MechaKoopaPartsRollerA",
            createNameObj< SimpleMapObj >,
            "MechaKoopaPartsRollerA",
        },
        {
            "MechaKoopaPartsMoveStepA",
            createNameObj< RailMoveObjSwitchShadow >,
            "MechaKoopaPartsMoveStepA",
        },
        {
            "MechaKoopaPartsWreckA",
            createNameObj< SimpleMapObj >,
            "MechaKoopaPartsWreckA",
        },
        {
            "CannonFortressBreakStepA",
            createNameObj< CannonFortressBreakStep >,
            "CannonFortressBreakStepA",
        },
        {
            "CannonFortressRotateBreakStepA",
            createNameObj< CannonFortressBreakStep >,
            "CannonFortressRotateBreakStepA",
        },
        {
            "CannonFortressRotateBreakStepB",
            createNameObj< CannonFortressBreakStep >,
            "CannonFortressRotateBreakStepB",
        },
        {
            "CannonFortressRotateBreakStepC",
            createNameObj< CannonFortressBreakStep >,
            "CannonFortressRotateBreakStepC",
        },
        {
            "CannonFortressMoveWall",
            createNameObj< RailMoveObjPress >,
            "CannonFortressMoveWall",
        },
        {
            "SpaceShipStep",
            createNameObj< SpaceShipStep >,
            "SpaceShipStep",
        },
        {
            "QuarterRollGravityRoomArrow",
            createNameObj< QuarterRollGravityRoomArrow >,
            "QuarterRollGravityRoomArrow",
        },
        {
            "NutShipFleetBridge",
            createNameObj< PhantomShipBridge >,
            "NutShipFleetBridge",
        },
        {
            "IceVolcanoMoveStepA",
            createNameObj< RailMoveObj >,
            "IceVolcanoMoveStepA",
        },
        {
            "IceVolcanoBalanceStepA",
            createNameObj< SimpleSeesawObj >,
            "IceVolcanoBalanceStepA",
        },
        {
            "IceVolcanoBalanceStepB",
            createNameObj< SimpleSeesawObj >,
            "IceVolcanoBalanceStepB",
        },
        {
            "IceVolcanoFloaterA",
            createNameObj< LavaFloater >,
            "IceVolcanoFloaterA",
        },
        {
            "IceVolcanoFloaterB",
            createNameObj< LavaFloater >,
            "IceVolcanoFloaterB",
        },
        {
            "IceVolcanoAppearStepA",
            createNameObj< AnmModelSwitchMove >,
            "IceVolcanoAppearStepA",
        },
        {
            "IceVolcanoAppearStepB",
            createNameObj< AnmModelSwitchMove >,
            "IceVolcanoAppearStepB",
        },
        {
            "IceVolcanoClimbingWall",
            createNameObj< AnmModelSwitchMove >,
            "IceVolcanoClimbingWall",
        },
        {
            "IceMountainFloaterA",
            createNameObj< SimpleFloaterObj >,
            "IceMountainFloaterA",
        },
        {
            "IceMountainSeesawNoSlipA",
            createNameObj< SimpleSeesawObj >,
            "IceMountainSeesawNoSlipA",
        },
        {
            "IceMountainSeesawSlipA",
            createNameObj< SimpleSeesawObj >,
            "IceMountainSeesawSlipA",
        },
        {
            "IceMountainSeesawSlipB",
            createNameObj< SimpleSeesawObj >,
            "IceMountainSeesawSlipB",
        },
        {
            "IceMountainSeesawSlipC",
            createNameObj< SimpleSeesawObj >,
            "IceMountainSeesawSlipC",
        },
        {
            "IceMountainSeesawSlipD",
            createNameObj< SimpleSeesawObj >,
            "IceMountainSeesawSlipD",
        },
        {
            "IceRingBumpyPartsA",
            createNameObj< SimpleMapObj >,
            "IceRingBumpyPartsA",
        },
        {
            "IceWaterUpDownParts",
            createNameObj< IceVolcanoUpDownPlane >,
            "IceWaterUpDownParts",
        },
        {
            "LavaUpDownParts",
            createNameObj< IceVolcanoUpDownPlane >,
            "LavaUpDownParts",
        },
        {
            "IceUpDownShutterDoorA",
            createNameObj< AnmModelSwitchMove >,
            "IceUpDownShutterDoorA",
        },
        {
            "IceLavaIslandSnowStepA",
            createNameObj< SimpleMapObj >,
            "IceLavaIslandSnowStepA",
        },
        {
            "IceLavaIslandIceMovableStepA",
            createNameObj< RailMoveObj >,
            "IceLavaIslandIceMovableStepA",
        },
        {
            "IceLavaIslandLavaMovableStepA",
            createNameObj< RailMoveObj >,
            "IceLavaIslandLavaMovableStepA",
        },
        {
            "HoleBeltConveyerPartsA",
            createNameObj< RailMoveObj >,
            "HoleBeltConveyerPartsA",
        },
        {
            "HoleBeltConveyerPartsB",
            createNameObj< RailMoveObj >,
            "HoleBeltConveyerPartsB",
        },
        {
            "HoleBeltConveyerPartsC",
            createNameObj< RailMoveObj >,
            "HoleBeltConveyerPartsC",
        },
        {
            "HoleBeltConveyerPartsD",
            createNameObj< RailMoveObj >,
            "HoleBeltConveyerPartsD",
        },
        {
            "HoleBeltConveyerPartsE",
            createNameObj< RailMoveObj >,
            "HoleBeltConveyerPartsE",
        },
        {
            "HoleBeltConveyerPartsF",
            createNameObj< RailMoveObj >,
            "HoleBeltConveyerPartsF",
        },
        {
            "HoleBeltConveyerPartsG",
            createNameObj< RotateMoveObj >,
            "HoleBeltConveyerPartsG",
        },
        {
            "HoleBeltConveyerPartsH",
            createNameObj< RailMoveObj >,
            "HoleBeltConveyerPartsH",
        },
        {
            "ChallengeBallAccelCylinderA",
            createNameObj< RotateMoveObj >,
            "ChallengeBallAccelCylinderA",
        },
        {
            "ChallengeBallGearA",
            createNameObj< RotateMoveObj >,
            "ChallengeBallGearA",
        },
        {
            "ChallengeBallMoveGroundA",
            createNameObj< RailMoveObj >,
            "ChallengeBallMoveGroundA",
        },
        {
            "ChallengeBallMoveGroundB",
            createNameObj< RailMoveObj >,
            "ChallengeBallMoveGroundB",
        },
        {
            "ChallengeBallRotateBridgeA",
            createNameObj< RotateMoveObj >,
            "ChallengeBallRotateBridgeA",
        },
        {
            "ChallengeBallVanishingRoadA",
            createNameObj< SimpleMapObj >,
            "ChallengeBallVanishingRoadA",
        },
        {
            "TrialBubbleTimerMoveWallA",
            createNameObj< TimerMoveWall >,
            "TrialBubbleTimerMoveWallA",
        },
        {
            "TrialBubbleRotateWallA",
            createNameObj< RotateMoveObj >,
            "TrialBubbleRotateWallA",
        },
        {
            "TrialBubbleRevolvingPartsA",
            createNameObj< RotateMoveObj >,
            "TrialBubbleRevolvingPartsA",
        },
        {
            "TrialBubbleMoveWallA",
            createNameObj< RailMoveObj >,
            "TrialBubbleMoveWallA",
        },
        {
            "CubeBubbleExHomeStep",
            createNameObj< SimpleMapObj >,
            "CubeBubbleExHomeStep",
        },
        {
            "CubeBubbleExStartStep",
            createNameObj< SimpleMapObj >,
            "CubeBubbleExStartStep",
        },
        {
            "CubeBubbleExRotateWallS",
            createNameObj< RotateMoveObj >,
            "CubeBubbleExRotateWallS",
        },
        {
            "CubeBubbleExRotateWallL",
            createNameObj< RotateMoveObj >,
            "CubeBubbleExRotateWallL",
        },
        {
            "CubeBubbleExPartsA",
            createNameObj< SimpleMapObj >,
            "CubeBubbleExPartsA",
        },
        {
            "UFONormalB",
            createNameObj< UFOBreakable >,
            "UFONormalB",
        },
        {
            "UFONormalD",
            createNameObj< UFOBreakable >,
            "UFONormalD",
        },
        {
            "UFOStrongA",
            createNameObj< UFOSolid >,
            "UFOStrongA",
        },
        {
            "UFOBattleStageC",
            createNameObj< UFOSolid >,
            "UFOBattleStageC",
        },
        {
            "UFOBattleStageD",
            createNameObj< UFOSolid >,
            "UFOBattleStageD",
        },
        {
            "UFOBattleStageE",
            createNameObj< UFOSolid >,
            "UFOBattleStageE",
        },
        {
            "UFOKinoko",
            createNameObj< UFOKinoko >,
            "UFOKinoko",
        },
        {
            "UFOKinokoLanding",
            createNameObj< SimpleMapObj >,
            "UFOKinokoLanding",
        },
        {
            "UFOBlueStarCupsule",
            createNameObj< BlueStarCupsulePlanet >,
            "UFOBlueStarCupsule",
        },
        {
            "KoopaShipA",
            createNameObj< SimpleMapObj >,
            "KoopaShipA",
        },
        {
            "KoopaShipB",
            createNameObj< SimpleMapObj >,
            "KoopaShipB",
        },
        {
            "KoopaShipC",
            createNameObj< SimpleMapObj >,
            "KoopaShipC",
        },
        {
            "KoopaShipD",
            createNameObj< SimpleMapObj >,
            "KoopaShipD",
        },
        {
            "KoopaShipE",
            createNameObj< SimpleMapObj >,
            "KoopaShipE",
        },
        {
            "KoopaShipSeesawStep",
            createNameObj< SimpleSeesawObj >,
            "KoopaShipSeesawStep",
        },
        {
            "KoopaJrSmallShipA",
            createNameObj< RailMoveObj >,
            "KoopaJrSmallShipA",
        },
        {
            "KoopaJrSmallShipAGuidePoint",
            createNameObj< SimpleMapObj >,
            "KoopaJrSmallShipAGuidePoint",
        },
        {
            "KoopaJrKillerShipA",
            createNameObj< SimpleMapObj >,
            "KoopaJrKillerShipA",
        },
        {
            "KoopaJrNormalShipA",
            createNameObj< SimpleMapObj >,
            "KoopaJrNormalShipA",
        },
        {
            "WaterRoadCaveStepA",
            createNameObj< RailMoveObj >,
            "WaterRoadCaveStepA",
        },
        {
            "WaterRoadCaveStepB",
            createNameObj< SimpleMapObj >,
            "WaterRoadCaveStepB",
        },
        {
            "WaterRoadCaveRotateGround",
            createNameObj< RotateMoveObj >,
            "WaterRoadCaveRotateGround",
        },
        {
            "SubmarineVolcanoInside",
            createNameObj< SimpleMapObj >,
            "SubmarineVolcanoInside",
        },
        {
            "SubmarineVolcanoBigColumn",
            createNameObj< SubmarineVolcanoBigColumn >,
            "SubmarineVolcanoBigColumn",
        },
        {
            "SubmarineVolcanoSmallColumn",
            createNameObj< SubmarineVolcanoBigColumn >,
            "SubmarineVolcanoSmallColumn",
        },
        {
            "OnimasuPlanetPartsGoal",
            createNameObj< SimpleMapObj >,
            "OnimasuPlanetPartsGoal",
        },
        {
            "OnimasuPlanetObstaclePartsA",
            createNameObj< SimpleMapObj >,
            "OnimasuPlanetObstaclePartsA",
        },
        {
            "OnimasuPlanetRailMovePartsA",
            createNameObj< RailMoveObj >,
            "OnimasuPlanetRailMovePartsA",
        },
        {
            "OnimasuPlanetRotatePartsA",
            createNameObj< RotateMoveObj >,
            "OnimasuPlanetRotatePartsA",
        },
        {
            "OnimasuPlanetRotatePartsB",
            createNameObj< RotateMoveObj >,
            "OnimasuPlanetRotatePartsB",
        },
        {
            "PhantomShipPropellerSmall",
            createNameObj< AnmModelSwitchMove >,
            "PhantomShipPropellerSmall",
        },
        {
            "PhantomShipPropellerBig",
            createNameObj< AnmModelSwitchMove >,
            "PhantomShipPropellerBig",
        },
        {
            "PhantomShipHandle",
            createNameObj< PhantomShipHandle >,
            "PhantomShipHandle",
        },
        {
            "PhantomShipBridge",
            createNameObj< PhantomShipBridge >,
            "PhantomShipBridge",
        },
        {
            "TakoBarrelA",
            createNameObj< SimpleBreakableObj >,
            "TakoBarrelA",
        },
        {
            "TakoBarrelB",
            createNameObj< SimpleMapObj >,
            "TakoBarrelB",
        },
        {
            "KoopaRestarterVs3",
            createNameObj< KoopaRestarterVs3 >,
            nullptr,
        },
        {
            "KoopaPowerUpSwitch",
            createNameObj< KoopaPowerUpSwitch >,
            nullptr,
        },
        {
            "KoopaSwitchKeeper",
            createNameObj< KoopaSwitchKeeper >,
            nullptr,
        },
        {
            "KoopaViewSwitchKeeper",
            createNameObj< KoopaViewSwitchKeeper >,
            nullptr,
        },
        {
            "KoopaBattleMapStair",
            createNameObj< KoopaBattleMapStair >,
            "KoopaBattleMapStair",
        },
        {
            "KoopaBattleMapPlate",
            createNameObj< KoopaBattleMapPlate >,
            "KoopaPlate",
        },
        {
            "KoopaBattleMapCoinPlate",
            createNameObj< KoopaBattleMapCoinPlate >,
            "KoopaPlateCoin",
        },
        {
            "KoopaBattleMapStairBig",
            createNameObj< KoopaBattleMapStair >,
            "KoopaBattleMapStairBig",
        },
        {
            "KoopaBattleMapStairTurn",
            createNameObj< KoopaBattleMapStair >,
            "KoopaBattleMapStairTurn",
        },
        {
            "KoopaBattleMapStairturnAppear",
            createNameObj< TimeAppearObj >,
            "KoopaBattleMapStairTurn",
        },
        {
            "KoopaVS1PartsSpiralRoad",
            createNameObj< SimpleMapObj >,
            "KoopaVS1PartsSpiralRoad",
        },
        {
            "KoopaVS1PartsMoveStepA",
            createNameObj< RailMoveObj >,
            "KoopaVS1PartsMoveStepA",
        },
        {
            "KoopaVS1PartsMoveStepB",
            createNameObj< RailMoveObj >,
            "KoopaVS1PartsMoveStepB",
        },
        {
            "KoopaVS1PartsRotateMoveStepA",
            createNameObj< RailRotateMoveObj >,
            "KoopaVS1PartsRotateMoveStepA",
        },
        {
            "KoopaVS1PartsRotateMoveStepB",
            createNameObj< RailRotateMoveObj >,
            "KoopaVS1PartsRotateMoveStepB",
        },
        {
            "KoopaVS1PartsReverseGRoad",
            createNameObj< SimpleMapObj >,
            "KoopaVS1PartsReverseGRoad",
        },
        {
            "KoopaVS1PartsStairRoad",
            createNameObj< SimpleMapObj >,
            "KoopaVS1PartsStairRoad",
        },
        {
            "KoopaVS1PartsBattleStage",
            createNameObj< SimpleMapObj >,
            "KoopaVS1PartsBattleStage",
        },
        {
            "KoopaVS2PartsReverseGRoadA",
            createNameObj< SimpleMapObj >,
            "KoopaVS2PartsReverseGRoadA",
        },
        {
            "KoopaVS2PartsReverseGRoadB",
            createNameObj< SimpleMapObj >,
            "KoopaVS2PartsReverseGRoadB",
        },
        {
            "KoopaVS2PartsStartRestStep",
            createNameObj< SimpleMapObj >,
            "KoopaVS2PartsStartRestStep",
        },
        {
            "KoopaVS2PartsRestStepA",
            createNameObj< SimpleMapObj >,
            "KoopaVS2PartsRestStepA",
        },
        {
            "KoopaVS2PartsRestStepB",
            createNameObj< SimpleMapObj >,
            "KoopaVS2PartsRestStepB",
        },
        {
            "KoopaVS2PartsRestStepC",
            createNameObj< SimpleMapObj >,
            "KoopaVS2PartsRestStepC",
        },
        {
            "KoopaVS2PartsRestClipC",
            createNameObj< SimpleClipPartsObj >,
            "KoopaVS2PartsRestClipC",
        },
        {
            "KoopaVS2PartsRestStepD",
            createNameObj< SimpleMapObj >,
            "KoopaVS2PartsRestStepD",
        },
        {
            "KoopaVS2PartsRestStepE",
            createNameObj< SimpleMapObj >,
            "KoopaVS2PartsRestStepE",
        },
        {
            "KoopaVS2PartsRestStepF",
            createNameObj< SimpleMapObj >,
            "KoopaVS2PartsRestStepF",
        },
        {
            "KoopaVS2PartsRestStepG",
            createNameObj< SimpleMapObj >,
            "KoopaVS2PartsRestStepG",
        },
        {
            "KoopaVS2PartsDarkMatterA",
            createNameObj< SimpleMapObj >,
            "KoopaVS2PartsDarkMatterA",
        },
        {
            "KoopaVS2PartsDarkMatterB",
            createNameObj< SimpleMapObj >,
            "KoopaVS2PartsDarkMatterB",
        },
        {
            "KoopaVS2PartsDarkMatterC",
            createNameObj< SimpleMapObj >,
            "KoopaVS2PartsDarkMatterC",
        },
        {
            "KoopaVS2PartsDarkMatterD",
            createNameObj< SimpleMapObj >,
            "KoopaVS2PartsDarkMatterD",
        },
        {
            "KoopaVS2PartsDarkMatterE",
            createNameObj< SimpleMapObj >,
            "KoopaVS2PartsDarkMatterE",
        },
        {
            "KoopaVS2PartsStartMoveStepA",
            createNameObj< RotateMoveObj >,
            "KoopaVS2PartsStartMoveStepA",
        },
        {
            "KoopaVS2PartsStartMoveStepB",
            createNameObj< RotateMoveObj >,
            "KoopaVS2PartsStartMoveStepB",
        },
        {
            "KoopaVS2PartsStartMoveStepC",
            createNameObj< RailMoveObj >,
            "KoopaVS2PartsStartMoveStepC",
        },
        {
            "KoopaVS2PartsRollingStep",
            createNameObj< RotateMoveObj >,
            "KoopaVS2PartsRollingStep",
        },
        {
            "KoopaVS2Parts2DMoveStepBarA",
            createNameObj< RailMoveObj >,
            "KoopaVS2Parts2DMoveStepBarA",
        },
        {
            "KoopaVS2Parts2DMoveStepBarB",
            createNameObj< RailMoveObj >,
            "KoopaVS2Parts2DMoveStepBarB",
        },
        {
            "KoopaVS2Parts2DMoveStepSBarB",
            createNameObj< RailMoveObj >,
            "KoopaVS2Parts2DMoveStepSBarB",
        },
        {
            "KoopaVS2Parts2DMoveStepConvex",
            createNameObj< RailMoveObjPress >,
            "KoopaVS2Parts2DMoveStepConvex",
        },
        {
            "KoopaVS2Parts2DMoveStepLShape",
            createNameObj< RailMoveObj >,
            "KoopaVS2Parts2DMoveStepLShape",
        },
        {
            "KoopaVS2Parts2DMoveStepCross",
            createNameObj< RailMoveObj >,
            "KoopaVS2Parts2DMoveStepCross",
        },
        {
            "KoopaVS2PartsJoinedMoveStep",
            createNameObj< RailMoveObj >,
            "KoopaVS2PartsJoinedMoveStep",
        },
        {
            "KoopaVS2PartsJoinedMoveClip",
            createNameObj< SimpleClipPartsObj >,
            "KoopaVS2PartsJoinedMoveClip",
        },
        {
            "KoopaVS2PartsSquareMoveStepA",
            createNameObj< RailMoveObj >,
            "KoopaVS2PartsSquareMoveStepA",
        },
        {
            "KoopaVS2PartsSquareMoveClipA",
            createNameObj< SimpleClipPartsObj >,
            "KoopaVS2PartsSquareMoveClipA",
        },
        {
            "KoopaVS2PartsSquareMoveStepB",
            createNameObj< RailMoveObj >,
            "KoopaVS2PartsSquareMoveStepB",
        },
        {
            "KoopaVS2PartsSquareMoveClipB",
            createNameObj< SimpleClipPartsObj >,
            "KoopaVS2PartsSquareMoveClipB",
        },
        {
            "KoopaVS2PartsClipAreaDisplayA",
            createNameObj< RailMoveIndirectObj >,
            "KoopaVS2PartsClipAreaDisplayA",
        },
        {
            "KoopaVS2PartsStairBig",
            createNameObj< SimpleMapObj >,
            "KoopaVS2PartsStairBig",
        },
        {
            "KoopaVS2Parts2DRailGuideA",
            createNameObj< SimpleMapObj >,
            "KoopaVS2Parts2DRailGuideA",
        },
        {
            "KoopaVS3RotateStepA",
            createNameObj< RotateMoveObj >,
            "KoopaVS3RotateStepA",
        },
        {
            "KoopaVS3RotateStepB",
            createNameObj< RotateMoveObj >,
            "KoopaVS3RotateStepB",
        },
        {
            "KoopaVS3RotateStepD",
            createNameObj< RotateMoveObj >,
            "KoopaVS3RotateStepD",
        },
        {
            "KoopaVS3RotateStepsA",
            createNameObj< RotateMoveObj >,
            "KoopaVS3RotateStepsA",
        },
        {
            "KoopaVS3Parts2DWallA",
            createNameObj< SimpleMapObj >,
            "KoopaVS3Parts2DWallA",
        },
        {
            "TreasureBoxCrackedEmpty",
            createNameObj< TreasureBoxCracked >,
            "TreasureBoxCracked",
        },
        {
            "TreasureBoxCrackedCoin",
            createNameObj< TreasureBoxCracked >,
            "TreasureBoxCracked",
        },
        {
            "TreasureBoxCrackedYellowChip",
            createNameObj< TreasureBoxCracked >,
            "TreasureBoxCracked",
        },
        {
            "TreasureBoxCrackedBlueChip",
            createNameObj< TreasureBoxCracked >,
            "TreasureBoxCracked",
        },
        {
            "TreasureBoxCrackedKinokoOneUp",
            createNameObj< TreasureBoxCracked >,
            "TreasureBoxCracked",
        },
        {
            "TreasureBoxCrackedKinokoLifeUp",
            createNameObj< TreasureBoxCracked >,
            "TreasureBoxCracked",
        },
        {
            "TreasureBoxCrackedAirBubble",
            createNameObj< TreasureBoxCracked >,
            "TreasureBoxCracked",
        },
        {
            "TreasureBoxCrackedPowerStar",
            createNameObj< TreasureBoxCracked >,
            "TreasureBoxCracked",
        },
        {
            "TreasureBoxEmpty",
            createNameObj< TreasureBoxCracked >,
            "TreasureBox",
        },
        {
            "TreasureBoxCoin",
            createNameObj< TreasureBoxCracked >,
            "TreasureBox",
        },
        {
            "TreasureBoxYellowChip",
            createNameObj< TreasureBoxCracked >,
            "TreasureBox",
        },
        {
            "TreasureBoxBlueChip",
            createNameObj< TreasureBoxCracked >,
            "TreasureBox",
        },
        {
            "TreasureBoxKinokoOneUp",
            createNameObj< TreasureBoxCracked >,
            "TreasureBox",
        },
        {
            "TreasureBoxKinokoLifeUp",
            createNameObj< TreasureBoxCracked >,
            "TreasureBox",
        },
        {
            "TreasureBoxGoldEmpty",
            createNameObj< TreasureBoxCracked >,
            "TreasureBoxGold",
        },
        {
            "OceanRingRuinsMove",
            createNameObj< RailMoveObj >,
            "OceanRingRuinsMove",
        },
        {
            "OceanRingRuinsGearSmall",
            createNameObj< RotateMoveObj >,
            "OceanRingRuinsGearSmall",
        },
        {
            "OceanRingRuinsGearBig",
            createNameObj< RotateMoveObj >,
            "OceanRingRuinsGearBig",
        },
        {
            "OceanRingRuinsColumn",
            createNameObj< SimpleMapObj >,
            "OceanRingRuinsColumn",
        },
        {
            "OceanRingRuinsBase",
            createNameObj< SimpleMapObj >,
            "OceanRingRuinsBase",
        },
        {
            "KameckShip",
            createNameObj< SimpleMapObj >,
            "KameckShip",
        },
        {
            "BeachParasol",
            createNameObj< SimpleMapObj >,
            "BeachParasol",
        },
        {
            "BeachChair",
            createNameObj< SimpleMapObj >,
            "BeachChair",
        },
        {
            "GhostShipCaveMoveGroundA",
            createNameObj< RailMoveObj >,
            "GhostShipCaveMoveGroundA",
        },
        {
            "GhostShipCaveMoveGroundB",
            createNameObj< RailMoveObj >,
            "GhostShipCaveMoveGroundB",
        },
        {
            "PhantomShipBoxFloaterA",
            createNameObj< PhantomShipBoxFloater >,
            "PhantomShipBoxFloaterA",
        },
        {
            "PhantomShipBoxFloaterB",
            createNameObj< PhantomShipBoxFloater >,
            "PhantomShipBoxFloaterB",
        },
        {
            "PhantomShipBoxFloaterC",
            createNameObj< PhantomShipBoxFloater >,
            "PhantomShipBoxFloaterC",
        },
        {
            "PhantomCaveStepA",
            createNameObj< SimpleMapObj >,
            "PhantomCaveStepA",
        },
        {
            "GhostShipCaveClosedRockA",
            createNameObj< SimpleMapObj >,
            "GhostShipCaveClosedRockA",
        },
        {
            "GhostShipBrokenHead",
            createNameObj< SimpleMapObj >,
            "GhostShipBrokenHead",
        },
        {
            "CannonUnderConstructionA",
            createNameObj< SimpleMapObj >,
            "CannonUnderConstructionA",
        },
        {
            "CannonUnderConstructionB",
            createNameObj< SimpleMapObj >,
            "CannonUnderConstructionB",
        },
        {
            "GhostShipCavePipeCollision",
            createNameObj< InvisiblePolygonObj >,
            "GhostShipCavePipeCollision",
        },
        {
            "AstroMapBoard",
            createNameObj< AstroMapBoard >,
            "AstroMapBoard",
        },
        {
            "AstroCore",
            createNameObj< AstroCore >,
            "AstroCore",
        },
        {
            "AstroCountDownPlate",
            createNameObj< AstroCountDownPlate >,
            "AstroCountDownPlate",
        },
        {
            "AstroDome",
            createNameObj< AstroDome >,
            nullptr,
        },
        {
            "AstroDomeEntrance",
            createNameObj< AstroMapObj >,
            nullptr,
        },
        {
            "AstroStarPlate",
            createNameObj< AstroMapObj >,
            nullptr,
        },
        {
            "AstroBaseA",
            createNameObj< AstroMapObj >,
            "AstroBaseA",
        },
        {
            "AstroBaseB",
            createNameObj< AstroMapObj >,
            "AstroBaseB",
        },
        {
            "AstroBaseC",
            createNameObj< AstroMapObj >,
            "AstroBaseC",
        },
        {
            "AstroBaseKitchen",
            createNameObj< AstroMapObj >,
            "AstroBaseKitchen",
        },
        {
            "AstroBaseCenterA",
            createNameObj< AstroMapObj >,
            "AstroBaseCenterA",
        },
        {
            "AstroBaseCenterB",
            createNameObj< AstroMapObj >,
            "AstroBaseCenterB",
        },
        {
            "AstroBaseCenterTop",
            createNameObj< AstroMapObj >,
            "AstroBaseCenterTop",
        },
        {
            "AstroRotateStepA",
            createNameObj< AstroMapObj >,
            "AstroRotateStepA",
        },
        {
            "AstroRotateStepB",
            createNameObj< AstroMapObj >,
            "AstroRotateStepB",
        },
        {
            "AstroDecoratePartsA",
            createNameObj< AstroMapObj >,
            "AstroDecoratePartsA",
        },
        {
            "AstroDecoratePartsGearA",
            createNameObj< AstroMapObj >,
            "AstroDecoratePartsGearA",
        },
        {
            "AstroChildRoom",
            createNameObj< AstroMapObj >,
            "AstroChildRoom",
        },
        {
            "AstroParking",
            createNameObj< AstroMapObj >,
            "AstroParking",
        },
        {
            "AstroLibrary",
            createNameObj< AstroMapObj >,
            "AstroLibrary",
        },
        {
            "AstroRoomLibrary",
            createNameObj< SimpleMapObj >,
            "AstroRoomLibrary",
        },
        {
            "AstroTorchLightRed",
            createNameObj< AstroEffectObj >,
            nullptr,
        },
        {
            "AstroTorchLightBlue",
            createNameObj< AstroEffectObj >,
            nullptr,
        },
        {
            "EffectTicoS",
            createNameObj< AstroEffectObj >,
            nullptr,
        },
        {
            "EffectTicoL",
            createNameObj< AstroEffectObj >,
            nullptr,
        },
        {
            "UFOKinokoUnderConstruction",
            createNameObj< UFOKinokoUnderConstruction >,
            nullptr,
        },
        {
            "UFOKinokoLandingAstro",
            createNameObj< SimpleMapObj >,
            "UFOKinokoLandingAstro",
        },
        {
            "AstroDomeSky",
            createNameObj< AstroDomeSky >,
            nullptr,
        },
        {
            "WhiteRoom",
            createNameObj< SimpleMapObj >,
            "WhiteRoom",
        },
        {
            "VROrbit",
            createNameObj< ProjectionMapSky >,
            "VROrbit",
        },
        {
            "VRDarkSpace",
            createNameObj< Sky >,
            "VRDarkSpace",
        },
        {
            "VRSandwichSun",
            createNameObj< ProjectionMapSky >,
            "VRSandwichSun",
        },
        {
            "SummerSky",
            createNameObj< Sky >,
            "SummerSky",
        },
        {
            "GalaxySky",
            createNameObj< Sky >,
            "GalaxySky",
        },
        {
            "MilkyWaySky",
            createNameObj< Sky >,
            "MilkyWaySky",
        },
        {
            "HalfGalaxySky",
            createNameObj< ProjectionMapSky >,
            "HalfGalaxySky",
        },
        {
            "GreenPlanetOrbitSky",
            createNameObj< ProjectionMapSky >,
            "GreenPlanetOrbitSky",
        },
        {
            "PhantomSky",
            createNameObj< Sky >,
            "PhantomSky",
        },
        {
            "KoopaVS1Sky",
            createNameObj< ProjectionMapSky >,
            "KoopaVS1Sky",
        },
        {
            "KoopaVS2Sky",
            createNameObj< Sky >,
            "KoopaVS2Sky",
        },
        {
            "FamicomMarioSky",
            createNameObj< Sky >,
            "FamicomMarioSky",
        },
        {
            "DesertSky",
            createNameObj< Sky >,
            "DesertSky",
        },
        {
            "ChildRoomSky",
            createNameObj< Sky >,
            "ChildRoomSky",
        },
        {
            "AuroraSky",
            createNameObj< Sky >,
            "AuroraSky",
        },
        {
            "CloudSky",
            createNameObj< ProjectionMapSky >,
            "CloudSky",
        },
        {
            "RockPlanetOrbitSky",
            createNameObj< ProjectionMapSky >,
            "RockPlanetOrbitSky",
        },
        {
            "StarrySky",
            createNameObj< Sky >,
            "StarrySky",
        },
        {
            "AstroDomeSkyA",
            createNameObj< Sky >,
            "AstroDomeSkyA",
        },
        {
            "HomeAir",
            createNameObj< Air >,
            "HomeAir",
        },
        {
            "SphereAir",
            createNameObj< PriorDrawAir >,
            "SphereAir",
        },
        {
            "SunsetAir",
            createNameObj< ProjectionMapAir >,
            "SunsetAir",
        },
        {
            "FineAir",
            createNameObj< ProjectionMapAir >,
            "FineAir",
        },
        {
            "DimensionAir",
            createNameObj< AirFar100m >,
            "DimensionAir",
        },
        {
            "DarknessRoomAir",
            createNameObj< Air >,
            "DarknessRoomAir",
        },
        {
            "TwilightAir",
            createNameObj< Air >,
            "TwilightAir",
        },
        {
            "ZoneHalo",
            createNameObj< Halo >,
            "ZoneHalo",
        },
        {
            "PowerStarHalo",
            createNameObj< PowerStarHalo >,
            "PowerStarHalo",
        },
        {
            "Coin",
            MR::createDirectSetCoin,
            "Coin",
        },
        {
            "PurpleCoin",
            MR::createDirectSetPurpleCoin,
            "PurpleCoin",
        },
        {
            "RailCoin",
            MR::createRailCoin,
            nullptr,
        },
        {
            "CircleCoinGroup",
            MR::createCircleCoinGroup,
            nullptr,
        },
        {
            "PurpleRailCoin",
            MR::createRailPurpleCoin,
            "PurpleCoin",
        },
        {
            "CirclePurpleCoinGroup",
            MR::createCirclePurpleCoinGroup,
            "PurpleCoin",
        },
        {
            "PurpleCoinStarter",
            createNameObj< PurpleCoinStarter >,
            nullptr,
        },
        {
            "CoinReplica",
            createNameObj< CoinReplica >,
            nullptr,
        },
        {
            "QuestionCoin",
            createNameObj< QuestionCoin >,
            "QuestionCoin",
        },
        {
            "BlueChip",
            createNameObj< BlueChip >,
            "BlueChip",
        },
        {
            "YellowChip",
            createNameObj< YellowChip >,
            "YellowChip",
        },
        {
            "BlueChipGroup",
            createNameObj< BlueChipGroup >,
            nullptr,
        },
        {
            "YellowChipGroup",
            createNameObj< YellowChipGroup >,
            nullptr,
        },
        {
            "StarPiece",
            createNameObj< StarPiece >,
            "StarPiece",
        },
        {
            "StarPieceFlow",
            createNameObj< StarPieceGroup >,
            nullptr,
        },
        {
            "StarPieceGroup",
            createNameObj< StarPieceGroup >,
            nullptr,
        },
        {
            "StarPieceFollowGroup",
            createNameObj< StarPieceFollowGroup >,
            nullptr,
        },
        {
            "StarPieceMother",
            createNameObj< StarPieceMother >,
            "StarPieceMother",
        },
        {
            "StarPieceSpot",
            createNameObj< StarPieceSpot >,
            nullptr,
        },
        {
            "OceanBowl",
            createNameObj< OceanBowl >,
            "WaterWave",
        },
        {
            "OceanRing",
            createNameObj< OceanRing >,
            "OceanRing",
        },
        {
            "OceanRingAndFlag",
            createNameObj< OceanRing >,
            "OceanRing",
        },
        {
            "OceanSphere",
            createNameObj< OceanSphere >,
            "WaterWave",
        },
        {
            "WhirlPool",
            createNameObj< WhirlPool >,
            nullptr,
        },
        {
            "WhirlPoolAccelerator",
            createNameObj< WhirlPoolAccelerator >,
            "Whirlpool",
        },
        {
            "FishGroupA",
            createNameObj< FishGroup >,
            "FishA",
        },
        {
            "FishGroupB",
            createNameObj< FishGroup >,
            "FishB",
        },
        {
            "FishGroupC",
            createNameObj< FishGroup >,
            "FishC",
        },
        {
            "FishGroupD",
            createNameObj< FishGroup >,
            "FishD",
        },
        {
            "FishGroupE",
            createNameObj< FishGroup >,
            "FishE",
        },
        {
            "FishGroupF",
            createNameObj< FishGroup >,
            "FishF",
        },
        {
            "Flag",
            createNameObj< Flag >,
            "Flag",
        },
        {
            "FlagKoopaA",
            createNameObj< Flag >,
            "FlagKoopaA",
        },
        {
            "FlagKoopaB",
            createNameObj< Flag >,
            "FlagKoopaB",
        },
        {
            "FlagRaceA",
            createNameObj< Flag >,
            "FlagRaceA",
        },
        {
            "FlagKoopaCastle",
            createNameObj< Flag >,
            "FlagKoopaCastle",
        },
        {
            "FlagPeachCastleA",
            createNameObj< Flag >,
            "FlagPeachCastleA",
        },
        {
            "FlagPeachCastleB",
            createNameObj< Flag >,
            "FlagPeachCastleB",
        },
        {
            "FlagPeachCastleC",
            createNameObj< Flag >,
            "FlagPeachCastleC",
        },
        {
            "FlagSurfing",
            createNameObj< Flag >,
            "FlagSurfing",
        },
        {
            "FlagTamakoro",
            createNameObj< Flag >,
            "FlagTamakoro",
        },
        {
            "SeaGullGroup",
            createNameObj< SeaGullGroup >,
            "SeaGull",
        },
        {
            "WaterPlant",
            createNameObj< WaterPlant >,
            "WaterPlant",
        },
        {
            "HitWallTimerSwitch",
            createNameObj< HitWallTimerSwitch >,
            "HitWallTimerSwitch",
        },
        {
            "PTimerSwitch",
            createNameObj< PTimerSwitch >,
            "PTimerSwitch",
        },
        {
            "CrystalSwitch",
            createNameObj< CrystalSwitch >,
            "CrystalSwitch",
        },
        {
            "HipDropSwitch",
            createNameObj< HipDropSwitch >,
            "HipDropSwitch",
        },
        {
            "HipDropTimerSwitch",
            createNameObj< HipDropTimerSwitch >,
            "HipDropTimerSwitch",
        },
        {
            "DesertLandUpSwitch",
            createNameObj< DesertLandMoveSwitch >,
            "DesertLandUpSwitch",
        },
        {
            "DesertLandDownSwitch",
            createNameObj< DesertLandMoveSwitch >,
            "DesertLandDownSwitch",
        },
        {
            "ScrewSwitch",
            createNameObj< ScrewSwitch >,
            "ScrewSwitch",
        },
        {
            "ScrewSwitchReverse",
            createNameObj< ScrewSwitchReverse >,
            "ScrewSwitchReverse",
        },
        {
            "PicketSwitch",
            createNameObj< PicketSwitch >,
            "PicketSwitch",
        },
        {
            "PicketSwitchBig",
            createNameObj< PicketSwitch >,
            "PicketSwitchBig",
        },
        {
            "ValveSwitch",
            createNameObj< ValveSwitch >,
            "ValveSwitch",
        },
        {
            "KeySwitch",
            createNameObj< KeySwitch >,
            "KeySwitch",
        },
        {
            "SpinLeverSwitch",
            createNameObj< SpinLeverSwitch >,
            "SpinLeverSwitch",
        },
        {
            "EffectObjSample",
            createNameObj< EffectObjR1000F50 >,
            nullptr,
        },
        {
            "ShootingStarArea",
            createNameObj< RandomEffectObj >,
            nullptr,
        },
        {
            "IcePlanetLight",
            createNameObj< EffectObjR100F50SyncClipping >,
            nullptr,
        },
        {
            "IceLayerBreak",
            createNameObj< EffectObjR500F50 >,
            nullptr,
        },
        {
            "LavaSparksS",
            createNameObj< EffectObj20x20x10SyncClipping >,
            nullptr,
        },
        {
            "LavaSparksL",
            createNameObj< EffectObj50x50x10SyncClipping >,
            nullptr,
        },
        {
            "SpaceDustS",
            createNameObj< EffectObj20x20x10SyncClipping >,
            nullptr,
        },
        {
            "SpaceDustL",
            createNameObj< EffectObj50x50x10SyncClipping >,
            nullptr,
        },
        {
            "BirdLouseS",
            createNameObj< EffectObj20x20x10SyncClipping >,
            nullptr,
        },
        {
            "BirdLouseL",
            createNameObj< EffectObj50x50x10SyncClipping >,
            nullptr,
        },
        {
            "SnowS",
            createNameObj< EffectObj10x10x10SyncClipping >,
            nullptr,
        },
        {
            "FallingSmallRock",
            createNameObj< FallingSmallRock >,
            nullptr,
        },
        {
            "LavaHomeVolcanoFallingRock",
            createNameObj< EffectObjR1000F50 >,
            nullptr,
        },
        {
            "Fountain",
            createNameObj< Fountain >,
            nullptr,
        },
        {
            "FountainBig",
            createNameObj< FountainBig >,
            "FountainBig",
        },
        {
            "WaterfallS",
            createNameObj< EffectObjR1000F50 >,
            nullptr,
        },
        {
            "WaterfallL",
            createNameObj< EffectObjR1000F50 >,
            nullptr,
        },
        {
            "DrainPipeBubbleA",
            createNameObj< EffectObjR500F50 >,
            nullptr,
        },
        {
            "DrainPipeBubbleB",
            createNameObj< EffectObjR500F50 >,
            nullptr,
        },
        {
            "WaterDropTop",
            createNameObj< EffectObjR1000F50 >,
            nullptr,
        },
        {
            "WaterDropBottom",
            createNameObj< EffectObjR1000F50 >,
            nullptr,
        },
        {
            "WaterDropMiddle",
            createNameObj< EffectObjR1000F50 >,
            nullptr,
        },
        {
            "SubmarineSteam",
            createNameObj< SubmarineSteam >,
            nullptr,
        },
        {
            "SubmarineVolcano",
            createNameObj< SubmarineSteam >,
            nullptr,
        },
        {
            "WaterLayerBreak",
            createNameObj< EffectObjR500F50 >,
            nullptr,
        },
        {
            "StarPieceClusterBreak",
            createNameObj< EffectObjR500F50 >,
            nullptr,
        },
        {
            "FireworksA",
            createNameObj< RandomEffectObj >,
            nullptr,
        },
        {
            "BattleShipExplosionRock",
            createNameObj< EffectObjR500F50 >,
            nullptr,
        },
        {
            "BattleShipExplosionMetal",
            createNameObj< EffectObjR500F50 >,
            nullptr,
        },
        {
            "ForestWaterfallS",
            createNameObj< EffectObjR1000F50 >,
            nullptr,
        },
        {
            "ForestWaterfallL",
            createNameObj< EffectObjR1000F50 >,
            nullptr,
        },
        {
            "SandBreezeS",
            createNameObj< EffectObj20x20x10SyncClipping >,
            nullptr,
        },
        {
            "SandBreezeL",
            createNameObj< EffectObj50x50x10SyncClipping >,
            nullptr,
        },
        {
            "SpringJetWater",
            createNameObj< SpringJetWater >,
            nullptr,
        },
        {
            "SandUpDownKGBaseEffect",
            createNameObj< SandUpDownEffectObj >,
            nullptr,
        },
        {
            "SandUpDownTowerTopEffect",
            createNameObj< SandUpDownEffectObj >,
            nullptr,
        },
        {
            "UFOSandObstacleAEffect",
            createNameObj< SandUpDownEffectObj >,
            nullptr,
        },
        {
            "UFOSandObstacleBEffect",
            createNameObj< SandUpDownEffectObj >,
            nullptr,
        },
        {
            "UFOSandObstacleCEffect",
            createNameObj< SandUpDownEffectObj >,
            nullptr,
        },
        {
            "GravityDust",
            createNameObj< EffectObjGravityDust >,
            nullptr,
        },
        {
            "Steam",
            createNameObj< SimpleEffectObj >,
            nullptr,
        },
        {
            "FallRedLeaf",
            createNameObj< EffectObj10x10x10SyncClipping >,
            nullptr,
        },
        {
            "FallGreenLeaf",
            createNameObj< EffectObj10x10x10SyncClipping >,
            nullptr,
        },
        {
            "IcicleRockLight",
            createNameObj< EffectObjR100F50SyncClipping >,
            nullptr,
        },
        {
            "TwinFallLakeWaterFall",
            createNameObj< EffectObjR1000F50 >,
            nullptr,
        },
        {
            "EffectTeresa",
            createNameObj< EffectObj50x50x10SyncClipping >,
            nullptr,
        },
        {
            "UFOKinokoLandingBlackSmoke",
            createNameObj< EffectObjR500F50 >,
            nullptr,
        },
        {
            "InvisibleWall10x10",
            createNameObj< InvisiblePolygonObj >,
            "InvisibleWall10x10",
        },
        {
            "InvisibleWall10x20",
            createNameObj< InvisiblePolygonObj >,
            "InvisibleWall10x20",
        },
        {
            "InvisibleWallJump10x10",
            createNameObj< InvisiblePolygonObj >,
            "InvisibleWallJump10x10",
        },
        {
            "InvisibleWallJump10x20",
            createNameObj< InvisiblePolygonObj >,
            "InvisibleWallJump10x20",
        },
        {
            "InvisibleWallGCapture10x10",
            createNameObj< InvisiblePolygonObjGCapture >,
            "InvisibleWallGCapture10x10",
        },
        {
            "InvisibleWallGCapture10x20",
            createNameObj< InvisiblePolygonObjGCapture >,
            "InvisibleWallGCapture10x20",
        },
        {
            "PolygonCodeRecoveryPlate",
            createNameObj< InvisiblePolygonObj >,
            "PolygonCodeRecoveryPlate",
        },
        {
            "PolygonCodeRecoveryBowl",
            createNameObj< InvisiblePolygonObj >,
            "PolygonCodeRecoveryBowl",
        },
        {
            "InvisibleWallBattleShipA",
            createNameObj< TransparentWall >,
            "InvisibleWallBattleShipA",
        },
        {
            "InvisibleWaterfall",
            createNameObj< TransparentWall >,
            "InvisibleWaterfall",
        },
        {
            "InvisibleWaterfallTwinFallLake",
            createNameObj< TransparentWall >,
            "InvisibleWaterfallTwinFallLake",
        },
        {
            "StatusFloor",
            createNameObj< TransparentWall >,
            "StatusFloor",
        },
        {
            "GlobalCubeGravity",
            MR::createGlobalCubeGravityObj,
            nullptr,
        },
        {
            "GlobalConeGravity",
            MR::createGlobalConeGravityObj,
            nullptr,
        },
        {
            "GlobalDiskGravity",
            MR::createGlobalDiskGravityObj,
            nullptr,
        },
        {
            "GlobalDiskTorusGravity",
            MR::createGlobalDiskTorusGravityObj,
            nullptr,
        },
        {
            "GlobalPlaneGravity",
            MR::createGlobalPlaneGravityObj,
            nullptr,
        },
        {
            "GlobalPlaneGravityInBox",
            MR::createGlobalPlaneInBoxGravityObj,
            nullptr,
        },
        {
            "GlobalPlaneGravityInCylinder",
            MR::createGlobalPlaneInCylinderGravityObj,
            nullptr,
        },
        {
            "GlobalPointGravity",
            MR::createGlobalPointGravityObj,
            nullptr,
        },
        {
            "GlobalSegmentGravity",
            MR::createGlobalSegmentGravityObj,
            nullptr,
        },
        {
            "GlobalWireGravity",
            MR::createGlobalWireGravityObj,
            nullptr,
        },
        {
            "MiniEggStarGalaxy",
            createNameObj< MiniatureGalaxy >,
            nullptr,
        },
        {
            "MiniHoneyBeeKingdomGalaxy",
            createNameObj< MiniatureGalaxy >,
            nullptr,
        },
        {
            "MiniFlipPanelExGalaxy",
            createNameObj< MiniatureGalaxy >,
            nullptr,
        },
        {
            "MiniSurfingLv1Galaxy",
            createNameObj< MiniatureGalaxy >,
            nullptr,
        },
        {
            "MiniTriLegLv1Galaxy",
            createNameObj< MiniatureGalaxy >,
            nullptr,
        },
        {
            "MiniStarDustGalaxy",
            createNameObj< MiniatureGalaxy >,
            nullptr,
        },
        {
            "MiniTamakoroExLv1Galaxy",
            createNameObj< MiniatureGalaxy >,
            nullptr,
        },
        {
            "MiniBattleShipGalaxy",
            createNameObj< MiniatureGalaxy >,
            nullptr,
        },
        {
            "MiniBreakDownPlanetGalaxy",
            createNameObj< MiniatureGalaxy >,
            nullptr,
        },
        {
            "MiniKoopaBattleVs1Galaxy",
            createNameObj< MiniatureGalaxy >,
            nullptr,
        },
        {
            "MiniHeavenlyBeachGalaxy",
            createNameObj< MiniatureGalaxy >,
            nullptr,
        },
        {
            "MiniCubeBubbleExLv1Galaxy",
            createNameObj< MiniatureGalaxy >,
            nullptr,
        },
        {
            "MiniPhantomGalaxy",
            createNameObj< MiniatureGalaxy >,
            nullptr,
        },
        {
            "MiniOceanFloaterLandGalaxy",
            createNameObj< MiniatureGalaxy >,
            nullptr,
        },
        {
            "MiniKoopaJrShipLv1Galaxy",
            createNameObj< MiniatureGalaxy >,
            nullptr,
        },
        {
            "MiniCosmosGardenGalaxy",
            createNameObj< MiniatureGalaxy >,
            nullptr,
        },
        {
            "MiniIceVolcanoGalaxy",
            createNameObj< MiniatureGalaxy >,
            nullptr,
        },
        {
            "MiniHoneyBeeExGalaxy",
            createNameObj< MiniatureGalaxy >,
            nullptr,
        },
        {
            "MiniSandClockGalaxy",
            createNameObj< MiniatureGalaxy >,
            nullptr,
        },
        {
            "MiniKoopaBattleVs2Galaxy",
            createNameObj< MiniatureGalaxy >,
            nullptr,
        },
        {
            "MiniReverseKingdomGalaxy",
            createNameObj< MiniatureGalaxy >,
            nullptr,
        },
        {
            "MiniOceanRingGalaxy",
            createNameObj< MiniatureGalaxy >,
            nullptr,
        },
        {
            "MiniFactoryGalaxy",
            createNameObj< MiniatureGalaxy >,
            nullptr,
        },
        {
            "MiniSkullSharkGalaxy",
            createNameObj< MiniatureGalaxy >,
            nullptr,
        },
        {
            "MiniFloaterOtaKingGalaxy",
            createNameObj< MiniatureGalaxy >,
            nullptr,
        },
        {
            "MiniOceanPhantomCaveGalaxy",
            createNameObj< MiniatureGalaxy >,
            nullptr,
        },
        {
            "MiniCannonFleetGalaxy",
            createNameObj< MiniatureGalaxy >,
            nullptr,
        },
        {
            "MiniDarkRoomGalaxy",
            createNameObj< MiniatureGalaxy >,
            nullptr,
        },
        {
            "MiniHellProminenceGalaxy",
            createNameObj< MiniatureGalaxy >,
            nullptr,
        },
        {
            "MiniKoopaBattleVs3Galaxy",
            createNameObj< MiniatureGalaxy >,
            nullptr,
        },
        {
            "SurprisedGalaxy",
            createNameObj< SurprisedGalaxy >,
            "MiniSurprisedGalaxy",
        },
        {
            "SurpBeltConveyerExGalaxy",
            createNameObj< SurprisedGalaxy >,
            "MiniSurprisedGalaxy",
        },
        {
            "SurpCocoonExGalaxy",
            createNameObj< SurprisedGalaxy >,
            "MiniSurprisedGalaxy",
        },
        {
            "SurpTearDropGalaxy",
            createNameObj< SurprisedGalaxy >,
            "MiniSurprisedGalaxy",
        },
        {
            "SurpTeresaMario2DGalaxy",
            createNameObj< SurprisedGalaxy >,
            "MiniSurprisedGalaxy",
        },
        {
            "SurpSnowCapsuleGalaxy",
            createNameObj< SurprisedGalaxy >,
            "MiniSurprisedGalaxy",
        },
        {
            "SurpTransformationExGalaxy",
            createNameObj< SurprisedGalaxy >,
            "MiniSurprisedGalaxy",
        },
        {
            "SurpFishTunnelGalaxy",
            createNameObj< SurprisedGalaxy >,
            "MiniSurprisedGalaxy",
        },
        {
            "SurpTamakoroExLv2Galaxy",
            createNameObj< SurprisedGalaxy >,
            "MiniSurprisedGalaxy",
        },
        {
            "SurpSurfingLv2Galaxy",
            createNameObj< SurprisedGalaxy >,
            "MiniSurprisedGalaxy",
        },
        {
            "SurpCubeBubbleExLv2Galaxy",
            createNameObj< SurprisedGalaxy >,
            "MiniSurprisedGalaxy",
        },
        {
            "SurpPeachCastleFinalGalaxy",
            createNameObj< SurprisedGalaxy >,
            "MiniSurprisedGalaxy",
        },
        {
            "ClipFieldSwitch",
            createNameObj< ClipFieldSwitch >,
            nullptr,
        },
        {
            "FallOutFieldDraw",
            MR::createFallOutFieldDraw,
            nullptr,
        },
        {
            "ClipFieldFillDraw",
            MR::createClipFieldFillDraw,
            nullptr,
        },
        {
            "SpotLightVolumeDraw",
            MR::createVolumeDrawSpotLight,
            nullptr,
        },
        {
            "GroupSwitchWatcher",
            createNameObj< GroupSwitchWatcher >,
            nullptr,
        },
        {
            "RaceRail",
            createNameObj< RaceRail >,
            nullptr,
        },
        {
            "TimerSwitch",
            createNameObj< TimerSwitch >,
            nullptr,
        },
        {
            "SwitchSynchronizerReverse",
            createNameObj< SwitchSynchronizer >,
            nullptr,
        },
        {
            "MovieStarter",
            createNameObj< MovieStarter >,
            "CinemaSuper",
        },
        {
            "DemoGroup",
            createNameObj< DemoExecutor >,
            nullptr,
        },
        {
            "DemoSubGroup",
            createNameObj< DemoCastSubGroup >,
            nullptr,
        },
        {
            "AstroDomeDemoStarter",
            createNameObj< AstroDomeDemoStarter >,
            "SuperSpinDriver",
        },
        {
            "AstroDomeGalaxySelector",
            createNameObj< AstroDomeGalaxySelector >,
            nullptr,
        },
        {
            "StarReturnDemoStarter",
            createNameObj< StarReturnDemoStarter >,
            nullptr,
        },
        {
            "GrandStarReturnDemoStarter",
            createNameObj< GrandStarReturnDemoStarter >,
            "GrandStar",
        },
        {
            "ScenarioStarter",
            createNameObj< ScenarioStarter >,
            "Welcome",
        },
        {
            "FileSelector",
            createNameObj< FileSelector >,
            nullptr,
        },
    };
    const NameObjFactory::Name2Archive cName2ArchiveNamesTable[] = {
        {
            "AirBubbleGenerator",
            "AirBubble",
        },
        {
            "AssemblyBlockPartsTimerA",
            "AssemblyBlockPartsTimerABloom",
        },
        {
            "AstroDomeBlueStar",
            "GCaptureRibbon",
        },
        {
            "AstroDomeComet",
            "AstroDomeCometBloom",
        },
        {
            "AstroDomeDemoStarter",
            "AstroDomeDemoCamera",
        },
        {
            "AstroDomeDemoStarter",
            "SpinDriverPath",
        },
        {
            "AstroDomeGalaxySelector",
            "BackButton",
        },
        {
            "AstroDomeGalaxySelector",
            "GalaxySelect",
        },
        {
            "BasaBasa",
            "BasaBasaHang",
        },
        {
            "BasaBasaIce",
            "BasaBasaIceHang",
        },
        {
            "BeeFlowerHover",
            "BeeFlowerHoverLow",
        },
        {
            "BegomanBabyLauncher",
            "BegomanBaby",
        },
        {
            "BegomanLauncher",
            "BegomanSpring",
        },
        {
            "BegomanLauncher",
            "BegomanSpringHead",
        },
        {
            "BegomanSpike",
            "BegomanBrokenPiece",
        },
        {
            "BegomanSpike",
            "BegomanSpikeHead",
        },
        {
            "BegomanSpring",
            "BegomanSpringHead",
        },
        {
            "BegomanSpringHide",
            "BegomanBaby",
        },
        {
            "BegomanSpringHide",
            "BegomanSpringHead",
        },
        {
            "BigBubbleGenerator",
            "BigBubble",
        },
        {
            "BigFan",
            "BigFanWind",
        },
        {
            "BigFanNoLeg",
            "BigFanWind",
        },
        {
            "BigObstructBubbleGenerator",
            "BigBubble",
        },
        {
            "BlackHole",
            "BlackHoleRange",
        },
        {
            "BlackHoleCube",
            "BlackHoleRange",
        },
        {
            "BlueStar",
            "BlueStarDemo",
        },
        {
            "BlueStarCupsulePlanet",
            "GCaptureRibbon",
        },
        {
            "BombBird",
            "BombBirdBomb",
        },
        {
            "BombHeiLauncher",
            "BombHei",
        },
        {
            "BombLauncher",
            "BombHei",
        },
        {
            "BossBegoman",
            "BegomanBaby",
        },
        {
            "BossBegoman",
            "BossBegomanHead",
        },
        {
            "BossKameck",
            "BossKameckBattleDemo",
        },
        {
            "BossKameck",
            "BossKameckFireBall",
        },
        {
            "BossKameck",
            "Koura",
        },
        {
            "BossKameck2",
            "BossKameckBarrier",
        },
        {
            "BossKameck2",
            "BossKameckBattleDemo",
        },
        {
            "BossKameck2",
            "BossKameckFireBall",
        },
        {
            "BossKameck2",
            "Koura",
        },
        {
            "BossStinkBug",
            "BossStinkBugBomb",
        },
        {
            "BossStinkBug",
            "BossStinkBugBombLauncher",
        },
        {
            "BossStinkBug",
            "BossStinkBugDemo",
        },
        {
            "BossStinkBug",
            "BossStinkBugWing",
        },
        {
            "BreakableCage",
            "BreakableCageBreak",
        },
        {
            "BreakableCageRotate",
            "BreakableCageBreak",
        },
        {
            "BreakableCageL",
            "BreakableCageBreak",
        },
        {
            "BreakableTrash",
            "BreakableCageBreak",
        },
        {
            "BrightObj",
            "GlareGlow",
        },
        {
            "BrightObj",
            "GlareLine",
        },
        {
            "BrightSun",
            "GlareGlow",
        },
        {
            "BrightSun",
            "GlareLine",
        },
        {
            "BrightSun",
            "Sun",
        },
        {
            "ButlerMap",
            "AllStarList",
        },
        {
            "ButlerMap",
            "BackButton",
        },
        {
            "ButlerMap",
            "MapButton",
        },
        {
            "ButlerMap",
            "MapDomeIcon",
        },
        {
            "ButlerMap",
            "MapGalaxyBG",
        },
        {
            "ButlerMap",
            "MapGalaxyIcon",
        },
        {
            "ButlerMap",
            "MapGrandGalaxy",
        },
        {
            "ButlerMap",
            "GalaxyDetail",
        },
        {
            "ButlerMap",
            "GalaxyInfoTexture",
        },
        {
            "ButlerMap",
            "IconComet",
        },
        {
            "ButlerMap",
            "IconMario",
        },
        {
            "ButlerMap",
            "MapTicoIcon",
        },
        {
            "ButlerMap",
            "MapTitle",
        },
        {
            "ChainedBall",
            "IronBall",
        },
        {
            "ChooChooTrain",
            "ChooChooTrainBody",
        },
        {
            "CocoSambo",
            "CocoSamboHead",
        },
        {
            "CollectTico",
            "ItemBubble",
        },
        {
            "ConcreteBlock",
            "ConcreteBlockBreak",
        },
        {
            "Creeper",
            "CreeperFlower",
        },
        {
            "Creeper",
            "CreeperLeaf",
        },
        {
            "CrystalCageL",
            "CrystalCageLBreak",
        },
        {
            "CrystalCageMoving",
            "Tico",
        },
        {
            "CrystalCageS",
            "CrystalCageSBreak",
        },
        {
            "CrystalCageM",
            "CrystalCageSBreak",
        },
        {
            "DemoKoopaJrShip",
            "KoopaJr",
        },
        {
            "DemoKoopaJrShip",
            "KoopaJrMiddle",
        },
        {
            "DemoKoopaJrShip",
            "KoopaJrLow",
        },
        {
            "DharmaSambo",
            "DharmaSamboParts",
        },
        {
            "DinoPackun",
            "DinoPackunDemoPosition",
        },
        {
            "DinoPackun",
            "DinoPackunEggShell",
        },
        {
            "DinoPackun",
            "DinoPackunEggShellBreak",
        },
        {
            "DinoPackun",
            "DinoPackunTailBall",
        },
        {
            "DinoPackunVs2",
            "DinoPackunDemoPosition",
        },
        {
            "DinoPackunVs2",
            "DinoPackunEggShell",
        },
        {
            "DinoPackunVs2",
            "DinoPackunEggShellBreak",
        },
        {
            "DinoPackunVs2",
            "DinoPackunFireBall",
        },
        {
            "DinoPackunVs2",
            "DinoPackunTailBall",
        },
        {
            "Dodoryu",
            "DodoryuBank",
        },
        {
            "Dodoryu",
            "DodoryuHillA",
        },
        {
            "Dodoryu",
            "DodoryuHillB",
        },
        {
            "Dodoryu",
            "DodoryuHillC",
        },
        {
            "Dodoryu",
            "DodoryuLeadHill",
        },
        {
            "Dodoryu",
            "DodoryuRabbit",
        },
        {
            "EarthenPipeInWater",
            "EarthenPipeStream",
        },
        {
            "ElectricBazooka",
            "ElectricBazookaBreak",
        },
        {
            "ElectricBazooka",
            "ElectricBullet",
        },
        {
            "ElectricBazooka",
            "MogucchiShooter",
        },
        {
            "ElectricBazooka",
            "WaterBazookaCapsule",
        },
        {
            "ElectricRail",
            "ElectricRailPoint",
        },
        {
            "ElectricRailMoving",
            "ElectricRailPoint",
        },
        {
            "ExterminationSkeletalFishBaby",
            "AirBubble",
        },
        {
            "ExterminationKuriboKeySwitch",
            "KeySwitch",
        },
        {
            "EyeBeamer",
            "EyeBeamerBeam",
        },
        {
            "EyeBeamer",
            "EyeBeamerBeamBloom",
        },
        {
            "EyeBeamer",
            "EyeBeamerBeamVolume",
        },
        {
            "FileSelector",
            "FileInfo",
        },
        {
            "FileSelector",
            "FileSelect",
        },
        {
            "FileSelector",
            "FileSelectDataPlanet",
        },
        {
            "FileSelector",
            "FileSelectDataMario",
        },
        {
            "FileSelector",
            "FileSelectDataYoshi",
        },
        {
            "FileSelector",
            "FileSelectDataLuigi",
        },
        {
            "FileSelector",
            "FileSelectDataPeach",
        },
        {
            "FileSelector",
            "FileSelectDataKinopio",
        },
        {
            "FileSelector",
            "CometNearOrbitSky",
        },
        {
            "FileSelector",
            "MiiFaceDatabase",
        },
        {
            "FileSelector",
            "BackButton",
        },
        {
            "FileSelector",
            "FileNumber",
        },
        {
            "FileSelector",
            "TitleLogo",
        },
        {
            "FileSelector",
            "PressStart",
        },
        {
            "FileSelector",
            "MiiSelect",
        },
        {
            "FileSelector",
            "MiiSelect",
        },
        {
            "FileSelector",
            "MiiIcon",
        },
        {
            "FileSelector",
            "MiiConfirmIcon",
        },
        {
            "FileSelector",
            "BrosButton",
        },
        {
            "FileSelector",
            "P2Manual",
        },
        {
            "FileSelector",
            "MiniatureGalaxySelect",
        },
        {
            "FileSelector",
            "SysPALInfo",
        },
        {
            "FileSelector",
            "MiiFont",
        },
        {
            "FireBallBeamKameck",
            "BossKameckFireBall",
        },
        {
            "FireBar",
            "FireBarBall",
        },
        {
            "FirePressure",
            "FireBullet",
        },
        {
            "FlipPanel",
            "FlipPanelBloom",
        },
        {
            "FlipPanelReverse",
            "FlipPanelReverseBloom",
        },
        {
            "Fluff",
            "FluffLeaf",
        },
        {
            "GCaptureTarget",
            "GCaptureRibbon",
        },
        {
            "GhostPlayer",
            "Race",
        },
        {
            "GrandStarReturnDemoStarter",
            "SpinDriverPath",
        },
        {
            "GrandStarReturnDemoStarter",
            "DemoAstroReturn",
        },
        {
            "GrandStarReturnDemoStarter",
            "ResultWindow",
        },
        {
            "GravityLightA",
            "GravityLightALightVolume",
        },
        {
            "HammerHeadPackun",
            "PackunLeaf",
        },
        {
            "Hanachan",
            "HanachanBody",
        },
        {
            "Hanachan",
            "HanachanBodyS",
        },
        {
            "HazeCube",
            "ShimmerBoard",
        },
        {
            "HeavensDoorInsidePlanetPartsA",
            "HeavensDoorInsidePlanetPartsABloom",
        },
        {
            "HipDropRock",
            "HipDropRockBreak",
        },
        {
            "HoneyBee",
            "HoneyBeeLow",
        },
        {
            "HoneyBee",
            "HoneyBeeMIddle",
        },
        {
            "HoneyQueen",
            "HoneyQueenLow",
        },
        {
            "HoneyQueen",
            "HoneyQueenMiddle",
        },
        {
            "HoneyQueen",
            "HoneyQueenWing",
        },
        {
            "IceMerameraKing",
            "IceManIce",
        },
        {
            "IceMerameraKing",
            "IceMerameraKingShock",
        },
        {
            "IceMerameraKing",
            "IceMerameraKingBreak",
        },
        {
            "IcicleRock",
            "IcicleRockBreak",
        },
        {
            "IronBall",
            "BallTornado",
        },
        {
            "IronBall",
            "IronBallSpin",
        },
        {
            "IronCannonLauncher",
            "KoopaJrShipCannonShell",
        },
        {
            "IronCannonLauncherPoint",
            "KoopaJrShipCannonShell",
        },
        {
            "JumpBeamer",
            "JumpBeamerBeam",
        },
        {
            "JumpBeamer",
            "JumpBeamerBeamBloom",
        },
        {
            "JumpBeamer",
            "JumpBeamerHead",
        },
        {
            "JumpGuarder",
            "BegomanBaby",
        },
        {
            "JumpGuarder",
            "JumpGuarderHead",
        },
        {
            "Kabokuri",
            "KabokuriBreak",
        },
        {
            "Kabokuri",
            "Kuribo",
        },
        {
            "Kart",
            "Coin",
        },
        {
            "Kinopio",
            "KinopioMiddle",
        },
        {
            "Kinopio",
            "KinopioLow",
        },
        {
            "KinopioAstro",
            "KinopioMiddle",
        },
        {
            "KinopioAstro",
            "KinopioLow",
        },
        {
            "Kiraira",
            "KirairaChain",
        },
        {
            "Kiraira",
            "KirairaFixPointBottom",
        },
        {
            "Kiraira",
            "KirairaFixPointTop",
        },
        {
            "KoopaJr",
            "KoopaJrLow",
        },
        {
            "KoopaJr",
            "KoopaJrMiddle",
        },
        {
            "KoopaJrNormalShipA",
            "KoopaJrNormalShipALow",
        },
        {
            "KoopaJrShip",
            "KoopaJr",
        },
        {
            "KoopaJrShip",
            "KoopaJrLow",
        },
        {
            "KoopaJrShip",
            "KoopaJrMiddle",
        },
        {
            "KoopaJrShip",
            "KoopaJrShipBreak",
        },
        {
            "KoopaJrShip",
            "KoopaJrShipCannonMainShell",
        },
        {
            "KoopaJrShip",
            "KoopaJrShipCannonShell",
        },
        {
            "KoopaJrShip",
            "KoopaJrShipPod",
        },
        {
            "KoopaJrShip",
            "Kameck",
        },
        {
            "KoopaJrShip",
            "HomingKiller",
        },
        {
            "KoopaJrShip",
            "BossKameckFireBall",
        },
        {
            "KoopaShipE",
            "KoopaShipELow",
        },
        {
            "KoopaStatue",
            "KoopaStatueBreak",
        },
        {
            "KoopaVs1",
            "KoopaPlate",
        },
        {
            "KoopaVs1",
            "KoopaPlateCoin",
        },
        {
            "KoopaVs1",
            "KoopaPlateDamage",
        },
        {
            "KoopaVs1",
            "KoopaPlanetShadow",
        },
        {
            "KoopaVs1",
            "KoopaArmorBreak",
        },
        {
            "KoopaVs1",
            "KoopaBattleMapPlanet",
        },
        {
            "KoopaVs1",
            "KoopaBattleMapPlanetBloom",
        },
        {
            "KoopaVs1",
            "KoopaFire",
        },
        {
            "KoopaVs1",
            "KoopaFireBloom",
        },
        {
            "KoopaVs1",
            "KoopaShockWave",
        },
        {
            "KoopaVs1",
            "KoopaShockWaveBloom",
        },
        {
            "KoopaVs1",
            "KoopaShockWaveShadow",
        },
        {
            "KoopaVs1",
            "KoopaThorn",
        },
        {
            "KoopaVs1",
            "KoopaThornBreak",
        },
        {
            "KoopaVs1",
            "MeteorStrike",
        },
        {
            "KoopaVs1",
            "MeteorStrikeBreak",
        },
        {
            "KoopaVs2",
            "KoopaArmorBreak",
        },
        {
            "KoopaVs2",
            "KoopaBattleMapPlanetVs2",
        },
        {
            "KoopaVs2",
            "KoopaBattleMapPlanetVs2Bloom",
        },
        {
            "KoopaVs2",
            "KoopaFire",
        },
        {
            "KoopaVs2",
            "KoopaFireBloom",
        },
        {
            "KoopaVs2",
            "KoopaPlate",
        },
        {
            "KoopaVs2",
            "KoopaPlateCoin",
        },
        {
            "KoopaVs2",
            "KoopaPlateDamage",
        },
        {
            "KoopaVs2",
            "KoopaRollBall",
        },
        {
            "KoopaVs2",
            "KoopaShockWave",
        },
        {
            "KoopaVs2",
            "KoopaShockWaveBloom",
        },
        {
            "KoopaVs2",
            "KoopaShockWaveShadow",
        },
        {
            "KoopaVs2",
            "KoopaThorn",
        },
        {
            "KoopaVs2",
            "KoopaThornBreak",
        },
        {
            "KoopaVs3",
            "KoopaArmorBreak",
        },
        {
            "KoopaVs3",
            "KoopaBattleMapPlanetVs3Lv1",
        },
        {
            "KoopaVs3",
            "KoopaBattleMapPlanetVs3Lv2",
        },
        {
            "KoopaVs3",
            "KoopaBattleMapPlanetVs3Lv3",
        },
        {
            "KoopaVs3",
            "KoopaFire",
        },
        {
            "KoopaVs3",
            "KoopaFireBloom",
        },
        {
            "KoopaVs3",
            "KoopaPlateDamage",
        },
        {
            "KoopaVs3",
            "KoopaPlanetShadow",
        },
        {
            "KoopaVs3",
            "KoopaRollBall",
        },
        {
            "KoopaVs3",
            "KoopaRock",
        },
        {
            "KoopaVs3",
            "KoopaRockBreak",
        },
        {
            "KoopaVs3",
            "KoopaShockWave",
        },
        {
            "KoopaVs3",
            "KoopaShockWaveBloom",
        },
        {
            "KoopaVs3",
            "KoopaShockWaveShadow",
        },
        {
            "KoopaVs3",
            "KoopaThorn",
        },
        {
            "KoopaVs3",
            "KoopaThornBreak",
        },
        {
            "KoopaVs3",
            "KoopaVS3HoleSunPlanet",
        },
        {
            "KoopaVs3",
            "KoopaVS3HoleSunPlanetBloom",
        },
        {
            "KoopaVs3",
            "KoopaVS3HoleSunInsidePlanet",
        },
        {
            "KoopaVs3",
            "KoopaVS3HoleSunInsidePlanetBloom",
        },
        {
            "KoopaVs3",
            "MeteorStrike",
        },
        {
            "KoopaVs3",
            "MeteorStrikeBreak",
        },
        {
            "KoopaVs3",
            "Peach",
        },
        {
            "KoopaVs3",
            "PeachMiddle",
        },
        {
            "KoopaVs3",
            "PeachLow",
        },
        {
            "KoopaVs3",
            "KoopaJr",
        },
        {
            "KoopaVs3",
            "KoopaJrMiddle",
        },
        {
            "KoopaVs3",
            "KoopaJrLow",
        },
        {
            "KoopaVs3",
            "KoopaJrShip",
        },
        {
            "LargeChain",
            "LargeChainFixPoint",
        },
        {
            "LavaProminence",
            "LavaProminenceBloom",
        },
        {
            "LavaProminenceEnvironment",
            "LavaProminenceEnvironmentBloom",
        },
        {
            "LavaProminenceTriple",
            "LavaProminenceTripleBloom",
        },
        {
            "LavaProminenceWithoutShadow",
            "LavaProminenceBloom",
        },
        {
            "LavaStrangeRockL",
            "LavaStrangeRockLLow",
        },
        {
            "LODTestPlanet",
            "LODTestBillboard",
        },
        {
            "LODTestPlanet",
            "LODTestLowPolygon",
        },
        {
            "MarioLauncher",
            "Telescope",
        },
        {
            "MarioLauncher",
            "Sight",
        },
        {
            "MechaKoopaPartsBody",
            "MechaKoopaPartsBodyLow",
        },
        {
            "MeteorCannon",
            "MeteorCannonBreak",
        },
        {
            "MeteorStrike",
            "MeteorStrikeBreak",
        },
        {
            "Mogu",
            "MoguHole",
        },
        {
            "Mogu",
            "MoguStone",
        },
        {
            "Mogucchi",
            "MogucchiHillA",
        },
        {
            "Mogucchi",
            "MogucchiHillB",
        },
        {
            "Mogucchi",
            "MogucchiHillC",
        },
        {
            "Mogucchi",
            "MogucchiHole",
        },
        {
            "NokonokoLand",
            "Koura",
        },
        {
            "NoteFairy",
            "Note",
        },
        {
            "OceanBowl",
            "WaterCameraFilter",
        },
        {
            "OceanRing",
            "WaterCameraFilter",
        },
        {
            "OceanRing",
            "WaterWave",
        },
        {
            "OceanRingAndFlag",
            "FlagSurfing",
        },
        {
            "OceanRingAndFlag",
            "WaterCameraFilter",
        },
        {
            "OceanRingAndFlag",
            "WaterWave",
        },
        {
            "OceanSphere",
            "WaterCameraFilter",
        },
        {
            "OtaRock",
            "FireBall",
        },
        {
            "PackunPetit",
            "PackunPetitHead",
        },
        {
            "Peach",
            "PeachLow",
        },
        {
            "Peach",
            "PeachMiddle",
        },
        {
            "PeachCastleTownBeforeAttack",
            "PeachCastleTownBeforeAttackBloom",
        },
        {
            "Penguin",
            "PenguinLow",
        },
        {
            "Penguin",
            "PenguinMiddle",
        },
        {
            "PenguinCoach",
            "TiltGuidance",
        },
        {
            "PenguinSkater",
            "PenguinLow",
        },
        {
            "PenguinSkater",
            "PenguinMiddle",
        },
        {
            "PenguinStudent",
            "KouraShine",
        },
        {
            "PenguinStudent",
            "PenguinLow",
        },
        {
            "PenguinStudent",
            "PenguinMiddle",
        },
        {
            "Plant",
            "PlantLeaf",
        },
        {
            "Plant",
            "PlantSeed",
        },
        {
            "Plant",
            "PlantTop",
        },
        {
            "Polta",
            "PoltaArmBreak",
        },
        {
            "Polta",
            "PoltaBodyBreak",
        },
        {
            "Polta",
            "PoltaFormationRock",
        },
        {
            "Polta",
            "PoltaGroundRock",
        },
        {
            "Polta",
            "PoltaGroundRockBreak",
        },
        {
            "Polta",
            "PoltaLeftArm",
        },
        {
            "Polta",
            "PoltaRightArm",
        },
        {
            "Polta",
            "PoltaRock",
        },
        {
            "PrologueDirector",
            "PeachLetterMini",
        },
        {
            "PrologueDirector",
            "PrologueDemo",
        },
        {
            "PrologueDirector",
            "DemoPeachCastleGate",
        },
        {
            "Rabbit",
            "MoonRabbitLow",
        },
        {
            "Rabbit",
            "MoonRabbitMiddle",
        },
        {
            "RaceRail",
            "Race",
        },
        {
            "RingBeamer",
            "JumpBeamerBeam",
        },
        {
            "RingBeamer",
            "JumpBeamerBeamBloom",
        },
        {
            "RockBall",
            "BallTornado",
        },
        {
            "RockCreator",
            "RockBreak",
        },
        {
            "Ruby",
            "LODTestLow",
        },
        {
            "Ruby",
            "LODTestMiddle",
        },
        {
            "RunawayRabbitCollect",
            "SpotMarkLight",
        },
        {
            "RunawayRabbitCollect",
            "Tico",
        },
        {
            "RunawayRabbitCollect",
            "TicoBaby",
        },
        {
            "RunawayRabbitCollect",
            "TrickRabbitBaby",
        },
        {
            "SandUpDownTowerBreakableWallA",
            "SandUpDownTowerBreakableWallBreak",
        },
        {
            "SandUpDownTowerBreakableWallB",
            "SandUpDownTowerBreakableWallBreak",
        },
        {
            "SkeletalFishBaby",
            "AirBubble",
        },
        {
            "SkeletalFishBoss",
            "SkeletalFishBossHeadA",
        },
        {
            "SkeletalFishBoss",
            "SkeletalFishBossLight",
        },
        {
            "SkeletalFishBoss",
            "SkeletalFishBossLightBloom",
        },
        {
            "SkeletalFishBoss",
            "SkeletalFishBossScarFlash",
        },
        {
            "SkeletalFishBoss",
            "SkeletalFishBossBreak",
        },
        {
            "SkeletalFishBoss",
            "SkeletalFishBossShadow",
        },
        {
            "SkeletalFishBoss",
            "SkeletalFishGuard",
        },
        {
            "SkeletalFishBoss",
            "SkeletalFishGUardShadow",
        },
        {
            "Snakehead",
            "SnakeheadShadow",
        },
        {
            "SnakeheadSmall",
            "SnakeheadSmallShadow",
        },
        {
            "SpiderCoin",
            "SpaceCocoon",
        },
        {
            "SpinDriver",
            "SpinDriverShadow",
        },
        {
            "StarReturnDemoStarter",
            "ResultWindow",
        },
        {
            "StringSpider",
            "StringSpiderString",
        },
        {
            "SubmarineVolcanoBigColumn",
            "SubmarineVolcanoBigColumnBreak",
        },
        {
            "SubmarineVolcanoSmallColumn",
            "AirBubble",
        },
        {
            "SubmarineVolcanoSmallColumn",
            "SubmarineVolcanoSmallColumnBreak",
        },
        {
            "SuperSpinDriver",
            "SpinDriverPath",
        },
        {
            "SuperSpinDriver",
            "SuperSpinDriverShadow",
        },
        {
            "SuperSpinDriverGreen",
            "SpinDriverPath",
        },
        {
            "SuperSpinDriverGreen",
            "SuperSpinDriverEmpty",
        },
        {
            "SuperSpinDriverGreen",
            "SuperSpinDriverShadow",
        },
        {
            "SuperSpinDriverRed",
            "SpinDriverPath",
        },
        {
            "SuperSpinDriverRed",
            "SuperSpinDriverEmpty",
        },
        {
            "SuperSpinDriverRed",
            "SuperSpinDriverShadow",
        },
        {
            "Syati",
            "PrizeRing",
        },
        {
            "Syati",
            "Race",
        },
        {
            "Takobo",
            "IceBox",
        },
        {
            "TakoHei",
            "TakoHeiInk",
        },
        {
            "TamakoroWithTutorial",
            "BallGuidance",
        },
        {
            "TamakoroWithTutorial",
            "SignBoard",
        },
        {
            "TeresaMansionLightA",
            "TeresaMansionGlaringLightA",
        },
        {
            "TeresaMansionLightA",
            "TeresaMansionLightALightVolume",
        },
        {
            "TeresaMansionLightB",
            "TeresaMansionGlaringLightB",
        },
        {
            "TeresaMansionLightB",
            "TeresaMansionGlaringLightBBloom",
        },
        {
            "TeresaMansionLightB",
            "TeresaMansionLightBLightVolume",
        },
        {
            "SandRiverLightA",
            "SandRiverGlaringLightA",
        },
        {
            "SandRiverLightA",
            "SandRiverLightALightVolume",
        },
        {
            "TicoFat",
            "StarPieceTargetMeter",
        },
        {
            "TicoFat",
            "StarPieceTargetCounter",
        },
        {
            "TicoGalaxy",
            "StarPieceTargetMeter",
        },
        {
            "TicoGalaxy",
            "StarPieceTargetCounter",
        },
        {
            "TicoStarRing",
            "TicoLow",
        },
        {
            "TicoStarRing",
            "TicoMiddle",
        },
        {
            "TicoRail",
            "TicoLow",
        },
        {
            "TicoRail",
            "TicoMiddle",
        },
        {
            "TogeBegomanLauncher",
            "BegomanBrokenPiece",
        },
        {
            "TogeBegomanLauncher",
            "BegomanSpike",
        },
        {
            "TogeBegomanLauncher",
            "BegomanSpikeHead",
        },
        {
            "TombSpider",
            "SpiderThread",
        },
        {
            "TombSpider",
            "TombSpiderCocoon",
        },
        {
            "TombSpider",
            "TombSpiderGland",
        },
        {
            "TombSpider",
            "TombSpiderPlanet",
        },
        {
            "TombSpider",
            "TombSpiderPlanetLow",
        },
        {
            "TombSpider",
            "TombSpiderVitalSpot",
        },
        {
            "Torpedo",
            "TorpedoPropeller",
        },
        {
            "Torpedo",
            "TorpedoLight",
        },
        {
            "TreasureBoxCrackedAirBubble",
            "AirBubble",
        },
        {
            "TrickRabbit",
            "SpotMarkLight",
        },
        {
            "TrickRabbitGhost",
            "SpotMarkLight",
        },
        {
            "TrickRabbitFreeRun",
            "SpotMarkLight",
        },
        {
            "TrickRabbitFreeRunCollect",
            "SpotMarkLight",
        },
        {
            "TrickRabbitFreeRunCollect",
            "BombTimer",
        },
        {
            "Tripod2Boss",
            "Tripod2BossLow",
        },
        {
            "Tripod2Boss",
            "Tripod2BossLegShadow",
        },
        {
            "TripodBoss",
            "TripodBossLegShadow",
        },
        {
            "TripodBoss",
            "TripodBossLow",
        },
        {
            "TripodBoss",
            "GrandStar",
        },
        {
            "TripodBossBottomKillerCannon",
            "HomingKiller",
        },
        {
            "TripodBossCore",
            "TripodBossCoreBloom",
        },
        {
            "TripodBossCore",
            "TripodBossCoreBreak",
        },
        {
            "TripodBossKillerGenerater",
            "HomingKiller",
        },
        {
            "TripodBossShell",
            "TripodBossShellBreak",
        },
        {
            "TripodBossUnderKillerCannon",
            "HomingKiller",
        },
        {
            "TripodBossUpperKillerCannon",
            "HomingKiller",
        },
        {
            "TurtleBeamKameck",
            "Koura",
        },
        {
            "UFOBattleStageA",
            "UFOBattleStageABloom",
        },
        {
            "UFOBlueStarCupsule",
            "GCaptureRibbon",
        },
        {
            "Unizo",
            "UnizoBreak",
        },
        {
            "UnizoLand",
            "UnizoLandBreak",
        },
        {
            "UnizoLauncher",
            "UnizoLandBreak",
        },
        {
            "UnizoShoal",
            "UnizoShoalBreak",
        },
        {
            "WanwanRolling",
            "WanwanRollingBreak",
        },
        {
            "WanwanRollingGold",
            "WanwanRollingGoldBreak",
        },
        {
            "WatchTowerRotateStep",
            "WatchTowerRotateStepLift",
        },
        {
            "WaterBazooka",
            "MogucchiShooter",
        },
        {
            "WaterBazooka",
            "WaterBazookaBreak",
        },
        {
            "WaterBazooka",
            "WaterBazookaCapsule",
        },
        {
            "WaterBazooka",
            "WaterBullet",
        },
        {
            "WaterfallCaveCover",
            "WaterfallCaveCoverBreak",
        },
        {
            "WaterLeakPipe",
            "IceStepNoSlip",
        },
        {
            "WaterPressure",
            "WaterBullet",
        },
        {
            "WaterRoad",
            "WaterCameraFilter",
        },
        {
            "WaterCube",
            "WaterCameraFilter",
        },
        {
            "WaterSphere",
            "WaterCameraFilter",
        },
        {
            "WaterCylinder",
            "WaterCameraFilter",
        },
        {
            "WhirlPoolAccelerator",
            "WaterCameraFilter",
        },
        {
            "WhiteRoom",
            "WhiteRoomBloom",
        },
        {
            "WoodBox",
            "WoodBoxBreak",
        },
        {
            "SummerSky",
            "SpaceInner",
        },
        {
            "KoopaBattleMapPole",
            "FlagKoopaCastle",
        },
        {
            "AsteroidB",
            "AsteroidBLow",
        },
        {
            "AsteroidC",
            "AsteroidCLow",
        },
        {
            "AsteroidD",
            "AsteroidDLow",
        },
        {
            "SpaceStickA",
            "SpaceStickALow",
        },
        {
            "SpaceStickB",
            "SpaceStickBLow",
        },
        {
            "LavaBlackUFO",
            "LavaBlackUFOLow",
        },
        {
            "LavaStepA",
            "LavaStepALow",
        },
        {
            "LavaStepB",
            "LavaStepBLow",
        },
        {
            "LavaStepC",
            "LavaStepCLow",
        },
        {
            "LavaFloatingStepA",
            "LavaFloatingStepALow",
        },
        {
            "LavaRotatePlanetStartStep",
            "LavaRotatePlanetStartStepLow",
        },
        {
            "SweetsDecoratePartsFork",
            "SweetsDecoratePartsForkLow",
        },
        {
            "SweetsDecoratePartsSpoon",
            "SweetsDecoratePartsSpoonLow",
        },
        {
            "MovieStarter",
            "MoviePadRumblePrologueA",
        },
        {
            "MovieStarter",
            "MoviePadRumblePrologueB",
        },
        {
            "MovieStarter",
            "MoviePadRumbleFinalBattle",
        },
        {
            "MovieStarter",
            "MoviePadRumbleEpilogueA",
        },
        {
            "MovieStarter",
            "MoviePadRumbleEndingA",
        },
        {
            "MovieStarter",
            "MoviePadRumbleEndingB",
        },
    };

    const NameObjFactory::Name2MakeArchiveListFunc cName2MakeArchiveListFuncTable[] = {
        {
            "BallBeamer",
            BallBeamer::makeArchiveList,
        },
        {
            "BreakableCage",
            MR::makeArchiveListDummyDisplayModel,
        },
        {
            "BreakableCageRotate",
            MR::makeArchiveListDummyDisplayModel,
        },
        {
            "BreakableCageL",
            MR::makeArchiveListDummyDisplayModel,
        },
        {
            "CrystalCageS",
            MR::makeArchiveListDummyDisplayModel,
        },
        {
            "CrystalCageM",
            MR::makeArchiveListDummyDisplayModel,
        },
        {
            "DemoRabbit",
            DemoRabbit::makeArchiveList,
        },
        {
            "GhostPlayer",
            GhostPlayer::makeArchiveList,
        },
        {
            "KuriboChief",
            KuriboChief::makeArchiveList,
        },
        {
            "YellowChip",
            ChipBase::makeArchiveList,
        },
        {
            "BlueChip",
            ChipBase::makeArchiveList,
        },
        {
            "CocoNut",
            CocoNut::makeArchiveList,
        },
        {
            "Coin",
            Coin::makeArchiveList,
        },
        {
            "PurpleCoin",
            Coin::makeArchiveList,
        },
        {
            "RailCoin",
            Coin::makeArchiveList,
        },
        {
            "CircleCoinGroup",
            Coin::makeArchiveList,
        },
        {
            "PurpleRailCoin",
            Coin::makeArchiveList,
        },
        {
            "CirclePurpleCoinGroup",
            Coin::makeArchiveList,
        },
        {
            "OtaKing",
            OtaKing::makeArchiveList,
        },
        {
            "OtaRock",
            OtaRock::makeArchiveList,
        },
        {
            "PowerStar",
            PowerStar::makeArchiveList,
        },
        {
            "RainCloud",
            RainCloud::makeArchiveList,
        },
        {
            "RosettaReading",
            RosettaReading::makeArchiveList,
        },
        {
            "RosettaPictureBook",
            RosettaPictureBook::makeArchiveList,
        },
        {
            "RunawayTico",
            RunawayTico::makeArchiveList,
        },
        {
            "Teresa",
            MR::makeArchiveListDummyDisplayModel,
        },
        {
            "Caretaker",
            Caretaker::makeArchiveList,
        },
        {
            "Kinopio",
            NPCActor::makeArchiveList,
        },
        {
            "KinopioAstro",
            KinopioAstro::makeArchiveList,
        },
        {
            "Tico",
            Tico::makeArchiveList,
        },
        {
            "TicoAstro",
            Tico::makeArchiveList,
        },
        {
            "TicoComet",
            NPCActor::makeArchiveListDefault,
        },
        {
            "TicoShop",
            NPCActor::makeArchiveListDefault,
        },
        {
            "TicoFat",
            TicoFat::makeArchiveList,
        },
        {
            "TicoGalaxy",
            TicoFat::makeArchiveList,
        },
        {
            "PenguinRacer",
            NPCActor::makeArchiveListDefault,
        },
        {
            "PenguinRacerLeader",
            NPCActor::makeArchiveListDefault,
        },
        {
            "Rosetta",
            Rosetta::makeArchiveList,
        },
        {
            "HoneyBee",
            HoneyBee::makeArchiveList,
        },
        {
            "LuigiNPC",
            LuigiNPC::makeArchiveList,
        },
        {
            "LuigiEvent",
            LuigiNPC::makeArchiveList,
        },
        {
            "StarReturnDemoStarter",
            StarReturnDemoStarter::makeArchiveList,
        },
        {
            "UFOKinoko",
            UFOKinoko::makeArchiveList,
        },
        {
            "UFOKinokoUnderConstruction",
            MR::makeArchiveListUFOKinokoUnderConstruction,
        },
        {
            "SunakazeKun",
            Sandstorm::makeArchiveList,
        },
        {
            "AstroDome",
            AstroMapObjFunction::makeArchiveListFromArg0,
        },
        {
            "AstroDomeEntrance",
            AstroMapObjFunction::makeArchiveListFromArg0,
        },
        {
            "AstroDomeSky",
            AstroMapObjFunction::makeArchiveListFromArg0,
        },
        {
            "AstroDomeGalaxySelector",
            GalaxySelectInfo::makeArchiveList,
        },
        {
            "AstroStarPlate",
            AstroMapObjFunction::makeArchiveListFromArg0,
        },
        {
            "AstroLibrary",
            AstroMapObjFunction::makeArchiveListAstroNamePlate,
        },
        {
            "AstroChildRoom",
            AstroMapObjFunction::makeArchiveListAstroNamePlate,
        },
        {
            "AstroParking",
            AstroMapObjFunction::makeArchiveListAstroNamePlate,
        },
        {
            "MiniEggStarGalaxy",
            MiniatureGalaxy::makeArchiveList,
        },
        {
            "MiniHoneyBeeKingdomGalaxy",
            MiniatureGalaxy::makeArchiveList,
        },
        {
            "MiniFlipPanelExGalaxy",
            MiniatureGalaxy::makeArchiveList,
        },
        {
            "MiniSurfingLv1Galaxy",
            MiniatureGalaxy::makeArchiveList,
        },
        {
            "MiniTriLegLv1Galaxy",
            MiniatureGalaxy::makeArchiveList,
        },
        {
            "MiniStarDustGalaxy",
            MiniatureGalaxy::makeArchiveList,
        },
        {
            "MiniTamakoroExLv1Galaxy",
            MiniatureGalaxy::makeArchiveList,
        },
        {
            "MiniBattleShipGalaxy",
            MiniatureGalaxy::makeArchiveList,
        },
        {
            "MiniBreakDownPlanetGalaxy",
            MiniatureGalaxy::makeArchiveList,
        },
        {
            "MiniKoopaBattleVs1Galaxy",
            MiniatureGalaxy::makeArchiveList,
        },
        {
            "MiniHeavenlyBeachGalaxy",
            MiniatureGalaxy::makeArchiveList,
        },
        {
            "MiniCubeBubbleExLv1Galaxy",
            MiniatureGalaxy::makeArchiveList,
        },
        {
            "MiniPhantomGalaxy",
            MiniatureGalaxy::makeArchiveList,
        },
        {
            "MiniOceanFloaterLandGalaxy",
            MiniatureGalaxy::makeArchiveList,
        },
        {
            "MiniKoopaJrShipLv1Galaxy",
            MiniatureGalaxy::makeArchiveList,
        },
        {
            "MiniCosmosGardenGalaxy",
            MiniatureGalaxy::makeArchiveList,
        },
        {
            "MiniIceVolcanoGalaxy",
            MiniatureGalaxy::makeArchiveList,
        },
        {
            "MiniHoneyBeeExGalaxy",
            MiniatureGalaxy::makeArchiveList,
        },
        {
            "MiniSandClockGalaxy",
            MiniatureGalaxy::makeArchiveList,
        },
        {
            "MiniKoopaBattleVs2Galaxy",
            MiniatureGalaxy::makeArchiveList,
        },
        {
            "MiniReverseKingdomGalaxy",
            MiniatureGalaxy::makeArchiveList,
        },
        {
            "MiniOceanRingGalaxy",
            MiniatureGalaxy::makeArchiveList,
        },
        {
            "MiniFactoryGalaxy",
            MiniatureGalaxy::makeArchiveList,
        },
        {
            "MiniSkullSharkGalaxy",
            MiniatureGalaxy::makeArchiveList,
        },
        {
            "MiniFloaterOtaKingGalaxy",
            MiniatureGalaxy::makeArchiveList,
        },
        {
            "MiniOceanPhantomCaveGalaxy",
            MiniatureGalaxy::makeArchiveList,
        },
        {
            "MiniCannonFleetGalaxy",
            MiniatureGalaxy::makeArchiveList,
        },
        {
            "MiniDarkRoomGalaxy",
            MiniatureGalaxy::makeArchiveList,
        },
        {
            "MiniHellProminenceGalaxy",
            MiniatureGalaxy::makeArchiveList,
        },
        {
            "MiniKoopaBattleVs3Galaxy",
            MiniatureGalaxy::makeArchiveList,
        },
        {
            "Hopper",
            MorphItemObjNeo::makeArchiveList,
        },
        {
            "BenefitItemInvincible",
            MorphItemObjNeo::makeArchiveList,
        },
        {
            "MorphItemNeoBee",
            MorphItemObjNeo::makeArchiveList,
        },
        {
            "MorphItemNeoFire",
            MorphItemObjNeo::makeArchiveList,
        },
        {
            "MorphItemNeoFoo",
            MorphItemObjNeo::makeArchiveList,
        },
        {
            "MorphItemNeoHopper",
            MorphItemObjNeo::makeArchiveList,
        },
        {
            "MorphItemNeoIce",
            MorphItemObjNeo::makeArchiveList,
        },
        {
            "MorphItemNeoTeresa",
            MorphItemObjNeo::makeArchiveList,
        },
    };
};  // namespace

MorphItemNeoHopper::~MorphItemNeoHopper() {}
MorphItemNeoIce::~MorphItemNeoIce() {}
MorphItemNeoFire::~MorphItemNeoFire() {}
MorphItemNeoFoo::~MorphItemNeoFoo() {}
MorphItemNeoBee::~MorphItemNeoBee() {}
MorphItemNeoTeresa::~MorphItemNeoTeresa() {}

EffectObjR100F50SyncClipping::~EffectObjR100F50SyncClipping() {}
EffectObjR500F50::~EffectObjR500F50() {}
EffectObjR1000F50::~EffectObjR1000F50() {}
EffectObj20x20x10SyncClipping::~EffectObj20x20x10SyncClipping() {}
EffectObj50x50x10SyncClipping::~EffectObj50x50x10SyncClipping() {}
EffectObj10x10x10SyncClipping::~EffectObj10x10x10SyncClipping() {}

bool EffectObj10x10x10SyncClipping::isSyncClipping() const {
    return true;
}

TVec3f* EffectObj10x10x10SyncClipping::getClippingCenterOffset() const {
    return &(TVec3f(0.0f, 580.0f, 0.0f));
}

f32 EffectObj10x10x10SyncClipping::getFarClipDistance() const {
    return 50.0f;
}

f32 EffectObj10x10x10SyncClipping::getClippingRadius() const {
    return 1000.0f;
}

bool EffectObj50x50x10SyncClipping::isSyncClipping() const {
    return true;
}

TVec3f* EffectObj50x50x10SyncClipping::getClippingCenterOffset() const {
    return &(TVec3f(0.0f, 200.0f, 0.0f));
}

f32 EffectObj50x50x10SyncClipping::getFarClipDistance() const {
    return 50.0f;
}

f32 EffectObj50x50x10SyncClipping::getClippingRadius() const {
    return 2500.0f;
}

bool EffectObj20x20x10SyncClipping::isSyncClipping() const {
    return true;
}

TVec3f* EffectObj20x20x10SyncClipping::getClippingCenterOffset() const {
    return &(TVec3f(0.0f, 200.0f, 0.0f));
}

f32 EffectObj20x20x10SyncClipping::getFarClipDistance() const {
    return 50.0f;
}

f32 EffectObj20x20x10SyncClipping::getClippingRadius() const {
    return 1000.0f;
}

f32 EffectObjR1000F50::getFarClipDistance() const {
    return 50.0f;
}

f32 EffectObjR1000F50::getClippingRadius() const {
    return 1000.0f;
}

f32 EffectObjR500F50::getFarClipDistance() const {
    return 50.0f;
}

f32 EffectObjR500F50::getClippingRadius() const {
    return 500.0f;
}

bool EffectObjR100F50SyncClipping::isSyncClipping() const {
    return true;
}

f32 EffectObjR100F50SyncClipping::getFarClipDistance() const {
    return 50.0f;
}

f32 EffectObjR100F50SyncClipping::getClippingRadius() const {
    return 100.0f;
}

namespace NameObjFactory {
    CreatorFuncPtr getCreator(const char* pName) {
        if (PlanetMapCreatorFunction::isRegisteredObj(pName)) {
            return PlanetMapCreatorFunction::getPlanetMapCreator(pName);
        }

        const Name2CreateFunc* pName2CreateFunc = getName2CreateFunc(pName, nullptr);

        if (pName2CreateFunc == nullptr) {
            return nullptr;
        }

        return pName2CreateFunc->mCreateFunc;
    }

    void requestMountObjectArchives(const char* pName, const JMapInfoIter& rIter) {
        NameObjArchiveListCollector archiveList;
        getMountObjectArchiveList(&archiveList, pName, rIter);

        for (s32 i = 0; i < archiveList.mCount; i++) {
            MR::mountAsyncArchiveByObjectOrLayoutName(archiveList.getArchive(i), nullptr);
        }
    }

    bool isReadResourceFromDVD(const char* pName, const JMapInfoIter& rIter) {
        NameObjArchiveListCollector archiveList;
        getMountObjectArchiveList(&archiveList, pName, rIter);

        for (s32 i = 0; i < archiveList.mCount; i++) {
            if (!MR::isLoadedObjectOrLayoutArchive(archiveList.getArchive(i))) {
                return true;
            }
        }

        return false;
    }

    bool isPlayerArchiveLoaderObj(const char* pArchive) {
        for (s32 i = 0; i < sizeof(cPlayerArchiveLoaderObjTable) / sizeof(*cPlayerArchiveLoaderObjTable); i++) {
            if (MR::isEqualStringCase(cPlayerArchiveLoaderObjTable[i], pArchive)) {
                return true;
            }
        }

        return false;
    }

    const Name2CreateFunc* getName2CreateFunc(const char* pName, const Name2CreateFunc* pTable) {
        if (pTable == nullptr) {
            pTable = cCreateTable;
        }

        for (const Name2CreateFunc* pName2CreateFunc = pTable; pName2CreateFunc != cCreateTable + sizeof(cCreateTable) / sizeof(*cCreateTable);
             pName2CreateFunc++) {
            if (pName2CreateFunc->mName == nullptr) {
                continue;
            }

            if (!MR::isEqualStringCase(pName2CreateFunc->mName, pName)) {
                continue;
            }

            return pName2CreateFunc;
        }

        return nullptr;
    }

    void getMountObjectArchiveList(NameObjArchiveListCollector* pArchiveList, const char* pName, const JMapInfoIter& rIter) {
        if (PlanetMapCreatorFunction::isRegisteredObj(pName)) {
            PlanetMapCreatorFunction::makeArchiveList(pArchiveList, rIter, pName);
        } else {
            const Name2CreateFunc* pName2CreateFunc = getName2CreateFunc(pName, nullptr);

            if (pName2CreateFunc != nullptr && pName2CreateFunc->mArchiveName != nullptr) {
                pArchiveList->addArchive(pName2CreateFunc->mArchiveName);
            }

            for (const Name2Archive* pName2Archive = cName2ArchiveNamesTable;
                 pName2Archive != cName2ArchiveNamesTable + sizeof(cName2ArchiveNamesTable) / sizeof(*cName2ArchiveNamesTable); pName2Archive++) {
                if (MR::isEqualString(pName2Archive->mObjectName, pName)) {
                    pArchiveList->addArchive(pName2Archive->mArchiveName);
                }
            }

            for (const Name2MakeArchiveListFunc* pName2ArchiveFunc = cName2MakeArchiveListFuncTable;
                 pName2ArchiveFunc !=
                 cName2MakeArchiveListFuncTable + sizeof(cName2MakeArchiveListFuncTable) / sizeof(*cName2MakeArchiveListFuncTable);
                 pName2ArchiveFunc++) {
                if (MR::isEqualString(pName2ArchiveFunc->mName, pName)) {
                    pName2ArchiveFunc->mArchiveFunc(pArchiveList, rIter);
                }
            }
        }
    }
};  // namespace NameObjFactory
