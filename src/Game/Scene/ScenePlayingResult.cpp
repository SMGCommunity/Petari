#include "Game/Scene/ScenePlayingResult.hpp"
#include "Game/System/GameDataFunction.hpp"
#include "Game/Util.hpp"
#include "Game/Util/EventUtil.hpp"
#include "Game/Util/PlayerUtil.hpp"
#include "Game/Util/SceneUtil.hpp"

namespace {
    void requestOneUpIfNeed(int oldAmount, int newAmount, int a3) NO_INLINE {
        int v3 = oldAmount / a3;
        if (v3 < newAmount / a3) {
            MR::incPlayerLeft();
            MR::requestOneUp();
        }
    }
};

void ScenePlayingResult::incCoin(int num) {
    s32 prev = mCoinNum;
    mCoinNum = MR::clamp(mCoinNum + num, 0, 999);
    ::requestOneUpIfNeed(prev, mCoinNum, 50);
}

s32 ScenePlayingResult::getCoinNum() const {
    return MR::clamp(mCoinNum, 0, 999);
}

void ScenePlayingResult::incPurpleCoin() {
    mPurpleCoinNum++;
    if (mPurpleCoinNum == 100) {
        MR::requestAppearPowerStarCoin100();
    }
}

void ScenePlayingResult::incStarPiece(int num) {
    int pieceNum = GameDataFunction::getStarPieceNum();
    GameDataFunction::addStarPiece(num);
    int newPieceNum = GameDataFunction::getStarPieceNum();
    int last1up = GameDataFunction::getLast1upStarPieceNum();

    if (!MR::isStageAstroLocation() && last1up + 50 <= newPieceNum) {
        if ((pieceNum / 50) < (newPieceNum / 50)) {
            MR::incPlayerLeft();
            MR::requestOneUp();
        }

        GameDataFunction::setLast1upStarPieceNum(last1up + 50);
    }
}

s32 ScenePlayingResult::getStarPieceNum() const {
    return GameDataFunction::getStarPieceNum();
}

ScenePlayingResult::~ScenePlayingResult() {

}

ScenePlayingResult::ScenePlayingResult() : NameObj("プレイ結果保持") {
    mCoinNum = 0;
    mPurpleCoinNum = 0;
}
