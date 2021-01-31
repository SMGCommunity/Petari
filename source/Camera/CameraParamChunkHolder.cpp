#include "Camera/CameraParamChunkHolder.h"

CameraParamChunkHolder::CameraParamChunkHolder(CameraHolder *pCameraHolder, const char *pName)
    : NameObj(pName)
{
    mHolder = pCameraHolder;
    mTotalChunkSize = 0x400;
    mChunkCount = 0;
    mChunks = new CameraParamChunk*[0x1000];
}

CameraParamChunkHolder::~CameraParamChunkHolder()
{
    
}

