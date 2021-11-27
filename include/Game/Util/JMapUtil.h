#pragma once

#include "Game/Util/JMapInfoIter.h"
#include "JSystem/JGeometry/TVec.h"
#include <revolution.h>

namespace MR {
    bool getJMapInfoLinkID(const JMapInfoIter &, s32 *);

    bool getJMapInfoTrans(const JMapInfoIter &, TVec3f *);
    bool getJMapInfoRotate(const JMapInfoIter &, TVec3f *);
    bool getJMapInfoScale(const JMapInfoIter &, TVec3f *);

    bool getJMapInfoArg0WithInit(const JMapInfoIter &, s32 *);
    bool getJMapInfoArg1WithInit(const JMapInfoIter &, s32 *);
    bool getJMapInfoArg2WithInit(const JMapInfoIter &, s32 *);
    bool getJMapInfoArg3WithInit(const JMapInfoIter &, s32 *);
    bool getJMapInfoArg4WithInit(const JMapInfoIter &, s32 *);
    bool getJMapInfoArg5WithInit(const JMapInfoIter &, s32 *);
    bool getJMapInfoArg6WithInit(const JMapInfoIter &, s32 *);
    bool getJMapInfoArg7WithInit(const JMapInfoIter &, s32 *);

    bool getObjectName(const char **, const JMapInfoIter &);

    const char* getDemoName(const JMapInfoIter &);

    s32 getDemoGroupID(const JMapInfoIter &);
    s32 getDemoGroupLinkID(const JMapInfoIter &);
};