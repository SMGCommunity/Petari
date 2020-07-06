#pragma once

#include <revolution.h>

class CameraParamChunkID
{
public:
    CameraParamChunkID();
    CameraParamChunkID(const CameraParamChunkID &);

    virtual char* getBuffer(u32);

    bool operator>(const CameraParamChunkID &) const;
    bool operator==(const CameraParamChunkID &) const;
    bool equals(s32, const char *) const;
    void createCubeID(s32, u16);
    void createGroupID(s32, const char *, u32, u32);
    void createOtherID(s32, const char *);
    void createEventID(s32, const char *);
    void createStartID(s32, u16);

    s8 mChunkLength; // _4
    char* mBuffer; // _8
};