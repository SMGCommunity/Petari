#include "Game/System/GameDataPlayerStatus.hpp"
#include "Game/Util/MathUtil.hpp"
#include <JSystem/JSupport/JSUMemoryInputStream.hpp>
#include <JSystem/JSupport/JSUMemoryOutputStream.hpp>

GameDataPlayerStatus::GameDataPlayerStatus() {
    initializeData();
}

u32 GameDataPlayerStatus::makeHeaderHashCode() const {
    return 0x27C90F;
}

u32 GameDataPlayerStatus::getSignature() const {
    return 'PLAY';
}

s32 GameDataPlayerStatus::serialize(u8* pBuffer, u32 size) const {
    JSUMemoryOutputStream stream = JSUMemoryOutputStream(pBuffer, size);

    u8 storyProgress = mStoryProgress;
    stream.write(&storyProgress, sizeof(mStoryProgress));

    u32 stockedStarPiece = mStockedStarPiece;
    stream.write(&stockedStarPiece, sizeof(mStockedStarPiece));

    u16 playerLeft = mPlayerLeft;
    stream.write(&playerLeft, sizeof(mPlayerLeft));

    return stream.mPosition;
}

s32 GameDataPlayerStatus::deserialize(const u8* pBuffer, u32 size) {
    initializeData();

    JSUMemoryInputStream stream = JSUMemoryInputStream(pBuffer, size);

    stream.read(&mStoryProgress, sizeof(mStoryProgress));
    stream.read(&mStockedStarPiece, sizeof(mStockedStarPiece));

    if (stream.getAvailable() != 0) {
        stream.read(&mPlayerLeftSupply, sizeof(mPlayerLeftSupply));
    }

    mPlayerLeft = 4;

    return 0;
}

void GameDataPlayerStatus::initializeData() {
    mPlayerLeft = 4;
    mPlayerLeftSupply = 4;
    mStockedStarPiece = 0;
    mStoryProgress = 0;
}

s32 GameDataPlayerStatus::getPlayerLeft() const {
    s32 playerLeft = mPlayerLeft;

    return MR::clamp(playerLeft, 0, 99);
}

void GameDataPlayerStatus::addPlayerLeft(int num) {
    s32 playerLeft = mPlayerLeft + num;

    mPlayerLeft = MR::clamp(playerLeft, 0, 99);
}

bool GameDataPlayerStatus::isPlayerLeftSupply() const {
    return mPlayerLeftSupply >= 10;
}

void GameDataPlayerStatus::offPlayerLeftSupply() {
    mPlayerLeftSupply = 0;
}

void GameDataPlayerStatus::addStockedStarPiece(int num) {
    s32 stockedStarPiece = mStockedStarPiece + num;

    mStockedStarPiece = MR::clamp(stockedStarPiece, 0, 9999);
}
