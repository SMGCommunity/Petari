#include "Game/Enemy/KoopaJrShip.hpp"
#include "Game/Enemy/Kameck.hpp"
#include "Game/Enemy/KoopaJrShipCannonMainShell.hpp"
#include "Game/Enemy/KoopaJrShipCannonShell.hpp"
#include "Game/LiveActor/ModelObj.hpp"
#include "Game/NPC/KoopaJr.hpp"
#include "Game/NameObj/NameObjArchiveListCollector.hpp"
#include "Game/Util.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "JSystem/JGeometry/TVec.hpp"
#include "math_types.hpp"

namespace {
    static const char* cJointNamePropellerBack0 = "Screw00";
    static const char* cJointNamePropellerBack1 = "Screw01";
    static const char* cJointNamePropellerTop = "Propeller00";
    static const char* cJointNameCannonMain = "FirePoint3";
    static const char* cJointNameKoopaJrPos = "KoopaJrShip";
    static const char* cJointNamePodPos = "KoopaJrShipPod";

    static TVec3f sKillerLauncherAngle[6] = {TVec3f(0.0f, 0.0f, 0.0f), TVec3f(0.0f, 0.0f, 0.0f), TVec3f(0.0f, 0.0f, 0.0f),
                                             TVec3f(0.0f, 0.0f, 0.0f), TVec3f(0.0f, 0.0f, 0.0f), TVec3f(0.0f, 0.0f, 0.0f)};

    static TVec3f sKoopaJrPos = TVec3f(35.0f, 188.0f, 0.0f);
    static TVec3f sKoopaJrPosFront = TVec3f(0.0f, 188.0f, 135.0f);

    static const char* cJointNameCannon[7] = {"FirePoint0", "FirePoint1", "FirePoint2", "FirePoint3", "FirePoint4", "FirePoint5", "FirePoint6"};
    static const char* cEffectNameShoot[7] = {"ShootJ0", "ShootJ1", "ShootJ2", "ShootJ3", "ShootJ4", "ShootJ5", "ShootJ6"};
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

KoopaJrShip::KoopaJrShip(const char* pName) : LiveActor(pName) {
}

void KoopaJrShip::init(const JMapInfoIter& rIter) {
    MR::initDefaultPos(this, rIter);
    initModelManagerWithAnm("KoopaJrShip", nullptr, false);
    MR::startBck(this, "KoopaJrShip", nullptr);
    MR::setBckRate(this, 0.0f);
    MR::initJointTransform(this);
    MR::setJointTransformLocalMtx(this, cJointNamePropellerTop, mPropellerMtx);
    MR::setJointTransformLocalMtx(this, cJointNamePropellerBack0, mScrew00Mtx);
    MR::setJointTransformLocalMtx(this, cJointNamePropellerBack1, mScrew01Mtx);
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
    _1F8.set< f32 >(mPosition);
}

void KoopaJrShip::kill() {
    MR::requestAppearPowerStar(this, mPosition);
    LiveActor::kill();
}

// seems close? kinda?
void KoopaJrShip::control() {
    _EC = MR::repeat(_EC + mPropRotateSpeed, 0.0f, 360.0f);
    mPropellerMtx.setRotateInline(_EC, _EC);
    mScrew00Mtx.setEulerZ(PI_180 * _EC);
    mScrew01Mtx.setEulerZ(PI_180 * _EC);
    MR::setRailCoordSpeed(this, _184);

    switch (_D0) {
    case 0:
        MR::startLevelSound(this, "SE_BM_LV_KOOPAJR_SHIP_MOVE", -1, -1, -1);
        break;
    case 1:
        MR::startLevelSound(this, "SE_BM_LV_KOOPAJR_SHIP_BURN1", -1, -1, -1);
        break;
    case 2:
        MR::startLevelSound(this, "SE_BM_LV_KOOPAJR_SHIP_BURN2", -1, -1, -1);
        break;
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
    TMtx34f mtx;
    mtx.set(getBaseMtx());
    mtx.mMtx[0][3] = mPosition.x;
    mtx.mMtx[1][3] = mPosition.y;
    mtx.mMtx[2][3] = mPosition.z;
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

void KoopaJrShip::killAllSubModels() {
    mJr->kill();
    mPodModel->makeActorDead();
    mShipBreakModel->makeActorDead();
}

void KoopaJrShip::setStateTurnFront() {
    mPosition.set< f32 >(_1F8);
    MR::moveCoordAndTransToNearestRailPos(this);
    mJr->setStateShipBattlePowerUp();
    _1EC.x = sKoopaJrPosFront.x;
    _1EC.y = sKoopaJrPosFront.y;
    _1EC.z = sKoopaJrPosFront.z;
    setNerve(&NrvKoopaJrShip::HostTypeTurnFront::sInstance);
}

KoopaJrShip::~KoopaJrShip() {
}

void KoopaJrShip::exeBreakEnd() {
    if (MR::isFirstStep(this)) {
        MR::startAfterBossBGM();
        kill();
    }
}
