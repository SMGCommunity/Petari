#include "Game/MapObj/JetTurtle.hpp"
#include "Game/LiveActor/Binder.hpp"
#include "Game/LiveActor/HitSensor.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/Util.hpp"
#include "JSystem/JGeometry/TVec.hpp"
#include "revolution/types.h"

#include "Game/Util/StringUtil.hpp"

void JetTurtle_FORCE_MATCH_STRINGS() {
    MR::isEqualString("Koura", "Koura");
    MR::isEqualString("KouraShine", "KouraShine");
    MR::isEqualString("body", "body");
    MR::isEqualString("SE_OJ_TURTLE_JET_LAUNCH_W", "SE_OJ_TURTLE_JET_LAUNCH_W");
    MR::isEqualString("SE_OJ_TURTLE_JET_LAUNCH_L", "SE_OJ_TURTLE_JET_LAUNCH_L");
    MR::isEqualString("BrakeLamp", "BrakeLamp");
    MR::isEqualString("Vanish", "Vanish");
    MR::isEqualString("VanishRed", "VanishRed");
    MR::isEqualString("VanishGold", "VanishGold");
    MR::isEqualString("SpinBlur", "SpinBlur");
    MR::isEqualString("SpinBlurRed", "SpinBlurRed");
    MR::isEqualString("SpinBlurGold", "SpinBlurGold");
    MR::isEqualString("Glow", "Glow");
    MR::isEqualString("Wait", "Wait");
    MR::isEqualString("HandyGlow", "HandyGlow");
    MR::isEqualString("SwimFlutterboard", "SwimFlutterboard");
    MR::isEqualString("SwimBubble", "SwimBubble");
    MR::isEqualString("SwimBubbleRed", "SwimBubbleRed");
    MR::isEqualString("SE_OJ_LV_JET_TURTLE_G_LIGHT", "SE_OJ_LV_JET_TURTLE_G_LIGHT");
    MR::isEqualString("CarryWait", "CarryWait");
    MR::isEqualString("WaterExplosion", "WaterExplosion");
    MR::isEqualString("Star", "Star");
    MR::isEqualString("Explosion", "Explosion");
    MR::isEqualString("ExplosionRed", "ExplosionRed");
    MR::isEqualString("ExplosionGold", "ExplosionGold");
    MR::isEqualString("SE_OJ_TURTLE_JET_BREAK_W", "SE_OJ_TURTLE_JET_BREAK_W");
    MR::isEqualString("SE_OJ_TURTLE_JET_BREAK_L", "SE_OJ_TURTLE_JET_BREAK_L");
    MR::isEqualString("Ignition", "Ignition");
    MR::isEqualString("BulletWater", "BulletWater");
    MR::isEqualString("WaterBlur", "WaterBlur");
    MR::isEqualString("SE_PV_THROW", "SE_PV_THROW");
    MR::isEqualString("Bullet", "Bullet");
    MR::isEqualString("Blur", "Blur");
    MR::isEqualString("BlurRed", "BlurRed");
    MR::isEqualString("BlurGold", "BlurGold");
    MR::isEqualString("SE_SY_GET_TURTLE_JET", "SE_SY_GET_TURTLE_JET");
    MR::isEqualString("SE_PV_CATCH", "SE_PV_CATCH");
    MR::isEqualString("AppearLight", "AppearLight");
    MR::isEqualString("SE_EM_LV_NOKOLAND_REVIVE_EFFECT", "SE_EM_LV_NOKOLAND_REVIVE_EFFECT");
    MR::isEqualString("KouraRevival", "KouraRevival");
    MR::isEqualString("SE_EM_NOKOLAND_REVIVE_APPEAR", "SE_EM_NOKOLAND_REVIVE_APPEAR");
    MR::isEqualString("BulletSearch", "BulletSearch");
    MR::isEqualString("Drop", "Drop");
    MR::isEqualString("YRotation", "YRotation");
    MR::isEqualString("SE_EM_TURTLE_SHELL_TRAMPLE", "SE_EM_TURTLE_SHELL_TRAMPLE");
}

namespace {
    static const f32 sThrowSpdStraight[] = {30.0f, 20.0f, 30.0f};
    static const f32 sThrowSpdHoming[] = {24.0f, 20.0f, 24.0f};
    static const f32 sGravityLevel[] = {0.017f, 0.01f, 0.0055f};
    static const u16 sResetStep[] = {300, 480, 300};
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

JetTurtle::JetTurtle(const char* pName) : LiveActor(pName), _E3() {
}

void JetTurtle::init(const JMapInfoIter& rIter) {
    init2(rIter, 0);
}

void JetTurtle::init2(const JMapInfoIter& rIter, s32 param2) {
    _D8 = 0;

    MR::initDefaultPos(this, rIter);

    s32 arg0 = 0;
    s32 arg1 = -1;

    if (MR::isValidInfo(rIter)) {
        MR::getJMapInfoArg0NoInit(rIter, &arg0);
        MR::getJMapInfoArg1NoInit(rIter, &arg1);

        arg1++;
    } else {
        arg1 = 0;
    }

    _92 = arg1;
    _E2 = false;
    _E0 = true;

    f32 shadowDropLength = 1000.0f;

    if (MR::isValidInfo(rIter)) {
        s32 arg2 = -1;
        MR::getJMapInfoArg2NoInit(rIter, &arg2);

        if (arg2 == 1) {
            _E0 = false;
        }

        s32 arg3 = -1;
        MR::getJMapInfoArg3NoInit(rIter, &arg3);

        if (arg3 == 1) {
            _E2 = true;
        }

        s32 arg7 = -1;
        MR::getJMapInfoArg7NoInit(rIter, &arg7);

        if (arg7 != -1) {
            shadowDropLength = arg7;
        }
    }

    if (param2 != 0) {
        arg0 = 2;
    }

    switch (arg0) {
    case 0:
    case 1:
        initModelManagerWithAnm("Koura", nullptr, false);
        break;
    case 2:
        initModelManagerWithAnm("KouraShine", nullptr, false);
        break;
    }

    MR::connectToSceneNoSilhouettedMapObjStrongLight(this);

    initHitSensor(2);
    MR::addHitSensor(this, "body", ATYPE_JET_TURTLE, 8, 50.0f, TVec3f(0.0f, 0.0f, 0.0f));
    MR::addHitSensorEye(this, "eye", 8, 1000.0f, TVec3f(0.0f, 0.0f, 0.0f));

    if (arg0 < 2) {
        MR::startBrk(this, "Koura");
        MR::setBrkFrameAndStop(this, 1.0f);
    }

    if (arg0 != 1) {
        if (arg0 == 0) {
            becomeSlowType();
        } else {
            getSensor("body")->setType(ATYPE_JET_TURTLE_SLOW);
        }
    }

    mShellType = arg0;
    _8C = 0.0f;

    initNerve(GET_NERVE(JetTurtle, JetTurtleNrvWait));

    MR::initShadowVolumeCylinder(this, 50.0f);
    MR::setShadowDropLength(this, nullptr, shadowDropLength);
    MR::setShadowVolumeEndDropOffset(this, nullptr, 50.0f);
    MR::onShadowVolumeCutDropLength(this, nullptr);

    initSound(6, false);
    initBinder(35.0f, 20.0f, 8);
    initEffectKeeper(8, "Koura", false);
    MR::addEffectHitNormal(this, nullptr);
    MR::validateShadow(this, nullptr);

    if (MR::useStageSwitchReadAppear(this, rIter)) {
        MR::syncStageSwitchAppear(this);
        makeActorDead();

        _E1 = false;
    } else {
        _E1 = true;

        makeActorAppeared();
    }

    MR::offBind(this);
}

void JetTurtle::initAfterPlacement() {
    _A8 = mPosition;
    _B4 = mRotation;
}

void JetTurtle::becomeSlowType() {
    getSensor("body")->setType(ATYPE_JET_TURTLE_SLOW);
    MR::startBrk(this, "Koura");
    MR::setBrkFrameAndStop(this, 0.0f);
    mShellType = 0;
}

void JetTurtle::becomeFastType() {
    getSensor("body")->setType(ATYPE_JET_TURTLE);
    MR::startBrk(this, "Koura");
    MR::setBrkFrameAndStop(this, 1.0f);
    mShellType = 1;
}

// ...

void JetTurtle::appearAndTryTaken() {
    appear();
    MR::validateHitSensors(this);
    setNerve(GET_NERVE(JetTurtle, JetTurtleNrvTakenReserve));
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
    MR::stopSound(this, "SE_OJ_TURTLE_JET_LAUNCH_W");
    MR::stopSound(this, "SE_OJ_TURTLE_JET_LAUNCH_L");
    mVelocity.zero();
    mRotation.zero();
    _94 = nullptr;
    setNerve(GET_NERVE(JetTurtle, JetTurtleNrvRestart));
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

void JetTurtle::exeWait() {
    if (MR::isFirstStep(this)) {
        if (isNerve(GET_NERVE(JetTurtle, JetTurtleNrvTakenReserve))) {
            switch (mShellType) {
            default:
                MR::emitEffect(this, "SpinBlur");
                break;
            case 1:
                MR::emitEffect(this, "SpinBlurRed");
                break;
            case 2:
                MR::emitEffect(this, "SpinBlurGold");
                break;
            }

            MR::invalidateClipping(this);
            return;
        }

        switch (mShellType) {
        case 2:
            MR::emitEffect(this, "Glow");
            break;
        }

        MR::calcGravity(this);
        MR::onCalcShadowOneTime(this, nullptr);
        MR::validateShadow(this, nullptr);
        MR::validateHitSensors(this);
        getSensor("eye")->invalidate();
        MR::startBck(this, "Wait");
    }

    if (_D8 != 0) {
        mRotation.y = 24.0f * _D8;
        _D8--;
    }

    if (isNerve(GET_NERVE(JetTurtle, JetTurtleNrvTakenReserve))) {
        if (getNerveStep() >= 120) {
            TVec3f direction = *MR::getPlayerCenterPos() - mPosition;
            f32 rate = MR::clamp((getNerveStep() - 120) / 120.0f, 0.0f, 1.0f);
            direction.setLength(mVelocity.length());
            MR::vecBlendSphere(mVelocity, direction, &mVelocity, rate);
        }

        if (MR::isStep(this, 300)) {
            reset(0);
        }
    }
}

void JetTurtle::exeThrowWait() {
    if (MR::isFirstStep(this)) {
        MR::onCalcGravity(this);
    }

    MR::invalidateHitSensors(this);

    if (mShellType == 2) {
        if (MR::isHiddenModel(this)) {
            MR::deleteEffect(this, "HandyGlow");
        } else {
            MR::emitEffect(this, "HandyGlow");
        }
    }

    if (MR::isPlayerInWaterMode()) {
        MR::tryStartBck(this, "SwimFlutterboard");

        if (mShellType != (s16)1) {
            MR::emitEffect(this, "SwimBubble");
        } else {
            MR::emitEffect(this, "SwimBubbleRed");
        }

        if (mShellType == 2) {
            MR::startLevelSound(this, "SE_OJ_LV_JET_TURTLE_G_LIGHT");
        }
    } else {
        MR::tryStartBck(this, "CarryWait");
        MR::deleteEffect(this, "SwimBubble");
        MR::deleteEffect(this, "SwimBubbleRed");
    }
}

void JetTurtle::appear() {
    LiveActor::appear();
    _98 = nullptr;
    _94 = nullptr;
    MR::showModel(this);

    if (_E1 != 0) {
        if (_92 == 0) {
            TVec3f gravityVec;
            TVec3f gravityVec2;
            TVec3f frontVec;
            MR::calcGravityVectorOrZero(this, mPosition, &gravityVec, nullptr, 0);
            MR::calcFrontVec(&frontVec, this);
            MR::calcGravityVectorOrZero(this, mPosition + frontVec * 100.0f, &gravityVec2, nullptr, 0);

            if (!MR::isNearZero(gravityVec - gravityVec2)) {
                _92 = 1;
            }
        }

        setNerve(GET_NERVE(JetTurtle, JetTurtleNrvWait2));
        HitSensor* pSensor = getSensor("body");
        pSensor->mRadius = 60.0f;
        initAfterPlacement();
        return;
    }

    setNerve(GET_NERVE(JetTurtle, JetTurtleNrvWait));
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
            _9C = v22 - *MR::getGroundNormal(this) * v3 * 0.80f;
        }
    }

    if (MR::isBindedWall(this)) {
        TVec3f v21;
        f32 v6 = MR::vecKillElement(_9C, *MR::getWallNormal(this), &v21);

        if (v6 < 0.0f) {
            _9C = v21 - *MR::getWallNormal(this) * v6 * 0.9f;
        }
    }

    if (MR::isBindedRoof(this)) {
        TVec3f v21;
        f32 v6 = MR::vecKillElement(_9C, *MR::getRoofNormal(this), &v21);

        if (v6 < 0.0f) {
            _9C = v21 - *MR::getRoofNormal(this) * v6 * 0.8f;
        }
    }
}

inline void JetTurtle::endWait() {
    if (mShellType == JETTURTLETYPE_GOLD) {
        MR::deleteEffect(this, "Glow");
    }
}

void JetTurtle::reset(u32 reason) {
    _E3 = 0;
    MR::invalidateHitSensors(this);
    MR::forceDeleteEffectAll(this);
    MR::stopSound(this, "SE_OJ_TURTLE_JET_LAUNCH_W");
    MR::stopSound(this, "SE_OJ_TURTLE_JET_LAUNCH_L");

    if (MR::isInWater(this, TVec3f(0.0f, 0.0f, 0.0f))) {
        MR::emitEffect(this, "WaterExplosion");
    } else if (reason != 0) {
        MR::emitEffect(this, "Star");
    } else {
        switch (mShellType) {
        default:
            MR::emitEffect(this, "Explosion");
            break;
        case 1:
            MR::emitEffect(this, "ExplosionRed");
            break;
        case 2:
            MR::emitEffect(this, "ExplosionGold");
            break;
        }
    }

    if (MR::isInWater(this, TVec3f(0.0f, 0.0f, 0.0f))) {
        MR::startSound(this, "SE_OJ_TURTLE_JET_BREAK_W");
        MR::releaseSoundHandle(this, "SE_OJ_TURTLE_JET_BREAK_W");
    } else {
        MR::startSound(this, "SE_OJ_TURTLE_JET_BREAK_L");
        MR::releaseSoundHandle(this, "SE_OJ_TURTLE_JET_BREAK_L");
    }

    mVelocity.zero();
    mRotation.zero();
    _94 = nullptr;
    MR::invalidateClipping(this);
    setNerve(GET_NERVE(JetTurtle, JetTurtleNrvRestart));
    MR::hideModel(this);
    _98 = nullptr;

    if (!_E0) {
        kill();
    }

    MR::offBind(this);
}

void JetTurtle::boundDrop() {
    TVec3f gravity;
    MR::calcGravityVector(this, &gravity, nullptr, 0);
    TVec3f horizontal;
    f32 vertical = MR::vecKillElement(mVelocity, gravity, &horizontal);

    if (MR::isBindedGround(this) && vertical > 0.0f) {
        if (vertical < 5.0f) {
            mVelocity.zero();
        } else {
            horizontal.scale(0.8999999761581421f);
            mVelocity = horizontal - gravity * vertical * 0.800000011920929f;
        }
    }

    if (MR::isBindedWall(this)) {
        horizontal.scale(-0.8999999761581421f);
        mVelocity = horizontal + gravity * vertical;
    }

    if (MR::isBindedRoof(this)) {
        mVelocity.zero();
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
                MR::startSound(this, "SE_OJ_TURTLE_JET_LAUNCH_W");
                MR::startBck(this, "BulletWater");
                MR::emitEffect(this, "WaterBlur");
            } else {
                MR::startSound(this, "SE_OJ_TURTLE_JET_LAUNCH_L");
                MR::startSoundPlayer("SE_PV_THROW", -1);
                MR::startBck(this, "Bullet");

                switch (mShellType) {
                default:
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

        if (MR::isStep(this, ::sResetStep[_92])) {
            if ((_C0 - mPosition).length() > 5000.0f) {
                reset(1);
            } else {
                reset(0);
            }

            return;
        }

        TVec3f v22;

        if (_98 != nullptr) {
            if (!_98->mValidByHost) {
                _98 = nullptr;
            } else {
                TVec3f v21 = _98->mPosition - mPosition;
                MR::normalizeOrZero(&v21);
                MR::vecBlendSphere(_9C, v21, &_9C, 0.2f);

                if (MR::isBckOneTimeAndStopped(this)) {
                    if (MR::isInWater(this, TVec3f(0.0f, 0.0f, 0.0f))) {
                        MR::startBck(this, "BulletWater");
                    } else {
                        MR::startBck(this, "Bullet");
                    }
                }

                v22 = _9C;
                v22.setLength(_8C + ::sThrowSpdHoming[_92]);
                mVelocity = v22;
            }
        }

        if (_98 == nullptr) {
            v22 = _9C;
            v22.setLength(_8C + ::sThrowSpdStraight[_92]);
            mVelocity = v22;
        }

        if ((_C0 - mPosition).length() > 10000.0f) {
            reset(1);
        } else {
            if (MR::isBindedWall(this) || MR::isBindedRoof(this)) {
                for (u32 i = 0; i < MR::getBindedPlaneNum(this); i++) {
                    MR::sendArbitraryMsg(ACTMES_JET_TURTLE_ATTACK, MR::getBindedPlaneSensor(this, i), getSensor("body"));
                }

                if (MR::isBindedWall(this) && MR::isWallCodeRebound(&mBinder->mWallInfo.mParentTriangle)) {
                    bound();
                } else if (MR::isBindedRoof(this) && MR::isWallCodeRebound(&mBinder->mRoofInfo.mParentTriangle)) {
                    bound();
                } else {
                    MR::shakeCameraWeak();
                    reset(0);
                    return;
                }
            }

            if (MR::isBindedGround(this)) {
                if (MR::sendMsgToGroundSensor(ACTMES_JET_TURTLE_ATTACK, getSensor("body"))) {
                    MR::shakeCameraWeak();
                    reset(0);
                    return;
                }

                if (MR::isWallCodeRebound(&mBinder->mGroundInfo.mParentTriangle)) {
                    bound();
                }
            }
        }

        MtxPtr jointMtx = MR::getJointMtx(this, 0);
        TVec3f v20(jointMtx[0][1], jointMtx[1][1], jointMtx[2][1]);
        TVec3f grav;
        MR::calcGravityVector(this, &grav, nullptr, 0);

        if (MR::isNearZero(grav)) {
            MR::shakeCameraWeak();
            reset(0);
            return;
        }

        if (!MR::isSameDirection(grav, mVelocity)) {
            v20 = -grav;
        }

        if (_98 == nullptr) {
            if (!(grav == _CC)) {
                MR::vecBlendSphere(_9C, grav, &_9C, ::sGravityLevel[_92]);
            }

            _CC = grav;
        }

        TVec3f v18 = mVelocity.cross(v20);
        v20.cross(v18, mVelocity);

        if (!MR::isNearZero(mVelocity)) {
            TPos3f frontUp;
            MR::makeMtxFrontUp(&frontUp, mVelocity, v20);
            frontUp.getEulerXYZ(mRotation);
            mRotation.mult(57.295776f);
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

    if (isNerve(GET_NERVE(JetTurtle, JetTurtleNrvTakenReserveD))) {
        v2 = 0.5f;
    } else {
        v2 = 0.25f;
    }

    TVec3f takePos;
    MR::getPlayerTakePos(&takePos);
    mPosition = mPosition * (1.0f - v2) + takePos * v2;

    if (isNerve(GET_NERVE(JetTurtle, JetTurtleNrvTakenReserveD))) {
        if (MR::isStep(this, 2)) {
            setNerve(GET_NERVE(JetTurtle, JetTurtleNrvTakenStart));
        }

        return;
    }

    if (MR::isStep(this, 8)) {
        setNerve(GET_NERVE(JetTurtle, JetTurtleNrvTakenStart));
    }
}

void JetTurtle::exeTakenStart() {
    if (MR::sendMsgTaken(_94, getSensor("body"))) {
        MR::startSystemSE("SE_SY_GET_TURTLE_JET");

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

        setNerve(GET_NERVE(JetTurtle, JetTurtleNrvWait));
        MR::invalidateShadow(this, nullptr);
    } else {
        reset(0);
    }
}

// missing branch
void JetTurtle::exeRestart() {
    if (MR::isFirstStep(this)) {
    }

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
            MR::startLevelSound(this, "SE_EM_LV_NOKOLAND_REVIVE_EFFECT");
        }
    } else {
        return;
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
        setNerve(GET_NERVE(JetTurtle, JetTurtleNrvWait));
        MR::emitEffect(this, "KouraRevival");
        MR::startSound(this, "SE_EM_NOKOLAND_REVIVE_APPEAR");
    }
}

void JetTurtle::exeDrop() {
    MR::calcGravity(this);
    MR::deleteEffect(this, "BrakeLamp");

    if (MR::isFirstStep(this)) {
        TVec3f front;
        MR::calcFrontVec(&front, this);
        mVelocity = -mGravity * 10.0f;
        mVelocity += front * 10.0f;
    }

    boundDrop();
    MR::addVelocityToGravity(this, 1.0f);

    if (MR::isStep(this, 90)) {
        mVelocity.zero();
        reset(0);
    }
}
inline bool JetTurtle::isNerveWait() {
    return isNerve(GET_NERVE(JetTurtle, JetTurtleNrvWait)) || isNerve(GET_NERVE(JetTurtle, JetTurtleNrvWait2));
}

void JetTurtle::attackSensor(HitSensor* pSender, HitSensor* pReceiver) {
    if (MR::isSensorPlayer(pReceiver)) {
        if (isNerveWait()) {
            if (getNerveStep() >= 2 && !MR::isSensorEye(pSender)) {
                MR::sendMsgPush(pReceiver, pSender);
            }
        }

        return;
    }

    if (isNerveWait()) {
        MR::sendMsgPush(pReceiver, pSender);
    }

    if (isNerve(GET_NERVE(JetTurtle, JetTurtleNrvThrowing)) && !MR::isSensorItem(pReceiver)) {
        if (MR::isSensorEye(pSender)) {
            if (_98 == nullptr && mShellType != 0 && MR::isExistInAttributeGroupSearchTurtle(pReceiver->mHost)) {
                TVec3f difference = pReceiver->mPosition - pSender->mPosition;

                if (MR::diffAngleAbs(mVelocity, difference) < 0.5235988f) {
                    MR::normalizeOrZero(&difference);
                    MR::vecBlendSphere(_9C, difference, &_9C, 0.2f);
                    _98 = pReceiver;
                    MR::startBck(this, "BulletSearch");
                }
            }
        } else if (MR::sendArbitraryMsg(ACTMES_JET_TURTLE_ATTACK, pReceiver, pSender)) {
            MR::shakeCameraWeak();
            _E3 = 1;
            reset(0);
        }
    }
}

bool JetTurtle::receiveOtherMsg(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    if (MR::isMsgItemGet(msg)) {
        if (mFlag.mIsDead) {
            return false;
        } else {
            bool v7 = false;

            if (isNerve(GET_NERVE(JetTurtle, JetTurtleNrvWait)) || isNerve(GET_NERVE(JetTurtle, JetTurtleNrvWait2))) {
                v7 = true;
            }

            if (v7 || isNerve(GET_NERVE(JetTurtle, JetTurtleNrvTakenReserve))) {
                _94 = pSender;
                setNerve(GET_NERVE(JetTurtle, JetTurtleNrvTakenReserveD));
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
            setNerve(GET_NERVE(JetTurtle, JetTurtleNrvDrop));
            MR::startBck(this, "Drop");
            return true;
        } else if (msg == ACTMES_IS_PULL_ENABLE) {
            bool v8 = false;

            if (isNerve(GET_NERVE(JetTurtle, JetTurtleNrvWait)) || isNerve(GET_NERVE(JetTurtle, JetTurtleNrvWait2))) {
                v8 = true;
            }

            if (v8) {
                return true;
            } else {
                return isNerve(GET_NERVE(JetTurtle, JetTurtleNrvTakenReserve));
            }
        } else {
            if (MR::isMsgItemPull(msg)) {
                bool v9 = false;

                if (isNerve(GET_NERVE(JetTurtle, JetTurtleNrvWait)) || isNerve(GET_NERVE(JetTurtle, JetTurtleNrvWait2))) {
                    v9 = true;
                }

                if (v9) {
                    bool v10 = false;

                    if (isNerve(GET_NERVE(JetTurtle, JetTurtleNrvWait)) || isNerve(GET_NERVE(JetTurtle, JetTurtleNrvWait2))) {
                        v10 = true;
                    }

                    if (v10) {
                        MR::startBck(this, "YRotation");
                    }

                    setNerve(GET_NERVE(JetTurtle, JetTurtleNrvTakenReserve));
                    MR::offBind(this);
                }

                return true;
            }

            return false;
        }
    }
}

bool JetTurtle::receiveMsgThrow(HitSensor* pSender, HitSensor* pReceiver) {
    setNerve(GET_NERVE(JetTurtle, JetTurtleNrvThrowing));
    MR::deleteEffect(this, "BrakeLamp");

    if (MR::isSensorPlayer(pSender)) {
        MR::getPlayerThrowVec(&_9C);
        _98 = nullptr;
    } else {
        if (MR::isExistInAttributeGroupSearchTurtle(pSender->mHost)) {
            _98 = pSender;
        } else {
            _98 = nullptr;
        }

        if (mShellType == 0) {
            _98 = nullptr;
        }

        _9C = pSender->mPosition - mPosition;
        MR::normalizeOrZero(&_9C);
    }

    if (!_92) {
        _8C = MR::getPlayerVelocity()->length();
    } else {
        _8C = 0.0f;
    }

    _C0 = mPosition;
    MR::invalidateClipping(this);
    MR::validateShadow(this, nullptr);
    return true;
}

bool JetTurtle::receiveMsgPlayerAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    if (MR::isMsgStarPieceReflect(msg)) {
        return true;
    }

    if (isNerve(GET_NERVE(JetTurtle, JetTurtleNrvWait)) || isNerve(GET_NERVE(JetTurtle, JetTurtleNrvWait2))) {
        if (MR::isMsgPlayerHipDrop(msg)) {
            reset(0);
            return true;
        }
    }

    if (MR::isMsgPlayerTrample(msg)) {
        MR::startSound(this, "SE_EM_TURTLE_SHELL_TRAMPLE");
        return true;
    } else {
        if (msg == ACTMES_FOO_ATTACK) {
            reset(0);
            return true;
        }
    }

    return false;
}

bool JetTurtle::receiveMsgEnemyAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    if (isNerve(GET_NERVE(JetTurtle, JetTurtleNrvTakenReserve)) || isNerve(GET_NERVE(JetTurtle, JetTurtleNrvTakenReserveD)) ||
        isNerve(GET_NERVE(JetTurtle, JetTurtleNrvTakenStart))) {
        return false;
    }

    MR::sendArbitraryMsg(ACTMES_JET_TURTLE_ATTACK, pSender, getSensor("eye"));
    MR::shakeCameraWeak();
    reset(0);
    return true;
}

bool JetTurtle::isWait() const {
    bool isWait = false;

    if (isNerve(GET_NERVE(JetTurtle, JetTurtleNrvWait)) || isNerve(GET_NERVE(JetTurtle, JetTurtleNrvWait2))) {
        isWait = true;
    }

    return isWait;
}

bool JetTurtle::isRestart() const {
    return isNerve(GET_NERVE(JetTurtle, JetTurtleNrvRestart));
}

void GoldenTurtle::init(const JMapInfoIter& rIter) {
    init2(rIter, 1);
    MR::startBtk(this, "KouraShine");
    getSensor("body")->setType(ATYPE_JET_TURTLE_SLOW);
}

void GoldenTurtle::resetPosition() {
    JetTurtle::resetPosition();
    getSensor("body")->setType(ATYPE_JET_TURTLE_SLOW);
}

void GoldenTurtle::exeThrowing() {
    if (MR::isFirstStep(this)) {
        getSensor("body")->setType(ATYPE_SPECIAL_WEAPON);

        if (mShellType == 2) {
            MR::deleteEffect(this, "HandyGlow");
        }
    }

    JetTurtle::exeThrowing();
}

void GoldenTurtle::reset(u32 a1) {
    JetTurtle::reset(a1);
    getSensor("body")->setType(ATYPE_JET_TURTLE_SLOW);
}

JetTurtle::~JetTurtle() {
}

GoldenTurtle::~GoldenTurtle() {
}
