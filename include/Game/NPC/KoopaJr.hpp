#pragma once

#include "Game/NPC/NPCActor.hpp"

class KoopaJr : public NPCActor {
public:
    /// @brief Creates a new `KoopaJr`.
    /// @param pName A pointer to the null-terminated name of the object.
    KoopaJr(const char* pName);

    /* 0x0C */ virtual void init(const JMapInfoIter& rIter);
    /* 0x2C */ virtual void kill();
    /* 0x48 */ virtual void control();
    /* 0x4C */ virtual void calcAndSetBaseMtx();

    void setStateShipBattleAppear();
    void setStateShipBattlePowerUp();
    void setStateShipBattleEscape();
    void setStateShipBattleShipDamage();
    void endShipBattleTalk();
    void startShipBattleTalk();

    void exeWait();
    void exeReaction();
    void exeReactionEnd();
    void exeShipBattleAppear();
    void exeShipBattlePowerUp();
    void exeShipBattleEscape();
    void exeShipBattleShipDamage();
    void exeShipBattleDemoTalkStart();
    void exeShipBattleDemoTalkWait();

private:
    /* 0x15C */ bool mIsInvalidAppearStarPiece;
    /* 0x15D */ bool mIsShipBattleTalk;
};
