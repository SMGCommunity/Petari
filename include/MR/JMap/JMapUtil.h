#ifndef JMAPUTIL_H
#define JMAPUTIL_H

#include "JGeometry/TVec3.h"
#include "JMap/JMapInfoIter.h"

bool getJMapInfoArgNoInit(const JMapInfoIter &, const char *, s32 *);
bool getJMapInfoArgNoInit(const JMapInfoIter &, const char *, f32 *);
bool getJMapInfoArgNoInit(const JMapInfoIter &, const char *, bool *);

namespace MR
{
    bool isValidInfo(const JMapInfoIter &);
    bool isObjectName(const JMapInfoIter &, const char *);
    bool getJMapInfoArg0NoInit(const JMapInfoIter &, s32 *);
    bool getJMapInfoArg0NoInit(const JMapInfoIter &, f32 *);
    bool getJMapInfoArg0NoInit(const JMapInfoIter &, bool *);
    bool getJMapInfoArg1NoInit(const JMapInfoIter &, s32 *);
    bool getJMapInfoArg1NoInit(const JMapInfoIter &, f32 *);
    bool getJMapInfoArg1NoInit(const JMapInfoIter &, bool *);
    bool getJMapInfoArg2NoInit(const JMapInfoIter &, s32 *);
    bool getJMapInfoArg2NoInit(const JMapInfoIter &, f32 *);
    bool getJMapInfoArg2NoInit(const JMapInfoIter &, bool *);
    bool getJMapInfoArg3NoInit(const JMapInfoIter &, s32 *);
    bool getJMapInfoArg3NoInit(const JMapInfoIter &, f32 *);
    bool getJMapInfoArg3NoInit(const JMapInfoIter &, bool *);
    bool getJMapInfoArg4NoInit(const JMapInfoIter &, s32 *);
    bool getJMapInfoArg4NoInit(const JMapInfoIter &, f32 *);
    bool getJMapInfoArg4NoInit(const JMapInfoIter &, bool *);
    bool getJMapInfoArg5NoInit(const JMapInfoIter &, s32 *);
    bool getJMapInfoArg5NoInit(const JMapInfoIter &, f32 *);
    bool getJMapInfoArg5NoInit(const JMapInfoIter &, bool *);
    bool getJMapInfoArg6NoInit(const JMapInfoIter &, s32 *);
    bool getJMapInfoArg6NoInit(const JMapInfoIter &, f32 *);
    bool getJMapInfoArg6NoInit(const JMapInfoIter &, bool *);
    bool getJMapInfoArg7NoInit(const JMapInfoIter &, s32 *);
    bool getJMapInfoArg7NoInit(const JMapInfoIter &, f32 *);
    bool getJMapInfoArg7NoInit(const JMapInfoIter &, bool *);
    bool isEqualObjectName(const JMapInfoIter &, const char *);
    s32 getDemoGroupID(const JMapInfoIter &);
    s32 getDemoGroupLinkID(const JMapInfoIter &);
    s32 getJMapInfoRailArg(const JMapInfoIter &, const char *, s32 *);
    s32 getJMapInfoRailArg0NoInit(const JMapInfoIter &, s32 *);
    s32 getRailId(const JMapInfoIter &, s32 *);
    bool getObjectName(const char **, const JMapInfoIter &);
    bool isExistJMapArg(const JMapInfoIter &);
    bool getJMapInfoShapeIdWithInit(const JMapInfoIter &, s32 *);
    bool getJMapInfoTransLocal(const JMapInfoIter &, JGeometry::TVec3<f32> *);
    bool getJMapInfoRotateLocal(const JMapInfoIter &, JGeometry::TVec3<f32> *);
    bool getJMapInfoScaleLocal(const JMapInfoIter &, JGeometry::TVec3<f32> *);
    bool getJMapInfoV3f(const JMapInfoIter &, const char *, JGeometry::TVec3<f32> *);
    bool getJMapInfoArg1WithInit(const JMapInfoIter &, s32 *);
    bool getJMapInfoArg2WithInit(const JMapInfoIter &, s32 *);
    bool getJMapInfoArg3WithInit(const JMapInfoIter &, s32 *);
    bool getJMapInfoArg4WithInit(const JMapInfoIter &, s32 *);
    bool getJMapInfoArg5WithInit(const JMapInfoIter &, s32 *);
    bool getJMapInfoArg6WithInit(const JMapInfoIter &, s32 *);
    bool getJMapInfoArg7WithInit(const JMapInfoIter &, s32 *);
    bool getJMapInfoFollowID(const JMapInfoIter &, s32 *);
    bool getJMapInfoGroupID(const JMapInfoIter &, s32 *);
    bool getJMapInfoClippingGroupID(const JMapInfoIter &, s32 *);
    bool getJMapInfoDemoGroupID(const JMapInfoIter &, s32 *);
    bool getJMapInfoLinkID(const JMapInfoIter &, s32 *);
    bool isConnectedWithRail(const JMapInfoIter &);
    bool isExistStageSwitchA(const JMapInfoIter &);
    bool isExistStageSwitchB(const JMapInfoIter &);
    bool isExistStageSwitchAppear(const JMapInfoIter &);
    bool isExistStageSwitchDead(const JMapInfoIter &);
    bool isExistStageSwitchSleep(const JMapInfoIter &);

    void getRailPointPos0(const JMapInfoIter &, JGeometry::TVec3<f32> *);
    void getRailPointPos1(const JMapInfoIter &, JGeometry::TVec3<f32> *);
    void getRailPointPos2(const JMapInfoIter &, JGeometry::TVec3<f32> *);
};

bool getJMapInfoArgNoInit(const JMapInfoIter &, const char *, s32 *);

template<typename T>
void getValue(const JMapInfoIter &, const char *, T *);

#endif // JMAPUTIL_H