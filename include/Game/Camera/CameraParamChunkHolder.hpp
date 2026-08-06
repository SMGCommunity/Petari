#pragma once

#include "Game/NameObj/NameObj.hpp"

class CameraHolder;
class CameraParamChunk;
class CameraParamChunkID;
class JKRHeap;

class CameraParamChunkHolder : public NameObj {
public:
    CameraParamChunkHolder(CameraHolder* pCameraHolder, const char* pName);
    virtual ~CameraParamChunkHolder();

    CameraParamChunk* createChunk(const CameraParamChunkID&, JKRHeap*);
    CameraParamChunk* getChunk(const CameraParamChunkID&);
    void sort();
    void loadCameraParameters();
    bool isNewAttribute(const CameraParamChunkID&);
    CameraParamChunk* findChunk(const CameraParamChunkID&);
    CameraParamChunk* findChunk(s32, const char*);
    void loadFile(s32);
    void arrangeChunk(CameraParamChunk*);

    /* 0x0C */ CameraHolder* mCameraHolder;
    /* 0x10 */ u32 mChunkCapacity;
    /* 0x14 */ u32 mNrChunks;
    /* 0x18 */ CameraParamChunk** mChunks;
    /* 0x1C */ u32 mCameraVersion;
    /* 0x20 */ bool mIsSorted;
};
