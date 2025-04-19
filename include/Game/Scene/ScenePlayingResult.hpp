#pragma once

#include "Game/NameObj/NameObj.hpp"

class ScenePlayingResult : public NameObj {
public:
    ScenePlayingResult();

    virtual ~ScenePlayingResult();

    void incCoin(int);
    s32 getCoinNum() const;
    void incPurpleCoin();
    void incStarPiece(int);
    s32 getStarPieceNum() const;
    
    s32 mCoinNum;               // 0x0C
    s32 mPurpleCoinNum;         // 0x10
};
