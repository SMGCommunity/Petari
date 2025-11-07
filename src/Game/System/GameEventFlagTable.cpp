#include "Game/System/GameEventFlagTable.hpp"
#include "Game/Util/HashUtil.hpp"
#include "Game/Util/StringUtil.hpp"
#include "Game/SingletonHolder.hpp"
#include "Game/Util/JMapInfo.hpp"

#include <algorithm>
#include <cstdio>


namespace {
    static const GameEventFlag sGameEventFlagStatic[0xbc] = {
    {"AppearFlipPanelExGalaxy", 2, 1, 0, 0, 0, "FlipPanelExGalaxy", 0},
    {"AppearSurfingLv1Galaxy", 2, 1, 0, 0, 0, "SurfingLv1Galaxy", 0},
    {"AppearTamakoroExLv1Galaxy", 2, 1, 0, 0, 0, "TamakoroExLv1Galaxy", 0},
    {"AppearBreakDownPlanetGalaxy", 2, 1, 0, 0, 0, "BreakDownPlanetGalaxy", 0},
    {"AppearCubeBubbleExLv1Galaxy", 2, 1, 0, 0, 0, "CubeBubbleExLv1Galaxy", 0},
    {"StarNumOceanFloaterLandGalaxy", 2, 1, 0, 0, 0, "OceanFloaterLandGalaxy", 0},
    {"SpecialStarOpenOceanFloaterLandGalaxy", 3, 1, 1, 0, 0, "HeavenlyBeachGalaxy", 0},
    {"AppearOceanFloaterLandGalaxy", 4, 1, 0, 0, 0, "StarNumOceanFloaterLandGalaxy", "SpecialStarOpenOceanFloaterLandGalaxy"},
    {"StarNumHoneyBeeExGalaxy", 2, 1, 0, 0, 0, "HoneyBeeExGalaxy", 0},
    {"SpecialStarOpenHoneyBeeExGalaxy", 3, 1, 1, 0, 0, "HoneyBeeKingdomGalaxy", 0},
    {"AppearHoneyBeeExGalaxy", 4, 1, 0, 0, 0, "StarNumHoneyBeeExGalaxy", "SpecialStarOpenHoneyBeeExGalaxy"},
    {"StarNumSkullSharkGalaxy", 2, 1, 0, 0, 0, "SkullSharkGalaxy", 0},
    {"SpecialStarOpenSkullSharkGalaxy", 3, 1, 1, 0, 0, "TearDropGalaxy", 0},
    {"AppearSkullSharkGalaxy", 4, 1, 0, 0, 0, "StarNumSkullSharkGalaxy", "SpecialStarOpenSkullSharkGalaxy"},
    {"StarNumDarkRoomGalaxy", 2, 1, 0, 0, 0, "DarkRoomGalaxy", 0},
    {"SpecialStarOpenDarkRoomGalaxy", 3, 1, 1, 0, 0, "FactoryGalaxy", 0},
    {"AppearDarkRoomGalaxy", 4, 1, 0, 0, 0, "StarNumDarkRoomGalaxy", "SpecialStarOpenDarkRoomGalaxy"},
    {"ハチマリオ初変身", 0, 0, 0, 0, 0, 0, 0},
    {"テレサマリオ初変身", 0, 0, 0, 0, 0, 0, 0},
    {"ホッパーマリオ初変身", 0, 0, 0, 0, 0, 0, 0},
    {"ファイアマリオ初変身", 0, 0, 0, 0, 0, 0, 0}, 
    {"アイスマリオ初変身", 0, 0, 0, 0, 0, 0, 0},
    {"フライングマリオ初変身", 0, 0, 0, 0, 0, 0, 0},
    {"無敵マリオ初変身", 0, 0, 0, 0, 0, 0, 0},
    {"ライフアップキノコ解説", 0, 0, 0, 0, 0, 0, 0},
    {"１ＵＰキノコ解説", 0, 0, 0, 0, 0, 0, 0},
    {"サーフィンチュートリアル", 0, 0, 0, 0, 0, 0, 0},
    {"タマコロチュートリアル", 0, 0, 0, 0, 0, 0, 0},
    {"PeachCastleGardenGalaxy", 6, 1, 0, 0, 0, 0, 0},
    {"EpilogueDemoStage", 6, 1, 0, 0, 0, 0, 0},
    {"HeavensDoorGalaxy", 6, 1, 0, 0, 0, 0, 0},
    {"EggStarGalaxy", 6, 0, 0, 0, 0, 0, 0},
    {"HoneyBeeKingdomGalaxy", 6, 0, 0, 0, 0, 0, 0},
    {"FlipPanelExGalaxy", 6, 0, 0, 0, 0, 0, 0},
    {"SurfingLv1Galaxy", 6, 0, 0, 0, 0, 0, 0},
    {"BeltConveyerExGalaxy", 6, 0, 0, 0, 0, 0, 0},
    {"TriLegLv1Galaxy", 6, 0, 0, 0, 0, 0, 0},
    {"StarDustGalaxy", 6, 0, 0, 0, 0, 0, 0},
    {"TamakoroExLv1Galaxy", 6, 0, 0, 0, 0, 0, 0},
    {"BattleShipGalaxy", 6, 0, 0, 0, 0, 0, 0},
    {"BreakDownPlanetGalaxy", 6, 0, 0, 0, 0, 0, 0},
    {"CocoonExGalaxy", 6, 0, 0, 0, 0, 0, 0},
    {"KoopaBattleVs1Galaxy", 6, 0, 0, 0, 0, 0, 0},
    {"HeavenlyBeachGalaxy", 6, 0, 0, 0, 0, 0, 0},
    {"CubeBubbleExLv1Galaxy", 6, 0, 0, 0, 0, 0, 0},
    {"PhantomGalaxy", 6, 0, 0, 0, 0, 0, 0},
    {"OceanFloaterLandGalaxy", 6, 0, 0, 0, 0, 0, 0},
    {"TearDropGalaxy", 6, 0, 0, 0, 0, 0, 0},
    {"TeresaMario2DGalaxy", 6, 0, 0, 0, 0, 0, 0},
    {"KoopaJrShipLv1Galaxy", 6, 0, 0, 0, 0, 0, 0},
    {"CosmosGardenGalaxy", 6, 0, 0, 0, 0, 0, 0},
    {"IceVolcanoGalaxy", 6, 0, 0, 0, 0, 0, 0},
    {"HoneyBeeExGalaxy", 6, 0, 0, 0, 0, 0, 0},
    {"SandClockGalaxy", 6, 0, 0, 0, 0, 0, 0},
    {"SnowCapsuleGalaxy", 6, 0, 0, 0, 0, 0, 0},
    {"KoopaBattleVs2Galaxy", 6, 0, 0, 0, 0, 0, 0},
    {"ReverseKingdomGalaxy", 6, 0, 0, 0, 0, 0, 0},
    {"OceanRingGalaxy", 6, 0, 0, 0, 0, 0, 0},
    {"SkullSharkGalaxy", 6, 0, 0, 0, 0, 0, 0},
    {"FactoryGalaxy", 6, 0, 0, 0, 0, 0, 0},
    {"TransformationExGalaxy", 6, 0, 0, 0, 0, 0, 0},
    {"FloaterOtaKingGalaxy", 6, 0, 0, 0, 0, 0, 0},
    {"OceanPhantomCaveGalaxy", 6, 0, 0, 0, 0, 0, 0},
    {"CannonFleetGalaxy", 6, 0, 0, 0, 0, 0, 0},
    {"DarkRoomGalaxy", 6, 0, 0, 0, 0, 0, 0},
    {"HellProminenceGalaxy", 6, 0, 0, 0, 0, 0, 0},
    {"FishTunnelGalaxy", 6, 0, 0, 0, 0, 0, 0},
    {"KoopaBattleVs3Galaxy", 6, 0, 0, 0, 0, 0, 0},
    {"TamakoroExLv2Galaxy", 6, 0, 0, 0, 0, 0, 0},
    {"SurfingLv2Galaxy", 6, 0, 0, 0, 0, 0, 0},
    {"CubeBubbleExLv2Galaxy", 6, 0, 0, 0, 0, 0, 0},
    {"PeachCastleFinalGalaxy", 6, 0, 0, 0, 0, 0, 0},
    {"NormalCometEggStarGalaxy", 7, 1, 1, 0, 0, "EggStarGalaxy", "EventCometStarter"},
    {"NormalCometHoneyBeeKingdomGalaxy", 7, 1, 1, 0, 0, "HoneyBeeKingdomGalaxy", "EventCometStarter"},
    {"NormalCometStarDustGalaxy", 7, 1, 2, 0, 0, "StarDustGalaxy", "EventCometStarter"},
    {"NormalCometBattleShipGalaxy", 7, 1, 3, 0, 0, "BattleShipGalaxy", "EventCometStarter"},
    {"NormalCometHeavenlyBeachGalaxy", 7, 1, 3, 0, 0, "HeavenlyBeachGalaxy", "EventCometStarter"},
    {"NormalCometPhantomGalaxy", 7, 1, 3, 0, 0, "PhantomGalaxy", "EventCometStarter"},
    {"NormalCometCosmosGardenGalaxy", 7, 1, 3, 0, 0, "CosmosGardenGalaxy", "EventCometStarter"},
    {"NormalCometIceVolcanoGalaxy", 7, 1, 1, 0, 0, "IceVolcanoGalaxy", "EventCometStarter"},
    {"NormalCometSandClockGalaxy", 7, 1, 3, 0, 0, "SandClockGalaxy", "EventCometStarter"},
    {"NormalCometReverseKingdomGalaxy", 7, 1, 3, 0, 0, "ReverseKingdomGalaxy", "EventCometStarter"},
    {"NormalCometOceanRingGalaxy", 7, 1, 1, 0, 0, "OceanRingGalaxy", "EventCometStarter"},
    {"NormalCometFactoryGalaxy", 7, 1, 6, 0, 0, "FactoryGalaxy", "EventCometStarter"},
    {"NormalCometOceanPhantomCaveGalaxy", 7, 1, 2, 0, 0, "OceanPhantomCaveGalaxy", "EventCometStarter"},
    {"NormalCometCannonFleetGalaxy", 7, 1, 3, 0, 0, "CannonFleetGalaxy", "EventCometStarter"},
    {"NormalCometHellProminenceGalaxy", 7, 1, 2, 0, 0, "HellProminenceGalaxy", "EventCometStarter"},
    {"SpecialStarCoin100CometRelease", 3, 1, 5, 0, 0, "EggStarGalaxy", 0},
    {"EventCoin100CometStarter", 4, 1, 0, 0, 0, "EventCometStarter", "SpecialStarCoin100CometRelease"},
    {"Coin100CometEggStarGalaxy", 07, 01, 01, 0, 0, "EggStarGalaxy", "ViewNormalEnding"},
    {"Coin100CometHoneyBeeKingdomGalaxy", 07, 01, 01, 0, 0, "HoneyBeeKingdomGalaxy", "EventCoin100CometStarter"},
    {"Coin100CometStarDustGalaxy", 07, 01, 01, 0, 0, "StarDustGalaxy", "EventCoin100CometStarter"},
    {"Coin100CometBattleShipGalaxy", 07, 01, 01, 0, 0, "BattleShipGalaxy", "EventCoin100CometStarter"},
    {"Coin100CometHeavenlyBeachGalaxy", 07, 01, 01, 0, 0, "HeavenlyBeachGalaxy", "EventCoin100CometStarter"},
    {"Coin100CometPhantomGalaxy", 07, 01, 02, 0, 0, "PhantomGalaxy", "EventCoin100CometStarter"},
    {"Coin100CometCosmosGardenGalaxy", 07, 01, 01, 0, 0, "CosmosGardenGalaxy", "EventCoin100CometStarter"},
    {"Coin100CometIceVolcanoGalaxy", 07, 01, 01, 0, 0, "IceVolcanoGalaxy", "EventCoin100CometStarter"},
    {"Coin100CometSandClockGalaxy", 07, 01, 02, 0, 0, "SandClockGalaxy", "EventCoin100CometStarter"},
    {"Coin100CometReverseKingdomGalaxy", 07, 01, 03, 0, 0, "ReverseKingdomGalaxy", "EventCoin100CometStarter"},
    {"Coin100CometOceanRingGalaxy", 07, 01, 03, 0, 0, "OceanRingGalaxy", "EventCoin100CometStarter"},
    {"Coin100CometFactoryGalaxy", 07, 01, 02, 0, 0, "FactoryGalaxy", "EventCoin100CometStarter"},
    {"Coin100CometOceanPhantomCaveGalaxy", 07, 01, 01, 0, 0, "OceanPhantomCaveGalaxy", "EventCoin100CometStarter"},
    {"Coin100CometCannonFleetGalaxy", 07, 01, 02, 0, 0, "CannonFleetGalaxy", "EventCoin100CometStarter"},
    {"Coin100CometHellProminenceGalaxy", 07, 01, 03, 0, 0, "HellProminenceGalaxy", "EventCoin100CometStarter"},
    {"SpecialStarGrand1", 03, 01, 01, 0, 0, "HeavensDoorGalaxy", 0},
    {"SpecialStarGrand2", 03, 01, 01, 0, 0, "TriLegLv1Galaxy", 0},
    {"SpecialStarGrand3", 03, 01, 01, 0, 0, "KoopaBattleVs1Galaxy", 0},
    {"SpecialStarGrand4", 03, 01, 01, 0, 0, "KoopaJrShipLv1Galaxy", 0},
    {"SpecialStarGrand5", 03, 01, 01, 0, 0, "KoopaBattleVs2Galaxy", 0},
    {"SpecialStarGrand6", 03, 01, 01, 0, 0,	"FloaterOtaKingGalaxy", 0},
    {"SpecialStarGrand7", 03, 01, 01, 0, 0, "KoopaBattleVs3Galaxy", 0},
    {"SpecialStarGreen1", 03, 01, 07, 0, 0, "BattleShipGalaxy", 0},
    {"SpecialStarGreen2", 03, 01, 02, 0, 0, "OceanFloaterLandGalaxy", 0},
    {"SpecialStarGreen3", 03, 01, 07, 0, 0, "SandClockGalaxy", 0},
    {"SpecialStarGreen1/2", 04, 01, 0, 0, 0, "SpecialStarGreen1", "SpecialStarGreen2"},
    {"SpecialStarGreenAll", 04, 01, 0, 0, 0, "SpecialStarGreen1/2", "SpecialStarGreen3"},
    {"SpecialStarRed1", 03, 01, 02, 0, 0, "HeavensDoorGalaxy", 0},
    {"AppearHeavensDoorGalaxy", 04, 01, 0, 0, 0, "SpecialStarRed1", 0},
    {"EventCometStarter", 01, 01, 0x0D, 0, 0, 0, 0},
    {"EventKinopioExplorerRescued", 03, 01, 01, 0, 0, "HoneyBeeKingdomGalaxy", 0},
    {"EventKinopioExplorerOrganize", 01, 01, 07, 0, 0, 0, 0},
    {"ExOpen1CocoonExGalaxy", 03, 01, 03, 0, 0, "StarDustGalaxy", 0},
    {"ExOpen1TearDropGalaxy", 03, 01, 01, 0, 0, "HeavenlyBeachGalaxy", 0},
    {"ExOpen1FishTunnelGalaxy", 03, 01, 01, 0, 0, "SandClockGalaxy", 0},
    {"ExOpenCommonBase", 03, 01, 01, 0, 0, "PhantomGalaxy", 0},
    {"ExOpenSubTransformationExGalaxy", 03, 01, 02, 0, 0, "OceanRingGalaxy", 0},
    {"ExOpenSubTeresaMario2DGalaxy", 03, 01, 02, 0, 0, "HeavensDoorGalaxy", 0},
    {"ExOpen1TransformationExGalaxy", 04, 01, 0, 0, 0, "ExOpenCommonBase", "ExOpenSubTransformationExGalaxy"},
    {"ExOpen1TeresaMario2DGalaxy", 04, 01, 0, 0, 0, "ExOpenCommonBase", "ExOpenSubTeresaMario2DGalaxy"},
    {"ExOpen1SnowCapsuleGalaxy", 03, 01, 01, 0, 0, "HellProminenceGalaxy", 0},
    {"ExOpen1BeltConveyerExGalaxy", 01, 01, 07, 0, 0, 0, 0},
    {"AppearBeltConveyerExGalaxy", 04, 01, 0, 0, 0, "SpecialStarGrand1", "ExOpen1BeltConveyerExGalaxy"},
    {"AppearCocoonExGalaxy", 04, 01, 0, 0, 0, "SpecialStarGrand2", "ExOpen1CocoonExGalaxy"},
    {"AppearTearDropGalaxy", 04, 01, 0, 0, 0, "SpecialStarGrand3", "ExOpen1TearDropGalaxy"},
    {"AppearFishTunnelGalaxy", 04, 01, 0, 0, 0, "SpecialStarGrand4", "ExOpen1FishTunnelGalaxy"},
    {"AppearTransformationExGalaxy", 04, 01, 0, 0, 0, "SpecialStarGrand5", "ExOpen1TransformationExGalaxy"},
    {"AppearTeresaMario2DGalaxy", 04, 01, 0, 0, 0, "SpecialStarGrand6", "ExOpen1TeresaMario2DGalaxy"},
    {"AppearSnowCapsuleGalaxy", 04, 01, 0, 0, 0, "SpecialStarGrand6", "ExOpen1SnowCapsuleGalaxy"},
    {"StarPieceBeltConveyerExGalaxy", 0x08, 01, 0, 0x28, 0, "BeltConveyerExGalaxy", 0},
    {"StarPieceCocoonExGalaxy", 0x08, 01, 01, 0x28, 0, "CocoonExGalaxy", 0},
    {"StarPieceTearDropGalaxy", 0x08, 01, 02, 0x3C, 0, "TearDropGalaxy", 0},
    {"StarPieceFishTunnelGalaxy", 0x08, 01, 06, 0x50, 0, "FishTunnelGalaxy", 0},
    {"StarPieceTransformationExGalaxy", 0x08, 01, 05, 0x64, 0, "TransformationExGalaxy", 0},
    {"StarPieceTeresaMario2DGalaxy", 0x08, 01, 03, 0x78, 0, "TeresaMario2DGalaxy", 0},
    {"StarPieceSnowCapsuleGalaxy", 0x08, 01, 04, 0xA0, 0, "SnowCapsuleGalaxy", 0},
    {"AppearTamakoroExLv2Galaxy", 04, 01, 0, 0, 0, "SpecialStarGreenAll", 0},
    {"AppearSurfingLv2Galaxy", 04, 01, 0, 0, 0, "SpecialStarGreenAll", 0},
    {"AppearCubeBubbleExLv2Galaxy", 04, 01, 0, 0, 0, "SpecialStarGreenAll", 0},
    {"AppearPeachCastleFinalGalaxy", 04, 01, 0, 0, 0, "ViewCompleteMarioAndLuigi", 0},
    {"LibraryOpenRequirement", 0x09, 01, 0, 0, 0, "LibraryOpenNewStarCount", "SpecialStarGrand3"},
    {"StarNumPictureBookB", 01, 01, 0x18, 0, 0, 0, 0},
    {"StarNumPictureBookC", 01, 01, 0x1C, 0, 0, 0, 0},
    {"StarNumPictureBookD", 01, 01, 0x20, 0, 0, 0, 0},
    {"StarNumPictureBookE", 01, 01, 0x28, 0, 0, 0, 0},
    {"StarNumPictureBookF", 01, 01, 0x31, 0, 0, 0, 0},
    {"StarNumPictureBookG", 01, 01, 0x3A, 0, 0, 0, 0},
    {"PictureBookA", 04, 01, 0, 0, 0, "LibraryOpenRequirement", 0},
    {"PictureBookB", 04, 01, 0, 0, 0, "StarNumPictureBookB", "LibraryOpenRequirement"},
    {"PictureBookC", 04, 01, 0, 0, 0, "StarNumPictureBookC", "LibraryOpenRequirement"},
    {"PictureBookD", 04, 01, 0, 0, 0, "StarNumPictureBookD", "LibraryOpenRequirement"},
    {"PictureBookE", 04, 01, 0, 0, 0, "StarNumPictureBookE", "LibraryOpenRequirement"},
    {"PictureBookF", 04, 01, 0, 0, 0, "StarNumPictureBookF", "LibraryOpenRequirement"},
    {"PictureBookG", 04, 01, 0, 0, 0, "StarNumPictureBookG", "LibraryOpenRequirement"},
    {"PictureBookH", 03, 01, 01, 0, 0, "KoopaBattleVs3Galaxy", 0},
    {"PictureBookI", 03, 01, 01, 0, 0, "KoopaBattleVs3Galaxy", 0},
    {"SpecialStarLuigiRescued", 03, 01, 01, 0, 0, "PhantomGalaxy", 0},
    {"LuigiTalkAfterRescued", 04, 0, 0, 0, 0, "SpecialStarLuigiRescued", 0},
    {"SpecialStarFindingLuigi1", 03, 01, 06, 0, 0, "EggStarGalaxy", 0},
    {"SpecialStarFindingLuigi2", 03, 01, 07, 0, 0, "BattleShipGalaxy", 0},
    {"SpecialStarFindingLuigi3", 03, 01, 06, 0, 0, "HoneyBeeKingdomGalaxy", 0},
    {"RosettaTalkAboutTico", 04, 0, 0, 0, 0, "SpecialStarGrand6", 0},
    {"RosettaTalkAfterNormalEnding", 04, 0, 0, 0, 0, "ViewNormalEnding", 0},
    {"ViewNormalEnding", 04, 01, 0, 0, 0, "PictureBookH", 0},
    {"PowerStarComplete", 01, 01, 0x78, 0, 0, 0, 0},
    {"ViewCompleteEnding", 04, 0, 0, 0, 0, "PowerStarComplete", 0},
    {"ViewCompleteMarioAndLuigi", 0x0A, 01, 0, 0, 0, 0, 0},
    {"SpecialStarFinalChallenge", 03, 01, 01, 0, 0, "PeachCastleFinalGalaxy",  0},
    {"StarPieceCounterStop", 0, 0, 0, 0, 0, 0, 0},
    {"PlayHeavensDoorGalaxy1", 04, 01, 0, 0, 0, 0, 0},
    {"PlayHeavensDoorGalaxy2", 04, 01, 0, 0, 0, "SpecialStarGrand6", 0},
    {"[同期]ピーチ城浮上後", 0x0B, 01, 0, 0, 0, "PlayHeavensDoorGalaxy1", "ピーチ城浮上後"},
    {"PlayEggStarGalaxy1", 04, 01, 0, 0, 0, "SpecialStarGrand1", 0},
    {"[同期]バトラー情報Ａ", 0x0B, 01, 0, 0, 0, "PlayEggStarGalaxy1", "バトラー情報Ａ"},
    {"SpecialStarEggStarGalaxy1", 03, 01, 01, 0, 0, "EggStarGalaxy", 0},
    {"PlayEggStarGalaxy2", 04, 01, 0, 0, 0, "SpecialStarEggStarGalaxy1", 0},
    {"[同期]スターピースレクチャー", 0x0B, 01, 0, 0, 0, "PlayEggStarGalaxy2", "スターピースレクチャー"},
    {"PowerStarNumForButlerKoopaDemo", 02, 01, 0, 0, 0, "KoopaBattleVs1Galaxy", 0},
    {"[同期]クッパスタープラント発見", 0x0B, 01, 0, 0, 0, "PowerStarNumForButlerKoopaDemo", "クッパスタープラント発見"}
    };

    struct GameEventFlagSortLt {
        bool operator()(GameEventFlagTableInstance::Key *key1, GameEventFlagTableInstance::Key *key2) {
            return (key1->mHashCode < key2->mHashCode) ? true : false;
        }
    };
};

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
    }
    else {
        return false;
    }
}

GameEventFlagTableInstance::GameEventFlagTableInstance(){ 
    mSortTable = nullptr;
    mLength = 0;

    GameEventFlagTableInstance::initSortTable();
}

const GameEventFlag* GameEventFlagTableInstance::findFlag(const char *flagName) {
    int i;
    u16 hashCode = MR::getHashCode(flagName);
    i = mLength;
    i = (i * 8) / 8;
    GameEventFlagTableInstance::Key *key = mSortTable;

    while (i > 0) {
        int mid = i / 2;
        if ((&key[i / 2])->mHashCode < hashCode) {
            i -= mid + 1;
            key = &key[mid] + 1;
        }
        else {
            i = mid;
        }
    }

    if (key->mHashCode == hashCode && MR::isEqualString(flagName,key->mFlag->mName)) {
        return key->mFlag;
    }

    return nullptr;
}

void GameEventFlagTableInstance::initSortTable() {
    mSortTable = new GameEventFlagTableInstance::Key[0xbc];
    mLength = 0xbc;

    for (int i = 0; i < mLength; i++) {
        const GameEventFlag* flag = GameEventFlagTable::getFlag(i);
        mSortTable[i].mFlag = flag;
        mSortTable[i].mHashCode = MR::getHashCode(flag->mName);
    }

    std::sort(&mSortTable[0], &mSortTable[mLength], GameEventFlagSortLt());
}

GameEventFlagIter GameEventFlagTable::getBeginIter() {
    return GameEventFlagIter();
}

GameEventFlagAccessor GameEventFlagTable::makeAccessor(const char* flagName) {
    return GameEventFlagTable::findFlag(flagName);
}

s32 GameEventFlagTable::getTableSize() {
    return 0xBC;
}

const GameEventFlag* GameEventFlagTable::getFlag(int index) {
    if (index < 0 || index >= GameEventFlagTable::getTableSize()) {
        return nullptr;
    }
    else {
        return &sGameEventFlagStatic[index];
    }
}

const GameEventFlag* GameEventFlagTable::findFlag(const char* flagName) {
    return SingletonHolder<GameEventFlagTableInstance>::get()->findFlag(flagName);
}

const char* GameEventFlagTable::getEventFlagNameSpecialPowerStar(const char *galaxyName, s32 starId) {
    for (GameEventFlagIter iter = GameEventFlagTable::getBeginIter(); !iter.isEnd(); iter.goNext()) {
        GameEventFlagAccessor flagAccessor = GameEventFlagAccessor(iter.getFlag());
        
        if (flagAccessor.isTypeSpecialStar() && flagAccessor.getStarId() == starId) {
            if(MR::isEqualString(galaxyName, flagAccessor.getGalaxyName())) {
                return flagAccessor.getName();
            }
        }
    }
    return nullptr;
}

 bool GameEventFlagTable::isPowerStarType(const char *galaxyName, s32 starId, const char *starType) {
    const char* name = GameEventFlagTable::getEventFlagNameSpecialPowerStar(galaxyName, starId);
    if (name == nullptr) {
        return false;
    }

    return MR::isEqualString(name, starType);
 };

s32 GameEventFlagTable::calcExclamationGalaxyNum() {
    s32 num = -1;

    for (GameEventFlagIter iter = GameEventFlagTable::getBeginIter(); !iter.isEnd(); iter.goNext()) {
        GameEventFlagAccessor flagAccessor = GameEventFlagAccessor(iter.getFlag());

        if(flagAccessor.isTypeStarPiece() && num < flagAccessor.getStarPieceIndex()) {
            num = flagAccessor.getStarPieceIndex();
        }
    }

    return num + 1;
};

const char* GameEventFlagTable::getExclamationGalaxyNameFromIndex(int index) {
    for (GameEventFlagIter iter = GameEventFlagTable::getBeginIter(); !iter.isEnd(); iter.goNext()) {
        GameEventFlagAccessor flagAccessor = GameEventFlagAccessor(iter.getFlag());

        if (flagAccessor.isTypeStarPiece() && flagAccessor.getStarPieceIndex() == index) {
            return flagAccessor.getGalaxyNameWithStarPiece();
        }
    }

    return nullptr;
};

bool GameEventFlagTable::isExist(const char * flagName) {
    return GameEventFlagTable::findFlag(flagName) != nullptr;
};

int GameEventFlagTable::getIndex(const GameEventFlag *eventFlag) {
    return eventFlag - &sGameEventFlagStatic[0];
};

bool GameEventFlagTable::isDependedAnother(const char *flagName1, const char *flagName2) {
    const GameEventFlag *flag1 = GameEventFlagTable::findFlag(flagName1);

    if (flag1->mType == 0x4) {
        if (flag1->mGalaxyName != nullptr && MR::isEqualString(flag1->mGalaxyName, flagName2)) {
            return true;
        }
        else if (flag1->mRequirement != nullptr && MR::isEqualString(flag1->mRequirement, flagName2)) {
            return true;
        }
    }
    else if (flag1->mType == 0x06) {
        const char* dependedFlags[3];
        s32 length = GameEventFlagTable::getGalaxyDependedFlags(dependedFlags, 3, flagName1);

        for (s32 i = 0; i < length; i++) {
            if (MR::isEqualString(dependedFlags[i], flagName2)) {
                return true;
            }
        }
    }

    return false;
};

int GameEventFlagTable::getIndexFromHashCode(u16 hashCode) {
    for (GameEventFlagIter iter = GameEventFlagTable::getBeginIter(); !iter.isEnd(); iter.goNext()) {

        if (!(iter.getFlag()->_5 & 0x1)) {
            GameEventFlagAccessor flagAccessor = GameEventFlagAccessor(iter.getFlag());

            if ((MR::getHashCode(flagAccessor.getName()) &0x7FFF) == hashCode) {
                return GameEventFlagTable::getFlag(iter.mIter) - &sGameEventFlagStatic[0];
            }
        }
    }

    return -1;
};

s32 GameEventFlagTable::calcSpecialPowerStarNum(const char *powerStarType) {
    s32 num = 0;

    for (GameEventFlagIter iter = GameEventFlagTable::getBeginIter(); !iter.isEnd(); iter.goNext()) {
        GameEventFlagAccessor flagAccessor = GameEventFlagAccessor(iter.getFlag());

        if (flagAccessor.isTypeSpecialStar() && strstr(flagAccessor.getName(), powerStarType)) {
            num++;
        }
    }

    return num;
};

s32 GameEventFlagTable::getStarPieceNumToOpenExclamationGalaxy(const char *galaxyName) {
    char dest[0x40];

    snprintf(dest,sizeof(dest), "StarPiece%s",galaxyName);

    GameEventFlagAccessor flagAccessor = makeAccessor(dest);
    return flagAccessor.getNeedStarPieceNum();
}

s32 GameEventFlagTable::calcGreenPowerStarNum() {
    return GameEventFlagTable::calcSpecialPowerStarNum("SpecialStarGreen");
}

s32 GameEventFlagTable::getGalaxyDependedFlags(const char **Dest, int length, const char *galaxyName) {
    JMapInfo info = JMapInfo();
    // this should be "Unlabeled Table 2" at https://www.lumasworkshop.com/wiki/Progress_checks
    void *temp;
    info.attach(temp);
    JMapInfoIter thing = info.findElement("name", galaxyName, 0);

    char string[0x20];
    int returnLen = 0;
    const char* value = "";

    for (int i = 0; i < 3; i++) {
        snprintf(string,sizeof(string),"OpenCondition%1d",i);

        if (info.searchItemInfo(string) < 0) {
            break;
        }

        info.getValue(thing.mIndex, string, &value);

        if(!MR::isEqualString(value,"")){
            Dest[returnLen] = value;
            returnLen++;
        }
    }
    
    return returnLen;
}
