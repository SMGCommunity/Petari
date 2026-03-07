#include "Game/System/GameEventFlagTable.hpp"
#include "Game/SingletonHolder.hpp"
#include "Game/Util/HashUtil.hpp"
#include "Game/Util/JMapInfo.hpp"
#include "Game/Util/StringUtil.hpp"
#include <algorithm>
#include <cstdio>

namespace {
    static const GameEventFlag sGameEventFlagStatic[] = {
        {"AppearFlipPanelExGalaxy", GameEventFlag::Type_GalaxyOpenStar, 1, 0, 0, 0, "FlipPanelExGalaxy", 0},
        {"AppearSurfingLv1Galaxy", GameEventFlag::Type_GalaxyOpenStar, 1, 0, 0, 0, "SurfingLv1Galaxy", 0},
        {"AppearTamakoroExLv1Galaxy", GameEventFlag::Type_GalaxyOpenStar, 1, 0, 0, 0, "TamakoroExLv1Galaxy", 0},
        {"AppearBreakDownPlanetGalaxy", GameEventFlag::Type_GalaxyOpenStar, 1, 0, 0, 0, "BreakDownPlanetGalaxy", 0},
        {"AppearCubeBubbleExLv1Galaxy", GameEventFlag::Type_GalaxyOpenStar, 1, 0, 0, 0, "CubeBubbleExLv1Galaxy", 0},
        {"StarNumOceanFloaterLandGalaxy", GameEventFlag::Type_GalaxyOpenStar, 1, 0, 0, 0, "OceanFloaterLandGalaxy", 0},
        {"SpecialStarOpenOceanFloaterLandGalaxy", GameEventFlag::Type_SpecialStar, 1, 1, 0, 0, "HeavenlyBeachGalaxy", 0},
        {"AppearOceanFloaterLandGalaxy", GameEventFlag::Type_4, 1, 0, 0, 0, "StarNumOceanFloaterLandGalaxy", "SpecialStarOpenOceanFloaterLandGalaxy"},
        {"StarNumHoneyBeeExGalaxy", GameEventFlag::Type_GalaxyOpenStar, 1, 0, 0, 0, "HoneyBeeExGalaxy", 0},
        {"SpecialStarOpenHoneyBeeExGalaxy", GameEventFlag::Type_SpecialStar, 1, 1, 0, 0, "HoneyBeeKingdomGalaxy", 0},
        {"AppearHoneyBeeExGalaxy", GameEventFlag::Type_4, 1, 0, 0, 0, "StarNumHoneyBeeExGalaxy", "SpecialStarOpenHoneyBeeExGalaxy"},
        {"StarNumSkullSharkGalaxy", GameEventFlag::Type_GalaxyOpenStar, 1, 0, 0, 0, "SkullSharkGalaxy", 0},
        {"SpecialStarOpenSkullSharkGalaxy", GameEventFlag::Type_SpecialStar, 1, 1, 0, 0, "TearDropGalaxy", 0},
        {"AppearSkullSharkGalaxy", GameEventFlag::Type_4, 1, 0, 0, 0, "StarNumSkullSharkGalaxy", "SpecialStarOpenSkullSharkGalaxy"},
        {"StarNumDarkRoomGalaxy", GameEventFlag::Type_GalaxyOpenStar, 1, 0, 0, 0, "DarkRoomGalaxy", 0},
        {"SpecialStarOpenDarkRoomGalaxy", GameEventFlag::Type_SpecialStar, 1, 1, 0, 0, "FactoryGalaxy", 0},
        {"AppearDarkRoomGalaxy", GameEventFlag::Type_4, 1, 0, 0, 0, "StarNumDarkRoomGalaxy", "SpecialStarOpenDarkRoomGalaxy"},
        {"ハチマリオ初変身", GameEventFlag::Type_0, 0, 0, 0, 0, 0, 0},
        {"テレサマリオ初変身", GameEventFlag::Type_0, 0, 0, 0, 0, 0, 0},
        {"ホッパーマリオ初変身", GameEventFlag::Type_0, 0, 0, 0, 0, 0, 0},
        {"ファイアマリオ初変身", GameEventFlag::Type_0, 0, 0, 0, 0, 0, 0},
        {"アイスマリオ初変身", GameEventFlag::Type_0, 0, 0, 0, 0, 0, 0},
        {"フライングマリオ初変身", GameEventFlag::Type_0, 0, 0, 0, 0, 0, 0},
        {"無敵マリオ初変身", GameEventFlag::Type_0, 0, 0, 0, 0, 0, 0},
        {"ライフアップキノコ解説", GameEventFlag::Type_0, 0, 0, 0, 0, 0, 0},
        {"１ＵＰキノコ解説", GameEventFlag::Type_0, 0, 0, 0, 0, 0, 0},
        {"サーフィンチュートリアル", GameEventFlag::Type_0, 0, 0, 0, 0, 0, 0},
        {"タマコロチュートリアル", GameEventFlag::Type_0, 0, 0, 0, 0, 0, 0},
        {"PeachCastleGardenGalaxy", GameEventFlag::Type_Galaxy, 1, 0, 0, 0, 0, 0},
        {"EpilogueDemoStage", GameEventFlag::Type_Galaxy, 1, 0, 0, 0, 0, 0},
        {"HeavensDoorGalaxy", GameEventFlag::Type_Galaxy, 1, 0, 0, 0, 0, 0},
        {"EggStarGalaxy", GameEventFlag::Type_Galaxy, 0, 0, 0, 0, 0, 0},
        {"HoneyBeeKingdomGalaxy", GameEventFlag::Type_Galaxy, 0, 0, 0, 0, 0, 0},
        {"FlipPanelExGalaxy", GameEventFlag::Type_Galaxy, 0, 0, 0, 0, 0, 0},
        {"SurfingLv1Galaxy", GameEventFlag::Type_Galaxy, 0, 0, 0, 0, 0, 0},
        {"BeltConveyerExGalaxy", GameEventFlag::Type_Galaxy, 0, 0, 0, 0, 0, 0},
        {"TriLegLv1Galaxy", GameEventFlag::Type_Galaxy, 0, 0, 0, 0, 0, 0},
        {"StarDustGalaxy", GameEventFlag::Type_Galaxy, 0, 0, 0, 0, 0, 0},
        {"TamakoroExLv1Galaxy", GameEventFlag::Type_Galaxy, 0, 0, 0, 0, 0, 0},
        {"BattleShipGalaxy", GameEventFlag::Type_Galaxy, 0, 0, 0, 0, 0, 0},
        {"BreakDownPlanetGalaxy", GameEventFlag::Type_Galaxy, 0, 0, 0, 0, 0, 0},
        {"CocoonExGalaxy", GameEventFlag::Type_Galaxy, 0, 0, 0, 0, 0, 0},
        {"KoopaBattleVs1Galaxy", GameEventFlag::Type_Galaxy, 0, 0, 0, 0, 0, 0},
        {"HeavenlyBeachGalaxy", GameEventFlag::Type_Galaxy, 0, 0, 0, 0, 0, 0},
        {"CubeBubbleExLv1Galaxy", GameEventFlag::Type_Galaxy, 0, 0, 0, 0, 0, 0},
        {"PhantomGalaxy", GameEventFlag::Type_Galaxy, 0, 0, 0, 0, 0, 0},
        {"OceanFloaterLandGalaxy", GameEventFlag::Type_Galaxy, 0, 0, 0, 0, 0, 0},
        {"TearDropGalaxy", GameEventFlag::Type_Galaxy, 0, 0, 0, 0, 0, 0},
        {"TeresaMario2DGalaxy", GameEventFlag::Type_Galaxy, 0, 0, 0, 0, 0, 0},
        {"KoopaJrShipLv1Galaxy", GameEventFlag::Type_Galaxy, 0, 0, 0, 0, 0, 0},
        {"CosmosGardenGalaxy", GameEventFlag::Type_Galaxy, 0, 0, 0, 0, 0, 0},
        {"IceVolcanoGalaxy", GameEventFlag::Type_Galaxy, 0, 0, 0, 0, 0, 0},
        {"HoneyBeeExGalaxy", GameEventFlag::Type_Galaxy, 0, 0, 0, 0, 0, 0},
        {"SandClockGalaxy", GameEventFlag::Type_Galaxy, 0, 0, 0, 0, 0, 0},
        {"SnowCapsuleGalaxy", GameEventFlag::Type_Galaxy, 0, 0, 0, 0, 0, 0},
        {"KoopaBattleVs2Galaxy", GameEventFlag::Type_Galaxy, 0, 0, 0, 0, 0, 0},
        {"ReverseKingdomGalaxy", GameEventFlag::Type_Galaxy, 0, 0, 0, 0, 0, 0},
        {"OceanRingGalaxy", GameEventFlag::Type_Galaxy, 0, 0, 0, 0, 0, 0},
        {"SkullSharkGalaxy", GameEventFlag::Type_Galaxy, 0, 0, 0, 0, 0, 0},
        {"FactoryGalaxy", GameEventFlag::Type_Galaxy, 0, 0, 0, 0, 0, 0},
        {"TransformationExGalaxy", GameEventFlag::Type_Galaxy, 0, 0, 0, 0, 0, 0},
        {"FloaterOtaKingGalaxy", GameEventFlag::Type_Galaxy, 0, 0, 0, 0, 0, 0},
        {"OceanPhantomCaveGalaxy", GameEventFlag::Type_Galaxy, 0, 0, 0, 0, 0, 0},
        {"CannonFleetGalaxy", GameEventFlag::Type_Galaxy, 0, 0, 0, 0, 0, 0},
        {"DarkRoomGalaxy", GameEventFlag::Type_Galaxy, 0, 0, 0, 0, 0, 0},
        {"HellProminenceGalaxy", GameEventFlag::Type_Galaxy, 0, 0, 0, 0, 0, 0},
        {"FishTunnelGalaxy", GameEventFlag::Type_Galaxy, 0, 0, 0, 0, 0, 0},
        {"KoopaBattleVs3Galaxy", GameEventFlag::Type_Galaxy, 0, 0, 0, 0, 0, 0},
        {"TamakoroExLv2Galaxy", GameEventFlag::Type_Galaxy, 0, 0, 0, 0, 0, 0},
        {"SurfingLv2Galaxy", GameEventFlag::Type_Galaxy, 0, 0, 0, 0, 0, 0},
        {"CubeBubbleExLv2Galaxy", GameEventFlag::Type_Galaxy, 0, 0, 0, 0, 0, 0},
        {"PeachCastleFinalGalaxy", GameEventFlag::Type_Galaxy, 0, 0, 0, 0, 0, 0},
        {"NormalCometEggStarGalaxy", GameEventFlag::Type_Comet, 1, 1, 0, 0, "EggStarGalaxy", "EventCometStarter"},
        {"NormalCometHoneyBeeKingdomGalaxy", GameEventFlag::Type_Comet, 1, 1, 0, 0, "HoneyBeeKingdomGalaxy", "EventCometStarter"},
        {"NormalCometStarDustGalaxy", GameEventFlag::Type_Comet, 1, 2, 0, 0, "StarDustGalaxy", "EventCometStarter"},
        {"NormalCometBattleShipGalaxy", GameEventFlag::Type_Comet, 1, 3, 0, 0, "BattleShipGalaxy", "EventCometStarter"},
        {"NormalCometHeavenlyBeachGalaxy", GameEventFlag::Type_Comet, 1, 3, 0, 0, "HeavenlyBeachGalaxy", "EventCometStarter"},
        {"NormalCometPhantomGalaxy", GameEventFlag::Type_Comet, 1, 3, 0, 0, "PhantomGalaxy", "EventCometStarter"},
        {"NormalCometCosmosGardenGalaxy", GameEventFlag::Type_Comet, 1, 3, 0, 0, "CosmosGardenGalaxy", "EventCometStarter"},
        {"NormalCometIceVolcanoGalaxy", GameEventFlag::Type_Comet, 1, 1, 0, 0, "IceVolcanoGalaxy", "EventCometStarter"},
        {"NormalCometSandClockGalaxy", GameEventFlag::Type_Comet, 1, 3, 0, 0, "SandClockGalaxy", "EventCometStarter"},
        {"NormalCometReverseKingdomGalaxy", GameEventFlag::Type_Comet, 1, 3, 0, 0, "ReverseKingdomGalaxy", "EventCometStarter"},
        {"NormalCometOceanRingGalaxy", GameEventFlag::Type_Comet, 1, 1, 0, 0, "OceanRingGalaxy", "EventCometStarter"},
        {"NormalCometFactoryGalaxy", GameEventFlag::Type_Comet, 1, 6, 0, 0, "FactoryGalaxy", "EventCometStarter"},
        {"NormalCometOceanPhantomCaveGalaxy", GameEventFlag::Type_Comet, 1, 2, 0, 0, "OceanPhantomCaveGalaxy", "EventCometStarter"},
        {"NormalCometCannonFleetGalaxy", GameEventFlag::Type_Comet, 1, 3, 0, 0, "CannonFleetGalaxy", "EventCometStarter"},
        {"NormalCometHellProminenceGalaxy", GameEventFlag::Type_Comet, 1, 2, 0, 0, "HellProminenceGalaxy", "EventCometStarter"},
        {"SpecialStarCoin100CometRelease", GameEventFlag::Type_SpecialStar, 1, 5, 0, 0, "EggStarGalaxy", 0},
        {"EventCoin100CometStarter", GameEventFlag::Type_4, 1, 0, 0, 0, "EventCometStarter", "SpecialStarCoin100CometRelease"},
        {"Coin100CometEggStarGalaxy", GameEventFlag::Type_Comet, 1, 1, 0, 0, "EggStarGalaxy", "ViewNormalEnding"},
        {"Coin100CometHoneyBeeKingdomGalaxy", GameEventFlag::Type_Comet, 1, 1, 0, 0, "HoneyBeeKingdomGalaxy", "EventCoin100CometStarter"},
        {"Coin100CometStarDustGalaxy", GameEventFlag::Type_Comet, 1, 1, 0, 0, "StarDustGalaxy", "EventCoin100CometStarter"},
        {"Coin100CometBattleShipGalaxy", GameEventFlag::Type_Comet, 1, 1, 0, 0, "BattleShipGalaxy", "EventCoin100CometStarter"},
        {"Coin100CometHeavenlyBeachGalaxy", GameEventFlag::Type_Comet, 1, 1, 0, 0, "HeavenlyBeachGalaxy", "EventCoin100CometStarter"},
        {"Coin100CometPhantomGalaxy", GameEventFlag::Type_Comet, 1, 2, 0, 0, "PhantomGalaxy", "EventCoin100CometStarter"},
        {"Coin100CometCosmosGardenGalaxy", GameEventFlag::Type_Comet, 1, 1, 0, 0, "CosmosGardenGalaxy", "EventCoin100CometStarter"},
        {"Coin100CometIceVolcanoGalaxy", GameEventFlag::Type_Comet, 1, 1, 0, 0, "IceVolcanoGalaxy", "EventCoin100CometStarter"},
        {"Coin100CometSandClockGalaxy", GameEventFlag::Type_Comet, 1, 2, 0, 0, "SandClockGalaxy", "EventCoin100CometStarter"},
        {"Coin100CometReverseKingdomGalaxy", GameEventFlag::Type_Comet, 1, 3, 0, 0, "ReverseKingdomGalaxy", "EventCoin100CometStarter"},
        {"Coin100CometOceanRingGalaxy", GameEventFlag::Type_Comet, 1, 3, 0, 0, "OceanRingGalaxy", "EventCoin100CometStarter"},
        {"Coin100CometFactoryGalaxy", GameEventFlag::Type_Comet, 1, 2, 0, 0, "FactoryGalaxy", "EventCoin100CometStarter"},
        {"Coin100CometOceanPhantomCaveGalaxy", GameEventFlag::Type_Comet, 1, 1, 0, 0, "OceanPhantomCaveGalaxy", "EventCoin100CometStarter"},
        {"Coin100CometCannonFleetGalaxy", GameEventFlag::Type_Comet, 1, 2, 0, 0, "CannonFleetGalaxy", "EventCoin100CometStarter"},
        {"Coin100CometHellProminenceGalaxy", GameEventFlag::Type_Comet, 1, 3, 0, 0, "HellProminenceGalaxy", "EventCoin100CometStarter"},
        {"SpecialStarGrand1", GameEventFlag::Type_SpecialStar, 1, 1, 0, 0, "HeavensDoorGalaxy", 0},
        {"SpecialStarGrand2", GameEventFlag::Type_SpecialStar, 1, 1, 0, 0, "TriLegLv1Galaxy", 0},
        {"SpecialStarGrand3", GameEventFlag::Type_SpecialStar, 1, 1, 0, 0, "KoopaBattleVs1Galaxy", 0},
        {"SpecialStarGrand4", GameEventFlag::Type_SpecialStar, 1, 1, 0, 0, "KoopaJrShipLv1Galaxy", 0},
        {"SpecialStarGrand5", GameEventFlag::Type_SpecialStar, 1, 1, 0, 0, "KoopaBattleVs2Galaxy", 0},
        {"SpecialStarGrand6", GameEventFlag::Type_SpecialStar, 1, 1, 0, 0, "FloaterOtaKingGalaxy", 0},
        {"SpecialStarGrand7", GameEventFlag::Type_SpecialStar, 1, 1, 0, 0, "KoopaBattleVs3Galaxy", 0},
        {"SpecialStarGreen1", GameEventFlag::Type_SpecialStar, 1, 7, 0, 0, "BattleShipGalaxy", 0},
        {"SpecialStarGreen2", GameEventFlag::Type_SpecialStar, 1, 2, 0, 0, "OceanFloaterLandGalaxy", 0},
        {"SpecialStarGreen3", GameEventFlag::Type_SpecialStar, 1, 7, 0, 0, "SandClockGalaxy", 0},
        {"SpecialStarGreen1/2", GameEventFlag::Type_4, 1, 0, 0, 0, "SpecialStarGreen1", "SpecialStarGreen2"},
        {"SpecialStarGreenAll", GameEventFlag::Type_4, 1, 0, 0, 0, "SpecialStarGreen1/2", "SpecialStarGreen3"},
        {"SpecialStarRed1", GameEventFlag::Type_SpecialStar, 1, 2, 0, 0, "HeavensDoorGalaxy", 0},
        {"AppearHeavensDoorGalaxy", GameEventFlag::Type_4, 1, 0, 0, 0, "SpecialStarRed1", 0},
        {"EventCometStarter", GameEventFlag::Type_1, 1, 13, 0, 0, 0, 0},
        {"EventKinopioExplorerRescued", GameEventFlag::Type_SpecialStar, 1, 1, 0, 0, "HoneyBeeKingdomGalaxy", 0},
        {"EventKinopioExplorerOrganize", GameEventFlag::Type_1, 1, 7, 0, 0, 0, 0},
        {"ExOpen1CocoonExGalaxy", GameEventFlag::Type_SpecialStar, 1, 3, 0, 0, "StarDustGalaxy", 0},
        {"ExOpen1TearDropGalaxy", GameEventFlag::Type_SpecialStar, 1, 1, 0, 0, "HeavenlyBeachGalaxy", 0},
        {"ExOpen1FishTunnelGalaxy", GameEventFlag::Type_SpecialStar, 1, 1, 0, 0, "SandClockGalaxy", 0},
        {"ExOpenCommonBase", GameEventFlag::Type_SpecialStar, 1, 1, 0, 0, "PhantomGalaxy", 0},
        {"ExOpenSubTransformationExGalaxy", GameEventFlag::Type_SpecialStar, 1, 2, 0, 0, "OceanRingGalaxy", 0},
        {"ExOpenSubTeresaMario2DGalaxy", GameEventFlag::Type_SpecialStar, 1, 2, 0, 0, "HeavensDoorGalaxy", 0},
        {"ExOpen1TransformationExGalaxy", GameEventFlag::Type_4, 1, 0, 0, 0, "ExOpenCommonBase", "ExOpenSubTransformationExGalaxy"},
        {"ExOpen1TeresaMario2DGalaxy", GameEventFlag::Type_4, 1, 0, 0, 0, "ExOpenCommonBase", "ExOpenSubTeresaMario2DGalaxy"},
        {"ExOpen1SnowCapsuleGalaxy", GameEventFlag::Type_SpecialStar, 1, 1, 0, 0, "HellProminenceGalaxy", 0},
        {"ExOpen1BeltConveyerExGalaxy", GameEventFlag::Type_1, 1, 7, 0, 0, 0, 0},
        {"AppearBeltConveyerExGalaxy", GameEventFlag::Type_4, 1, 0, 0, 0, "SpecialStarGrand1", "ExOpen1BeltConveyerExGalaxy"},
        {"AppearCocoonExGalaxy", GameEventFlag::Type_4, 1, 0, 0, 0, "SpecialStarGrand2", "ExOpen1CocoonExGalaxy"},
        {"AppearTearDropGalaxy", GameEventFlag::Type_4, 1, 0, 0, 0, "SpecialStarGrand3", "ExOpen1TearDropGalaxy"},
        {"AppearFishTunnelGalaxy", GameEventFlag::Type_4, 1, 0, 0, 0, "SpecialStarGrand4", "ExOpen1FishTunnelGalaxy"},
        {"AppearTransformationExGalaxy", GameEventFlag::Type_4, 1, 0, 0, 0, "SpecialStarGrand5", "ExOpen1TransformationExGalaxy"},
        {"AppearTeresaMario2DGalaxy", GameEventFlag::Type_4, 1, 0, 0, 0, "SpecialStarGrand6", "ExOpen1TeresaMario2DGalaxy"},
        {"AppearSnowCapsuleGalaxy", GameEventFlag::Type_4, 1, 0, 0, 0, "SpecialStarGrand6", "ExOpen1SnowCapsuleGalaxy"},
        {"StarPieceBeltConveyerExGalaxy", GameEventFlag::Type_StarPiece, 1, 0, 40, 0, "BeltConveyerExGalaxy", 0},
        {"StarPieceCocoonExGalaxy", GameEventFlag::Type_StarPiece, 1, 1, 40, 0, "CocoonExGalaxy", 0},
        {"StarPieceTearDropGalaxy", GameEventFlag::Type_StarPiece, 1, 2, 60, 0, "TearDropGalaxy", 0},
        {"StarPieceFishTunnelGalaxy", GameEventFlag::Type_StarPiece, 1, 6, 80, 0, "FishTunnelGalaxy", 0},
        {"StarPieceTransformationExGalaxy", GameEventFlag::Type_StarPiece, 1, 5, 100, 0, "TransformationExGalaxy", 0},
        {"StarPieceTeresaMario2DGalaxy", GameEventFlag::Type_StarPiece, 1, 3, 120, 0, "TeresaMario2DGalaxy", 0},
        {"StarPieceSnowCapsuleGalaxy", GameEventFlag::Type_StarPiece, 1, 4, 160, 0, "SnowCapsuleGalaxy", 0},
        {"AppearTamakoroExLv2Galaxy", GameEventFlag::Type_4, 1, 0, 0, 0, "SpecialStarGreenAll", 0},
        {"AppearSurfingLv2Galaxy", GameEventFlag::Type_4, 1, 0, 0, 0, "SpecialStarGreenAll", 0},
        {"AppearCubeBubbleExLv2Galaxy", GameEventFlag::Type_4, 1, 0, 0, 0, "SpecialStarGreenAll", 0},
        {"AppearPeachCastleFinalGalaxy", GameEventFlag::Type_4, 1, 0, 0, 0, "ViewCompleteMarioAndLuigi", 0},
        {"LibraryOpenRequirement", GameEventFlag::Type_EventValueIsZero, 1, 0, 0, 0, "LibraryOpenNewStarCount", "SpecialStarGrand3"},
        {"StarNumPictureBookB", GameEventFlag::Type_1, 1, 24, 0, 0, 0, 0},
        {"StarNumPictureBookC", GameEventFlag::Type_1, 1, 28, 0, 0, 0, 0},
        {"StarNumPictureBookD", GameEventFlag::Type_1, 1, 32, 0, 0, 0, 0},
        {"StarNumPictureBookE", GameEventFlag::Type_1, 1, 40, 0, 0, 0, 0},
        {"StarNumPictureBookF", GameEventFlag::Type_1, 1, 49, 0, 0, 0, 0},
        {"StarNumPictureBookG", GameEventFlag::Type_1, 1, 58, 0, 0, 0, 0},
        {"PictureBookA", GameEventFlag::Type_4, 1, 0, 0, 0, "LibraryOpenRequirement", 0},
        {"PictureBookB", GameEventFlag::Type_4, 1, 0, 0, 0, "StarNumPictureBookB", "LibraryOpenRequirement"},
        {"PictureBookC", GameEventFlag::Type_4, 1, 0, 0, 0, "StarNumPictureBookC", "LibraryOpenRequirement"},
        {"PictureBookD", GameEventFlag::Type_4, 1, 0, 0, 0, "StarNumPictureBookD", "LibraryOpenRequirement"},
        {"PictureBookE", GameEventFlag::Type_4, 1, 0, 0, 0, "StarNumPictureBookE", "LibraryOpenRequirement"},
        {"PictureBookF", GameEventFlag::Type_4, 1, 0, 0, 0, "StarNumPictureBookF", "LibraryOpenRequirement"},
        {"PictureBookG", GameEventFlag::Type_4, 1, 0, 0, 0, "StarNumPictureBookG", "LibraryOpenRequirement"},
        {"PictureBookH", GameEventFlag::Type_SpecialStar, 1, 1, 0, 0, "KoopaBattleVs3Galaxy", 0},
        {"PictureBookI", GameEventFlag::Type_SpecialStar, 1, 1, 0, 0, "KoopaBattleVs3Galaxy", 0},
        {"SpecialStarLuigiRescued", GameEventFlag::Type_SpecialStar, 1, 1, 0, 0, "PhantomGalaxy", 0},
        {"LuigiTalkAfterRescued", GameEventFlag::Type_4, 0, 0, 0, 0, "SpecialStarLuigiRescued", 0},
        {"SpecialStarFindingLuigi1", GameEventFlag::Type_SpecialStar, 1, 6, 0, 0, "EggStarGalaxy", 0},
        {"SpecialStarFindingLuigi2", GameEventFlag::Type_SpecialStar, 1, 7, 0, 0, "BattleShipGalaxy", 0},
        {"SpecialStarFindingLuigi3", GameEventFlag::Type_SpecialStar, 1, 6, 0, 0, "HoneyBeeKingdomGalaxy", 0},
        {"RosettaTalkAboutTico", GameEventFlag::Type_4, 0, 0, 0, 0, "SpecialStarGrand6", 0},
        {"RosettaTalkAfterNormalEnding", GameEventFlag::Type_4, 0, 0, 0, 0, "ViewNormalEnding", 0},
        {"ViewNormalEnding", GameEventFlag::Type_4, 1, 0, 0, 0, "PictureBookH", 0},
        {"PowerStarComplete", GameEventFlag::Type_1, 1, 120, 0, 0, 0, 0},
        {"ViewCompleteEnding", GameEventFlag::Type_4, 0, 0, 0, 0, "PowerStarComplete", 0},
        {"ViewCompleteMarioAndLuigi", GameEventFlag::Type_10, 1, 0, 0, 0, 0, 0},
        {"SpecialStarFinalChallenge", GameEventFlag::Type_SpecialStar, 1, 1, 0, 0, "PeachCastleFinalGalaxy", 0},
        {"StarPieceCounterStop", GameEventFlag::Type_0, 0, 0, 0, 0, 0, 0},
        {"PlayHeavensDoorGalaxy1", GameEventFlag::Type_4, 1, 0, 0, 0, 0, 0},
        {"PlayHeavensDoorGalaxy2", GameEventFlag::Type_4, 1, 0, 0, 0, "SpecialStarGrand6", 0},
        {"[同期]ピーチ城浮上後", GameEventFlag::Type_11, 1, 0, 0, 0, "PlayHeavensDoorGalaxy1", "ピーチ城浮上後"},
        {"PlayEggStarGalaxy1", GameEventFlag::Type_4, 1, 0, 0, 0, "SpecialStarGrand1", 0},
        {"[同期]バトラー情報Ａ", GameEventFlag::Type_11, 1, 0, 0, 0, "PlayEggStarGalaxy1", "バトラー情報Ａ"},
        {"SpecialStarEggStarGalaxy1", GameEventFlag::Type_SpecialStar, 1, 1, 0, 0, "EggStarGalaxy", 0},
        {"PlayEggStarGalaxy2", GameEventFlag::Type_4, 1, 0, 0, 0, "SpecialStarEggStarGalaxy1", 0},
        {"[同期]スターピースレクチャー", GameEventFlag::Type_11, 1, 0, 0, 0, "PlayEggStarGalaxy2", "スターピースレクチャー"},
        {"PowerStarNumForButlerKoopaDemo", GameEventFlag::Type_GalaxyOpenStar, 1, 0, 0, 0, "KoopaBattleVs1Galaxy", 0},
        {"[同期]クッパスタープラント発見", GameEventFlag::Type_11, 1, 0, 0, 0, "PowerStarNumForButlerKoopaDemo", "クッパスタープラント発見"},
    };

    struct GameEventFlagSortLt {
        bool operator()(GameEventFlagTableInstance::Key* key1, GameEventFlagTableInstance::Key* key2) {
            return (key1->mHashCode < key2->mHashCode) ? true : false;
        }
    };
};  // namespace

bool GameEventFlagIter::isEnd() const {
    if (GameEventFlagIter::isValid() == false) {
        return true;
    }

    return GameEventFlagTable::getFlag(mIter) == nullptr;
}

void GameEventFlagIter::goNext() {
    if (GameEventFlagIter::isValid()) {
        mIter++;
    }
}

const GameEventFlag* GameEventFlagIter::getFlag() const {
    return GameEventFlagTable::getFlag(mIter);
}

bool GameEventFlagIter::isValid() const {
    if (mIter >= 0) {
        return true;
    } else {
        return false;
    }
}

GameEventFlagTableInstance::GameEventFlagTableInstance() : mSortTable(nullptr), mLength(0) {
    initSortTable();
}

const GameEventFlag* GameEventFlagTableInstance::findFlag(const char* flagName) {
    int i;
    u16 hashCode = MR::getHashCode(flagName);
    i = mLength;
    i = (i * 8) / 8;
    GameEventFlagTableInstance::Key* key = mSortTable;

    while (i > 0) {
        int mid = i / 2;

        if (key[i / 2].mHashCode < hashCode) {
            i -= mid + 1;
            key = &key[mid] + 1;
        } else {
            i = mid;
        }
    }

    if (key->mHashCode == hashCode && MR::isEqualString(flagName, key->mFlag->mName)) {
        return key->mFlag;
    }

    return nullptr;
}

void GameEventFlagTableInstance::initSortTable() {
    mSortTable = new GameEventFlagTableInstance::Key[GameEventFlagTable::getTableSize()];
    mLength = GameEventFlagTable::getTableSize();

    for (int i = 0; i < mLength; i++) {
        const GameEventFlag* pFlag = GameEventFlagTable::getFlag(i);

        mSortTable[i].mFlag = pFlag;
        mSortTable[i].mHashCode = MR::getHashCode(pFlag->mName);
    }

    std::sort(&mSortTable[0], &mSortTable[mLength], GameEventFlagSortLt());
}

namespace GameEventFlagTable {
    GameEventFlagIter getBeginIter() {
        return GameEventFlagIter();
    }

    GameEventFlagAccessor makeAccessor(const char* pFlagName) {
        return findFlag(pFlagName);
    }

    s32 getTableSize() {
        return sizeof(sGameEventFlagStatic) / sizeof(*sGameEventFlagStatic);
    }

    const GameEventFlag* getFlag(int index) {
        if (index < 0 || index >= getTableSize()) {
            return nullptr;
        } else {
            return &sGameEventFlagStatic[index];
        }
    }

    const GameEventFlag* findFlag(const char* pFlagName) {
        return SingletonHolder< GameEventFlagTableInstance >::get()->findFlag(pFlagName);
    }

    const char* getEventFlagNameSpecialPowerStar(const char* pGalaxyName, s32 starId) {
        for (GameEventFlagIter iter = getBeginIter(); !iter.isEnd(); iter.goNext()) {
            GameEventFlagAccessor accessor = GameEventFlagAccessor(iter.getFlag());

            if (!accessor.isTypeSpecialStar()) {
                continue;
            }

            if (accessor.getStarId() != starId) {
                continue;
            }

            if (!MR::isEqualString(pGalaxyName, accessor.getGalaxyName())) {
                continue;
            }

            return accessor.getName();
        }

        return nullptr;
    }

    bool isPowerStarType(const char* pGalaxyName, s32 starId, const char* pStarType) {
        const char* pFlagName = getEventFlagNameSpecialPowerStar(pGalaxyName, starId);

        if (pFlagName == nullptr) {
            return false;
        }

        return MR::isEqualString(pFlagName, pStarType);
    }

    s32 calcExclamationGalaxyNum() {
        s32 num = -1;

        for (GameEventFlagIter iter = getBeginIter(); !iter.isEnd(); iter.goNext()) {
            GameEventFlagAccessor accessor = GameEventFlagAccessor(iter.getFlag());

            if (!accessor.isTypeStarPiece()) {
                continue;
            }

            if (num >= accessor.getStarPieceIndex()) {
                continue;
            }

            num = accessor.getStarPieceIndex();
        }

        return num + 1;
    }

    const char* getExclamationGalaxyNameFromIndex(int index) {
        for (GameEventFlagIter iter = getBeginIter(); !iter.isEnd(); iter.goNext()) {
            GameEventFlagAccessor accessor = GameEventFlagAccessor(iter.getFlag());

            if (!accessor.isTypeStarPiece()) {
                continue;
            }

            if (accessor.getStarPieceIndex() != index) {
                continue;
            }

            return accessor.getGalaxyNameWithStarPiece();
        }

        return nullptr;
    }

    bool isExist(const char* pName) {
        return findFlag(pName) != nullptr;
    }

    int getIndex(const GameEventFlag* pFlag) {
        return pFlag - &sGameEventFlagStatic[0];
    }

    bool isDependedAnother(const char* pFlagName1, const char* pFlagName2) {
        const GameEventFlag* pFlag1 = findFlag(pFlagName1);

        if (pFlag1->mType == GameEventFlag::Type_4) {
            if (pFlag1->mRequirement1 != nullptr && MR::isEqualString(pFlag1->mRequirement1, pFlagName2)) {
                return true;
            }

            if (pFlag1->mRequirement2 != nullptr && MR::isEqualString(pFlag1->mRequirement2, pFlagName2)) {
                return true;
            }
        } else if (pFlag1->mType == GameEventFlag::Type_Galaxy) {
            const char* galaxyDependedFlags[3];
            s32 length = getGalaxyDependedFlags(galaxyDependedFlags, sizeof(galaxyDependedFlags) / sizeof(*galaxyDependedFlags), pFlagName1);

            for (s32 i = 0; i < length; i++) {
                if (MR::isEqualString(galaxyDependedFlags[i], pFlagName2)) {
                    return true;
                }
            }
        }

        return false;
    }

    int getIndexFromHashCode(u16 hashCode) {
        for (GameEventFlagIter iter = getBeginIter(); !iter.isEnd(); iter.goNext()) {
            if ((iter.getFlag()->mSaveFlag & 0x1) != 0) {
                continue;
            }

            GameEventFlagAccessor accessor = GameEventFlagAccessor(iter.getFlag());

            if ((MR::getHashCode(accessor.getName()) & 0x7FFF) != hashCode) {
                continue;
            }

            return getIndex(getFlag(iter.mIter));
        }

        return -1;
    }

    s32 calcSpecialPowerStarNum(const char* pPowerStarType) {
        s32 num = 0;

        for (GameEventFlagIter iter = getBeginIter(); !iter.isEnd(); iter.goNext()) {
            GameEventFlagAccessor accessor = GameEventFlagAccessor(iter.getFlag());

            if (!accessor.isTypeSpecialStar()) {
                continue;
            }

            if (strstr(accessor.getName(), pPowerStarType) == nullptr) {
                continue;
            }

            num++;
        }

        return num;
    }

    s32 getStarPieceNumToOpenExclamationGalaxy(const char* pGalaxyName) {
        char flagName[64];
        snprintf(flagName, sizeof(flagName), "StarPiece%s", pGalaxyName);

        GameEventFlagAccessor accessor = makeAccessor(flagName);

        return accessor.getNeedStarPieceNum();
    }

    s32 calcGreenPowerStarNum() {
        return calcSpecialPowerStarNum("SpecialStarGreen");
    }
};  // namespace GameEventFlagTable
