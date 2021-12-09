#include "Game/Camera/CameraAnim.h"

inline BaseCamAnmDataAccessor::BaseCamAnmDataAccessor() {
    
}

inline BaseCamAnmDataAccessor::~BaseCamAnmDataAccessor() {

}

inline KeyCamAnmDataAccessor::KeyCamAnmDataAccessor() {
    _04 = 0;
    _08 = 0;
}

KeyCamAnmDataAccessor::~KeyCamAnmDataAccessor() {

}

void KeyCamAnmDataAccessor::setParam(u8 *a1, u8 *a2) {
    _04 = a1;
    _08 = a2;
}

inline CamAnmDataAccessor::CamAnmDataAccessor() {
    _04 = 0;
    _08 = 0;
}

CamAnmDataAccessor::~CamAnmDataAccessor() {

}

void CamAnmDataAccessor::setParam(u8 *a1, u8 *a2) {
    _04 = a1;
    _08 = a2;
}

CameraAnim::CameraAnim(const char *pName) : Camera(pName) {
    _4C = 0;
    _50 = 1;
    _54 = 0;
    mIsKey = 0;
    _5C = 1.0f;
    mFileDataAccessor = NULL;
    mDataAccessor = new CamAnmDataAccessor();
    mKeyDataAccessor = new KeyCamAnmDataAccessor();
    _70 = 0;
    _74 = 0;
    mFileData = NULL;
    _7C = 0;
}

CameraAnim::~CameraAnim() {

}

bool CameraAnim::loadBin(unsigned char *pFile) {
    CanmFileHeader *pHeader = reinterpret_cast<CanmFileHeader *>(pFile);

    if (pHeader->mMagic[0] != 'A' || pHeader->mMagic[1] != 'N' || pHeader->mMagic[2] != 'D' || pHeader->mMagic[3] != 'O') {
        return false;
    }

    if (pHeader->mType[0] == 'C' && pHeader->mType[1] == 'A' && pHeader->mType[2] == 'N' && pHeader->mType[3] == 'M') {
        mIsKey = 0;
        mFileDataAccessor = mDataAccessor;
    }
    else if (pHeader->mType[0] == 'C' && pHeader->mType[1] == 'K' && pHeader->mType[2] == 'A' && pHeader->mType[3] == 'N') {
        mIsKey = 1;
        mFileDataAccessor = mKeyDataAccessor;
    }
    else {
        return false;
    }

    if (pHeader->_08 == 0) {
        return false;
    }

    u8 *pEntry = reinterpret_cast<u8 *>(pFile + sizeof(CanmFileHeader));
    _4C = pHeader->_0C;
    _50 = pHeader->_10;
    _54 = pHeader->_18;

    s32 vOffset = pHeader->_1C;

    _70 = *(reinterpret_cast<u32 *>(&pntry[vOffset])) / 4;
E
    mFileDataAccessor->setParam(pEntry, pEntry + vOffset + 1);

    return true;
}