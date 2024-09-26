#ifndef OSBOOTINFO_H
#define OSBOOTINFO_H

#include <revolution/dvd.h>

typedef struct OSBootInfo_s {
    DVDDiskID DVDDiskID;
    u32 magic;
    u32 version;
    u32 memorySize;
    u32 consoleType;
    void* arenaLo;
    void* arenaHi;
    void* FSTLocation;
    u32 FSTMaxLength;
} OSBootInfo;

#endif // OSBOOTINFO_H