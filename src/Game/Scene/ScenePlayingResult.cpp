#include "Game/Scene/ScenePlayingResult.hpp"
#include "Game/System/GameDataFunction.hpp"
#include "Game/Util/EventUtil.hpp"
#include "Game/Util/MathUtil.hpp"
#include "Game/Util/ObjUtil.hpp"
#include "Game/Util/SceneUtil.hpp"
#include "Game/Util/ScreenUtil.hpp"

#define COIN_NUM_MIN 0
#define COIN_NUM_MAX 999
#define COIN_STEP_NUM 50
#define PURPLE_COIN_POWER_STAR_APPEAR_NUM 100
#define STAR_PIECE_STEP_NUM 50

namespace {
    void requestOneUpIfNeed(int prevNum, int currNum, int stepNum) NO_INLINE {
        if (prevNum / stepNum < currNum / stepNum) {
            MR::incPlayerLeft();
            MR::requestOneUp();
        }
    }
};

void ScenePlayingResult::incCoin(int num) {
    s32 prevNum = mCoinNum;
    s32 currNum = MR::clamp(mCoinNum + num, COIN_NUM_MIN, COIN_NUM_MAX);
    mCoinNum = currNum;

    requestOneUpIfNeed(prevNum, currNum, COIN_STEP_NUM);
}

s32 ScenePlayingResult::getCoinNum() const {
    return MR::clamp(mCoinNum, COIN_NUM_MIN, COIN_NUM_MAX);
}

void ScenePlayingResult::incPurpleCoin() {
    mPurpleCoinNum++;

    if (mPurpleCoinNum == PURPLE_COIN_POWER_STAR_APPEAR_NUM) {
        MR::requestAppearPowerStarCoin100();
    }
}

void ScenePlayingResult::incStarPiece(int num) {
    s32 prevNum = GameDataFunction::getStarPieceNum();
    GameDataFunction::addStarPiece(num);
    s32 currNum = GameDataFunction::getStarPieceNum();
    s32 last1upNum = GameDataFunction::getLast1upStarPieceNum();

    if (MR::isStageAstroLocation()) {
        return;
    }

    if (last1upNum + STAR_PIECE_STEP_NUM > currNum) {
        return;
    }

    if (prevNum / STAR_PIECE_STEP_NUM < currNum / STAR_PIECE_STEP_NUM) {
        MR::incPlayerLeft();
        MR::requestOneUp();
    }

    GameDataFunction::setLast1upStarPieceNum(last1upNum + STAR_PIECE_STEP_NUM);
}

s32 ScenePlayingResult::getStarPieceNum() const {
    return GameDataFunction::getStarPieceNum();
}

ScenePlayingResult::~ScenePlayingResult() {
    
}

ScenePlayingResult::ScenePlayingResult() :
    NameObj("プレイ結果保持"),
    mCoinNum(0),
    mPurpleCoinNum(0)
{
    
}
