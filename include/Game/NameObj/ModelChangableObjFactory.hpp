#pragma once

#include <revolution/types.h>

class NameObj;

typedef NameObj* (*CreatorFuncPtr)(const char*);

struct Model2CreateFunc {
    /* 0x0 */ const char* _0;
    /* 0x4 */ const char* mArchiveName;
    /* 0x8 */ CreatorFuncPtr mCreatorFunc;
};

namespace MR {
    CreatorFuncPtr getModelChangableObjCreator(const char*);
    void requestMountModelChangableObjArchives(const char*, s32);
    bool isReadResourceFromDVDAtModelChangableObj(const char*, s32);
};  // namespace MR
