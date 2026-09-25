#include "Game/Player/MarioFoo.hpp"
#include "Game/LiveActor/HitSensor.hpp"
#include "Game/MapObj/DashRing.hpp"
#include "Game/Player/Mario.hpp"
#include "Game/Player/MarioActor.hpp"
#include "Game/Player/MarioAnimator.hpp"
#include "Game/Player/MarioConst.hpp"
#include "Game/Util/AreaObjUtil.hpp"
#include "Game/Util/CameraUtil.hpp"
#include "Game/Util/DirectDraw.hpp"
#include "Game/Util/DirectDrawUtil.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/MathUtil.hpp"
#include "Game/Util/ModelUtil.hpp"
#include "Game/Util/MtxUtil.hpp"
#include <JSystem/JUtility/JUTTexture.hpp>

void MarioFoo_FORCE_MATCH_SDATA2() {
    (void)1.0f;
    (void)0.0f;
    (void)0.5f;
    (void)3.14159274f;
    (void)1.57079637f;
    (void)2.0f;
    (void)0.52359879f;
    (void)0.100000001f;
    (void)120.0f;
    (void)0.125f;
    (void)10.0f;
    (void)100.0f;
    (void)0.0500000007f;
    (void)5.0f;
    (void)1.04719758f;
    (void)1.10000002f;
    (void)-0.100000001f;
    (void)0.00999999978f;
    (void)25.0f;
    (void)0.980000019f;
    (void)1.01999998f;
    (void)0.899999976f;
    (void)1.20000005f;
    (void)50.0f;
    (void)1.5f;
    (void)12.0f;
    (void)2.09439516f;
    (void)0.314159274f;
    (void)0.785398185f;
    (void)0.25f;
    (void)30.0f;
    (void)0.015625f;
    (void)70.0f;
    (void)2000.0f;
    (void)255.0f;
    (void)1.00010002f;
    (void)6.0f;
}

void MarioFoo_FORCE_MATCH(TVec3f& rVec, f32 a, f32 b, f32 c) {
    rVec.setLength(a);
    rVec.setLength(b);
    rVec.setLength(c);
}

namespace {
    f32 cTurnMotionSpeed = 5.0f;
    f32 cWidth = 70.0f;
    f32 cLimitAngleSink = JGeometry::TUtil< f32 >::PI() / 1.0001f;
    f32 cNeutralAngleWait = JGeometry::TUtil< f32 >::PI() / 2.0f - JGeometry::TUtil< f32 >::PI() / 6.0f;
    f32 cLimitAngleWait = JGeometry::TUtil< f32 >::PI() / 1.0001f;
    f32 cUpperAngleWait = JGeometry::TUtil< f32 >::PI() / 100.0f;
}  // namespace

void Mario::tryStartFoo() {
    if (MR::getAreaObj("CelestrialSphere", getTrans()) || !mFoo->_48) {
        changeStatus(mFoo);
        mFoo->setStartSpeed(mActor->getConst().getTable()->mSwimFrontMaxSpeed);
    }
}

MarioFoo::MarioFoo(MarioActor* pActor) : MarioState(pActor, MarioStatus_Foo) {
    _11 = 0;
    _12 = 0;
    _14 = 0;
    _18 = 0;
    _1A = 0;
    _1C = 0;
    _1E = 0;
    _20 = 0.0f;
    _24 = 0.0f;
    _28 = 0.0f;
    _2C = 0.0f;
    _30.zero();
    _3C.zero();
    _48 = nullptr;
    _4C = 0;
    _4E = 0;
    _50 = 0;
    _54 = 0.0f;
    _58 = 0;
    _59 = 0;
    _5A = 0;
    _5C = 0.0f;
    _60 = 0;
    _64.zero();
    _70 = 0.0f;
    _74 = 0.0f;
    PSMTXIdentity(_78);
    _A8 = 0;
    _AA = 0;
    _6B0 = 0;
    _6B4 = 0;
    _6B8 = nullptr;
    _AC = 0;
    _AE = 0;

    for (u32 i = 0; i < 64; i++) {
        _B0[i].zero();
        _3B0[i].zero();
    }
}

void MarioFoo::init() {
    _6B8 = new JUTTexture(MR::getTexture(MR::getModelResourceHolder(mActor), "FooLine.bti"), 0);
}

bool MarioFoo::start() {
    _20 = 0.0f;
    _24 = 0.0f;
    _28 = 0.0f;
    _4C = 30;
    _A8 = 0;
    _11 = 0;
    _6B0 = 0;
    _6B4 = 0;
    _4E = 0;
    _50 = 0;
    _54 = 0.0f;
    _1C = 0;
    _1E = 0;
    _60 = 0;
    _64.zero();
    _70 = 0.0f;
    _74 = 0.0f;
    _14 = 0;
    _2C = 0.5235988f;
    _30 = getFrontVec();
    MR::normalize(&_30);

    stopAnimationUpper(nullptr);
    getPlayer()->mMovementStates._1 = false;
    getPlayer()->mMovementStates._A = false;
    _18 = 0;
    if (checkLvlA()) {
        _12 = 1;
    }

    changeAnimation("フーファイター飛行開始", "フーファイター飛行");
    startPadVib(2);
    _48 = MR::getAreaObj("CelestrialSphere", getTrans());
    _AC = 0;
    _AE = 0;
    return true;
}

bool MarioFoo::update() {
    _14++;
    if (checkTrgZ()) {
        playSound("声尻ドロップ");
        playSound("フーブレーキ");
        _11 = 1;
        _59 = 3;
    }

    if (!checkLvlA()) {
        _12 = 0;
    }

    if (_11) {
        return false;
    }

    _3C = -getGravityVec();
    jet();
    if (getPlayer()->_1C._9) {
        if (_6B0) {
            _6B0--;
        }
    } else {
        _B0[_6B4] = getTrans();
        TVec3f leftHand;
        TVec3f rightHand;
        TVec3f handDirection;
        mActor->getRealPos("HandL", &leftHand);
        mActor->getRealPos("HandR", &rightHand);
        handDirection = rightHand - leftHand;
        MR::normalizeOrZero(&handDirection);
        _3B0[_6B4] = handDirection;
        if (_6B0 < 64) {
            _6B0++;
        }

        _6B4 = (_6B4 + 1) % 64;
    }

    f32 speedRatio = 1.0f - MR::clamp(_28 / mActor->getConst().getTable()->mSwimFrontMaxSpeed, 0.0f, 1.0f);
    _20 = _20 * mActor->getConst().getTable()->mSwimRotXIne + getStickY() * (1.0f - mActor->mConst->getTable()->mSwimRotXIne);
    _24 = _24 * mActor->getConst().getTable()->mSwimRotZIne + getStickX() * (1.0f - mActor->mConst->getTable()->mSwimRotZIne);
    if (!_1C && _18) {
        _2C += speedRatio * (_20 * mActor->getConst().getTable()->mSwimRotSpeedX);
    }

    if (!checkLvlA() && !checkLvlZ() && !_18 && _28 < ::cTurnMotionSpeed) {
        f32 turnAngle = _2C;
        if (_2C > 1.5707964f) {
            turnAngle = 1.5707964f;
        } else if (getStickY() > 0.0f) {
            f32 angle = 0.5235988f + getStickY() * (JGeometry::TUtil< f32 >::PI() / 6.0f);
            if (angle > _2C) {
                turnAngle = angle;
            }
        }

        if (!_60) {
            _1C++;
        }

        if (_1C >= 120) {
            _1C = 120;
        }
    } else {
        _1C = 0;
    }

    f32 blend;
    f32 targetAngle;
    if (MR::isNearZero(getStickY(), 0.1f)) {
        f32 ratio = _1C / 120.0f;
        if (ratio > 1.0f) {
            ratio = 1.0f;
        }

        targetAngle = _2C + ratio * (::cNeutralAngleWait - _2C);
    } else {
        if (getStickY() > 0.0f) {
            f32 slowRatio = 0.0f;
            f32 ratio = _1C / 120.0f;
            if (_28 < 2.0f) {
                slowRatio = 1.0f;
            } else if (_28 > 10.0f) {
                slowRatio = 0.0f;
            } else {
                slowRatio = 1.0f - (_28 - 2.0f) / 8.0f;
            }

            if (ratio < slowRatio) {
                ratio = slowRatio;
            }

            ratio = MR::clamp(ratio, 0.0f, 1.0f);
            f32 limit = ratio * ::cLimitAngleWait + (1.0f - ratio) * ::cLimitAngleSink;
            if (getPlayer()->mVerticalSpeed < 100.0f) {
                limit = ::cNeutralAngleWait;
            }

            targetAngle = ::cNeutralAngleWait + (limit - ::cNeutralAngleWait) * getStickY();
        } else if (getStickY() < 0.0f) {
            targetAngle = ::cNeutralAngleWait + (::cUpperAngleWait - ::cNeutralAngleWait) * -getStickY();
        }
    }

    blend = 0.05f;
    if (_28 > 5.0f) {
        blend = 0.05f - 0.05f * (5.0f / _28);
        if (blend < 0.0f) {
            blend = 0.0f;
        }
    }

    blend *= mActor->getConst().getTable()->mSwimXJetRotRatio;
    if (getStickP() == 0.0f) {
        targetAngle = ::cLimitAngleSink;
        blend *= 0.5f;
    }

    if (getPlayer()->_1C._9) {
        targetAngle = 1.0471976f;
        if (getStickY() > 0.1f) {
            f32 ratio = 1.1f * (getStickY() - 0.1f);
            targetAngle = ::cLimitAngleSink * ratio + targetAngle * (1.0f - ratio);
        } else if (getStickY() < -0.1f) {
            f32 ratio = 1.1f * (-getStickY() - 0.1f);
            targetAngle = ::cUpperAngleWait * ratio + targetAngle * (1.0f - ratio);
        }

        blend = 0.01f;
    }

    _2C = _2C * (1.0f - blend) + targetAngle * blend;
    bool stopTurn = true;
    if (_1C && !_60 && getStickY() > 0.0f) {
        f32 angle = 3.1415927f / mActor->getConst().getTable()->mSwimTiltReal;
        f32 stick = getStickY();
        targetAngle = 0.5235988f + stick * (angle - 0.5235988f);
        if (_2C < targetAngle) {
            f32 ratio = mActor->getConst().getTable()->mSwimTiltSpd;
            _2C = _2C * ratio + targetAngle * (1.0f - ratio);
        } else {
            _2C = _2C * (1.0f - blend) + targetAngle * blend;
        }

        stopTurn = false;
    }

    if (stopTurn) {
        stopAnimation("水泳ターン下");
    }

    _2C = MR::clamp(_2C, ::cUpperAngleWait, ::cLimitAngleSink);

    f32 turnRatio = speedRatio + mActor->getConst().getTable()->mSwimRotSpeedZStop;
    MR::rotAxisVecRad(_30, -_3C, &_30, turnRatio * (_24 * mActor->mConst->getTable()->mSwimRotSpeedZ));
    MR::vecKillElement(_30, _3C, &_30);
    MR::normalize(&_30);
    TVec3f side = _3C.cross(_30);
    MR::normalize(&side);
    getPlayer()->setSideVec(side);
    TVec3f front;
    MR::rotAxisVecRad(_30, side, &front, _2C);
    TVec3f head(getPlayer()->mHeadVec);
    TVec3f oldFront(getFrontVec());
    TVec3f blendedFront;
    MR::vecBlendSphere(oldFront, front, &blendedFront, 0.1f);
    getPlayer()->setFrontVecKeepSide(blendedFront);
    spin();

    TVec3f direction;
    direction = getPlayer()->_1FC;
    TVec3f velocity = direction * _28;
    f32 vertical = MR::vecKillElement(velocity, _3C, &velocity);
    f32 ratio;
    if (_1A > 25) {
        ratio = (50 - _1A) / 25.0f;
    } else {
        ratio = _1A / 25.0f;
    }

    f32 verticalRatio = ratio + (1.0f - ratio) * mActor->getConst().getTable()->mSwimSpdYratio;
    velocity += _3C * vertical * verticalRatio;
    addVelocity(velocity);
    if (_60) {
        _60--;
        addVelocity(_64);
        if (_60 < 120) {
            _64.x *= 0.98f;
            _64.y *= 0.98f;
            _64.z *= 0.98f;
        }
    }

    if (_18) {
        _18--;
    }

    if (_1A) {
        _1A--;
    }

    updateTilt();

    if (_48 != nullptr) {
        TVec3f center;
        MR::calcSpherePos(&center, _48);
        f32 radius = MR::getSphereRadius(_48);
        TVec3f distance = getTrans() - center;
        if (!_1E && distance.length() > radius) {
            _1E = 30;
            hitWall(-distance, nullptr);
        } else if (_1E) {
            _1E--;
        }
    }

    return true;
}

bool MarioFoo::notice() {
    if (getNoticedStatus() == MarioStatus_FpView) {
        return true;
    }

    return getNoticedStatus() == MarioStatus_Swim;
}

bool MarioFoo::close() {
    stopEffect("フーマリオブレーキ左");
    stopEffect("フーマリオブレーキ右");
    stopEffect("フーマリオグロー左");
    stopEffect("フーマリオグロー右");
    playEffect("フーマリオ解除左");
    playEffect("フーマリオ解除右");
    stopAnimationUpper(nullptr);
    setYangleOffset(0.0f);
    u16 upperJoint = getAnimator()->getUpperJointID();
    setJointGlobalMtx(upperJoint, nullptr);

    switch (_59) {
    case 0:
        changeAnimation("飛び込み失敗回転着地");
        break;
    case 1:
        changeAnimation("フーファイター着地");
        break;
    case 2:
        stopAnimation(nullptr);
        break;
    case 3:
        changeAnimation("フーファイター解除");
        getPlayer()->mJumpVec = TVec3f(0.0f, 0.0f, 0.0f);
        getPlayer()->_10._21 = true;
        break;
    }

    if (getPlayer()->mMovementStates.jumping) {
        changeAnimation(nullptr, "落下");
    } else {
        changeAnimation(nullptr, "基本");
    }

    Mario* player = getPlayer();
    player->_4B0 = player->mPosition;
    getPlayer()->forceSetHeadVecKeepSide(-getGravityVec());
    changeAnimationInterpoleFrame(6);
    mActor->setBlendMtxTimer(4);
    getPlayer()->unlockGroundCheck(this);
    mActor->_F44 = true;
    mActor->resetWaterLife();
    _54 = 0.0f;
    getPlayer()->_10._7 = true;
    return true;
}

const TVec3f& MarioFoo::getGravityVec() const {
    return MarioModule::getGravityVec();
}

void MarioFoo::jet() {
    f32 acceleration = calcRingAcc();
    f32 speed;
    if (acceleration != 1.0f) {
        speed = acceleration * mActor->getConst().getTable()->mSwimFrontMaxSpeed;
    } else {
        speed = mActor->getConst().getTable()->mSwimFrontJetSpeed;
    }

    f32 speedUp = 1.02f;
    f32 slowDown = 0.98f;
    if (checkLvlA() && !_12) {
        speed = 0.0f;
        slowDown = 0.9f;
        _AC++;
        if (!isAnimationRun("フーファイタースピン")) {
            changeAnimation("フーファイター静止");
        }

        playEffect("フーマリオブレーキ左");
        playEffect("フーマリオブレーキ右");
        stopEffect("フーマリオグロー左");
        stopEffect("フーマリオグロー右");
        if (!_AE) {
            playSound("フーブレーキ");
        }

        _AE = 1;
        getPlayer()->_1C._9 = true;
    } else if (_14 < 8) {
        _AC++;
        _AE = 1;
        _28 = 0.0f;
    } else if (_AC) {
        if (_AC < 8) {
            _AC = 0;
            _AE = 0;
        } else {
            if (_AE) {
                playEffect("共通ひこうきブースト");
                playSound("フー加速");
                _AE = 0;
                if (isAnimationRun("フーファイター静止")) {
                    changeAnimation("フーファイター飛行再開");
                }
            }

            if (_AC > 20) {
                _AC = 20;
            }

            _AC--;
            speed *= 2.0f;
            speedUp = 1.2f;
        }

        stopAnimation("フーファイター静止");
        playEffect("フーマリオグロー左");
        playEffect("フーマリオグロー右");
        stopEffect("フーマリオブレーキ左");
        stopEffect("フーマリオブレーキ右");
    }

    f32 soundSpeed = 50.0f * (1.0f + getStickY());
    if (!_AE) {
        playSound("フー飛行中", soundSpeed);
    }

    playSound("フー滞空中", soundSpeed);
    if (_28 < speed) {
        if (_28 < 1.0f) {
            _28 = 1.0f;
        }

        _28 *= speedUp;
        if (_4E) {
            _28 *= 1.5f;
        }
    } else if (_28 > speed) {
        _28 *= slowDown;
    } else {
        _28 = speed;
    }
}

void MarioFoo::updateTilt() {
    f32 sideTilt;
    f32 blend = 0.1f;
    sideTilt = getStickX() * JGeometry::TUtil< f32 >::PI() / 5.0f;
    f32 frontTilt = 0.0f;
    if (!_1C) {
        frontTilt = getStickY() * JGeometry::TUtil< f32 >::PI() / 12.0f;
        if (getStickY() > 0.0f && _2C <= 2.0943952f) {
            frontTilt = getStickY() * JGeometry::TUtil< f32 >::PI() / 8.0f;
        }
    } else if (getStickY() < 0.0f) {
        frontTilt = getStickY() * JGeometry::TUtil< f32 >::PI() / mActor->getConst().getTable()->mSwimTiltZup;
    } else if (getStickY() > 0.0f) {
        frontTilt = getStickY() * JGeometry::TUtil< f32 >::PI() / mActor->getConst().getTable()->mSwimTiltZdown;
    } else if (_1C) {
        f32 ratio = _1C / 120.0f;
        if (ratio > 1.0f) {
            ratio = 1.0f;
        }

        frontTilt -= (JGeometry::TUtil< f32 >::PI() / 10.0f) * ratio;
    } else {
        frontTilt = 0.0f;
    }

    _70 = blend * sideTilt + (1.0f - blend) * _70;
    _74 = 0.1f * frontTilt + 0.9f * _74;
    TMtx34f sideMtx;
    TMtx34f frontMtx;
    PSMTXRotRad(sideMtx, 'X', _70);
    PSMTXRotRad(frontMtx, 'Z', _74);
    PSMTXConcat(sideMtx, frontMtx, _78);
    u16 upperJoint = getAnimator()->getUpperJointID();
    setJointGlobalMtx(upperJoint, _78);
}

void MarioFoo::hitWall(const TVec3f& rNormal, HitSensor* pSensor) {
    if (sendPunch(pSensor, false) != true) {
        _11 = 1;
        if (MR::diffAngleAbs(rNormal, -getGravityVec()) <= 0.7853982f) {
            if (MR::diffAngleAbs(rNormal, -getPlayer()->_1FC) <= 0.7853982f) {
                _59 = 0;
            } else {
                _59 = 1;
            }
        } else {
            _59 = 2;
        }

        playSound("フーブレーキ");
    }
}

f32 MarioFoo::getStickY() const {
    return MarioModule::getStickY();
}

const char* MarioFoo_FORCE_MATCH_DATA(u32 index) {
    static const char* const strings[] = {"水泳ダメージ", "水中ダメージ", "声小ダメージ", "ダメージ", "水泳ダメージ中"};
    return strings[index];
}

void MarioFoo::spin() {
    if (!_60) {
        if (mActor->isRequestSpin() && !_4C) {
            _4C = 20;
            if (!isAnimationRun("フーファイタースピン") && !checkLvlA()) {
                playSound("声スピン");
                playSound("スピンジャンプ");
            }

            changeAnimation("フーファイタースピン");
            MarioActor* actor = mActor;
            if (!actor->_944) {
                actor->_945 = 0;
                actor->_974 = 0;
            }

            actor->_944 = 30;
            mActor->_946 = 60;
        }

        if (_4C) {
            _4C--;
        }
    }
}

bool MarioFoo::passRing(const HitSensor* pSensor) {
    const TVec3f& center = pSensor->mPosition;
    if (!_4E) {
        _50 = mActor->getConst().getTable()->mSwimRingDashChargeTime;
        changeAnimation("リングダッシュ準備");
    }

    if (_50) {
        getPlayer()->push((center - getTrans()) * 0.25f);
    }

    const DashRing* ring = static_cast< const DashRing* >(pSensor->mHost);
    _5A = ring->mMaxDuration;
    _5C = ring->mSpeedScale;
    _4E = ring->mBoostTime;
    return true;
}

f32 MarioFoo::calcRingAcc() {
    if (_4E) {
        if (_50) {
            _50--;
            if (_50 == 1) {
                MarioActor* actor = mActor;
                actor->_1AA = actor->getConst().getTable()->mStarPieceFogTime;
                actor->_1AC = 1.0f;
                actor->_1B0.set(255, 255, 255, 0);
                actor->_1B5 = false;
            }

            if (!_50) {
                startPadVib(3);
                changeAnimation("リングダッシュ");
            }

            return 1.0f;
        }

        _54 += 0.5f;
        if (_54 > 30.0f) {
            _54 = 30.0f;
        }

        u16 duration = _5A;
        u16 remaining = _4E;
        f32 acceleration = _5C;
        if (remaining < duration) {
            f32 ratio = static_cast< f32 >(remaining) / static_cast< f32 >(duration);
            acceleration = (1.0f - ratio) + acceleration * ratio;
        }

        _4E--;
        return acceleration;
    }

    if (_54 > 0.0f) {
        _54 -= 0.5f;
    } else {
        _54 = 0.0f;
    }

    return 1.0f;
}

void MarioFoo::draw3D() const {
    TVec3f beamVertices[4];
    TVec3f leftEdge[2];
    TVec3f rightEdge[2];
    TVec3f nextLeftEdge[2];
    TVec3f nextRightEdge[2];

    if (getPlayer()->_1C._9) {
        TDDraw::setup(1, 1, 0);
        MR::ddSetVtxFormat(2);
        MR::ddLightingOff();
        GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_C0, GX_CC_C1, GX_CC_TEXA, GX_CC_ZERO);
        GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
        GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_TEXA, GX_CA_A0, GX_CA_ZERO);
        GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_2, GX_TRUE, GX_TEVPREV);
        GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_ONE, GX_LO_NOOP);
        _6B8->load(GX_TEXMAP0);
        const Color8 color0(255, 32, 32, 128);
        GXSetTevColor(GX_TEVREG0, color0);
        const Color8 color1(255, 64, 64, 255);
        GXSetTevColor(GX_TEVREG1, color1);

        TVec3f center = mActor->_2A0 + getPlayer()->getUpVec() * 50.0f;
        TVec3f back = -getPlayer()->getUpVec();
        TVec3f side(getPlayer()->mSideVec);
        TVec3f up;
        up.cross(back, side);
        MR::normalizeOrZero(&up);
        TVec3f front = -back;
        f32 length = (64 - _6B0) / 64.0f;

        beamVertices[0] = center - side * 70.0f;
        beamVertices[1] = center - side * 70.0f + front * 2000.0f * length;
        beamVertices[2] = center - side * 50.0f + front * 2000.0f * length;
        beamVertices[3] = center - side * 50.0f;
        GXBegin(GX_QUADS, GX_VTXFMT0, 4);
        MR::ddSendVtxData(beamVertices[0], TVec2f(0.0f, 0.0f));
        MR::ddSendVtxData(beamVertices[1], TVec2f(0.5f, 0.0f));
        MR::ddSendVtxData(beamVertices[2], TVec2f(0.5f, 1.0f));
        MR::ddSendVtxData(beamVertices[3], TVec2f(0.0f, 1.0f));
        GXEnd();

        beamVertices[0] = center + side * 70.0f;
        beamVertices[1] = center + side * 70.0f + front * 2000.0f * length;
        beamVertices[2] = center + side * 50.0f + front * 2000.0f * length;
        beamVertices[3] = center + side * 50.0f;
        GXBegin(GX_QUADS, GX_VTXFMT0, 4);
        MR::ddSendVtxData(beamVertices[0], TVec2f(0.0f, 0.0f));
        MR::ddSendVtxData(beamVertices[1], TVec2f(0.5f, 0.0f));
        MR::ddSendVtxData(beamVertices[2], TVec2f(0.5f, 1.0f));
        MR::ddSendVtxData(beamVertices[3], TVec2f(0.0f, 1.0f));
        GXEnd();
    }

    const Color8 color2(255, 64, 64, 255);

    GXSetTevColor(GX_TEVREG1, color2);
    if (_6B0) {
        TDDraw::setup(1, 1, 0);
        MR::ddSetVtxFormat(2);
        MR::ddLightingOff();
        GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_C0, GX_CC_C1, GX_CC_TEXA, GX_CC_ZERO);
        GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
        GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_TEXA, GX_CA_A0, GX_CA_ZERO);
        GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_2, GX_TRUE, GX_TEVPREV);
        GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_ONE, GX_LO_NOOP);
        _6B8->load(GX_TEXMAP0);

        u32 midpoint;
        u32 count = _6B0;
        midpoint = count / 2;
        TVec3f previous;
        TVec2f nextTexCoords[2];
        TVec2f texCoords[2];
        for (u32 i = 0; i < count; i++) {
            u8 alpha;
            if (i > midpoint) {
                alpha = 255.0f * (static_cast< f32 >(count - i) / static_cast< f32 >(count));
            } else {
                alpha = 255.0f * (static_cast< f32 >(i) / static_cast< f32 >(count));
            }

            const Color8 color3(255, 0, 0, alpha);
            GXSetTevColor(GX_TEVREG0, color3);
            u32 index = (_6B4 + 127 - i) % 64;
            if (i == 0) {
                u32 j;
                for (j = 1; j < count; j++) {
                    TVec3f direction = _B0[(_6B4 + 127 - j) % 64] - _B0[index];
                    TVec3f horizontal;
                    MR::vecKillElement(direction, MR::getCamZdir(), &horizontal);
                    if (!MR::normalizeOrZero(&horizontal)) {
                        TVec3f width;
                        width.cross(horizontal, MR::getCamZdir());
                        width.scale(10.0f);
                        leftEdge[0] = _B0[index] + width - _3B0[index] * ::cWidth;
                        leftEdge[1] = _B0[index] - width - _3B0[index] * ::cWidth;
                        rightEdge[0] = _B0[index] + width + _3B0[index] * ::cWidth;
                        rightEdge[1] = _B0[index] - width + _3B0[index] * ::cWidth;
                        break;
                    }
                }

                if (j >= count) {
                    break;
                }

                previous = _B0[index];
                texCoords[0].set(0.0f, 0.0f);
                texCoords[1].set(1.0f, 0.0f);
            } else {
                f32 textureY = static_cast< f32 >(i + 1) / static_cast< f32 >(count);
                nextTexCoords[0].set(0.0f, textureY);
                nextTexCoords[1].set(1.0f, textureY);
                const TVec3f& rPosition = _B0[index];
                TVec3f direction = rPosition - previous;
                TVec3f width(_3B0[index]);
                width.setLength(10.0f);
                nextLeftEdge[0] = rPosition + width - _3B0[index] * ::cWidth;
                nextLeftEdge[1] = rPosition - width - _3B0[index] * ::cWidth;
                GXBegin(GX_QUADS, GX_VTXFMT0, 4);
                MR::ddSendVtxData(leftEdge[0], texCoords[0]);
                MR::ddSendVtxData(nextLeftEdge[0], nextTexCoords[0]);
                MR::ddSendVtxData(nextLeftEdge[1], nextTexCoords[1]);
                MR::ddSendVtxData(leftEdge[1], texCoords[1]);
                GXEnd();

                nextRightEdge[0] = rPosition + width + _3B0[index] * ::cWidth;
                nextRightEdge[1] = rPosition - width + _3B0[index] * ::cWidth;
                GXBegin(GX_QUADS, GX_VTXFMT0, 4);
                MR::ddSendVtxData(rightEdge[0], texCoords[0]);
                MR::ddSendVtxData(nextRightEdge[0], nextTexCoords[0]);
                MR::ddSendVtxData(nextRightEdge[1], nextTexCoords[1]);
                MR::ddSendVtxData(rightEdge[1], texCoords[1]);
                GXEnd();

                leftEdge[0] = nextLeftEdge[0];
                texCoords[0] = nextTexCoords[0];
                leftEdge[1] = nextLeftEdge[1];
                texCoords[1] = nextTexCoords[1];
                rightEdge[0] = nextRightEdge[0];
                rightEdge[1] = nextRightEdge[1];
                previous = rPosition;
            }
        }
    }
}

f32 MarioFoo::getBlurOffset() const {
    return _54;
}
