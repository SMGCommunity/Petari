#pragma once

#include "Game/Util/Array.hpp"
#include "Game/Util/JMapInfo.hpp"
#include "JSystem/JParticle/JPAResourceManager.hpp"
#include <revolution.h>

class ResTIMG;

class ParticleResourceHolder {
public:
    struct Particle {
        Particle() {
        }
        Particle(const char* pGroupName, int count) : mGroupName(pGroupName), mCount(count) {
        }
        /* 0x0 */ const char* mGroupName;
        /* 0x4 */ s32 mCount;
    };

    ParticleResourceHolder(const char* pFilePath);

    u16 getUserIndex(const char* pName) const;
    void countAutoEffectNum();
    void swapTexture(const ResTIMG* pName, const char* pSwapName);
    bool isExistInResource(const char* pName, u16* pOutIndex) const;
    JMapInfo* getAutoEffectListBinary() const;
    s32 getAutoEffectNum(const char* pName) const;

    /* 0x0 */ JPAResourceManager* mResourceMgr;
    /* 0x4 */ JMapInfo* mAutoEffectList;
    /* 0x8 */ JMapInfo* mParticleNames;
    /* 0xC */ MR::Vector< MR::FixedArray< Particle*, 1024 > > mParticles;
};
