#include "Game/Player/MarioHang.hpp"
#include "Game/Map/HitInfo.hpp"
#include "Game/Player/Mario.hpp"
#include "Game/Player/MarioActor.hpp"
#include "Game/Player/MarioAnimator.hpp"
#include "Game/Player/MarioConst.hpp"
#include "Game/Util/MapUtil.hpp"
#include "Game/Util/MathUtil.hpp"
#include "Game/Util/MtxUtil.hpp"
#include <cstring>

bool Mario::isHanging() const {
    return getCurrentStatus() == MarioStatus_Hang;
}

bool Mario::fixHangDir(const TVec3f& position, TVec3f* direction) {
    TVec3f horizontal(position - mPosition);
    MR::vecKillElement(horizontal, *getGravityVec(), &horizontal);
    MR::normalizeOrZero(&horizontal);
    Triangle triangle;
    if (MR::getFirstPolyOnLineToMap(nullptr, &triangle, mPosition, horizontal * 150.0f)) {
        *direction = -*triangle.getNormal(0);
        mHang->recordWallPolygon(&triangle);
        return true;
    }
    if (direction->dot(horizontal) < 0.0f) {
        *direction = horizontal;
        return false;
    }
    return false;
}

bool Mario::isEnableBackHang() {
    bool turn = false;
    if (!_474->isValid()) {
        return false;
    }
    if (_430 == 6) {
        if (isRising()) {
            return false;
        }
        turn = true;
    } else if (!mMovementStates._30) {
        return false;
    }
    if (!mMovementStates._19) {
        return false;
    }
    if (mMovementStates._B) {
        return false;
    }
    if (checkWallCode("NotGrab", false)) {
        return false;
    }
    const char* wallCode = MR::getWallCodeString(_474);
    if (wallCode && strcmp(wallCode, "NotGrab") == 0) {
        return false;
    }
    if (isRising()) {
        return false;
    }
    if (mStickPos.z > mActor->getConst().getTable()->mWallBackHangStickPower) {
        return false;
    }
    if (mWalkSpeed > mActor->getConst().getTable()->mWallBackHangWalkSpeed) {
        return false;
    }

    TVec3f ground;
    TVec3f horizontal;
    if (getPlayer()->getShadowNorm().dot(*getGravityVec()) < -0.707f) {
        if (turn) {
            ground = mPosition;
        } else {
            getLastGroundPos(&ground);
        }
        f32 height = MR::vecKillElement(mShadowPos - ground, *getGravityVec(), &horizontal);
        if (height < mActor->getConst().getTable()->mWallHangGrHeight) {
            return false;
        }
    }
    if (!turn) {
        if ((_4F4 - mGroundPos).dot(*getGravityVec()) < 0.0f) {
            return false;
        }
    } else {
        setFrontVecKeepUp(-mFrontVec);
        return false;
    }
    TVec3f distance;
    TVec3f position;
    getLastGroundPos(&position);
    MR::vecKillElement(position - mPosition, getAirGravityVec(), &distance);
    return !(distance.length() >= 84.0f);
}

bool Mario::isEnableSideHang() {
    if (_1C._1 || _1C._2) {
        return false;
    }
    if (!mMovementStates._30) {
        return false;
    }
    if (!mMovementStates._1A) {
        return false;
    }
    if (checkWallCode("NotGrab", false)) {
        return false;
    }
    if (checkWallCode("NoAction", false)) {
        return false;
    }
    const char* wallCode = MR::getWallCodeString(_474);
    if (wallCode) {
        if (strcmp(wallCode, "NotGrab") == 0) {
            return false;
        }
        if (strcmp(wallCode, "NoAction") == 0) {
            return false;
        }
    }
    if (isRising()) {
        return false;
    }
    if (mStickPos.z > mActor->getConst().getTable()->mWallBackHangStickPower) {
        return false;
    }
    if (mWalkSpeed > mActor->getConst().getTable()->mWallBackHangWalkSpeed) {
        return false;
    }

    TVec3f ground;
    TVec3f horizontal;
    if (getPlayer()->getShadowNorm().dot(*getGravityVec()) < -0.707f) {
        getLastGroundPos(&ground);
        f32 height = MR::vecKillElement(mShadowPos - ground, *getGravityVec(), &horizontal);
        if (height < mActor->getConst().getTable()->mWallHangGrHeight) {
            return false;
        }
    }
    if (!mMovementStates._33) {
        return false;
    }
    if ((_500 - mGroundPos).dot(*getGravityVec()) < -20.0f) {
        return false;
    }
    TVec3f distance;
    TVec3f position;
    getLastGroundPos(&position);
    MR::vecKillElement(position - mPosition, getAirGravityVec(), &distance);
    return !(distance.length() >= 84.0f);
}

void MarioHang::recordWallPolygon(const Triangle* triangle) {
    *_20 = *triangle;
    mWallSensor = triangle->mSensor;
    TPos3f matrix;
    MR::makeMtxWithoutScale(&matrix, *_20->getBaseInvMtx());
    PSMTXMultVecSR(matrix.toMtxPtr(), _20->getNormal(0), &_28);
    _34 = *_20->getNormal(0);
}

void MarioHang::recordHangNorm(const TVec3f& normal) {
    _34 = normal;
    TPos3f matrix;
    MR::makeMtxWithoutScale(&matrix, *_20->getBaseInvMtx());
    PSMTXMultVecSR(matrix.toMtxPtr(), &normal, &_28);
}

void MarioHang::forceDrop() {
    _1C = 1;
}

MarioHang::MarioHang(MarioActor* actor) : MarioState(actor, MarioStatus_Hang) {
    _12 = 0;
    _14 = 0;
    _16 = 0;
    mHangTimer = 0;
    _1A = 0;
    _1B = 0;
    _1C = 0;
    _1D = 0;
    _1E = 0;
    _28.zero();
    _34.zero();
    mWallSensor = nullptr;
    _20 = new Triangle;
    _24 = new Triangle;
}

bool MarioHang::close() {
    stopAnimation("崖つかまり開始");
    stopAnimation("崖つかまり中");
    stopAnimation("崖つかまり終了");
    stopAnimation("崖つかまり終了坂");
    if (getPlayer()->mMovementStates._1) {
        changeAnimation(nullptr, "基本");
    } else {
        changeAnimation(nullptr, "落下");
    }
    getPlayer()->setWallCancel();
    mHangTimer = 120;
    if (_1A) {
        getPlayer()->tryHangSlipUp();
    } else {
        getPlayer()->_10._F = true;
    }
    mActor->_F44 = 1;
    return true;
}

bool MarioHang::notice() {
    if (getNoticedStatus() == MarioStatus_Faint) {
        addTrans(getPlayer()->mFrontVec * -150.0f, "Module");
        mActor->setBlendMtxTimer(16);
        stopAnimation("崖つかまり中", "基本");
    }
    return false;
}

bool MarioHang::postureCtrl(MtxPtr matrix) {
    TVec3f up;
    TVec3f direction(getPlayer()->mHeadVec);
    direction += _34;
    if (MR::isNearZero(direction)) {
        return false;
    }
    if (!_12) {
        if (MR::isSameDirection(getPlayer()->_368, _34, 0.01f)) {
            return false;
        }
        MR::makeMtxFrontUp(reinterpret_cast< TPos3f* >(matrix), -_34, getPlayer()->_368);
    } else {
        f32 blend = MR::clamp(_14 / 30.0f, 0.0f, 1.0f);
        MR::vecBlendSphere(getPlayer()->_368, getPlayer()->mHeadVec, &up, blend);
        if (MR::isSameDirection(up, _34, 0.01f)) {
            return false;
        }
        MR::makeMtxFrontUp(reinterpret_cast< TPos3f* >(matrix), -_34, up);
    }
    return true;
}

void Mario::checkHang() {
    if (mHang->mHangTimer) {
        return;
    }
    if (mMovementStates.jumping && isRising()) {
        mMovementStates._30 = false;
    }
    if (mMovementStates.jumping && _3BC > 20) {
        mMovementStates._30 = false;
    }
    if (isCeiling()) {
        mMovementStates._31 = false;
    }
    if (!mMovementStates._31 || getPlayer()->mDrawStates.mIsUnderwater || isStatusActive(MarioStatus_Rabbit) || mActor->_468 ||
        getPlayerMode() == PlayerMode_Bee) {
        return;
    }
    if (getPlayerMode() == PlayerMode_Teresa) {
        return;
    }

    bool front = true;
    bool back = true;
    bool side = true;
    if (mMovementStates._1A) {
        const Triangle* triangle = mSideWallTriangle;
        if (!MR::isSameMtx(triangle->getBaseMtx()->toMtxPtr(), triangle->getPrevBaseMtx()->toMtxPtr())) {
            front = false;
            back = false;
        }
    }
    if (mMovementStates._8) {
        const Triangle* triangle = mFrontWallTriangle;
        if (!MR::isSameMtx(triangle->getBaseMtx()->toMtxPtr(), triangle->getPrevBaseMtx()->toMtxPtr())) {
            side = false;
            back = false;
        }
    }
    if (mMovementStates._19) {
        const Triangle* triangle = mBackWallTriangle;
        if (!MR::isSameMtx(triangle->getBaseMtx()->toMtxPtr(), triangle->getPrevBaseMtx()->toMtxPtr())) {
            front = false;
            side = false;
        }
    }
    TVec3f previousPosition(mPosition);
    if (front && mMovementStates._39 && isEnableHang()) {
        TVec3f direction(_4A4 - mPosition);
        MR::vecKillElement(direction, getAirGravityVec(), &direction);
        MR::normalizeOrZero(&direction);
        if (getFrontWallNorm().dot(direction) >= -0.8f) {
            return;
        }
        if (!MR::isExistMapCollision(_4A4 - mFrontVec * 50.0f - getAirGravityVec() * 5.0f, mFrontVec * 60.0f)) {
            u8 started = 1;
            if (isAnimationRun("落下")) {
                started = 0;
            }
            changeStatus(mHang);
            mVelocity.zero();
            stopWalk();
            stopJump();
            setTrans(_4A4, nullptr);
            setFrontVecKeepUp(-getWallNorm());
            mHang->recordWallPolygon(mFrontWallTriangle);
            mHang->_1E = started;
        }
    } else if (back && isEnableBackHang()) {
        TVec3f position;
        getLastGroundPos(&position);
        if ((mPosition - position).dot(getAirGravityVec()) >= 160.0f) {
            return;
        }
        TVec3f direction(-mFrontVec);
        u32 edge = getLastGroundEdgeIndex(mActor->_2A0, direction);
        direction = _4F4 - mActor->_2A0;
        MR::normalizeOrZero(&direction);
        u32 positionEdge = getLastGroundEdgeIndex(mActor->_2A0, direction);
        direction = -*mBackWallTriangle->getNormal(0);
        u32 normalEdge = getLastGroundEdgeIndex(mActor->_2A0, direction);
        TVec3f normal;
        if (edge == positionEdge && edge == normalEdge) {
            normal = *getLastGroundEdgeNrm(edge);
        } else {
            return;
        }
        if (normal.dot(getAirGravityVec()) >= 0.1f) {
            return;
        }
        if (normal.dot(mHeadVec) >= 0.1f) {
            return;
        }
        direction = -*mBackWallTriangle->getNormal(0);
        if (MR::diffAngleAbsHorizontal(normal, -direction, getAirGravityVec()) > 0.08726647f) {
            return;
        }
        TVec3f horizontal;
        f32 distance = 0.0f;
        switch (edge) {
        case 0:
        case 1:
            distance = MR::vecKillElement(*_474->calcAndGetPos(0) - position, normal, &horizontal);
            position += normal * distance;
            break;
        case 2:
            distance = MR::vecKillElement(*_474->calcAndGetPos(1) - position, normal, &horizontal);
            position += normal * distance;
            break;
        }
        if (distance > 100.0f) {
            return;
        }
        MR::vecKillElement(normal, getAirGravityVec(), &normal);
        MR::normalizeOrZero(&normal);
        if (!MR::isExistMapCollision(position - direction * 50.0f - getAirGravityVec() * 5.0f, direction * 60.0f)) {
            changeStatus(mHang);
            mVelocity.zero();
            stopWalk();
            stopJump();
            TVec3f hangDirection;
            bool found = fixHangDir(position, &hangDirection);
            setTrans(position, nullptr);
            setFrontVecKeepUp(normal);
            mMovementStates._1D = true;
            if (!found) {
                mHang->recordWallPolygon(mBackWallTriangle);
            }
            mHang->recordHangNorm(normal);
            mHang->_1E = 0;
        }
    } else if (side && isEnableSideHang()) {
        TVec3f position;
        getLastGroundPos(&position);
        if ((mPosition - position).dot(getAirGravityVec()) >= 160.0f) {
            return;
        }
        TVec3f direction(_500 - mActor->_2A0);
        MR::normalizeOrZero(&direction);
        u32 edge = getLastGroundEdgeIndex(mActor->_2A0, direction);
        TVec3f sideDirection(mSideVec);
        if (sideDirection.dot(direction) < 0.0f) {
            direction = -sideDirection;
        } else {
            direction = sideDirection;
        }
        u32 sideEdge = getLastGroundEdgeIndex(mActor->_2A0, direction);
        direction = -*mSideWallTriangle->getNormal(0);
        u32 normalEdge = getLastGroundEdgeIndex(mActor->_2A0, direction);
        TVec3f normal;
        if (edge == sideEdge && edge == normalEdge) {
            normal = *getLastGroundEdgeNrm(edge);
        } else {
            return;
        }
        if (normal.dot(getAirGravityVec()) >= 0.1f) {
            return;
        }
        if (normal.dot(mHeadVec) >= 0.1f) {
            return;
        }
        direction = -*mSideWallTriangle->getNormal(0);
        if (MR::diffAngleAbsHorizontal(normal, -direction, getAirGravityVec()) > 0.08726647f) {
            return;
        }
        MR::vecKillElement(normal, getAirGravityVec(), &normal);
        MR::normalizeOrZero(&normal);
        if (!MR::isExistMapCollision(position - direction * 50.0f - getAirGravityVec() * 5.0f, direction * 60.0f)) {
            changeStatus(mHang);
            mVelocity.zero();
            stopWalk();
            stopJump();
            TVec3f hangDirection;
            bool found = fixHangDir(position, &hangDirection);
            setTrans(position, nullptr);
            setFrontVecKeepUp(normal);
            mMovementStates._1D = true;
            if (!found) {
                mHang->recordWallPolygon(mSideWallTriangle);
            }
            mHang->recordHangNorm(normal);
            mHang->_1E = 0;
        }
    }

    if (isStatusActive(MarioStatus_Hang)) {
        _414 = 0;
        for (u32 i = 0; i < 10; i++) {
            updateGroundInfo();
            if (mMovementStates._1) {
                return;
            }
            addTrans(mFrontVec * 1.0f - getAirGravityVec() * 1.0f, nullptr);
        }
    }
}

bool Mario::isEnableHang() {
    if (!mMovementStates._8) {
        return false;
    }
    if (!mMovementStates._15) {
        return false;
    }
    if (mMovementStates._B) {
        return false;
    }
    if (mDrawStates._1E) {
        return false;
    }
    if (isStatusActive(MarioStatus_Recovery)) {
        return false;
    }
    if (checkWallCode("NotGrab", false)) {
        return false;
    }
    if (checkWallCode("NoAction", false)) {
        return false;
    }
    const char* wallCode = MR::getWallCodeString(_47C);
    if (wallCode) {
        if (strcmp(wallCode, "NotGrab") == 0) {
            return false;
        }
        if (strcmp(wallCode, "NoAction") == 0) {
            return false;
        }
    }

    TVec3f shadowPos(mShadowPos);
    TVec3f up(-getAirGravityVec());
    f32 slope = _45C->getNormal(0)->dot(up);
    f32 height = mActor->getConst().getTable()->mWallHangGrHeight;
    if (slope > 0.0f) {
        if (slope > 0.2f) {
            slope = 0.2f;
        }
        height -= 30.0f * (5.0f * slope);
    }
    if (getPlayer()->getShadowNorm().dot(*getGravityVec()) < -0.707f) {
        if ((_4A4 - mShadowPos).dot(-*getGravityVec()) < height) {
            return false;
        }
    }
    f32 heightFromMario = (_4A4 - mPosition).dot(-*getGravityVec());
    if (heightFromMario > mActor->getConst().getTable()->mWallHangMyHeight) {
        return false;
    }
    return !isRising();
}

bool MarioHang::start() {
    getPlayer()->cancelSquatMode();
    stopAnimationUpper(nullptr, nullptr);
    changeAnimation("崖つかまり開始", "崖つかまり中");
    mActor->setBlendMtxTimer(mActor->getConst().getTable()->mHangBlendTime);
    _12 = 0;
    _14 = 0;
    _16 = 0;
    _1B = 0;
    _1C = 0;
    _1D = 0;
    _1A = 0;
    Mario* player = getPlayer();
    player->_74C = 0.0f;
    player->_750 = 0;
    player->_754 = 0;
    mActor->_F44 = 0;
    return true;
}

bool MarioHang::update() {
    getPlayer()->mDrawStates._A = false;
    TPos3f matrix;
    matrix.identity();
    MR::makeMtxWithoutScale(&matrix, *_20->getBaseMtx());
    TVec3f normal;
    PSMTXMultVecSR(matrix.toMtxPtr(), &_28, &normal);
    getPlayer()->setFrontVecKeepUp(-normal);
    _34 = normal;
    _14++;
    if (_16) {
        _16--;
        if (!_16) {
            _1A = 1;
        } else {
            return true;
        }
    }
    if (_1A) {
        return false;
    }
    if (MR::isSameMtx(_20->getBaseMtx()->toMtxPtr(), _20->getPrevBaseMtx()->toMtxPtr())) {
        if (getPlayer()->_8D4) {
            addTrans(getPlayer()->mFrontVec * -100.0f, "Module");
            mActor->setBlendMtxTimer(16);
            stopAnimation("崖つかまり中", "基本");
            return false;
        }
    } else {
        if (getPlayer()->mMovementStates._1A && _14 > 2) {
            TVec3f movement(mActor->getLastMove());
            if (movement.dot(getPlayer()->getSideWallNorm()) < 0.0f) {
                _1C = 1;
            }
        }
        Mario* player = getPlayer();
        if (player->_184.dot(getFrontVec()) > 0.707f && getPlayer()->_4E4 < 45.0f && !isAnimationRun("崖つかまり開始")) {
            _1C = 1;
        }
    }
    if (getPlayer()->_8D4 && mWallSensor != getPlayer()->_8D4) {
        addTrans(getPlayer()->mFrontVec * -100.0f, "Module");
        mActor->setBlendMtxTimer(16);
        stopAnimation("崖つかまり中", "基本");
        return false;
    }
    if (_1C) {
        addTrans(getPlayer()->mFrontVec * -100.0f, "Module");
        mActor->setBlendMtxTimer(16);
        stopAnimation("崖つかまり中", "基本");
        return false;
    }

    switch (_12) {
    case 0:
        if (getPlayer()->mVerticalSpeed < 160.0f) {
            if (getPlayer()->getShadowNorm().dot(getGravityVec()) < -0.707f) {
                addTrans(getPlayer()->mFrontVec * -100.0f, "Module");
                mActor->setBlendMtxTimer(16);
                stopAnimation("崖つかまり中", "基本");
                return false;
            }
        }
        if (!getPlayer()->mMovementStates._1) {
            _1C = 1;
        }
        if (getPlayer()->calcDistToCeilHead() < 80.0f) {
            _1C = 1;
        }
        break;
    case 1:
        if (calcAngleD(getPlayer()->_368) > 5.0f) {
            changeAnimationNonStop("崖つかまり終了坂");
            changeAnimation(nullptr, "基本");
        } else {
            changeAnimation("崖つかまり終了", "基本");
        }
        playSound("声崖つかまり終了");
        _12++;
        return true;
    case 2:
        if (!getPlayer()->mMovementStates._1) {
            addVelocity(getFrontVec() * 6.0f);
        }
        if (!isAnimationRun(nullptr) || isAnimationTerminate(nullptr)) {
            getPlayer()->mMovementStates._1 = true;
            return false;
        }
        return true;
    }

    if (_1C) {
        addTrans(getPlayer()->mFrontVec * -100.0f, "Module");
        mActor->setBlendMtxTimer(16);
        stopAnimation("崖つかまり中", "基本");
        return false;
    }
    if (checkTrgA()) {
        tryClimb(true);
    }
    if (getPlayer()->mMovementStates._1D) {
        return true;
    }
    u8 direction = 0;
    if (!_16) {
        direction = getPlayer()->checkStickFrontBack();
        if (getStickP() < 0.95f) {
            direction = 0;
        }
        if (_1D) {
            direction = 2;
        }
    }
    switch (direction) {
    case 1:
        if (_14 > 18) {
            tryClimb(false);
        }
        break;
    case 2:
        if (isAnimationRun("崖つかまり開始") && getAnimator()->getFrame() < 20.0f) {
            _1D = 1;
        } else {
            addTrans(getPlayer()->mFrontVec * -100.0f, "Module");
            mActor->setBlendMtxTimer(16);
            Mario* player = getPlayer();
            player->_3C0 = 0;
            getPlayer()->mMovementStates._31 = false;
            getPlayer()->mMovementStates._1 = false;
            getPlayer()->tryDrop();
            player = getPlayer();
            player->_3CA = 120;
            return false;
        }
        break;
    }
    if (_1B && !_12 && _14 > 18) {
        _12 = 1;
        _14 = 0;
    }
    if (!_12 && _14 == 2) {
        playSound("声崖つかまり");
    }
    return true;
}

void MarioHang::tryClimb(bool jump) {
    if (_12) {
        return;
    }
    if (getPlayer()->calcDistToCeil(false) < 160.0f) {
        return;
    }
    Triangle frontTriangle;
    if (MR::getFirstPolyOnLineToMap(nullptr, &frontTriangle, getTrans() - getGravityVec() * 30.0f, getFrontVec() * 60.0f)) {
        return;
    }

    u32 i;
    for (i = 0; i < 3; i++) {
        Triangle sideTriangle;
        Mario* player = getPlayer();
        Mario* sidePlayer = getPlayer();
        bool left = MR::getFirstPolyOnLineToMap(nullptr, &sideTriangle, getTrans() - getGravityVec() * 30.0f - sidePlayer->mSideVec * 15.0f,
                                                player->mSideVec * 55.0f);
        bool right = MR::getFirstPolyOnLineToMap(nullptr, &sideTriangle, getTrans() - getGravityVec() * 30.0f + getPlayer()->mSideVec * 15.0f,
                                                 getPlayer()->mSideVec * -55.0f);
        if (right && left) {
            _1C = 1;
            return;
        }
        if (left) {
            addTrans(getPlayer()->mSideVec * -15.0f, "Module");
        } else if (right) {
            addTrans(getPlayer()->mSideVec * 15.0f, "Module");
        } else {
            break;
        }
    }
    if (i == 3) {
        _1C = 1;
        return;
    }
    if (isAnimationRun("崖つかまり開始")) {
        if (_1E && jump && getAnimationFrame() < 30.0f) {
            _16 = 5;
            changeAnimationNonStop("つかまりスリップアップ");
        }
        _1B = 1;
        return;
    }
    _12 = 1;
    _14 = 0;
}
