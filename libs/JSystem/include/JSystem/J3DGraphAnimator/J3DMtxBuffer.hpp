#pragma once

#include <revolution.h>

class J3DJointTree;

class J3DMtxBuffer {
public:
    J3DJointTree* mJointTree;  // 0x00
    u8* mpScaleFlagArr;        // 0x04
    u8* mpEvlpScaleFlagArr;    // 0x08
    Mtx* mpAnmMtx;             // 0x0C
    Mtx* mpWeightEvlpMtx;      // 0x10
    Mtx** mpDrawMtxArr[2];     // 0x14
    Mtx33** mpNrmMtxArr[2];    // 0x18
    Mtx33*** mpBumpMtxArr[2];  // 0x1C
    u32 mFlags;                // 0x20
    u32 mCurrentViewNo;        //
    Mtx* mpUserAnmMtx;
};
