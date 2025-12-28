#include "Game/NameObj/NameObjFactory.hpp"
#include "Game/AreaObj.hpp"
#include "Game/Boss.hpp"
#include "Game/Demo.hpp"
#include "Game/Effect.hpp"
#include "Game/Enemy.hpp"
#include "Game/Gravity.hpp"
#include "Game/Map.hpp"
#include "Game/MapObj.hpp"
#include "Game/NPC.hpp"
#include "Game/Player.hpp"
#include "Game/Ride.hpp"

namespace {
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
            createBaseOriginCube< BigBubbleCameraArea >,
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
            nullptr,
            "Caretaker",
        },
        {
            "Rabbit",
            nullptr,
            "MoonRabbit",
        },
        {
            "TalkSyati",
            nullptr,
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
            nullptr,
            "TrickRabbit",
        },
        {
            "RunawayRabbitCollect",
            nullptr,
            "TrickRabbit",
        },
        {
            "TrickRabbitGhost",
            createNameObj< TrickRabbit >,
            "TrickRabbit",
        },
        {
            "Kinopio",
            nullptr,
            "Kinopio",
        },
        {
            "KinopioAstro",
            nullptr,
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
            nullptr,
            "PenguinCoach",
        },
        {
            "PenguinMaster",
            createNameObj< PenguinMaster >,
            "PenguinMaster",
        },
        {
            "PenguinRacer",
            nullptr,
            "Penguin",
        },
        {
            "PenguinRacerLeader",
            nullptr,
            "Penguin",
        },
        {
            "PenguinSkater",
            nullptr,
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
            nullptr,
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
            nullptr,
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
            nullptr,
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
            nullptr,
            "TeresaRacer",
        },
        {
            "NPCSupportRail",
            createNameObj< NPCSupportRail >,
            nullptr,
        },
        {
            "GhostPlayer",
            nullptr,
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
            nullptr,
            "KoopaJrShip",
        },
        {
            "Creeper",
            createNameObj< Creeper >,
            "Creeper",
        },
        {
            "JetTurtle",
            nullptr,
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
            nullptr,
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
            nullptr,
            "SwingRope",
        },
        {
            "SpaceCocoon",
            nullptr,
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
            nullptr,
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
            nullptr,
            "Takobo",
        },
        {
            "Karikari",
            nullptr,
            "Karipon",
        },
        {
            "KoteBug",
            nullptr,
            "KoteBug",
        },
        {
            "Balloonfish",
            nullptr,
            "Balloonfish",
        },
        {
            "BombHei",
            nullptr,
            "BombHei",
        },
        {
            "Bomb",
            nullptr,
            "BombHei",
        },
        {
            "Jiraira",
            createNameObj< Jiraira >,
            "Jiraira",
        },
        {
            "BombHeiLauncher",
            nullptr,
            "BombHeiLauncher",
        },
        {
            "BombLauncher",
            nullptr,
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
            nullptr,
            "KuriboChief",
        },
        {
            "KuriboMini",
            nullptr,
            "KuriboMini",
        },
        {
            "Kabokuri",
            nullptr,
            "Kabokuri",
        },
        {
            "TakoHei",
            nullptr,
            "TakoHei",
        },
        {
            "OtaRock",
            nullptr,
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
            createNameObj< Birikyu >,
            "Birikyu",
        },
        {
            "HomingKiller",
            nullptr,
            "HomingKiller",
        },
        {
            "CocoSambo",
            nullptr,
            "CocoSamboBody",
        },
        {
            "SamboHead",
            nullptr,
            "SamboHead",
        },
        {
            "BegomanBaby",
            nullptr,
            "BegomanBaby",
        },
        {
            "BegomanSpring",
            nullptr,
            "BegomanSpring",
        },
        {
            "BegomanSpringHide",
            nullptr,
            "BegomanSpring",
        },
        {
            "BegomanSpike",
            nullptr,
            "BegomanSpike",
        },
        {
            "BegomanLauncher",
            nullptr,
            "BegomanLauncher",
        },
        {
            "TogeBegomanLauncher",
            nullptr,
            "BegomanLauncher",
        },
        {
            "BegomanBabyLauncher",
            nullptr,
            "BegomanLauncher",
        },
        {
            "FireBubble",
            nullptr,
            "FireBubble",
        },
        {
            "BallBeamer",
            createNameObj< BallBeamer >,
            "BallBeamer",
        },
        {
            "JumpBeamer",
            nullptr,
            "JumpBeamerBody",
        },
        {
            "JumpGuarder",
            nullptr,
            "JumpGuarder",
        },
        {
            "RingBeamer",
            createNameObj< RingBeamer >,
            "RingBeamer",
        },
        {
            "Pukupuku",
            nullptr,
            "Pukupuku",
        },
        {
            "BombTeresa",
            createNameObj< BombTeresa >,
            "BombTeresa",
        },
        {
            "Teresa",
            nullptr,
            "Teresa",
        },
        {
            "TeresaChief",
            nullptr,
            "Teresa",
        },
        {
            "TeresaWater",
            nullptr,
            "TeresaWater",
        },
        {
            "Torpedo",
            nullptr,
            "Torpedo",
        },
        {
            "Dossun",
            createNameObj< Dossun >,
            "Dossun",
        },
        {
            "Onimasu",
            nullptr,
            "Onimasu",
        },
        {
            "OnimasuPivot",
            nullptr,
            "Onimasu",
        },
        {
            "Unizo",
            nullptr,
            "Unizo",
        },
        {
            "UnizoLand",
            nullptr,
            "UnizoLand",
        },
        {
            "UnizoShoal",
            nullptr,
            "UnizoShoal",
        },
        {
            "UnizoLauncher",
            nullptr,
            "UnizoLand",
        },
        {
            "Kiraira",
            nullptr,
            "Kiraira",
        },
        {
            "KirairaRail",
            nullptr,
            "Kiraira",
        },
        {
            "NokonokoLand",
            nullptr,
            "NokonokoLand",
        },
        {
            "Gesso",
            nullptr,
            "Gesso",
        },
        {
            "Poihana",
            createNameObj< Poihana >,
            "Poihana",
        },
        {
            "Snakehead",
            nullptr,
            "Snakehead",
        },
        {
            "SnakeheadSmall",
            nullptr,
            "SnakeheadSmall",
        },
        {
            "Jellyfish",
            nullptr,
            "Jellyfish",
        },
        {
            "JellyfishElectric",
            nullptr,
            "JellyfishElectric",
        },
        {
            "Kanina",
            nullptr,
            "Kanina",
        },
        {
            "KaninaRed",
            nullptr,
            "KaninaRed",
        },
        {
            "Mogu",
            nullptr,
            "Mogu",
        },
        {
            "Mogucchi",
            nullptr,
            "Mogucchi",
        },
        {
            "MogucchiRefuseTerritory",
            nullptr,
            nullptr,
        },
        {
            "KoopaJrShip",
            nullptr,
            "KoopaJrShip",
        },
        {
            "Hanachan",
            nullptr,
            "HanachanHead",
        },
        {
            "JumpSpider",
            nullptr,
            "JumpSpider",
        },
        {
            "StringSpider",
            nullptr,
            "StringSpider",
        },
        {
            "PackunPetit",
            createNameObj< PackunPetit >,
            "PackunPetit",
        },
        {
            "StinkBugSmall",
            nullptr,
            "StinkBugSmall",
        },
        {
            "StinkBugParent",
            nullptr,
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
            nullptr,
            "Metbo",
        },
        {
            "BombBird",
            createNameObj< BombBird >,
            "BombBird",
        },
        {
            "MechanicKoopaMini",
            nullptr,
            "MechanicKoopaMini",
        },
        {
            "Karon",
            createNameObj< Karon >,
            "Karon",
        },
        {
            "HammerHeadPackun",
            nullptr,
            "PackunFlower",
        },
        {
            "DharmaSambo",
            nullptr,
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
            nullptr,
            "MagnumKiller",
        },
        {
            "BossBegoman",
            nullptr,
            "BossBegoman",
        },
        {
            "BossStinkBug",
            nullptr,
            "BossStinkBug",
        },
        {
            "DinoPackun",
            nullptr,
            "DinoPackun",
        },
        {
            "DinoPackunVs2",
            nullptr,
            "DinoPackun2",
        },
        {
            "Dodoryu",
            nullptr,
            "Dodoryu",
        },
        {
            "KoopaVs1",
            nullptr,
            "Koopa",
        },
        {
            "KoopaVs2",
            nullptr,
            "Koopa",
        },
        {
            "KoopaVs3",
            nullptr,
            "Koopa",
        },
        {
            "OtaKing",
            nullptr,
            nullptr,
        },
        {
            "Polta",
            nullptr,
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
            nullptr,
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
            nullptr,
            "Kameck",
        },
        {
            "TurtleBeamKameck",
            nullptr,
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
            nullptr,
            "SpiderThreadAttachPoint",
        },
        {
            "SpiderCoin",
            nullptr,
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
            nullptr,
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
            nullptr,
            "JumpHole",
        },
        {
            "BallOpener",
            createNameObj< BallOpener >,
            "BallOpener",
        },
        {
            "DragonHeadFlower",
            nullptr,
            "DragonHeadFlower",
        },
        {
            "ConcreteBlock",
            nullptr,
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
            nullptr,
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
            nullptr,
            "Flower",
        },
        {
            "FlowerBlueGroup",
            nullptr,
            "FlowerBlue",
        },
        {
            "CutBushGroup",
            nullptr,
            "CutBush",
        },
        {
            "SmallStone",
            nullptr,
            "SmallStone",
        },
        {
            "CircleShell",
            nullptr,
            "CircleShell",
        },
        {
            "CircleStrawberry",
            nullptr,
            "CircleStrawberry",
        },
        {
            "Tsukidashikun",
            createNameObj< Tsukidashikun >,
            "Tsukidashikun",
        },
        {
            "TsukidashikunBig",
            nullptr,
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
            nullptr,
            "ElectricRail",
        },
        {
            "ElectricRailMoving",
            nullptr,
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
            nullptr,
            nullptr,
        },
        {
            "BigBubbleMoveLimitterPlane",
            nullptr,
            nullptr,
        },
        {
            "BigBubbleGenerator",
            nullptr,
            "AirBubbleGenerator",
        },
        {
            "BigObstructBubbleGenerator",
            nullptr,
            "AirBubbleGenerator",
        },
        {
            "GCaptureTarget",
            nullptr,
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
            nullptr,
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
            nullptr,
            "ItemBubble",
        },
        {
            "SpinningBox",
            nullptr,
            "SpinningBox",
        },
        {
            "ElectricBall",
            nullptr,
            "ElectricBall",
        },
        {
            "SpaceMine",
            createNameObj< SpaceMine >,
            "SpaceMine",
        },
        {
            "MorphItemNeoHopper",
            nullptr,
            "PowerupHopper",
        },
        {
            "MorphItemNeoBee",
            nullptr,
            "PowerUpBee",
        },
        {
            "MorphItemNeoTeresa",
            nullptr,
            "PowerupTeresa",
        },
        {
            "MorphItemNeoIce",
            nullptr,
            "PowerupIce",
        },
        {
            "MorphItemNeoFire",
            nullptr,
            "PowerupFire",
        },
        {
            "MorphItemNeoFoo",
            nullptr,
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
            nullptr,
            "Butterfly",
        },
        {
            "Pompon2Plant",
            nullptr,
            "Pompon2Plant",
        },
        {
            "Pompon4Plant",
            nullptr,
            "Pompon4Plant",
        },
        {
            "LavaGeyser",
            nullptr,
            "LavaGeyser",
        },
        {
            "LavaProminence",
            nullptr,
            "LavaProminence",
        },
        {
            "LavaProminenceTriple",
            nullptr,
            "LavaProminenceTriple",
        },
        {
            "LavaProminenceEnvironment",
            nullptr,
            "LavaProminenceEnvironment",
        },
        {
            "LavaProminenceWithoutShadow",
            nullptr,
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
            nullptr,
            "LavaHomeFloaterB",
        },
        {
            "LavaFloatingStepA",
            nullptr,
            "LavaFloatingStepA",
        },
        {
            "LavaFloatingStepB",
            nullptr,
            "LavaFloatingStepB",
        },
        {
            "LavaFloatingStepC",
            nullptr,
            "LavaFloatingStepC",
        },
        {
            "LavaFloatingStepD",
            nullptr,
            "LavaFloatingStepD",
        },
        {
            "LavaHomeSeesaw",
            nullptr,
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
            nullptr,
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
            nullptr,
            "LavaStrangeRock",
        },
        {
            "LavaStrangeRockL",
            nullptr,
            "LavaStrangeRockL",
        },
        {
            "LavaStrangeRockTable",
            nullptr,
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
            nullptr,
            "Shellfish",
        },
        {
            "ShellfishYellowChip",
            nullptr,
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
            nullptr,
            "OceanFloaterTypeU",
        },
        {
            "DashRing",
            createNameObj< DashRing >,
            "DashRing",
        },
        {
            "MeteorStrike",
            nullptr,
            "MeteorStrike",
        },
        {
            "MeteorStrikeEnvironment",
            nullptr,
            "MeteorStrike",
        },
        {
            "MeteorCannon",
            nullptr,
            "MeteorStrike",
        },
        {
            "BrightObj",
            nullptr,
            "LensFlare",
        },
        {
            "BrightSun",
            nullptr,
            "LensFlare",
        },
        {
            "PhantomTorch",
            nullptr,
            nullptr,
        },
        {
            "PhantomBonfire",
            nullptr,
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
            nullptr,
            "MarioLauncher",
        },
        {
            "MarioLauncherAttractor",
            createNameObj< MarioLauncherAttractor >,
            "MarioLauncherAttractor",
        },
        {
            "WaterBazooka",
            nullptr,
            "WaterBazooka",
        },
        {
            "ElectricBazooka",
            nullptr,
            "ElectricBazooka",
        },
        {
            "WaterPressure",
            nullptr,
            "WaterPressure",
        },
        {
            "FirePressure",
            createNameObj< FirePressure >,
            "FirePressure",
        },
        {
            "IronCannonLauncher",
            nullptr,
            "IronCannonLauncher",
        },
        {
            "IronCannonLauncherPoint",
            nullptr,
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
            nullptr,
            "SunkenShip",
        },
        {
            "BeeFlowerHover",
            createNameObj< BeeFlowerHover >,
            "BeeFlowerHover",
        },
        {
            "WaterfallCaveCover",
            nullptr,
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
            nullptr,
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
            nullptr,
            "OceanSmallTurtle",
        },
        {
            "OceanHexagonFloater",
            nullptr,
            "OceanHexagonFloater",
        },
        {
            "SpringWaterFloaterA",
            nullptr,
            "SpringWaterFloaterA",
        },
        {
            "SpringWaterFloaterB",
            nullptr,
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
            nullptr,
            "IcicleRock",
        },
        {
            "ItemAppearStoneSpinDriver",
            createNameObj< ItemAppearStone >,
            "ItemAppearStoneSpinDriver",
        },
        {
            "SnowFloor",
            nullptr,
            nullptr,
        },
        {
            "SnowFloorTile",
            nullptr,
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
            nullptr,
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
            nullptr,
            "SandUpDownTriRock",
        },
        {
            "JumpStand",
            nullptr,
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
            nullptr,
            "SandRiverLightA",
        },
        {
            "SnowMan",
            createNameObj< SnowMan >,
            "SnowMan",
        },
        {
            "SnowBlockA",
            nullptr,
            "SnowBlockA",
        },
        {
            "ShockWaveGenerator",
            nullptr,
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
            nullptr,
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
            nullptr,
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
            nullptr,
            "QuestionBoxMoveStepA",
        },
        {
            "QuestionBoxMoveStepB",
            nullptr,
            "QuestionBoxMoveStepB",
        },
        {
            "QuestionBoxMoveStepC",
            nullptr,
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
            nullptr,
            "TeresaMansionLightA",
        },
        {
            "TeresaMansionLightB",
            nullptr,
            "TeresaMansionLightB",
        },
        {
            "TeresaMansionCandlestand",
            createNameObj< Candlestand >,
            "TeresaMansionCandlestand",
        },
        {
            "TeresaMansionHoleCover",
            nullptr,
            "TeresaMansionHoleCover",
        },
        {
            "KoopaStatue",
            nullptr,
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
            nullptr,
            "SeesawMoveNutA",
        },
        {
            "SeesawMoveNutB",
            nullptr,
            "SeesawMoveNutB",
        },
        {
            "SeesawMoveNutC",
            nullptr,
            "SeesawMoveNutC",
        },
        {
            "TwisterSan",
            nullptr,
            "TwisterSan",
        },
        {
            "TwisterSanSlim",
            nullptr,
            "TwisterSanSlim",
        },
        {
            "SunakazeKun",
            nullptr,
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
            nullptr,
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
            nullptr,
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
            nullptr,
            "SandUpDownTowerBreakableWallA",
        },
        {
            "SandUpDownTowerBreakableWallB",
            nullptr,
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
            createNameObj< SimpleMapObj >,
            "ChoConveyorMoveChocoA",
        },
        {
            "HoneyQueenLeafA",
            createNameObj< RailMoveObj >,
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
            nullptr,
            "GravityLightA",
        },
        {
            "GravityLightRoad",
            nullptr,
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
            nullptr,
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
            nullptr,
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
            createNameObj< MechaKoopaPartsHead >,
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
            nullptr,
            "SpaceShipStep",
        },
        {
            "QuarterRollGravityRoomArrow",
            nullptr,
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
            nullptr,
            "IceVolcanoFloaterA",
        },
        {
            "IceVolcanoFloaterB",
            nullptr,
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
            nullptr,
            "IceWaterUpDownParts",
        },
        {
            "LavaUpDownParts",
            nullptr,
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
            nullptr,
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
            nullptr,
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
            nullptr,
            "PhantomShipHandle",
        },
        {
            "PhantomShipBridge",
            createNameObj< PhantomShipBridge >,
            "PhantomShipBridge",
        },
        {
            "TakoBarrelA",
            nullptr,
            "TakoBarrelA",
        },
        {
            "TakoBarrelB",
            createNameObj< SimpleMapObj >,
            "TakoBarrelB",
        },
        {
            "KoopaRestarterVs3",
            nullptr,
            nullptr,
        },
        {
            "KoopaPowerUpSwitch",
            nullptr,
            nullptr,
        },
        {
            "KoopaSwitchKeeper",
            nullptr,
            nullptr,
        },
        {
            "KoopaViewSwitchKeeper",
            nullptr,
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
            nullptr,
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
            nullptr,
            "PhantomShipBoxFloaterA",
        },
        {
            "PhantomShipBoxFloaterB",
            nullptr,
            "PhantomShipBoxFloaterB",
        },
        {
            "PhantomShipBoxFloaterC",
            nullptr,
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
            createNameObj< ProjectionMapSky >,
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
            createNameObj< Sky >,
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
            nullptr,
            nullptr,
        },
        {
            "CircleCoinGroup",
            MR::createCircleCoinGroup,
            nullptr,
        },
        {
            "PurpleRailCoin",
            nullptr,
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
            nullptr,
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
            nullptr,
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
            nullptr,
            nullptr,
        },
        {
            "StarPieceMother",
            nullptr,
            "StarPieceMother",
        },
        {
            "StarPieceSpot",
            nullptr,
            nullptr,
        },
        {
            "OceanBowl",
            nullptr,
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
            nullptr,
            "WaterWave",
        },
        {
            "WhirlPool",
            nullptr,
            nullptr,
        },
        {
            "WhirlPoolAccelerator",
            nullptr,
            "Whirlpool",
        },
        {
            "FishGroupA",
            nullptr,
            "FishA",
        },
        {
            "FishGroupB",
            nullptr,
            "FishB",
        },
        {
            "FishGroupC",
            nullptr,
            "FishC",
        },
        {
            "FishGroupD",
            nullptr,
            "FishD",
        },
        {
            "FishGroupE",
            nullptr,
            "FishE",
        },
        {
            "FishGroupF",
            nullptr,
            "FishF",
        },
        {
            "Flag",
            nullptr,
            "Flag",
        },
        {
            "FlagKoopaA",
            nullptr,
            "FlagKoopaA",
        },
        {
            "FlagKoopaB",
            nullptr,
            "FlagKoopaB",
        },
        {
            "FlagRaceA",
            nullptr,
            "FlagRaceA",
        },
        {
            "FlagKoopaCastle",
            nullptr,
            "FlagKoopaCastle",
        },
        {
            "FlagPeachCastleA",
            nullptr,
            "FlagPeachCastleA",
        },
        {
            "FlagPeachCastleB",
            nullptr,
            "FlagPeachCastleB",
        },
        {
            "FlagPeachCastleC",
            nullptr,
            "FlagPeachCastleC",
        },
        {
            "FlagSurfing",
            nullptr,
            "FlagSurfing",
        },
        {
            "FlagTamakoro",
            nullptr,
            "FlagTamakoro",
        },
        {
            "SeaGullGroup",
            createNameObj< SeaGullGroup >,
            "SeaGull",
        },
        {
            "WaterPlant",
            nullptr,
            "WaterPlant",
        },
        {
            "HitWallTimerSwitch",
            nullptr,
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
            nullptr,
            "HipDropTimerSwitch",
        },
        {
            "DesertLandUpSwitch",
            nullptr,
            "DesertLandUpSwitch",
        },
        {
            "DesertLandDownSwitch",
            nullptr,
            "DesertLandDownSwitch",
        },
        {
            "ScrewSwitch",
            createNameObj< ScrewSwitch >,
            "ScrewSwitch",
        },
        {
            "ScrewSwitchReverse",
            nullptr,
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
            nullptr,
            nullptr,
        },
        {
            "ShootingStarArea",
            nullptr,
            nullptr,
        },
        {
            "IcePlanetLight",
            nullptr,
            nullptr,
        },
        {
            "IceLayerBreak",
            nullptr,
            nullptr,
        },
        {
            "LavaSparksS",
            nullptr,
            nullptr,
        },
        {
            "LavaSparksL",
            nullptr,
            nullptr,
        },
        {
            "SpaceDustS",
            nullptr,
            nullptr,
        },
        {
            "SpaceDustL",
            nullptr,
            nullptr,
        },
        {
            "BirdLouseS",
            nullptr,
            nullptr,
        },
        {
            "BirdLouseL",
            nullptr,
            nullptr,
        },
        {
            "SnowS",
            nullptr,
            nullptr,
        },
        {
            "FallingSmallRock",
            createNameObj< FallingSmallRock >,
            nullptr,
        },
        {
            "LavaHomeVolcanoFallingRock",
            nullptr,
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
            nullptr,
            nullptr,
        },
        {
            "WaterfallL",
            nullptr,
            nullptr,
        },
        {
            "DrainPipeBubbleA",
            nullptr,
            nullptr,
        },
        {
            "DrainPipeBubbleB",
            nullptr,
            nullptr,
        },
        {
            "WaterDropTop",
            nullptr,
            nullptr,
        },
        {
            "WaterDropBottom",
            nullptr,
            nullptr,
        },
        {
            "WaterDropMiddle",
            nullptr,
            nullptr,
        },
        {
            "SubmarineSteam",
            nullptr,
            nullptr,
        },
        {
            "SubmarineVolcano",
            nullptr,
            nullptr,
        },
        {
            "WaterLayerBreak",
            nullptr,
            nullptr,
        },
        {
            "StarPieceClusterBreak",
            nullptr,
            nullptr,
        },
        {
            "FireworksA",
            nullptr,
            nullptr,
        },
        {
            "BattleShipExplosionRock",
            nullptr,
            nullptr,
        },
        {
            "BattleShipExplosionMetal",
            nullptr,
            nullptr,
        },
        {
            "ForestWaterfallS",
            nullptr,
            nullptr,
        },
        {
            "ForestWaterfallL",
            nullptr,
            nullptr,
        },
        {
            "SandBreezeS",
            nullptr,
            nullptr,
        },
        {
            "SandBreezeL",
            nullptr,
            nullptr,
        },
        {
            "SpringJetWater",
            nullptr,
            nullptr,
        },
        {
            "SandUpDownKGBaseEffect",
            nullptr,
            nullptr,
        },
        {
            "SandUpDownTowerTopEffect",
            nullptr,
            nullptr,
        },
        {
            "UFOSandObstacleAEffect",
            nullptr,
            nullptr,
        },
        {
            "UFOSandObstacleBEffect",
            nullptr,
            nullptr,
        },
        {
            "UFOSandObstacleCEffect",
            nullptr,
            nullptr,
        },
        {
            "GravityDust",
            nullptr,
            nullptr,
        },
        {
            "Steam",
            createNameObj< SimpleEffectObj >,
            nullptr,
        },
        {
            "FallRedLeaf",
            nullptr,
            nullptr,
        },
        {
            "FallGreenLeaf",
            nullptr,
            nullptr,
        },
        {
            "IcicleRockLight",
            nullptr,
            nullptr,
        },
        {
            "TwinFallLakeWaterFall",
            nullptr,
            nullptr,
        },
        {
            "EffectTeresa",
            nullptr,
            nullptr,
        },
        {
            "UFOKinokoLandingBlackSmoke",
            nullptr,
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
            nullptr,
            nullptr,
        },
        {
            "ClipFieldFillDraw",
            nullptr,
            nullptr,
        },
        {
            "SpotLightVolumeDraw",
            nullptr,
            nullptr,
        },
        {
            "GroupSwitchWatcher",
            nullptr,
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
            nullptr,
            "CinemaSuper",
        },
        {
            "DemoGroup",
            nullptr,
            nullptr,
        },
        {
            "DemoSubGroup",
            createNameObj< DemoCastSubGroup >,
            nullptr,
        },
        {
            "AstroDomeDemoStarter",
            nullptr,
            "SuperSpinDriver",
        },
        {
            "AstroDomeGalaxySelector",
            createNameObj< AstroDomeGalaxySelector >,
            nullptr,
        },
        {
            "StarReturnDemoStarter",
            nullptr,
            nullptr,
        },
        {
            "GrandStarReturnDemoStarter",
            nullptr,
            "GrandStar",
        },
        {
            "ScenarioStarter",
            nullptr,
            "Welcome",
        },
        {
            "FileSelector",
            nullptr,
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
        {"DemoRabbit", /* DemoRabbit::makeArchiveList, */},
        {"GhostPlayer", /* GhostPlayer::makeArchiveList, */},
        {"KuriboChief", /* KuriboChief::makeArchiveList, */},
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
        {"OtaKing", /* OtaKing::makeArchiveList, */},
        {"OtaRock", /* OtaRock::makeArchiveList, */},
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
        {"RunawayTico", /* RunawayTico::makeArchiveList, */},
        {
            "Teresa",
            MR::makeArchiveListDummyDisplayModel,
        },
        {"Caretaker", /* Caretaker::makeArchiveList, */},
        {
            "Kinopio",
            NPCActor::makeArchiveList,
        },
        {"KinopioAstro", /* KinopioAstro::makeArchiveList, */},
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
        {"Rosetta", /* Rosetta::makeArchiveList, */},
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
        {"StarReturnDemoStarter", /* StarReturnDemoStarter::makeArchiveList, */},
        {
            "UFOKinoko",
            UFOKinoko::makeArchiveList,
        },
        {
            "UFOKinokoUnderConstruction",
            MR::makeArchiveListUFOKinokoUnderConstruction,
        },
        {"SunakazeKun", /* Sandstorm::makeArchiveList, */},
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
        {"MiniEggStarGalaxy", /* MiniatureGalaxy::makeArchiveList, */},
        {"MiniHoneyBeeKingdomGalaxy", /* MiniatureGalaxy::makeArchiveList, */},
        {"MiniFlipPanelExGalaxy", /* MiniatureGalaxy::makeArchiveList, */},
        {"MiniSurfingLv1Galaxy", /* MiniatureGalaxy::makeArchiveList, */},
        {"MiniTriLegLv1Galaxy", /* MiniatureGalaxy::makeArchiveList, */},
        {"MiniStarDustGalaxy", /* MiniatureGalaxy::makeArchiveList, */},
        {"MiniTamakoroExLv1Galaxy", /* MiniatureGalaxy::makeArchiveList, */},
        {"MiniBattleShipGalaxy", /* MiniatureGalaxy::makeArchiveList, */},
        {"MiniBreakDownPlanetGalaxy", /* MiniatureGalaxy::makeArchiveList, */},
        {"MiniKoopaBattleVs1Galaxy", /* MiniatureGalaxy::makeArchiveList, */},
        {"MiniHeavenlyBeachGalaxy", /* MiniatureGalaxy::makeArchiveList, */},
        {"MiniCubeBubbleExLv1Galaxy", /* MiniatureGalaxy::makeArchiveList, */},
        {"MiniPhantomGalaxy", /* MiniatureGalaxy::makeArchiveList, */},
        {"MiniOceanFloaterLandGalaxy", /* MiniatureGalaxy::makeArchiveList, */},
        {"MiniKoopaJrShipLv1Galaxy", /* MiniatureGalaxy::makeArchiveList, */},
        {"MiniCosmosGardenGalaxy", /* MiniatureGalaxy::makeArchiveList, */},
        {"MiniIceVolcanoGalaxy", /* MiniatureGalaxy::makeArchiveList, */},
        {"MiniHoneyBeeExGalaxy", /* MiniatureGalaxy::makeArchiveList, */},
        {"MiniSandClockGalaxy", /* MiniatureGalaxy::makeArchiveList, */},
        {"MiniKoopaBattleVs2Galaxy", /* MiniatureGalaxy::makeArchiveList, */},
        {"MiniReverseKingdomGalaxy", /* MiniatureGalaxy::makeArchiveList, */},
        {"MiniOceanRingGalaxy", /* MiniatureGalaxy::makeArchiveList, */},
        {"MiniFactoryGalaxy", /* MiniatureGalaxy::makeArchiveList, */},
        {"MiniSkullSharkGalaxy", /* MiniatureGalaxy::makeArchiveList, */},
        {"MiniFloaterOtaKingGalaxy", /* MiniatureGalaxy::makeArchiveList, */},
        {"MiniOceanPhantomCaveGalaxy", /* MiniatureGalaxy::makeArchiveList, */},
        {"MiniCannonFleetGalaxy", /* MiniatureGalaxy::makeArchiveList, */},
        {"MiniDarkRoomGalaxy", /* MiniatureGalaxy::makeArchiveList, */},
        {"MiniHellProminenceGalaxy", /* MiniatureGalaxy::makeArchiveList, */},
        {"MiniKoopaBattleVs3Galaxy", /* MiniatureGalaxy::makeArchiveList, */},
        {"Hopper", /* MorphItemObjNeo::makeArchiveList, */},
        {"BenefitItemInvincible", /* MorphItemObjNeo::makeArchiveList, */},
        {"MorphItemNeoBee", /* MorphItemObjNeo::makeArchiveList, */},
        {"MorphItemNeoFire", /* MorphItemObjNeo::makeArchiveList, */},
        {"MorphItemNeoFoo", /* MorphItemObjNeo::makeArchiveList, */},
        {"MorphItemNeoHopper", /* MorphItemObjNeo::makeArchiveList, */},
        {"MorphItemNeoIce", /* MorphItemObjNeo::makeArchiveList, */},
        {"MorphItemNeoTeresa", /* MorphItemObjNeo::makeArchiveList, */},
    };
};  // namespace

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
