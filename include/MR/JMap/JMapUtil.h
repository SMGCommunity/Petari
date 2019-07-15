#ifndef JMAPUTIL_H
#define JMAPUTIL_H

#include "JMap/JMapInfoIter.h"

namespace MR
{
    void getJMapInfoClippingGroupID(const JMapInfoIter &, s64 *);
    void getJMapInfoDemoGroupID(const JMapInfoIter &, s64 *);
    void getJMapInfoLinkID(const JMapInfoIter &, s64 *);

    bool isConnectedWIthRail(const JMapInfoIter &);
    bool isExistStageSwitchA(const JMapInfoIter &);
    bool isExistStageSwitchB(const JMapInfoIter &);
};

bool getJMapInfoArgNoInit(const JMapInfoIter &, const char *, s64 *);

template<typename T>
void getValue(const JMapInfoIter &, const char *, T *);

#endif // JMAPUTIL_H