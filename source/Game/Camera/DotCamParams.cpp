#include "Game/Camera/DotCamParams.hpp"
#include <cstdio>
#include <cstring>

DotCamReader::~DotCamReader() {

}

DotCamReaderInBin::DotCamReaderInBin(const void *pData) :
    mVersion(0), _8(nullptr), mMapInfo() {
    mMapIter.mInfo = nullptr;
    mMapIter._4 = -1;
    init(pData);
}

DotCamReaderInBin::~DotCamReaderInBin() {
    if (_8 != nullptr) {
        delete[] _8;
    }
}

u32 DotCamReaderInBin::getVersion() const {
    return mVersion;
}

#ifdef NON_MATCHING
// Stack issues
bool DotCamReaderInBin::hasMoreChunk() const {
    bool hasMore = false;

    if (mMapIter.isValid()) {
        const JMapData *mapData = mMapInfo.mData;
        s32 iVar2 = mapData != nullptr ? mapData->_0 : 0; 

        bool bVar1 = false;

        const JMapInfo &mapInfo = mMapInfo;

        if (mMapIter._4 == iVar2 && mMapIter.mInfo != nullptr && mapInfo.mData != nullptr) {
            if (mMapIter.mInfo->mData == mapInfo.mData) {
                bVar1 = true;
            }
        } 

        if (!bVar1) {
            hasMore = true;
        }
    }

    return hasMore;
}
#endif

void DotCamReaderInBin::nextToChunk() {
    if (mMapIter.isValid()) {
        mMapIter._4++;
    }
}

bool DotCamReaderInBin::getValueInt(const char *pName, long *pOut) {
    return mMapIter.getValue<long>(pName, pOut);
}

bool DotCamReaderInBin::getValueFloat(const char *pName, float *pOut) {
    return mMapIter.getValue<float>(pName, pOut);
}

bool DotCamReaderInBin::getValueVec(const char *pName, TVec3f *pOut) {
    char buffer[0x100];
    char *pBuffer = &buffer[0];

    f32 z;
    f32 y;
    f32 x;

    snprintf(pBuffer, sizeof(buffer), "%s.X", pName);
    bool success = mMapIter.getValue<float>(pBuffer, &x) & 1;

    snprintf(pBuffer, sizeof(buffer), "%s.Y", pName);
    success &= mMapIter.getValue<float>(pBuffer, &y);

    snprintf(pBuffer, sizeof(buffer), "%s.Z", pName);
    success &= mMapIter.getValue<float>(pBuffer, &z);

    if (success) {
        pOut->x = x;
        pOut->y = y;
        pOut->z = z;
    }

    return success;
}

bool DotCamReaderInBin::getValueString(const char *pName, const char **pOut) {
    s32 iVar3 = mMapIter._4;
    JMapInfo *info = mMapIter.mInfo;
    s32 index = info->searchItemInfo(pName);

    if (index < 0) {
        return false;
    }

    return info->getValueFast(iVar3, index, pOut);
}

#ifdef NON_MATCHING
// Stack issues
void DotCamReaderInBin::init(const void *pData) {
    mMapInfo.attach(pData);
    s32 index = mMapInfo.searchItemInfo("version");  

    bool valid = index >= 0;

    if (valid) {
        mMapInfo.getValueFast(0, index, &mVersion);
    }

    mMapIter.mInfo = &mMapInfo;
    mMapIter._4 = 0;
}
#endif
