#include "Game/Effect/EffectSystem.hpp"
#include "Game/Effect/EffectSystemUtil.hpp"
#include "Game/Effect/SingleEmitter.hpp"
#include "Game/Scene/SceneObjHolder.hpp"
#include <JSystem/JParticle/JPAEmitter.hpp>
#include <JSystem/JParticle/JPAEmitterManager.hpp>

void EffectSystem::init(const JMapInfoIter& rIter) {
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

void EffectSystem::createSingleEmitter(SingleEmitter* pSingleEmitter, MultiEmitterCallBackBase* pCallBack, MultiEmitterParticleCallBack* pParticleCallBack) {
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
            pEmitter->mEmitter->_EC = pCallBack;
        }

        if (pParticleCallBack != nullptr) {
            pEmitter->mEmitter->_F0 = pParticleCallBack;
        }
    }
}

namespace MR {
    EffectSystem* getEffectSystem() {
        return getSceneObj< EffectSystem >(SceneObj_EffectSystem);
    }
}; // namespace MR
