#pragma once

#include "Game/Util/JMapInfo.hpp"
#include "JSystem/JParticle/JPAResourceManager.hpp"
#include <revolution.h>

class ResTIMG;

class ParticleResourceHolder {
public:
    struct Particle {
        const char* mGroupName;  // 0x0
        int mCount;              // 0x4
    };

    ParticleResourceHolder(const char*);

    void countAutoEffectNum();
    void swapTexture(const ResTIMG*, const char*);
    bool isExistInResource(const char*, u16*) const;
    JMapInfo* getAutoEffectListBinary() const;
    int getAutoEffectNum(const char*) const;

    JPAResourceManager* mResourceMgr;  // 0x0
    JMapInfo* mAutoEffectList;         // 0x4
    JMapInfo* mParticleNames;          // 0x8
    Particle mParticles[512];          // 0xC
    int mNumParticles;                 // 0x100C
};