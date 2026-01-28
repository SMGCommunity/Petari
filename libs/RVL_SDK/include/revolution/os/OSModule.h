#ifndef OSMODULE_H
#define OSMODULE_H

#include <revolution/types.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OSModuleHeader OSModuleHeader;

typedef u32 OSModuleID;
typedef struct OSModuleQueue OSModuleQueue;
typedef struct OSModuleLink OSModuleLink;
typedef struct OSModuleInfo OSModuleInfo;
typedef struct OSSectionInfo OSSectionInfo;
typedef struct OSImportInfo OSImportInfo;
typedef struct OSRel OSRel;

struct OSModuleQueue {
    OSModuleInfo* head;
    OSModuleInfo* tail;
};

struct OSModuleLink {
    OSModuleInfo* next;
    OSModuleInfo* prev;
};

struct OSModuleInfo {
    OSModuleID id;
    OSModuleLink link;
    u32 numSections;
    u32 sectionInfoOffset;
    u32 nameOffset;
    u32 nameSize;
    u32 version;
};

struct OSSectionInfo {
    u32 offset;
    u32 size;
};

#ifdef __cplusplus
}
#endif

#endif  // OSMODULE_H
