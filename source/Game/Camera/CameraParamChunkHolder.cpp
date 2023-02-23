#include "Game/Camera/CameraHolder.h"
#include "Game/Camera/CameraLocalUtil.h"
#include "Game/Camera/CameraParamChunk.h"
#include "Game/Camera/CameraParamChunkHolder.h"
#include "Game/Camera/CameraParamChunkID.h"
#include "Game/Camera/DotCamParams.h"
#include "Game/Util/SceneUtil.h"
#include "JSystem/JKernel/JKRHeap.h"
#include <cstring>

#define CHUNK_CAPACITY 0x400

CameraParamChunkHolder::CameraParamChunkHolder(CameraHolder *pCameraHolder, const char *pName) : NameObj(pName) {
    mCameraHolder = pCameraHolder;
    mChunkCapacity = CHUNK_CAPACITY;
    mNrChunks = 0;
    mChunks = new CameraParamChunk *[CHUNK_CAPACITY];
    mIsSorted = 0;
}

CameraParamChunkHolder::~CameraParamChunkHolder() {

}

CameraParamChunk *CameraParamChunkHolder::createChunk(const CameraParamChunkID &rID, JKRHeap *pHeap) {
    if (isNewAttribute(rID)) {
        char firstChar = rID.mName[0];
        CameraParamChunk *chunk;

        if (firstChar == 'e') {
            chunk = new(pHeap, 0) CameraParamChunkEvent(mCameraHolder, rID);
        }
        else if (firstChar == 'g' || firstChar == 'c' || firstChar == 's') {
            chunk = new(pHeap, 0) CameraParamChunkGame(mCameraHolder, rID);
        }
        else {
            chunk = new(pHeap, 0) CameraParamChunk(mCameraHolder, rID);
        }

        mChunks[mNrChunks++] = chunk;
        
        return chunk;
    }
    
    return nullptr;
}

CameraParamChunk *CameraParamChunkHolder::getChunk(const CameraParamChunkID &rID) {
    if (rID.mName == nullptr) {
        return nullptr;
    }

    return findChunk(rID);
}

void CameraParamChunkHolder::sort() {
    // Selection sort
    for (s32 i = 0; i < mNrChunks - 1; i++) {
        s32 minIndex = i;

        for (s32 j = i + 1; j < mNrChunks; j++) {
            if (*mChunks[j]->mParamChunkID > *mChunks[minIndex]->mParamChunkID) {
                minIndex = j;
            }
        }

        if (minIndex != i) {
            CameraParamChunk *temp = mChunks[i];
            mChunks[i] = mChunks[minIndex];
            mChunks[minIndex] = temp;
        }
    }

    mIsSorted = 1;
}

void CameraParamChunkHolder::loadCameraParameters() {
    for (s32 i = 0; i < MR::getZoneNum(); i++) {
        loadFile(i);
    }
}

bool CameraParamChunkHolder::isNewAttribute(const CameraParamChunkID &rID) {
    for (s32 i = 0; i < mNrChunks; i++) {
        if (*mChunks[i]->mParamChunkID == rID) {
            return false;
        }
    }

    return true;
}

CameraParamChunk *CameraParamChunkHolder::findChunk(const CameraParamChunkID &rID) {
    if (mIsSorted) {
        s32 low = 0;
        s32 high = mNrChunks - 1;

        while (low < high) {
            s32 middle = (low + high) / 2;

            if (*mChunks[middle]->mParamChunkID > rID) {
                low = middle + 1;
            }
            else {
                high = middle;
            }
        }

        if (*mChunks[low]->mParamChunkID == rID) {
            return mChunks[low];
        }
    }
    else {
        for (s32 i = 0; i < mNrChunks; i++) {
            if (*mChunks[i]->mParamChunkID == rID) {
                return mChunks[i];
            }
        }
    }

    return nullptr;
}

CameraParamChunk *CameraParamChunkHolder::findChunk(long zoneID, const char *pName) {
    for (s32 i = 0; i < mNrChunks; i++) {
        if (mChunks[i]->mParamChunkID->equals(zoneID, pName)) {
            return mChunks[i];
        }
    }

    return nullptr;
}

#ifdef NON_MATCHING
// Stack is 0x10 bytes smaller
void CameraParamChunkHolder::loadFile(long zoneID) {
    void *data;
    long local44;
    MR::getStageCameraData(&data, &local44, zoneID);

    if (local44 != 0) {
        DotCamReaderInBin reader = DotCamReaderInBin(data);
        mCameraVersion = reader.mVersion;

        while (reader.hasMoreChunk()) {
            const char *id = nullptr;
            reader.getValueString("id", &id);

            CameraParamChunk *chunk = findChunk(zoneID, id);

            if (chunk != nullptr && chunk->_64 != 0) {
                reader.nextToChunk();
            }
            else {
                if (chunk != nullptr) {
                    chunk->load(&reader, mCameraHolder);
                    arrangeChunk(chunk);
                }
                
                reader.nextToChunk();
            }
        }
    }
}
#endif

void CameraParamChunkHolder::arrangeChunk(CameraParamChunk *pChunk) {
    s32 index = pChunk->mCameraTypeIndex;

    if (mCameraHolder->getIndexOf("CAM_TYPE_FOLLOW") == index) {
        if (mCameraVersion < 0x30001) {
            CameraGeneralParam *param = pChunk->mGeneralParam;

            param->mAngleA = 0.17453294f; // 10 degrees
            param->mAngleB = 0.34906587f; // 20 degrees
        }

        if (mCameraVersion < 0x30002) {
            pChunk->mGeneralParam->mDist = 0.15f;
        }
    }

    if (mCameraVersion < 0x30003) {
        const char *className = pChunk->getClassName();

        if (strcmp(className, "Game") == 0) {
            CameraParamChunkGame *pGameChunk = reinterpret_cast<CameraParamChunkGame *>(pChunk);
            pGameChunk->mThru = 0;
        }
    }

    index = pChunk->mCameraTypeIndex;

    if (mCameraHolder->getIndexOf("CAM_TYPE_RAIL_FOLLOW") == index) {
        if (mCameraVersion < 0x30007) {
            CameraGeneralParam *param = pChunk->mGeneralParam;

            param->mWPoint.x = 30.0f;
            param->mWPoint.y = 0.35f;
            param->mWPoint.z = 0.0f;
        }
    }

    if (mCameraVersion < 0x30008) {
        TVec3f temp;
        temp.x = 0.0f;
        temp.y = 1.0f;
        temp.z = 0.0f;

        pChunk->mExParam.mVPanAxis.x = temp.x;
        pChunk->mExParam.mVPanAxis.y = temp.y;
        pChunk->mExParam.mVPanAxis.z = temp.z;
    }

    if (mCameraVersion < 0x3000A) {
        index = pChunk->mCameraTypeIndex;

        if (mCameraHolder->getIndexOf("CAM_TYPE_CUBE_PLANET") == index) {
            pChunk->mGeneralParam->mAngleB = 0.35f;
        }
    }

    if (mCameraVersion < 0x3000B) {
        index = pChunk->mCameraTypeIndex;

        if (mCameraHolder->getIndexOf("CAM_TYPE_WATER_FOLLOW") == index) {
            pChunk->mGeneralParam->mDist = 0.01f;
        }
    }

    if (mCameraVersion < 0x3000C) {
        index = pChunk->mCameraTypeIndex;

        if (mCameraHolder->getIndexOf("CAM_TYPE_CHARMED_VECREG") == index) {
            CameraGeneralParam *param = pChunk->mGeneralParam;
            param->mString.setCharPtr(CameraLocalUtil::getDummyVecRegName());
        }
    }

    if (mCameraVersion < 0x3000D) {
        index = pChunk->mCameraTypeIndex;

        if (mCameraHolder->getIndexOf("CAM_TYPE_FOLLOW") == index) {
            pChunk->mGeneralParam->mNum1 = 1;
        }
    }

    if (mCameraVersion < 0x3000E) {
        index = pChunk->mCameraTypeIndex;

        if (mCameraHolder->getIndexOf("CAM_TYPE_CHARMED_VECREG") == index) {
            pChunk->mGeneralParam->mAngleA = 0.5f;
        }
    }

    if (mCameraVersion < 0x3000F) {
        index = pChunk->mCameraTypeIndex;

        if (mCameraHolder->getIndexOf("CAM_TYPE_MEDIAN_TOWER") == index) {
            pChunk->mGeneralParam->mUp.zero();
        }
    }

    if (mCameraVersion < 0x30010) {
        index = pChunk->mCameraTypeIndex;

        if (mCameraHolder->getIndexOf("CAM_TYPE_MEDIAN_TOWER") == index) {
            pChunk->mGeneralParam->mUp.y = 0.5f;
        }
    }

    if (mCameraVersion < 0x30011) {
        index = pChunk->mCameraTypeIndex;

        if (mCameraHolder->getIndexOf("CAM_TYPE_CHARMED_VECREG") == index) {
            pChunk->mGeneralParam->mAngleB = 0.02f;
        }
    }

    if (mCameraVersion < 0x30012) {
        index = pChunk->mCameraTypeIndex;

        if (mCameraHolder->getIndexOf("CAM_TYPE_MEDIAN_TOWER") == index) {
            pChunk->mGeneralParam->mUp.z = 0.992f;
        }
    }

    if (mCameraVersion < 0x30013) {
        index = pChunk->mCameraTypeIndex;

        if (mCameraHolder->getIndexOf("CAM_TYPE_EYEPOS_FIX_THERE") == index) {
            pChunk->mExParam.mRoll = 0.0f;
        }
    }

    if (mCameraVersion < 0x30014) {
        index = pChunk->mCameraTypeIndex;

        if (mCameraHolder->getIndexOf("CAM_TYPE_MEDIAN_PLANET") == index) {
            pChunk->mGeneralParam->mWPoint.x = 0.5f;
        }
    }

    if (mCameraVersion < 0x30015) {
        index = pChunk->mCameraTypeIndex;

        if (mCameraHolder->getIndexOf("CAM_TYPE_DPD") == index) {
            pChunk->mGeneralParam->mUp.zero();
            pChunk->mGeneralParam->mNum2 = 0;
        }
    }

    if (mCameraVersion < 0x30016) {
        index = pChunk->mCameraTypeIndex;

        if (mCameraHolder->getIndexOf("CAM_TYPE_XZ_PARA") == index) {
            pChunk->mGeneralParam->mNum1 = 0;
        }
    }
}