#include "Game/Effect/MultiEmitter.hpp"
#include "Game/Effect/EffectSystem.hpp"
#include "Game/Effect/MultiEmitterCallBack.hpp"
#include "Game/Effect/MultiEmitterParticleCallBack.hpp"
#include "Game/Effect/SingleEmitter.hpp"
#include "Game/Util/HashUtil.hpp"
#include <JSystem/JParticle/JPAEmitter.hpp>
#include <algorithm>

MultiEmitter::MultiEmitter(const char* pName, const TVec3f* vec1, const TVec3f* vec2, const TVec3f* vec3, const TVec3f& rVec)
    : mEmitters(), mCallBack(), mParticleCallBack(), _24(), _28(), _2C(), _30(), mHash(), mFlags() {
    mCallBack = new MultiEmitterCallBack(this, vec1, vec2, vec3, rVec);
    mParticleCallBack = new MultiEmitterParticleCallBack();

    allocateEmitter(pName);
}

MultiEmitter::MultiEmitter(const char* pName, MtxPtr pMtx, const TVec3f& rVec)
    : mEmitters(), mCallBack(), mParticleCallBack(), _24(), _28(), _2C(), _30(), mHash(), mFlags() {
    mCallBack = new MultiEmitterCallBack(this, pMtx, rVec);
    mParticleCallBack = new MultiEmitterParticleCallBack();

    allocateEmitter(pName);
}

MultiEmitter::MultiEmitter(const char* pName, MtxPtr pMtx, const TVec3f* pVec, const TVec3f& rVec)
    : mEmitters(), mCallBack(), mParticleCallBack(), _24(), _28(), _2C(), _30(), mHash(), mFlags() {
    mCallBack = new MultiEmitterCallBack(this, pMtx, pVec, rVec);
    mParticleCallBack = new MultiEmitterParticleCallBack();

    allocateEmitter(pName);
}

MultiEmitter::MultiEmitter(const char* pName) : mEmitters(), mCallBack(), mParticleCallBack(), _24(), _28(), _2C(), _30(), mHash(), mFlags() {
    mCallBack = new MultiEmitterCallBack(this, TVec3f(0.0f, 0.0f, 0.0f));
    mParticleCallBack = new MultiEmitterParticleCallBack();

    allocateEmitter(pName);
}

void MultiEmitter::createEmitter() {
    create(MR::getEffectSystem());
}

void MultiEmitter::createEmitterWithCallBack(MultiEmitterCallBackBase* pCallBackBase) {
    for (SingleEmitter* pEmitter = mEmitters.begin(); pEmitter != mEmitters.end(); pEmitter++) {
        MR::getEffectSystem()->createSingleEmitter(pEmitter, pCallBackBase, nullptr);
    }
}

void MultiEmitter::deleteEmitter() {
    for_each_array(mEmitters.begin(), mEmitters.end(), std::mem_fun_t< void, SingleEmitter >(&SingleEmitter::deleteEmitter));
    for_each(mChildren.begin(), mChildren.end(), std::mem_fun_t< void, MultiEmitter >(&MultiEmitter::deleteEmitter));
}

void MultiEmitter::forceDeleteEmitter() {
    for (SingleEmitter* pEmitter = mEmitters.begin(); pEmitter != mEmitters.end(); pEmitter++) {
        MR::getEffectSystem()->forceDeleteSingleEmitter(pEmitter);
    }

    for_each(mChildren.begin(), mChildren.end(), std::mem_fun_t< void, MultiEmitter >(&MultiEmitter::forceDeleteEmitter));
}

void MultiEmitter::forceDelete(EffectSystem* pSystem) {
    for (SingleEmitter* pEmitter = mEmitters.begin(); pEmitter != mEmitters.end(); pEmitter++) {
        pSystem->forceDeleteSingleEmitter(pEmitter);
    }
}

void MultiEmitter::deleteForeverEmitter() {
    for (SingleEmitter* pEmitter = mEmitters.begin(); pEmitter != mEmitters.end(); pEmitter++) {
        if (pEmitter->isValid() && !pEmitter->isOneTime()) {
            pEmitter->deleteEmitter();
        }
    }

    for_each(mChildren.begin(), mChildren.end(), std::mem_fun_t< void, MultiEmitter >(&MultiEmitter::deleteForeverEmitter));
}

void MultiEmitter::playCalcAndDeleteForeverEmitter() {
    playCalcEmitter(-1);
    deleteForeverEmitter();
}

bool MultiEmitter::isValid() const {
    for (const SingleEmitter* pEmitter = mEmitters.begin(); pEmitter != mEmitters.end(); pEmitter++) {
        if (pEmitter->isValid()) {
            return true;
        }
    }

    return false;
}

bool MultiEmitter::isExistOneTimeEmitter() const {
    for (const SingleEmitter* pEmitter = mEmitters.begin(); pEmitter != mEmitters.end(); pEmitter++) {
        if (pEmitter->isOneTime()) {
            return true;
        }
    }

    return false;
}

void MultiEmitter::setHostSRT(const TVec3f* pScale, const TVec3f* pRotation, const TVec3f* pPosition) {
    mCallBack->setHostSRT(pScale, pRotation, pPosition);
}

void MultiEmitter::setHostMtx(MtxPtr pMtx) {
    mCallBack->setHostMtx(pMtx);
}

void MultiEmitter::setOffset(const TVec3f& rOffet) {
    mCallBack->_18.set(rOffet);
}

void MultiEmitter::setBaseScale(f32 scale) {
    mCallBack->setBaseScale(scale);
}

void MultiEmitter::setName(const char* pName) {
    mHash = MR::getHashCode(pName);
}

bool MultiEmitter::isEqualName(u16 hash) const {
    return mHash == hash;
}

ParticleEmitter* MultiEmitter::getParticleEmitter(int idx) const {
    const SingleEmitter* pEmitter = &mEmitters[idx];
    if (pEmitter->isValid()) {
        return pEmitter->mEmitter;
    }

    return nullptr;
}

void MultiEmitter::create(EffectSystem* pSystem) {
    for (SingleEmitter* pEmitter = mEmitters.begin(); pEmitter != mEmitters.end(); pEmitter++) {
        pSystem->createSingleEmitter(pEmitter, mCallBack, mParticleCallBack);
    }

    mCallBack->resetFollowCurrent();

    for (MultiEmitter** pEmitter = mChildren.begin(); pEmitter != mChildren.end(); pEmitter++) {
        (*pEmitter)->create(pSystem);
    }
}

/* void MultiEmitter::scanParticleEmitter(EffectSystem* pSystem) {
} */

void MultiEmitter::forceFollowOn() {
    mCallBack->forceFollowOn();
}

void MultiEmitter::forceFollowOff() {
    mCallBack->forceFollowOff();
}

void MultiEmitter::forceScaleOn() {
    mCallBack->forceScaleOn();
}

/*
void MultiEmitter::initSyncBck(XanimePlayer* pPlayer, const char* pName, s32 i1, f32 f2) {
}

void MultiEmitter::onDeleteSyncBck(bool, f32) {
}

void MultiEmitter::addSyncBck(const XanimePlayer*, const char*) {
}

void MultiEmitter::setContinueBckEnd(bool) {
}
 */
void MultiEmitter::onCreateSyncClipping() {
    turnFlagOn(JPAEmtrStts_FirstEmit);
}

void MultiEmitter::onForceDeleteSyncClipping() {
    turnFlagOn(JPAEmtrStts_Immortal);
}

void MultiEmitter::stopEmitterOnClipped() {
    if (isFlagOn(JPAEmtrStts_Immortal)) {
        forceDeleteEmitter();
    } else if (isFlagOn(JPAEmtrStts_RateStepEmit)) {
        deleteEmitter();
    } else if (_24 != nullptr) {
        stopCalcEmitter(-1);
        stopDrawParticle(-1);
    }

    for (SingleEmitter* pEmitter = mEmitters.begin(); pEmitter != mEmitters.end(); pEmitter++) {
        if (pEmitter->isValid() && !pEmitter->isOneTime()) {
            pEmitter->mEmitter->mEmitter->setStatus(JPAEmtrStts_StopCalc);
            pEmitter->mEmitter->mEmitter->setStatus(JPAEmtrStts_StopDraw);
        }
    }
}

void MultiEmitter::playEmitterOffClipped() {
    if (isFlagOn(JPAEmtrStts_FirstEmit)) {
        create(MR::getEffectSystem());
    } else if (_24 != nullptr) {
        playCalcEmitter(-1);
        playDrawParticle(-1);
    }

    for (SingleEmitter* pEmitter = mEmitters.begin(); pEmitter != mEmitters.end(); pEmitter++) {
        if (pEmitter->isValid() && !pEmitter->isOneTime()) {
            pEmitter->mEmitter->mEmitter->clearStatus(JPAEmtrStts_StopCalc);
            pEmitter->mEmitter->mEmitter->clearStatus(JPAEmtrStts_StopDraw);
        }
    }
}
/*
void MultiEmitter::setDrawOrder(s32 idx) {
}
 */
void MultiEmitter::addChildEmitter(MultiEmitter* pChild) {
    mChildren.push_back(pChild);
}

void MultiEmitter::setGlobalRotationDegree(const TVec3f& rRotation, s32 idx) {
    setGlobalRotation(TVec3s(rRotation.x * DEGREE_TO_S16, rRotation.y * DEGREE_TO_S16, rRotation.z * DEGREE_TO_S16), idx);
}

/* void MultiEmitter::allocateEmitter(const char* pName) {
} */

SingleEmitter* MultiEmitter::getValidEmitter(s32 idx, bool) {
    SingleEmitter* pEmitter = &mEmitters[idx];
    if (pEmitter->isValid()) {
        return pEmitter;
    }

    return nullptr;
}

void MultiEmitter::createOneTimeEmitter() {
    for (SingleEmitter* pEmitter = mEmitters.begin(); pEmitter != mEmitters.end(); pEmitter++) {
        if (pEmitter->isOneTime()) {
            MR::getEffectSystem()->createSingleEmitter(pEmitter, mCallBack, nullptr);
        }
    }
}

void MultiEmitter::createForeverEmitter() {
    for (SingleEmitter* pEmitter = mEmitters.begin(); pEmitter != mEmitters.end(); pEmitter++) {
        if (!pEmitter->isOneTime()) {
            MR::getEffectSystem()->createSingleEmitter(pEmitter, mCallBack, nullptr);
        }
    }
}

void SingleEmitter::setGroupID(u8 groupId) {
    mGroupId = groupId;
}
