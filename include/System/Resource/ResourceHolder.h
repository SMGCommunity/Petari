#ifndef RESOURCEHOLDER_H
#define RESOURCEHOLDER_H

#include "JKR/JKRArchivePri.h"
#include "System/Resource/ResourceInfo.h"

class ResourceHolder
{
public:
    ResourceHolder(JKRArchive &);

    bool isExistMaterialAnim();

    ResTable* mResourceTable; // _0
    u8 _4[0x38-0x4];
    u32 _38;
    u32 _3C;
    u32 _40;
    JKRArchive* mArchive; // _44
    u32* _48; // JKRHeap*
    u32 _4C;
};

#endif // RESOURCEHOLDER_H