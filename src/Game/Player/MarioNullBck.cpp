#include "Game/Player/MarioNullBck.hpp"
#include "Game/Util/MtxUtil.hpp"

MarioNullBck::MarioNullBck(const char* pName) : LiveActor(pName) {}

MarioNullBck::~MarioNullBck() {}

void MarioNullBck::init(const JMapInfoIter& rIter) {
    initModelManagerWithAnm("MarioDummyItem", nullptr, false);
    MR::startBck(this, "PickUpBall", nullptr);
    makeActorDead();
}

void MarioNullBck::appear() {
    _8C = 0;
    LiveActor::appear();
}

void MarioNullBck::getLastPos(TVec3f* pDest) {
    MR::copyJointPos(this, "DummyItem", pDest);
}

bool MarioNullBck::getFramePos(f32 a1, TVec3f* a2, TVec3f* a3) {
    bool ret = false;
    if (MR::isDead(this)) {
        return true;
    }
    if (a1 > (f32)MR::getBckCtrl(this)->mEnd - 0.01f) {
        a1 = ((f32)MR::getBckCtrl(this)->mEnd) - 0.01f;
        ret = true;
    }
    if ((f32)_8C >= a1) {
        ret = true;
        a1 = (f32)MR::getBckCtrl(this)->mEnd;
    }
    _8C = (s16)a1;
    MR::setBckFrame(this, a1);
    MR::setBckRate(this, 0.0f);
    MR::updateModelManager(this);
    MR::calcAnimModelManager(this);
    MR::copyJointPos(this, "DummyItem", a2);
    MR::makeRTFromMtxPtr(a2, a3, MR::getJointMtx(this, "DummyPoly"), true);
    return ret;
}
