#include "Game/Screen/PaneEffectKeeper.hpp"
#include "Game/Effect/EffectSystemUtil.hpp"
#include "Game/Effect/MultiEmitter.hpp"
#include "Game/Effect/ParticleResourceHolder.hpp"
#include "Game/Screen/LayoutActor.hpp"
#include "Game/Screen/LayoutManager.hpp"
#include "Game/Util/HashUtil.hpp"
#include "Game/Util/SystemUtil.hpp"

extern "C" void __ptmf_scall(...);

PaneEffectKeeper::PaneEffectKeeper(LayoutActor* pActor, const LayoutManager* pManager, int myInt, const char* pName) : _0(pActor), mName(pName), mEmitters() {
    if (pName == nullptr) {
        mName = pManager->_78;
    }

    s32 effectNum = MR::getParticleResourceHolder()->getAutoEffectNum(mName);
    mEmitters.init(myInt + effectNum);

    for (s32 idx = 0; idx < mEmitters.mCount; idx++) {
        mEmitters.mArray[idx] = nullptr;
    }
}

void PaneEffectKeeper::init(const LayoutActor* pActor, const EffectSystem* pSystem) {
    if (mName == nullptr) {
        return;
    }

    if (pSystem != nullptr) {
        MR::Effect::registerAutoEffectInfoGroup(this, pSystem, pActor, mName);
        return;
    }

    MR::Effect::registerAutoEffectInfoGroup(this, pActor, mName);
}

void PaneEffectKeeper::add(const char* pName1, const char* pName2, const char* pName3) {
    if (_0->getLayoutManager()->isExistPaneMtxRef(pName1) == nullptr) {
        _0->createPaneMtxRef(pName1);
    }

    MultiEmitter* pEmitter = new MultiEmitter(pName2, _0->getPaneMtxRef(pName1), TVec3f(0.0f, 0.0f, 0.0f));

    registerEffect(pEmitter, pName3);
}

MultiEmitter* PaneEffectKeeper::createEmitter(const char* pName) {
    MultiEmitter* pEmitter = find(pName);
    pEmitter->createEmitter();

    return pEmitter;
}

void PaneEffectKeeper::deleteEmitter(const char* pName) {
    find(pName)->deleteEmitter();
}

void PaneEffectKeeper::forceDeleteEmitter(const char* pName) {
    find(pName)->forceDeleteEmitter();
}

void PaneEffectKeeper::deleteEmitterAll() {
    for (s32 idx = 0; idx < mEmitters.mCount; idx++) {
        mEmitters.mArray[idx]->deleteEmitter();
    }
}

void PaneEffectKeeper::forceDeleteEmitterAll() {
    for (s32 idx = 0; idx < mEmitters.mCount; idx++) {
        mEmitters.mArray[idx]->forceDeleteEmitter();
    }
}

void PaneEffectKeeper::clear() {
    for (MultiEmitter* const* pEmitter = mEmitters.begin(); pEmitter != mEmitters.end(); pEmitter++) {
        __ptmf_scall(*pEmitter);
    }
}

MultiEmitter* PaneEffectKeeper::getEmitter(const char* pName) const {
    find(pName);
}

void PaneEffectKeeper::changeAnim() {
}

void PaneEffectKeeper::registerEffect(MultiEmitter* pEmitter, const char* pName) {
    if (pName != nullptr) {
        pEmitter->setName(pName);
    }

    mEmitters.push_back(pEmitter);
}

MultiEmitter* PaneEffectKeeper::find(const char* pName) const {
    u16 hash = MR::getHashCode(pName);
    for (MultiEmitter* const* pEmitter = mEmitters.begin(); pEmitter != mEmitters.end(); pEmitter++) {
        if ((*pEmitter)->isEqualName(hash)) {
            return *pEmitter;
        }
    }

    return nullptr;
}
