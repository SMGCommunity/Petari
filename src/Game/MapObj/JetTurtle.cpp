#include "Game/MapObj/JetTurtle.hpp"
#include "Game/LiveActor/Binder.hpp"
#include "Game/LiveActor/HitSensor.hpp"
#include "Game/LiveActor/MessageSensorHolder.hpp"
#include "Game/Util/ActorSensorUtil.hpp"
#include "Game/Util/ActorShadowUtil.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/PlayerUtil.hpp"
#include "revolution/mtx.h"
#include <JSystem/JMath/JMATrigonometric.hpp>

namespace {
    static f32 sThrowSpdStraight[3] = {30.0f, 20.0f, 30.0f};
    static f32 sThrowSpdHoming[3] = {24.0f, 20.0f, 24.0f};
    static f32 sGravityLevel[3] = {0.017000001f, 0.0099999998f, 0.0055f};
    static u16 sResetStep[3] = {0x12C, 0x1E0, 0x12C};
};  // namespace

namespace NrvJetTurtle {
    NEW_NERVE_ONEND(JetTurtleNrvWait, JetTurtle, Wait, Wait);
    NEW_NERVE_ONEND(JetTurtleNrvWait2, JetTurtle, Wait, Wait);
    NEW_NERVE(JetTurtleNrvThrowWait, JetTurtle, ThrowWait);
    NEW_NERVE(JetTurtleNrvThrowing, JetTurtle, Throwing);
    NEW_NERVE(JetTurtleNrvTakenReserve, JetTurtle, Wait);
    NEW_NERVE(JetTurtleNrvTakenReserveD, JetTurtle, TakenReserve);
    NEW_NERVE(JetTurtleNrvTakenStart, JetTurtle, TakenStart);
    NEW_NERVE(JetTurtleNrvRestart, JetTurtle, Restart);
    NEW_NERVE(JetTurtleNrvDrop, JetTurtle, Drop);
};  // namespace NrvJetTurtle

JetTurtle::JetTurtle(const char* pName) : LiveActor(pName) {
    _E3 = 0;
}

void JetTurtle::init(const JMapInfoIter& rIter) {
    init2(rIter, 0);
}

// init2

void JetTurtle::initAfterPlacement() {
    _A8 = mPosition;
    _B4 = mRotation;
}

void JetTurtle::becomeSlowType() {
    getSensor("body")->setType(0x10);
    MR::startBrk(this, "Koura");
    MR::setBrkFrameAndStop(this, 0.0f);
    mShellType = 0;
}

void JetTurtle::becomeFastType() {
    getSensor("body")->setType(0xF);
    MR::startBrk(this, "Koura");
    MR::setBrkFrameAndStop(this, 0.0f);
    mShellType = 1;
}

// ...

void JetTurtle::appearAndTryTaken() {
    appear();
    MR::validateHitSensors(this);
    setNerve(&NrvJetTurtle::JetTurtleNrvTakenReserve::sInstance);
    MR::tryPlayerPullActor(getSensor("body"));
}

void JetTurtle::control() {
    if (_E3) {
        reset(0);
    }
}

void JetTurtle::resetPosition() {
    MR::invalidateClipping(this);
    MR::invalidateHitSensors(this);
    MR::forceDeleteEffectAll(this);
    MR::stopSound(this, "SE_OJ_TURTLE_JET_LAUNCH_W", 0);
    MR::stopSound(this, "SE_OJ_TURTLE_JET_LANCH_L", 0);
    mVelocity.zero();
    mRotation.zero();
    _94 = nullptr;
    setNerve(&NrvJetTurtle::JetTurtleNrvRestart::sInstance);
    MR::hideModel(this);
    _98 = nullptr;
}

void JetTurtle::resetPositionAndVanish() {
    resetPosition();
    MR::deleteEffect(this, "BrakeLamp");

    switch (mShellType) {
    default:
        // why?
        MR::emitEffect(this, "Vanish");
        break;
    case 1:
        MR::emitEffect(this, "VanishRed");
        break;
    case 2:
        MR::emitEffect(this, "VanishGold");
        break;
    }

    if (!_E0) {
        kill();
    }
}

void JetTurtle::bound() {
    TVec3f v24;
    MR::calcGravityVector(this, &v24, nullptr, 0);
    TVec3f v23;
    MR::vecKillElement(mVelocity, v24, &v23);
    if (MR::isBindedGround(this)) {
        TVec3f v22;
        f32 v3 = MR::vecKillElement(_9C, *MR::getGroundNormal(this), &v22);

        if (v3 < 0.0f) {
            TVec3f v17(*MR::getGroundNormal(this));
            v17.scale(v3);
            TVec3f v18(v17);
            v18.scale(0.80f);
            _9C = v22 - v18;
        }
    }

    if (MR::isBindedWall(this)) {
        TVec3f v21;
        f32 v6 = MR::vecKillElement(_9C, *MR::getWallNormal(this), &v21);

        if (v6 < 0.0f) {
            TVec3f v14(*MR::getWallNormal(this));
            v14.scale(v6);
            TVec3f v15(v14);
            v15.scale(0.89f);
            _9C = v21 - v15;
        }
    }

    if (MR::isBindedRoof(this)) {
        TVec3f v21;
        f32 v6 = MR::vecKillElement(_9C, *MR::getRoofNormal(this), &v21);

        if (v6 < 0.0f) {
            TVec3f v14(*MR::getRoofNormal(this));
            v14.scale(v6);
            TVec3f v15(v14);
            v15.scale(0.89f);
            _9C = v21 - v15;
        }
    }
}

void JetTurtle::exeThrowing() {
    if (!_E3) {
        if (MR::isFirstStep(this)) {
            MR::onBind(this);
            MR::onCalcShadow(this, nullptr);
            MR::validateHitSensors(this);
            MR::emitEffect(this, "Ignition");

            if (MR::isInWater(this, TVec3f(0.0f, 0.0f, 0.0f))) {
                MR::startSound(this, "SE_OJ_TURTLE_JET_LAUNCH_W", -1, -1);
                MR::startBck(this, "BulletWater", nullptr);
                MR::emitEffect(this, "WaterBlur");
            } else {
                MR::startSound(this, "SE_OJ_TURTLE_JET_LAUNCH_L", -1, -1);
                MR::startSoundPlayer("SE_PV_THROW", -1);
                MR::startBck(this, "Bullet", nullptr);

                switch (mShellType) {
                case 0:
                    MR::emitEffect(this, "Blur");
                    break;
                case 1:
                    MR::emitEffect(this, "BlurRed");
                    break;
                case 2:
                    MR::emitEffect(this, "BlurGold");
                    break;
                }
            }
        }

        if (MR::isStep(this, sResetStep[_92])) {
            TVec3f v16 = _C0 - mPosition;
            if (PSVECMag(&v16) > 5000.0f) {
                reset(1);
            } else {
                reset(0);
            }

            return;
        }

        if (_98 != nullptr) {
            if (_98->mValidByHost) {
                TVec3f v21 = _98->mPosition - mPosition;
                MR::normalizeOrZero(&v21);
                MR::vecBlendSphere(_9C, v21, &_9C, 0.2f);
                if (MR::isBckOneTimeAndStopped(this)) {
                    if (MR::isInWater(this, TVec3f(0.0f, 0.0f, 0.0f))) {
                        MR::startBck(this, "BulletWater", nullptr);
                    } else {
                        MR::startBck(this, "Bullet", nullptr);
                    }

                    TVec3f v22 = _9C;
                    v22.setLength(_8C + sThrowSpdHoming[_92]);
                    mVelocity = v22;
                }
            } else {
                _98 = nullptr;
            }
        }

        if (_98 == nullptr) {
            TVec3f v22 = _9C;
            v22.setLength(_8C + sThrowSpdStraight[_92]);
            mVelocity = v22;
        }

        TVec3f v14 = _C0 - mPosition;
        if (PSVECMag(&v14) > 10000.0f) {
            reset(1);
        } else {
            if (MR::isBindedWall(this) || MR::isBindedRoof(this)) {
                for (u32 i = 0; i < MR::getBindedPlaneNum(this); i++) {
                    MR::sendArbitraryMsg(ACTMES_JET_TURTLE_ATTACK, MR::getBindedPlaneSensor(this, i), getSensor("body"));
                }

                if (MR::isBindedWall(this)) {
                    if (MR::isWallCodeRebound(&mBinder->mWallInfo.mParentTriangle)) {
                        bound();
                    }
                } else if (MR::isBindedRoof(this)) {
                    if (MR::isWallCodeRebound(&mBinder->mRoofInfo.mParentTriangle)) {
                        bound();
                    }
                } else {
                    MR::shakeCameraWeak();
                    reset(0);
                    return;
                }

                if (MR::isBindedGround(this)) {
                    if (MR::sendMsgToGroundSensor(6, getSensor("body"))) {
                        MR::shakeCameraWeak();
                        reset(0);
                        return;
                    }

                    if (MR::isWallCodeRebound(&mBinder->mGroundInfo.mParentTriangle)) {
                        bound();
                    }
                }
            }
        }

        MtxPtr jointMtx = MR::getJointMtx(this, 0);
        TVec3f v20(jointMtx[0][3], jointMtx[1][3], jointMtx[2][3]);
        TVec3f grav;
        MR::calcGravityVector(this, &grav, nullptr, 0);

        if (MR::isNearZero(grav)) {
            MR::shakeCameraWeak();
            reset(0);
            return;
        }

        if (!MR::isSameDirection(grav, mVelocity, 0.0099999998f)) {
            TVec3f v13 = -grav;
            v20 = v13;
        }

        if (!_98) {
            bool v10 = false;

            if (JGeometry::TUtil< f32 >::epsilonEquals(grav.x, _CC.x, 0.0000038146973f) &&
                JGeometry::TUtil< f32 >::epsilonEquals(grav.y, _CC.y, 0.0000038146973f) &&
                JGeometry::TUtil< f32 >::epsilonEquals(grav.z, _CC.z, 0.0000038146973f)) {
                v10 = true;
            }

            if (!v10) {
                MR::vecBlendSphere(_9C, grav, &_9C, sGravityLevel[_92]);
            }

            _CC = grav;
        }

        TVec3f v18;
        PSVECCrossProduct(&mVelocity, &v20, &v18);
        PSVECCrossProduct(&v18, &mVelocity, &v20);

        if (!MR::isNearZero(mVelocity)) {
            TPos3f frontUp;
            MR::makeMtxFrontUp(&frontUp, mVelocity, v20);

            if ((frontUp.mMtx[2][0] - 1.0f) >= -0.0000038146973f) {
                mRotation.x = JMath::sAtanTable.atan2_(-frontUp.mMtx[0][1], frontUp.mMtx[1][1]);
                mRotation.y = -1.5707964f;
                mRotation.z = 0.0f;
            } else {
                if ((frontUp.mMtx[2][0] + 1.0f) <= 0.0000038146973f) {
                    mRotation.x = JMath::sAtanTable.atan2_(frontUp.mMtx[0][1], frontUp.mMtx[1][1]);
                    mRotation.y = 1.5707964f;
                    mRotation.z = 0.0f;
                } else {
                    mRotation.x = JMath::sAtanTable.atan2_(frontUp.mMtx[2][1], frontUp.mMtx[2][2]);
                    mRotation.z = JMath::sAtanTable.atan2_(frontUp.mMtx[1][0], frontUp.mMtx[0][0]);
                    mRotation.y = JGeometry::TUtil< f32 >::asin(-frontUp.mMtx[2][0]);
                }
            }

            mRotation.x *= 57.295776f;
            mRotation.y *= 57.295776f;
            mRotation.z *= 57.295776f;
        }
    }
}

void JetTurtle::exeTakenReserve() {
    if (MR::isFirstStep(this)) {
        HitSensor* bodySensor = getSensor("body");
        bodySensor->mRadius = 50.0f;
        mVelocity.zero();
        if (MR::isPlayerInWaterMode()) {
            if (mShellType != JETTURTLETYPE_RED) {
                MR::emitEffect(this, "SwimBubble");
            } else {
                MR::emitEffect(this, "SwimBubbleRed");
            }
        }
    }

    f32 v2;

    if (isNerve(&NrvJetTurtle::JetTurtleNrvTakenReserveD::sInstance)) {
        v2 = 0.5f;
    } else {
        v2 = 0.25f;
    }

    TVec3f takePos;
    MR::getPlayerTakePos(&takePos);
    TVec3f v3(takePos);
    v3.scale(v2);
    TVec3f v4(mPosition);
    v4.scale(1.0f - v2);
    TVec3f v5(v4);
    v5 += v3;
    mPosition = v5;

    if (isNerve(&NrvJetTurtle::JetTurtleNrvTakenReserveD::sInstance)) {
        if (MR::isStep(this, 2)) {
            setNerve(&NrvJetTurtle::JetTurtleNrvTakenStart::sInstance);
            return;
        }

        if (MR::isStep(this, 8)) {
            setNerve(&NrvJetTurtle::JetTurtleNrvTakenStart::sInstance);
        }
    }
}

void JetTurtle::exeTakenStart() {
    if (MR::sendMsgTaken(_94, getSensor("body"))) {
        MR::startSystemSE("SE_SY_GET_TURTLE_JET", -1, -1);
        if (!MR::isInWater(this, TVec3f(0.0f, 0.0f, 0.0f))) {
            MR::startSoundPlayer("SE_PV_CATCH", -1);
        }

        MR::deleteEffect(this, "SpinBlur");
        MR::deleteEffect(this, "SpinBlurRed");
        MR::deleteEffect(this, "SpinBlurGold");

        if (mShellType == 2) {
            MR::deleteEffect(this, "Glow");
            MR::emitEffect(this, "HandyGlow");
        }

        setNerve(&NrvJetTurtle::JetTurtleNrvWait::sInstance);
        MR::invalidateShadow(this, nullptr);
    } else {
        reset(0);
    }
}

// missing branch
void JetTurtle::exeRestart() {
    MR::isFirstStep(this);

    if (MR::isStep(this, 45)) {
        mPosition = _A8;
        mRotation = _B4;
        MR::calcGravity(this);
        MR::onCalcShadowOneTime(this, nullptr);
        MR::validateShadow(this, nullptr);
        MR::calcAnimDirect(this);
        MR::validateClipping(this);

        if (_E2) {
            MR::emitEffect(this, "AppearLight");
        }
    }

    if (MR::isGreaterStep(this, 45)) {
        if (_E2) {
            MR::startLevelSound(this, "SE_EM_LV_NOKOLAND_REVIVE_EFFECT", -1, -1, -1);
        } else {
        }

        if (!_E2) {
            s32 step = getNerveStep();

            switch (step & 0x3) {
            case 0:
                MR::showModel(this);
                break;
            case 2:
                MR::hideModel(this);
                break;
            }
        }

        if (MR::isStep(this, 105)) {
            MR::showModel(this);
            setNerve(&NrvJetTurtle::JetTurtleNrvWait::sInstance);
            MR::emitEffect(this, "KouraRevival");
            MR::startSound(this, "SE_EM_NOKOLAND_REVIVE_APPEAR", -1, -1);
        }
    }
}

void JetTurtle::exeDrop() {
    MR::calcGravity(this);
    MR::deleteEffect(this, "BrakeLamp");

    if (MR::isFirstStep(this)) {
        TVec3f front;
        MR::calcFrontVec(&front, this);
        TVec3f v3 = -mGravity;
        TVec3f v4(v3);
        v4.scale(10.0f);
        mVelocity = v4;
        TVec3f v2(front);
        v2.scale(10.0f);
        mVelocity += v2;
    }

    boundDrop();
    MR::addVelocityToGravity(this, 1.0f);

    if (MR::isStep(this, 90)) {
        mVelocity.zero();
        reset(0);
    }
}

// JetTurtle::attackSensor

bool JetTurtle::receiveOtherMsg(u32 msg, HitSensor* a2, HitSensor* a3) {
    if (MR::isMsgItemGet(msg)) {
        if (mFlag.mIsDead) {
            return false;
        } else {
            bool v7 = false;

            if (isNerve(&NrvJetTurtle::JetTurtleNrvWait::sInstance) || isNerve(&NrvJetTurtle::JetTurtleNrvWait2::sInstance)) {
                v7 = true;
            }

            if (v7 || isNerve(&NrvJetTurtle::JetTurtleNrvTakenReserve::sInstance)) {
                _94 = a2;
                setNerve(&NrvJetTurtle::JetTurtleNrvTakenReserveD::sInstance);
                return true;
            } else {
                return false;
            }
        }
    } else {
        if (msg == ACTMES_DAMAGEDROP) {
            resetPositionAndVanish();
            return true;
        } else if (msg == ACTMES_ATTACKDROP) {
            MR::shakeCameraWeak();
            reset(0);
            return true;
        } else if (msg == ACTMES_RUSHDROP) {
            setNerve(&NrvJetTurtle::JetTurtleNrvDrop::sInstance);
            MR::startBck(this, "Drop", nullptr);
            return true;
        } else if (msg == ACTMES_IS_PULL_ENABLE) {
            bool v8 = false;

            if (isNerve(&NrvJetTurtle::JetTurtleNrvWait::sInstance) || isNerve(&NrvJetTurtle::JetTurtleNrvWait2::sInstance)) {
                v8 = true;
            }

            if (v8) {
                return true;
            } else {
                return isNerve(&NrvJetTurtle::JetTurtleNrvTakenReserve::sInstance);
            }
        } else {
            if (MR::isMsgItemPull(msg)) {
                bool v9 = false;

                if (isNerve(&NrvJetTurtle::JetTurtleNrvWait::sInstance) || isNerve(&NrvJetTurtle::JetTurtleNrvWait2::sInstance)) {
                    v9 = true;
                }

                if (v9) {
                    bool v10 = false;

                    if (isNerve(&NrvJetTurtle::JetTurtleNrvWait::sInstance) || isNerve(&NrvJetTurtle::JetTurtleNrvWait2::sInstance)) {
                        v10 = true;
                    }

                    if (v10) {
                        MR::startBck(this, "YRotation", nullptr);
                    }

                    setNerve(&NrvJetTurtle::JetTurtleNrvTakenReserve::sInstance);
                    MR::offBind(this);
                }

                return true;
            }

            return false;
        }
    }
}

bool JetTurtle::receiveMsgThrow(HitSensor* a1, HitSensor* a2) {
    setNerve(&NrvJetTurtle::JetTurtleNrvThrowing::sInstance);
    MR::deleteEffect(this, "BrakeLamp");

    if (MR::isSensorPlayer(a1)) {
        MR::getPlayerThrowVec(&_9C);
        _98 = nullptr;
    } else {
        if (MR::isExistInAttributeGroupSearchTurtle(a1->mHost)) {
            _98 = a1;
        } else {
            _98 = nullptr;
        }

        if (mShellType == 0) {
            _98 = nullptr;
        }

        _9C = a1->mPosition - mPosition;
        MR::normalizeOrZero(&_9C);
    }

    if (!_92) {
        _8C = PSVECMag(MR::getPlayerVelocity());
    } else {
        _8C = 0.0f;
    }

    _C0 = mPosition;
    MR::invalidateClipping(this);
    MR::validateShadow(this, nullptr);
    return true;
}

bool JetTurtle::receiveMsgPlayerAttack(u32 msg, HitSensor*, HitSensor*) {
    if (MR::isMsgStarPieceReflect(msg)) {
        return true;
    }

    if (isNerve(&NrvJetTurtle::JetTurtleNrvWait::sInstance) || isNerve(&NrvJetTurtle::JetTurtleNrvWait2::sInstance)) {
        if (MR::isMsgPlayerHipDrop(msg)) {
            reset(0);
            return true;
        }
    }

    if (MR::isMsgPlayerTrample(msg)) {
        MR::startSound(this, "SE_EM_TURTLE_SHELL_TRAMPLE", -1, -1);
        return true;
    } else {
        if (msg == ACTMES_FOO_ATTACK) {
            reset(0);
            return true;
        }
    }

    return 0;
}

bool JetTurtle::receiveMsgEnemyAttack(u32 msg, HitSensor* a2, HitSensor* a3) {
    if (isNerve(&NrvJetTurtle::JetTurtleNrvTakenReserve::sInstance) || isNerve(&NrvJetTurtle::JetTurtleNrvTakenReserveD::sInstance) ||
        isNerve(&NrvJetTurtle::JetTurtleNrvTakenStart::sInstance)) {
        return false;
    }

    MR::sendArbitraryMsg(ACTMES_JET_TURTLE_ATTACK, a2, getSensor("eye"));
    MR::shakeCameraWeak();
    reset(0);
    return true;
}

bool JetTurtle::isWait() const {
    bool isWait = false;

    if (isNerve(&NrvJetTurtle::JetTurtleNrvWait::sInstance) || isNerve(&NrvJetTurtle::JetTurtleNrvWait2::sInstance)) {
        isWait = true;
    }

    return isWait;
}

bool JetTurtle::isRestart() const {
    return isNerve(&NrvJetTurtle::JetTurtleNrvRestart::sInstance);
}

void GoldenTurtle::init(const JMapInfoIter& rIter) {
    init2(rIter, 1);
    MR::startBtk(this, "KouraShine");
    getSensor("body")->setType(0x10);
}

void GoldenTurtle::resetPosition() {
    JetTurtle::resetPosition();
    getSensor("body")->setType(0x10);
}

void GoldenTurtle::exeThrowing() {
    if (MR::isFirstStep(this)) {
        getSensor("body")->setType(0x11);

        if (mShellType == 2) {
            MR::deleteEffect(this, "HandyGlow");
        }
    }

    JetTurtle::exeThrowing();
}

void GoldenTurtle::reset(u32 a1) {
    JetTurtle::reset(a1);
    getSensor("body")->setType(0x10);
}

JetTurtle::~JetTurtle() {}

GoldenTurtle::~GoldenTurtle() {}
