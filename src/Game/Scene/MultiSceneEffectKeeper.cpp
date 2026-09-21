#include "Game/Scene/MultiSceneEffectKeeper.hpp"
#include "Game/Effect/EffectSystemUtil.hpp"
#include "Game/Effect/MultiEmitter.hpp"
#include "Game/Effect/ParticleResourceHolder.hpp"
#include "Game/LiveActor/ModelManager.hpp"
#include "Game/System/ResourceHolder.hpp"
#include "Game/Util/HashUtil.hpp"
#include "Game/Util/SystemUtil.hpp"
#include <JSystem/JGeometry/TVec.hpp>
#include <algorithm>

MultiSceneEffectKeeper::MultiSceneEffectKeeper(const char*, ModelManager* pModelManager, int a3, const char* pResName)
    : mEmitter(), mResName(pResName) {
    if (mResName == nullptr) {
        mResName = pModelManager->getModelResourceHolder()->getModelName();
    }

    s32 num = a3 + MR::getParticleResourceHolder()->getAutoEffectNum(mResName);
    mEmitter.init(num);
}

void MultiSceneEffectKeeper::init(const MultiSceneActor* pActor, const EffectSystem* pSystem) {
    if (mResName == nullptr) {
        return;
    }

    MR::Effect::registerAutoEffectInfoGroup(this, pSystem, pActor, mResName);
}

void MultiSceneEffectKeeper::add(const char* pName, const TVec3f* pScale, const TVec3f* pRotation, const TVec3f* pTranslation,
                                 const char* pEmitterName) {
    MultiEmitter* pEmitter = new MultiEmitter(pName, pScale, pRotation, pTranslation, TVec3f(0.0f, 0.0f, 0.0f));
    registerEmitter(pEmitter, pEmitterName);
}

void MultiSceneEffectKeeper::add(const char* pName, MtxPtr pMtx, const char* pEmitterName) {
    MultiEmitter* pEmitter = new MultiEmitter(pName, pMtx, TVec3f(0.0f, 0.0f, 0.0f));
    registerEmitter(pEmitter, pEmitterName);
}

MultiEmitter* MultiSceneEffectKeeper::create(const char* pName, EffectSystem* pSystem) {
    MultiEmitter* pEmitter = find(pName);
    pEmitter->create(pSystem);
    return pEmitter;
}

void MultiSceneEffectKeeper::deleteAll() {
    std::for_each(mEmitter.begin(), mEmitter.end(), std::mem_func(&MultiEmitter::deleteEmitter));
}

void MultiSceneEffectKeeper::forceDeleteAll(EffectSystem* pSystem) {
    std::for_each(
        mEmitter.begin(), mEmitter.end(),
        std::binder2nd< std::mem_fun1_t< void, MultiEmitter, EffectSystem* >, EffectSystem* >(std::mem_func(&MultiEmitter::forceDelete), pSystem));
}

void MultiSceneEffectKeeper::clear() {
    std::for_each(mEmitter.begin(), mEmitter.end(), std::mem_func(&MultiEmitter::playCalcAndDeleteForeverEmitter));
}

MultiEmitter* MultiSceneEffectKeeper::get(const char* pName) const {
    return find(pName);
}

MultiEmitter* MultiSceneEffectKeeper::find(const char* pName) const {
    if (mEmitter.size() == 0) {
        return nullptr;
    }

    u16 hash = MR::getHashCode(pName);
    MultiEmitter* const* pEmitter =
        std::find_if(mEmitter.begin(), mEmitter.end(),
                     std::binder2nd< std::const_mem_fun1_t< bool, MultiEmitter, u16 >, u16 >(std::mem_func(&MultiEmitter::isEqualName), hash));

    if (pEmitter != mEmitter.end()) {
        return *pEmitter;
    }

    return nullptr;
}

void MultiSceneEffectKeeper::registerEmitter(MultiEmitter* pEmitter, const char* pEmitterName) {
    if (pEmitterName != nullptr) {
        pEmitter->setName(pEmitterName);
    }

    mEmitter.push_back(pEmitter);
}
