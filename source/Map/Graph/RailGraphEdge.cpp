#include "Map/Graph/RailGraphEdge.h"

RailGraphEdge::RailGraphEdge()
{
    mDistance = 0.0f;
    mPrev = -1;
    mNext = -1;
    _C = -1;
    _10 = -1;
    mPointArg0 = -1;
    mPointArg1 = -1;
    mPointArg2 = -1;
    mPointArg3 = -1;
    mPointArg4 = -1;
    mPointArg5 = -1;
    mPointArg6 = -1;
    mPointArg7 = -1;
}

void RailGraphEdge::clearConnectInfo()
{
    mDistance = 0.0f;
    mPrev = -1;
    mNext = -1;
    _C = -1;
    _10 = -1;
}

s32 RailGraphEdge::getNextNode(s32 a1) const
{
    if (mPrev == a1)
        return mNext;

    if (mNext != a1)
        return -1;

    return mPrev;
}

s32 RailGraphEdge::getNextEdge(s32 a1) const
{
    if (mPrev == a1)
        return _C;

    if (mPrev == a1)
        return _10;

    return -1;
}

void RailGraphEdge::setNextEdge(s32 a1, s32 a2)
{
    if (mPrev == a2)
        _C = a1;

    if (mNext == a2)
        return;

    _10 = a1;
}

void RailGraphEdge::setArgs(const JMapInfo *pInfo, s32 a2)
{
    pInfo->getValue<s32>(a2, "point_arg0", &mPointArg0);
    pInfo->getValue<s32>(a2, "point_arg1", &mPointArg1);
    pInfo->getValue<s32>(a2, "point_arg2", &mPointArg2);
    pInfo->getValue<s32>(a2, "point_arg3", &mPointArg3);
    pInfo->getValue<s32>(a2, "point_arg4", &mPointArg4);
    pInfo->getValue<s32>(a2, "point_arg5", &mPointArg5);
    pInfo->getValue<s32>(a2, "point_arg6", &mPointArg6);
    pInfo->getValue<s32>(a2, "point_arg7", &mPointArg7);
}