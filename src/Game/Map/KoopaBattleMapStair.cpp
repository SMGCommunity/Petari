#include "Game/Map/KoopaBattleMapStair.hpp"
#include "Game/Boss/KoopaFunction.hpp"
#include "Game/Util/ActorMovementUtil.hpp"
#include "Game/Util/ActorSensorUtil.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "JSystem/JMath/JMath.hpp"

namespace NrvKoopaBattleMapStair {
    NEW_NERVE(KoopaBattleMapStairNrvWaitSwitch, KoopaBattleMapStair, WaitSwitch);
    NEW_NERVE(KoopaBattleMapStairNrvWaitKoopaFire, KoopaBattleMapStair, WaitKoopaFire);
    NEW_NERVE(KoopaBattleMapStairNrvWaitFall, KoopaBattleMapStair, WaitFall);
    NEW_NERVE(KoopaBattleMapStairNrvFall, KoopaBattleMapStair, Fall);
    NEW_NERVE(KoopaBattleMapStairNrvDisappear, KoopaBattleMapStair, Disappear);
};  // namespace NrvKoopaBattleMapStair

struct OffsetPair {
    f32 _0;
    f32 _4;
};

KoopaBattleMapStair::KoopaBattleMapStair(const char* pName) : LiveActor(pName) {
    mFireTimer = 300;
    _90 = -1;
    _94 = 0;
    _98 = -1;
    _9C = 0;
    mType = 0;
    mIsStairBig = false;
    mIsStairTurn = false;
    _A6 = 0;
    _A8 = -1;
    _AC.x = 0.0f;
    _AC.y = 0.0f;
    _AC.z = 0.0f;
    _B8 = 60;
    _BC = 240;
}

void KoopaBattleMapStair::init(const JMapInfoIter& rIter) {
    MR::initDefaultPos(this, rIter);
    MR::getJMapInfoArg0NoInit(rIter, &mFireTimer);
    MR::getJMapInfoArg1NoInit(rIter, &_94);
    MR::getJMapInfoArg2NoInit(rIter, &mType);
    MR::getJMapInfoArg3NoInit(rIter, &_B8);
    MR::getJMapInfoArg4NoInit(rIter, &_BC);
    MR::getJMapInfoArg5NoInit(rIter, &_98);
    MR::getJMapInfoArg6NoInit(rIter, &_9C);
    const char* objName = nullptr;
    MR::getObjectName(&objName, rIter);
    initModelManagerWithAnm(objName, nullptr, false);
    if (MR::isEqualString(objName, "KoopaBattleMapStairBig")) {
        mIsStairBig = 1;
    } else if (MR::isEqualString(objName, "KoopaBattleMapStairTurn")) {
        mIsStairTurn = 1;
    }

    MR::connectToSceneMapObj(this);
    initHitSensor(1);
    MR::addBodyMessageSensorMapObj(this);
    MR::initCollisionParts(this, objName, getSensor("body"), nullptr);
    initEffectKeeper(0, nullptr, false);
    initSound(4, false);
    initNerve(&NrvKoopaBattleMapStair::KoopaBattleMapStairNrvWaitSwitch::sInstance);
    MR::needStageSwitchReadA(this, rIter);
    if (mIsStairBig) {
        MR::setClippingTypeSphere(this, 800.0f);
    } else {
        MR::setClippingTypeSphere(this, 500.0f);
    }

    makeActorAppeared();
}

void KoopaBattleMapStair::initAfterPlacement() {
    if (!isTypeNoRequestFire()) {
        _90 = KoopaFunction::registerBattleMapStair(this);
    }
}

bool KoopaBattleMapStair::isRequestAttackVs1() const {
    if (isTypeNormal() && isNerve(&NrvKoopaBattleMapStair::KoopaBattleMapStairNrvWaitKoopaFire::sInstance) && MR::isStep(this, _90)) {
        return true;
    }

    return false;
}

s32 KoopaBattleMapStair::calcRemainTimeToBreak() const {
    return mFireTimer - getNerveStep();
}

bool KoopaBattleMapStair::isRequestAttackVs3() const {
    if (!_A6 && isTypeNormal() && isNerve(&NrvKoopaBattleMapStair::KoopaBattleMapStairNrvWaitKoopaFire::sInstance)) {
        if (MR::isStep(this, _90) || _90 < 0) {
            return true;
        }
    }

    return false;
}

namespace {
    static OffsetPair offset_table[] = {{0.0f, -1.0f}, {1.0f, -1.0f}, {1.0f, 0.0f},  {1.0f, 1.0f},
                                        {0.0f, 1.0f},  {-1.0f, 1.0f}, {-1.0f, 0.0f}, {-1.0f, -1.0f}};

    void updateNearestPos(TVec3f* pPos, f32* pTable, const TVec3f& a3, const TVec3f& a4, s32 a5, s32 a6) {
        if (a5 < 0) {
            f32 dist = PSVECDistance(&a3, &a4);
            if (dist > *pTable) {
            } else {
                pPos->set(a3);
                *pTable = dist;
            }
        } else if (a5 == a6) {
            pPos->set(a3);
        }
    }
};  // namespace

f32 KoopaBattleMapStair::calcAndSetTargetPos(TVec3f* pPos, const TVec3f& a2) {
    TVec3f v41, v40, v39;
    MR::calcActorAxis(&v39, &v40, &v41, this);
    TVec3f v38 = (v40 * 100) + mPosition;
    f32 v20 = PSVECDistance(&v38, &a2);
    pPos->set< f32 >(v38);
    s32 val;
    if (mIsStairBig) {
        val = _98;
        for (s32 i = 0; i < 8; i++) {
            TVec3f v31 = (v38 + (v39 * 600.0f) * offset_table[i]._0) + (v41 * 400.0f * offset_table[i]._4);
            updateNearestPos(pPos, &v20, v31, a2, val, i);
        }
    } else if (mIsStairTurn) {
        TVec3f v30 = v39 * 140.0f;
        TVec3f tmp = v38 + (v41 * 300.0f);
        TVec3f v27(tmp);
        JMathInlineVEC::PSVECSubtract(&v27, &v30, &v27);
        updateNearestPos(pPos, &v20, v27, a2, -1, -1);
    } else {
        val = _98;
        for (s32 i = 0; i < 8; i++) {
            TVec3f v21 = (v38 + (v39 * 300.0f) * offset_table[i]._0) + (v41 * 200.0f * offset_table[i]._4);
            updateNearestPos(pPos, &v20, v21, a2, val, i);
        }
    }

    _AC.set< f32 >(*pPos);
    return v20;
}

f32 KoopaBattleMapStair::calcTimeRate() const {
    return (f32)(getNerveStep() - _90) / (mFireTimer - _90);
}

bool KoopaBattleMapStair::isBreak() const {
    return isNerve(&NrvKoopaBattleMapStair::KoopaBattleMapStairNrvWaitFall::sInstance);
}

bool KoopaBattleMapStair::isTypeNormal() const {
    return mType == 0;
}

bool KoopaBattleMapStair::isTypeDemoFar() const {
    return mType == 2;
}

bool KoopaBattleMapStair::isTypeDemoNear() const {
    return mType == 3;
}

bool KoopaBattleMapStair::isTypeNoRequestFire() const {
    return mType == 1;
}

void KoopaBattleMapStair::exeWaitSwitch() {
    if (MR::isFirstStep(this)) {
        MR::startAllAnim(this, "Wait");
    }

    if (MR::isOnSwitchA(this)) {
        MR::invalidateClipping(this);
        setNerve(&NrvKoopaBattleMapStair::KoopaBattleMapStairNrvWaitKoopaFire::sInstance);
    }
}

void KoopaBattleMapStair::exeWaitKoopaFire() {
    if (MR::isStep(this, mFireTimer)) {
        setNerve(&NrvKoopaBattleMapStair::KoopaBattleMapStairNrvWaitFall::sInstance);
    }
}

void KoopaBattleMapStair::exeWaitFall() {
    if (MR::isFirstStep(this)) {
        MR::startSound(this, "SE_OJ_STAIR_BREAK_START", -1, -1);
        MR::startAllAnim(this, "WaitFall");
    }

    MR::startLevelSound(this, "SE_OJ_LV_STAIR_BREAK", -1, -1, -1);
    if (MR::isStep(this, _B8)) {
        mVelocity.x = mVelocity.y = mVelocity.z = 0.0f;
        setNerve(&NrvKoopaBattleMapStair::KoopaBattleMapStairNrvFall::sInstance);
    }
}

void KoopaBattleMapStair::exeFall() {
    if (MR::isFirstStep(this)) {
        MR::onCalcGravity(this);
        MR::startAllAnim(this, "Fall");
    }

    MR::addVelocityToGravity(this, 0.03f);
    MR::restrictVelocity(this, 1.5f);
    MR::startLevelSound(this, "SE_OJ_LV_STAIR_BREAK", -1, -1, -1);
    if (MR::isStep(this, _BC)) {
        MR::startSound(this, "SE_OJ_STAIR_BREAK_END", -1, -1);
        setNerve(&NrvKoopaBattleMapStair::KoopaBattleMapStairNrvDisappear::sInstance);
    }
}

void KoopaBattleMapStair::exeDisappear() {
    if (MR::isFirstStep(this)) {
        MR::startAllAnim(this, "Disappear");
        MR::setBrkRate(this, 0.25f);
    }

    MR::addVelocityToGravity(this, 0.03f);
    MR::restrictVelocity(this, 1.5f);
    if (MR::isBrkStopped(this)) {
        kill();
    }
}
