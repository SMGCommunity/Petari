#include "Game/Enemy/BegomanSpringHead.hpp"

namespace NrvBegomanSpringHead {
    NEW_NERVE(HostTypeNrvWait, BegomanSpringHead, Wait);
    NEW_NERVE(HostTypeNrvHopStart, BegomanSpringHead, HopStart);
    NEW_NERVE(HostTypeNrvHopWait, BegomanSpringHead, HopWait);
    NEW_NERVE(HostTypeNrvHopEnd, BegomanSpringHead, HopEnd);
    NEW_NERVE(HostTypeNrvHopJump, BegomanSpringHead, HopJump);
}  // namespace NrvBegomanSpringHead

void BegomanHead::calcAndSetBaseMtx() {
    PartsModel::calcAndSetBaseMtx();
    TVec3f v7;
    JMathInlineVEC::PSVECMultiply(_9C, mScale, v7);
    MR::setBaseScale(this, v7);
}

BegomanSpringHead::BegomanSpringHead(LiveActor* pActor, MtxPtr pMtx)
    : BegomanHead(pActor, "バネ頭", "BegomanSpringHead", pMtx, 10, false), _9C(nullptr) {
}

BegomanHead::~BegomanHead() {
}

void BegomanSpringHead::init(const JMapInfoIter& rIter) {
    initNerve(&NrvBegomanSpringHead::HostTypeNrvWait::sInstance);
    MR::initLightCtrl(this);
    initHitSensor(1);
    MR::addHitSensorAtJoint(this, "head", "SpringJoint5", 71, 8, 100.0f, TVec3f(0.0f, 100.0f, 0.0f));
    PartsModel::init(rIter);
}

bool BegomanSpringHead::isSpringHop() {
    return LiveActor::isNerve(&NrvBegomanSpringHead::HostTypeNrvWait::sInstance) == 0;
}

void BegomanSpringHead::getHopEndBckFrameMax() {
    MR::getBckFrameMax(this, "HopEnd");
}

void BegomanSpringHead::tryHopStart() {
    if (!isNerve(&NrvBegomanSpringHead::HostTypeNrvHopStart::sInstance)) {
        setNerve(&NrvBegomanSpringHead::HostTypeNrvHopStart::sInstance);
    }
}

void BegomanSpringHead::tryHopEnd() {
    if (isNerve(&NrvBegomanSpringHead::HostTypeNrvHopEnd::sInstance) || isNerve(&NrvBegomanSpringHead::HostTypeNrvWait::sInstance)) {
        return;
    }
    setNerve(&NrvBegomanSpringHead::HostTypeNrvHopEnd::sInstance);
}

void BegomanSpringHead::tryHopJump() {
    if (!isNerve(&NrvBegomanSpringHead::HostTypeNrvHopJump::sInstance)) {
        setNerve(&NrvBegomanSpringHead::HostTypeNrvHopJump::sInstance);
    }
}

void BegomanSpringHead::forceWaitImmediately() {
    MR::startBck(this, "Wait", nullptr);
    MR::startBrk(this, "Green");
    setNerve(&NrvBegomanSpringHead::HostTypeNrvWait::sInstance);
}

void BegomanSpringHead::exeWait() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "Wait", nullptr);
        MR::startBrk(this, "Green");
    }
}

void BegomanSpringHead::exeHopStart() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "HopStart", nullptr);
        MR::startBrk(this, "OnAndOff");
        MR::setNerveAtBckStopped(this, &NrvBegomanSpringHead::HostTypeNrvHopWait::sInstance);
    }
}

void BegomanSpringHead::exeHopWait() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "HopWait", nullptr);
        MR::startBrk(this, "OnAndOff");
    }
}

void BegomanSpringHead::exeHopEnd() {
    if (MR::isFirstStep(this)) {
        MR::startSound(this, "SE_EM_BEGOMAN_CLOSE_SPRING", -1, -1);
    }
    MR::startBckAtFirstStep(this, "HopEnd");
    MR::setNerveAtBckStopped(this, &NrvBegomanSpringHead::HostTypeNrvWait::sInstance);
}

void BegomanSpringHead::exeHopJump() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "HopJump", nullptr);
        MR::startBrk(this, "OnAndOff");
    }
    MR::setNerveAtBckStopped(this, &NrvBegomanSpringHead::HostTypeNrvHopWait::sInstance);
}

BegomanSpringHead::~BegomanSpringHead() {
}
