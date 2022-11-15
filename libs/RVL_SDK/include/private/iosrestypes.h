#pragma once

#include "revolution/types.h"

typedef u32 IOSUid;
typedef u16 IOSGid;
typedef u32 IOSResourceHandle;

typedef struct {
    u8 *path;
    u32 flags;
    IOSUid uid; 
    IOSGid gid;
} IOSResourceOpen;

typedef struct {
    u8 *outPtr;
    u32 outLen;
} IOSResourceRead;

typedef struct {
    u8 *inPtr;
    u32 inLen;
} IOSResourceWrite;

typedef struct {
    s32 offset;
    u32 whence;
} IOSResourceSeek;

typedef struct {
    u32 cmd;
    u8 *inPtr;
    u32 inLen;
    u8 *outPtr;
    u32 outLen;
} IOSResourceIoctl;

typedef struct {
    u8 *base;
    u32 length;
} IOSIoVector;

typedef struct {
    u32 cmd;
    u32 readCount;
    u32 writeCount;
    IOSIoVector *vector;
} IOSResourceIoctlv;

typedef struct {
    u32 cmd;
    s32 status;
    IOSResourceHandle handle;
    union {
        IOSResourceOpen open;
        IOSResourceRead read;
        IOSResourceWrite write;
        IOSResourceSeek seek;
        IOSResourceIoctl ioctl;
        IOSResourceIoctlv ioctlv;
    } args;
} IOSResourceRequest;