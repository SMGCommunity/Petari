#include "Game/Enemy/KoopaJrShip.hpp"
#include "Game/Enemy/HomingKiller.hpp"
#include "Game/Enemy/Kameck.hpp"
#include "Game/Enemy/KoopaJrShipCannonMainShell.hpp"
#include "Game/Enemy/KoopaJrShipCannonShell.hpp"
#include "Game/LiveActor/HitSensor.hpp"
#include "Game/LiveActor/ModelObj.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/NPC/KoopaJr.hpp"
#include "Game/NameObj/NameObjArchiveListCollector.hpp"
#include "Game/Util/ActorMovementUtil.hpp"
#include "Game/Util/ActorSensorUtil.hpp"
#include "Game/Util/DemoUtil.hpp"
#include "Game/Util/EffectUtil.hpp"
#include "Game/Util/Functor.hpp"
#include "Game/Util/JointUtil.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/MathUtil.hpp"
#include "Game/Util/MtxUtil.hpp"
#include "Game/Util/ObjUtil.hpp"
#include "Game/Util/PlayerUtil.hpp"
#include "Game/Util/RailUtil.hpp"
#include "Game/Util/SceneUtil.hpp"
#include "Game/Util/SoundUtil.hpp"
#include <algorithm>

namespace {
    static const char* cJointNamePropellerBack0 = "Screw00";
    static const char* cJointNamePropellerBack1 = "Screw01";
    static const char* cJointNamePropellerTop = "Propeller00";
    static const char* cJointNameCannonMain = "FirePoint3";
    static const char* cJointNameKoopaJrPos = "KoopaJrShip";
    static const char* cJointNamePodPos = "KoopaJrShipPod";

    static const Vec sKillerLauncherAngle[6] = {{0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f},
                                                {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}};

    static const Vec sKoopaJrPos = {135.0f, 188.0f, 0.0f};
    static const Vec sKoopaJrPosFront = {0.0f, 188.0f, 135.0f};

    static const char* const cJointNameCannon[6] = {"FirePoint0", "FirePoint1", "FirePoint2", "FirePoint4", "FirePoint5", "FirePoint6"};
    static const char* const cEffectNameShoot[6] = {"ShootJ0", "ShootJ1", "ShootJ2", "ShootJ4", "ShootJ5", "ShootJ6"};
};  // namespace

namespace NrvKoopaJrShip {
    NEW_NERVE(HostTypeAppear, KoopaJrShip, Appear);
    NEW_NERVE(HostTypeMove, KoopaJrShip, Move);
    NEW_NERVE(HostTypeMoveFrontAttack, KoopaJrShip, MoveFrontAttack);
    NEW_NERVE(HostTypeShoot1, KoopaJrShip, Shoot);
    NEW_NERVE(HostTypeShoot2, KoopaJrShip, Shoot);
    NEW_NERVE(HostTypeShootMain, KoopaJrShip, ShootMain);
    NEW_NERVE(HostTypeStopAtEnd, KoopaJrShip, StopAtEnd);
    NEW_NERVE(HostTypePowerUp, KoopaJrShip, PowerUp);
    NEW_NERVE(HostTypeDamage, KoopaJrShip, Damage);
    NEW_NERVE(HostTypeBreakStart, KoopaJrShip, BreakStart);
    NEW_NERVE(HostTypeBreak, KoopaJrShip, Break);
    NEW_NERVE(HostTypeBreakEnd, KoopaJrShip, BreakEnd);
    NEW_NERVE(HostTypeTurnFront, KoopaJrShip, TurnFront);
};  // namespace NrvKoopaJrShip

void KoopaJrShip_float_ordering0() {
    (void)1.0f;
    (void)0.0f;
    (void)2.0f;
}

KoopaJrShip::KoopaJrShip(const char* pName)
    : LiveActor(pName), mShellHolder(nullptr), mMainShellHolder(nullptr), mJr(nullptr), mShipBreakModel(nullptr), mPodModel(nullptr), _D0(5),
      _D4(gZeroVec), _E0(0.0f, 0.0f, 1.0f), _EC(0), mPropRotateSpeed(20.0f), _184(0.0f), _188(0), _1EC(::sKoopaJrPos), _1F8(gZeroVec) {
    mScrew00Mtx.identity();
    mScrew01Mtx.identity();
    mPropellerMtx.identity();
    mShipMtx.identity();
    mPodMtx.identity();
}

bool KoopaJrShip::isStateBreak(void) const {
    return isNerve(&NrvKoopaJrShip::HostTypeBreakStart::sInstance) || isNerve(&NrvKoopaJrShip::HostTypeBreak::sInstance) ||
           isNerve(&NrvKoopaJrShip::HostTypeBreakEnd::sInstance);
}

void KoopaJrShip::init(const JMapInfoIter& rIter) {
    MR::initDefaultPos(this, rIter);
    initModelManagerWithAnm("KoopaJrShip", nullptr, false);
    MR::startBck(this, "KoopaJrShip", nullptr);
    MR::setBckRate(this, 0.0f);
    MR::initJointTransform(this);
    MR::setJointTransformLocalMtx(this, ::cJointNamePropellerTop, mPropellerMtx);
    MR::setJointTransformLocalMtx(this, ::cJointNamePropellerBack0, mScrew00Mtx);
    MR::setJointTransformLocalMtx(this, ::cJointNamePropellerBack1, mScrew01Mtx);
    MR::connectToSceneCollisionEnemyStrongLight(this);
    initEffectKeeper(1, nullptr, false);
    initSound(8, false);
    initHitSensor(1);
    MR::initCollisionParts(this, "KoopaJrShip", MR::addBodyMessageSensorEnemy(this), nullptr);
    MR::invalidateClipping(this);
    initRailRider(rIter);
    MR::declarePowerStar(this);
    MR::addToAttributeGroupSearchTurtle(this);
    initShells();
    initKoopaJr(rIter);
    initKameck(rIter);
    initKiller();
    mShipBreakModel = MR::createModelObjMapObjStrongLight("クッパJrシップ壊れモデル", "KoopaJrShipBreak", mShipMtx);
    mShipBreakModel->makeActorDead();
    mPodModel = MR::createModelObjMapObjStrongLight("クッパJrシップポッドモデル", "KoopaJrShipPod", mPodMtx);
    mPodModel->makeActorDead();

    if (MR::tryRegisterDemoCast(this, rIter)) {
        MR::registerDemoActionNerve(this, &NrvKoopaJrShip::HostTypeAppear::sInstance, "出現");
        MR::registerDemoActionFunctor(this, MR::Functor_Inline(this, &KoopaJrShip::setStateTurnFront), "旋廻");
        MR::tryRegisterDemoCast(mShipBreakModel, rIter);
        MR::tryRegisterDemoCast(mPodModel, rIter);
        initNerve(&NrvKoopaJrShip::HostTypeAppear::sInstance);
        makeActorDead();
    } else {
        initNerve(&NrvKoopaJrShip::HostTypeMove::sInstance);
        makeActorAppeared();
    }
}

void KoopaJrShip::initAfterPlacement() {
    MR::moveCoordAndTransToNearestRailPos(this);
    _1F8.set(mPosition);
}

void KoopaJrShip::kill() {
    MR::requestAppearPowerStar(this, mPosition);
    LiveActor::kill();
}

void KoopaJrShip::control() {
    _EC = MR::repeat(_EC + mPropRotateSpeed, 0.0f, 360.0f);

    f32 angle = MR::toRadian(_EC);
    f32 s = sin(angle);
    f32 c = cos(angle);

    mPropellerMtx[0][0] = c;
    mPropellerMtx[1][1] = 1.0f;
    mPropellerMtx[0][2] = s;
    mPropellerMtx[2][0] = -s;
    mPropellerMtx[2][2] = c;
    mPropellerMtx[2][1] = 0.0f;
    mPropellerMtx[1][2] = 0.0f;
    mPropellerMtx[1][0] = 0.0f;
    mPropellerMtx[0][1] = 0.0f;

    mScrew00Mtx.setEulerZ(MR::toRadian(_EC));
    mScrew01Mtx.setEulerZ(MR::toRadian(_EC));
    MR::setRailCoordSpeed(this, _184);

    if (_D0 > 0) {
        if (_D0 <= 3) {
            MR::startLevelSound(this, "SE_BM_LV_KOOPAJR_SHIP_BURN1", -1, -1, -1);
        }
        if (_D0 <= 1) {
            MR::startLevelSound(this, "SE_BM_LV_KOOPAJR_SHIP_BURN2", -1, -1, -1);
        }
        MR::startLevelSound(this, "SE_BM_LV_KOOPAJR_SHIP_MOVE", -1, -1, -1);
    }

    updateKoopaJrPos();
    updateKillerPos();
}

bool KoopaJrShip::receiveMsgPlayerAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    if (MR::isMsgJetTurtleAttack(msg)) {
        if (receiveMsgJetTurtleAttack(pSender, pReceiver)) {
            return true;
        }
    }

    return false;
}

void KoopaJrShip::initShells() {
    mShellHolder = new CannonShellHolder(0x12);
    CannonShellUtil::registerKoopaJrShipCannonShell(mShellHolder, 0x12);
    mMainShellHolder = new CannonShellHolder(9);
    CannonShellUtil::registerKoopaJrShipCannonMainShell(mMainShellHolder, 9);
}

void KoopaJrShip::initKoopaJr(const JMapInfoIter& rIter) {
    mJr = new KoopaJr("クッパJr");
    TPos3f mtx;
    mtx.set(getBaseMtx());
    mtx.setTrans(mPosition);
    mtx.mult(_1EC, mJr->mPosition);
    mJr->init(rIter);
    mJr->kill();
}

void KoopaJrShip::initKameck(const JMapInfoIter& rIter) {
    for (s32 i = 0; i < 2; i++) {
        Kameck* kameck = (Kameck*)MR::createFireBallBeamKameck("カメック[クッパＪｒシップ]");
        MR::initChildObj(kameck, rIter, i);
        kameck->makeActorDead();
        mKamecks.push_back(kameck);
    }
}

void KoopaJrShip::initKiller() {
    for (HomingKiller** pActor = mKillers.begin(); pActor != mKillers.end(); pActor++) {
        *pActor = new HomingKiller("ホーミングキラー");
        (*pActor)->initWithoutIter();
        (*pActor)->setChaseStartEndDistance(5500.0f, 6500.0f);
    }
}

bool KoopaJrShip::receiveMsgJetTurtleAttack(HitSensor* pSender, HitSensor* pReceiver) {
    if (!MR::isSensorEnemy(pReceiver)) {
        return false;
    }

    if (isNerve(&NrvKoopaJrShip::HostTypeDamage::sInstance)) {
        return false;
    }

    if (isNerve(&NrvKoopaJrShip::HostTypePowerUp::sInstance)) {
        return false;
    }

    if (isStateBreak()) {
        return false;
    }

    _D4.set< f32 >(pSender->mPosition);
    _E0.set< f32 >(pSender->mHost->mVelocity);
    MR::normalizeOrZero(&_E0);

    if (isNextStateDamage(_D0)) {
        MR::startSound(this, "SE_BM_KOOPAJR_SHIP_DAMAGE_L");
        MR::startSound(mJr, "SE_BV_KOOPAJR_DAMAGE_L");
        MR::startSystemSE("SE_SY_VS_BOSS_LAST_HIT");
        setNerve(&NrvKoopaJrShip::HostTypeBreakStart::sInstance);
    } else {
        MR::startSound(this, "SE_BM_KOOPAJR_SHIP_DAMAGE");
        MR::startSound(mJr, "SE_BV_KOOPAJR_DAMAGE_S");
        MR::startSystemSE("SE_SY_VS_BOSS_DAMAGE_1");
        setNerve(&NrvKoopaJrShip::HostTypeDamage::sInstance);
    }

    return true;
}

s32 KoopaJrShip::getNumShootShells() const {
    if (isNerve(&NrvKoopaJrShip::HostTypeShoot1::sInstance)) {
        return 1;
    }

    return isNerve(&NrvKoopaJrShip::HostTypeShoot2::sInstance) != 0 ? 5 : 0;
}

void KoopaJrShip_float_ordering1() {
    (void)4.0f;
}

f32 KoopaJrShip::getPropellerRotSpeed() const {
    if (isNerve(&NrvKoopaJrShip::HostTypeBreakStart::sInstance)) {
        return (40.0f * (1.0f - MR::calcNerveRate(this, 60)));
    }

    if (isNerve(&NrvKoopaJrShip::HostTypeBreak::sInstance)) {
        return 0.0f;
    }

    if (isNerve(&NrvKoopaJrShip::HostTypePowerUp::sInstance) || 2 >= _D0 && !isNerve(&NrvKoopaJrShip::HostTypeDamage::sInstance)) {
        return 40.0f;
    }

    return 20.0f;
}

void KoopaJrShip::updateCoordSpeed() {
    switch (_D0) {
    case 2:
        0.0f;
    }
    f32 rate = 0.0f;
    MR::calcRailRateToNextPoint(&rate, this);

    if (0.95f < rate) {
        f32 v3;
        if (_D0 <= 2) {
            v3 = 2.0f;
        } else {
            v3 = 4.0f;
        }

        _184 = MR::getEaseInValue(rate, v3, 1.0f, 1.0f);
    } else {
        if (_188 >= 0) {
            f32 v5;
            if (_D0 <= 2) {
                v5 = 2.0f;
            } else {
                v5 = 4.0f;
            }

            f32 ease = MR::getEaseInValue(_188, v5, 0.0f, 60.0f);
            _184 = ease;
            _188 = _188 - 1;
            MR::startLevelSound(this, "SE_BM_LV_KOOPAJR_SHIP_ACCEL");
        }

        mPropRotateSpeed = getPropellerRotSpeed();
    }
}

void KoopaJrShip::calcLauncherInfo(TVec3f* a1, TVec3f* a2, s32 idx) const NO_INLINE {
    TPos3f mtx;
    mtx.set(MR::getJointMtx(this, ::cJointNameCannon[idx]));
    mtx.getTrans(*a1);
    mtx.getXDir(*a2);
    MR::normalize(a2);
}

void KoopaJrShip::calcLauncherInfoKiller(TVec3f* a1, TVec3f* a2, s32 idx) const {
    TPos3f mtx;
    TVec3f v16;
    TVec3f v15;

    mtx.set(MR::getJointMtx(this, ::cJointNameCannon[idx]));
    mtx.getTrans(*a1);
    mtx.getXDir(*a2);
    mtx.getYDir(v16);
    MR::normalize(&v16);

    mtx.getZDir(v15);
    MR::normalize(&v15);
    MR::rotateVecDegree(a2, v16, ::sKillerLauncherAngle[idx].y);
    MR::rotateVecDegree(a2, v15, ::sKillerLauncherAngle[idx].z);
    MR::normalize(a2);
    a1->add(*a2 * -100.0f);
}

void KoopaJrShip::shootShell(s32 idx) {
    CannonShellBase* shell = mShellHolder->getValidShell();

    if (shell != nullptr) {
        TVec3f v8, v7;
        calcLauncherInfo(&v8, &v7, idx);
        shell->launch(v8, v7 * 15.0f);
    }

    MR::emitEffect(this, ::cEffectNameShoot[idx]);
}

bool KoopaJrShip::tryShootAllKillers() {
    if (isExistActiveKiller()) {
        return false;
    }

    if (isExistActiveKameck()) {
        return false;
    }

    for (HomingKiller** pActor = mKillers.begin(); pActor != mKillers.end(); pActor++) {
        TVec3f v6, v5;
        s32 idx = mKillers.indexOf(pActor);
        calcLauncherInfoKiller(&v6, &v5, idx);
        (*pActor)->appear(v6, v5);
        MR::emitEffect(this, ::cEffectNameShoot[idx]);
    }

    return true;
}

void KoopaJrShip::shootKiller(s32 idx) {
    HomingKiller* killer = mKillers[idx];
    if (MR::isDead(killer)) {
        TVec3f v6, v5;
        calcLauncherInfoKiller(&v6, &v5, idx);
        killer->appear(v6, v5);
        MR::emitEffect(this, ::cEffectNameShoot[idx]);
    }
}

void KoopaJrShip::shootKillersAfterDamage() {
    if (_D0 <= 2) {
        tryShootAllKillers();
    } else {
        shootKiller(0);
        shootKiller(1);
        shootKiller(2);
    }
}

bool KoopaJrShip::isExistActiveKiller() const {
    return std::find_if(mKillers.begin(), mKillers.end(), std::not1(std::ptr_fun(&MR::isDead))) != mKillers.end();
}

bool KoopaJrShip::isExistActiveKameck() const {
    return std::find_if(mKamecks.begin(), mKamecks.end(), std::not1(std::ptr_fun(&MR::isDead))) != mKamecks.end();
}

void KoopaJrShip::shootMainShells() {
    TPos3f m;
    m.set(MR::getJointMtx(this, ::cJointNameCannonMain));

    TVec3f pos;
    m.getTrans(pos);

    // Straight ahead
    {
        TVec3f forward;
        m.getZDir(forward);
        MR::normalize(&forward);
        CannonShellBase* p = mMainShellHolder->getValidShell();
        p->launch(pos, forward * 23.0f);
    }

    MR::rotateMtxLocalYDegree(m, 15.0f);

    // Side
    {
        TVec3f forward;
        m.getZDir(forward);
        MR::normalize(&forward);
        CannonShellBase* p = mMainShellHolder->getValidShell();
        p->launch(pos, forward * 23.0f);
    }

    MR::rotateMtxLocalYDegree(m, -30.0f);

    // Other side
    {
        TVec3f forward;
        m.getZDir(forward);
        MR::normalize(&forward);
        CannonShellBase* p = mMainShellHolder->getValidShell();
        p->launch(pos, forward * 23.0f);
    }
}

void KoopaJrShip::emitDamageSmokeEffect() {
    if (_D0 == 3) {
        MR::emitEffect(this, "DamageSmoke1");
    } else if (_D0 == 1) {
        MR::emitEffect(this, "DamageSmoke2");
    }
}

void KoopaJrShip::emitDamageHitEffect() {
    TPos3f v7;
    v7.identity();
    TVec3f up;
    MR::calcUpVec(&up, this);
    MR::makeMtxSideUpPos(&v7, -_E0, up, _D4);
    MR::emitEffectHit(this, v7, "DamageFire");
    MR::startSound(this, "SE_BM_KOOPAJR_SHIP_IGNIT");
}

void KoopaJrShip::updateKoopaJrPos() {
    if (isNerve(&NrvKoopaJrShip::HostTypeBreak::sInstance)) {
        TPos3f v8;
        v8.set(MR::getJointMtx(mPodModel, ::cJointNamePodPos));
        v8.mult(_1EC, mJr->mPosition);
    } else {
        TPos3f v7;
        v7.set(MR::getJointMtx(this, ::cJointNameKoopaJrPos));
        MR::faceToPoint(v7, *MR::getPlayerPos(), 5.0f);
        v7.mult(_1EC, mJr->mPosition);
    }
}

void KoopaJrShip::updateKillerPos() {
    for (HomingKiller** pActor = mKillers.begin(); pActor != mKillers.end(); pActor++) {
        if (MR::isDead(*pActor)) {
            continue;
        }

        TVec3f v7;
        TVec3f v6;
        calcLauncherInfoKiller(&v7, &v6, mKillers.indexOf(pActor));
        (*pActor)->setBasePosAndFront(v7, v6);
    }
}

void KoopaJrShip::killAllSubModels() {
    mJr->kill();
    mPodModel->makeActorDead();
    mShipBreakModel->makeActorDead();
}

void KoopaJrShip::setStateTurnFront() {
    mPosition.set< f32 >(_1F8);
    MR::moveCoordAndTransToNearestRailPos(this);
    mJr->setStateShipBattlePowerUp();
    _1EC.x = ::sKoopaJrPosFront.x;
    _1EC.y = ::sKoopaJrPosFront.y;
    _1EC.z = ::sKoopaJrPosFront.z;
    setNerve(&NrvKoopaJrShip::HostTypeTurnFront::sInstance);
}

void KoopaJrShip::exeAppear() {
    if (MR::isFirstStep(this)) {
        MR::startSound(this, "SE_BM_KOOPAJR_SHIP_ENTER");
        MR::startBck(this, "Arrival", nullptr);
        MR::hideModel(this);
    }

    if (MR::isStep(this, 1)) {
        MR::showModel(this);
        mJr->appear();
        mJr->setStateShipBattleAppear();
    }

    if (MR::isDemoPartLastStep("クッパJr会話")) {
        mJr->endShipBattleTalk();
        _188 = 60;
        MR::startStageBGM("MBGM_BOSS_06_A", false);
        setNerve(&NrvKoopaJrShip::HostTypeMove::sInstance);
    }
}

void KoopaJrShip::exeMove() {
    if (MR::isFirstStep(this)) {
        f32 speed = _D0 <= 2 ? 2.0f : 4.0f;
        MR::setRailCoordSpeed(this, speed);
    }

    updateCoordSpeed();
    MR::moveCoordAndFollowTrans(this);

    if (MR::isGreaterStep(this, 0)) {
        if (!(getNerveStep() % 120) && !isExistActiveKiller()) {
            switch (_D0) {
            case 5:
                setNerve(&NrvKoopaJrShip::HostTypeShoot1::sInstance);
                break;
            case 4:
                setNerve(&NrvKoopaJrShip::HostTypeShoot2::sInstance);
                break;
            case 3:
                setNerve(&NrvKoopaJrShip::HostTypeShoot2::sInstance);
                break;
            case 2:
                setNerve(&NrvKoopaJrShip::HostTypeShoot2::sInstance);
                break;
            }
        }
    }

    if (MR::isRailReachedEdge(this)) {
        setNerve(&NrvKoopaJrShip::HostTypeStopAtEnd::sInstance);
    }
}

void KoopaJrShip::exeMoveFrontAttack() {
    if (MR::isFirstStep(this)) {
        f32 speed = _D0 <= 2 ? 2.0f : 4.0f;
        MR::setRailCoordSpeed(this, speed);
        MR::emitEffect(this, "HeadShootSign");
    }

    MR::startLevelSound(this, "SE_BM_LV_KOOPAJR_SHIP_MAIN_PREP");
    updateCoordSpeed();
    MR::moveCoordAndFollowTrans(this);

    if (MR::isRailReachedGoal(this)) {
        if (MR::isGreaterStep(this, 120)) {
            MR::reverseRailDirection(this);
            setNerve(&NrvKoopaJrShip::HostTypeShootMain::sInstance);
        }
    } else if (MR::isStep(this, 120)) {
        setNerve(&NrvKoopaJrShip::HostTypeShootMain::sInstance);
    }
}

void KoopaJrShip::exeShoot() {
    MR::moveCoordAndFollowTrans(this);
    updateCoordSpeed();

    for (s32 i = 0; i < getNumShootShells(); i++) {
        if (MR::isStep(this, 45 * i)) {
            shootShell(1);

            if (i == getNumShootShells() - 1) {
                setNerve(&NrvKoopaJrShip::HostTypeMove::sInstance);
                return;
            }
        }
    }

    if (MR::isRailReachedEdge(this)) {
        setNerve(&NrvKoopaJrShip::HostTypeStopAtEnd::sInstance);
    }
}

void KoopaJrShip::exeShootMain() {
    if (MR::isFirstStep(this)) {
        MR::emitEffect(this, "HeadShoot");
        MR::startSound(this, "SE_BM_KOOPAJR_SHIP_SHOOT_MAIN");
        MR::shakeCameraNormal();

        if (_D0 <= 1) {
            tryShootAllKillers();
        }
    }

    if (MR::isStep(this, 15)) {
        shootMainShells();
    }

    MR::moveCoordAndFollowTrans(this);
    updateCoordSpeed();

    if (MR::isStep(this, 150)) {
        setNerve(&NrvKoopaJrShip::HostTypeMoveFrontAttack::sInstance);
    }
}

void KoopaJrShip::exeStopAtEnd() {
    s32 step = (_D0 <= 2) ? 90 : 150;

    if (MR::isStep(this, step)) {
        MR::reverseRailDirection(this);
        _188 = 60;

        if (_D0 <= 2) {
            setNerve(&NrvKoopaJrShip::HostTypeMoveFrontAttack::sInstance);
        } else {
            setNerve(&NrvKoopaJrShip::HostTypeMove::sInstance);
        }
    }
}

void KoopaJrShip::exePowerUp() {
    if (MR::isStep(this, 90)) {
        _188 = 60;
        setNerve(&NrvKoopaJrShip::HostTypeMove::sInstance);
    }
}

void KoopaJrShip::exeDamage() {
    if (MR::isFirstStep(this)) {
        _184 = 0.0f;
        MR::startBck(this, "Damage", nullptr);
        mJr->setStateShipBattleShipDamage();
        emitDamageSmokeEffect();
        emitDamageHitEffect();
        MR::shakeCameraNormal();
        MR::tryRumbleDefaultHit(this, WPAD_CHAN0);
        MR::stopScene(12);
    }

    if (MR::isStep(this, 90)) {
        if (_D0 == 2) {
            MR::requestStartDemoRegistered(this, nullptr, nullptr, "旋廻");
            return;
        }

        shootKillersAfterDamage();
        if (_D0 == 3) {
            mKamecks.mArray[0]->appear();
        }

        if (_D0 == 1) {
            mKamecks.mArray[1]->appear();
        }

        if (_D0 == 2) {
            setNerve(&NrvKoopaJrShip::HostTypePowerUp::sInstance);
            return;
        }

        _188 = 60;

        // Needed to force another load and comparison
        if (get_D0() <= 2) {
            setNerve(&NrvKoopaJrShip::HostTypeMoveFrontAttack::sInstance);
        } else {
            setNerve(&NrvKoopaJrShip::HostTypeMove::sInstance);
        }
    }
}

void KoopaJrShip::exeBreakStart() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "Damage", nullptr);
        mJr->setStateShipBattleShipDamage();
        emitDamageHitEffect();
        MR::shakeCameraNormal();
        MR::tryRumbleDefaultHit(this, WPAD_CHAN0);
        MR::stopScene(12);
    }

    if (MR::isStep(this, 60)) {
        mShellHolder->killActiveShells();
        mMainShellHolder->killActiveShells();

        Kameck* const* pActor;

        for (pActor = mKamecks.begin(); pActor != mKamecks.end(); pActor++) {
            if (!MR::isDead(*pActor)) {
                (*pActor)->makeActorDeadForce();
            }
        }

        for (HomingKiller** pActor = mKillers.begin(); pActor != mKillers.end(); pActor++) {
            if (!MR::isDead(*pActor)) {
                (*pActor)->kill();
            }
        }

        MR::requestStartDemoRegistered(this, nullptr, nullptr, "破壊");
        mShipMtx.set(getBaseMtx());
        mShipBreakModel->appear();
        MR::startBck(mShipBreakModel, "Break", nullptr);
        mPodMtx.set(getBaseMtx());
        mPodModel->appear();
        MR::startBck(mPodModel, "Escape", nullptr);
        setNerve(&NrvKoopaJrShip::HostTypeBreak::sInstance);
    }
}

void KoopaJrShip::exeBreak() {
    if (MR::isFirstStep(this)) {
        MR::deleteEffect(this, "EyeLight");
        MR::tryRumblePadStrong(this, WPAD_CHAN0);
        MR::shakeCameraNormal();
        MR::startSound(this, "SE_BM_KOOPAJR_SHIP_BREAK_S");
        mJr->setStateShipBattleEscape();
        MR::hideModel(this);
        MR::deleteEffect(this, "DamageSmoke1");
        MR::deleteEffect(this, "DamageSmoke2");
    }

    if (!MR::isHiddenModel(mPodModel) && MR::isBckStopped(mPodModel)) {
        MR::hideModel(mPodModel);
        MR::hideModel(mJr);
    }

    if (MR::isStep(this, 287)) {
        MR::startSound(this, "SE_BM_KOOPAJR_SHIP_BREAK_S");
    }

    if (MR::isLessStep(this, 300)) {
        MR::startLevelSound(this, "SE_BM_LV_KOOPAJR_SHIP_BURN1");
        MR::startLevelSound(this, "SE_BM_LV_KOOPAJR_SHIP_BURN2");
    }

    if (MR::isStep(this, 300)) {
        MR::shakeCameraNormal();
        MR::tryRumblePadStrong(this, WPAD_CHAN0);
        MR::startSound(this, "SE_BM_KOOPAJR_SHIP_BREAK_L");
    }

    if (!MR::isHiddenModel(mShipBreakModel) && (MR::isBckStopped(mShipBreakModel) || (MR::isStep(this, 320)) != 0)) {
        mShipBreakModel->makeActorDead();
    }

    if (MR::isDemoLastStep()) {
        killAllSubModels();
        setNerve(&NrvKoopaJrShip::HostTypeBreakEnd::sInstance);
    }
}

void KoopaJrShip::exeTurnFront() {
    if (MR::isFirstStep(this)) {
        MR::stopStageBGM(30);
        mShellHolder->killActiveShells();
        mMainShellHolder->killActiveShells();

        if (!MR::isDead(mKamecks.mArray[0])) {
            _AC.push_back(mKamecks[0]);
        }

        mKamecks[0]->makeActorDeadForce();
        std::for_each(mKillers.begin(), mKillers.end(), std::mem_func(&HomingKiller::makeActorDead));
    }

    MR::startLevelSound(this, "SE_BM_LV_KOOPAJR_SHIP_3RD_DEMO");

    if (MR::isLessEqualStep(this, 150)) {
        mRotation.y += 0.60f;
    }

    if (MR::isStep(this, 150)) {
        MR::emitEffect(this, "EyeLight");
        MR::startSound(this, "SE_BM_KOOPAJR_SHIP_EYE_BLINK");
    }

    if (MR::isDemoPartLastStep("旋廻")) {
        _188 = 60;
        MR::startStageBGM("MBGM_BOSS_06_B", false);

        if (_AC[2] != nullptr) {
            mKamecks[0]->appear();
        }

        setNerve(&NrvKoopaJrShip::HostTypeMoveFrontAttack::sInstance);
    }
}

void KoopaJrShip::exeBreakEnd() {
    if (MR::isFirstStep(this)) {
        MR::startAfterBossBGM();
        kill();
    }
}
