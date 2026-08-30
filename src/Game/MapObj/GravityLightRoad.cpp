#include "Game/MapObj/GravityLightRoad.hpp"
#include "Game/LiveActor/HitSensor.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/Util/ActorMovementUtil.hpp"
#include "Game/Util/ActorSensorUtil.hpp"
#include "Game/Util/ActorSwitchUtil.hpp"
#include "Game/Util/Functor.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/MathUtil.hpp"
#include "Game/Util/ModelUtil.hpp"
#include "Game/Util/MtxUtil.hpp"
#include "Game/Util/ObjUtil.hpp"
#include "Game/Util/PlayerUtil.hpp"

namespace {
    const f32 sBaseInterpoleRate = 0.2f;
    const f32 sAccel = 2.0f;
    const f32 sSpeedMax = 20.0f;
    const s32 sForceEndBindTime = 1800;
};  // namespace

namespace NrvGravityLightRoad {
    NEW_NERVE(GravityLightRoadNrvWait, GravityLightRoad, Wait);
    NEW_NERVE(GravityLightRoadNrvDraw, GravityLightRoad, Draw);
    NEW_NERVE(GravityLightRoadNrvDisappear, GravityLightRoad, Disappear);
};  // namespace NrvGravityLightRoad

void GravityLightRoad_FORCE_MATCH_SDATA2() {
    (void)0.0f;
    (void)0.5f;
    (void)2.0f;
    (void)2500.0f;
    (void)250.0f;
    (void)20.0f;
    (void)0.2f;
}

GravityLightRoad::GravityLightRoad(const char* pName) : LiveActor(pName), mHost(), mSpeed(), mRadius(2500.0f), mEndPos(0.0f, 0.0f, 0.0f) {
}

void GravityLightRoad::init(const JMapInfoIter& rIter) {
    MR::initDefaultPos(this, rIter);

    mRadius = 2500.0f * mScale.y;

    initModelManagerWithAnm("GravityLightRoad", nullptr, false);

    MR::connectToSceneMapObj(this);

    initHitSensor(1);
    MR::addHitSensorCallbackPriorBinder(this, "body", 4, 250.0f);

    f32 boundingRadius;
    MR::calcModelBoundingRadius(&boundingRadius, this);

    boundingRadius += 0.5f * mRadius;

    TVec3f upVec;
    MR::calcUpVec(&upVec, this);
    upVec.negate();

    mEndPos.scaleAdd(0.5f * mRadius, upVec, mPosition);

    MR::setClippingTypeSphere(this, boundingRadius, &mEndPos);

    MR::useStageSwitchReadA(this, rIter);

    if (MR::isValidSwitchA(this)) {
        MR::listenStageSwitchOnA(this, MR::Functor_Inline(this, &GravityLightRoad::startDisappear));
    }

    MR::startAllAnim(this, "GravityLightRoad");

    initNerve(GET_NERVE(GravityLightRoad, GravityLightRoadNrvWait));

    makeActorAppeared();
}

void GravityLightRoad::exeWait() {
    if (MR::isFirstStep(this)) {
        MR::startBrk(this, "Wait");
    }
}

void GravityLightRoad::exeDraw() {
    if (MR::isFirstStep(this)) {
        mSpeed = 0.0f;
        MR::startSoundPlayer("SE_PM_GRAV_LIGHT_DRAWN_ST", -1);
    }

    TVec3f upVec;
    MR::calcUpVec(&upVec, this);

    mSpeed += ::sAccel;

    mHost->mVelocity.scale(mSpeed, upVec);
    MR::restrictVelocity(mHost, ::sSpeedMax);

    mHost->mPosition.add(mHost->mVelocity);

    TVec3f dir;
    dir.sub(mPosition, *MR::getPlayerPos());
    TVec3f fwdVec = upVec.getOrthogonal(dir);

    if (!MR::isNearZero(fwdVec, ::sAccel)) {
        TVec3f fwdDir;
        MR::normalize(fwdVec, &fwdDir);
        mHost->mPosition.scaleAdd(::sAccel, fwdDir, mHost->mPosition);
    }

    if (MR::isStep(this, ::sForceEndBindTime)) {
        startDisappear();
    }
}

void GravityLightRoad::exeDisappear() {
    if (MR::isFirstStep(this)) {
        MR::startBrk(this, "Disappear");
    }

    if (MR::isBrkStopped(this)) {
        kill();
    }
}

bool GravityLightRoad::receiveOtherMsg(u32 msg, HitSensor* pReceiver, HitSensor* pSender) {
    if (isNerve(GET_NERVE(GravityLightRoad, GravityLightRoadNrvDisappear))) {
        return false;
    }

    if (MR::isMsgAutoRushBegin(msg) && MR::isSensorPlayer(pReceiver)) {
        return tryStartBind(pReceiver->mHost);
    }

    if (MR::isMsgUpdateBaseMtx(msg) && mHost != nullptr) {
        updateBindActorMtx();
        return true;
    }

    return false;
}

void GravityLightRoad::updateHitSensor(HitSensor* pSensor) {
    TVec3f upVec;
    MR::calcUpVec(&upVec, this);
    upVec.negate();

    TVec3f vec;
    vec.scaleAdd(mRadius, upVec, mPosition);

    MR::calcPerpendicFootToLineInside(&pSensor->mPosition, *MR::getPlayerCenterPos(), mPosition, vec);
}

void GravityLightRoad::updateBindActorMtx() {
    TPos3f baseMtx;
    baseMtx.identity();

    TVec3f upVec;
    MR::calcUpVec(&upVec, this);
    upVec.negate();

    MR::makeMtxUpNoSupport(&baseMtx, upVec);

    MR::blendMtxRotate(mHost->getBaseMtx(), baseMtx, ::sBaseInterpoleRate, baseMtx);
    baseMtx.setTrans(mHost->mPosition);

    MR::setBaseTRMtx(mHost, baseMtx);
}

void GravityLightRoad::endBind() {
    TVec3f upVec;
    MR::calcUpVec(&upVec, this);
    upVec.scale(::sAccel);

    MR::endBindAndPlayerForceJump(this, upVec, 0);

    mHost = nullptr;

    MR::validateClipping(this);
}

void GravityLightRoad::startDisappear() {
    if (mHost != nullptr) {
        endBind();
    }

    setNerve(GET_NERVE(GravityLightRoad, GravityLightRoadNrvDisappear));
}

bool GravityLightRoad::tryStartBind(LiveActor* pActor) {
    if (mHost != nullptr) {
        return false;
    }

    if (!isPlayerInCylinder()) {
        return false;
    }

    mHost = pActor;

    MR::startBckPlayer("Fall", static_cast< const char* >(nullptr));

    MR::invalidateClipping(this);

    setNerve(GET_NERVE(GravityLightRoad, GravityLightRoadNrvDraw));

    return true;
}

bool GravityLightRoad::isPlayerInCylinder() const {
    TVec3f upVec;
    MR::calcUpVec(&upVec, this);

    TVec3f dir;
    dir.sub(*MR::getPlayerPos(), mPosition);
    TVec3f fwdVec = upVec.getOrthogonal(dir);

    TVec3f lengthVec;
    lengthVec.scale(upVec.dot(dir), upVec);

    TBox3f boundingBox;
    MR::calcModelBoundingBox(&boundingBox, this);

    if (fwdVec.length() > boundingBox.f.x) {
        return false;
    }

    return lengthVec.length() > boundingBox.f.y - boundingBox.i.y == false;
}
