#pragma once

#include <revolution.h>
#include "JSystem/JGeometry/TVec.hpp"

class LiveActor;
class J3DModel;

namespace MR {
    MtxPtr getJointMtx(const LiveActor *, int);
    MtxPtr getJointMtx(const LiveActor *, const char *);

    void copyJointPos(const LiveActor *, int, TVec3f *);
    void copyJointPos(const LiveActor *, const char *, TVec3f *);

    void showJoint(J3DModel *, const char *);

    void copyJointScale(const LiveActor *, const char *, TVec3f *);
    void hideJoint(J3DModel *, const char *);

    bool isExistJoint(const LiveActor *, const char *);

    u32 getJointNum(const LiveActor *);

    u16 getJointIndex(const LiveActor *, const char *) NO_INLINE;
    
};
