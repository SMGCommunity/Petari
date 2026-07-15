#include "Game/MapObj/JumpStand.hpp"
#include "Game/LiveActor/HitSensor.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/Util.hpp"
#include "Game/Util/PlayerUtil.hpp"

namespace NrvJumpStand {
    NEW_NERVE(JumpStandNrvWait, JumpStand, Wait);
    NEW_NERVE(JumpStandNrvTrampleBound, JumpStand, TrampleBound);
    NEW_NERVE(JumpStandNrvHipDropBound, JumpStand, HipDropBound);
    NEW_NERVE(JumpStandNrvStarPieceBound, JumpStand, StarPieceBound);
};  // namespace NrvJumpStand

JumpStand::JumpStand(const char* pName) : LiveActor(pName), mBindedActor(nullptr), _C0(0) {
    _90.identity();
}

void JumpStand::init(const JMapInfoIter& rIter) {
    MR::initDefaultPos(this, rIter);
    initModelManagerWithAnm("JumpStand", nullptr, false);
    MR::connectToSceneMapObjStrongLight(this);
    MR::initLightCtrl(this);
    initHitSensor(2);
    MR::addBodyMessageSensorMapObj(this);
    TVec3f binderOffs;
    binderOffs.x = 0.0f;
    binderOffs.y = 100.0f;
    binderOffs.z = 0.0f;
    MR::addHitSensorPriorBinder(this, "binder", 4, 200.0f, binderOffs);
    MR::invalidateHitSensor(this, "binder");

    JMath::gekko_ps_copy12(&_90, MR::getJointMtx(this, "JumpStandJoint03"));
    MR::initCollisionParts(this, "JumpStand", getSensor("body"), _90);
    initEffectKeeper(0, nullptr, false);
    initSound(4, false);
    MR::setGroupClipping(this, rIter, 16);
    MR::initShadowFromCSV(this, "Shadow");
    MR::onCalcGravity(this);
    initNerve(&NrvJumpStand::JumpStandNrvWait::sInstance);
    makeActorAppeared();
}

void JumpStand::exeWait() {
    if (MR::isFirstStep(this)) {
        MR::validateClipping(this);
        MR::startBck(this, "Wait", nullptr);
    }
}

void JumpStand::exeTrampleBound() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "Bound", nullptr);
        MR::startSound(this, "SE_OJ_JUMP_STAND_LAND_S");
    }

    if (MR::isLessEqualStep(this, 5) && MR::testCorePadTriggerA(0)) {
        _C0 = 1;
        MR::startBck(this, "BoundJump", nullptr);
        MR::setBckFrame(this, getNerveStep());
    }

    if (MR::isStep(this, 5)) {
        if (_C0) {
            MR::startSound(this, "SE_OJ_JUMP_STAND_BOUND_M");
            MR::startSoundPlayer("SE_PV_JUMP_L", -1);
        } else {
            MR::startSound(this, "SE_OJ_JUMP_STAND_BOUND_S");
            MR::startSoundPlayer("SE_PV_JUMP_M", -1);
        }

        if (_C0) {
            endBindAndShootUp(32.0f, UNK_1);
        } else {
            endBindAndShootUp(26.0f, UNK_0);
        }

        MR::validateCollisionParts(this);
    }

    if (MR::isStep(this, 10)) {
        MR::validateCollisionParts(this);
    }

    if (MR::isBckStopped(this)) {
        _C0 = 0;
        setNerve(&NrvJumpStand::JumpStandNrvWait::sInstance);
    }
}

void JumpStand::exeHipDropBound() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "BoundHipDrop", nullptr);
        MR::stopSound(mBindedActor, "SE_PM_HIPDROP", 0);
        MR::startSound(this, "SE_OJ_JUMP_STAND_LAND_L");
    }

    if (MR::isLessStep(this, 15) && mBindedActor != nullptr) {
        f32 v2 = (6.0f - MR::calcNerveEaseOutRate(this, 15));
        TVec3f v3;
        MR::calcUpVec(&v3, this);
        JMAVECScaleAdd(&v3, &mBindedActor->mPosition, &mBindedActor->mPosition, -v2);
    }

    if (MR::isStep(this, 15)) {
        MR::startSound(this, "SE_OJ_JUMP_STAND_BOUND_L");
        MR::startSoundPlayer("SE_PV_JUMP_JOY", -1);
        endBindAndShootUp(43.0f, UNK_2);
    }

    if (MR::isStep(this, 20)) {
        MR::validateCollisionParts(this);
    }

    if (MR::isBckStopped(this)) {
        MR::validateCollisionParts(this);
        setNerve(&NrvJumpStand::JumpStandNrvWait::sInstance);
    }
}

void JumpStand::exeStarPieceBound() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "Bound", nullptr);
        MR::startSound(this, "SE_OJ_JUMP_STAND_LAND_S");
    }

    if (MR::isBckStopped(this)) {
        setNerve(&NrvJumpStand::JumpStandNrvWait::sInstance);
    }
}

void JumpStand::control() {
    TVec3f pos;
    MR::copyJointPos(this, "JumpStandJoint03", &pos);
    _90.setTrans(pos);
}

bool JumpStand::receiveMsgPlayerAttack(u32 msg, HitSensor*, HitSensor*) {
    if (isNerve(&NrvJumpStand::JumpStandNrvTrampleBound::sInstance)) {
        return false;
    }

    if (isNerve(&NrvJumpStand::JumpStandNrvHipDropBound::sInstance)) {
        return false;
    }

    if (MR::isMsgStarPieceAttack(msg)) {
        setNerve(&NrvJumpStand::JumpStandNrvStarPieceBound::sInstance);
        return true;
    }

    return false;
}

bool JumpStand::receiveOtherMsg(u32 msg, HitSensor* pSender, HitSensor* pReceiving) {
    if (MR::isMsgUpdateBaseMtx(msg) && mBindedActor != nullptr) {
        updateBindActorMtx();
        return true;
    } else if (MR::isMsgFloorTouch(msg)) {
        MR::validateHitSensor(this, "binder");
        return true;
    } else if (!MR::isMsgAutoRushBegin(msg)) {
        return false;
    }

    if (!MR::isSensorPlayer(pSender)) {
        return false;
    }

    if (!MR::isOnPlayer(getSensor("body"))) {
        return false;
    } else {
        if (!MR::isPlayerHipDropLand()) {
            if (!tryToStartBind(pSender)) {
                return false;
            } else {
                setNerve(&NrvJumpStand::JumpStandNrvTrampleBound::sInstance);
                return true;
            }

        } else if (MR::isPlayerHipDropLand()) {
            if (!tryToStartBind(pSender)) {
                return false;
            } else {
                setNerve(&NrvJumpStand::JumpStandNrvHipDropBound::sInstance);
                return true;
            }
        }
    }

    return false;
}

bool JumpStand::tryToStartBind(HitSensor* pSensor) {
    if (mBindedActor != nullptr) {
        return false;
    }

    MR::tryRumblePadMiddle(this, WPAD_CHAN0);
    MR::invalidateCollisionParts(this);
    MR::invalidateClipping(this);
    mBindedActor = pSensor->mHost;
    MR::offBind(mBindedActor);
    return true;
}

void JumpStand::updateBindActorMtx() {
    TPos3f v6;
    v6.identity();
    TVec3f v5;
    MR::calcPlayerWorldPadDir(&v5, MR::getSubPadStickX(0), MR::getSubPadStickY(0));
    MR::calcMtxFromGravityAndZAxis(&v6, mBindedActor, mGravity, v5);
    MR::blendMtxRotate(mBindedActor->getBaseMtx(), v6, 0.40f, v6);
    v6.setTrans(mBindedActor->mPosition);
    MR::setBaseTRMtx(mBindedActor, v6);
}

// https://decomp.me/scratch/GI33x
void JumpStand::endBindAndShootUp(f32 v1, JumpType type) {
    TVec3f v28;

    if (MR::isPlayerInRush()) {
        if (type == UNK_2) {
            MR::calcUpVec(&v28, this);
        } else {
            JMathInlineVEC::PSVECNegate(&mGravity, &v28);
        }

        v28.x *= v1;
        v28.y *= v1;
        v28.z *= v1;

        f32 v9 = MR::getSubPadStickY(0) * MR::getSubPadStickY(0);
        f32 v11 = MR::getSubPadStickX(0) * MR::getSubPadStickX(0);
        f32 v12 = v11 + v9;

        f32 v13;
        f32 v14;

        // FIXME: JGeometry::TUtil<f32>::sqrt(v12)
        if (v12 > 0.0f) {
            v13 = __frsqrte(v12);
            v14 = ((-(((v13 * v12) * v13) - 3.0f) * (v13 * v12)) * 0.5f);
        } else {
            v14 = v11 + v9;
        }

        TVec3f v27;

        if (v14 > 0.1f) {
            MR::normalize(v28, &v27);
            TVec3f v26;
            MR::calcPlayerWorldPadDir(&v26, MR::getSubPadStickX(0), MR::getSubPadStickY(0));
            TVec3f v25;
            v25.scale(3.0f * v14, v26);
            JMAVECScaleAdd(&v27, &v25, &v25, -v27.dot(v25));
            v28.add(v25);
        }

        if (type == UNK_0) {
            MR::startBckPlayer("TrampolineJumpLow", (const char*)0);
        } else if (type == UNK_1) {
            MR::startBckPlayer("TrampolineJumpMiddle", (const char*)0);
        } else if (type == UNK_2) {
            MR::startBckPlayer("TrampolineJumpHigh", (const char*)0);
        }

        MR::endBindAndPlayerJump(this, v28, 0);

        if (type != UNK_2) {
            MR::becomePlayerNormalJumpStatus();
        }

        MR::invalidateHitSensor(this, "binder");
    }

    mBindedActor = nullptr;
}

JumpStand::~JumpStand() {
}
