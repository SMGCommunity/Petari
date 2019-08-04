#ifndef JMAPUTIL_H
#define JMAPUTIL_H

#include "JMap/JMapInfoIter.h"

bool getJMapInfoArgNoInit(const JMapInfoIter &, const char *, s32 *);
bool getJMapInfoArgNoInit(const JMapInfoIter &, const char *, f32 *);
bool getJMapInfoArgNoInit(const JMapInfoIter &, const char *, bool *);

namespace MR
{
    bool isValidInfo(const JMapInfoIter &);
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

    bool getObjectName(const char **, const JMapInfoIter &);

    void getJMapInfoClippingGroupID(const JMapInfoIter &, s64 *);
    void getJMapInfoDemoGroupID(const JMapInfoIter &, s64 *);
    void getJMapInfoLinkID(const JMapInfoIter &, s64 *);

    bool isConnectedWithRail(const JMapInfoIter &);
    bool isExistStageSwitchA(const JMapInfoIter &);
    bool isExistStageSwitchB(const JMapInfoIter &);
    bool isExistStageSwitchAppear(const JMapInfoIter &);
    bool isExistStageSwitchDead(const JMapInfoIter &);
    bool isExistStageSwitchSleep(const JMapInfoIter &);
};

bool getJMapInfoArgNoInit(const JMapInfoIter &, const char *, s64 *);

template<typename T>
void getValue(const JMapInfoIter &, const char *, T *);

#endif // JMAPUTIL_H