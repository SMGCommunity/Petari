#pragma once

#include <JSystem/JGeometry/TVec.hpp>
#include <revolution/types.h>

class J3DJoint;
class J3DModel;
class J3DModelData;
class LiveActor;

namespace MR {
    J3DJoint* getJoint(J3DModel*, const char*);
    J3DJoint* getJoint(const LiveActor*, const char*);
    J3DJoint* getJoint(const LiveActor*, u16);
    MtxPtr getJointMtx(J3DModel*, const char*);
    MtxPtr getJointMtx(const LiveActor*, const char*);
    MtxPtr getJointMtx(const LiveActor*, int);
    s32 getJointIndex(const LiveActor*, const char*);
    const char* getJointName(const LiveActor*, int);
    s32 getJointNum(const LiveActor*);
    bool isExistJoint(const LiveActor*, const char*);
    void copyJointPos(J3DModel*, const char*, TVec3f*);
    void copyJointPos(const LiveActor*, const char*, TVec3f*);
    void copyJointPos(J3DModel*, int, TVec3f*);
    void copyJointPos(const LiveActor*, int, TVec3f*);
    void copyJointScale(const LiveActor*, const char*, TVec3f*);
    void hideJoint(J3DJoint*);
    void hideJoint(J3DModel*, const char*);
    void hideJoint(const LiveActor*, const char*);
    void hideJointAndChildren(J3DJoint*);
    void hideJointAndChildren(J3DModel*, const char*);
    void hideJointAndChildren(const LiveActor*, const char*);
    void showJoint(J3DJoint*);
    void showJoint(J3DModel*, const char*);
    void showJoint(const LiveActor*, const char*);
    void showJointAndChildren(J3DJoint*);
    void showJointAndChildren(J3DModel*, const char*);
    void showJointAndChildren(const LiveActor*, const char*);
    f32 getJointTransX(const J3DJoint*);
    f32 getJointTransY(const J3DJoint*);
    f32 getJointTransZ(const J3DJoint*);
    s32 searchChildJoint(J3DJoint*, J3DJoint*);
    s32 getParentJoint(J3DModelData*, J3DJoint*);
    s32 getParentJoint(const LiveActor*, J3DJoint*);
};  // namespace MR
