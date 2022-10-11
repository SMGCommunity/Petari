#pragma once

#include "revolution.h"
#include "Game/AreaObj/AstroChangeStageCube.h"
#include "Game/AreaObj/AudioEffectArea.h"
#include "Game/AreaObj/BigBubbleCameraArea.h"
#include "Game/AreaObj/BgmProhibitArea.h"
#include "Game/AreaObj/ChangeBgmCube.h"
#include "Game/AreaObj/CubeCamera.h"
#include "Game/AreaObj/DeathArea.h"
#include "Game/AreaObj/DepthOfFieldArea.h"
#include "Game/AreaObj/LightArea.h"
#include "Game/AreaObj/SwitchArea.h"
#include "Game/Gravity/GlobalGravityObj.h"
#include "Game/Map.h"
#include "Game/MapObj.h"
#include "Game/Map/PlanetMapCreator.h"

class AreaObj;
class NameObj;
class NameObjArchiveListCollector;

typedef NameObj* (*CreationFuncPtr)(const char *);

/// @brief Class that creates NameObj instances.
class NameObjFactory {
public:
    struct Name2CreateFunc {
        const char* mName;                          // _0
        NameObj* (*mCreationFunc)(const char *);   // _4
        const char* mArchiveName;                   // _8
    };

    struct Name2Archive {
        const char* mObjectName;    // _0
        const char* mArchiveName;   // _4
    };

    struct Name2MakeArchiveListFunc {
        const char* mName;                                                          // _0
        void (*mArchiveFunc)(NameObjArchiveListCollector *, const JMapInfoIter &);  // _4
    };

    static CreationFuncPtr getCreator(const char *);
    static void requestMountObjectArchives(const char *, const JMapInfoIter &);
    static bool isReadResourceFromDVD(const char *, const JMapInfoIter &);
    static const Name2CreateFunc* getName2CreateFunc(const char *, const Name2CreateFunc *);
    static void getMountObjectArchiveList(NameObjArchiveListCollector *, const char *, const JMapInfoIter &);
    static bool isPlayerArchiveLoaderObj(const char *);
};

namespace {
    template<typename T>
    NameObj* createCenterOriginCube(const char *pName) {
        return new T(0, pName);
    }

    template<typename T>
    NameObj* createBaseOriginCube(const char *pName) {
        return new T(1, pName);
    }

    template<typename T>
    NameObj* createSphere(const char *pName) {
        return new T(2, pName);
    }

    template<typename T>
    NameObj* createBaseOriginCylinder(const char *pName) {
        return new T(3, pName);
    }

    template<typename T>
    NameObj* createBowl(const char *pName) {
        return new T(4, pName);
    }

    template<typename T>
    NameObj* createNameObj(const char *pName) {
        return new T(pName);
    }

    const char* cPlayerArchiveLoaderObjTable[0x8] =
    {
        "Hopper",
        "BenefitItemInvincible",
        "MorphItemNeoBee",
        "MorphItemNeoFire",
        "MorphItemNeoFoo",
        "MorphItemNeoHopper",
        "MorphItemNeoIce",
        "MorphItemNeoTeresa"
    };


    /* we can organize this after it's done */
    const NameObjFactory::Name2CreateFunc cCreateTable[150] = {
        { "SwitchCube", createBaseOriginCube<SwitchArea>, 0 },
        { "SwitchSphere", createSphere<SwitchArea>, 0 },
        { "SwitchCylinder", createBaseOriginCylinder<SwitchArea>, 0 },
        { "CubeCameraBox", createCenterOriginCube<CubeCameraArea>, 0 },
        { "CubeCameraCylinder", createBaseOriginCylinder<CubeCameraArea>, 0 },
        { "CubeCameraSphere", createSphere<CubeCameraArea>, 0 },
        { "CubeCameraBowl", createBowl<CubeCameraArea>, 0 },

        { "DeathCube", createBaseOriginCube<DeathArea>, 0 },
        { "DeathSphere", createSphere<DeathArea>, 0 },
        { "DeathCylinder", createBaseOriginCylinder<DeathArea>, 0 },

        { "ChangeBgmCube", createBaseOriginCube<ChangeBgmCube>, 0 },
        { "BgmProhibitArea", createSphere<BgmProhibitArea>, 0 },

        { "DepthOfFieldCube", createCenterOriginCube<DepthOfFieldArea>, 0 },
        { "DepthOfFieldSphere", createSphere<DepthOfFieldArea>, 0 },
        { "DepthOfFieldCylinder", createBaseOriginCylinder<DepthOfFieldArea>, 0 },

        { "LightCtrlCube", createBaseOriginCube<LightArea>, 0 },
        { "LightCtrlCylinder", createBaseOriginCylinder<LightArea>, 0 },

        { "BigBubbleCameraBox", createCenterOriginCube<BigBubbleCameraArea>, 0 },
        { "BigBubbleCameraCylinder", createBaseOriginCylinder<BigBubbleCameraArea>, 0 },
        { "BigBubbleCameraSphere", createSphere<BigBubbleCameraArea>, 0 },

        { "AudioEffectCube", createBaseOriginCube<AudioEffectArea>, 0 },
        { "AudioEffectSphere", createSphere<AudioEffectArea>, 0 },
        { "AudioEffectCylinder", createBaseOriginCylinder<AudioEffectArea>, 0 },

        { "AstroChangeStageCube", createBaseOriginCube<AstroChangeStageCube>, 0 },

        { "VROrbit", createNameObj<ProjectionMapSky>, "VROrbit" },
        { "VRDarkSpace", createNameObj<Sky>, "VRDarkSpace" },
        { "VRSandwichSun", createNameObj<ProjectionMapSky>, "VRSandwichSun" },
        { "SummerSky", createNameObj<Sky>, "SummerSky" },
        { "GalaxySky", createNameObj<Sky>, "GalaxySky" },
        { "MilkyWaySky", createNameObj<Sky>, "MilkyWaySky" },
        { "HalfGalaxySky", createNameObj<ProjectionMapSky>, "HalfGalaxySky" },
        { "GreenPlanetOrbitSky", createNameObj<ProjectionMapSky>, "GreenPlanetOrbitSky" },
        { "PhantomSky", createNameObj<Sky>, "PhantomSky" },
        { "KoopaVS1Sky", createNameObj<ProjectionMapSky>, "KoopaVS1Sky" },
        { "KoopaVS2Sky", createNameObj<Sky>, "KoopaVS2Sky" },
        { "FamicomMarioSky", createNameObj<Sky>, "FamicomMarioSky" },
        { "DesertSky", createNameObj<Sky>, "DesertSky" },
        { "ChildRoomSky", createNameObj<Sky>, "ChildRoomSky" },
        { "AuroraSky", createNameObj<Sky>, "AuroraSky" },
        { "CloudSky", createNameObj<ProjectionMapSky>, "CloudSky" },
        { "RockPlanetOrbitSky", createNameObj<ProjectionMapSky>, "RockPlanetOrbitSky" },
        { "StarrySky", createNameObj<Sky>, "StarrySky" },
        { "AstroDomeSky", createNameObj<Sky>, "StarrySky" },

        { "Coin", MR::createDirectSetCoin, "Coin" },
        { "PurpleCoin", MR::createDirectSetPurpleCoin, "PurpleCoin" },

        { "CircleCoinGroup", MR::createCircleCoinGroup, "CircleCoinGroup" },
        { "CirclePurpleCoinGroup", MR::createCirclePurpleCoinGroup, "CirclePurpleCoinGroup" },

        { "BlueChip", createNameObj<BlueChip>, "BlueChip" },
        { "YellowChip", createNameObj<YellowChip>, "YellowChip" },
        //{ "BlueChipGroup", createNameObj<YellowChipGroup}

        { "SeaGullGroup", createNameObj<SeaGullGroup>, "SeaGullGroup" },

        { "CrystalSwitch", createNameObj<CrystalSwitch>, "CrystalSwitch "},

        { "GlobalCubeGravity", MR::createGlobalCubeGravityObj, NULL },
        { "GlobalConeGravity", MR::createGlobalConeGravityObj, NULL },
        { "GlobalDiskGravity", MR::createGlobalDiskGravityObj, NULL },
        { "GlobalDiskTorusGravity", MR::createGlobalDiskTorusGravityObj, NULL },
        { "GlobalPlaneGravity", MR::createGlobalPlaneGravityObj, NULL },
        { "GlobalPlaneGravityInBox", MR::createGlobalPlaneInBoxGravityObj, NULL },
        { "GlobalPlaneGravityInCylinder", MR::createGlobalPlaneInCylinderGravityObj, NULL },
        { "GlobalPointGravity", MR::createGlobalPointGravityObj, NULL },
        { "GlobalSegmentGravity", MR::createGlobalSegmentGravityObj, NULL },
        { "GlobalWireGravity", MR::createGlobalWireGravityObj, NULL },

        { "BlackHole", createNameObj<BigFan>, "BlackHole" },
        { "BlackHoleCube", createNameObj<BigFan>, "BlackHole" },

        { "BigFan", createNameObj<BigFan>, "BigFan" },
        { "BigFanNoLeg", createNameObj<BigFan>, "BigFanNoLeg" },

        { "PowerStar", createNameObj<PowerStar>, "PowerStar" },
        { "GrandStar", createNameObj<PowerStar>, "GrandStar", },
        { "PowerStarAppearPoint", createNameObj<PowerStarAppearPoint>, NULL },

        { "CoconutTree", createNameObj<CoconutTree>, "CoconutTree" },

        { "FlipPanel", createNameObj<FlipPanel>, "FlipPanel" },
        { "FlipPanelObserver", createNameObj<FlipPanelObserver>, NULL },
        { "FlipPanelReverse", createNameObj<FlipPanel>, "FlipPanelReverse" },

        { "TreasureSpot", createNameObj<TreasureSpot>, "TreasureSpot" },
        { "CoinFlower", createNameObj<TreasureSpot>, "CoinFlower" },

        { "FireBar", createNameObj<FireBar>, "FireBarCore" },

        { "TimerSwitch", createNameObj<TimerSwitch>, "" },

        { "LavaHomeVolcanoInnerFlow", createNameObj<RailMoveObj>, "LavaHomeVolcanoInnerFlow" },
        { "LavaRotatePlanetStartStep", createNameObj<RailMoveObj>, "LavaRotatePlanetStartStep" },

        { "CrystalCageS", createNameObj<CrystalCage>, "CrystalCageS" },
        { "CrystalCageM", createNameObj<CrystalCage>, "CrystalCageM" },
        { "CrystalCageL", createNameObj<CrystalCage>, "CrystalCageL" },
        { "CrystalCageMoving", createNameObj<CrystalCageMoving>, "CrystalCageMoving" },

        { "DashRing", createNameObj<DashRing>, "DashRing" },

        { "ChooChooTrain", createNameObj<ChooChooTrain>, "ChooChooTrain" },

        { "DangerSignBoard", createNameObj<SimpleMapObjPush>, "DangerSignBoard" },

        { "MirrorModelTest", createNameObj<SimpleMirrorReflectionObj>, "MirrorModelTest" },
        { "MirrorModelPeachCastle", createNameObj<SimpleMirrorReflectionObj>, "MirrorModelPeachCastle" },
        { "MirrorModelTwinFallLake", createNameObj<SimpleMirrorReflectionObj>, "MirrorModelTwinFallLake" },
        { "MirrorReflectionTest", createNameObj<SimpleMirrorObj>, "MirrorReflectionTest" },
        { "MirrorReflectionPeachCastle", createNameObj<SimpleMirrorObj>, "MirrorReflectionPeachCastle" },
        { "MirrorReflectionTwinFallLake", createNameObj<SimpleMirrorObj>, "MirrorReflectionTwinFallLake" },

        { "PhantomFirewood", createNameObj<SimpleMapObj>, "PhantomFirewood" },
        { "ArrowBoard", createNameObj<SimpleMapObj>, "ArrowBoard" },
        { "TeresaRoomGlaringLight", createNameObj<GlaringLight>, "TeresaRoomGlaringLight" },
        { "SpaceDustWoodA", createNameObj<SimpleEnvironmentObj>, "SpaceDustWoodA" },
        { "SpaceDustWoodB", createNameObj<SimpleEnvironmentObj>, "SpaceDustWoodB" },

        { "BeeFlowerHover", createNameObj<BeeFlowerHover>, "BeeFlowerHover" },

        { "HoneyQueenLeafA", createNameObj<SimpleSeesawObj>, "HoneyQueenLeafA", },

        { "DeadLeaves", createNameObj<DeadLeaves>, "DeadLeaves" },

        { "CoinBox", createNameObj<CoinBox>, NULL },

        { "ShootingStar", createNameObj<ShootingStar>, "ShootingStar" },

        { "BenefitItemLifeUp", createNameObj<BenefitItemLifeUp>, "KinokoLifeUp" },
        { "BenefitItemInvincible", createNameObj<BenefitItemInvincible>, "BenefitItemInvincible" },

        { "CollisionBlocker", createNameObj<CollisionBlocker>, NULL }
    };

    static PlanetEntry sUniquePlanetCreateFuncTable[100] = {
        { "HeavensDoorInsidePlanet", createNameObj<SimpleMapObj> }
    };

    NameObjFactory::Name2Archive cName2ArchiveNamesTable[0x1B9] = {
        { "AirBubbleGenerator", "AirBubble" }
    };

    const NameObjFactory::Name2MakeArchiveListFunc cName2MakeArchiveListFuncTable[0x5B] = {
        // todo
    };
};