#include "Game/Effect/MultiEmitterCallBack.hpp"
#include "Game/Effect/EffectSystemUtil.hpp"
#include "Game/Effect/MultiEmitter.hpp"
#include "Game/Util/MathUtil.hpp"
#include "Game/Util/MtxUtil.hpp"
#include <JSystem/JParticle/JPAEmitter.hpp>

namespace {
    Color8 getSyntheticColor(const Color8& rColor1, const Color8& rColor2) {
        u8 r = rColor1.r * rColor2.r / 255;
        u8 g = rColor1.g * rColor2.g / 255;
        u8 b = rColor1.b * rColor2.b / 255;
        
        return Color8(r, g, b, 255);
    }
}

MultiEmitterCallBack::MultiEmitterCallBack(const MultiEmitter* pEmitter, const TVec3f& rVec)
    : MultiEmitterCallBackBase(), mEmitter(pEmitter), mScale(), mRotation(), mTranslation(), mMtx(), _18(rVec), mBaseScale(1.0f), mPrmColor(-1, -1, -1, -1), mEnvColor(-1, -1, -1, -1), mFlags() {
}

MultiEmitterCallBack::MultiEmitterCallBack(const MultiEmitter* pEmitter, const TVec3f* pScale, const TVec3f* pRotation, const TVec3f* pTranslation,
                                           const TVec3f& rVec)
    : MultiEmitterCallBackBase(), mEmitter(pEmitter), mScale(pScale), mRotation(pRotation), mTranslation(pTranslation), mMtx(), _18(rVec), mBaseScale(1.0f), mPrmColor(-1, -1, -1, -1), mEnvColor(-1, -1, -1, -1),
      mFlags() {
}

MultiEmitterCallBack::MultiEmitterCallBack(const MultiEmitter* pEmitter, MtxPtr pMtx, const TVec3f& rVec)
    : MultiEmitterCallBackBase(), mEmitter(pEmitter), mScale(), mRotation(), mTranslation(), mMtx(pMtx), _18(rVec), mBaseScale(1.0f), mPrmColor(-1, -1, -1, -1), mEnvColor(-1, -1, -1, -1), mFlags() {
}

MultiEmitterCallBack::MultiEmitterCallBack(const MultiEmitter* pEmitter, MtxPtr pMtx, const TVec3f* pTranslation, const TVec3f& rVec)
    : MultiEmitterCallBackBase(), mEmitter(pEmitter), mScale(), mRotation(), mTranslation(pTranslation), mMtx(pMtx), _18(rVec), mBaseScale(1.0f), mPrmColor(-1, -1, -1, -1), mEnvColor(-1, -1, -1, -1),
      mFlags() {
}

void MultiEmitterCallBack::execute(JPABaseEmitter* pEmitter) {
    followSRT(pEmitter, false);
    effectLight(pEmitter);
    setColor(pEmitter);
}

void MultiEmitterCallBack::setHostSRT(const TVec3f* pScale, const TVec3f* pRotation, const TVec3f* pTranslation) {
    mScale = pScale;
    mRotation = pRotation;
    mTranslation = pTranslation;
    mMtx = nullptr;
}

void MultiEmitterCallBack::setHostMtx(MtxPtr pMtx) {
    mMtx = pMtx;
    mScale = nullptr;
    mRotation = nullptr;
    mTranslation = nullptr;
}

void MultiEmitterCallBack::setBaseScale(f32 scale) {
    if (isFlagOff(BASE_SCALE)) {
        turnFlagOn(BASE_SCALE);
    }

    mBaseScale = scale;
}

void MultiEmitterCallBack::forceFollowOn() {
    turnFlagOn(FORCE_FOLLOW_ON);
}

void MultiEmitterCallBack::forceFollowOff() {
    turnFlagOn(FORCE_FOLLOW_OFF);
}

void MultiEmitterCallBack::forceScaleOn() {
    turnFlagOn(FORCE_SCALE_ON);
}

void MultiEmitterCallBack::resetFollowCurrent() {
    turnFlagOff(RESET_FOLLOW_CURRENT);
}

void MultiEmitterCallBack::init(JPABaseEmitter* pEmitter) {
    followSRT(pEmitter, true);
}

void MultiEmitterCallBack::setSRTFromHostMtx(JPABaseEmitter* pEmitter, MtxPtr pMtx, const FlagSRT& rFlag, bool b2) {
    TPos3f mtx = pMtx;
    TRot3f mtx94;
    TVec3f vecD0;
    TVec3f vecDC;
    JPASetRMtxSTVecfromMtx(mtx, mtx94, &vecDC, &vecD0);

    if (rFlag.mScale) {
        TVec3f vecE8;
        mtx94.mult33(_18, vecE8);

        if (rFlag.mTranslation) {
            vecE8.mul(vecDC);
        }

        vecD0.add(vecE8);
        pEmitter->mGlobalTrs.set(vecD0);
    }

    if (rFlag.mRotation) {
        if (MR::Effect::isEffect2D(mEmitter)) {
            TRot3f mtxC4;
            mtxC4.identity();
            mtxC4.setEulerZ(PI);
            mtx94.concat(mtxC4);
        }

        JPASetRMtxfromMtx(mtx94, pEmitter->mGlobalRot);
    }

    setScaleFromHostScale(pEmitter, vecDC, rFlag.mTranslation, b2);
}

void MultiEmitterCallBack::setSRTFromHostSRT(JPABaseEmitter* pEmitter, const FlagSRT& rFlag, bool b2) {
    if (rFlag.mScale) {
        TVec3f vecE0;
        if (rFlag.mRotation) {
            TRot3f mtxD4;
            mtxD4.identity();

            TVec3f vecEC(*mRotation);
            vecEC.scale(PI_180);
            mtxD4.makeMatrixFromRotAxesInline(vecEC.x, vecEC.y, vecEC.z);
            mtxD4.mult33(_18, vecE0);
        } else {
            vecE0.set(_18);
        }

        if (rFlag.mTranslation && mTranslation != nullptr) {
            vecE0.mul(*mTranslation);
        }

        vecE0.add(*mScale);

        pEmitter->mGlobalTrs.set(vecE0);
    }

    if (rFlag.mRotation) {
        MR::makeMtxRotate(pEmitter->mGlobalRot, mRotation->x, mRotation->y, mRotation->z);
    }

    setScaleFromHostScale(pEmitter, TVec3f(1.0f, 1.0f, 1.0f), rFlag.mTranslation, b2);
}

void MultiEmitterCallBack::setScaleFromHostScale(JPABaseEmitter* pEmitter, const TVec3f& rVec, bool b1, bool b2) {
    TVec3f vec28(rVec);

    if (b1) {
        if (mTranslation != nullptr) {
            vec28.set(*mTranslation);
        }

        if (isFlagOn(BASE_SCALE)) {
            vec28.scale(mBaseScale);
        }
        pEmitter->setGlobalScale(vec28);
    } else if (b2 && isFlagOn(BASE_SCALE)) {
        vec28.setAll< f32 >(mBaseScale);
        pEmitter->setGlobalScale(vec28);
    }
}

bool MultiEmitterCallBack::effectLight(JPABaseEmitter*) {
    return MR::isNearZero(mEmitter->_2C);
}

void MultiEmitterCallBack::followSRT(JPABaseEmitter* pEmitter, bool b2) {
    FlagSRT flag;
    isFollowSRT(&flag, b2);

    if (flag.isTransform() || (isFlagOn(BASE_SCALE))) {
        if (mMtx != nullptr) {
            setSRTFromHostMtx(pEmitter, mMtx, flag, b2);
        } else {
            setSRTFromHostSRT(pEmitter, flag, b2);
        }
    }
}

void MultiEmitterCallBack::setColor(JPABaseEmitter* pEmitter) {
    // FIXME: something goes wrong with GXColor/Color8
    if (MR::isNearZero(mEmitter->_2C)) {
        pEmitter->setGlobalPrmColor(mPrmColor.r, mPrmColor.g, mPrmColor.b);
        pEmitter->setGlobalEnvColor(mEnvColor.r, mEnvColor.g, mEnvColor.b);

        return;
    } 

    GXColor colorC = pEmitter->mGlobalPrmClr;
    GXColor color10 = pEmitter->mGlobalEnvClr;

    GXColor col = getSyntheticColor(colorC, color10);

    pEmitter->setGlobalPrmColor(col.r, col.g, col.g);

    col = getSyntheticColor(colorC, mEnvColor);
    pEmitter->setGlobalEnvColor(col.r, col.g, col.b);

}

void MultiEmitterCallBack::isFollowSRT(FlagSRT* rFlag, bool b2) const {
    // FIXME: missing class at _28 in MultiEmitter

    if (mMtx == nullptr && mScale == nullptr) {
        rFlag->mScale = false;
    } else if (rFlag->mScale == false) {
        rFlag->mScale = !((mFlags & 0x1) - 0x1);
    }

    if (mMtx == nullptr && mRotation == nullptr) {
        rFlag->mRotation = false;
    } else if (rFlag->mRotation == false) {
        rFlag->mRotation = !((mFlags & 0x4) - 0x4);
    }

    if (mMtx == nullptr && mTranslation == nullptr) {
        rFlag->mTranslation = false;

        return;
    }

    if (rFlag->mTranslation) {
        return;
    }

    rFlag->mTranslation = !((mFlags & 0x10) - 0x10);
}
