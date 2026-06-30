#include "Game/Map/KoopaBattleMapStair.hpp"
#include "Game/Boss/KoopaFunction.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/Util/ActorMovementUtil.hpp"
#include "Game/Util/ActorSensorUtil.hpp"
#include "Game/Util/ActorSwitchUtil.hpp"
#include "Game/Util/JMapUtil.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/ObjUtil.hpp"
#include "Game/Util/SoundUtil.hpp"
#include "Game/Util/StringUtil.hpp"

struct OffsetPair {
    /* 0x00 */ f32 x;
    /* 0x04 */ f32 z;
};

namespace {
    static const s32 sDefaultTimeToBreak = 300;
    static const f32 sNormalHalfWidthX = 300.0f;
    static const f32 sNormalHalfWidthZ = 200.0f;
    static const f32 sHalfHeight = 100.0f;
    static const f32 sBigHalfWidthX = 600.0f;
    static const f32 sBigHalfWidthZ = 400.0f;
    static const f32 sTurnHalfWidthX = 140.0f;
    static const f32 sTurnHalfWidthZ = 300.0f;
    static const s32 sDefaultStepWaitFall = 60;
    static const s32 sDefaultStepFall = 240;
    static const f32 sFallGravity = 0.03f;
    static const f32 sFallSpeedMax = 1.5f;
    // static const f32 sDebugRadius = _;
    static const OffsetPair offset_table[] = {{0.0f, -1.0f}, {1.0f, -1.0f}, {1.0f, 0.0f},  {1.0f, 1.0f},
                                              {0.0f, 1.0f},  {-1.0f, 1.0f}, {-1.0f, 0.0f}, {-1.0f, -1.0f}};
};  // namespace

namespace NrvKoopaBattleMapStair {
    NEW_NERVE(KoopaBattleMapStairNrvWaitSwitch, KoopaBattleMapStair, WaitSwitch);
    NEW_NERVE(KoopaBattleMapStairNrvWaitKoopaFire, KoopaBattleMapStair, WaitKoopaFire);
    NEW_NERVE(KoopaBattleMapStairNrvWaitFall, KoopaBattleMapStair, WaitFall);
    NEW_NERVE(KoopaBattleMapStairNrvFall, KoopaBattleMapStair, Fall);
    NEW_NERVE(KoopaBattleMapStairNrvDisappear, KoopaBattleMapStair, Disappear);
};  // namespace NrvKoopaBattleMapStair

KoopaBattleMapStair::KoopaBattleMapStair(const char* pName)
    : LiveActor(pName), mTimeToBreak(::sDefaultTimeToBreak), mFireAttackStep(-1), mArg1(), mArg5(-1), mArg6(), mType(), mIsBig(), mIsTurn(), _A6(),
      _A8(-1), _AC(0.0f, 0.0f, 0.0f), mWaitFallStep(::sDefaultStepWaitFall), mFallStep(::sDefaultStepFall) {
}

void KoopaBattleMapStair::init(const JMapInfoIter& rIter) {
    MR::initDefaultPos(this, rIter);
    MR::getJMapInfoArg0NoInit(rIter, &mTimeToBreak);
    MR::getJMapInfoArg1NoInit(rIter, &mArg1);
    MR::getJMapInfoArg2NoInit(rIter, &mType);
    MR::getJMapInfoArg3NoInit(rIter, &mWaitFallStep);
    MR::getJMapInfoArg4NoInit(rIter, &mFallStep);
    MR::getJMapInfoArg5NoInit(rIter, &mArg5);
    MR::getJMapInfoArg6NoInit(rIter, &mArg6);

    const char* objName = nullptr;
    MR::getObjectName(&objName, rIter);

    initModelManagerWithAnm(objName, nullptr, false);

    if (MR::isEqualString(objName, "KoopaBattleMapStairBig")) {
        mIsBig = true;
    } else if (MR::isEqualString(objName, "KoopaBattleMapStairTurn")) {
        mIsTurn = true;
    }

    MR::connectToSceneMapObj(this);
    initHitSensor(1);
    MR::addBodyMessageSensorMapObj(this);
    MR::initCollisionParts(this, objName, getSensor("body"), nullptr);
    initEffectKeeper(0, nullptr, false);
    initSound(4, false);
    initNerve(&NrvKoopaBattleMapStair::KoopaBattleMapStairNrvWaitSwitch::sInstance);
    MR::needStageSwitchReadA(this, rIter);

    if (mIsBig) {
        MR::setClippingTypeSphere(this, 800.0f);
    } else {
        MR::setClippingTypeSphere(this, 500.0f);
    }

    makeActorAppeared();
}

void KoopaBattleMapStair::initAfterPlacement() {
    if (!isTypeNoRequestFire()) {
        mFireAttackStep = KoopaFunction::registerBattleMapStair(this);
    }
}

bool KoopaBattleMapStair::isRequestAttackVs1() const {
    if (isTypeNormal() && isNerve(&NrvKoopaBattleMapStair::KoopaBattleMapStairNrvWaitKoopaFire::sInstance) && MR::isStep(this, mFireAttackStep)) {
        return true;
    }

    return false;
}

s32 KoopaBattleMapStair::calcRemainTimeToBreak() const {
    return mTimeToBreak - getNerveStep();
}

bool KoopaBattleMapStair::isRequestAttackVs3() const {
    if (!_A6 && isTypeNormal() && isNerve(&NrvKoopaBattleMapStair::KoopaBattleMapStairNrvWaitKoopaFire::sInstance)) {
        if (MR::isStep(this, mFireAttackStep) || mFireAttackStep < 0) {
            return true;
        }
    }

    return false;
}

namespace {
    void updateNearestPos(TVec3f* pPos, f32* pDist, const TVec3f& a3, const TVec3f& a4, s32 a5, s32 a6) {
        if (a5 >= 0) {
            if (a5 == a6) {
                pPos->set(a3);
            }
        } else {
            f32 dist = a3.distance(a4);

            if (dist > *pDist) {
                return;
            }

            pPos->set(a3);
            *pDist = dist;
        }
    }
};  // namespace

f32 KoopaBattleMapStair::calcAndSetTargetPos(TVec3f* pPos, const TVec3f& a2) {
    TVec3f axisZ, axisY, axisX;
    MR::calcActorAxis(&axisX, &axisY, &axisZ, this);
    TVec3f v38 = (axisY * ::sHalfHeight) + mPosition;
    f32 v20 = v38.distance(a2);
    pPos->set< f32 >(v38);
    s32 val;

    if (mIsBig) {
        val = mArg5;

        for (s32 i = 0; i < ARRAY_SIZE(offset_table); i++) {
            TVec3f v31 = v38 + (axisX * ::sBigHalfWidthX * offset_table[i].x) + (axisZ * ::sBigHalfWidthZ * offset_table[i].z);

            ::updateNearestPos(pPos, &v20, v31, a2, val, i);
        }
    } else if (mIsTurn) {
        TVec3f v30 = axisX * ::sTurnHalfWidthX;
        TVec3f tmp = v38 + (axisZ * ::sTurnHalfWidthZ);
        TVec3f v27 = tmp;
        v27 -= v30;

        ::updateNearestPos(pPos, &v20, v27, a2, -1, -1);
    } else {
        val = mArg5;

        for (s32 i = 0; i < ARRAY_SIZE(offset_table); i++) {
            TVec3f v21 = v38 + (axisX * ::sNormalHalfWidthX * offset_table[i].x) + (axisZ * ::sNormalHalfWidthZ * offset_table[i].z);

            ::updateNearestPos(pPos, &v20, v21, a2, val, i);
        }
    }

    _AC.set< f32 >(*pPos);

    return v20;
}

f32 KoopaBattleMapStair::calcTimeRate() const {
    return static_cast< f32 >(getNerveStep() - mFireAttackStep) / (mTimeToBreak - mFireAttackStep);
}

bool KoopaBattleMapStair::isBreak() const {
    return isNerve(&NrvKoopaBattleMapStair::KoopaBattleMapStairNrvWaitFall::sInstance);
}

bool KoopaBattleMapStair::isTypeNormal() const {
    return mType == Type_Normal;
}

bool KoopaBattleMapStair::isTypeDemoFar() const {
    return mType == Type_DemoFar;
}

bool KoopaBattleMapStair::isTypeDemoNear() const {
    return mType == Type_DemoNear;
}

bool KoopaBattleMapStair::isTypeNoRequestFire() const {
    return mType == Type_NoRequestFire;
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
    if (MR::isStep(this, mTimeToBreak)) {
        setNerve(&NrvKoopaBattleMapStair::KoopaBattleMapStairNrvWaitFall::sInstance);
    }
}

void KoopaBattleMapStair::exeWaitFall() {
    if (MR::isFirstStep(this)) {
        MR::startSound(this, "SE_OJ_STAIR_BREAK_START");
        MR::startAllAnim(this, "WaitFall");
    }

    MR::startLevelSound(this, "SE_OJ_LV_STAIR_BREAK");

    if (MR::isStep(this, mWaitFallStep)) {
        mVelocity.zeroInline();
        setNerve(&NrvKoopaBattleMapStair::KoopaBattleMapStairNrvFall::sInstance);
    }
}

void KoopaBattleMapStair::exeFall() {
    if (MR::isFirstStep(this)) {
        MR::onCalcGravity(this);
        MR::startAllAnim(this, "Fall");
    }

    MR::addVelocityToGravity(this, ::sFallGravity);
    MR::restrictVelocity(this, ::sFallSpeedMax);
    MR::startLevelSound(this, "SE_OJ_LV_STAIR_BREAK");

    if (MR::isStep(this, mFallStep)) {
        MR::startSound(this, "SE_OJ_STAIR_BREAK_END");
        setNerve(&NrvKoopaBattleMapStair::KoopaBattleMapStairNrvDisappear::sInstance);
    }
}

void KoopaBattleMapStair::exeDisappear() {
    if (MR::isFirstStep(this)) {
        MR::startAllAnim(this, "Disappear");
        MR::setBrkRate(this, 0.25f);
    }

    MR::addVelocityToGravity(this, ::sFallGravity);
    MR::restrictVelocity(this, ::sFallSpeedMax);

    if (MR::isBrkStopped(this)) {
        kill();
    }
}
