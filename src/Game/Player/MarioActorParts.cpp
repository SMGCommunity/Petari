#include "Game/Animation/XanimeCore.hpp"
#include "Game/LiveActor/HitSensor.hpp"
#include "Game/Map/HitInfo.hpp"
#include "Game/MapObj/CollectCounter.hpp"
#include "Game/MapObj/IceStep.hpp"
#include "Game/Player/FireMarioBall.hpp"
#include "Game/Player/JetTurtleShadow.hpp"
#include "Game/Player/MarioActor.hpp"
#include "Game/Player/MarioAnimator.hpp"
#include "Game/Player/MarioConst.hpp"
#include "Game/Player/MarioNullBck.hpp"
#include "Game/Player/MarioParts.hpp"
#include "Game/Player/MarioState.hpp"
#include "Game/Player/MarioSwim.hpp"
#include "Game/Player/TornadoMario.hpp"
#include "Game/Screen/GameSceneLayoutHolder.hpp"
#include "Game/Util/ActorSensorUtil.hpp"
#include "Game/Util/EffectUtil.hpp"
#include "Game/Util/FixedPosition.hpp"
#include "Game/Util/JointUtil.hpp"
#include "Game/Util/LightUtil.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/MapUtil.hpp"
#include "Game/Util/MathUtil.hpp"
#include "Game/Util/ModelUtil.hpp"
#include "Game/Util/MtxUtil.hpp"

void MarioActor::init2D() {
    MR::getGameSceneLayoutHolder()->initLifeCount(mMaxHealth);

    _1B8 = new CollectCounter("マリオ連続踏み");

    _1B8->initWithoutIter();
}

void MarioActor::initParts() {
    mNullAnimation = new MarioNullBck("NULLアニメ");
    mNullAnimation->initWithoutIter();

    mSearchLight = 0;
    mSearchLightThrowPos = nullptr;

    initSearchLight();
    initThrowing();

    _9C4 = new MarioParts(this, "氷結モデル", "MarioFreezeIce", true, nullptr, nullptr);
    _9C4->initWithoutIter();
    _9C4->makeActorDead();
    _9C4->initFixedPosition(TVec3f(0.0f, 0.0f, 0.0f), TVec3f(0.0f, 0.0f, 0.0f), nullptr);

    _9A0 = new JetTurtleShadow("カメシャドウモデル");
    _9A0->initWithoutIter();

    _994 = new MarioParts(this, "スピンチコ", "SpinTico", false, getBaseMtx(), nullptr);
    _994->initWithoutIter();
    _994->kill();
}

void MarioActor::updateBeeWingAnimation() {
    if (mPlayerMode != 4) {
        getJointCtrl("HandR")->setLocalScale(1.0f);
        getJointCtrl("HandL")->setLocalScale(1.0f);

        return;
    }

    getJointCtrl("HandR")->setLocalScale(0.9f);
    getJointCtrl("HandL")->setLocalScale(0.9f);

    if (mMario->checkLvlA() && mMario->_402 && getMovementStates().jumping) {
        if (_9F0 != 1) {
            MR::startBck(_9E8, "Fly", nullptr);
            MR::startBva(_9E8, "Fly");
            MR::startBtk(_9E8, "Fly");
        }

        _9F0 = 1;
        return;
    }

    s32 val;
    if (getMovementStates()._1 || mMario->isStatusActive(MarioStatus_Stick)) {
        val = 0;
    } else {
        val = mMario->_402 != 0 ? 2 : 3;
    }

    if (_9F0 == val) {
        return;
    }

    switch (val) {
    case 0:
        MR::startBck(_9E8, "Wait", nullptr);
        MR::startBva(_9E8, "Wait");
        break;

    case 2:
        MR::startBck(_9E8, "FlyWait", nullptr);
        MR::startBva(_9E8, "FlyWait");
        break;

    case 3:
        MR::startBck(_9E8, "FlyFall", nullptr);
        MR::startBva(_9E8, "FlyFall");
        break;
    }

    MR::stopBtk(_9E8);
    _9F0 = val;
}

void MarioActor::updateTornado() {
    if (mTornadoMario == nullptr) {
        return;
    }

    if (mMario->getMovementStates()._F && mMario->_544 > 1) {
        mTornadoMario->show();
    } else if ((!isAnimationRun("空中ひねり") || mMario->_430 != 8) && !mMario->isStatusActive(MarioStatus_Magic)) {
        if (mMario->getDrawStates()._8 || _990 != 0) {
            mTornadoMario->hideForce();
        } else {
            mTornadoMario->hide();
        }

        _990 = 0;
    }

    mTornadoMario->setTrHeight(mPosition, mMario->mFrontVec, mMario->_54C, _240);
}

void MarioActor::updateTakingPosition() {
    if (_480) {
        const HitSensor* sensor = _424;
        if (!sensor)
            sensor = getCarrySensor();
        if (!sensor) {
            _480 = false;
        } else {
            switch (sensor->mType) {
            case ATYPE_BOMBHEI:
                if (mMario->isAnimationTerminate("カブ抜き")) {
                    stopAnimation(nullptr);
                    _480 = false;
                    mMario->changeAnimationUpper("カブウエイト", nullptr);
                }
                break;
            case ATYPE_JET_TURTLE:
            case ATYPE_JET_TURTLE_SLOW:
                if (mMario->isAnimationTerminate(nullptr))
                    _480 = false;
                break;
            }
        }
    }
    if (_B92 < 0) {
        Mtx base;
        TVec3f position;
        TVec3f rotation;
        bool updateAnimation;
        if (_B92 == -3) {
            _494->calc();
            _494->copyTrans(&position);
            _494->copyRotate(&rotation);
            MarioAnimator* animator = mMarioAnim;
            s32 stopped = !isAnimationRun(nullptr);
            s32 terminated = mMario->isAnimationTerminate(nullptr);
            terminated |= stopped;
            s32 landing = animator->isLandingAnimationRun();
            landing |= terminated;
            updateAnimation = landing;
        } else {
            f32 frame;
            if (_B92 == -1)
                frame = mMarioAnim->getUpperFrame();
            if (_B92 == -2)
                frame = mMarioAnim->getFrame();
            PSMTXConcat(getBaseMtx(), _E3C.toMtxPtr(), base);
            PSMTXCopy(base, MR::getJ3DModel(mNullAnimation)->getBaseTRMtx());
            if (mNullAnimation->getFramePos(frame, &position, &rotation)) {
                clearNullAnimation(-3);
                if (_424)
                    MR::sendArbitraryMsg(ACTMES_TAKE_TOUCH, _424, getSensor("body"));
                else if (_428[0])
                    MR::sendArbitraryMsg(ACTMES_TAKE_TOUCH, _428[0], getSensor("body"));
                else {
                    _480 = false;
                    clearNullAnimation(0);
                }
            }
            updateAnimation = false;
        }
        if (updateAnimation) {
            if (_424)
                mMarioAnim->updateTakingAnimation(_424);
            else if (_468)
                mMarioAnim->updateTakingAnimation(_428[0]);
        }
        if (_424) {
            _424->mHost->mPosition = position;
            _424->mHost->mRotation = rotation;
            return;
        }
        if (getCarrySensor()) {
            getCarrySensor()->mHost->mPosition = position;
            getCarrySensor()->mHost->mRotation = rotation;
        }
    } else if (_468) {
        if (_428[0]->isType(ATYPE_COINTHROW) || _428[0]->isType(ATYPE_JET_TURTLE) || _428[0]->isType(ATYPE_JET_TURTLE_SLOW) ||
            _428[0]->isType(ATYPE_BOMBHEI)) {
            TVec3f position;
            _494->calc();
            _494->copyTrans(&position);
            TVec3f& carryPosition = _428[0]->mHost->mPosition;
            const TVec3f& velocity = mVelocity;
            carryPosition = position + velocity;
            _494->copyRotate(&_428[0]->mHost->mRotation);
            return;
        }
        TVec3f position;
        mNullAnimation->getLastPos(&position);
        PSMTXMultVec(_E3C.toMtxPtr(), &position, &position);
        PSMTXMultVec(getBaseMtx(), &position, &position);
        f32 offset = 10.0f * (mMario->mWalkSpeed * mMario->mWalkSpeed);
        offset *= JMASinRadian(_490);
        _490 += MR::getRandom(0.1f, 1.0f);
        position.y += offset;
        _428[0]->mHost->mPosition = position;
        TVec3f& carryRotation = _428[0]->mHost->mRotation;
        const TVec3f& rotation = mRotation;
        carryRotation = _438[0] + rotation;
    }
}

const HitSensor* MarioActor::getCarrySensor() const {
    if (_468 == nullptr) {
        return nullptr;
    }

    return _428[0];
}

void MarioActor::changeSpecialModeAnimation(const char* pAnimName) {
    switch (mPlayerMode) {
    case 6:
        if (!strcmp(pAnimName, "特殊ウエイト1A")) {
            changeTeresaAnimation("SleepStart", -1);
            return;
        }

        if (strcmp(pAnimName, "特殊ウエイト1B")) {
            return;
        }

        changeTeresaAnimation("Sleep", 16);
        MR::emitEffect(_9A4, "Sleep");
    }
}

void MarioActor::updateSpecialModeAnimation() {
    if (!mMario->mMovementStates._A && mMario->getCurrentStatus() == MarioStatus_None) {
        if (mMario->mMovementStates._1 && mMario->_960 == 0x20 && mMarioAnim->isAnimationStop()) {
            mMarioAnim->mXanimePlayer->changeTrackAnimation(0, "泥低速歩行");
            mMarioAnim->mXanimePlayer->changeTrackAnimation(1, "泥高速歩行");
            _B96 = 2;
        }
    } else {
        _B96 = 0;
    }
    switch (mPlayerMode) {
    case PlayerMode_Teresa:
        updateTeresaAnimation();
        break;
    case PlayerMode_Bee:
        if (mBeeWallWalk && !isJumping() && mMarioAnim->isAnimationStop()) {
            mMarioAnim->mXanimePlayer->changeTrackAnimation(0, "ハチ匍匐前進");
            mMarioAnim->mXanimePlayer->changeTrackAnimation(1, "ハチ匍匐前進");
            mMarioAnim->mXanimePlayer->changeTrackAnimation(2, "ハチ匍匐前進");
            mMarioAnim->mXanimePlayer->changeTrackAnimation(3, "ハチ匍匐ウエイト");
        }
        break;
    default:
        mMario->_418 = 0;
        break;
    }
    if (_B96) {
        _B96--;
        if (!_B96 && mMario->mMovementStates._1 && mMarioAnim->isAnimationStop()) {
            mMarioAnim->mXanimePlayer->changeTrackAnimation(0, "鈍行");
            mMarioAnim->mXanimePlayer->changeTrackAnimation(1, "歩行");
        }
    }
}

void MarioActor::initFireBall() {
    for (u32 idx = 0; idx < ARRAY_SIZE(_B54); idx++) {
        _B54[idx] = new FireMarioBall("マリオ炎球");
        _B54[idx]->initWithoutIter();
    }
}

void MarioActor::shootFireBall() {
    if (isAnimationRun("ファイア投げ"))
        return;
    if (isAnimationRun("ファイアスピン空中"))
        return;
    if (isAnimationRun("ファイアスピン"))
        return;
    if (getMovementStates()._8) {
        sendMsgToSensor(mMario->getWallPolygon()->mSensor, ACTMES_FIREBALL_ATTACK);
        changeAnimation("ファイアスピン", nullptr);
        return;
    }
    u32 index;
    for (index = 0; index < ARRAY_SIZE(_B54); index++) {
        if (MR::isDead(_B54[index]))
            break;
    }
    if (index == ARRAY_SIZE(_B54))
        return;
    FireMarioBall* ball = _B54[index];
    TVec3f direction;
    direction = mMario->mFrontVec;
    direction += mMario->mHeadVec;
    MR::normalize(&direction);
    TVec3f position;
    getRealPos("HandR", &position);
    position += mMario->mFrontVec * 30.0f;
    ball->appearAndThrow(position, direction);
    playSound("声投げ", -1);
    if (!isJumping()) {
        mMario->_420 = 45;
        changeAnimation("ファイアスピン", nullptr);
        return;
    }
    if (mMario->_42C >= 3) {
        changeAnimation("ファイア投げ", nullptr);
        return;
    }
    if (mMario->_42C == 0)
        changeAnimation("ファイアスピン空中", nullptr);
    else
        changeAnimation("ファイア投げ", nullptr);
    jumpHop();
    mMario->_42C++;
    f32 gravitySpeed = mMario->cutGravityElementFromJumpVec(true);
    Mario* mario = mMario;
    mario->mJumpVec.x *= 0.5f;
    mario->mJumpVec.y *= 0.5f;
    mario->mJumpVec.z *= 0.5f;
    mMario->mJumpVec += _240 * gravitySpeed;
}

void MarioActor::showFreezeModel() {
    _9C4->appear();
    MR::onCalcAnim(_9C4);
    MR::startBva(_9C4, "Nomal");
}

void MarioActor::hideFreezeModel() {
    MR::startBck(_9C4, "Break", nullptr);
    MR::startBva(_9C4, "Break");

    mMario->startFreezeEnd();
}

void MarioActor::updateFairyStar() {
    if (_482)
        return;
    if (!_EEB)
        return;
    if (!isEnableNerveChange())
        return;
    bool enabled = true;
    if (selectAction("スピン回復エフェクト") != 1)
        enabled = false;
    if (_94C && enabled) {
        if (MR::isDead(_994)) {
            _994->appear();
            _994->mRotation.set(0.0f, 0.0f, 0.0f);
            MR::startBck(_994, "SpinTimer", nullptr);
            playSound("スピン許可", -1);
        }
        TVec3f position;
        MR::copyJointPos(_994, "Center", &position);
        Color8 color(255, 225, 225, 255);
        f32 power = 0.001f;
        MR::requestPointLight(_994, position, color, power, 0);
        return;
    }
    if (!MR::isDead(_994))
        _994->kill();
}

void MarioActor::update2D() {
    GameSceneLayoutHolder* layoutHolder = MR::getGameSceneLayoutHolder();
    layoutHolder->setLifeCount(mHealth);

    if (mMario->getPlayerMode() == 4) {
        layoutHolder->setBeePowerRatio((f32)mMario->_402 / mConst->getTable()->mAirWalkTime);
    }

    if (mMario->isSwimming()) {
        layoutHolder->setOxygenRatio((f32)mMario->mSwim->mOxygen / mConst->getTable()->mOxygenMax);
    }

    if (_989 == 0) {
        _1B8->kill();
    }
}

void MarioActor::updateThrowVector() {
    HitSensor* sensor = _46C;
    if (sensor && !MR::isExistInAttributeGroupSearchTurtle(sensor->mHost))
        sensor = nullptr;
    if (sensor && _468) {
        if (!getDrawStates()._5) {
            if (_470 && _46C != _470)
                _470 = nullptr;
        }
        if (!getDrawStates()._5) {
            if (!_47C)
                _484 = _2A0;
            _47C++;
            TVec3f direction = _46C->mPosition - _484;
            if (direction.length() <= 40.0f + _46C->getRadius()) {
                _484 = _46C->mPosition;
                _470 = _46C;
                if (MR::isSensorEnemy(_46C)) {
                    _47C = 60;
                    return;
                }
                _47C = 2;
                return;
            }
            MR::normalize(&direction);
            _484 += direction * 40.0f;
            if (MR::checkStrikePointToMap(_484, nullptr)) {
                _47C = 0;
                _470 = nullptr;
            }
        }
    } else if (!getDrawStates()._5) {
        if (_47C)
            _47C--;
        if (!_47C)
            _470 = nullptr;
    }
}

void MarioActor::createIceFloor(const TVec3f& rVec) {
    TPos3f mtx;
    mtx.identity();

    TVec3f upVec;
    getUpVec(&upVec);

    MR::makeMtxUpFront(&mtx, -getAirGravityVec(), mMario->mFrontVec);

    TVec3f vec;
    mtx.getEuler(vec);

    vec *= _180_PI;
    createIceFloor(rVec, vec);
}

void MarioActor::createIceFloor(const TVec3f& rVec1, const TVec3f& rVec2) {
    _B4C[_B50]->setOn(_B50, rVec1, rVec2);

    _B50 = (_B50 + 1) % 20;

    if (!MR::isDead(_B4C[_B50])) {
        _B4C[_B50]->destroy();
    }
}

void MarioActor::createIceWall(const TVec3f& rVec1, const TVec3f& rVec2) {
    TPos3f mtx;
    mtx.identity();
    TVec3f upVec;
    getUpVec(&upVec);

    MR::makeMtxFrontUp(&mtx, getGravityVector(), rVec2);

    TVec3f vec;
    mtx.getEuler(vec);
    vec *= _180_PI;
    _B4C[_B50]->setOn(_B50, rVec1, vec);

    _B50 = (_B50 + 1) % 20;

    if (!MR::isDead(_B4C[_B50])) {
        _B4C[_B50]->destroy();
    }
}

void MarioActor::updateBaseMtxTeresa(MtxPtr mtx) {
    TVec3f horizontal;
    f32 tilt = MR::clamp(MR::vecKillElement(mVelocity, getGravityVec(), &horizontal) / 10.0f, -1.0f, 1.0f);
    if (!MR::isNearZero(mMario->getWorldPadDir())) {
        const TVec3f& front = mMario->mFrontVec;
        if (MR::diffAngleAbsHorizontal(mMario->getWorldPadDir(), front, getGravityVec()) > 0.3926991f)
            tilt = 0.0f;
    }
    _9AC = 0.98f * _9AC + 0.02f * tilt;
    f32 angle = mConst->getTable()->mTeresaAngleDown;
    if (_9AC > 0.0f)
        angle = mConst->getTable()->mTeresaAngleUp;
    PSMTXConcat(mtx, MR::tmpMtxRotXRad(_9AC * angle), mtx);
}

bool MarioActor::finalizeFreezeModel() {
    if (MR::isBckStopped(_9C4)) {
        _9C4->kill();
        MR::offCalcAnim(_9C4);

        return false;
    }

    return true;
}

void MarioActor::offTakingFlag() {
    _480 = false;
}
