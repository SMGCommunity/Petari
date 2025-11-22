#pragma once

#include "Game/NameObj/NameObj.hpp"
#include "JSystem/JGeometry/TVec.hpp"

class JMapLinkInfo;

/// @brief Class that contains the NamePos attributes. NamePos defines the spawn location of other actors (ie Power Stars)
class NamePosInfo {
public:
    NamePosInfo();

    /* 0x0 */ const char* mName;         ///< The name of the NamePos.
    /* 0x4 */ TVec3f mPosition;          ///< The position of the NamePos.
    /* 0x10 */ TVec3f mRotation;         ///< The rotation of the NamePos.
    /* 0x1C */ JMapLinkInfo* mLinkInfo;  ///< The information to link this NamePos to other objects.
    u32 _20;
};

/// @brief Class that contains the array of NamePosInfo instances.
class NamePosHolder : public NameObj {
public:
    NamePosHolder();

    virtual ~NamePosHolder();

    bool tryRegisterLinkObj(const NameObj*, const JMapInfoIter&);

    bool find(const NameObj*, const char*, TVec3f*, const TVec3f*) const;

    /* 0xC */ s32 mPosNum;           ///< The number of NamePosInfo instances.
    /* 0x10 */ NamePosInfo* mInfos;  ///< The array of NamePosInfo instances.
};

namespace MR {
    NamePosHolder* getNamePosHolder();
};
