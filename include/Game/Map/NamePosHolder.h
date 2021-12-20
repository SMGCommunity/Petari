#pragma once

#include "Game/NameObj/NameObj.h"
#include "JSystem/JGeometry/TVec.h"

class JMapLinkInfo;

class NamePosInfo {
public:
    NamePosInfo();

    const char* mName;          // _0
    TVec3f mPosition;           // _4
    TVec3f mRotation;           // _10
    JMapLinkInfo* mLinkInfo;    // _1C
    u32 _20;
};

class NamePosHolder : public NameObj { 
public:
    NamePosHolder();

    virtual ~NamePosHolder();

    s32 mPosNum;            // _C
    NamePosInfo* mInfos;    // _10
};

namespace MR {
    NamePosHolder* getNamePosHolder();
};