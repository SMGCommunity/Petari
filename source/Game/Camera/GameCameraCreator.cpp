#include "Game/Camera/CameraDirector.h"
#include "Game/Camera/CameraParamChunkHolder.h"
#include "Game/Camera/GameCameraCreator.h"
#include "Game/Util/AreaObjUtil.h"
#include <cstring>

GameCameraCreator::GameCameraCreator(CameraParamChunkHolder *pChunkHolder) {
    mChunkHolder = pChunkHolder;
    _4 = nullptr;
    _8 = -1;

    memset(&mCodes[0], 0, sizeof(mCodes));
}

void GameCameraCreator::initCameraCodeCollection(const char *a1, long a2) {
    _4 = a1;
    _8 = a2;
}

void GameCameraCreator::registerCameraCode(unsigned long code) {
    if (code >= 0xFF) {
        return;
    }

    mCodes[code] = 1;
}

void GameCameraCreator::termCameraCodeCollection() {
    for (u32 i = 0; i < 0xFF; i++) {
        if (mCodes[i] != 0) {
            mCodes[i] = 0;

            CameraParamChunkID_Tmp chunk = CameraParamChunkID_Tmp();
            chunk.createGroupID(_8, _4, i, 0);

            mChunkHolder->createChunk(chunk, nullptr);
        }
    }

    _4 = nullptr;
    _8 = -1;
}