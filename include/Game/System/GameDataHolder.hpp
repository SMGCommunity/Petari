#pragma once

class GameDataHolder {
public:
    u16 getGameEventValue(const char *) const;
    s32 calcCurrentPowerStarNum() const;
    bool hasPowerStar(const char *, s32) const;
    s32 getStarPieceNumGivingToTicoSeed(int) const;
    bool isCompleteMarioAndLuigi() const;
    bool isPassedStoryEvent(const char *) const;
};
