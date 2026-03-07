#include "Game/Player/MarioSlider.hpp"
#include "Game/Enemy/KariKariDirector.hpp"
#include "Game/Player/Mario.hpp"
#include "Game/Map/HitInfo.hpp"
#include "Game/Player/MarioActor.hpp"
#include "Game/Player/MarioConst.hpp"
#include "Game/Player/MarioModule.hpp"

bool Mario::isSliderFloor() const {
    return _960 == 9;
}

bool Mario::checkSliderMode() const {
    if (!mMovementStates.jumping && mMovementStates._1) {
        if (isSliderFloor()) {
            return true;
        }
    }

    return false;
}

MarioSlider::MarioSlider(MarioActor* pActor) : MarioState(pActor, 7) {
    _14.zero();
    _20.zero();
    _2C.zero();
    _40 = 0;
    _38 = 0.0f;
    _3C = 0.0f;
}

void MarioSlider::calcGroundAccel() {
    TVec3f* val = &getPlayer()->_368;
    TVec3f v6;
    PSVECCrossProduct(val, getPlayer()->getGravityVec(), &v6);
    MR::normalizeOrZero(&v6);
    PSVECCrossProduct(&v6, val, &_2C);
    MR::normalizeOrZero(&_2C);
}

bool MarioSlider::postureCtrl(MtxPtr) {
    Mtx rotMtx;
    TVec3f& frontVec = getFrontVec();
    f32 rotateAngle = _3C * 3.1415927f;
    rotateAngle = 0.2f * (rotateAngle * 0.5f);
    PSMTXRotAxisRad(rotMtx, &frontVec, rotateAngle);

    TVec3f newHeadVec;
    PSMTXMultVec(rotMtx, &getPlayer()->_368, &newHeadVec);

    MarioConst* pConst = mActor->mConst;
    Mario* pPlayer = getPlayer();
    MR::vecBlendSphere(getPlayer()->_1FC, newHeadVec, &pPlayer->_1FC,
                       pConst->mTable[pConst->mCurrentTable]->mSliderHeadRotateRatio);
    return false;
}

void MarioSlider::calcWallHit() {
    if (getPlayer()->mMovementStates._1A) {
        TVec3f stack_50(_14);
        f32 sideDot = MR::vecKillElement(stack_50, *getPlayer()->mSideWallTriangle->getNormal(0), &stack_50);

        if (sideDot < 0.0f) {
            if (-sideDot < 10.0f) {
                sideDot = -10.0f;
            }

            TVec3f stack_20(*getPlayer()->mSideWallTriangle->getNormal(0));
            stack_20.scale(sideDot);
            TVec3f stack_2C(stack_50 - stack_20);
            _14 = stack_2C;
        }
    }

    if (getPlayer()->mMovementStates._8) {
        TVec3f stack_44(_14);
        f32 frontDot = MR::vecKillElement(stack_44, *getPlayer()->mFrontWallTriangle->getNormal(0), &stack_44);

        if (frontDot < 0.0f) {
            if (-frontDot < 10.0f) {
                frontDot = -10.0f;
            }

            f32 frontScale = 0.5f * frontDot;
            TVec3f stack_8(*getPlayer()->mFrontWallTriangle->getNormal(0));
            stack_8.scale(frontScale);
            TVec3f stack_14(stack_44 - stack_8);
            _14 = stack_14;

            TVec3f stack_38(_14);
            MR::normalize(&stack_38);
            getPlayer()->setFrontVecKeepUp(stack_38);
        }
    }
}

void Mario::startSlider() {
    if (!isStatusActive(mSlider->mStatusId)) {
        if (getPlayer()->mMovementStates._B) {
            if (getPlayer()->mMovementStates.jumping) {
                mDrawStates._4 = 1;
                playSound("尻ドロップ着地", -1);
                playSound("声尻ドロップ着地", -1);
                playEffectRT("属性尻ドロップ", _368, mPosition);
                startPadVib("最強");
                startCamVib(0);
                MR::removeAllClingingKarikari();
            }
        }

        stopWalk();
        stopJump();
        changeStatus(mSlider);
    }
}

bool MarioSlider::start() {
    _14 = getPlayer()->_16C;
    MR::vecKillElement(_14, getPlayer()->_368, &_14);
    _20 = getFrontVec();
    getPlayer()->cancelSquatMode();
    changeAnimation("スライダー尻", "スライダー尻");
    _40 = 10;
    _38 = 0.0f;
    _3C = 0.0f;
    return true;
}

bool MarioSlider::update() {
    if (getPlayer()->checkSliderMode()) {
        _40 = 10;
    } else {
        if (getPlayer()->mMovementStates._1 != 0) {
            if (isSlipPolygon(getGroundPolygon())) {
                Mario* player = getPlayer();
                player->_8F8 = _14;
                player->_8F0 = 0.0f;
                return false;
            }
        }

        if (_40-- == 0) {
            getPlayer()->tryDrop();
            getPlayer()->mJumpVec = mActor->getLastMove();
            return false;
        }
    }

    calcGroundAccel();

    if (checkTrgA()) {
        f32 v11 = (_14.length() / mActor->mConst->getTable()->mJumpFrontSpeed);
        getPlayer()->_278 = v11;
        getPlayer()->tryJump();
        return false;
    } else {
        f32 stickY = -getStickY();
        f32 stickX = getStickX();
        _38 = (stickY * (1.0f - mActor->mConst->getTable()->mSliderBrakeIne)) + (_38 * mActor->mConst->getTable()->mSliderBrakeIne);
        MarioConstTable* tbl = mActor->mConst->getTable();
        _3C = (stickX * (1.0f - tbl->mSliderWeightIne)) + (_3C * tbl->mSliderWeightIne);
        f32 v18 = (_14.length() - mActor->mConst->getTable()->mSliderBrakePow);

        if (v18 > 20.0f) {
            v18 = (v18 * (1.0f - (0.1f * _38)));
        }

        TVec3f v48(_14);

        if (MR::isNearZero(v48)) {
            f32 slopePow = mActor->mConst->getTable()->mSliderSlopePow;
            TVec3f v42(_2C);
            v42.scale(slopePow);
            v48 = v42;
        }

        if (MR::isNearZero(v48)) {
            return false;
        } else {
            MR::normalize(&v48);
            TVec3f v47(getPlayer()->_368);
            TVec3f v46;
            PSVECCrossProduct(&v48, &v47, &v46);
            MR::vecKillElement(v46, getPlayer()->_368, &v46);
            MR::normalize(&v46);
            _14.setLength(v18);
            f32 slopePow = mActor->mConst->getTable()->mSliderSlopePow;
            TVec3f v41(_2C);
            v41.scale(slopePow);
            _14 += v41;

            f32 v23 = _3C;
            f32 weightPow = mActor->mConst->getTable()->mSliderWeightPow;
            TVec3f v39(v46);
            v39.scale(v23);
            TVec3f v40(v39);
            v40.scale(weightPow);
            _14 += v40;
            MR::vecKillElement(_14, getPlayer()->_368, &_14);
            if (_14.length() > mActor->mConst->getTable()->mSliderMaxSpeed) {
                _14.setLength(mActor->mConst->getTable()->mSliderMaxSpeed);
            }

            addVelocity(_14);

            TVec3f v38;
            JMathInlineVEC::PSVECNegate(&getPlayer()->_368, &v38);

            TVec3f v45(v38);
            v45.scale(15.0f);

            f32 v31 = MR::vecKillElement(v45, getFrontVec(), &v45);
            addVelocity(v45, v31);

            TVec3f v44(getPlayer()->mFrontVec);
            TVec3f v43;
            MR::vecKillElement(_14, *getPlayer()->getGravityVec(), &v43);

            if (!MR::isNearZero(v43)) {
                MR::normalize(&v43);
                getPlayer()->setFrontVecKeepUp(v43, mActor->mConst->getTable()->mSliderFrontTurnRatio);
            }

            calcWallHit();
            playSound("坂滑り", -1);
            return true;
        }
    }

    return true;
}

bool MarioSlider::close() {
    if (getPlayer()->isSwimming()) {
        stopAnimation("スライダー尻", static_cast<const char*>(nullptr));
    } else if (getPlayer()->mMovementStates.jumping == 0) {
        stopAnimation("スライダー尻", "基本");
    } else {
        stopAnimation("スライダー尻", "落下");
    }

    return true;
}

namespace NrvMarioActor {
    INIT_NERVE(MarioActorNrvWait);
    INIT_NERVE(MarioActorNrvGameOver);
    INIT_NERVE(MarioActorNrvGameOverAbyss);
    INIT_NERVE(MarioActorNrvGameOverAbyss2);
    INIT_NERVE(MarioActorNrvGameOverFire);
    INIT_NERVE(MarioActorNrvGameOverBlackHole);
    INIT_NERVE(MarioActorNrvGameOverNonStop);
    INIT_NERVE(MarioActorNrvGameOverSink);
    INIT_NERVE(MarioActorNrvTimeWait);
    INIT_NERVE(MarioActorNrvNoRush);
};  // namespace NrvMarioActor
