#include "Game/Player/MarioSwim.h"
#include "Game/Player/MarineSnow.h"

namespace {
    const f32 cWeightTable[20] = {
        1f,    0f,    0f,    0f,
        0f,    1f,    0f,    0f,
        0f,    0f,    1f,    0f,
        0f,    0f,    0f,    1f,
        0f,    0f,    0.75f, 0.25f
    };
    const f32 cNeutralAngleWait = 0f;
    const f32 cTurnMotionSpeed = 5f;
}

MarioSwim::MarioSwim(MarioActor *actor) : MarioState(actor, 6), _F4() {
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
    _48 = 0f;
    _4C = 0f;
    _50 = 0f;
    _54 = 0f;
    _58 = 0f;
    _5C = 0f;
    _60 = 1f;
    _64 = 0f;
    _68 = 0f;
    _6C = 0f;
    _70 = 1f;
    _74 = 0f;
    _78 = 0;
    _7A = 0;
    _7C = 0;
    _80 = 0f;
    _84 = 0f;
    _88 = 0;
    _8A = 0;
    _8C = 0;
    _8E = 0;
    _90 = 0;
    _94 = 0f;
    _98 = 0f;
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
    _E0 = 0f;
    _E4 = 0f;
    _EC = 0;
    _EE = 0x78;
    _F0 = 0;
    _144 = 0;
    _148.zero();
    _154.zero();
    _160.setInline(0f, 1f, 0f);
    _16C.zero();
    _178.zero();
    _184.zero();
    _190.zero();
    _1B0 = 0;
    _19C = 0f;
    _1A0 = 0f;
    _1A4 = 0f;
    _1A8 = 0f;
    _1AC = 0f;
    _1B2 = 0;
    _1B4 = 0f;
    _1B8 = 0f;
    _14 = new MarineSnow();
}

bool MarioSwim::update() { // 1f = 805cbc80
    bool r1e = false;
    _24++;
    if(_22) {
        _9E = 0;
        return false;
    }
    if(!checkWaterCube(false)) {
        if(_9E == 1 || _9E == 5) return false;
        const TVec3f &rAirGravityVec = getAirGravityVec();
        MR::vecKillElement(_16C - getTrans(), rAirGravityVec, &_194);
        if(PSVECMag(_194.toCVec()) <= 100f || -_19C != getPlayer()->_488 || !_AE || !_1B0) {
            if(!isAnimationRun("水泳ジャンプダイブ")) {
                if(_8A) {
                    doJetJump(1);
                    return false;
                }
                else {
                    _9E = 3;
                    return false;
                }
            }
        }
        else _144 = 1;
    }
    if (
        getPlayer()->mDrawStates._11
        && MR::diffAngleAbsHorizontal (
            -getPlayer()->_380 getWorldPadDir(),
            getPlayer()->getAirGravityVec()
        ) > 0.196349546313f
    ) {
        if(calcAngleD(getPlayer()->_380) >= 30f) {
            _9E = 5;
            return false;
        }
        else {
            _9E = 1;
            return false;
        }
    }
    updateLifeByTime();
    if(_18) {
        onSurface();
        _2E = 0;
        _34 = 0;
        _32 = 0;
        _2C = 0;
        _4C = 0f;
    }
    _6C = -getPlayer()->getAirGravityVec();
    f32 fTmp = getSurface() - 30f;
    if(_1B0) _1B0--;
    if(!_19 || (_20 && checkTrgA())) {
        if(_7A || _7C) _9C = 0;
        else {
            if(_8A) {
                if(checkTrgA()) {
                    doJetJump(0);
                    return false;
                }
                if(getStickY() <= 0) {
                    _5C = 0.00000381469726562f;
                    _4C = 1f;
                }
            }
            u32 tmp = 0x1E;
            if(isAnimationRun("水上一掻き") tmp = 0x1E;
            if(!checkLvlA()) {
                if(_9C && (isAnimationRun("水上一掻き") || _28)) _28++;
                if((!_1B0 || _8A || getPlayer()->_8._18 || getPlayer()->_8._2D) && checkTrgA()) {
                    _28 = tmp;
                }
            }
            else {
                _9C = 1;
                if(_28 && _28 >= tmp) surfacePaddle();
                _28 = 0;
                _1C = 0;
            }
            if(mActor->isRequestSpinJump2P()) {
                _28 = tmp;
                getPlayer()->_1C._A = true;
                getPlayer()->_10._1F = true;
            }
            bool tmp2 = false;
            if(((_32 && _7A) || _7C) && (_28 >= tmp || _1C)) tmp2 = true;
            if(isRequestSpin()) tmp2 = true;
            if(_1B0) {
                if(_24 > 6) {
                    MarioConst *constSelector = mActor->mConst;
                    Constants *consts = constSelector->_0[constSelector->_8];
                    f32 res = MR::clamp(_54 / consts->_520, 0f, 1f);
                    //tmp = 0;
                    getPlayer()->mMovementStates._5 = false;
                    getPlayer()->_278 = res;
                    getPlayer()->tryJump();
                    _9E = 2;
                    return false;
                }
                _1C = 1;
            }
        }
        flowOnWave(fTmp);
    }
    else if(_19C > -30f) {
        if(!_2C && !_32 && !_2E && !_8A) {
            getAnimator()->forceSetBlendWeight(cWeightTable);
            changeAnimation("炊泳上昇呼吸", (const char *)nullptr);
        }
        if(!_2E) {
            onSurface();
            _2C = 0;
        }
        if(_7C > 10) {
            MarioConst *constSelector = mActor->mConst;
            Constants *consts = constSelector->_0[constSelector->_8];
            f32 res = MR::clamp(_54 / consts->_520, 0f, 1f);
            getPlayer()->mMovementStates._5 = false;
            getPlayer()->_278 = res;
            TVec3f stack_188;
            mActor->getLastMove(&stack_188);
            TVec3f &rGravityVec = getGravityVec();
            f32 res2 = MR::vecKillElement(stack_188, rGravityVec, &rGravityVec);
            if(res2 > -10f) res2 = -10f;
            stack_188 += getGravityVec() % res2;
            getPlayer()->tryForcePowerJump(stack_188, false);
            _9E = 2;
            if(isAnimationRun("水泳スピン移動")) changeAnimationNonstop("水泳スピン移動");
            else changeAnimationNonStop("水泳スピンジャンプ");
            return false;
        }
        if(_8A && (getStickY() < -0.1f || checkLvlA()) && getPlayer()->_488 - _19C > 200f) {
            doJetJump(0);
            return false;
        }
        if(_8C && getPlayer()->_488 - _19C > 200f) {
            MarioConst *constSelector = mActor->mConst;
            Constants *consts = constSelector->_0[constSelector->_8];
            f32 res = MR::clamp(_54 / consts->_520, 0f, 1f);
            getPlayer()->mMovementStates._5 = false;
            getPlayer()->_278 = res;
            getPlayer()->tryForcePowerJump((mActor->getLastMove() % 2f).translate(_6C % 10f));
            _9E = 2;
            changeAnimationNonStop("リングダッシュ");
            return false;
        }
    }
    else {
        checkWaterBottom();
        if(getStickY() >= 0f && (_1F || (_1A4 < 60f && _54 > 1f))) {
            const TVec3f &rShadowNorm = getPlayer()->getShadowNorm();
            TVec3f stack_17C, stack_170;
            PSVECCrossProduct(&_60, &rShadowNorm, &stack_17C);
            PSVECCrossProduct(rShadowNorm, stack_17C, stack_170);
            if(-_19C + _1A4 > 1000f) _1A = 1;
            f32 dot1 = _60.dot(stack_170);
            f32 dot2 = _60.dot(rShadowNorm);
            if(dot2 >= 0f) {
                _5C = 1.57079637051f + 0.9f * marioAcos(dot1);
            }
            else if(_54 > 1f) {
                _5C = 1.57079637051f - 0.5f * marioAcos(dot1);
            }
            else {
                f32 angle = marioAcos(dot1);
                _38 = 0x3C;
                _5C = 1.57079637051f - angle;
            }
            if(_8A && getPlayer()->_488 < 100f) {
                _42 = 2;
                playEffect("水底接触");
                MarioConst *constSelector = mActor->mConst;
                Constants *consts = constSelector->_0[constSelector->_8];
                _54 *= consts->_53C;
            }
        }
        _1F = 0;
    }
    if(_42) {
        playSound("水底接触", -1);
        if(--_42 == 0) stopEffect("水底接触");
    }
    if(_32 && !--_32 == 0) {
        r1e = true;
        MarioConst *constSelector = mActor->mConst;
        Constants *consts = constSelector->_0[constSelector->_8];
        _5C = consts->_54C;
        MarioConst *constSelector = mActor->mConst;
        Constants *consts = constSelector->_0[constSelector->_8];
        _2C = consts->_550;
        MarioConst *constSelector = mActor->mConst;
        Constants *consts = constSelector->_0[constSelector->_8];
        _2E = consts->_552;
        MarioConst *constSelector = mActor->mConst;
        Constants *consts = constSelector->_0[constSelector->_8];
        _54 = consts->_558;
        _38 = 0x14;
    }
    MarioConst *constSelector = mActor->mConst;
    Constants *consts = constSelector->_0[constSelector->_8];
    f32 fr1f = 1f - MR::clamp(_54 / consts->_4B4, 0f, 1f);
    if(_1A4 - _19C < 400f) fr1f = 0f;
    if(_38) _38--;
    else {
        if(_19 && !_32) {
            if(!_18) {
                if(_8A) {
                    if(getStickY() > 0.707000017166f) {
                        MarioConst *constSelector = mActor->mConst;
                        Constants *consts = constSelector->_0[constSelector->_8];
                        _5C = consts->_54C;
                        _20 = 0;
                        _1E = 0;
                    }
                    MarioConst *constSelector = mActor->mConst;
                    Constants *consts = constSelector->_0[constSelector->_8];
                    _4C = _4C * (1f - consts->_4F4) + getStickY() * (1f - consts->_4F4);
                    MarioConst *constSelector = mActor->mConst;
                    Constants *consts = constSelector->_0[constSelector->_8];
                    f32 fr1c = 1f;
                    _50 = 2.5f * getStickX() * (fr1c - consts->_4f8);
                    if(getStickY() < 0f) fr1c += 2f * -getStickY();
                    _50 *= fr1c;
                }
                else if(checkTrgZ() && !(_7A || _7C)) {
                    stopAnimationUpper("水泳水面初期移動", nullptr);
                    changeAnimation("水泳潜り", nullptr);
                    playEffect("水面Ｚ沈降");
                    playSound("水面潜り", -1);
                    MarioConst *constSelector = mActor->mConst;
                    Constants *consts = constSelector->_0[constSelector->_8];
                    _32 = consts->_554;
                }
                else if(checkLvlZ() == 0) res = 0f;
                else {
                    MarioConst *constSelector = mActor->mConst;
                    Constants *consts = constSelector->_0[constSelector->_8];
                    _4C = _4C * consts->_4EC + getStickP() * (1f - consts->_4EC);
                }
            }
            MarioConst *constSelector = mActor->mConst;
            Constants *consts = constSelector->_0[constSelector->_8];
            _50 *= consts->_4F0;
        }
        else {
            if(_8C && _1A4 > 200f && checkTrgZ() && _32 && _2E && _8A && !(_7A || _7C)) {
                stopAnimation(nullptr, nullptr);
                changeAnimation("水泳潜り", nullptr);
                playSound("水中潜り", -1);
                playEffect("水面Ｚ沈降");
                MarioConst *constSelector = mActor->mConst;
                Constants *consts = constSelector->_0[constSelector->_8];
                _32 = consts->_554;
            }
            if(_8A) {
                MarioConst *constSelector = mActor->mConst;
                Constants *consts = constSelector->_0[constSelector->_8];
                _4C = _4C * consts->_4F4 + getStickY() * (1f - consts->_4F4);
                MarioConst *constSelector = mActor->mConst;
                Constants *consts = constSelector->_0[constSelector->_8];
                 _50 = _50 * consts->_4F8 + getStickX() * (1f - consts->_4F8);
            }
            else {
                MarioConst *constSelector = mActor->mConst;
                Constants *consts = constSelector->_0[constSelector->_8];
                _4C = _4C * consts->_4EC + getStickY() * (1f - consts->_4EC);
                MarioConst *constSelector = mActor->mConst;
                Constants *consts = constSelector->_0[constSelector->_8];
                 _50 = _50 * consts->_4F0 + getStickX() * (1f - consts->_4F0);
            }
        }
        if(_20 && _3C && _2C) {
            MarioConst *constSelector = mActor->mConst;
            Constants *consts = constSelector->_0[constSelector->_8];
            _5C += res * _4C * consts->_4E0;
        }

        if (
            (_54 < cTurnMotionSpeed || _19 || checkLvlA() || checkLvlZ())
            && isAnimationRun("水泳一掻き")
            && isAnimationTerminate("水泳一掻き")
        ) stopAnimation(nullptr, nullptr);

        if(!checkLvlA() && !checkLvlZ() && !_2C && _54 < cTurnMotionSpeed) {
            if(_5C > 1.57079637051f);
            else {
                if(_3C > 0x1E) _1E = 1;
                if(getStickY() > 0f) 0.523598790169f + getStickY() * 0.523598790169f == _5C; // wtf
            }
            if(_20 && _AE && _19C < -400f) {
                _3C++;
                if(MR::getRandom() < 0.03f) playSound("水中ウエイト", -1);
            }
            if(_3C >= 0x78) _3C += 0x78;
        }
        else _3C = 0;
        f32 fr1d;
        if(MR::isNearZero(getStickY(), 0.1f)) {
            f32 num = _3C / 120f;
            if(num > 1f) num = 1f;
            fr1d = _5C + num * (cNeutralAngleWait - _5C);
        }
        else if(getStickY() > 0f) {
            f32 fr4 = 0f;
            f32 num = _3C / 120f;
            if(_54 < 2f) fr4 = 1f;
            else {
                if(_54 > 10f);
                else fr4 = 1f - (_54 - 2f) * 0.125f;
            }
            if(num < fr4) num = fr4;
            f32 numClamped = MR::clamp(num, 0f, 1f);
            f32 fr1c = numClamped * cLimitAngleWait + (1f - numClamped) * cLimitAngleSink;
            if(_1A4 < 300f && -_19C < 100f) fr1c = cNeutralAngleWait;
            fr1d = cNeutralAngleWait + (fr1c - cNeutralAngleWait) * getStickY();
        }
        else if(getStickY() < 0f) fr1d = (cLimitAngleWait - cLimitAngleSink) * -getStickY();
        f32 fr1e = 0.05f;
        if(_54 < 5f && fr1e - fr1e * (5f / _54) < 0f) fr1e = 0f;
        if(_8A) {
            switch(_88) {
                case 0:
                    MarioConst *constSelector = mActor->mConst;
                    Constants *consts = constSelector->_0[constSelector->_8];
                    fr1e *= _52C;
                    break;
                default:
                    MarioConst *constSelector = mActor->mConst;
                    Constants *consts = constSelector->_0[constSelector->_8];
                    fr1e *= _530;
                    break;
            }
        }
        if(_20) {
            fr1d = 0.523598790169f;
            fr1e = 0.01f;
        }
        if(_19) fr1d = 0.523598790169f;
        if(_32) {
            fr1e = 0.15f;
            MarioConst *constSelector = mActor->mConst;
            Constants *consts = constSelector->_0[constSelector->_8];
            fr1d = consts->_54C;
        }
        bool r1b = true;
        f32 prod = fr1d * fr1e;
        _5C = _5C * (1f - prod) + prod;
        if(_3C && _AE && _32 && getStickY() > 0f) {
            MarioConst *constSelector = mActor->mConst;
            Constants *consts = constSelector->_0[constSelector->_8];
            fr1d = 3.14159274101f / consts->_5B8 * (getStickY() - 0.523598790169f) ;
            if(_5C < fr1d) {
                MarioConst *constSelector = mActor->mConst;
                Constants *consts = constSelector->_0[constSelector->_8];
                _5C = _5C * (1f - consts->_5BC) + fr1d * (1f - consts->_5BC);
            }
            else _5C = _5C * (1f - fr1d * fr1e) + fr1d * fr1e;
            if(!(_7A || _7C) && !_8A) changeAnimation("水泳ターン下", nullptr);
            r1b = false;
        }
        if(r1b) stopAnimation("水泳ターン下", nullptr);
        if(_32) {
            MarioConst *constSelector = mActor->mConst;
            Constants *consts = constSelector->_0[constSelector->_8];
            _5C = MR::clamp(_5C, 0.523598790169f, consts->_54C);
        }
        else _5C = MR::clamp(_5C, cLimitAngleWait, cUpperAngleWait);
        if(MR::isStageSwimAngleLimit()) _5C = MR::clamp(_5C, 0.872664690018f, 2.26892805099f);
    }
    
    // 802fc7d8
    if((_20 || (_19 && isStickOn())) && _8A) {
        TVec3f stack_164(getWorldPadDir());
        f32 tmp;
        if(_19) {
            f32 tmptmp = 0.01f * (12f - _54);
            if(tmptmp > 0.01f) tmp = tmptmp;
        }
        else {
            tmp = 0.01f * (10f - _54);
        }
        if(_8A) tmp *= 3f;
        MarioConst *constSelector = mActor->mConst;
        Constants *consts = constSelector->_0[constSelector->_8];
        f32 res = MR::clamp(tmp * consts->_518, 0f, 1f);
        f32 diffAngle = MR::diffAngleAbs(_60, stack_164);
        if(diffAngle > 0f) {
            if(diffAngle < res) _60 = stack_164;
            else MR::vecBlendSphere(_60, stack_164, &_60, res / diffAngle);
        }
    }
    else {
        MarioConst *constSelector = mActor->mConst;
        Constants *consts = constSelector->_0[constSelector->_8];
        f32 tmp = fr1f - consts->_4e8;
        MR::rotAxisVecRad(_60, -_6C, &_60, tmp * _50 * constSelector->_0[constSelector->_8]->_4e4);
    }
    TVec3f stack_158(_60);
    MR:vecKillElement(_60, _6C, &_60);
    TVec3f _14C, _140;
    if(!MR::normalizeOrZero(&_60)) {
        PSVECCrossProduct(_6C.toCVec(), _60.toCVec(), _140.toVec());
        MR::normalize(&_140);
        getPlayer()->setSideVec(_140);
        MR::rotAxisVecRad(_60, _140, &_14C, _5C);
        TVec3f stack_134(getPlayer()->_1F0);
        TVec3f stack_128(getFrontVec())
        TVec3f stack_11C;
        f32 something = 0.1f;
        if(_r1e) something = 1f;
        MR::vecBlendSphere(stack_128, stack_14C, &stack_11C, something);
        getPlayer()->setFrontVecKeepSide(stack_11C);
    }
    else {
        stack_14C = getFrontVec();
        stack_140 = getPlayer()->_310;
    }
    spin();
    TVec3f stack_110 = getPlayer()->_1FC;
    if(_34 || _2E || isAnimationRun("水泳ジャンプダイブ回転") || isAnimationRun("水泳潜り")) {
        PSVECCrossProduct(_14C.toCVec(), _140.toCVec(), _110.toVec());
    }
    if(!_8A) decideVelocity();
    TVec3f stack_104(stack_110, _54);
    if(_19 && _6C.dot(stack_110) > 0f) {
        f32 mag = PSVECMag(stack_104.toCVec());
        MR::vecKillElement(stack_104, _6C, &stack_104);
        stack_104.setLength(mag);
    }
    f32 element = MR::vecKillElement(stack_104, _6C, &stack_104);
    f32 fr2;
    if(_30 > 0x19) {
        fr2 = (_30 - 50) / 25f;
    }
    else {
        fr2 = _30 / 25f;
    }
    MarioConst *constSelector = mActor->mConst;
    Constants *consts = constSelector->_0[constSelector->_8];
    f32 fr1d = fr2 + (1f - fr2) * consts->_514;
    stack_104 += _6C % element % fr1d;
    addVelocity(stack_104);
    u16 fr1b = _2E;
    if(fr1b) {
        addVelocity(getGravityVec(), 0.25f * fr1b);
        _2E--;
    }
    if(_34) _34--;
    if(!_19 && !_2C && !_8A) procBuoyancy();
    else if(_6C.dot(stack_110) < 0f) {
        if(_19) _19 = 0;
    }
    else if(_19) {
        AreaObj *obj = MR::getAreaObj(getTrans().translate(getGravityVec() % 100f));
        if(obj) {
            TPos3f *followMtx = obj->getFollowMtx();
            if(followMtx) {
                TVec3f stack_F8;
                MR::extractMtxTrans(followMtx.toMtxPtr(), &stack_F8);
                if (!(
                    JGeometry::TUtil::epsilonEquals(stack_F8.x, _154.x, 0.000003814697265625f)
                    && JGeometry::TUtil::epsilonEquals(stack_F8.y, _154.y, 0.000003814697265625f)
                    && JGeometry::TUtil::epsilonEquals(stack_F8.z, _154.y, 0.000003814697265625f)
                )) {
                    // Probably some inline function for the magnitude
                    if(PSVECMag((stack_F8 - _154).toCVec()) < 10f) addVelocity(stack_F8 - _154);
                    _154 = stack_F8;
                }
            }
        }
    }
    if(_AE) {
        _AE--;
        addVelocity(_A0);
        if(_AE < 0x78) {
            _A0.setInline(_A0.scaleInline(0.98f));
        }
    }
    if(_8A) {
        MarioConst *constSelector = mActor->mConst;
        Constants *consts = constSelector->_0[constSelector->_8];
        addVelocity(_184, consts->_534);
    }
    else addVelocity(_184);
    TVec3f stack_EC(_184);
    f32 fr1d = PSVECMag(stack_EC);
    if(!MR::normalizeOrZero(&stack_EC)) {
        if(MR::diffAngleAbs(stack_EC, _60) < 1.57079637051f) {
            MR::vecBlendSphere(_60, stack_EC, &_60, 0.001f * fr1d);
        }
        else {
            MR::vecBlendSphere(_60, -stack_EC, &_60, fr1d);
        }
    }
    TVec3f stack_E0;
    if(MR::calcWhirlPoolAccelInfo(getTrans(), &stack_E0) && !MR::isNearZero(stack_E0, 0.001f)) {
        _184 += stack_E0;
        TVec3f stack_D4(stack_E0);
        MR::normalizeOrZero(&stack_D4);
        MR::vecBlendSphere(_60, stack_D4, &_60, 0.1f);
    }
    if(!_8A && !_19 && !MR::isNearZero(_190, 0.001f)) {
        f32 angleDiff = MR::diffAngleSignedHorizontal(_190, _60, getPlayer()->getAirGravityVec());
        if(angleDiff > 1.04719758034f) {
            PSMTXRotAxisRad(stack_1D0.toMtxPtr(), _6C.toCVec(), angleDiff - 1.04719758034f);
            PSMTXMultVecSR(stack_1D0.toMtxPtr(), _60.toCVec(), _60.toVec());
        }
        else if(angleDiff < -(1.04719758034f)) {
            PSMTXRotAxisRad(stack_1A0.toMtxPtr(), _6C.toCVec(), -(-angleDiff - 1.04719758034f));
            PSMTXMultVecSR(stack_1A0.toMtxPtr(), _6C.toCVec(), _6C.toVec());
        }
    }
    decideAnimation();
    decideEffect(false);
    if(_2C) _2C--;
    if(_30) _30--;
    if(_44) _44--;
    updateTilt();
    jet();
    pushedByWaterWall();
    return true;
}
