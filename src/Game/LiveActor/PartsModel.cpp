#include "Game/LiveActor/PartsModel.hpp" 
#include "Game/NameObj/NameObjExecuteHolder.hpp"

PartsModel::PartsModel(LiveActor *pActor, const char *pName, const char *pModelName, MtxPtr mtx, int a5, bool a6) : LiveActor(pName) {
    mHost = pActor;
    mFixedPos = nullptr;
    mMtx = mtx;
    mCalcOwnMtx = true;
    _99 = false;
    mIsDead = false;

    if (a5 > 0) {
        a5 = 8;
    }

    if (mMtx) {
        mPosition.set(mMtx[0][3], mMtx[1][3], mMtx[2][3]);
    }
    else {
        mPosition.set<f32>(pActor->mPosition);
    }

    mRotation.set<f32>(pActor->mRotation);
    mScale.set<f32>(pActor->mScale);

    initModelManagerWithAnm(pModelName, nullptr, a6);

    u32 v14 = 43;
    u32 v15 = 11;

    if ((a5 - 21) <= (u32)2) {
        v14 = 38;
        v15 = 10;
    }

    if (a5 == 16 || a5 == 27) {
        v14 = 40;
        v15 = 6;
    }
    MR::connectToScene(this, v14, v15, a5, -1);
}

void PartsModel::makeActorAppeared() {
    LiveActor::makeActorAppeared();
    mIsDead = false;
}

void PartsModel::makeActorDead() {
    LiveActor::makeActorDead();
    mIsDead = true;
}

// 99.9% sure the call to makeActorAppered is meant to be inlined but ¯\_(ツ)_/¯ this matches
void PartsModel::init(const JMapInfoIter &rIter) {
    initEffectKeeper(8, nullptr, false);
    initSound(8, false);
    MR::isInvalidClipping(this);
    makeActorAppeared();
    mIsDead = false;
}

void PartsModel::movement() {
    if (MR::isDead(this) || MR::isDead(mHost) || MR::isClipped(mHost) || !_99 && MR::isHiddenModel(mHost)) {
        if (!mIsDead) {
            mIsDead = true;
            MR::invalidateHitSensors(this);

            if (!MR::isHiddenModel(this)) {
                MR::disconnectToDrawTemporarily(this);
            }
        }
    }
    else {
        if (mIsDead) {
            mIsDead = false;
            MR::validateHitSensors(this);

            if (!MR::isHiddenModel(this)) {
                MR::connectToDrawTemporarily(this);
            }
        }
        LiveActor::movement();
    }
}

#ifdef NON_MATCHING
void PartsModel::calcAnim() {
    if (mIsDead || MR::isDead(this) || MR::isNoCalcAnim(this) || MR::isClipped(this)) {
        return;
    }

    if (mFixedPos) {
        mFixedPos->calc();
    }
    LiveActor::calcAnim();
}
#endif

void PartsModel::calcViewAndEntry() {
    if (!mIsDead) {
        LiveActor::calcViewAndEntry();
    }
}

void PartsModel::initFixedPosition(const TVec3f &rLocalTrans, const TVec3f &a2, const char *pJointName) {
    if (pJointName) {
        mFixedPos = new FixedPosition(mHost, pJointName, rLocalTrans, a2);
    }
    else {
        mFixedPos = new FixedPosition(mHost, rLocalTrans, a2);
    }
    mMtx = (MtxPtr)&mFixedPos->_1C;
}

void PartsModel::initFixedPosition(const char *pJointName) {
    initFixedPosition(TVec3f(0.0f, 0.0f, 0.0f), TVec3f(0.0f, 0.0f, 0.0f), pJointName);
}

void PartsModel::initFixedPosition(MtxPtr mtx, const TVec3f &rLocalTrans, const TVec3f &a3) {
    mFixedPos = new FixedPosition(mtx, rLocalTrans, a3);
    mMtx = (MtxPtr)&mFixedPos->_1C;
}

void PartsModel::loadFixedPosition(const char *pJointName) {
    mFixedPos = new FixedPosition(mHost, pJointName, nullptr);
    mMtx = (MtxPtr)&mFixedPos->_1C;
}

void PartsModel::offFixedPosNormalizeScale() {
    mFixedPos->mNormalizeScale = false;
}

void PartsModel::calcAndSetBaseMtx() {
    if (mMtx && mCalcOwnMtx) {
        mPosition.set(mMtx[0][3], mMtx[1][3], mMtx[2][3]);
        MR::setBaseTRMtx(this, mMtx);
    }
    else {
        LiveActor::calcAndSetBaseMtx();
    }
}
