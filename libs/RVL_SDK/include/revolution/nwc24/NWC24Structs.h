#ifndef NWC24STRUCTS_H
#define NWC24STRUCTS_H

typedef struct NWC24WorkMem {
    char StrBuffer_LineTmp[1024];  // offset 0x0, size 0x400
    char StrBuffer_WorkTmp[1024];  // offset 0x400, size 0x400
    char StrBuffer_FilePath[256];  // offset 0x800, size 0x100
    char FileRdBuffer[512];        // offset 0x900, size 0x200
    char FileWrBuffer[2048];       // offset 0xB00, size 0x800
    char ConfigCache[1024];        // offset 0x1300, size 0x400
    char MBCHdCache_SendBox[128];  // offset 0x1700, size 0x80
    char MBCHdCache_RecvBox[128];  // offset 0x1780, size 0x80
    char MsgObjCache[256];         // offset 0x1800, size 0x100
    char MimeTable[256];           // offset 0x1900, size 0x100
    char SearchWork[4096];         // offset 0x1A00, size 0x1000
    char FLHdCache[1024];          // offset 0x2A00, size 0x400
    char DWCFLHdCache[2048];       // offset 0x2E00, size 0x800
    char DLHdCache[2048];          // offset 0x3600, size 0x800
    char DLBuffer[512];            // offset 0x3E00, size 0x200
} NWC24WorkMem;

#endif  // NWC24STRUCTS_H
