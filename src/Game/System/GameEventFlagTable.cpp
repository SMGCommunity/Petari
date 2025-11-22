#include "Game/System/GameEventFlagTable.hpp"
#include "Game/Util/HashUtil.hpp"
#include "Game/Util/StringUtil.hpp"
#include "Game/SingletonHolder.hpp"
#include "Game/Util/JMapInfo.hpp"

#include <algorithm>
#include <cstdio>

namespace {
    static const GameEventFlag sGameEventFlagStatic[] = {
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
        {"Coin100CometEggStarGalaxy", 7, 1, 1, 0, 0, "EggStarGalaxy", "ViewNormalEnding"},
        {"Coin100CometHoneyBeeKingdomGalaxy", 7, 1, 1, 0, 0, "HoneyBeeKingdomGalaxy", "EventCoin100CometStarter"},
        {"Coin100CometStarDustGalaxy", 7, 1, 1, 0, 0, "StarDustGalaxy", "EventCoin100CometStarter"},
        {"Coin100CometBattleShipGalaxy", 7, 1, 1, 0, 0, "BattleShipGalaxy", "EventCoin100CometStarter"},
        {"Coin100CometHeavenlyBeachGalaxy", 7, 1, 1, 0, 0, "HeavenlyBeachGalaxy", "EventCoin100CometStarter"},
        {"Coin100CometPhantomGalaxy", 7, 1, 2, 0, 0, "PhantomGalaxy", "EventCoin100CometStarter"},
        {"Coin100CometCosmosGardenGalaxy", 7, 1, 1, 0, 0, "CosmosGardenGalaxy", "EventCoin100CometStarter"},
        {"Coin100CometIceVolcanoGalaxy", 7, 1, 1, 0, 0, "IceVolcanoGalaxy", "EventCoin100CometStarter"},
        {"Coin100CometSandClockGalaxy", 7, 1, 2, 0, 0, "SandClockGalaxy", "EventCoin100CometStarter"},
        {"Coin100CometReverseKingdomGalaxy", 7, 1, 3, 0, 0, "ReverseKingdomGalaxy", "EventCoin100CometStarter"},
        {"Coin100CometOceanRingGalaxy", 7, 1, 3, 0, 0, "OceanRingGalaxy", "EventCoin100CometStarter"},
        {"Coin100CometFactoryGalaxy", 7, 1, 2, 0, 0, "FactoryGalaxy", "EventCoin100CometStarter"},
        {"Coin100CometOceanPhantomCaveGalaxy", 7, 1, 1, 0, 0, "OceanPhantomCaveGalaxy", "EventCoin100CometStarter"},
        {"Coin100CometCannonFleetGalaxy", 7, 1, 2, 0, 0, "CannonFleetGalaxy", "EventCoin100CometStarter"},
        {"Coin100CometHellProminenceGalaxy", 7, 1, 3, 0, 0, "HellProminenceGalaxy", "EventCoin100CometStarter"},
        {"SpecialStarGrand1", 3, 1, 1, 0, 0, "HeavensDoorGalaxy", 0},
        {"SpecialStarGrand2", 3, 1, 1, 0, 0, "TriLegLv1Galaxy", 0},
        {"SpecialStarGrand3", 3, 1, 1, 0, 0, "KoopaBattleVs1Galaxy", 0},
        {"SpecialStarGrand4", 3, 1, 1, 0, 0, "KoopaJrShipLv1Galaxy", 0},
        {"SpecialStarGrand5", 3, 1, 1, 0, 0, "KoopaBattleVs2Galaxy", 0},
        {"SpecialStarGrand6", 3, 1, 1, 0, 0, "FloaterOtaKingGalaxy", 0},
        {"SpecialStarGrand7", 3, 1, 1, 0, 0, "KoopaBattleVs3Galaxy", 0},
        {"SpecialStarGreen1", 3, 1, 7, 0, 0, "BattleShipGalaxy", 0},
        {"SpecialStarGreen2", 3, 1, 2, 0, 0, "OceanFloaterLandGalaxy", 0},
        {"SpecialStarGreen3", 3, 1, 7, 0, 0, "SandClockGalaxy", 0},
        {"SpecialStarGreen1/2", 4, 1, 0, 0, 0, "SpecialStarGreen1", "SpecialStarGreen2"},
        {"SpecialStarGreenAll", 4, 1, 0, 0, 0, "SpecialStarGreen1/2", "SpecialStarGreen3"},
        {"SpecialStarRed1", 3, 1, 2, 0, 0, "HeavensDoorGalaxy", 0},
        {"AppearHeavensDoorGalaxy", 4, 1, 0, 0, 0, "SpecialStarRed1", 0},
        {"EventCometStarter", 1, 1, 13, 0, 0, 0, 0},
        {"EventKinopioExplorerRescued", 3, 1, 1, 0, 0, "HoneyBeeKingdomGalaxy", 0},
        {"EventKinopioExplorerOrganize", 1, 1, 7, 0, 0, 0, 0},
        {"ExOpen1CocoonExGalaxy", 3, 1, 3, 0, 0, "StarDustGalaxy", 0},
        {"ExOpen1TearDropGalaxy", 3, 1, 1, 0, 0, "HeavenlyBeachGalaxy", 0},
        {"ExOpen1FishTunnelGalaxy", 3, 1, 1, 0, 0, "SandClockGalaxy", 0},
        {"ExOpenCommonBase", 3, 1, 1, 0, 0, "PhantomGalaxy", 0},
        {"ExOpenSubTransformationExGalaxy", 3, 1, 2, 0, 0, "OceanRingGalaxy", 0},
        {"ExOpenSubTeresaMario2DGalaxy", 3, 1, 2, 0, 0, "HeavensDoorGalaxy", 0},
        {"ExOpen1TransformationExGalaxy", 4, 1, 0, 0, 0, "ExOpenCommonBase", "ExOpenSubTransformationExGalaxy"},
        {"ExOpen1TeresaMario2DGalaxy", 4, 1, 0, 0, 0, "ExOpenCommonBase", "ExOpenSubTeresaMario2DGalaxy"},
        {"ExOpen1SnowCapsuleGalaxy", 3, 1, 1, 0, 0, "HellProminenceGalaxy", 0},
        {"ExOpen1BeltConveyerExGalaxy", 1, 1, 7, 0, 0, 0, 0},
        {"AppearBeltConveyerExGalaxy", 4, 1, 0, 0, 0, "SpecialStarGrand1", "ExOpen1BeltConveyerExGalaxy"},
        {"AppearCocoonExGalaxy", 4, 1, 0, 0, 0, "SpecialStarGrand2", "ExOpen1CocoonExGalaxy"},
        {"AppearTearDropGalaxy", 4, 1, 0, 0, 0, "SpecialStarGrand3", "ExOpen1TearDropGalaxy"},
        {"AppearFishTunnelGalaxy", 4, 1, 0, 0, 0, "SpecialStarGrand4", "ExOpen1FishTunnelGalaxy"},
        {"AppearTransformationExGalaxy", 4, 1, 0, 0, 0, "SpecialStarGrand5", "ExOpen1TransformationExGalaxy"},
        {"AppearTeresaMario2DGalaxy", 4, 1, 0, 0, 0, "SpecialStarGrand6", "ExOpen1TeresaMario2DGalaxy"},
        {"AppearSnowCapsuleGalaxy", 4, 1, 0, 0, 0, "SpecialStarGrand6", "ExOpen1SnowCapsuleGalaxy"},
        {"StarPieceBeltConveyerExGalaxy", 8, 1, 0, 40, 0, "BeltConveyerExGalaxy", 0},
        {"StarPieceCocoonExGalaxy", 8, 1, 1, 40, 0, "CocoonExGalaxy", 0},
        {"StarPieceTearDropGalaxy", 8, 1, 2, 60, 0, "TearDropGalaxy", 0},
        {"StarPieceFishTunnelGalaxy", 8, 1, 6, 80, 0, "FishTunnelGalaxy", 0},
        {"StarPieceTransformationExGalaxy", 8, 1, 5, 100, 0, "TransformationExGalaxy", 0},
        {"StarPieceTeresaMario2DGalaxy", 8, 1, 3, 120, 0, "TeresaMario2DGalaxy", 0},
        {"StarPieceSnowCapsuleGalaxy", 8, 1, 4, 160, 0, "SnowCapsuleGalaxy", 0},
        {"AppearTamakoroExLv2Galaxy", 4, 1, 0, 0, 0, "SpecialStarGreenAll", 0},
        {"AppearSurfingLv2Galaxy", 4, 1, 0, 0, 0, "SpecialStarGreenAll", 0},
        {"AppearCubeBubbleExLv2Galaxy", 4, 1, 0, 0, 0, "SpecialStarGreenAll", 0},
        {"AppearPeachCastleFinalGalaxy", 4, 1, 0, 0, 0, "ViewCompleteMarioAndLuigi", 0},
        {"LibraryOpenRequirement", 9, 1, 0, 0, 0, "LibraryOpenNewStarCount", "SpecialStarGrand3"},
        {"StarNumPictureBookB", 1, 1, 24, 0, 0, 0, 0},
        {"StarNumPictureBookC", 1, 1, 28, 0, 0, 0, 0},
        {"StarNumPictureBookD", 1, 1, 32, 0, 0, 0, 0},
        {"StarNumPictureBookE", 1, 1, 40, 0, 0, 0, 0},
        {"StarNumPictureBookF", 1, 1, 49, 0, 0, 0, 0},
        {"StarNumPictureBookG", 1, 1, 58, 0, 0, 0, 0},
        {"PictureBookA", 4, 1, 0, 0, 0, "LibraryOpenRequirement", 0},
        {"PictureBookB", 4, 1, 0, 0, 0, "StarNumPictureBookB", "LibraryOpenRequirement"},
        {"PictureBookC", 4, 1, 0, 0, 0, "StarNumPictureBookC", "LibraryOpenRequirement"},
        {"PictureBookD", 4, 1, 0, 0, 0, "StarNumPictureBookD", "LibraryOpenRequirement"},
        {"PictureBookE", 4, 1, 0, 0, 0, "StarNumPictureBookE", "LibraryOpenRequirement"},
        {"PictureBookF", 4, 1, 0, 0, 0, "StarNumPictureBookF", "LibraryOpenRequirement"},
        {"PictureBookG", 4, 1, 0, 0, 0, "StarNumPictureBookG", "LibraryOpenRequirement"},
        {"PictureBookH", 3, 1, 1, 0, 0, "KoopaBattleVs3Galaxy", 0},
        {"PictureBookI", 3, 1, 1, 0, 0, "KoopaBattleVs3Galaxy", 0},
        {"SpecialStarLuigiRescued", 3, 1, 1, 0, 0, "PhantomGalaxy", 0},
        {"LuigiTalkAfterRescued", 4, 0, 0, 0, 0, "SpecialStarLuigiRescued", 0},
        {"SpecialStarFindingLuigi1", 3, 1, 6, 0, 0, "EggStarGalaxy", 0},
        {"SpecialStarFindingLuigi2", 3, 1, 7, 0, 0, "BattleShipGalaxy", 0},
        {"SpecialStarFindingLuigi3", 3, 1, 6, 0, 0, "HoneyBeeKingdomGalaxy", 0},
        {"RosettaTalkAboutTico", 4, 0, 0, 0, 0, "SpecialStarGrand6", 0},
        {"RosettaTalkAfterNormalEnding", 4, 0, 0, 0, 0, "ViewNormalEnding", 0},
        {"ViewNormalEnding", 4, 1, 0, 0, 0, "PictureBookH", 0},
        {"PowerStarComplete", 1, 1, 120, 0, 0, 0, 0},
        {"ViewCompleteEnding", 4, 0, 0, 0, 0, "PowerStarComplete", 0},
        {"ViewCompleteMarioAndLuigi", 10, 1, 0, 0, 0, 0, 0},
        {"SpecialStarFinalChallenge", 3, 1, 1, 0, 0, "PeachCastleFinalGalaxy", 0},
        {"StarPieceCounterStop", 0, 0, 0, 0, 0, 0, 0},
        {"PlayHeavensDoorGalaxy1", 4, 1, 0, 0, 0, 0, 0},
        {"PlayHeavensDoorGalaxy2", 4, 1, 0, 0, 0, "SpecialStarGrand6", 0},
        {"[同期]ピーチ城浮上後", 11, 1, 0, 0, 0, "PlayHeavensDoorGalaxy1", "ピーチ城浮上後"},
        {"PlayEggStarGalaxy1", 4, 1, 0, 0, 0, "SpecialStarGrand1", 0},
        {"[同期]バトラー情報Ａ", 11, 1, 0, 0, 0, "PlayEggStarGalaxy1", "バトラー情報Ａ"},
        {"SpecialStarEggStarGalaxy1", 3, 1, 1, 0, 0, "EggStarGalaxy", 0},
        {"PlayEggStarGalaxy2", 4, 1, 0, 0, 0, "SpecialStarEggStarGalaxy1", 0},
        {"[同期]スターピースレクチャー", 11, 1, 0, 0, 0, "PlayEggStarGalaxy2", "スターピースレクチャー"},
        {"PowerStarNumForButlerKoopaDemo", 2, 1, 0, 0, 0, "KoopaBattleVs1Galaxy", 0},
        {"[同期]クッパスタープラント発見", 11, 1, 0, 0, 0, "PowerStarNumForButlerKoopaDemo", "クッパスタープラント発見"}};

    struct GameEventFlagSortLt {
        bool operator()(GameEventFlagTableInstance::Key* key1, GameEventFlagTableInstance::Key* key2) {
            return (key1->mHashCode < key2->mHashCode) ? true : false;
        }
    };
}; // namespace

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

GameEventFlagTableInstance::GameEventFlagTableInstance() {
    mSortTable = nullptr;
    mLength = 0;

    GameEventFlagTableInstance::initSortTable();
}

const GameEventFlag* GameEventFlagTableInstance::findFlag(const char* flagName) {
    int i;
    u16 hashCode = MR::getHashCode(flagName);
    i = mLength;
    i = (i * 8) / 8;
    GameEventFlagTableInstance::Key* key = mSortTable;

    while (i > 0) {
        int mid = i / 2;
        if ((&key[i / 2])->mHashCode < hashCode) {
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
    s32 tableLength = sizeof(sGameEventFlagStatic) / sizeof(*sGameEventFlagStatic);
    mSortTable = new GameEventFlagTableInstance::Key[tableLength];
    mLength = tableLength;

    for (int i = 0; i < mLength; i++) {
        const GameEventFlag* flag = GameEventFlagTable::getFlag(i);
        mSortTable[i].mFlag = flag;
        mSortTable[i].mHashCode = MR::getHashCode(flag->mName);
    }

    std::sort(&mSortTable[0], &mSortTable[mLength], GameEventFlagSortLt());
}
namespace GameEventFlagTable {

    GameEventFlagIter getBeginIter() {
        return GameEventFlagIter();
    }

    GameEventFlagAccessor makeAccessor(const char* flagName) {
        return findFlag(flagName);
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

    const GameEventFlag* findFlag(const char* flagName) {
        return SingletonHolder< GameEventFlagTableInstance >::get()->findFlag(flagName);
    }

    const char* getEventFlagNameSpecialPowerStar(const char* galaxyName, s32 starId) {
        for (GameEventFlagIter iter = getBeginIter(); !iter.isEnd(); iter.goNext()) {
            GameEventFlagAccessor flagAccessor = GameEventFlagAccessor(iter.getFlag());

            if (flagAccessor.isTypeSpecialStar() && flagAccessor.getStarId() == starId) {
                if (MR::isEqualString(galaxyName, flagAccessor.getGalaxyName())) {
                    return flagAccessor.getName();
                }
            }
        }
        return nullptr;
    }

    bool isPowerStarType(const char* galaxyName, s32 starId, const char* starType) {
        const char* name = getEventFlagNameSpecialPowerStar(galaxyName, starId);
        if (name == nullptr) {
            return false;
        }

        return MR::isEqualString(name, starType);
    };

    s32 calcExclamationGalaxyNum() {
        s32 num = -1;

        for (GameEventFlagIter iter = getBeginIter(); !iter.isEnd(); iter.goNext()) {
            GameEventFlagAccessor flagAccessor = GameEventFlagAccessor(iter.getFlag());

            if (flagAccessor.isTypeStarPiece() && num < flagAccessor.getStarPieceIndex()) {
                num = flagAccessor.getStarPieceIndex();
            }
        }

        return num + 1;
    };

    const char* getExclamationGalaxyNameFromIndex(int index) {
        for (GameEventFlagIter iter = getBeginIter(); !iter.isEnd(); iter.goNext()) {
            GameEventFlagAccessor flagAccessor = GameEventFlagAccessor(iter.getFlag());

            if (flagAccessor.isTypeStarPiece() && flagAccessor.getStarPieceIndex() == index) {
                return flagAccessor.getGalaxyNameWithStarPiece();
            }
        }

        return nullptr;
    };

    bool isExist(const char* flagName) {
        return findFlag(flagName) != nullptr;
    };

    int getIndex(const GameEventFlag* eventFlag) {
        return eventFlag - &sGameEventFlagStatic[0];
    };

    bool isDependedAnother(const char* flagName1, const char* flagName2) {
        const GameEventFlag* flag1 = findFlag(flagName1);

        if (flag1->mType == 0x4) {
            if (flag1->mRequirement1 != nullptr && MR::isEqualString(flag1->mRequirement1, flagName2)) {
                return true;
            } else if (flag1->mRequirement2 != nullptr && MR::isEqualString(flag1->mRequirement2, flagName2)) {
                return true;
            }
        } else if (flag1->mType == 0x06) {
            const char* dependedFlags[3];
            s32         length = getGalaxyDependedFlags(dependedFlags, 3, flagName1);

            for (s32 i = 0; i < length; i++) {
                if (MR::isEqualString(dependedFlags[i], flagName2)) {
                    return true;
                }
            }
        }

        return false;
    };

    int getIndexFromHashCode(u16 hashCode) {
        for (GameEventFlagIter iter = getBeginIter(); !iter.isEnd(); iter.goNext()) {

            if (!(iter.getFlag()->saveFlag & 0x1)) {
                GameEventFlagAccessor flagAccessor = GameEventFlagAccessor(iter.getFlag());

                if ((MR::getHashCode(flagAccessor.getName()) & 0x7FFF) == hashCode) {
                    return getFlag(iter.mIter) - &sGameEventFlagStatic[0];
                }
            }
        }

        return -1;
    };

    s32 calcSpecialPowerStarNum(const char* powerStarType) {
        s32 num = 0;

        for (GameEventFlagIter iter = getBeginIter(); !iter.isEnd(); iter.goNext()) {
            GameEventFlagAccessor flagAccessor = GameEventFlagAccessor(iter.getFlag());

            if (flagAccessor.isTypeSpecialStar() && strstr(flagAccessor.getName(), powerStarType)) {
                num++;
            }
        }

        return num;
    };

    s32 getStarPieceNumToOpenExclamationGalaxy(const char* galaxyName) {
        char dest[0x40];

        snprintf(dest, sizeof(dest), "StarPiece%s", galaxyName);

        GameEventFlagAccessor flagAccessor = makeAccessor(dest);
        return flagAccessor.getNeedStarPieceNum();
    }

    s32 calcGreenPowerStarNum() {
        return calcSpecialPowerStarNum("SpecialStarGreen");
    }

} // namespace GameEventFlagTable
