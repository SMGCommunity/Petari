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
    static const f32 sNormalHalfWidthX = 200.0f;
    static const f32 sNormalHalfWidthZ = 300.0f;
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
}  // namespace

namespace NrvKoopaBattleMapStair {
    NEW_NERVE(KoopaBattleMapStairNrvWaitSwitch, KoopaBattleMapStair, WaitSwitch);
    NEW_NERVE(KoopaBattleMapStairNrvWaitKoopaFire, KoopaBattleMapStair, WaitKoopaFire);
    NEW_NERVE(KoopaBattleMapStairNrvWaitFall, KoopaBattleMapStair, WaitFall);
    NEW_NERVE(KoopaBattleMapStairNrvFall, KoopaBattleMapStair, Fall);
    NEW_NERVE(KoopaBattleMapStairNrvDisappear, KoopaBattleMapStair, Disappear);
}  // namespace NrvKoopaBattleMapStair

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
    initNerve(GET_NERVE(KoopaBattleMapStair, KoopaBattleMapStairNrvWaitSwitch));
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
    if (isTypeNormal() && isNerve(GET_NERVE(KoopaBattleMapStair, KoopaBattleMapStairNrvWaitKoopaFire)) && MR::isStep(this, mFireAttackStep)) {
        return true;
    }

    return false;
}

s32 KoopaBattleMapStair::calcRemainTimeToBreak() const {
    return mTimeToBreak - getNerveStep();
}

bool KoopaBattleMapStair::isRequestAttackVs3() const {
    if (!_A6 && isTypeNormal() && isNerve(GET_NERVE(KoopaBattleMapStair, KoopaBattleMapStairNrvWaitKoopaFire))) {
        if (MR::isStep(this, mFireAttackStep) || mFireAttackStep < 0) {
            return true;
        }
    }

    return false;
}

namespace {
    void updateNearestPos(TVec3f* pPos, f32* pDist, const TVec3f& rCandidatePos, const TVec3f& rReferencePos, s32 targetIndex, s32 index) {
        if (targetIndex >= 0) {
            if (targetIndex == index) {
                pPos->set(rCandidatePos);
            }
        } else {
            f32 dist = rCandidatePos.distance(rReferencePos);

            if (dist > *pDist) {
                return;
            }

            pPos->set(rCandidatePos);
            *pDist = dist;
        }
    }

    inline void updateOffsetPos(TVec3f* pPos, f32* pDist, const TVec3f& rCenter, const TVec3f& rX, const TVec3f& rZ, f32 width, f32 depth,
                                const OffsetPair& rOffset, const TVec3f& rOrigin, s32 selection, s32 index) {
        TVec3f candidate = rCenter + (rX * width * rOffset.x) + (rZ * depth * rOffset.z);

        updateNearestPos(pPos, pDist, candidate, rOrigin, selection, index);
    }

    inline void updateTurnPos(TVec3f* pPos, f32* pDist, const TVec3f& rCenter, const TVec3f& rX, const TVec3f& rZ, const TVec3f& rOrigin) {
        updateNearestPos(pPos, pDist, rCenter + rZ * ::sTurnHalfWidthZ - rX * ::sTurnHalfWidthX, rOrigin, -1, -1);
    }
}  // namespace

f32 KoopaBattleMapStair::calcAndSetTargetPos(TVec3f* pPos, const TVec3f& rReferencePos) {
    TVec3f axisZ;
    TVec3f axisY;
    TVec3f axisX;
    MR::calcActorAxis(&axisX, &axisY, &axisZ, this);

    TVec3f center = (axisY * ::sHalfHeight) + mPosition;
    f32 distance = center.distance(rReferencePos);
    pPos->set(center);

    if (mIsBig) {
        s32 targetIndex = mArg5;

        for (s32 i = 0; i < ARRAY_SIZE(::offset_table); i++) {
            const OffsetPair& rOffset = ::offset_table[i];
            updateOffsetPos(pPos, &distance, center, axisX, axisZ, ::sBigHalfWidthX, ::sBigHalfWidthZ, rOffset, rReferencePos, targetIndex, i);
        }
    } else if (mIsTurn) {
        updateTurnPos(pPos, &distance, center, axisX, axisZ, rReferencePos);
    } else {
        const OffsetPair* pOffset;
        s32 targetIndex = mArg5;

        for (s32 i = 0; i < ARRAY_SIZE(::offset_table); i++) {
            pOffset = &::offset_table[i];
            updateOffsetPos(pPos, &distance, center, axisX, axisZ, ::sNormalHalfWidthX, ::sNormalHalfWidthZ, *pOffset, rReferencePos, targetIndex, i);
        }
    }

    _AC.set(*pPos);

    return distance;
}

f32 KoopaBattleMapStair::calcTimeRate() const {
    return static_cast< f32 >(getNerveStep() - mFireAttackStep) / (mTimeToBreak - mFireAttackStep);
}

bool KoopaBattleMapStair::isBreak() const {
    return isNerve(GET_NERVE(KoopaBattleMapStair, KoopaBattleMapStairNrvWaitFall));
}

bool KoopaBattleMapStair::isTypeNormal() const NO_INLINE {
    return mType == Type_Normal;
}

bool KoopaBattleMapStair::isTypeDemoFar() const {
    return mType == Type_DemoFar;
}

bool KoopaBattleMapStair::isTypeDemoNear() const {
    return mType == Type_DemoNear;
}

bool KoopaBattleMapStair::isTypeNoRequestFire() const NO_INLINE {
    return mType == Type_NoRequestFire;
}

void KoopaBattleMapStair::exeWaitSwitch() {
    if (MR::isFirstStep(this)) {
        MR::startAllAnim(this, "Wait");
    }

    if (MR::isOnSwitchA(this)) {
        MR::invalidateClipping(this);
        setNerve(GET_NERVE(KoopaBattleMapStair, KoopaBattleMapStairNrvWaitKoopaFire));
    }
}

void KoopaBattleMapStair::exeWaitKoopaFire() {
    if (MR::isStep(this, mTimeToBreak)) {
        setNerve(GET_NERVE(KoopaBattleMapStair, KoopaBattleMapStairNrvWaitFall));
    }
}

void KoopaBattleMapStair::exeWaitFall() {
    if (MR::isFirstStep(this)) {
        MR::startSound(this, "SE_OJ_STAIR_BREAK_START");
        MR::startAllAnim(this, "WaitFall");
    }

    MR::startLevelSound(this, "SE_OJ_LV_STAIR_BREAK");

    if (MR::isStep(this, mWaitFallStep)) {
        mVelocity.zero();
        setNerve(GET_NERVE(KoopaBattleMapStair, KoopaBattleMapStairNrvFall));
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
        setNerve(GET_NERVE(KoopaBattleMapStair, KoopaBattleMapStairNrvDisappear));
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
