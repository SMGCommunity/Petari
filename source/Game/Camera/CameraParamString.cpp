#include "Game/Camera/CameraParamString.h"
#include "revolution.h"

CameraParamString::CameraParamString() {
    mString = NULL;
}

CameraParamString &CameraParamString::operator=(const CameraParamString &rOther) {
    copy(rOther.mString);

    return *this;
}

void CameraParamString::setCharPtr(const char *pString) {
    copy(pString);
}

const char *CameraParamString::getCharPtr() const {
    return mString;
}

void CameraParamString::copy(const char *pStringOther) {
    if (pStringOther != NULL && *pStringOther != 0) {
        mString = pStringOther;
    }
    else {
        mString = NULL;
    }
}