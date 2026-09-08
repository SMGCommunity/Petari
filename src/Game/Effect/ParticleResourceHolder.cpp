#include "Game/Effect/ParticleResourceHolder.hpp"
#include "Game/Effect/ParticleEmitter.hpp"
#include "Game/Util/FileUtil.hpp"
#include "Game/Util/JMapInfo.hpp"
#include "Game/Util/MemoryUtil.hpp"
#include "Game/Util/StringUtil.hpp"

#include <JSystem/JKernel/JKRMemArchive.hpp>
#include <JSystem/JParticle/JPAEmitter.hpp>
#include <JSystem/JParticle/JPAResourceManager.hpp>

bool ParticleEmitter::isValid() const {
    return mEmitter != nullptr;
}

bool ParticleEmitter::isContinuousParticle() const {
    return mEmitter && mEmitter->mMaxFrame == 0;
}

ParticleResourceHolder::ParticleResourceHolder(const char* pFilePath)
    : mResourceMgr(nullptr), mAutoEffectList(new JMapInfo()), mParticleNames(new JMapInfo()) {
    JKRMemArchive* arch = MR::mountArchive(pFilePath, nullptr);
    mResourceMgr = new JPAResourceManager(arch->getResource("Particles.jpc"), MR::getCurrentHeap());
    mParticleNames->attach(arch->getResource("ParticleNames.bcsv"));
    mAutoEffectList->attach(arch->getResource("AutoEffectList.bcsv"));
    countAutoEffectNum();
}

u16 ParticleResourceHolder::getUserIndex(const char* pName) const {
    return mParticleNames->findElementIndexByName(pName);
}

void ParticleResourceHolder::countAutoEffectNum() {
    for (JMapInfoIter iter = mAutoEffectList->begin(); iter != mAutoEffectList->end(); iter.mIndex++) {
        const char* pGroupName = nullptr;
        iter.getValue("GroupName", &pGroupName);
        if (pGroupName) {
            bool found = false;

            for (Particle** particle = mParticles.begin(); particle != mParticles.end(); particle++) {
                if (MR::isEqualStringCase((*particle)->mGroupName, pGroupName)) {
                    found = true;
                    (*particle)->mCount++;
                    break;
                }
            }

            if (!found) {
                mParticles.push_back(new Particle(pGroupName, 1));
            }
        }
    }
}

void ParticleResourceHolder::swapTexture(const ResTIMG* pImg, const char* pSwapName) {
    mResourceMgr->swapTexture(pImg, pSwapName);
}

bool ParticleResourceHolder::isExistInResource(const char* pName, u16* pOutIndex) const {
    const u16 idx = getUserIndex(pName);
    if (idx == 0xFFFF) {
        return false;
    } else if (pOutIndex != nullptr) {
        *pOutIndex = idx;
    }

    return true;
}

JMapInfo* ParticleResourceHolder::getAutoEffectListBinary() const {
    return mAutoEffectList;
}

s32 ParticleResourceHolder::getAutoEffectNum(const char* pName) const {
    if (pName == nullptr) {
        return 0u;
    }

    for (Particle* const* particle = mParticles.begin(); particle != mParticles.end(); particle++) {
        if (MR::isEqualStringCase((*particle)->mGroupName, pName)) {
            return (*particle)->mCount;
        }
    }

    return 0u;
}
