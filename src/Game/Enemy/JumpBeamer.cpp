#include "Game/Enemy/JumpBeamer.hpp"
#include "Game/Enemy/JumpGuarder.hpp"
#include "Game/Enemy/RingBeam.hpp"
#include "Game/LiveActor/HitSensor.hpp"

namespace NrvJumpBeamer {
    NEW_NERVE(JumpBeamerNrvHide, JumpBeamer, Hide);
    NEW_NERVE(JumpBeamerNrvUp, JumpBeamer, Up);
    NEW_NERVE(JumpBeamerNrvWait, JumpBeamer, Wait);
    NEW_NERVE(JumpBeamerNrvDown, JumpBeamer, Down);
    NEW_NERVE(JumpBeamerNrvHopStart, JumpBeamer, HopStart);
    NEW_NERVE(JumpBeamerNrvHopWait, JumpBeamer, HopWait);
    NEW_NERVE(JumpBeamerNrvHopJump, JumpBeamer, HopJump);
    NEW_NERVE(JumpBeamerNrvHopEnd, JumpBeamer, HopEnd);
    NEW_NERVE(JumpBeamerNrvPreOpen, JumpBeamer, PreOpen);
    NEW_NERVE(JumpBeamerNrvOpen, JumpBeamer, Open);
    NEW_NERVE(JumpBeamerNrvClose, JumpBeamer, Close);
    NEW_NERVE(JumpBeamerNrvInter, JumpBeamer, Inter);
};  // namespace NrvJumpBeamer

JumpBeamer::JumpBeamer(const char* pName) : JumpEmitter(pName) {
}

JumpEmitter::~JumpEmitter() {
}

void JumpBeamer::control() {
    PartsModel* head = mHeadModel;
    TMtx34f mtx;
    mtx.identity();
    MR::makeMtxTRS(mtx, TVec3f(0.0f, 0.0f, 0.0f), head->mRotation, head->mScale);
    JMath::gekko_ps_copy12(&_90, _8C);
    TMtx34f v5;
    v5.concat(_90, mtx);
    JMath::gekko_ps_copy12(&_90, &v5);
    updateEventCamera();
}

void JumpBeamer::init(const JMapInfoIter& rIter) {
    initModelManagerWithAnm("JumpBeamerBody", nullptr, false);
    mHeadModel = MR::createPartsModelMapObjStrongLight(this, "ジャンプビーマー頭", "JumpBeamerHead", _90);
    MR::initLightCtrl(mHeadModel);
    MR::initDefaultPos(this, rIter);
    MR::connectToSceneEnemy(this);
    MR::initLightCtrl(this);
    initHitSensor(2);
    MR::addHitSensorMtx(this, "Jump", 31, 8, 145.0f, MR::getJointMtx(mHeadModel, "SpringJoint3"), TVec3f(0.0f, -100.0f, 0.0f));
    MR::addHitSensorMtxEnemy(this, "Body", 8, 145.0f, MR::getJointMtx(this, "Body"), TVec3f(0.0f, 35.0f, 0.0f));
    getSensor("Body")->setType(29);
    getSensor("Body")->validate();
    getSensor("Jump")->invalidate();
    MR::initShadowVolumeSphere(this, 140.0f);
    MR::invalidateShadow(this, nullptr);
    initEffectKeeper(1, nullptr, false);
    initSound(8, false);
    MR::invalidateClipping(this);
    initNerve(&NrvJumpBeamer::JumpBeamerNrvHide::sInstance);
    MR::startBckWithInterpole(this, "Down", 0);
    MR::setBckFrame(this, MR::getBckCtrl(this)->mEnd - 1);
    MR::calcAnimDirect(this);
    _8C = MR::getJointMtx(this, "Top");
    MR::useStageSwitchReadA(this, rIter);

    if (MR::useStageSwitchReadB(this, rIter)) {
        MR::listenStageSwitchOffB(this, MR::Functor_Inline(this, &JumpBeamer::syncSwitchOffB));
    }

    initEventCamera(rIter);
    MR::joinToGroupArray(this, rIter, nullptr, 32);
    makeActorAppeared();
    f32 arg1 = 20.0f;
    s32 arg0 = 100;
    MR::getJMapInfoArg0NoInit(rIter, &arg1);
    MR::getJMapInfoArg1NoInit(rIter, &arg0);
    mBeams = new RingBeam*[5];

    for (s32 i = 0; i < 5; i++) {
        mBeams[i] = nullptr;
    }

    for (s32 i = 0; i < 3; i++) {
        mBeams[i] = new RingBeam("リングビーム", this, false, false);
        mBeams[i]->init(rIter);
        mBeams[i]->setSpeed(arg1);
        mBeams[i]->setLife(arg0);
    }
}

void JumpBeamer::attackSensor(HitSensor* a1, HitSensor* a2) {
    if (MR::isSensorPlayer(a2)) {
        MR::sendMsgPush(a2, a1);
    }
}

bool JumpBeamer::receiveMsgPlayerAttack(u32 msg, HitSensor* a2, HitSensor* a3) {
    if (MR::isMsgPlayerTrample(msg)) {
        if (a3->isType(31)) {
            TVec3f up;
            MR::calcUpVec(&up, this);
            MR::setPlayerJumpVec(up);
            startEventCamera();
            setNerve(&NrvJumpBeamer::JumpBeamerNrvHopJump::sInstance);
        } else {
            setNerve(&NrvJumpBeamer::JumpBeamerNrvHopStart::sInstance);
        }

        return true;
    } else if (MR::isMsgPlayerHipDrop(msg)) {
        TVec3f force;
        JMathInlineVEC::PSVECNegate(&mGravity, &force);
        MR::forceJumpPlayer(force);
        return true;
    } else if (MR::isMsgPlayerSpinAttack(msg)) {
        if (!isNerve(&NrvJumpBeamer::JumpBeamerNrvHopStart::sInstance)) {
            setNerve(&NrvJumpBeamer::JumpBeamerNrvHopStart::sInstance);
        }

        return true;
    }

    return MR::isMsgStarPieceReflect(msg);
}

void JumpBeamer::syncSwitchOffB() {
    setNerve(&NrvJumpBeamer::JumpBeamerNrvInter::sInstance);

    for (s32 i = 0; i < 3; i++) {
        mBeams[i]->kill();
    }
}

bool JumpBeamer::receiveOtherMsg(u32 msg, HitSensor* a2, HitSensor* a3) {
    if (msg == 106) {
        MR::invalidateClipping(this);
        setNerve(&NrvJumpBeamer::JumpBeamerNrvUp::sInstance);
        return true;
    } else if (msg == 108) {
        setNerve(&NrvJumpBeamer::JumpBeamerNrvDown::sInstance);
        return true;
    }

    return false;
}

void JumpBeamer::exeHide() {
    bool v2 = false;

    if (MR::isValidSwitchB(this) && !MR::isOnSwitchB(this)) {
        v2 = true;
    }

    if (!v2) {
        updateRotate();
    }

    if (MR::isFirstStep(this)) {
        MR::startBck(mHeadModel, "Wait", nullptr);
        MR::startBrk(mHeadModel, "Green");
        MR::validateClipping(this);
        MR::setShadowVolumeSphereRadius(this, nullptr, 110.0f);
    }

    if (MR::enableGroupAttack(this, 3000.0f, 500.0f)) {
        MR::sendMsgToGroupMember(106, this, getSensor("Body"), "Body");
    }
}

void JumpBeamer::exeUp() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "Up", nullptr);
        MR::startSound(this, "SE_EM_JGUARDER_APPEAR", -1, -1);
        MR::validateShadow(this, nullptr);
    }

    s16 end = MR::getBckCtrl(this)->mEnd;
    f32 frame = MR::getBckFrame(this);
    MR::setShadowVolumeSphereRadius(this, nullptr, (110.0f + (30.0f * (frame / (end)))));

    if (MR::isBckStopped(this)) {
        MR::invalidateShadow(this, nullptr);
        setNerve(&NrvJumpBeamer::JumpBeamerNrvWait::sInstance);
    }
}

void JumpBeamer::exeWait() {
    updateRotate();

    if (!MR::enableGroupAttack(this, 3200.0f, 500.0f)) {
        MR::sendMsgToGroupMember(108, this, getSensor("Body"), "Body");
    } else {
        bool v3 = false;

        if (MR::isValidSwitchB(this) && !MR::isOnSwitchB(this)) {
            v3 = true;
        }

        if (!v3) {
            setNerve(&NrvJumpBeamer::JumpBeamerNrvPreOpen::sInstance);
        }
    }
}

void JumpBeamer::exeDown() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "Down", nullptr);
        MR::startSound(this, "SE_EM_JGUARDER_HIDE", -1, -1);
    }

    s16 end = MR::getBckCtrl(this)->mEnd;
    f32 frame = MR::getBckFrame(this);
    MR::setShadowVolumeSphereRadius(this, nullptr, (110.0f + (30.0f * (1.0f - frame / (end)))));
    if (MR::isBckStopped(this)) {
        setNerve(&NrvJumpBeamer::JumpBeamerNrvHide::sInstance);
    }
}

void JumpBeamer::exeHopStart() {
    if (MR::isFirstStep(this)) {
        MR::startBck(mHeadModel, "HopStart", nullptr);
        MR::startBrk(mHeadModel, "OnAndOff");
        MR::startBck(this, "Damage", nullptr);
        MR::deleteEffect(this, "Charge");
        MR::startSound(this, "SE_EM_JGUARDER_HIT", -1, -1);
        HitSensor* bodySensor = getSensor("Body");
        bodySensor->mRadius = 120.0f;
    }

    MR::startLevelSound(this, "SE_EM_LV_JGUARDER_SHAKE", -1, -1, -1);

    if (MR::isBckStopped(mHeadModel)) {
        setNerve(&NrvJumpBeamer::JumpBeamerNrvHopWait::sInstance);
    }
}

void JumpBeamer::exeHopWait() {
    if (MR::isFirstStep(this)) {
        MR::stopBck(this);
        MR::startBck(mHeadModel, "HopWait", nullptr);
        getSensor("Jump")->validate();
    }

    MR::startLevelSound(this, "SE_EM_LV_JGUARDER_SHAKE", -1, -1, -1);

    if (MR::isStep(this, 300)) {
        setNerve(&NrvJumpBeamer::JumpBeamerNrvHopEnd::sInstance);
    }
}

void JumpBeamer::exeHopJump() {
    if (MR::isFirstStep(this)) {
        MR::startBck(mHeadModel, "HopJump", nullptr);
        MR::startSound(this, "SE_EM_JGUARDER_TRAMPLE", -1, -1);
    }

    if (MR::isBckStopped(mHeadModel)) {
        setNerve(&NrvJumpBeamer::JumpBeamerNrvHopWait::sInstance);
    }
}

void JumpBeamer::exeHopEnd() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "HopEnd", nullptr);
        MR::startBck(mHeadModel, "HopEnd", nullptr);
        MR::startBrk(mHeadModel, "Green");
        MR::startSound(this, "SE_EM_JGUARDER_CLOSE_SPRING", -1, -1);
    }

    if (MR::isBckStopped(this)) {
        if (MR::isBckStopped(mHeadModel)) {
            HitSensor* bodySensor = getSensor("Body");
            bodySensor->mRadius = 145.0f;
            getSensor("Jump")->invalidate();
            setNerve(&NrvJumpBeamer::JumpBeamerNrvWait::sInstance);
        }
    }
}

void JumpBeamer::exePreOpen() {
    updateRotate();

    if (!MR::enableGroupAttack(this, 3200.0f, 500.0f)) {
        MR::sendMsgToGroupMember(108, this, getSensor("Body"), "Body");

    } else {
        if (MR::isStep(this, 0)) {
            setNerve(&NrvJumpBeamer::JumpBeamerNrvOpen::sInstance);
        }
    }
}

void JumpBeamer::exeOpen() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "Open", nullptr);
    }

    if (MR::isGreaterEqualStep(this, 240)) {
        setNerve(&NrvJumpBeamer::JumpBeamerNrvClose::sInstance);
    } else {
        if (!(getNerveStep() % 80)) {
            MR::emitEffect(this, "Charge");
        }

        if (getNerveStep() % 80 == 79) {
            MR::deleteEffect(this, "Charge");

            if (mBeams[getNerveStep() / 80] != nullptr) {
                mBeams[getNerveStep() / 80]->appear();
            }
        }
    }
}

void JumpBeamer::exeClose() {
    MR::isFirstStep(this);
    MR::startLevelSound(this, "SE_EM_LV_JGUARDER_SHUTTER_CLOSE", -1, -1, -1);
    if (MR::isBckStopped(this)) {
        setNerve(&NrvJumpBeamer::JumpBeamerNrvInter::sInstance);
    }
}

void JumpBeamer::exeInter() {
    u8 v2 = false;

    if (MR::isValidSwitchB(this) && !MR::isOnSwitchB(this)) {
        v2 = true;
    }

    if (!v2) {
        updateRotate();
    }

    if (!MR::enableGroupAttack(this, 3200.0f, 500.0f)) {
        MR::sendMsgToGroupMember(108, this, getSensor("Body"), "Body");
    } else {
        if (MR::isGreaterEqualStep(this, 80)) {
            for (s32 i = 0; i < 3; i++) {
                if (!MR::isDead(mBeams[i])) {
                    return;
                }
            }

            setNerve(&NrvJumpBeamer::JumpBeamerNrvWait::sInstance);
            return;
        }
    }
}

JumpBeamer::~JumpBeamer() {
}
