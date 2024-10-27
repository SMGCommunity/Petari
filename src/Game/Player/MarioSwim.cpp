#include "Game/Player/MarioSwim.hpp"
#include "Game/AreaObj/AreaObj.hpp"
#include "Game/Player/MarineSnow.hpp"
#include "Game/Player/MarioActor.hpp"
#include "Game/Player/MarioAnimator.hpp"
#include "Game/Player/MarioConst.hpp"
#include "Game/Screen/GameSceneLayoutHolder.hpp"
#include "Game/Util/SceneUtil.hpp"

namespace {
    f32 cFrontAcc[40] = {-0.2f, -0.2f, -0.2f, -0.1f, -0.1f, -0.1f, -0.1f, -0.1f, -0.0f, -0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.2f, 0.2f, 0.2f, 0.3f, 0.3f, 0.4f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.4f, 0.4f, 0.3f, 0.3f, 0.2f, 0.2f, 0.1f, 0.1f, 0.1f, 0.1f, 0.1f, 0.0f};

    f32 cFrontAccSpin[30] = {0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 8.0f, 6.0f, 4.0f, 2.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f};

    f32 cFrontAccSpinSurface[20] = {0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 8.0f, 6.0f, 4.0f, 2.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f};

    f32 cWeightTable[16] = {
        1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
    };

    f32 cWeightTableSP[4] = {0.0f, 0.0f, 0.75f, 0.25f};

    // In non-const mem region?

    f32 cLimitAngleSink = 0.0f;
    f32 cNeutralAngleWait = 0.0f;
    f32 cLimitAngleWait = 0.0f;
    f32 cUpperAngleWait = 0.0f;

    // In const mem region?
    const f32 cTurnMotionSpeed = 5.0f;

}    // namespace

bool Mario::isSwimming() const
{
    if (isStatusActive(6)) {
        return true;
    }
    return isStatusActive(24);
}

bool Mario::forceStartSwimAndShoot(const TVec3f &v)
{
    WaterInfo waterInfo;
    if (!MR::getWaterAreaObj(&waterInfo, mActor->mPosition)) {
        return false;
    }
    startSwim();
    mSwim->setDamage(v, 0x1E);
    return true;
}

bool Mario::forceExitSwim()
{
    if (!isStatusActive(6)) {
        return false;
    }
    mSwim->_9E = 6;
    closeStatus(mSwim);
    return true;
}

void MarioSwim::setDamage(const TVec3f &v, u16 damage)
{
    _A0 = v;
    _AE = damage;
    _4C = 0.0f;
    _50 = 0.0f;
    _54 = 0.0f;
    _7C = 0;
    _7A = 0;
    _34 = 0;
    _2E = 0;
    _32 = 0;
    _2C = damage;
    _30 = 0;
    _24 = 0x12C;
    if (_19) {
        _19 = 0;
    }
}

bool Mario::checkStartSwim()
{
    if (isStatusActive(6)) {
        return false;
    }
    if (isStatusActive(29)) {
        return false;
    }
    if (_10._7 && getMovementStates()._1) {
        getAnimator()->waterToGround();
        _10._7 = false;
    }
    if (mSwim->checkWaterCube(false)) {
        if (getPlayerMode() == 3) {
            TVec3f stack_14(-getGravityVec() % 100.0f);
            addTrans(stack_14, nullptr);
            if (getPlayer()->getMovementStates()._0) {
                stopJump();
            }
            if (getPlayer()->isStatusActive(1)) {
                getPlayer()->closeStatus(0);
            }
            if (getPlayer()->isStatusActive(4)) {
                getPlayer()->closeStatus(0);
            }
            return false;
        }
        startSwim();
        return true;
    }
    mSwim->updateUnderwater();
    return false;
}

void Mario::startSwim()
{
    if (!isStatusActive(6)) {
        if (getPlayer()->isDamaging()) {
            mSwim->_9D = 4;
            playSound("水落下突入", -1);
        }
        else if (getPlayer()->getMovementStates()._0) {
            mSwim->_9D = 1;
            playSound("水落下突入", -1);
        }
        else {
            mSwim->_9D = 0;
            playSound("水歩行突入", -1);
        }
        changeStatus(mSwim);
        clearSlope();
        stopWalk();
        stopJump();
        _735 = 0;
        mMovementStates._3E = 0;
        mMovementStates._12 = false;
        if (mMovementStates._8 || mMovementStates._1A || mMovementStates._19) {
            addVelocity(getWallNorm(), 50.0f);
        }
        getPlayer()->lockGroundCheck(mSwim, true);
        checkBaseTransBall();
        mActor->_F44 = false;
    }
}

MarioSwim::MarioSwim(MarioActor *actor) : MarioState(actor, 6), _F4()
{
    _18 = 0;
    _19 = 0;
    _1A = 0;
    _1B = 0;
    _1C = 0;
    _1D = 0;
    _1E = 0;
    _1F = 0;
    _20 = 0;
    _21 = 0;
    _22 = 0;
    _24 = 0;
    _28 = 0;
    _2C = 0;
    _2E = 0;
    _30 = 0;
    _32 = 0;
    _34 = 0;
    _36 = 0;
    _38 = 0;
    _3A = 0;
    _3C = 0;
    _3E = 0;
    _40 = 0;
    _42 = 0;
    _44 = 0;
    _48 = 0.0f;
    _4C = 0.0f;
    _50 = 0.0f;
    _54 = 0.0f;
    _58 = 0.0f;
    _5C = 0.0f;

    _60.x = 1.0f;
    _60.y = 0.0f;
    _60.z = 0.0f;

    _6C.x = 0.0f;
    _6C.y = 1.0f;
    _6C.z = 0.0f;

    _78 = 0;
    _7A = 0;
    _7C = 0;
    _80 = 0.0f;
    _84 = 0.0f;
    _88 = 0;
    _8A = 0;
    _8C = 0;
    _8E = 0;
    _90 = 0;
    _94 = 0.0f;
    _98 = 0.0f;
    _9C = 0;
    _9D = 0;
    _9E = 0;
    _9F = 0xFF;
    _A0.zero();
    _AC = 0;
    _AD = 0;
    _AE = 0;
    PSMTXIdentity(_B0.toMtxPtr());
    _E8 = 0;
    _E0 = 0.0f;
    _E4 = 0.0f;
    _EC = 0;
    _EE = 0x78;
    _F0 = 0;
    _144 = 0;
    _148.zero();
    _154.zero();
    _160.setInline(0.0f, 1.0f, 0.0f);
    _16C.zero();
    _178.zero();
    _184.zero();
    _190.zero();
    _1B0 = 0;
    _19C = 0.0f;
    _1A0 = 0.0f;
    _1A4 = 0.0f;
    _1A8 = 0.0f;
    _1AC = 0.0f;
    _1B2 = 0;
    _1B4 = 0.0f;
    _1B8 = 0.0f;
    _14 = new MarineSnow();
}

void MarioSwim::init()
{
    _EA = mActor->getConst().getConstants()->_528;
}

inline f32 getSpecialNumber()
{
    return 1.74532938004f;
}

bool MarioSwim::start()
{
    _7C = 0;
    _4C = 0.0f;
    _50 = 0.0f;
    _54 = 0.0f;
    _7A = 0;
    _E8 = 0;
    _EA = mActor->getConst().getConstants()->_528;
    _EE = 0x78;
    _F0 = 0;
    _AC = 0;
    _44 = 0;
    _EC = 0xB4;
    _42 = 0;
    _9E = 0;
    _8C = 0;
    _8E = 0;
    _98 = 0.0f;
    _3C = 0;
    _80 = 0.0f;
    _9F = 0xFF;
    _AE = 0;
    _A0.zero();
    _28 = 0;
    _E0 = 0.0f;
    _E4 = 0.0f;
    _1B0 = 0;
    _24 = 0;
    _48 = 0.0f;
    _3E = 0;
    _184.zero();
    _190.zero();
    _1B2 = 0;
    _1B4 = 0.0f;
    _1F = 0;
    _22 = 0;
    _58 = 0.0f;
    if (!getPlayer()->getMovementStates()._0 || getPlayer()->_488 <= 100.0f) {
        _58 = mActor->getConst().getConstants()->_500;
    }
    _9C = 1;
    if (checkLvlA()) {
        _9C = 0;
    }
    _5C = 0.523598790169f;
    onSurface();
    if ((_19C < -200.0f || mActor->_F74) && _19) {
        _19 = 0;
    }
    _1B = 0;
    u32 r1e = 0, r1d = 0;
    _1D = 0;
    _1E = 0;
    if (isAnimationRun("飛び込みジャンプ")) {
        r1e = 1;
    }
    if (isAnimationRun("後方飛び込みジャンプ")) {
        r1e = 1;
        getPlayer()->setFrontVecKeepUp(-getFrontVec(), -1.0f);
    }
    if (!mActor->_468.x) {
        _8A = 0;
    }
    if (_9D == 4) {
        r1d = 1;
    }
    if (_8A && getPlayer()->getMovementStates()._0) {
        if (!getPlayer()->_10._1E) {
            r1e = 2;
        }
        else {
            getPlayer()->mMovementStates._0 = false;
        }
    }
    getPlayer()->_10._1E = false;
    if (isAnimationRun("リングダッシュ")) {
        r1e = 2;
    }
    if (isAnimationRun("水泳スピン移動")) {
        r1e = 3;
    }
    if (isAnimationRun("水泳スピンジャンプ")) {
        r1e = 3;
    }
    _60 = getFrontVec();
    MR::normalize(&_60);
    stopAnimationUpper(nullptr, nullptr);
    changeAnimation(nullptr, "水泳基本");
    changeAnimationNonStop("水泳基本");
    getPlayer()->mMovementStates._1 = false;
    f32 fr1f = _1A4;
    _2C = 0;
    _32 = 0;
    _2E = 0;
    _34 = 0;
    _36 = 0;
    _38 = 0;
    _3A = 0;
    _1C = 0;
    if (fr1f < -_19C) {
        MarioActor *actor = mActor;
        actor->emitEffectWaterColumn(-_160, getTrans() - _160 % fr1f);
    }
    else {
        mActor->emitEffectWaterColumn(_160, _16C);
    }
    if (!getPlayer()->getMovementStates()._0 || getPlayer()->_488 > 100.0f || r1e) {
        TVec3f stack_44;
        _24 = 0;
        Mario *mario = getPlayer();
        MR::vecKillElement(mario->_2D4, getGravityVec(), &stack_44);
        fr1f = PSVECMag(stack_44.toCVec());
        if (getFrontVec().dot(stack_44) < 0.0f) {
            fr1f = 0.0f;
        }
        fr1f *= 0.1f;
        fr1f = MR::clamp(fr1f, 0.0f, 1.0f);
        if (_19) {
            _19 = 0;
        }
        if (_144 == 3) {
            f32 ftmp = mActor->getConst().getConstants()->_4B4;
            f32 ftmp2 = (0.2f + 0.1f * fr1f);
            _34 = 0x3C;
            _54 = ftmp2 * ftmp;
        }
        else if (getPlayer()->getMovementStates()._B || r1e) {
            _54 = 10.0f;
            if (_8A) {
                changeAnimationNonStop("水泳ジェット");
            }
            else {
                changeAnimation("水泳ジャンプダイブ回転", (const char *)nullptr);
            }
            playEffect("水面Ｚ沈降");
            switch (r1e) {
            case 0:
            case 1:
                _2C = mActor->getConst().getConstants()->_55C;
                _2E = mActor->getConst().getConstants()->_562;
                break;
            case 2:
                _2C = mActor->getConst().getConstants()->_55E;
                _2E = mActor->getConst().getConstants()->_564;
                break;
            case 3:
                _2C = mActor->getConst().getConstants()->_560;
                _2E = mActor->getConst().getConstants()->_566;
                break;
            }
            // Possibly related to similar operations? (eg getSwimValue(), etc)
            f32 ftmp2 = cLimitAngleSink * (1.0f - fr1f);
            _5C = fr1f * getSpecialNumber() + ftmp2;
            TVec3f stack_38(_60);
            stack_38.y = -10.0f;
            MR::normalize(&stack_38);
            getPlayer()->forceSetHeadVecKeepSide(stack_38);
        }
        else if (r1d) {
            changeAnimation("水上ダメージ着水", (const char *)nullptr);
            _2E = mActor->getConst().getConstants()->_51E;
            _AE = 0x78;
        }
        else {
            _54 = (0.2f + 0.1f * fr1f) * mActor->getConst().getConstants()->_4B4;
            changeAnimation("水泳ジャンプダイブ", (const char *)nullptr);
            mActor->setBlendMtxTimer(8);
            _2C = 15;
            _2E = mActor->getConst().getConstants()->_51C;
            _20 = 1;
            _5C = 1.49599659443f;
            if (getPlayer()->_488 < 200.0f) {
                f32 ftmp = (getPlayer()->_488 / 200.0f);
                _2C *= ftmp;
                _2E *= ftmp;
                mActor->setBlendMtxTimer(8.0f * ftmp);
            }
        }
    }
    else if (_19) {
        _AE = 0x5A;
        MR::vecKillElement(mActor->getLastMove(), _6C, &_A0);
        _A0.scaleInline(0.5f);
    }
    if (mActor->_468.x) {
        if (!_8A) {
            if (mActor->getCarrySensor().isType(15) || mActor->getCarrySensor().isType(16)) {
                u32 r1b = 0;
                if (mActor->getCarrySensor().isType(16)) {
                    r1b = 1;
                }
                startJet(r1b);
                r1e = 2;
                _20 = 0;
                _5C = 2.09439516068f;
                if (_19) {
                    _19 = 0;
                }
            }
        }
        else {
            r1e = 2;
        }
    }
    if (r1e == 2) {
        if (!isAnimationRun("水泳ジェット")) {
            changeAnimation("水泳ジェット開始", "水泳ジェット");
        }
        else {
            changeAnimation(nullptr, "水泳ジェット");
        }
    }
    updateLifeByTime();
    if (!getPlayer()->_1C._3) {
        MR::getGameSceneLayoutHolder().changeLifeMeterModeWater();
    }
    _9E = 0;
    return true;
}

static inline f32 getTwoPi()
{
    return 6.28318548203f;
}

static inline f32 funConversions1(u32 a, u16 tmp)
{
    return a * getTwoPi() / tmp;
}

static inline f32 funVecTime(const MarioSwim &self)
{
    return self.getTrans().y - self._19C;
}

static inline f32 funCalcTime()
{
    f32 tmp = 0.0f;    // This line is included only to fix a register allocation mismatch
    return 40.0f;
}

f32 MarioSwim::getSurface() const
{
    f32 fr1f = funVecTime(*this);
    f32 fr1e = funCalcTime();
    f32 fr1d = 0.00999999977648f;
    f32 fr1c = 0.0299999993294f;
    u16 tmp = 1800;
    fr1d = 1.0f + fr1d * MR::sin(funConversions1(_24, tmp));
    tmp = 500;
    f32 ftmp = fr1c * MR::sin(funConversions1(_24, tmp));
    tmp = 180;

    // I love sin composition
    return fr1f + fr1d * (fr1e * (MR::sin(ftmp + funConversions1(_24, tmp)) - 1.0f));
}

inline f32 procAngle(f32 l, f32 r)
{
    return l * (1.0f - r);
}

// possibly related to other calculations that take similar form? (eg getSwimValue)
inline f32 calculate(f32 stick)
{
    return cLimitAngleSink + (cLimitAngleWait - cLimitAngleSink) * stick;
}

static inline TVec3f createAndScale(f32 scalar, const TVec3f &v)
{
    TVec3f ret(v);
    ret.scale(scalar);
    return ret;
}

bool MarioSwim::update()
{
    bool r1e = false;
    _24++;
    if (_22) {
        _9E = 5;
        return false;
    }
    if (!checkWaterCube(false)) {
        if (_9E == 1 || _9E == 5) {
            return false;
        }
        const TVec3f &rAirGravityVec = getAirGravityVec();
        TVec3f stack_194;
        MR::vecKillElement(_16C - getTrans(), rAirGravityVec, &stack_194);
        if (PSVECMag(stack_194.toCVec()) > 100.0f || -_19C >= getPlayer()->_488 || _AE || _1B0) {
            if (!isAnimationRun("水泳ジャンプダイブ")) {
                if (_8A) {
                    doJetJump(1);
                    return false;
                }
                else {
                    _9E = 3;
                    return false;
                }
            }
        }
        else {
            _144 = 1;
        }
    }
    if (getPlayer()->mDrawStates._F && _19C > 0.963775992393f && MR::diffAngleAbsHorizontal(-getPlayer380(), getWorldPadDir(), getPlayer()->getAirGravityVec()) < 0.196349546313f) {
        if (calcAngleD(getPlayer380()) >= 30.0f) {
            _9E = 5;
            return false;
        }
        else {
            _9E = 1;
            return false;
        }
    }
    updateLifeByTime();
    if (_18) {
        onSurface();
        _2E = 0;
        _34 = 0;
        _32 = 0;
        _2C = 0;
        _4C = 0.0f;
    }
    _6C = -getPlayer()->getAirGravityVec();
    f32 fTmp = getSurface() - 30.0f;
    if (_1B0) {
        _1B0--;
    }
    if (_19 || (_20 && checkTrgA())) {
        if (_7A || _7C) {
            _9C = 0;
        }
        else {
            if (_8A) {
                if (checkTrgA()) {
                    doJetJump(0);
                    return false;
                }
                if (getStickY() <= 0) {
                    _5C = 0.00000381469726562f;
                    _4C = 1.0f;
                }
            }
            else {
                u32 tmp = 0x1E;
                if (isAnimationRun("水上一掻き")) {
                    tmp = 0x1E;
                }
                if (checkLvlA()) {
                    if (_9C && (!isAnimationRun("水上一掻き") || !_28)) {
                        _28++;
                    }
                    if ((_1B0 || _8A || getPlayer()->mMovementStates._8 || getPlayer()->mMovementStates._32) && checkTrgA()) {
                        _28 = tmp;
                    }
                }
                else {
                    _9C = 1;
                    if (_28 && _28 < tmp) {
                        surfacePaddle();
                    }
                    _28 = 0;
                    _1C = 0;
                }
                if (mActor->isRequestSpinJump2P()) {
                    _28 = tmp;
                    getPlayer()->_1C._A = true;
                    getPlayer()->_10._1F = true;
                }
                bool tmp2 = false;
                if ((!_32 || (_7A || _7C)) && (_28 >= tmp || _1C)) {
                    tmp2 = true;
                }
                if (mActor->isRequestSpin()) {
                    tmp2 = true;
                }
                if (tmp2) {
                    if (_24 > 6) {
                        f32 res = MR::clamp(_54 / mActor->getConst().getConstants()->_520, 0.0f, 1.0f);
                        getPlayer()->mMovementStates._5 = false;
                        getPlayer()->_278 = res;
                        getPlayer()->tryJump();
                        _9E = 2;
                        return false;
                    }
                    _1C = 1;
                }
            }
        }
        flowOnWave(fTmp);
    }
    else if (_19C > -30.0f) {
        if (!_2C && !_32 && !_2E && !_8A) {
            getAnimator()->forceSetBlendWeight(cWeightTable);
            changeAnimation("炊泳上昇呼吸", (const char *)nullptr);
        }
        if (!_2E) {
            onSurface();
            _2C = 0;
        }
        if (_7C > 10) {
            f32 res = MR::clamp(_54 / mActor->getConst().getConstants()->_520, 0.0f, 1.0f);
            getPlayer()->mMovementStates._5 = false;
            getPlayer()->_278 = res;
            TVec3f stack_188;
            mActor->getLastMove(&stack_188);
            f32 res2 = MR::vecKillElement(stack_188, getGravityVec(), &stack_188);
            if (res2 > -10.0f) {
                res2 = -10.0f;
            }
            stack_188 += getGravityVec() % res2;
            getPlayer()->tryForcePowerJump(stack_188, false);
            _9E = 2;
            if (isAnimationRun("水泳スピン移動")) {
                changeAnimationNonStop("水泳スピン移動");
            }
            else {
                changeAnimationNonStop("水泳スピンジャンプ");
            }
            return false;
        }
        if (_8A && (getStickY() < -0.1f || checkLvlA()) && getPlayer()->_488 - _19C > 200.0f) {
            doJetJump(0);
            return false;
        }
        if (_8C && getPlayer()->_488 - _19C > 200.0f) {
            f32 res = MR::clamp(_54 / mActor->getConst().getConstants()->_520, 0.0f, 1.0f);
            getPlayer()->mMovementStates._5 = false;
            getPlayer()->_278 = res;
            getPlayer()->tryForcePowerJump((mActor->getLastMove() % 2.0f).translate(_6C % 10.0f), false);
            _9E = 2;
            changeAnimationNonStop("リングダッシュ");
            return false;
        }
    }
    else {
        checkWaterBottom();
        if (getStickY() >= 0.0f && (_1F || (_1A4 < 60.0f && _54 > 1.0f))) {
            const TVec3f &rShadowNorm = getPlayer()->getShadowNorm();
            TVec3f stack_17C, stack_170;
            PSVECCrossProduct(_60.toCVec(), rShadowNorm.toCVec(), stack_17C.toVec());
            PSVECCrossProduct(rShadowNorm.toCVec(), stack_17C.toCVec(), stack_170.toVec());
            if (-_19C + _1A4 > 1000.0f) {
                _1A = 1;
            }
            f32 dot1 = _60.dot(stack_170);
            f32 dot2 = _60.dot(rShadowNorm);
            if (dot2 >= 0.0f) {
                _5C = 1.57079637051f + 0.9f * marioAcos(dot1);
            }
            else if (_54 > 1.0f) {
                _5C = 1.57079637051f - 0.5f * marioAcos(dot1);
            }
            else {
                f32 angle = marioAcos(dot1);
                _38 = 0x3C;
                _5C = 1.57079637051f - angle;
            }
            if (_8A && getPlayer()->_488 < 100.0f) {
                _42 = 2;
                playEffect("水底接触");
                _54 *= mActor->getConst().getConstants()->_53C;
            }
        }
        _1F = 0;
    }
    if (_42) {
        playSound("水底接触", -1);
        if (--_42 == 0) {
            stopEffect("水底接触");
        }
    }
    if (_32 && --_32 == 0) {
        r1e = true;
        _5C = mActor->getConst().getConstants()->_54C;
        _2C = mActor->getConst().getConstants()->_550;
        _2E = mActor->getConst().getConstants()->_552;
        _54 = mActor->getConst().getConstants()->_558;
        _38 = 0x14;
    }
    f32 fr1f = 1.0f - MR::clamp(_54 / mActor->getConst().getConstants()->_4B4, 0.0f, 1.0f);
    if (_1A4 - _19C < 400.0f) {
        fr1f = 0.0f;
    }
    if (_38) {
        _38--;
    }
    else {
        f32 res = fr1f;
        if (_19 && !_32) {
            if (!_18) {
                if (_8A) {
                    if (getStickY() > 0.707000017166f) {
                        _5C = mActor->getConst().getConstants()->_54C;
                        _20 = 0;
                        _1E = 0;
                    }

                    f32 a;
                    f32 b;
                    b = getSwimValue(a = getStickY(), 2, mActor->getConst().getConstants());
                    _4C = _4C * b + procAngle(a, b);

                    _50 = 2.5f * (getStickX() * (1.0f - mActor->getConst().getConstants()->_4EC[3]));
                    f32 fr1c = 1.0f;
                    if (getStickY() < 0.0f) {
                        fr1c += 2.0f * -getStickY();
                    }
                    _50 *= fr1c;
                }
                else if (checkTrgZ() && !check7Aand7C()) {
                    stopAnimationUpper("水泳水面初期移動", nullptr);
                    changeAnimation("水泳潜り", (const char *)nullptr);
                    playEffect("水面Ｚ沈降");
                    playSound("水面潜り", -1);
                    _32 = mActor->getConst().getConstants()->_554;
                }
                else if (checkLvlZ() == 0) {
                    res = 0.0f;
                }
                else {
                    f32 a, b;
                    b = getSwimValue(a = getStickP(), 0, mActor->getConst().getConstants());
                    _4C = _4C * b + procAngle(a, b);
                }
            }
            _50 *= mActor->getConst().getConstants()->_4EC[1];
        }
        else {
            if (!_8C && _1A4 > 200.0f && checkTrgZ() && !_32 && !_2E && !_8A && !check7Aand7C()) {
                stopAnimation(nullptr, (const char *)nullptr);
                changeAnimation("水泳潜り", (const char *)nullptr);
                playSound("水中潜り", -1);
                playEffect("水面Ｚ沈降");
                _32 = mActor->getConst().getConstants()->_554;
            }
            if (_8A) {
                f32 a;
                f32 b;
                b = getSwimValue(a = getStickY(), 2, mActor->getConst().getConstants());
                _4C = _4C * b + procAngle(a, b);
                b = getSwimValue(a = getStickX(), 3, mActor->getConst().getConstants());
                _50 = _50 * b + procAngle(a, b);
            }
            else {
                f32 a, b;
                b = getSwimValue(a = getStickY(), 0, mActor->getConst().getConstants());
                _4C = _4C * b + procAngle(a, b);
                b = getSwimValue(a = getStickX(), 1, mActor->getConst().getConstants());
                _50 = _50 * b + procAngle(a, b);
            }
        }
        if (!_20 && !_3C && _2C) {
            _5C += res * (_4C * mActor->getConst().getConstants()->_4E0);
        }

        if ((_54 < cTurnMotionSpeed || _19 || checkLvlA() || checkLvlZ()) && isAnimationRun("水泳一掻き") && isAnimationTerminate("水泳一掻き")) {
            stopAnimation(nullptr, (const char *)nullptr);
        }

        if (!checkLvlA() && !checkLvlZ() && !_2C && _54 < cTurnMotionSpeed) {
            bool worthless;
            bool &worthlesser = worthless;
            funReferenceTime(worthlesser);
            if (!_20 && !_AE && _19C < -400.0f) {
                _3C++;
                if (MR::getRandom() < 0.03f) {
                    playSound("水中ウエイト", -1);
                }
            }
            if (_3C >= 0x78) {
                _3C = 0x78;
            }
        }
        else {
            _3C = 0;
        }
        f32 fr1d;
        if (MR::isNearZero(getStickY(), 0.1f)) {
            f32 num = _3C / 120.0f;
            if (num > 1.0f) {
                num = 1.0f;
            }
            fr1d = _5C + num * (cNeutralAngleWait - _5C);
        }
        else if (getStickY() > 0.0f) {
            f32 fr4 = 0.0f;
            f32 num = _3C / 120.0f;
            if (_54 < 2.0f) {
                fr4 = 1.0f;
            }
            else {
                bool confusion;
                bool &confusionion = confusion;
                if (_54 > 10.0f) {
                    confusionion = true;
                }
                else {
                    fr4 = 1.0f - (_54 - 2.0f) / 8;
                }
            }
            if (num < fr4) {
                num = fr4;
            }
            f32 numClamped = MR::clamp(num, 0.0f, 1.0f);
            f32 fr1c = numClamped * cLimitAngleWait + (1.0f - numClamped) * cLimitAngleSink;
            if (_1A4 < 300.0f && -_19C < 100.0f) {
                fr1c = cNeutralAngleWait;
            }
            fr1d = cNeutralAngleWait + (fr1c - cNeutralAngleWait) * getStickY();
        }
        else if (getStickY() < 0.0f) {
            fr1d = cLimitAngleSink + (cLimitAngleWait - cLimitAngleSink) * -getStickY();
        }
        f32 fr1e = 0.05f;
        if (_54 > 5.0f && (fr1e -= fr1e * (5.0f / _54)) < 0.0f) {
            fr1e = 0.0f;
        }
        if (_8A) {
            switch (_88) {
            case 0:
                fr1e *= mActor->getConst().getConstants()->_52C;
                break;
            default:
                fr1e *= mActor->getConst().getConstants()->_530;
                break;
            }
        }
        if (_20) {
            fr1d = 0.523598790169f;
            fr1e = 0.01f;
        }
        if (_19) {
            fr1d = 0.523598790169f;
        }
        if (_32) {
            fr1e = 0.15f;
            fr1d = mActor->getConst().getConstants()->_54C;
        }
        bool r1b = true;
        _5C = _5C * (1.0f - fr1e) + fr1d * fr1e;
        if (_3C && !_AE && !_32 && getStickY() > 0.0f) {
            f32 fr1c = 3.14159274101f / mActor->getConst().getConstants()->_5B8;
            f32 tmpsticky = getStickY();
            fr1d = 0.523598790169f + tmpsticky * (fr1c - 0.523598790169f);
            if (_5C < fr1d) {
                _5C = _5C * mActor->getConst().getConstants()->_5BC + fr1d * (1.0f - mActor->getConst().getConstants()->_5BC);
            }
            else {
                _5C = _5C * (1.0f - fr1e) + fr1d * fr1e;
            }
            if (!check7Aand7C() && !_8A) {
                changeAnimation("水泳ターン下", (const char *)nullptr);
            }
            r1b = false;
        }
        if (r1b) {
            stopAnimation("水泳ターン下", (const char *)nullptr);
        }
        if (_32) {
            _5C = MR::clamp(_5C, 0.523598790169f, mActor->getConst().getConstants()->_54C);
        }
        else {
            _5C = MR::clamp(_5C, cLimitAngleWait, cUpperAngleWait);
        }
        if (MR::isStageSwimAngleLimit()) {
            _5C = MR::clamp(_5C, 0.872664690018f, 2.26892805099f);
        }
    }

    if ((_20 || (_19 && isStickOn())) && !_8A) {
        TVec3f stack_164(getWorldPadDir());
        f32 tmp;
        if (_19) {
            tmp = 0.01f * (12.0f - _54);
            if (tmp <= 0.01f) {
                tmp = 0.01f;
            }
        }
        else {
            tmp = 0.01f * (10.0f - _54);
        }
        if (_8A) {
            tmp *= 3.0f;
        }
        f32 res = MR::clamp(tmp * mActor->getConst().getConstants()->_518, 0.0f, 1.0f);
        f32 diffAngle = MR::diffAngleAbs(_60, stack_164);
        if (diffAngle > 0.0f) {
            if (diffAngle < res) {
                _60 = stack_164;
            }
            else {
                MR::vecBlendSphere(_60, stack_164, &_60, res / diffAngle);
            }
        }
    }
    else {
        f32 tmp = fr1f + mActor->getConst().getConstants()->_4E8;
        MR::rotAxisVecRad(_60, -_6C, &_60, tmp * (_50 * mActor->mConst->getConstants()->_4E4));
    }
    TVec3f stack_158(_60);
    MR::vecKillElement(_60, _6C, &_60);
    TVec3f stack_14C, stack_140;
    if (!MR::normalizeOrZero(&_60)) {
        PSVECCrossProduct(_6C.toCVec(), _60.toCVec(), stack_140.toVec());
        MR::normalize(&stack_140);
        getPlayer()->setSideVec(stack_140);
        MR::rotAxisVecRad(_60, stack_140, &stack_14C, _5C);
        TVec3f stack_134(getPlayer()->_1F0);
        TVec3f stack_128(getFrontVec());
        TVec3f stack_11C;
        f32 something = 0.1f;
        if (r1e) {
            something = 1.0f;
        }
        MR::vecBlendSphere(stack_128, stack_14C, &stack_11C, something);
        getPlayer()->setFrontVecKeepSide(stack_11C);
    }
    else {
        stack_14C = getFrontVec();
        stack_140 = getPlayer()->_310;
    }
    spin();
    TVec3f stack_110 = getPlayer()->_1FC;
    if (_34 || _2E || isAnimationRun("水泳ジャンプダイブ回転") || isAnimationRun("水泳潜り")) {
        PSVECCrossProduct(stack_14C.toCVec(), stack_140.toCVec(), stack_110.toVec());
    }
    if (!_8A) {
        decideVelocity();
    }
    TVec3f stack_104 = createAndScale(_54, stack_110);
    if (_19 && _6C.dot(stack_110) > 0.0f) {
        f32 mag = PSVECMag(stack_104.toCVec());
        MR::vecKillElement(stack_104, _6C, &stack_104);
        stack_104.setLength(mag);
    }
    f32 element = MR::vecKillElement(stack_104, _6C, &stack_104);
    f32 fr2;
    if (_30 > 0x19) {
        fr2 = (50 - _30) / 25.0f;
    }
    else {
        fr2 = _30 / 25.0f;
    }
    f32 fr1d = fr2 + (1.0f - fr2) * mActor->getConst().getConstants()->_514;
    stack_104 += _6C % element % fr1d;
    addVelocity(stack_104);
    u16 fr1b = _2E;
    if (fr1b) {
        addVelocity(getGravityVec(), 0.25f * fr1b);
        _2E--;
    }

    if (_34) {
        _34--;
    }
    if (!_19 && !_2C && !_8A) {
        procBuoyancy();
    }
    else if (_6C.dot(stack_110) < 0.0f) {
        if (_19) {
            _19 = 0;
        }
    }
    else if (_19) {
        AreaObj *obj = MR::getAreaObj("WaterArea", getTrans().translate(getGravityVec() % 100f));
        if (obj) {
            TPos3f *followMtx = obj->getFollowMtx();
            if (followMtx) {
                TVec3f stack_F8;
                MR::extractMtxTrans(followMtx->toMtxPtr(), &stack_F8);
                bool cond = JGeometry::TUtil<f32>::epsilonEquals(stack_F8.x, _154.x, 0.000003814697265625f) && JGeometry::TUtil<f32>::epsilonEquals(stack_F8.y, _154.y, 0.000003814697265625f) && JGeometry::TUtil<f32>::epsilonEquals(stack_F8.z, _154.z, 0.000003814697265625f);
                if (!cond) {
                    if (PSVECMag((stack_F8 - _154).toCVec()) < 10.0f) {
                        addVelocity(stack_F8 - _154);
                    }
                    _154 = stack_F8;
                }
            }
        }
    }
    if (_AE) {
        _AE--;
        addVelocity(_A0);
        if (_AE < 0x78) {
            _A0.scaleInline(0.98f);
        }
    }
    if (_8A) {
        addVelocity(_184, mActor->getConst().getConstants()->_534);
    }
    else {
        addVelocity(_184);
    }
    TVec3f stack_EC(_184);
    f32 fr1dv2 = PSVECMag(stack_EC.toCVec());
    if (!MR::normalizeOrZero(&stack_EC)) {
        if (MR::diffAngleAbs(stack_EC, _60) < 1.57079637051f) {
            MR::vecBlendSphere(_60, stack_EC, &_60, 0.001f * fr1dv2);
        }
        else {
            MR::vecBlendSphere(_60, -stack_EC, &_60, 0.001f * fr1dv2);
        }
    }
    TVec3f stack_E0;
    if (MR::calcWhirlPoolAccelInfo(getTrans(), &stack_E0) && !MR::isNearZero(stack_E0, 0.001f)) {
        _184 += stack_E0;
        TVec3f stack_D4(stack_E0);
        MR::normalizeOrZero(&stack_D4);
        MR::vecBlendSphere(_60, stack_D4, &_60, 0.1f);
    }
    if (!_8A && !_19 && !MR::isNearZero(_190, 0.001f)) {
        f32 angleDiff = MR::diffAngleSignedHorizontal(_190, _60, getPlayer()->getAirGravityVec());
        f32 cmp = 1.04719758034f;
        if (angleDiff > cmp) {
            Mtx stack_1D0;
            PSMTXRotAxisRad(stack_1D0, _6C.toCVec(), angleDiff - 1.04719758034f);
            PSMTXMultVecSR(stack_1D0, _60.toCVec(), _60.toVec());
        }
        else if (angleDiff < -cmp) {
            Mtx stack_1A0;
            PSMTXRotAxisRad(stack_1A0, _6C.toCVec(), -(-angleDiff - 1.04719758034f));
            PSMTXMultVecSR(stack_1A0, _60.toCVec(), _60.toVec());
        }
    }
    decideAnimation();
    decideEffect(false);
    if (_2C) {
        _2C--;
    }
    if (_30) {
        _30--;
    }
    if (_44) {
        _44--;
    }
    updateTilt();
    jet();
    pushedByWaterWall();
    return true;
}

/*
 *
 * ***********************************************
 * IMPORTANT: THIS FUNCTION SHOULD BE DELETED ONCE
 * THIS OBJECT FILE MATCHES
 * ***********************************************
 *
 * This function is only present so that the
 * compiler does not cull these variables
 *
 */
void dummyFunction()
{
    cFrontAcc[0] = 1.0f;
    cFrontAccSpin[0] = 1.0f;
    cFrontAccSpinSurface[0] = 1.0f;
    cWeightTableSP[0] = 1.0f;
};
