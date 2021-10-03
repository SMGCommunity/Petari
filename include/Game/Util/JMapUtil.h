#pragma once

#include "Game/Util/JMapInfoIter.h"
#include "JSystem/JGeometry/TVec.h"
#include <revolution.h>

namespace MR {
    bool getJMapInfoLinkID(const JMapInfoIter &, s32 *);

    bool getJMapInfoTrans(const JMapInfoIter &, TVec3f *);
    bool getJMapInfoRotate(const JMapInfoIter &, TVec3f *);
    bool getJMapInfoScale(const JMapInfoIter &, TVec3f *);
};