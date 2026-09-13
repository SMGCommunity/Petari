#pragma once

#include <revolution/types.h>

class J3DAnmColorKey;
class J3DAnmTexPattern;
class J3DAnmTextureSRTKey;
class J3DAnmTevRegKey;
class J3DMaterialAnm;
class J3DModelData;
class ResourceHolder;

class MaterialAnmBuffer {
public:
    MaterialAnmBuffer(const ResourceHolder*, J3DModelData*, bool);

    u32 getDiffFlag(s32) const;

    /* 0x00 */ J3DMaterialAnm* _0;
    /* 0x04 */ void* _4;
};

namespace MR {
    void onDiffFlagBpk(u32* pFlags, const J3DAnmColorKey* pAnm, const char* pName);
    void offDiffFlagBpk(u32* pFlags, const J3DAnmColorKey* pAnm, const char* pName);
    void onDiffFlagBtp(u32* pFlags, const J3DAnmTexPattern* pAnm, const char* pName);
    void offDiffFlagBtp(u32* pFlags, const J3DAnmTexPattern* pAnm, const char* pName);
    void onDiffFlagBtk(u32* pFlags, const J3DAnmTextureSRTKey* pAnm, const char* pName);
    void offDiffFlagBtk(u32* pFlags, const J3DAnmTextureSRTKey* pAnm, const char* pName);
    void onDiffFlagBrk(u32* pFlags, const J3DAnmTevRegKey* pAnm, const char* pName);
    void offDiffFlagBrk(u32* pFlags, const J3DAnmTevRegKey* pAnm, const char* pName);
}  // namespace MR
