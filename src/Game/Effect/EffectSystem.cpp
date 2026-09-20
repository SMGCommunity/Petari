#include "Game/Effect/EffectSystem.hpp"
#include "Game/Effect/AutoEffectGroupHolder.hpp"
#include "Game/Effect/EffectSystemUtil.hpp"
#include "Game/Effect/MultiEmitterCallBack.hpp"
#include "Game/Effect/MultiEmitterParticleCallBack.hpp"
#include "Game/Effect/ParticleCalcExecutor.hpp"
#include "Game/Effect/ParticleDrawExecutor.hpp"
#include "Game/Effect/ParticleEmitterHolder.hpp"
#include "Game/Effect/ParticleResourceHolder.hpp"
#include "Game/Effect/SingleEmitter.hpp"
#include "Game/Scene/SceneObjHolder.hpp"
#include "Game/Util/MemoryUtil.hpp"
#include "Game/Util/ScreenUtil.hpp"
#include <JSystem/JParticle/JPAParticle.hpp>
#include <JSystem/JParticle/JPAEmitter.hpp>
#include <JSystem/JParticle/JPAEmitterManager.hpp>

EffectSystem::EffectSystem(const char* pName, bool createAdaptors)
    : NameObj(pName), mEmitterManager(), mDrawExec(), mCalcExec(), mGroupHolder(new AutoEffectGroupHolder()), _20(true) {
    mDrawExec = new ParticleDrawExecutor(this, createAdaptors);
    mCalcExec = new ParticleCalcExecutor(this, createAdaptors);
}

void EffectSystem::init(const JMapInfoIter& rIter) {
}

ParticleEmitter* EffectSystem::createEmitter(u16 resourceId, u8 groupId, u8 resourceManagerId) {
    if (!_20) {
        return nullptr;
    }

    ParticleEmitter* pEmitter = mEmitterHolder->findAvailableParticleEmitter();
    if (pEmitter == nullptr) {
        return nullptr;
    }

    MR::Effect::createParticleEmitter(pEmitter, mEmitterManager, TVec3f(0.0f, 0.0f, 0.0f), resourceId, groupId, resourceManagerId);
    if (pEmitter->mEmitter != nullptr) {
        return pEmitter;
    }

    return nullptr;
}

void EffectSystem::forceDeleteEmitter(ParticleEmitter* pEmitter) const {
    if (pEmitter->mEmitter != nullptr) {
        SingleEmitter* pSingleEmitter = MR::Effect::getLinkSingleEmitter(pEmitter->mEmitter);

        if (pSingleEmitter != nullptr) {
            pSingleEmitter->mEmitter = nullptr;
        }

        mEmitterManager->forceDeleteEmitter(pEmitter->mEmitter);
        pEmitter->invalidate();
    }
}

void EffectSystem::forceDeleteSingleEmitter(SingleEmitter* pSingleEmitter) const {
    ParticleEmitter* pEmitter = pSingleEmitter->mEmitter;

    if (pSingleEmitter->mEmitter != nullptr) {
        pSingleEmitter->mEmitter = nullptr;

        mEmitterManager->forceDeleteEmitter(pEmitter->mEmitter);
        pEmitter->invalidate();
    }
}

void EffectSystem::createSingleEmitter(SingleEmitter* pSingleEmitter, MultiEmitterCallBackBase* pCallBack,
                                       MultiEmitterParticleCallBack* pParticleCallBack) {
    if (pSingleEmitter->isValid()) {
        if (!pSingleEmitter->isOneTime()) {
            return;
        }

        pSingleEmitter->unlink();
    }

    ParticleEmitter* pEmitter = createEmitter(pSingleEmitter->_4, pSingleEmitter->mGroupId, 0);

    if (pEmitter != nullptr) {
        pSingleEmitter->link(pEmitter);

        if (pCallBack != nullptr) {
            pEmitter->mEmitter->mpEmtrCallBack = pCallBack;
        }

        if (pParticleCallBack != nullptr) {
            pEmitter->mEmitter->mpPtclCallBack = pParticleCallBack;
        }
    }
}

namespace MR {
    EffectSystem* getEffectSystem() {
        return getSceneObj< EffectSystem >(SceneObj_EffectSystem);
    }
};  // namespace MR

EffectSystem::~EffectSystem() {
}

void EffectSystem::entry(ParticleResourceHolder* pResourceHolder, u32 particleCount, u32 emitterCount) {
    mEmitterManager = new JPAEmitterManager(particleCount, emitterCount, MR::getCurrentHeap(), 9, 1);
    mEmitterManager->entryResourceManager(pResourceHolder->mResourceMgr, 0);
    mEmitterHolder = new ParticleEmitterHolder(this, emitterCount);
    pResourceHolder->swapTexture(MR::getScreenResTIMG(), "IndDummy");
}
