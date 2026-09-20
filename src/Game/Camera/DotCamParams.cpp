#include "Game/Camera/DotCamParams.hpp"
#include "Game/Util/JMapInfo.hpp"
#include <cstdio>
#include <cstring>

template bool JMapInfoIter::getValue< s32 >(const char*, s32*) const;

DotCamReaderInBin::DotCamReaderInBin(const void* pData) : mVersion(), _8(), mMapInfo() {
    init(pData);
}

DotCamReaderInBin::~DotCamReaderInBin() {
    if (_8 != nullptr) {
        delete[] _8;
    }
}

bool DotCamReaderInBin::hasMoreChunk() const {
    return mMapIter.isValid() && mMapIter != mMapInfo.end();
}

void DotCamReaderInBin::nextToChunk() {
    if (mMapIter.isValid()) {
        mMapIter.mIndex++;
    }
}

bool DotCamReaderInBin::getValueInt(const char* pName, s32* pOut) {
    return mMapIter.getValue< s32 >(pName, pOut);
}

bool DotCamReaderInBin::getValueFloat(const char* pName, f32* pOut) {
    return mMapIter.getValue< f32 >(pName, pOut);
}

bool DotCamReaderInBin::getValueString(const char* pName, const char** pOut) {
    return mMapIter.getValue(pName, pOut);
}

DotCamReader::~DotCamReader() {
}

u32 DotCamReaderInBin::getVersion() const {
    return mVersion;
}

void DotCamReaderInBin::init(const void* pData) {
    mMapInfo.attach(pData);
    mMapInfo.begin().getValue("version", &mVersion);

    mMapIter = mMapInfo.begin();
}

bool DotCamReaderInBin::getValueVec(const char* pName, TVec3f* pOut) {
    char buffer[0x100];
    char* pBuffer = &buffer[0];

    f32 z;
    f32 y;
    f32 x;

    snprintf(pBuffer, sizeof(buffer), "%s.X", pName);
    bool success = mMapIter.getValue< f32 >(pBuffer, &x) & 1;

    snprintf(pBuffer, sizeof(buffer), "%s.Y", pName);
    success &= mMapIter.getValue< f32 >(pBuffer, &y);

    snprintf(pBuffer, sizeof(buffer), "%s.Z", pName);
    success &= mMapIter.getValue< f32 >(pBuffer, &z);

    if (success) {
        pOut->x = x;
        pOut->y = y;
        pOut->z = z;
    }

    return success;
}
