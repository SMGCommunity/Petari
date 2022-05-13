#pragma once

#include "Game/Util/JMapInfo.h"
#include "JSystem/JParticle/JPAResourceManager.h"
#include <revolution.h>

class ResTIMG;

class ParticleResourceHolder {
public:

    struct Particle {
        const char* mGroupName; // _0
        int mCount;             // _4
    };

    ParticleResourceHolder(const char *);

    void countAutoEffectNum();
    void swapTexture(const ResTIMG *, const char *);
    bool isExistInResource(const char *, u16 *) const;
    JMapInfo* getAutoEffectListBinary() const;
    int getAutoEffectNum(const char *) const;

    JPAResourceManager* mResourceMgr;   // _0
    JMapInfo* mAutoEffectList;          // _4
    JMapInfo* mParticleNames;           // _8
    Particle mParticles[1024];          // _C
    int mNumParticles;                  // _100C
};