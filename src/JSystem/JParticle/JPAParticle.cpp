#include "JSystem/JParticle/JPAParticle.hpp"
#include "JSystem/JParticle/JPABaseShape.hpp"
#include "JSystem/JParticle/JPAChildShape.hpp"
#include "JSystem/JParticle/JPAEmitter.hpp"
#include "JSystem/JParticle/JPAEmitterManager.hpp"
#include "JSystem/JParticle/JPAExtraShape.hpp"

JPAParticleCallBack::~JPAParticleCallBack() {
}

void JPABaseParticle::init_p(JPAEmitterWorkData* pWork) {
    JPABaseEmitter* emtr = pWork->mpEmtr;
    JPAExtraShape* esp = pWork->mpRes->mpExtraShape;
    JPABaseShape* bsp = pWork->mpRes->mpBaseShape;
    JPADynamicsBlock* dyn = pWork->mpRes->mpDynamicsBlock;

    mAge = -1;
    mLifeTime = (1.0f - dyn->getLifetimeRndm() * emtr->get_r_f()) * emtr->mLifeTime;
    mTime = 0.0f;

    initStatus(0);
    PSMTXMultVecSR(pWork->mGlobalSR, &pWork->mVolumeCalcData.mVolumePos, &mLocalPosition);

    if (emtr->checkFlag(8)) {
        setStatus(0x20);
    }

    mOffsetPosition.set(pWork->mGlobalPos);
    mPosition.set< f32 >(mOffsetPosition.x + mLocalPosition.x * pWork->mPublicScale.x, mOffsetPosition.y + mLocalPosition.y * pWork->mPublicScale.y,
                         mOffsetPosition.z + mLocalPosition.z * pWork->mPublicScale.z);

    JGeometry::TVec3< f32 > velOmni;

    if (emtr->mAwayFromCenterSpeed) {
        velOmni.setLength(pWork->mVolumeCalcData.mVelOmni, emtr->mAwayFromCenterSpeed);
    } else {
        velOmni.zero();
    }

    JGeometry::TVec3< f32 > velAxis;

    if (emtr->mAwayFromAxisSpeed) {
        velAxis.setLength(pWork->mVolumeCalcData.mVelAxis, emtr->mAwayFromAxisSpeed);
    } else {
        velAxis.zero();
    }

    JGeometry::TVec3< f32 > velDir;

    if (emtr->mDirSpeed) {
        Mtx mtx;
        JPAGetYZRotateMtx(emtr->get_r_zp() * 0x8000 * emtr->mSpread, emtr->mRndm.get_rndm_ss(), mtx);
        PSMTXConcat(pWork->mDirectionMtx, mtx, mtx);
        velDir.set< f32 >(emtr->mDirSpeed * mtx[0][2], emtr->mDirSpeed * mtx[1][2], emtr->mDirSpeed * mtx[2][2]);
    } else {
        velDir.zero();
    }

    JGeometry::TVec3< f32 > velRndm;

    if (emtr->mRndmDirSpeed) {
        f32 z = emtr->mRndm.get_rndm_zh();
        f32 y = emtr->mRndm.get_rndm_zh();
        f32 x = emtr->mRndm.get_rndm_zh();
        velRndm.set< f32 >(emtr->mRndmDirSpeed * x, emtr->mRndmDirSpeed * y, emtr->mRndmDirSpeed * z);
    } else {
        velRndm.zero();
    }

    f32 ratio = emtr->get_r_zp() * dyn->getInitVelRatio() + 1.0f;
    mVelType1.set< f32 >(ratio * (velOmni.x + velAxis.x + velDir.x + velRndm.x), ratio * (velOmni.y + velAxis.y + velDir.y + velRndm.y),
                         ratio * (velOmni.z + velAxis.z + velDir.z + velRndm.z));

    if (emtr->checkFlag(4)) {
        mVelType1.mul(emtr->mLocalScl);
    }

    PSMTXMultVecSR(pWork->mGlobalRot, &mVelType1, &mVelType1);

    mVelType0.zero();

    mMoment = 1.0f - dyn->getMomentRndm() * emtr->get_r_f();
    mDrag = 1.0f;
    mUserWork = 0;

    mBaseAxis.set< f32 >(pWork->mGlobalRot[0][1], pWork->mGlobalRot[1][1], pWork->mGlobalRot[2][1]);

    mPrmClr = emtr->mPrmClr;
    mEnvClr = emtr->mEnvClr;

    mAnmRandom = emtr->get_r_f() * bsp->getLoopOfstValue();

    if (esp != nullptr && esp->isEnableScaleAnm()) {
        mParticleScaleX = mParticleScaleY = mScaleOut = emtr->mScaleOut * (emtr->get_r_zp() * esp->getScaleRndm() + 1.0f);
    } else {
        mParticleScaleX = mParticleScaleY = mScaleOut = emtr->mScaleOut;
    }

    mPrmColorAlphaAnm = 0xff;

    if (esp != nullptr && esp->isEnableAlphaFlick()) {
        mAlphaWaveRandom = emtr->get_r_zp() * esp->getAlphaFreqRndm() + 1.0f;
    } else {
        mAlphaWaveRandom = 1.0f;
    }

    if (esp != nullptr) {
        if (esp->isEnableRotateAnm()) {
            mRotateAngle = esp->getRotateInitAngle() + esp->getRotateRndmAngle() * emtr->mRndm.get_rndm_zh();
            mRotateSpeed = esp->getRotateInitSpeed() * (esp->getRotateRndmSpeed() * emtr->get_r_zp() + 1.0f);
            mRotateSpeed = emtr->get_r_zp() < esp->getRotateDirection() ? mRotateSpeed : (s16)-mRotateSpeed;
        } else {
            mRotateAngle = 0;
            mRotateSpeed = 0;
        }
    } else {
        mRotateAngle = 0;
        mRotateSpeed = 0;
    }
}

void JPABaseParticle::init_c(JPAEmitterWorkData* pWork, JPABaseParticle* pParent) {
    JPABaseEmitter* emtr = pWork->mpEmtr;
    JPAChildShape* csp = pWork->mpRes->getCsp();

    mAge = -1;
    mLifeTime = csp->getLife();
    mTime = 0.0f;
    initStatus(4);
    mLocalPosition.set(pParent->mLocalPosition);

    f32 pos_rndm = csp->getPosRndm();

    if (pos_rndm != 0.0f) {
        JGeometry::TVec3< f32 > rnd(emtr->mRndm.get_rndm_zh(), emtr->mRndm.get_rndm_zh(), emtr->mRndm.get_rndm_zh());
        rnd.setLength(pos_rndm * emtr->get_r_f());
        mLocalPosition.add(rnd);
    }

    if (emtr->checkFlag(0x10)) {
        setStatus(0x20);
    }

    mOffsetPosition.set(pParent->mOffsetPosition);

    f32 base_speed = csp->getBaseVel() * (csp->getBaseVelRndm() * emtr->get_r_zp() + 1.0f);
    JGeometry::TVec3< f32 > base_vel(emtr->get_r_zp(), emtr->get_r_zp(), emtr->get_r_zp());
    base_vel.setLength(base_speed);
    mVelType1.scaleAdd(csp->getVelInhRate(), pParent->mVelType1, base_vel);
    mVelType0.scale(csp->getVelInhRate(), pParent->mVelType2);

    mMoment = pParent->mMoment;

    if (csp->isFieldAffected()) {
        mDrag = pParent->mDrag;
    } else {
        setStatus(0x40);
        mDrag = 1.0f;
    }

    mUserWork = pParent->mUserWork;

    mVelType2.set(mVelType0);
    f32 ratio = mMoment * mDrag;
    mVelocity.set< f32 >(ratio * (mVelType1.x + mVelType2.x), ratio * (mVelType1.y + mVelType2.y), ratio * (mVelType1.z + mVelType2.z));

    mBaseAxis.set(pParent->mBaseAxis);

    if (csp->isScaleInherited()) {
        mParticleScaleX = mScaleOut = pParent->mParticleScaleX * csp->getScaleInhRate();
        mParticleScaleY = mAlphaWaveRandom = pParent->mParticleScaleY * csp->getScaleInhRate();
    } else {
        mParticleScaleX = mScaleOut = mParticleScaleY = mAlphaWaveRandom = 1.0f;
    }

    if (csp->isColorInherited()) {
        mPrmClr.r = pParent->mPrmClr.r * csp->getColorInhRate();
        mPrmClr.g = pParent->mPrmClr.g * csp->getColorInhRate();
        mPrmClr.b = pParent->mPrmClr.b * csp->getColorInhRate();
        mEnvClr.r = pParent->mEnvClr.r * csp->getColorInhRate();
        mEnvClr.g = pParent->mEnvClr.g * csp->getColorInhRate();
        mEnvClr.b = pParent->mEnvClr.b * csp->getColorInhRate();
    } else {
        csp->getPrmClr(&mPrmClr);
        csp->getEnvClr(&mEnvClr);
    }

    mPrmColorAlphaAnm = 0xff;
    if (csp->isAlphaInherited()) {
        mPrmClr.a = COLOR_MULTI(pParent->mPrmClr.a, pParent->mPrmColorAlphaAnm) * csp->getAlphaInhRate();
    } else {
        mPrmClr.a = csp->getPrmAlpha();
    }

    mRotateAngle = pParent->getRotateAngle();
    if (csp->isRotateOn()) {
        mRotateSpeed = csp->getRotInitSpeed();
    } else {
        mRotateSpeed = 0;
    }

    mTexAnmIdx = 0;
}

bool JPABaseParticle::calc_p(JPAEmitterWorkData* pWork) {
    if (++mAge >= mLifeTime) {
        return true;
    }

    mTime = (f32)mAge / (f32)mLifeTime;

    if (checkStatus(0x20)) {
        mOffsetPosition.set(pWork->mGlobalPos);
    }

    mVelType2.zero();
    if (!checkStatus(0x40)) {
        pWork->mpRes->calcField(pWork, this);
    }

    mVelType2.add(mVelType0);
    mVelType1.scale(pWork->mpEmtr->mAirResist);
    f32 ratio = mMoment * mDrag;
    mVelocity.set< f32 >(ratio * (mVelType1.x + mVelType2.x), ratio * (mVelType1.y + mVelType2.y), ratio * (mVelType1.z + mVelType2.z));

    if (pWork->mpEmtr->mpPtclCallBack != nullptr) {
        pWork->mpEmtr->mpPtclCallBack->execute(pWork->mpEmtr, this);
    }

    if (checkStatus(2)) {
        return true;
    }

    pWork->mpRes->calc_p(pWork, this);
    u16 angle = getRotateAngle();
    mRotateAngle = angle + mRotateSpeed;

    if (pWork->mpRes->getCsp() != nullptr && canCreateChild(pWork)) {
        for (int i = pWork->mpRes->getCsp()->getRate(); i > 0; i--) {
            pWork->mpEmtr->createChild(this);
        }
    }

    mLocalPosition.add(mVelocity);
    mPosition.set< f32 >(mOffsetPosition.x + mLocalPosition.x * pWork->mPublicScale.x, mOffsetPosition.y + mLocalPosition.y * pWork->mPublicScale.y,
                         mOffsetPosition.z + mLocalPosition.z * pWork->mPublicScale.z);

    return false;
}

bool JPABaseParticle::calc_c(JPAEmitterWorkData* pWork) {
    if (++mAge >= mLifeTime) {
        return true;
    }

    mTime = (f32)mAge / (f32)mLifeTime;

    if (mAge != 0) {
        if (checkStatus(0x20)) {
            mOffsetPosition.set(pWork->mGlobalPos);
        }

        mVelType1.y -= pWork->mpRes->getCsp()->getGravity();
        mVelType2.zero();
        if (!checkStatus(0x40)) {
            pWork->mpRes->calcField(pWork, this);
        }

        mVelType2.add(mVelType0);
        mVelType1.scale(pWork->mpEmtr->mAirResist);
        f32 ratio = mMoment * mDrag;
        mVelocity.set< f32 >(ratio * (mVelType1.x + mVelType2.x), ratio * (mVelType1.y + mVelType2.y), ratio * (mVelType1.z + mVelType2.z));
    }

    if (pWork->mpEmtr->mpPtclCallBack != nullptr) {
        pWork->mpEmtr->mpPtclCallBack->execute(pWork->mpEmtr, this);
    }

    if (checkStatus(2)) {
        return true;
    }

    pWork->mpRes->calc_c(pWork, this);
    u16 angle = getRotateAngle();
    mRotateAngle = angle + mRotateSpeed;
    mLocalPosition.add(mVelocity);
    mPosition.set< f32 >(mOffsetPosition.x + mLocalPosition.x * pWork->mPublicScale.x, mOffsetPosition.y + mLocalPosition.y * pWork->mPublicScale.y,
                         mOffsetPosition.z + mLocalPosition.z * pWork->mPublicScale.z);

    return false;
}

bool JPABaseParticle::canCreateChild(JPAEmitterWorkData* pWork) {
    JPAChildShape* csp = pWork->mpRes->getCsp();
    bool ret = false;
    int time = mAge - (int)((mLifeTime - 1) * csp->getTiming());
    if (time >= 0 && time % (csp->getStep() + 1) == 0) {
        ret = true;
    }

    return ret;
}

/* NEW IN SMG1 */
f32 JPABaseParticle::getCalcCurrentPositionX(const JPABaseEmitter* pEmitter) const {
    return (mOffsetPosition.x + (pEmitter->mpEmtrMgr->pWd->mPublicScale.x) * (mLocalPosition.x + mVelocity.x));
}

f32 JPABaseParticle::getCalcCurrentPositionY(const JPABaseEmitter* pEmitter) const {
    return (mOffsetPosition.y + (pEmitter->mpEmtrMgr->pWd->mPublicScale.y) * (mLocalPosition.y + mVelocity.y));
}

f32 JPABaseParticle::getCalcCurrentPositionZ(const JPABaseEmitter* pEmitter) const {
    return (mOffsetPosition.z + (pEmitter->mpEmtrMgr->pWd->mPublicScale.z) * (mLocalPosition.z + mVelocity.z));
}
