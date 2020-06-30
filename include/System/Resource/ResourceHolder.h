#pragma once

#include "JKernel/JKRArchivePri.h"
#include "System/Resource/ResourceInfo.h"

class ResourceHolder
{
public:
    ResourceHolder(JKRArchive &);

    const char* getResName(u32) const;
    bool isExistMaterialAnim();

    ResTable* mResourceTable; // _0
    ResTable* _4;
    u8 _8[0x38-0x8];
    u32 _38;
    u32 _3C;
    u32 _40;
    JKRArchive* mArchive; // _44
    u32* _48; // JKRHeap*
    u32 _4C;
};