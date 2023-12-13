#pragma once

#include "Game/NameObj/NameObj.hpp"

class CameraHolder;
class CameraParamChunk;
class CameraParamChunkID;
class JKRHeap;

class CameraParamChunkHolder : public NameObj {
public:
    CameraParamChunkHolder(CameraHolder *pCameraHolder, const char *pName);
    virtual ~CameraParamChunkHolder();

    CameraParamChunk *createChunk(const CameraParamChunkID &, JKRHeap *);
    CameraParamChunk *getChunk(const CameraParamChunkID &);
    void sort();
    void loadCameraParameters();
    bool isNewAttribute(const CameraParamChunkID &);
    CameraParamChunk *findChunk(const CameraParamChunkID &);
    CameraParamChunk *findChunk(long, const char *);
    void loadFile(long);
    void arrangeChunk(CameraParamChunk *);

    CameraHolder *mCameraHolder;    // _C
    u32 mChunkCapacity;             // _10
    u32 mNrChunks;                  // _14
    CameraParamChunk **mChunks;     // _18
    u32 mCameraVersion;             // _1C
    u8 mIsSorted;                   // _20
    u8 _21[3];
};