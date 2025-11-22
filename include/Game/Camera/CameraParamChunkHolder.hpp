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

    CameraHolder* mCameraHolder;  // 0xC
    u32 mChunkCapacity;           // 0x10
    u32 mNrChunks;                // 0x14
    CameraParamChunk** mChunks;   // 0x18
    u32 mCameraVersion;           // 0x1C
    u8 mIsSorted;                 // 0x20
    u8 _21[3];
};