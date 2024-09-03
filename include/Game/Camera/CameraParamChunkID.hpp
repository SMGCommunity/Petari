#pragma once

#include "revolution.h"

class CameraParamChunkID {
public:
    CameraParamChunkID();
    CameraParamChunkID(const CameraParamChunkID &);

    bool operator>(const CameraParamChunkID &) const;
    bool operator==(const CameraParamChunkID &) const;

    virtual char *getBuffer(unsigned long);

    bool equals(long, const char *) const;
    void createCubeID(long, unsigned short);
    void createGroupID(long, const char *, unsigned long, unsigned long);
    void createOtherID(long, const char *);
    void createEventID(long, const char *);
    void createStartID(long, unsigned short);

    s8 mZoneID;     // 0x4
    u8 _5[3];
    char *mName;    // 0x8
};