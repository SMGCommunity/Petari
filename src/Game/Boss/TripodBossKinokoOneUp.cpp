#include "Game/Boss/TripodBossKinokoOneUp.hpp"
#include "Game/Boss/TripodBossAccesser.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/MapObj/BenefitItemObj.hpp"
#include "Game/Util/ObjUtil.hpp"

namespace NrvTripodBossKinokoOneUp {
    NEW_NERVE(TripodBossKinokoOneUpNrvActive, TripodBossKinokoOneUp, Active);
    NEW_NERVE(TripodBossKinokoOneUpNrvEnd, TripodBossKinokoOneUp, End);
} // namespace NrvTripodBossKinokoOneUp

TripodBossKinokoOneUp::TripodBossKinokoOneUp(const char* pName)
    : LiveActor(pName) {
    mOneUp = nullptr;
    _F0 = -1;
    _8C.identity();
    _BC.identity();
}

void TripodBossKinokoOneUp::init(const JMapInfoIter& rIter) {
    MR::getJMapInfoMatrixFromRT(rIter, &_8C);
    MR::connectToSceneMapObjDecorationMovement(this);
    MR::invalidateClipping(this);
    MR::getJMapInfoArg0NoInit(rIter, &_F0);
    initNerve(&NrvTripodBossKinokoOneUp::TripodBossKinokoOneUpNrvActive::sInstance);
    mOneUp = new BenefitItemOneUp("1UPキノコ", "KinokoOneUp");
    mOneUp->setFollowMtx(_BC);
    mOneUp->initWithoutIter();
    MR::invalidateClipping(mOneUp);
    makeActorDead();
    MR::addTripodBossParts(this);
}

void TripodBossKinokoOneUp::control() {
}

void TripodBossKinokoOneUp::exeActive() {
    _BC.setInline(_8C);
    MR::concatTripodBossAttachJointMatrix(&_BC, _F0);
    f32 z = _BC.mMtx[2][3];
    f32 y = _BC.mMtx[1][3];
    f32 x = _BC.mMtx[0][3];
    mPosition.set< f32 >(x, y, z);
    if (MR::isFirstStep(this)) {
        mOneUp->makeActorAppeared();
    }

    if (MR::isBrokenTripodBoss() || MR::isDead(mOneUp)) {
        mOneUp->kill();
        setNerve(&NrvTripodBossKinokoOneUp::TripodBossKinokoOneUpNrvEnd::sInstance);
    }
}

TripodBossKinokoOneUp::~TripodBossKinokoOneUp() {
}

void TripodBossKinokoOneUp::exeEnd() {
}

MtxPtr TripodBossKinokoOneUp::getBaseMtx() const {
    return (MtxPtr)_BC.mMtx;
}
