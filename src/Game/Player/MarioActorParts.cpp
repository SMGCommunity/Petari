#include "Game/Animation/XanimeCore.hpp"
#include "Game/MapObj/CollectCounter.hpp"
#include "Game/MapObj/IceStep.hpp"
#include "Game/Player/FireMarioBall.hpp"
#include "Game/Player/JetTurtleShadow.hpp"
#include "Game/Player/MarioActor.hpp"
#include "Game/Player/MarioConst.hpp"
#include "Game/Player/MarioNullBck.hpp"
#include "Game/Player/MarioParts.hpp"
#include "Game/Player/MarioState.hpp"
#include "Game/Player/MarioSwim.hpp"
#include "Game/Player/TornadoMario.hpp"
#include "Game/Screen/GameSceneLayoutHolder.hpp"
#include "Game/Util/EffectUtil.hpp"
#include "Game/Util/JointUtil.hpp"
#include "Game/Util/LightUtil.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/MathUtil.hpp"
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

    if (mMario->checkLvlA() && mMario->_402 && mMario->getMovementStates().jumping) {
        if (_9F0 != 1) {
            MR::startBck(_9E8, "Fly", nullptr);
            MR::startBva(_9E8, "Fly");
            MR::startBtk(_9E8, "Fly");
        }

        _9F0 = 1;
        return;
    }

    s8 val;
    if (mMario->getMovementStates()._1 || mMario->isStatusActive(MarioStatus_Stick)) {
        val = 0;
    } else {
        val = mMario->_402 == 0;
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

// void MarioActor::updateTakingPosition() {}

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

// void MarioActor::updateSpecialModeAnimation() {}

void MarioActor::initFireBall() {
    for (u32 idx = 0; idx < ARRAY_SIZE(_B54); idx++) {
        _B54[idx] = new FireMarioBall("マリオ炎球");
        _B54[idx]->initWithoutIter();
    }
}

// void MarioActor::shootFireBall() {}

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

//void MarioActor::updateFairyStar() {}

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

//void MarioActor::updateThrowVector() {}

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

//void MarioActor::updateBaseMtxTeresa(MtxPtr) {}

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
