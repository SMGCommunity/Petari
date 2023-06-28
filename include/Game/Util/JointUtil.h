#pragma once

#include <revolution.h>
#include "JSystem/JGeometry/TVec.h"

class LiveActor;

namespace MR {
    MtxPtr getJointMtx(const LiveActor *, int);
    MtxPtr getJointMtx(const LiveActor *, const char *);

    void copyJointPos(const LiveActor *, int, TVec3f *);
    void copyJointPos(const LiveActor *, const char *, TVec3f *);

    bool isExistJoint(const LiveActor *, const char *);

    unsigned long getJointNum(const LiveActor *);
};