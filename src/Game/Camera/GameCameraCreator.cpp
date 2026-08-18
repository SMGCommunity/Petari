#include "Game/Camera/GameCameraCreator.hpp"
#include "Game/AreaObj/CubeCamera.hpp"
#include "Game/Camera/CameraDirector.hpp"
#include "Game/Camera/CameraParamChunkHolder.hpp"
#include "Game/Camera/CameraParamChunkID.hpp"
#include "Game/Util/AreaObjUtil.hpp"
#include "Game/Util/JMapIdInfo.hpp"
#include "Game/Util/SceneUtil.hpp"
#include <cstring>
#include <mem.h>

GameCameraCreator::GameCameraCreator(CameraParamChunkHolder* pChunkHolder) {
    mChunkHolder = pChunkHolder;
    mName = nullptr;
    _8 = -1;

    memset(&mCodes[0], 0, sizeof(mCodes));
}

void GameCameraCreator::initCameraCodeCollection(const char* pName, s32 a2) {
    mName = pName;
    _8 = a2;
}

void GameCameraCreator::registerCameraCode(u32 code) {
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
            chunk.createGroupID(_8, mName, i, 0);

            mChunkHolder->createChunk(chunk, nullptr);
        }
    }

    mName = nullptr;
    _8 = -1;
}

void GameCameraCreator::scanStartPos() {
    for (s32 idx = 0; idx < MR::getStartPosNum(); idx++) {
        // TODO: ctor
        JMapIdInfo info;
        info._0 = -1;
        info.mZoneID = -1;

        MR::getStartCameraIdInfoFromStartDataIndex(&info, idx);

        CameraParamChunkID_Tmp chunk = CameraParamChunkID_Tmp();
        chunk.createStartID(info.mZoneID, info._0);

        mChunkHolder->createChunk(chunk, nullptr);
    }
}

void GameCameraCreator::scanArea() {
    // FIXME: regswap
    // https://decomp.me/scratch/vU2zD

    CubeCameraMgr* mgr = static_cast< CubeCameraMgr* >(MR::getAreaObjManager("CubeCamera"));
    mgr->initAfterLoad();

    for (s32 idx = 0; idx < mgr->getNumAreaObj(); idx++) {
        CubeCameraArea* area = static_cast< CubeCameraArea* >(mgr->getAreaObj(idx));
        if (area->isValidCameraID()) {
            CameraParamChunkID_Tmp chunk = CameraParamChunkID_Tmp();
            chunk.createCubeID(area->getZoneID(), area->getCameraID());

            mChunkHolder->createChunk(chunk, nullptr);
        }
    }
}
