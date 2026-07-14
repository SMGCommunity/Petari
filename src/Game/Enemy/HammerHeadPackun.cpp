#include "Game/Enemy/HammerHeadPackun.hpp"
#include "Game/LiveActor/EffectKeeper.hpp"
#include "Game/LiveActor/HitSensor.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/LiveActor/PartsModel.hpp"
#include "Game/Util/ActorMovementUtil.hpp"
#include "Game/Util/ActorSensorUtil.hpp"
#include "Game/Util/ActorShadowUtil.hpp"
#include "Game/Util/ActorSwitchUtil.hpp"
#include "Game/Util/EffectUtil.hpp"
#include "Game/Util/JMapInfo.hpp"
#include "Game/Util/JMapUtil.hpp"
#include "Game/Util/JointRumbler.hpp"
#include "Game/Util/JointUtil.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/MathUtil.hpp"
#include "Game/Util/ObjUtil.hpp"
#include "Game/Util/PlayerUtil.hpp"
#include "Game/Util/SoundUtil.hpp"
#include "Game/Util/StarPointerUtil.hpp"
#include "JSystem/JGeometry/TMatrix.hpp"
#include "JSystem/JGeometry/TVec.hpp"
#include "JSystem/JMath/JMATrigonometric.hpp"
#include "JSystem/JMath/JMath.hpp"
#include "revolution/mtx.h"
#include "revolution/types.h"

void HammerHeadPackun_FORCE_MATCH_SDATA2() {
    (void)1.0f;
    (void)0.0f;
    (void)3.1415927f;
}

void DUMMY() {
    // to emit operator-
    TVec3f a, b;
    TVec3f c = a - b;
}

HammerHeadPackun::HammerHeadPackun(const char* pName) : LiveActor(pName) {
    mLeafModel = nullptr;
    mImmunityTime = 0;
    _94.x = 0.0f;
    _94.y = 1.0f;
    _94.z = 0.0f;
    _A0.x = 0.0f;
    _A0.y = 0.0f;
    _A0.z = 1.0f;
    _AC.x = 0.0f;
    _AC.y = 0.0f;
    _AC.z = 1.0f;
    mJointRumbler = nullptr;
    mNumbTime = 0;
}

namespace {
    NEW_NERVE(HammerHeadPackunNrvWait, HammerHeadPackun, Wait);
    NEW_NERVE(HammerHeadPackunNrvSearch, HammerHeadPackun, Search);
    NEW_NERVE(HammerHeadPackunNrvAttack, HammerHeadPackun, Attack);
    NEW_NERVE(HammerHeadPackunNrvRest, HammerHeadPackun, Rest);
    NEW_NERVE(HammerHeadPackunNrvRecover, HammerHeadPackun, Recover);
    NEW_NERVE(HammerHeadPackunNrvDamage, HammerHeadPackun, Damage);
    NEW_NERVE(HammerHeadPackunNrvRise, HammerHeadPackun, Rise);
    NEW_NERVE(HammerHeadPackunNrvHit, HammerHeadPackun, Hit);
    NEW_NERVE(HammerHeadPackunNrvCrow, HammerHeadPackun, Crow);
    NEW_NERVE(HammerHeadPackunNrvSmashKill, HammerHeadPackun, SmashKill);
    NEW_NERVE(HammerHeadPackunNrvBlowKill, HammerHeadPackun, BlowKill);
    NEW_NERVE_ONEND(HammerHeadPackunNrvNumb, HammerHeadPackun, Numb, Numb);
};  // namespace

HammerHeadPackun::~HammerHeadPackun() {
}

void HammerHeadPackun::init(const JMapInfoIter& rIter) {
    MR::initDefaultPos(this, rIter);
    initPosture();
    initModelManagerWithAnm("PackunFlower", nullptr, false);
    MR::connectToSceneEnemy(this);
    MR::initLightCtrl(this);
    initSensor();
    initEffectKeeper(1, nullptr, false);
    MR::addEffect(this, "PointerTouchManual");
    MR::setEffectHostMtx(this, "PointerTouchManual", MR::getJointMtx(this, "MouthUp"));
    initSound(4, false);
    initNerve(&::HammerHeadPackunNrvWait::sInstance);
    MR::useStageSwitchWriteDead(this, rIter);
    initShadow();
    initLeaf(rIter);
    MR::initStarPointerTargetAtJoint(this, "Head", 169.0f, TVec3f(120.0f, 0.0f, 0.0f));
    MR::declareStarPiece(this, 9);
    mJointRumbler = new JointRumbler(this, "AllRoot", 8.0f, 0.06f, 60, true, 0);
    makeActorAppeared();
}

void HammerHeadPackun::makeActorAppeared() {
    LiveActor::makeActorAppeared();
    setNerve(&::HammerHeadPackunNrvWait::sInstance);
}

void HammerHeadPackun::kill() {
    if (MR::isValidSwitchDead(this)) {
        MR::onSwitchDead(this);
    }

    MR::startSound(this, "SE_EM_HHPACKUN_DEAD");
    TVec3f pos;
    MR::copyJointPos(this, "Head", &pos);
    MR::appearStarPiece(this, pos, 9, 10.0f, 40.0f, false);
    MR::startSound(this, "SE_OJ_STAR_PIECE_BURST");
    LiveActor::kill();
}

void HammerHeadPackun::control() {
    mJointRumbler->update();
}

void HammerHeadPackun::exeWait() {
    if (MR::isFirstStep(this)) {
        const char* wait = "Wait";
        MR::startBck(this, wait, nullptr);
        MR::startBck(mLeafModel, wait, nullptr);
    }

    if (!tryShiftNumb() && isTargetInRange()) {
        setNerve(&::HammerHeadPackunNrvSearch::sInstance);
    }
}

void HammerHeadPackun::exeSearch() {
    TVec3f dir;
    if (calcPlayerDir(&dir) && !tryShiftNumb()) {
        turnTo(dir, 1.0f);
        if (isInFieldOfView(dir)) {
            setNerve(&::HammerHeadPackunNrvAttack::sInstance);
        }
    }
}

void HammerHeadPackun::exeAttack() {
    if (MR::isFirstStep(this)) {
        const char* attack = "Attack";
        MR::startBck(this, attack, nullptr);
        MR::startBck(mLeafModel, attack, nullptr);
    }

    TVec3f dir;
    if (!tryShiftNumb()) {
        if (MR::isStep(this, 156)) {
            MR::shakeCameraNormalStrong();
            MR::tryRumblePadStrong(this, 0);
        }

        if (MR::isLessEqualStep(this, 60)) {
            if (calcPlayerDir(&dir)) {
                turnTo(dir, 1.0f);
            }
        } else {
            turnToAttackVec(60);
        }
        MR::setNerveAtBckStopped(this, &::HammerHeadPackunNrvRest::sInstance);
    }
}

void HammerHeadPackun::exeRest() {
    if (MR::isFirstStep(this)) {
        const char* chance = "Chance";
        MR::startBck(this, chance, nullptr);
        MR::startBck(mLeafModel, chance, nullptr);
        mImmunityTime = 0;
    }

    if (!tryShiftNumb()) {
        MR::setNerveAtStep(this, &::HammerHeadPackunNrvRecover::sInstance, 90);
    }
}

void HammerHeadPackun::exeRecover() {
    if (MR::isFirstStep(this)) {
        const char* recovery = "Recovery";
        MR::startBck(this, recovery, nullptr);
        MR::startBck(mLeafModel, recovery, nullptr);
    }

    if (!tryShiftNumb()) {
        MR::startLevelSound(this, "SE_EM_LV_HHPACKUN_RECOVERY");
        MR::setNerveAtBckStopped(this, &::HammerHeadPackunNrvWait::sInstance);
    }
}

void HammerHeadPackun::exeDamage() {
    if (MR::isFirstStep(this)) {
        const char* swoonStart = "SwoonStart";
        MR::startBck(this, swoonStart, nullptr);
        MR::startBck(mLeafModel, swoonStart, nullptr);
        MR::startBlowHitSound(this);
        mImmunityTime++;
    }

    if (MR::isStep(this, 76)) {
        MR::shakeCameraNormal();
        MR::tryRumblePadMiddle(this, 0);
    }

    if (MR::isBckOneTimeAndStopped(this)) {
        const char* swoon = "Swoon";
        MR::startBck(this, swoon, nullptr);
        MR::startBck(mLeafModel, swoon, nullptr);
    }

    if (MR::isGreaterEqualStep(this, 60)) {
        MR::startLevelSound(this, "SE_EM_LV_SWOON_S");
    }
    MR::setNerveAtStep(this, &::HammerHeadPackunNrvRise::sInstance, 180);
}

void HammerHeadPackun::exeRise() {
    if (MR::isFirstStep(this)) {
        const char* swoonEnd = "SwoonEnd";
        MR::startBck(this, swoonEnd, nullptr);
        MR::startBck(mLeafModel, swoonEnd, nullptr);
    }

    if (!tryShiftNumb()) {
        if (MR::isStep(this, 56)) {
            MR::shakeCameraNormalWeak();
            MR::tryRumblePadWeak(this, false);
        }

        if (MR::isBckStopped(this)) {
            if (isTargetInRange()) {
                const char* wait = "Wait";
                MR::startBck(this, wait, nullptr);
                MR::startBck(mLeafModel, wait, nullptr);
                setNerve(&::HammerHeadPackunNrvSearch::sInstance);
            } else {
                setNerve(&::HammerHeadPackunNrvWait::sInstance);
            }
        }
    }
}

inline void HammerHeadPackun::exeHit() {
    if (!tryShiftNumb()) {
        MR::setNerveAtBckStopped(this, &::HammerHeadPackunNrvCrow::sInstance);
    }
}

void HammerHeadPackun::exeCrow() {
    if (MR::isFirstStep(this)) {
        const char* crow = "Crow";
        MR::startBck(this, crow, nullptr);
        MR::startBck(mLeafModel, crow, nullptr);
    }

    if (!tryShiftNumb()) {
        if (MR::isStep(this, 45)) {
            MR::shakeCameraNormalWeak();
            MR::tryRumblePadWeak(this, 0);
        }
        MR::setNerveAtBckStopped(this, &::HammerHeadPackunNrvWait::sInstance);
    }
}

void HammerHeadPackun::exeSmashKill() {
    if (MR::isFirstStep(this)) {
        const char* down = "Down";
        MR::startBck(this, down, nullptr);
        MR::startBck(mLeafModel, down, nullptr);
        MR::startSound(this, "SE_EM_HHPACKUN_STOMPED");
        MR::startSound(this, "SE_EM_STOMPED_S");
    }

    if (MR::isBckStopped(this)) {
        kill();
    }
}

void HammerHeadPackun::exeBlowKill() {
    if (MR::isFirstStep(this)) {
        const char* blowDown = "BlowDown";
        MR::startBck(this, blowDown, nullptr);
        MR::startBck(mLeafModel, blowDown, nullptr);
        MR::tryRumblePadMiddle(this, 0);
        MR::startSound(this, "SE_EM_HHPACKUN_HIT_PUNCH");
        MR::startSound(this, "SE_EV_HHPACKUN_DEAD");
    }

    if (MR::isBckOneTimeAndStopped(this)) {
        kill();
    }
}

void HammerHeadPackun::exeNumb() {
    if (MR::isFirstStep(this)) {
        MR::startDPDHitSound();
        mEffectKeeper->forceDeleteEmitterAll();
        MR::stopBck(this);
        MR::stopBck(mLeafModel);
        MR::emitEffectWithScale(this, "PointerTouchManual", 1.5f, -1);
        mNumbTime = 0;
    }

    MR::startDPDFreezeLevelSound(this);
    if (!(getNerveStep() % 30)) {
        mJointRumbler->start();
    }

    if (MR::isStarPointerPointing2POnPressButton(this, "弱", true, false)) {
        mNumbTime = 0;
    }

    mNumbTime++;
    if (mNumbTime >= 20) {
        MR::deleteEffect(this, "PointerTouchManual");
        setNerve(&::HammerHeadPackunNrvWait::sInstance);
    }
}

void HammerHeadPackun::endNumb() {
    MR::deleteEffect(this, "PointerTouchManual");
}

void HammerHeadPackun::calcAndSetBaseMtx() {
    TVec3f vec;
    TPos3f mtx;
    vec.cross(_94, _A0);
    mtx[0][0] = vec.x;
    mtx[1][0] = vec.y;
    mtx[2][0] = vec.z;
    mtx[0][1] = _94.x;
    mtx[1][1] = _94.y;
    mtx[2][1] = _94.z;
    mtx[0][2] = _A0.x;
    mtx[1][2] = _A0.y;
    mtx[2][2] = _A0.z;
    mtx[0][3] = mPosition.x;
    mtx[1][3] = mPosition.y;
    mtx[2][3] = mPosition.z;
    MR::setBaseTRMtx(this, mtx);
}

inline bool HammerHeadPackun::checkBlowAttack() const {
    return isNerve(&::HammerHeadPackunNrvSmashKill::sInstance) || isNerve(&::HammerHeadPackunNrvBlowKill::sInstance);
}

void HammerHeadPackun::attackSensor(HitSensor* pSender, HitSensor* pReceiver) {
    if (!checkBlowAttack() && MR::isSensor(pSender, "blow") && MR::getSensorHost(pReceiver) &&
        !strcmp(MR::getSensorHost(pReceiver)->mName, "パンチングキノコ")) {
        f32 recvRadius = pReceiver->mRadius;
        f32 sendRadius = pSender->mRadius;
        f32 temp = recvRadius / (sendRadius + recvRadius);
        TVec3f hitMtx = pSender->mPosition * temp + (pReceiver->mPosition) * (1.0f - temp);
        MR::tryRumblePadMiddle(this, 0);
        MR::emitEffectHit(this, hitMtx, nullptr);
        MR::stopScene(8);
        setNerve(&::HammerHeadPackunNrvBlowKill::sInstance);
        MR::sendMsgPush(pReceiver, pSender);
        return;
    }

    if (MR::isSensorPlayerOrRide(pReceiver)) {
        bool isSensorBody = getSensor("body") != pSender;
        if (isSensorBody) {
            if (!MR::sendMsgEnemyAttack(pReceiver, pSender)) {
                MR::sendMsgPush(pReceiver, pSender);
            }
        } else if (isAttackSensorValid() && MR::sendMsgEnemyAttackStrong(pReceiver, pSender)) {
            MR::shakeCameraNormal();
            MR::tryRumblePadMiddle(this, 0);
            if (isNerve(&::HammerHeadPackunNrvAttack::sInstance) && !MR::isNewNerve(this)) {
                setNerve(&::HammerHeadPackunNrvHit::sInstance);
            } else {
                mJointRumbler->start();
            }
        } else {
            MR::sendMsgPush(pReceiver, pSender);
        }
    }
}

bool HammerHeadPackun::receiveMsgPlayerAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    MR::isSensor(pReceiver, "body");
    MR::isSensor(pReceiver, "blow");
    MR::isSensor(pReceiver, "push1");
    MR::isSensor(pReceiver, "push2");
    MR::isSensor(pReceiver, "push3");
    MR::isSensor(pReceiver, "push4");

    if (!checkBlowAttack() && (MR::isMsgJetTurtleAttack(msg) || MR::isMsgFireBallAttack(msg))) {
        f32 sendRadius = pReceiver->mRadius;
        f32 recvRadius = pSender->mRadius;
        f32 temp = recvRadius / (recvRadius + sendRadius);
        TVec3f hitMtx = pReceiver->mPosition * temp + (pSender->mPosition) * (1.0f - temp);
        MR::tryRumblePadMiddle(this, 0);
        MR::emitEffectHit(this, hitMtx, nullptr);
        MR::stopScene(8);
        setNerve(&::HammerHeadPackunNrvBlowKill::sInstance);
        return true;
    }

    if (MR::isMsgStarPieceReflect(msg)) {
        mJointRumbler->start();
        MR::startSound(this, "SE_EM_STARPIECE_REFRECT_PLIABLE");
        return true;
    }

    if (pReceiver != getSensor("body")) {
        return false;
    }

    if (isChance()) {
        return receiveMsgPlayerAttackChance(msg, pSender, pReceiver);
    }

    if (checkBlowAttack()) {
        return MR::isMsgPlayerTrample(msg);
    } else {
        return false;
    }
}

bool HammerHeadPackun::receiveMsgEnemyAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    if (MR::isMsgToEnemyAttackBlow(msg) && !checkBlowAttack()) {
        f32 sendRadius = pReceiver->mRadius;
        f32 recvRadius = pSender->mRadius;
        f32 temp = recvRadius / (recvRadius + sendRadius);
        TVec3f hitMtx = pReceiver->mPosition * temp + (pSender->mPosition) * (1.0f - temp);
        MR::tryRumblePadMiddle(this, 0);
        MR::emitEffectHit(this, hitMtx, nullptr);
        MR::stopScene(8);
        setNerve(&::HammerHeadPackunNrvBlowKill::sInstance);
        return true;
    }
    return false;
}

void HammerHeadPackun::initPosture() {
    TPos3f mtx;
    MR::makeMtxTR(mtx, this);

    f32 z1 = mtx[2][1];
    f32 y1 = mtx[1][1];
    f32 x1 = mtx[0][1];
    _94.set< f32 >(x1, y1, z1);

    f32 z2 = mtx[2][2];
    f32 y2 = mtx[1][2];
    f32 x2 = mtx[0][2];
    _A0.set< f32 >(x2, y2, z2);
}

void HammerHeadPackun::initSensor() {
    initHitSensor(6);
    MR::addHitSensorAtJointMapObj(this, "body", "Head", 32, 130.0f, TVec3f(120.0f, 0.0f, 0.0f));
    MR::addHitSensorAtJointMapObj(this, "blow", "Spin1", 32, 120.0f, TVec3f(0.0f, 0.0f, 0.0f));
    MR::addHitSensorAtJointMapObj(this, "push1", "Spin2", 8, 45.0f, TVec3f(0.0f, 0.0f, 0.0f));
    MR::addHitSensorAtJointMapObj(this, "push2", "Spin2", 8, 45.0f, TVec3f(50.0f, 0.0f, 0.0f));
    MR::addHitSensorAtJointMapObj(this, "push3", "Spin3", 8, 45.0f, TVec3f(0.0f, 0.0f, 0.0f));
    MR::addHitSensorAtJointMapObj(this, "push4", "Spin3", 8, 60.0f, TVec3f(80.0f, 0.0f, 0.0f));
}

void HammerHeadPackun::initShadow() {
    MR::initShadowFromCSV(this, "Shadow");
    mGravity.set(-_94);
}

inline s32 JMapInfo_getEntries(const JMapInfo* jmap) {
    return jmap->mData ? jmap->mData->mNumEntries : nullptr;
}

inline bool JMapInfo_inRange(const JMapInfoIter& rIter) {
    return rIter.mInfo && rIter.mIndex >= 0 && rIter.mIndex < JMapInfo_getEntries(rIter.mInfo);
}

void HammerHeadPackun::initLeaf(const JMapInfoIter& rIter) {
    mLeafModel = new PartsModel(this, "パックンの葉", "PackunLeaf", nullptr, 18, false);
    MR::initLightCtrl(mLeafModel);
    s32 arg = -1;
    if (JMapInfo_inRange(rIter)) {
        MR::getJMapInfoArg0NoInit(rIter, &arg);
    }

    if (arg > 0) {
        mLeafModel->initFixedPosition(TVec3f(100.0f, 0.0f, 0.0f), TVec3f(0.0f, -90.0f, -90.0f), "Spin1");
    } else {
        mLeafModel->initFixedPosition(TVec3f(0.0f, 0.0f, 0.0f), TVec3f(0.0f, -90.0f, -90.0f), "Spin1");
    }

    mLeafModel->initWithoutIter();
}

bool HammerHeadPackun::isTargetInRange() const {
    TVec3f vec;
    MR::getPlayerUpVec(&vec);
    MtxPtr baseMtx1 = getBaseMtx();
    MtxPtr baseMtx2 = getBaseMtx();
    MtxPtr baseMtx3 = getBaseMtx();
    TVec3f a2(baseMtx3[0][1], baseMtx2[1][1], baseMtx1[2][1]);

    if (vec.dot(a2) <= 0.0f) {
        return false;
    } else if (MR::calcDistanceToPlayer(mPosition) < 800.0f) {
        return true;
    }

    return false;
}

bool HammerHeadPackun::isInFieldOfView(const TVec3f& rPos) const {
    f32 deg = 30.0f;
    if (30.0f < 0.0f) {
        deg = -30.0f;
    }

    return rPos.dot(_A0) >= MR::cosDegree(deg);
}

bool HammerHeadPackun::isAttackSensorValid() const {
    bool isValid = false;
    if ((!isNerve(&::HammerHeadPackunNrvAttack::sInstance) || !MR::isNewNerve(this)) && !isNerve(&::HammerHeadPackunNrvRest::sInstance) &&
        (!isNerve(&::HammerHeadPackunNrvRecover::sInstance) || !MR::isLessEqualStep(this, 40)) && !isNerve(&::HammerHeadPackunNrvDamage::sInstance) &&
        (!isNerve(&::HammerHeadPackunNrvRise::sInstance) || !MR::isLessStep(this, 60)) && !isNerve(&::HammerHeadPackunNrvNumb::sInstance) &&
        !isNerve(&::HammerHeadPackunNrvSmashKill::sInstance) && !isNerve(&::HammerHeadPackunNrvBlowKill::sInstance)) {
        isValid = true;
    }
    return isValid;
}

bool HammerHeadPackun::isChance() const {
    bool isChance = false;
    if (isNerve(&::HammerHeadPackunNrvRest::sInstance) && !MR::isNewNerve(this) || isNerve(&::HammerHeadPackunNrvDamage::sInstance) ||
        isNerve(&::HammerHeadPackunNrvRecover::sInstance) && MR::isLessEqualStep(this, 50) ||
        isNerve(&::HammerHeadPackunNrvRise::sInstance) && MR::isLessEqualStep(this, 30) || isNerve(&::HammerHeadPackunNrvNumb::sInstance)) {
        isChance = true;
    }
    return isChance;
}

bool HammerHeadPackun::calcPlayerDir(TVec3f* pPos) const {
    // https://decomp.me/scratch/yaccF
    // it's (*MR::getPlayerPos() - mPosition), but it gets inlined
    pPos->set(*MR::getPlayerPos() - mPosition);
    f32 dot = _94.dot(*pPos);
    JMAVECScaleAdd(_94, pPos, pPos, -dot);
    if (MR::isNearZero(*pPos, 0.001f)) {
        return false;
    }

    MR::normalize(pPos);
    return true;
}

void HammerHeadPackun::turnTo(const TVec3f& rFrom, f32 rate) {
    f32 absAngle = _A0.angle(rFrom);
    f32 ratio = 1.0f;
    f32 maxAngle = rate * MR::pi() / 180.0f;
    if (absAngle > maxAngle) {
        ratio = maxAngle / absAngle;
    }
    TQuat4f quat;
    quat.setRotate(_A0, rFrom, ratio);
    quat.transform(_A0);
    verticalizeFrontVec();
}

void HammerHeadPackun::turnToAttackVec(s32 rate) {
    TQuat4f mtx;
    if (getNerveStep() <= rate) {
        f32 step = (f32)getNerveStep() / rate;
        if (step > 1.0f) {
            step = 1.0f;
        }

        mtx.setRotate(_A0, _AC, (step * step));
        mtx.transform(_A0);
        verticalizeFrontVec();
    }
}

void HammerHeadPackun::verticalizeFrontVec() {
    f32 dot = _94.dot(_A0);
    JMAVECScaleAdd(&_94, &_A0, &_A0, -dot);
    MR::normalize(&_A0);
}

bool HammerHeadPackun::tryShiftNumb() {
    if (MR::isStarPointerPointing2POnPressButton(this, "弱", true, false)) {
        setNerve(&::HammerHeadPackunNrvNumb::sInstance);
        return true;
    }
    return false;
}

bool HammerHeadPackun::receiveMsgPlayerAttackChance(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    if (MR::isMsgPlayerHipDrop(msg)) {
        MR::tryRumblePadMiddle(this, 0);
        setNerve(&::HammerHeadPackunNrvSmashKill::sInstance);
        MR::sendMsgAwayJump(pSender, pReceiver);
        return true;
    } else if (MR::isMsgPlayerTrample(msg)) {
        MR::tryRumblePadMiddle(this, 0);
        setNerve(&::HammerHeadPackunNrvSmashKill::sInstance);
        return true;
    } else if (MR::isMsgPlayerSpinAttack(msg)) {
        if (mImmunityTime >= 3u) {
            setNerve(&::HammerHeadPackunNrvRise::sInstance);
        } else {
            setNerve(&::HammerHeadPackunNrvDamage::sInstance);
        }
        return true;
    }
    return false;
}
