#pragma once

#include "Camera/CameraHolder.h"
#include "Camera/CameraParamChunk.h"
#include "NameObj/NameObj.h"
#include "JKernel/JKRHeap.h"

class CameraParamChunkHolder : public NameObj
{
public:
    CameraParamChunkHolder(CameraHolder *, const char *);
    
    virtual ~CameraParamChunkHolder();

    CameraParamChunk* createChunk(const CameraParamChunkID &, JKRHeap *);
    CameraParamChunk* getChunk(const CameraParamChunkID &);
    void sort();
    void loadCameraParameters();
    bool isNewAttribute(const CameraParamChunkID &);
    CameraParamChunk* findChunk(const CameraParamChunkID &);

    CameraHolder* mHolder; // _C
    s32 mTotalChunkSize; // _10
    s32 mChunkCount; // _14
    CameraParamChunk** mChunks; // _18;
    u32 _1C;
    u8 _20;
};