#include "Game/LiveActor/PartsModel.hpp"
#include "Game/NameObj/NameObjExecuteHolder.hpp"
#include "Game/Scene/SceneFunction.hpp"
#include "Game/Util/ActorSensorUtil.hpp"
#include "Game/Util/FixedPosition.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/ObjUtil.hpp"

PartsModel::PartsModel(LiveActor* pActor, const char* pName, const char* pModelName, MtxPtr mtx, int drawBufferType, bool a6)
    : LiveActor(pName), mHost(pActor), mFixedPosition(), mMtx(mtx), mIsCalcOwnMtx(true), _99(), mIsDead() {
    if (drawBufferType < 0) {
        drawBufferType = MR::DrawBufferType_MapObj;
    }

    if (mMtx != nullptr) {
        mPosition.set< f32 >(mMtx[0][3], mMtx[1][3], mMtx[2][3]);
    } else {
        mPosition.set< f32 >(pActor->mPosition);
    }

    mRotation.set< f32 >(pActor->mRotation);
    mScale.set< f32 >(pActor->mScale);

    initModelManagerWithAnm(pModelName, nullptr, a6);

    u32 movementType = MR::MovementType_EnemyDecoration;
    u32 calcAnimType = MR::CalcAnimType_MapObjDecoration;

    if (drawBufferType == MR::DrawBufferType_PlayerDecoration || drawBufferType == MR::DrawBufferType_CrystalBox ||
        drawBufferType == MR::DrawBufferType_UNK_0x17) {
        movementType = MR::MovementType_PlayerDecoration;
        calcAnimType = MR::CalcAnimType_PlayerDecoration;
    }

    if (drawBufferType == MR::DrawBufferType_NPC || drawBufferType == MR::DrawBufferType_IndirectNpc) {
        movementType = MR::MovementType_NPC;
        calcAnimType = MR::CalcAnimType_NPC;
    }

    MR::connectToScene(this, movementType, calcAnimType, drawBufferType, -1);
}

void PartsModel::makeActorAppeared() {
    LiveActor::makeActorAppeared();

    mIsDead = false;
}

void PartsModel::makeActorDead() {
    LiveActor::makeActorDead();

    mIsDead = true;
}

void PartsModel::init(const JMapInfoIter& rIter) {
    initEffectKeeper(8, nullptr, false);
    initSound(8, false);
    MR::invalidateClipping(this);
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
    } else {
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

void PartsModel::calcAnim() {
    if (mIsDead || MR::isDead(this) || MR::isNoCalcAnim(this) || MR::isClipped(this)) {
        return;
    }

    if (isInitFixedPosition()) {
        mFixedPosition->calc();
    }

    LiveActor::calcAnim();
}

void PartsModel::calcViewAndEntry() {
    if (mIsDead) {
        return;
    }

    LiveActor::calcViewAndEntry();
}

void PartsModel::initFixedPosition(const TVec3f& rLocalTrans, const TVec3f& rLocalRotate, const char* pJointName) {
    if (pJointName != nullptr) {
        mFixedPosition = new FixedPosition(mHost, pJointName, rLocalTrans, rLocalRotate);
    } else {
        mFixedPosition = new FixedPosition(mHost, rLocalTrans, rLocalRotate);
    }

    mMtx = mFixedPosition->mMtx;
}

void PartsModel::initFixedPosition(const char* pJointName) {
    initFixedPosition(TVec3f(0.0f, 0.0f, 0.0f), TVec3f(0.0f, 0.0f, 0.0f), pJointName);
}

void PartsModel::initFixedPosition(MtxPtr pMtx, const TVec3f& rLocalTrans, const TVec3f& rLocalRotate) {
    mFixedPosition = new FixedPosition(pMtx, rLocalTrans, rLocalRotate);
    mMtx = mFixedPosition->mMtx;
}

void PartsModel::loadFixedPosition(const char* pJointName) {
    mFixedPosition = new FixedPosition(mHost, pJointName, nullptr);
    mMtx = mFixedPosition->mMtx;
}

void PartsModel::offFixedPosNormalizeScale() {
    mFixedPosition->mNormalizeScale = false;
}

void PartsModel::calcAndSetBaseMtx() {
    if (mMtx != nullptr && mIsCalcOwnMtx) {
        mPosition.set(mMtx[0][3], mMtx[1][3], mMtx[2][3]);
        MR::setBaseTRMtx(this, mMtx);
    } else {
        LiveActor::calcAndSetBaseMtx();
    }
}
