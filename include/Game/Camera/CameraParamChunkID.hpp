#pragma once

#include <revolution/types.h>

class CameraParamChunkID {
public:
    CameraParamChunkID();
    CameraParamChunkID(const CameraParamChunkID&);

    bool operator>(const CameraParamChunkID&) const;
    bool operator==(const CameraParamChunkID&) const;

    virtual char* getBuffer(u32);

    bool equals(s32, const char*) const;
    void createCubeID(s32, u16);
    void createGroupID(s32, const char*, u32, u32);
    void createOtherID(s32, const char*);
    void createEventID(s32, const char*);
    void createStartID(s32, u16);

    /* 0x04 */ s8 mZoneID;
    /* 0x08 */ char* mName;
};