#include "Game/Enemy/IceMerameraKing.hpp"
#include "Game/Camera/CameraTargetObj.hpp"
#include "Game/Effect/SpinPullParticleCallBack.hpp"
#include "Game/Enemy/Meramera.hpp"
#include "Game/Enemy/MoguStone.hpp"
#include "Game/LiveActor/Binder.hpp"
#include "Game/LiveActor/HitSensor.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/Util/ActorMovementUtil.hpp"
#include "Game/Util/ActorSensorUtil.hpp"
#include "Game/Util/ActorShadowUtil.hpp"
#include "Game/Util/ActorSwitchUtil.hpp"
#include "Game/Util/DemoUtil.hpp"
#include "Game/Util/EffectUtil.hpp"
#include "Game/Util/FixedPosition.hpp"
#include "Game/Util/JMapUtil.hpp"
#include "Game/Util/JointController.hpp"
#include "Game/Util/JointUtil.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/MapUtil.hpp"
#include "Game/Util/MathUtil.hpp"
#include "Game/Util/MtxUtil.hpp"
#include "Game/Util/ObjUtil.hpp"
#include "Game/Util/PlayerUtil.hpp"
#include "Game/Util/SceneUtil.hpp"
#include "Game/Util/SoundUtil.hpp"
#include <JSystem/JGeometry/TUtil.hpp>

// TODO: the symbol order for header funcs is out of order between JointControlDelegator funcs
//       and Array funcs. This needs to be fixed in order to link.

void IceMerameraKing_FORCE_MATCH_SDATA2() {
    (void)1.0f;
    (void)0.0f;
    f32 f3 = JGeometry::TUtil< f32 >::epsilon();
    (void)0.5f;
    (void)3.0f;
    (void)2.0f;
}

namespace NrvIceMerameraKing {
    NEW_NERVE(HostTypeNrvSearch, IceMerameraKing, Search);
    NEW_NERVE_ONEND(HostTypeNrvBeginDemo, IceMerameraKing, Throw, Throw);
    NEW_NERVE_ONEND(HostTypeNrvThrow, IceMerameraKing, Throw, Throw);
    NEW_NERVE_ONEND(HostTypeNrvThrow2nd, IceMerameraKing, Throw, Throw);
    NEW_NERVE(HostTypeNrvExtinguish, IceMerameraKing, Extinguish);
    NEW_NERVE_ONEND(HostTypeNrvEscape, IceMerameraKing, Escape, Escape);
    NEW_NERVE(HostTypeNrvEscapeJump, IceMerameraKing, EscapeJump);
    NEW_NERVE(HostTypeNrvDamage, IceMerameraKing, Damage);
    NEW_NERVE(HostTypeNrvPreRecover, IceMerameraKing, PreRecover);
    NEW_NERVE(HostTypeNrvRecover, IceMerameraKing, Recover);
    NEW_NERVE(HostTypeNrvPreAttack, IceMerameraKing, PreAttack);
    NEW_NERVE(HostTypeNrvAttack, IceMerameraKing, Attack);
    NEW_NERVE(HostTypeNrvAttackAfter, IceMerameraKing, AttackAfter);
    NEW_NERVE(HostTypeNrvAngryDemo1st, IceMerameraKing, AngryDemo);
    NEW_NERVE(HostTypeNrvAngryDemo2nd, IceMerameraKing, AngryDemo);
    NEW_NERVE(HostTypeNrvDeathDemoWait, IceMerameraKing, DeathDemoWait);
    NEW_NERVE(HostTypeNrvDeathDemo, IceMerameraKing, DeathDemo);
};  // namespace NrvIceMerameraKing

namespace {
    static const MR::ActorMoveParam hOnAirParam = {0.0f, 3.0f, 0.96f, 0.0f};
    static const MR::ActorMoveParam hOnGroundParam = {0.0f, 1.0f, 0.8f, 0.0f};
    static const MR::ActorMoveParam hFlyParam = {0.0f, 0.5f, 0.9f, 1.0f};
    static const MR::ActorMoveParam hAngryDemoParam = {0.0f, 0.5f, 0.9f, 1.5f};
    static const MR::ActorMoveParam hEscapeOnGroundParam = {-0.7f, 1.0f, 0.96f, 180.0f};
    static const MR::ActorMoveParam hEscapeOnAirParam = {0.0f, 3.0f, 0.96f, 0.0f};
    static const MR::ActorMoveParam hPreRecoverJumpParam = {0.0f, 1.0f, 0.96f, 0.0f};
    static const MR::ActorMoveParam hDamageJumpParam = {0.0f, 1.9f, 0.98f, 0.0f};
    static const MR::ActorMoveParam hAttackParam = {0.0f, 6.0f, 0.98f, 0.0f};
    static const MR::ActorMoveParam hExtinguishOnAirParam = {0.0f, 0.125f, 0.96f, 0.0f};
    static const MR::ActorMoveParam hExtinguishFallOnAirParam = {0.0f, 4.0f, 0.96f, 0.0f};
    static const TVec3f hBinderOffset(-100.586f, 222.65601f, -91.796898f);
    static const TVec3f hEscapeBinderOffset(-39.0625f, 91.796898f, -47.851601f);
};  // namespace

IceMerameraKing::IceMerameraKing(const char* pName)
    : LiveActor(pName), mFixedPos(nullptr), mThrowingIce(nullptr), mMeramera(nullptr), mIce(), _A8(nullptr), _AC(nullptr), _B0(0, 0, 1), _BC(0, 1, 0),
      _C8(0, 0, 0), _D4(0, 0, 0), _E0(0), _E4(0), _EC(3), _F0(0), mModelArray(nullptr), mCameraTarget(nullptr), mJointController(nullptr),
      _100(0, 0, 0, 1), _110(0.0f, 0.0f, 0.0f), _11C(0.0f), _120(false), _121(false) {
}

void IceMerameraKing::init(const JMapInfoIter& rIter) {
    if (MR::isValidInfo(rIter)) {
        MR::initDefaultPos(this, rIter);
        _C8.set< f32 >(mPosition);
        _D4.set< f32 >(mRotation);
        MR::useStageSwitchWriteDead(this, rIter);
        MR::useStageSwitchReadA(this, rIter);
    }

    mScale.x = 1.5f;
    mScale.y = 1.5f;
    mScale.z = 1.5f;
    initModelManagerWithAnm("IceMerameraKing", nullptr, false);
    MR::calcUpVec(&_BC, this);
    MR::calcFrontVec(&_B0, this);
    MR::connectToSceneEnemy(this);
    f32 v5 = (mScale.y / 1.8f);
    initBinder((130.0f * v5), (130.0f * v5), 0);
    MR::setBinderOffsetVec(this, &_110, true);
    initHitSensor(1);
    MR::addHitSensorAtJointEnemy(this, "body", "JointRoot", 32, (130.0f * v5), TVec3f(0.0f, 0.0f, 0.0f));
    initEffectKeeper(0, nullptr, false);
    initSound(4, false);
    initNerve(&NrvIceMerameraKing::HostTypeNrvSearch::sInstance);
    MR::calcGravity(this);
    MR::initShadowVolumeSphere(this, (130.0f * mScale.y));
    MR::setShadowDropLength(this, nullptr, 1500.0f);
    MR::declarePowerStar(this);
    MR::declareStarPiece(this, 24);
    mFixedPos = new FixedPosition(this, "Top", TVec3f(0, 0, 0), TVec3f(0.0f, 0.0f, 0.0f));
    MR::tryRegisterDemoCast(this, rIter);
    mIce.init(6);
    for (s32 i = 0; i < 6; i++) {
        ThrowingIce* newIce = new ThrowingIce("投擲用の氷");
        newIce->initWithoutIter();
        mIce.push_back(newIce);
        MR::tryRegisterDemoCast(newIce, rIter);
    }

    MR::initLightCtrl(this);
    _A8 = new IceMerameraKingShockWave();
    _A8->initWithoutIter();
    _A8->mRotation.set< f32 >(mRotation);
    _A8->makeActorDead();
    _AC = new ModelObj("壊れモデル", "IceMerameraKingBreak", nullptr, -2, -2, -2, false);
    _AC->initWithoutIter();
    _AC->mScale.set< f32 >(mScale);
    _AC->makeActorDead();
    s32 childNum = MR::getChildObjNum(rIter);
    _F0 = childNum;
    mModelArray = new Meramera*[childNum];

    for (s32 i = 0; i < _F0; i++) {
        mModelArray[i] = new Meramera("メラメラ");
        MR::initChildObj(mModelArray[i], rIter, i);
        mModelArray[i]->makeActorDead();
        MR::tryRegisterDemoCast(mModelArray[i], rIter);
    }

    MR::tryRegisterDemoCast(_AC, rIter);
    MR::registerDemoActionNerve(this, &NrvIceMerameraKing::HostTypeNrvBeginDemo::sInstance, nullptr);
    mSpinParticle = new MerameraParticleCallBack();
    mJointController = MR::createJointDelegatorWithNullChildFunc(this, &IceMerameraKing::calcJoint, "JointRoot");
    makeActorAppeared();
    MR::emitEffect(this, "BodyIce");
    mCameraTarget = new CameraTargetDemoActor(getBaseMtx(), "アクター注目");
    mCameraTarget->initWithoutIter();
    mCameraTarget->mPosition.set< f32 >(_C8);
    mCameraTarget->mRotation.set< f32 >(_D4);
    mCameraTarget->setName("メラキン注目ターゲット");
    MR::tryRegisterDemoCast(mCameraTarget, rIter);
}

void IceMerameraKing::initAfterPlacement() {
    MR::trySetMoveLimitCollision(this);
    u32 binderFlag = mBinder->_8;

    for (s32 i = 0; i < _F0; i++) {
        Binder* binder = mModelArray[i]->mBinder;
        binder->_8 = binderFlag;

        if (!binderFlag) {
            binder->_1EC._2 = false;
        } else {
            binder->_1EC._2 = true;
        }
    }
}

void IceMerameraKing::kill() {
    LiveActor::kill();

    if (MR::isValidSwitchDead(this)) {
        MR::onSwitchDead(this);
    }
    MR::startAfterBossBGM();
    s32 temp = mIce.mCount;

    for (s32 i = 0; i < temp; i++) {
        if (!MR::isDead(mIce[i])) {
            mIce[i]->kill();
        }
    }
}

void IceMerameraKing::control() {
    TVec3f v7;
    MR::copyJointScale(this, "Center", &v7);
    f32 v3 = 130.0f * v7.x;
    MR::setBinderRadius(this, v3);
    _110.y = v3;
    _110.x = 0.0f;
    _110.z = 0.0f;
    getSensor("body")->mRadius = v3;
    MR::setShadowVolumeSphereRadius(this, nullptr, v3);

    if (MR::isValidSwitchA(this)) {
        if (MR::isOnSwitchA(this)) {
            MR::invalidateClipping(this);
        } else {
            MR::validateClipping(this);
        }
    }

    if (!isNerve(&NrvIceMerameraKing::HostTypeNrvEscape::sInstance) && !isNerve(&NrvIceMerameraKing::HostTypeNrvEscapeJump::sInstance)) {
        TQuat4f quad(0, 0, 0, 1);
        _100.slerp(quad, 0.25f);
    }

    if (!isNerve(&NrvIceMerameraKing::HostTypeNrvEscape::sInstance) && !isNerve(&NrvIceMerameraKing::HostTypeNrvEscapeJump::sInstance)) {
        MR::startLevelSound(this, "SE_BM_LV_ICEMERAKING_WAIT");
    }

    if (MR::isDemoActive("メラキンオープニング")) {
        MR::startSystemLevelSE("SE_DM_LV_MUTE_BGM");
        if (MR::isDemoPartLastStep("カメラ寄る")) {
            MR::playLevelMarioPinchBGM(_120);
            _120 = true;
            _121 = true;
        }
    } else if (!isNerve(&NrvIceMerameraKing::HostTypeNrvDeathDemoWait::sInstance) && !isNerve(&NrvIceMerameraKing::HostTypeNrvDeathDemo::sInstance) &&
               MR::isNearPlayer(this, 5000.0f) && _121) {
        MR::playLevelMarioPinchBGM(_120);
        _120 = true;
    }
}

void IceMerameraKing::exeSearch() {
    if (MR::isFirstStep(this)) {
        MR::startAction(this, "Wait");
    }
    addVelocityToInitPos();
    MR::moveAndTurnToPlayer(this, &_B0, ::hFlyParam._0, ::hFlyParam._4, ::hFlyParam._8, ::hFlyParam._C);
    MR::addVelocityKeepHeightUseShadow(this, 400.0f, 1.5f, 300.0f, nullptr);
    f32 shadow = MR::calcDistanceToPlayer(this);

    if (!(4500.0f < shadow)) {
        if (shadow < 1000.0f) {
            setNerve(&NrvIceMerameraKing::HostTypeNrvPreAttack::sInstance);
        } else {
            if (isEnableThrow()) {
                if (!(_EC > 2)) {
                    if (MR::isGreaterStep(this, 100) && isDeadAllIce()) {
                        _E0 = 0;
                        setNerve(&NrvIceMerameraKing::HostTypeNrvThrow2nd::sInstance);
                    }
                } else if (MR::isGreaterStep(this, 200) && isDeadAllIce()) {
                    _E0 = 0;
                    setNerve(&NrvIceMerameraKing::HostTypeNrvThrow::sInstance);
                }
            }
        }
    }
}

void IceMerameraKing::exeThrow() {
    if (MR::isFirstStep(this)) {
        MR::startAction(this, "AttackThrow");
        if (isNerve(&NrvIceMerameraKing::HostTypeNrvBeginDemo::sInstance)) {
            MR::calcFrontVec(&_B0, mCameraTarget);
        }
    }
    MR::moveAndTurnToPlayer(this, &_B0, ::hFlyParam._0, ::hFlyParam._4, ::hFlyParam._8, ::hFlyParam._C);
    MR::addVelocityKeepHeightUseShadow(this, 400.0f, 1.5f, 300.0f, nullptr);
    addVelocityToInitPos();

    if (MR::isStep(this, 22)) {
        mThrowingIce = getDeadWeaponAndAppear();
        mFixedPos->mMtx.getTrans(mThrowingIce->mPosition);
        ThrowingIce* ice = mThrowingIce;
        if (!ice) {
            setNerve(&NrvIceMerameraKing::HostTypeNrvSearch::sInstance);
            return;
        }

        _E0 += 1;

        mThrowingIce->emitIce(ice->mPosition, *MR::getPlayerCenterPos() + *MR::getPlayerVelocity() * 35.0f, -5.0f, mGravity);
        mThrowingIce = nullptr;
        MR::startSound(this, "SE_BM_ICEMERAKING_THROW");
    }

    if (MR::isActionEnd(this)) {
        if (MR::calcDistanceToPlayer(this) < 1000.0f) {
            setNerve(&NrvIceMerameraKing::HostTypeNrvPreAttack::sInstance);
        } else {
            u32 v10 = 3;
            if (isNerve(&NrvIceMerameraKing::HostTypeNrvThrow2nd::sInstance)) {
                v10 = 6;
            }

            if (v10 <= _E0 || !isEnableThrow()) {
                setNerve(&NrvIceMerameraKing::HostTypeNrvSearch::sInstance);
            } else if (isNerve(&NrvIceMerameraKing::HostTypeNrvThrow2nd::sInstance)) {
                setNerve(&NrvIceMerameraKing::HostTypeNrvThrow2nd::sInstance);
            } else {
                setNerve(&NrvIceMerameraKing::HostTypeNrvThrow::sInstance);
            }
        }
    }
}

void IceMerameraKing::endThrow() {
    tearDownThrow();
}

void IceMerameraKing::tearDownThrow() {
    if (mThrowingIce) {
        mThrowingIce->kill();
        mThrowingIce = nullptr;
    }

    if (mMeramera) {
        mMeramera->kill();
        mMeramera = nullptr;
    }
}

void IceMerameraKing::exeExtinguish() {
    f32 temp = ::hOnAirParam._4;  // it just exists
    if (MR::isFirstStep(this)) {
        MR::startAction(this, "GoOut");
        MR::startSound(this, "SE_BM_ICEMERAKING_BLOW");
        MR::startSound(this, "SE_BM_ICEMERAKING_SMOKE");
        MR::deleteEffect(this, "BodyIce");
        MR::emitEffectWithParticleCallBack(this, "BodyIceOff", mSpinParticle);
        TVec3f v4 = mPosition - *MR::getPlayerCenterPos();
        MR::vecKillElement(v4, mGravity, &v4);
        MR::normalizeOrZero(&v4);
        MR::setVelocitySeparateHV(this, v4, 20.0f, 0.0f);
        _11C = 200.0f + MR::getShadowNearProjectionLength(this);
    }

    if (MR::isLessStep(this, 40)) {
        MR::addVelocityKeepHeightUseShadow(this, _11C, 1.3f, 700.0f, nullptr);
        MR::moveAndTurnToPlayer(this, &_B0, ::hExtinguishOnAirParam._0, ::hExtinguishOnAirParam._4, ::hExtinguishOnAirParam._8,
                                ::hExtinguishOnAirParam._C);
    } else if (MR::isOnGround(this) && MR::isActionEnd(this)) {
        MR::moveAndTurnToPlayer(this, &_B0, ::hOnGroundParam._0, ::hOnGroundParam._4, ::hOnGroundParam._8, ::hOnGroundParam._C);
        MR::startSound(this, "SE_BM_ICEMERAKING_LAND");
        _E8 = 0;
        setNerve(&NrvIceMerameraKing::HostTypeNrvEscape::sInstance);
    } else {
        MR::moveAndTurnToPlayer(this, &_B0, ::hExtinguishFallOnAirParam._0, ::hExtinguishFallOnAirParam._4, ::hExtinguishFallOnAirParam._8,
                                ::hExtinguishFallOnAirParam._C);
    }
}

void IceMerameraKing::exeEscape() {
    if (MR::isFirstStep(this)) {
    }
    _E8++;

    if (_E8 > 600 && MR::isOnGround(this)) {
        _E8 = 0;
        setNerve(&NrvIceMerameraKing::HostTypeNrvPreRecover::sInstance);
    } else {
        MR::emitEffect(this, "Rolling");
        if (MR::isOnGround(this)) {
            f32 v11 = MR::getLinerValueFromMinMax(mVelocity.length(), 2.0f, 6.0f, 0.0f, 100.0f);
            MR::startLevelSound(this, "SE_BM_LV_ICEMERAKING_ROLL", v11, -1, 15);
        }

        if (MR::isOnGround(this)) {
            f32 radius = getSensor("body")->mRadius;
            MR::rotateQuatRollBall(&_100, mVelocity, *MR::getGroundNormal(this), (0.5f * radius));
            MR::moveAndTurnToPlayer(this, &_B0, ::hEscapeOnGroundParam._0, ::hEscapeOnGroundParam._4, ::hEscapeOnGroundParam._8,
                                    ::hEscapeOnGroundParam._C);
            if (MR::isBindedWallFront(this, -_B0, 0.25f)) {
                setNerve(&NrvIceMerameraKing::HostTypeNrvEscapeJump::sInstance);
            }
        } else {
            f32 sensor = getSensor("body")->mRadius;
            MR::rotateQuatRollBall(&_100, mVelocity, -mGravity, 0.5f * sensor);
            MR::moveAndTurnToPlayer(this, &_B0, ::hEscapeOnAirParam._0, ::hEscapeOnAirParam._4, ::hEscapeOnAirParam._8, ::hEscapeOnAirParam._C);
        }
    }
}

void IceMerameraKing::endEscape() {
    MR::deleteEffect(this, "Rolling");
}

void IceMerameraKing::exeEscapeJump() {
    if (MR::isStep(this, 20)) {
        MR::addVelocitySeparateHV(this, _B0, 20.0f, 90.0f);
        MR::startSound(this, "SE_BM_ICEMERAKING_JUMP");
    }
    _E8++;

    if (MR::isOnGround(this)) {
        f32 radius = getSensor("body")->mRadius;
        MR::rotateQuatRollBall(&_100, mVelocity, *MR::getGroundNormal(this), (0.5f * radius));
        MR::moveAndTurnToPlayer(this, &_B0, ::hEscapeOnGroundParam._0, ::hEscapeOnGroundParam._4, ::hEscapeOnGroundParam._8,
                                ::hEscapeOnGroundParam._C);

        if (MR::isGreaterStep(this, 20)) {
            MR::emitEffect(this, "Land");
            MR::startSound(this, "SE_BM_ICEMERAKING_LAND");
            setNerve(&NrvIceMerameraKing::HostTypeNrvEscape::sInstance);
        }
    } else {
        f32 radius = getSensor("body")->mRadius;
        MR::rotateQuatRollBall(&_100, mVelocity, -mGravity, (0.5f * radius));
        MR::moveAndTurnToPlayer(this, &_B0, ::hEscapeOnAirParam._0, ::hEscapeOnAirParam._4, ::hEscapeOnAirParam._8, ::hEscapeOnAirParam._C);
    }
}

void IceMerameraKing::exeDamage() {
    if (MR::isFirstStep(this)) {
        MR::startAction(this, "Damage");
        MR::emitEffect(this, "Damage");
        MR::startBlowHitSound(this);
        MR::startSound(this, "SE_BM_ICEMERAKING_DAMAGE");
        MR::setVelocityJump(this, 80.0f);
        MR::stopSceneForDefaultHit(4);
    }

    if (MR::isGreaterStep(this, 20)) {
        MR::addVelocityKeepHeightUseShadow(this, 900.0f, 3.0f, 300.0f, nullptr);
    }

    if (MR::isOnGround(this)) {
        MR::moveAndTurnToPlayer(this, &_B0, ::hOnGroundParam._0, ::hOnGroundParam._4, ::hOnGroundParam._8, ::hOnGroundParam._C);
    } else {
        MR::moveAndTurnToPlayer(this, &_B0, ::hDamageJumpParam._0, ::hDamageJumpParam._4, ::hDamageJumpParam._8, ::hDamageJumpParam._C);
    }

    if (MR::isGreaterStep(this, 60)) {
        if (_EC == 1) {
            if (MR::tryStartDemoRegisteredMarioPuppetable(this, "怒りデモ")) {
                setNerve(&NrvIceMerameraKing::HostTypeNrvAngryDemo2nd::sInstance);
            }
        } else if (MR::tryStartDemoRegisteredMarioPuppetable(this, "怒りデモ")) {
            setNerve(&NrvIceMerameraKing::HostTypeNrvAngryDemo1st::sInstance);
        }
    }
}

void IceMerameraKing::exePreRecover() {
    if (MR::isFirstStep(this)) {
        MR::setVelocityJump(this, 70.0f);
        MR::startSound(this, "SE_BM_ICEMERAKING_JUMP");
    }
    addVelocityToInitPos();

    if (MR::isLessStep(this, 50)) {
        MR::moveAndTurnToPlayer(this, &_B0, ::hPreRecoverJumpParam._0, ::hPreRecoverJumpParam._4, ::hPreRecoverJumpParam._8,
                                ::hPreRecoverJumpParam._C);
    } else if (MR::isStep(this, 50)) {
        MR::startAction(this, "PreRecover");
        MR::setBinderRadius(this, 130.0f);
        HitSensor* sensor = getSensor("body");
        sensor->mRadius = 130.0f;
        MR::setShadowVolumeSphereRadius(this, nullptr, 130.0f);
        MR::moveAndTurnToPlayer(this, &_B0, ::hFlyParam._0, ::hFlyParam._4, ::hFlyParam._8, ::hFlyParam._C);
    } else if (MR::isLessStep(this, 120)) {
        MR::addVelocityKeepHeightUseShadow(this, 750.0f, 1.5f, 300.0f, nullptr);
        MR::moveAndTurnToPlayer(this, &_B0, ::hFlyParam._0, ::hFlyParam._4, ::hFlyParam._8, ::hFlyParam._C);
    } else {
        setNerve(&NrvIceMerameraKing::HostTypeNrvRecover::sInstance);
    }
}

void IceMerameraKing::exeRecover() {
    if (MR::isFirstStep(this)) {
        MR::startAction(this, "Recover");
        MR::emitEffect(this, "BodyIce");
        MR::startSound(this, "SE_BM_ICEMERAKING_PRE_RECOVER");
    }

    if (MR::isStep(this, 80)) {
        MR::startSound(this, "SE_BM_ICEMERAKING_RECOVER");
    }
    addVelocityToInitPos();
    MR::moveAndTurnToPlayer(this, &_B0, ::hFlyParam._0, ::hFlyParam._4, ::hFlyParam._8, ::hFlyParam._C);
    MR::addVelocityKeepHeightUseShadow(this, 750.0f, 1.5f, 300.0f, nullptr);

    if (MR::isGreaterStep(this, 160)) {
        setNerve(&NrvIceMerameraKing::HostTypeNrvSearch::sInstance);
    }
}

void IceMerameraKing::exePreAttack() {
    if (MR::isFirstStep(this)) {
    }

    if (MR::isLessStep(this, 120)) {
        MR::addVelocityKeepHeightUseShadow(this, 800.0f, 1.5f, 300.0f, nullptr);
    } else if (MR::isDead(_A8)) {
        setNerve(&NrvIceMerameraKing::HostTypeNrvAttack::sInstance);
        return;
    }
    addVelocityToInitPos();
    MR::moveAndTurnToPlayer(this, &_B0, ::hFlyParam._0, ::hFlyParam._4, ::hFlyParam._8, ::hFlyParam._C);
}

void IceMerameraKing::exeAttack() {
    if (MR::isFirstStep(this)) {
        MR::startAction(this, "Attack");
        MR::startSound(this, "SE_BM_ICEMERAKING_PRE_ATTACK");
    }

    if (!MR::isActionEnd(this)) {
        MR::addVelocityKeepHeightUseShadow(this, 800.0f, 1.5f, 300.0f, nullptr);
        MR::moveAndTurnToPlayer(this, &_B0, ::hFlyParam._0, ::hFlyParam._4, ::hFlyParam._8, ::hFlyParam._C);
    } else {
        MR::moveAndTurnToPlayer(this, &_B0, ::hAttackParam._0, ::hAttackParam._4, ::hAttackParam._8, ::hAttackParam._C);
    }

    if (MR::isOnGround(this)) {
        setNerve(&NrvIceMerameraKing::HostTypeNrvAttackAfter::sInstance);
    }
}

void IceMerameraKing::exeAttackAfter() {
    if (MR::isFirstStep(this)) {
        _A8->appear();
        _A8->mPosition.set< f32 >(mPosition);
        _A8->mRotation.set< f32 >(_D4);
        MR::emitEffect(this, "Land");
        MR::startAction(this, "AttackEnd");
        MR::startSound(this, "SE_BM_ICEMERAKING_HIP_DROP");
        MR::tryRumblePadAndCameraDistanceStrong(this, 800.0f, 1200.0f, 2000.0f);
    }
    MR::moveAndTurnToPlayer(this, &_B0, ::hOnAirParam._0, ::hOnAirParam._4, ::hOnAirParam._8, ::hOnAirParam._C);

    if (MR::isGreaterStep(this, 90)) {
        setNerve(&NrvIceMerameraKing::HostTypeNrvSearch::sInstance);
    }
}

void IceMerameraKing::exeAngryDemo() {
    if (MR::isFirstStep(this)) {
        MR::emitEffect(this, "BodyIce");
        if (isNerve(&NrvIceMerameraKing::HostTypeNrvAngryDemo2nd::sInstance)) {
            MR::startAction(this, "Summon");
            MR::startSound(this, "SE_BM_ICEMERAKING_PRE_ANGRY2");
        } else {
            MR::startAction(this, "Recover");
            MR::startSound(this, "SE_BM_ICEMERAKING_PRE_ANGRY1");
        }
    }

    if (MR::isStep(this, 1)) {
        MR::resetPosition(this, _C8);
    }
    MR::moveAndTurnToPlayer(this, &_B0, ::hAngryDemoParam._0, ::hAngryDemoParam._4, ::hAngryDemoParam._8, ::hAngryDemoParam._C);
    MR::addVelocityKeepHeightUseShadow(this, 400.0f, 1.5f, 300.0f, nullptr);

    if (isNerve(&NrvIceMerameraKing::HostTypeNrvAngryDemo2nd::sInstance) && MR::isStep(this, 90)) {
        for (int i = 0; i < _F0; i++) {
            mModelArray[i]->makeActorAppeared();
        }
    }

    if (isNerve(&NrvIceMerameraKing::HostTypeNrvAngryDemo2nd::sInstance) && MR::isStep(this, 85)) {
        MR::startSound(this, "SE_BM_ICEMERAKING_ANGRY2");
    } else if (MR::isStep(this, 85)) {
        MR::startSound(this, "SE_BM_ICEMERAKING_ANGRY1");
    }
    MR::playLevelMarioPinchBGM(true);

    if (MR::isDemoPartLastStep("怒りデモ")) {
        if (!(_EC > 2)) {
            MR::appearStarPiece(this, mPosition - mGravity * 200.0f, 8, 15.0f, 70.0f, false);
        } else {
            MR::appearStarPiece(this, mPosition - mGravity * 200.0f, 16, 15.0f, 70.0f, false);
        }
        MR::startSound(this, "SE_OJ_STAR_PIECE_BURST");
        setNerve(&NrvIceMerameraKing::HostTypeNrvSearch::sInstance);
    }
}

void IceMerameraKing::exeDeathDemoWait() {
    if (MR::isFirstStep(this)) {
        MR::stopStageBGM(30);
    }

    mVelocity.zero();
}

void IceMerameraKing::exeDeathDemo() {
    if (MR::isFirstStep(this)) {
        MR::startAction(this, "Death");
        MR::startSound(this, "SE_BM_ICEMERAKING_DEAD");
        mCameraTarget->mPosition.set< f32 >(mPosition);
        _AC->appear();
        _AC->mRotation.set< f32 >(_D4);
        MR::resetPosition(_AC, mPosition);
        MR::startAction(_AC, "Break");
        MR::requestMovementOn(_AC);

        for (s32 i = 0; i < _F0; i++) {
            mModelArray[i]->kill();
        }

        MR::invalidateShadow(this, nullptr);
    }

    s32 frame = MR::getBckFrameMax(this);

    if (MR::isOnGround(this)) {
        mVelocity.zero();
    } else {
        MR::moveAndTurnToPlayer(this, &_B0, ::hOnAirParam._0, ::hOnAirParam._4, ::hOnAirParam._8, ::hOnAirParam._C);
    }

    if (!MR::isHiddenModel(this) && MR::isGreaterStep(this, frame - 37)) {
        MR::hideModelAndOnCalcAnim(this);
    }

    if (!MR::isHiddenModel(_AC) && MR::isActionEnd(_AC)) {
        MR::hideModel(_AC);
    }

    if (MR::isActionEnd(this) && MR::isHiddenModel(this) && MR::isHiddenModel(_AC)) {
        MR::endDemo(this, "メラキンオープニング");
    }

    if (!MR::isDemoActive()) {
        kill();
        _AC->kill();
        MR::requestAppearPowerStar(this, mPosition);
    }
}

void IceMerameraKing::attackSensor(HitSensor* pSender, HitSensor* pReceiver) {
    if (pSender != getSensor("body")) {
        return;
    }

    if (!MR::isSensorPlayer(pReceiver)) {
        return;
    }

    if (isNerve(&NrvIceMerameraKing::HostTypeNrvDamage::sInstance) || isNerve(&NrvIceMerameraKing::HostTypeNrvExtinguish::sInstance) ||
        isNerve(&NrvIceMerameraKing::HostTypeNrvEscape::sInstance) || isNerve(&NrvIceMerameraKing::HostTypeNrvEscapeJump::sInstance) ||
        isNerve(&NrvIceMerameraKing::HostTypeNrvPreRecover::sInstance) || isNerve(&NrvIceMerameraKing::HostTypeNrvRecover::sInstance) ||
        isNerve(&NrvIceMerameraKing::HostTypeNrvDeathDemoWait::sInstance) || isNerve(&NrvIceMerameraKing::HostTypeNrvAngryDemo1st::sInstance) ||
        isNerve(&NrvIceMerameraKing::HostTypeNrvAngryDemo2nd::sInstance) || isNerve(&NrvIceMerameraKing::HostTypeNrvDeathDemo::sInstance) ||
        MR::isPlayerElementModeIce()) {
        MR::sendMsgPush(pReceiver, pSender);
    } else {
        MR::sendMsgEnemyAttackFreeze(pReceiver, pSender);
    }
}

bool IceMerameraKing::receiveMsgPlayerAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    if (MR::isMsgStarPieceReflect(msg)) {
        return true;
    }

    if (!isNerve(&NrvIceMerameraKing::HostTypeNrvEscape::sInstance) && !isNerve(&NrvIceMerameraKing::HostTypeNrvEscapeJump::sInstance)) {
        return false;
    }

    if (MR::isMsgFreezeAttack(msg)) {
        _EC--;

        if (_EC == 0) {
            MR::stopMarioPinchBGMSoon();
            MR::requestStartTimeKeepDemo(this, "メラキンオープニング", &NrvIceMerameraKing::HostTypeNrvDeathDemo::sInstance,
                                         &NrvIceMerameraKing::HostTypeNrvDeathDemoWait::sInstance, "死亡デモ");
        } else {
            setNerve(&NrvIceMerameraKing::HostTypeNrvDamage::sInstance);
        }

        return true;
    }

    if (MR::isMsgPlayerSpinAttack(msg)) {
        _EC--;

        if (_EC == 0) {
            MR::requestStartTimeKeepDemo(this, "メラキンオープニング", &NrvIceMerameraKing::HostTypeNrvDeathDemo::sInstance,
                                         &NrvIceMerameraKing::HostTypeNrvDeathDemoWait::sInstance, "死亡デモ");
        } else {
            setNerve(&NrvIceMerameraKing::HostTypeNrvDamage::sInstance);
        }

        return true;
    }

    return false;
}

bool IceMerameraKing::receiveOtherMsg(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    if (MR::isInSpinStormRange(msg, pSender, pReceiver, 600.0f)) {
        if (isNerve(&NrvIceMerameraKing::HostTypeNrvDeathDemo::sInstance) || isNerve(&NrvIceMerameraKing::HostTypeNrvExtinguish::sInstance) ||
            isNerve(&NrvIceMerameraKing::HostTypeNrvDamage::sInstance) || isNerve(&NrvIceMerameraKing::HostTypeNrvAngryDemo1st::sInstance) ||
            isNerve(&NrvIceMerameraKing::HostTypeNrvAngryDemo2nd::sInstance) || isNerve(&NrvIceMerameraKing::HostTypeNrvEscape::sInstance) ||
            isNerve(&NrvIceMerameraKing::HostTypeNrvEscapeJump::sInstance) || isNerve(&NrvIceMerameraKing::HostTypeNrvPreRecover::sInstance) ||
            isNerve(&NrvIceMerameraKing::HostTypeNrvRecover::sInstance)) {
            return false;
        } else {
            TVec3f v10 = *MR::getPlayerCenterPos() - mPosition;
            MR::vecKillElement(v10, mGravity, &v10);

            if (v10.length() < getSensor("body")->getRadius()) {
                return false;
            } else {
                setNerve(&NrvIceMerameraKing::HostTypeNrvExtinguish::sInstance);
                return true;
            }
        }
    }

    return false;
}

void IceMerameraKing::addVelocityToInitPos() {
    TVec3f v12 = _C8 - mPosition;

    if (0.0f < mGravity.dot(v12)) {
        MR::vecKillElement(v12, mGravity, &v12);
    }
    v12.setLength(0.5f);
    mVelocity.add(v12);
}

void IceMerameraKing::calcAndSetBaseMtx() {
    TPos3f v3;
    MR::makeMtxUpFrontPos(&v3, _BC, _B0, mPosition);
    MR::setBaseTRMtx(this, v3);
    mFixedPos->calc();
    mJointController->registerCallBack();
}

bool IceMerameraKing::isDeadAllIce() {
    s32 count = mIce.mCount;
    for (s32 i = 0; i < count; i++) {
        if (!MR::isDead(mIce[i])) {
            return false;
        }
    }
    return true;
}

bool IceMerameraKing::isEnableThrow() {
    if (MR::isValidSwitchA(this) && !MR::isOnSwitchA(this)) {
        return false;
    }

    if (MR::isPlayerInWaterMode() || MR::isPlayerOnWaterSurface()) {
        return false;
    }
    TVec3f v10 = *MR::getPlayerCenterPos() - mPosition;
    MR::normalizeOrZero(&v10);
    TVec3f v9;
    MR::calcUpVec(&v9, this);
    bool flag = false;
    flag = -0.60f < v9.dot(v10);
    return flag;
}

ThrowingIce* IceMerameraKing::getDeadWeaponAndAppear() {
    ThrowingIce* v7;
    s32 tempE4 = _E4;
    s32 count = mIce.mCount;
    for (s32 i = 0; i < count; i++) {
        if (MR::isDead(mIce[tempE4])) {
            MR::resetPosition(mIce[tempE4], mPosition);
            LiveActor* weapon = mIce[tempE4];
            weapon->appear();
            mIce.begin();
            _E4 += count;
            _E4++;
            _E4 %= count;
            v7 = mIce[tempE4];
            return v7;
        }

        tempE4 = (tempE4 + count + 1) % count;
    }
    return nullptr;
}

bool IceMerameraKing::calcJoint(TPos3f* a2, const JointControllerInfo& info) {
    TPos3f mtx;
    TVec3f v3;
    mtx.makeQuatInline(_100);
    a2->getTrans(v3);
    a2->zeroTransInline2();
    a2->concat(mtx, *a2);
    a2->setTransInline(v3);
    return true;
}

namespace {
    static const char* hScaleJointName[] = {"WideInSide", "WideOutSide", "Hirgh1"};
};

IceMerameraKingShockWave::IceMerameraKingShockWave() : ModelObj("衝撃", "IceMerameraKingShock", nullptr, -2, -2, -2, false) {
    initHitSensor(2);
    MR::addHitSensorEnemyAttack(this, "circle", 16, 15.0f, TVec3f(0.0f, -100.0f, 0.0f));
    MR::addHitSensorEnemyAttack(this, "circle_end", 16, 24.0f, TVec3f(0.0f, -100.0f, 0.0f));
}

void IceMerameraKingShockWave::appear() {
    LiveActor::appear();
    MR::startAction(this, "Shock");
    HitSensor* sensor = getSensor("circle");
    sensor->mRadius = 0.0f;
}

void IceMerameraKingShockWave::control() {
    TVec3f v7;
    MR::copyJointScale(this, ::hScaleJointName[2], &v7);
    f32 v3 = 15.0f * v7.y;
    getSensor("circle")->mRadius = v3;
    MR::copyJointScale(this, ::hScaleJointName[0], &v7);
    f32 v4 = 24.0f * v7.y;
    getSensor("circle_end")->mRadius = v4;
    if (MR::isActionEnd(this)) {
        kill();
    }
}

void IceMerameraKingShockWave::attackSensor(HitSensor* pSender, HitSensor* pReceiver) {
    if (pSender != getSensor("circle_end") && MR::isSensorPlayer(pReceiver)) {
        HitSensor* sensor = getSensor("circle_end");
        TVec3f v14 = sensor->mPosition - pReceiver->mPosition;
        if (v14.length() < sensor->getRadius() + pReceiver->getRadius()) {
            return;
        }

        TVec3f v15 = pReceiver->mPosition - pSender->mPosition;
        TVec3f v16;
        MR::calcUpVec(&v16, this);

        if (__fabsf(v15.dot(v16)) < 200.0f) {
            MR::vecKillElement(v15, v16, &v15);
            MR::sendMsgEnemyAttackFlipMaximumToDir(pReceiver, pSender, v15);
        }
    }
}
