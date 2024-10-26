#include "Game/Camera/CameraParamString.hpp"
#include "revolution.h"

CameraParamString::CameraParamString() {
    mString = nullptr;
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
    if (pStringOther != nullptr && *pStringOther != 0) {
        mString = pStringOther;
    }
    else {
        mString = nullptr;
    }
}