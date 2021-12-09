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

#ifdef NON_MATCHING
// Float operation order, does not use fmadds, fmsubs and fnmsubs
float KeyCamAnmDataAccessor::calcHermite(float a1, float a2, float a3, float a4, float a5, float a6, float a7) const {
    float fVar1 = a4 / 30.0f;
    float fVar2 = (a1 - a2) / (a5 - a2);
    float fVar3 = fVar2 * fVar2 - fVar2;

    return -(a1 - a2) * (fVar2 * fVar1 - a7 / 30.0f * fVar3 + fVar1 * fVar3 + fVar1) - (((fVar2 + fVar2) * fVar3 - fVar2 * fVar2) * (a3 - a6) + a3);
}
#endif

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
    mNrFrames = 0;
    mIsKey = 0;
    mSpeed = 1.0f;
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

void CameraAnim::setParam(unsigned char *pFile, float speed) {
    loadBin(pFile);
    mFileData = pFile;
    mSpeed = speed;
}

bool CameraAnim::isAnimEnd() const {
    bool hasEnded = true;
    u32 nrFrames = mNrFrames;

    if (nrFrames != 0) {
        if (!(mCurrentFrame >= nrFrames)) {
            hasEnded = false;
        }
    }

    return hasEnded;
}

u32 CameraAnim::getAnimFrame(unsigned char *pFile) {
    if (pFile == NULL) {
        return 0;
    }

    CanmFileHeader *pHeader = reinterpret_cast<CanmFileHeader *>(pFile);

    if (pHeader->mMagic[0] != 'A' || pHeader->mMagic[1] != 'N' || pHeader->mMagic[2] != 'D' || pHeader->mMagic[3] != 'O') {
        return 0;
    }

    if ((pHeader->mType[0] != 'C' || pHeader->mType[1] != 'A' || pHeader->mType[2] != 'N' || pHeader->mType[3] != 'M') &&
        (pHeader->mType[0] != 'C' || pHeader->mType[1] != 'K' || pHeader->mType[2] != 'A' || pHeader->mType[3] != 'N')) {
        return 0;
    }
    
    if (pHeader->_08 == 0) {
        return 0;
    }

    return pHeader->mNrFrames;
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
    mNrFrames = pHeader->mNrFrames;

    s32 someOffset = pHeader->_1C;

    _70 = *(reinterpret_cast<u32 *>(&pEntry[someOffset])) / 4;
    
    mFileDataAccessor->setParam(pEntry, pEntry + someOffset + 1);

    return true;
}