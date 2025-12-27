#include "Game/MapObj/TreasureBoxCracked.hpp"

namespace {
    static Color8 sPointLightColor(0xFF, 0xFF, 0x96, 0xFF);
    const Vec sTopOpenStartOffset = {0.0f, -100.0f, -250.0f};
    const Vec sTopOpenEndOffset = {0.0f, 23.0f, -250.0f};
    const Vec sCoinOffset = {0.0f, 130.0f, 0.0f};
    const Vec sChipOffset = {0.0f, 170.0f, 0.0f};
    const Vec sKinokoOneUpOffset = {0.0f, 130.0f, 0.0f};
    const Vec sKinokoLifeUpOffset = {0.0f, 130.0f, 0.0f};
    const Vec sAirBubbleOffset = {0.0f, 220.0f, 0.0f};
};  // namespace

TreasureBoxCracked::TreasureBoxCracked(const char* pName) : LiveActor(pName) {}
