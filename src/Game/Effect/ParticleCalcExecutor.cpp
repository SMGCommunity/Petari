#include "Game/Effect/ParticleCalcExecutor.hpp"
#include "Game/Effect/EffectSystem.hpp"
#include "Game/Effect/ParticleEmitterHolder.hpp"
#include "Game/NameObj/NameObjAdaptor.hpp"
#include "Game/Scene/SceneFunction.hpp"
#include "Game/Util/Functor.hpp"
#include "Game/Util/ObjUtil.hpp"
#include <JSystem/JParticle/JPAEmitterManager.hpp>

namespace {
    void connectToSceneCalcAnimAdaptor(NameObjAdaptor* pAdaptor, const MR::FunctorBase& rFunctor, int calcAnimType) {
        pAdaptor->connectToCalcAnim(rFunctor);
        MR::connectToScene(pAdaptor, -1, calcAnimType, -1, -1);
    }

    void connectToSceneMovementAdaptor(NameObjAdaptor* pAdaptor, const MR::FunctorBase& rFunctor, int movementType) {
        pAdaptor->connectToMovement(rFunctor);
        MR::connectToScene(pAdaptor, movementType, -1, -1, -1);
    }
}  // namespace

ParticleCalcExecutor::ParticleCalcExecutor(const EffectSystem* pEffectSystem, bool createAdaptors)
    : mEffectSystem(pEffectSystem), mNormalAdaptor(), mIgnorePause3DAdaptor(), mIgnorePause2DAdaptor(), mCheckUpdateAdaptor(), mMovementEnabled(true),
      mUpdateRequested() {
    if (createAdaptors) {
        initMovementAdaptor();
    }
}

void ParticleCalcExecutor::movementNormal() {
    if (mMovementEnabled && mUpdateRequested) {
        mEffectSystem->mEmitterHolder->update(false);
        mEffectSystem->mEmitterManager->calc(0);
        mEffectSystem->mEmitterManager->calc(1);
        mEffectSystem->mEmitterManager->calc(2);
        mEffectSystem->mEmitterManager->calc(3);
        mEffectSystem->mEmitterManager->calc(4);
        mEffectSystem->mEmitterManager->calc(5);
        mEffectSystem->mEmitterManager->calc(6);
        mEffectSystem->mEmitterManager->calc(7);
        mEffectSystem->mEmitterManager->calc(8);
    }

    mUpdateRequested = false;
}

void ParticleCalcExecutor::movementIgnorePause3D() {
    if (mUpdateRequested) {
        mEffectSystem->mEmitterHolder->update(true);
        mEffectSystem->mEmitterManager->calc(1);
    }
}

void ParticleCalcExecutor::movementIgnorePause2D() {
    if (mUpdateRequested) {
        mEffectSystem->mEmitterHolder->update(true);
        mEffectSystem->mEmitterManager->calc(7);
    }
}

void ParticleCalcExecutor::movementCheckUpdate() {
    mUpdateRequested = true;
}

void ParticleCalcExecutor::requestMovementOnPauseIgnore() {
    MR::requestMovementOn(mNormalAdaptor);
    MR::requestMovementOn(mIgnorePause3DAdaptor);
    MR::requestMovementOn(mIgnorePause2DAdaptor);
}

void ParticleCalcExecutor::initMovementAdaptor() {
    mNormalAdaptor = new NameObjAdaptor("パーティクル");
    connectToSceneCalcAnimAdaptor(mNormalAdaptor, MR::Functor(this, &ParticleCalcExecutor::movementNormal), MR::CalcAnimType_AnimParticle);

    mIgnorePause3DAdaptor = new NameObjAdaptor("ポーズ無効3Dパーティクル");
    connectToSceneCalcAnimAdaptor(mIgnorePause3DAdaptor, MR::Functor(this, &ParticleCalcExecutor::movementIgnorePause3D),
                                  MR::CalcAnimType_AnimParticleIgnorePause);

    mIgnorePause2DAdaptor = new NameObjAdaptor("ポーズ無効2Dパーティクル");
    connectToSceneCalcAnimAdaptor(mIgnorePause2DAdaptor, MR::Functor(this, &ParticleCalcExecutor::movementIgnorePause2D),
                                  MR::CalcAnimType_AnimParticleIgnorePause);

    mCheckUpdateAdaptor = new NameObjAdaptor("更新チェック");
    connectToSceneMovementAdaptor(mCheckUpdateAdaptor, MR::Functor(this, &ParticleCalcExecutor::movementCheckUpdate), MR::MovementType_UNK_0x14);
}
