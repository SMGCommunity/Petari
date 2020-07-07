#include "Camera/CameraParamString.h"
#include "smg.h"

#include <revolution.h>

CameraParamString::CameraParamString()
{
    mCharPtr = nullptr;
}

CameraParamString* CameraParamString::operator=(const CameraParamString &otherParam)
{
    copy(otherParam.mCharPtr);
    return this;
}

void CameraParamString::setCharPtr(const char *pCharPtr)
{
    copy(pCharPtr);
}

const char* CameraParamString::getCharPtr() const
{
    return mCharPtr;
}

void CameraParamString::copy(const char *pCharPtr)
{
    if (pCharPtr != nullptr && (s32)*pCharPtr != nullptr)
    {
        mCharPtr = pCharPtr;
    }
    else
    {
        mCharPtr = nullptr;
    }
}