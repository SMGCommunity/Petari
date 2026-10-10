#pragma once

#include <revolution.h>

class JMapInfo;

class RailGraphEdge {
public:
    RailGraphEdge();

    void clearConnectInfo();
    s32 getNextNode(s32) const;
    s32 getNextEdge(s32) const;
    void setNextEdge(s32, s32);
    void setArgs(const JMapInfo*, s32);

    void operator=(const RailGraphEdge& rEdge) {
        mDistance = rEdge.mDistance;
        _4 = rEdge._4;
        _8 = rEdge._8;
        _C = rEdge._C;
        _10 = rEdge._10;
        mPointArg0 = rEdge.mPointArg0;
        mPointArg1 = rEdge.mPointArg1;
        mPointArg2 = rEdge.mPointArg2;
        mPointArg3 = rEdge.mPointArg3;
        mPointArg4 = rEdge.mPointArg4;
        mPointArg5 = rEdge.mPointArg5;
        mPointArg6 = rEdge.mPointArg6;
        mPointArg7 = rEdge.mPointArg7;
    }

    /* 0x00 */ f32 mDistance;
    /* 0x04 */ s32 _4;
    /* 0x08 */ s32 _8;
    /* 0x0C */ s32 _C;
    /* 0x10 */ s32 _10;
    /* 0x14 */ s32 mPointArg0;
    /* 0x18 */ s32 mPointArg1;
    /* 0x1C */ s32 mPointArg2;
    /* 0x20 */ s32 mPointArg3;
    /* 0x24 */ s32 mPointArg4;
    /* 0x28 */ s32 mPointArg5;
    /* 0x2C */ s32 mPointArg6;
    /* 0x30 */ s32 mPointArg7;
};
