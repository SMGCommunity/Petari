#pragma once

#include "JMap/JMapInfo.h"

class RailGraphEdge
{
public:
    RailGraphEdge();

    RailGraphEdge operator=(const RailGraphEdge &);
    void clearConnectInfo();
    s32 getNextNode(s32) const;
    s32 getNextEdge(s32) const;
    void setNextEdge(s32, s32);
    void setArgs(const JMapInfo *, s32);

    f32 mDistance; // _0
    s32 mNext; // _4
    s32 mPrev; // _8
    s32 _C;
    s32 _10;
    s32 mPointArg0; // _14
    s32 mPointArg1; // _18
    s32 mPointArg2; // _1C
    s32 mPointArg3; // _20
    s32 mPointArg4; // _24
    s32 mPointArg5; // _28
    s32 mPointArg6; // _2C
    s32 mPointArg7; // _30
};