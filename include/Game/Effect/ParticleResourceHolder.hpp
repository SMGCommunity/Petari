#pragma once

#include <revolution/types.h>

class JMapInfo;
class JPAResourceManager;
struct ResTIMG;

class ParticleResourceHolder {
public:
    struct EffectNum {
        EffectNum(const char* pGroupName) : mGroupName(pGroupName), mCount(1) {
        }

        /* 0x00 */ const char* mGroupName;
        /* 0x04 */ int mCount;
    };

    ParticleResourceHolder(const char* pArchiveName);

    u16 getUserIndex(const char* pName) const;
    void countAutoEffectNum();
    void swapTexture(const ResTIMG* pImage, const char* pName);
    bool isExistInResource(const char* pName, u16* pIndex) const;
    JMapInfo* getAutoEffectListBinary() const;
    int getAutoEffectNum(const char* pGroupName) const;

    /* 0x0000 */ JPAResourceManager* mResourceMgr;
    /* 0x0004 */ JMapInfo* mAutoEffectList;
    /* 0x0008 */ JMapInfo* mParticleNames;
    /* 0x000C */ EffectNum* mEffectNums[1024];
    /* 0x100C */ int mNumEffectNums;
};
