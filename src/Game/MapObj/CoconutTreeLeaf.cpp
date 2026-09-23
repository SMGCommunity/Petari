#include "Game/MapObj/CoconutTreeLeaf.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/Util.hpp"

void CoconutTreeLeaf_FORCE_MATCH_SDATA2() {
    (void)1.0f;
    (void)0.0f;
    (void)-1.0f;
}

namespace {
    const f32 sPointInterval = 100.0f;
    const f32 sSpringAccelRateFree = 0.001f;
    const f32 sSpringAccelRateLand = 0.01f;
    const f32 sFrictionFree = 0.99f;
    const f32 sFrictionLand = 0.95f;
    const f32 sGravityFree = 0.005f;
    const f32 sGravityLand = 0.2f;
    const f32 sDistanceNear = 1000.0f;
    const f32 sDistanceFar = 5000.0f;
    const f32 sAxisLeanRateY = 0.01f;
    const f32 sHipDropAccel = 0.1f;
    const f32 sWindAccelNear = 0.02f;
    const f32 sWindAccelMiddle = 0.03f;
    const f32 sWindAccelFar = 0.05f;
    const f32 sWindRandomAccelNear = 0.005f;
    const f32 sWindRandomAccelMiddle = 0.01f;
    const f32 sWindRandomAccelFar = 0.03f;
    const s32 sStepToWindStartMin = 15;
    const s32 sStepToWindStartMax = 150;
    const s32 sSteptoWindStartAfterLand = 120;
    const s32 sStepWindMin = 10;
    const s32 sStepWindMax = 30;
}  // namespace

CoconutTreeLeaf::CoconutTreeLeaf(const CoconutTreeLeafGroup* pGroup, MtxPtr pJointMtx, const TVec3f* pWindDirection)
    : LiveActor("ヤシの木の葉"), mGroup(pGroup), mTipPosition(0.0f), mRestTipPosition(0.0f),
      mRestUp(pJointMtx[0][1], pJointMtx[1][1], pJointMtx[2][1]), mAxisX(pJointMtx[0][0], pJointMtx[1][0], pJointMtx[2][0]),
      mAxisY(pJointMtx[0][1], pJointMtx[1][1], pJointMtx[2][1]), mAxisZ(pJointMtx[0][2], pJointMtx[1][2], pJointMtx[2][2]), mJointMtx(pJointMtx),
      mWindStep(), mWindStartStep(), mWindAccel(0.0f), mWindDirection(pWindDirection) {
    mPosition.set< f32 >(pJointMtx[0][3], pJointMtx[1][3], pJointMtx[2][3]);
    mRestTipPosition.set(mAxisZ);
    mRestTipPosition.mult(::sPointInterval);
    mRestTipPosition.add(mPosition);
    mTipPosition.set(mRestTipPosition);
}

void CoconutTreeLeaf::init(const JMapInfoIter& rIter) {
    initHitSensor(1);
    MR::addBodyMessageSensorMapObj(this);
    MR::initCollisionPartsFromResourceHolder(this, "CoconutTreeLeaf", getSensor("body"), MR::getResourceHolder(mGroup), mJointMtx);
    MR::invalidateClipping(this);
    makeActorAppeared();
}

void CoconutTreeLeaf::update(f32 windAccel, f32 windRandomAccel) {
    f32 springAccelRate = ::sSpringAccelRateFree;
    f32 friction = ::sFrictionFree;
    f32 gravity = ::sGravityFree;

    if (MR::isOnPlayer(this)) {
        mWindStep = 0;
        mWindStartStep = ::sSteptoWindStartAfterLand;
        springAccelRate = ::sSpringAccelRateLand;
        friction = ::sFrictionLand;
        gravity = ::sGravityLand;
    } else if (mWindStep > 0) {
        mVelocity.add(mWindAccel);
        mWindStep--;

        if (mWindStep <= 0) {
            mWindStartStep = MR::getRandom(::sStepToWindStartMin, ::sStepToWindStartMax);
        }
    } else {
        mWindStartStep--;

        if (mWindStartStep <= 0) {
            mWindAccel.set(*mWindDirection);
            mWindAccel.scale(windAccel);

            TVec3f randomAccel(mRestUp);
            randomAccel.scale(windRandomAccel * MR::getRandom(-1.0f, 1.0f));
            mWindAccel.add(randomAccel);

            mWindStep = MR::getRandom(::sStepWindMin, ::sStepWindMax);
        }
    }

    TVec3f gravityAccel(mRestUp);
    gravityAccel.scale(-gravity);
    mVelocity.add(gravityAccel);

    TVec3f springAccel(mRestTipPosition);
    springAccel.sub(mTipPosition);
    f32 lean = -springAccel.dot(*mWindDirection);
    springAccel.scale(springAccelRate);
    mVelocity.add(springAccel);
    mVelocity.scale(friction);
    mTipPosition.add(mVelocity);

    TVec3f axisZ(mTipPosition);
    axisZ.sub(mPosition);

    if (!MR::isNearZero(mAxisZ)) {
        MR::normalize(&axisZ);
    } else {
        axisZ.set(mAxisZ);
    }

    mAxisZ.set(axisZ);

    if (lean < 0.0f) {
        lean = 0.0f;
    }

    TVec3f up(*mWindDirection);
    up.scale(::sAxisLeanRateY * lean);
    up.add(mRestUp);

    TVec3f axisX(mAxisX);
    axisX.cross(up, mAxisZ);

    if (!MR::isNearZero(mAxisX)) {
        MR::normalize(&axisX);
    } else {
        axisX.set(mAxisX);
    }

    mAxisX.set(axisX);
    mAxisY.cross(mAxisZ, mAxisX);
    MR::normalize(&mAxisY);
    MR::setMtxAxisXYZ(mJointMtx, mAxisX, mAxisY, mAxisZ);

    if (mGroup->mDistanceToPlayer < ::sDistanceNear && (MR::isOnGroundPlayer() || MR::getPlayerVelocity()->dot(mAxisY) <= 0.0f)) {
        if (!MR::isValidCollisionParts(this)) {
            MR::validateCollisionParts(this);
        } else {
            MR::setCollisionMtx(this);
        }
    } else if (MR::isValidCollisionParts(this)) {
        MR::invalidateCollisionParts(this);
    }
}

bool CoconutTreeLeaf::receiveMsgPlayerAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    if (MR::isMsgPlayerHipDropFloor(msg)) {
        TVec3f accel(mRestUp);
        accel.scale(-::sHipDropAccel);
        mVelocity.add(accel);
        return true;
    }

    return false;
}

CoconutTreeLeafGroup::CoconutTreeLeafGroup(const char* pName)
    : LiveActor(pName), mNumLeaves(), mLeaves(), mWindDirection(0.0f, 0.0f, 1.0f), mDistanceToPlayer() {
}

void CoconutTreeLeafGroup::init(const JMapInfoIter& rIter) {
    MR::initDefaultPos(this, rIter);
    initModelManagerWithAnm("CoconutTreeLeaf", nullptr, false);
    MR::connectToSceneMapObjNoCalcAnim(this);
    mNumLeaves = MR::getJointNum(this) - 1;
    mLeaves = new CoconutTreeLeaf*[mNumLeaves];

    for (s32 i = 0; i < mNumLeaves; i++) {
        mLeaves[i] = new CoconutTreeLeaf(this, MR::getJointMtx(this, i + 1), &mWindDirection);
        mLeaves[i]->initWithoutIter();
    }

    initHitSensor(1);
    MR::addBodyMessageSensorMapObj(this);
    MR::setClippingTypeSphere(this, 500.0f);
    makeActorAppeared();
}

void CoconutTreeLeafGroup::startClipped() {
    LiveActor::startClipped();

    for (s32 i = 0; i < mNumLeaves; i++) {
        mLeaves[i]->startClipped();
    }
}

void CoconutTreeLeafGroup::endClipped() {
    LiveActor::endClipped();

    for (s32 i = 0; i < mNumLeaves; i++) {
        mLeaves[i]->endClipped();
    }
}

void CoconutTreeLeafGroup::control() {
    mDistanceToPlayer = MR::calcDistanceToPlayer(this);
    f32 windAccel = ::sWindAccelMiddle;
    f32 windRandomAccel = ::sWindRandomAccelMiddle;

    if (mDistanceToPlayer < ::sDistanceNear) {
        windAccel = ::sWindAccelNear;
        windRandomAccel = ::sWindRandomAccelNear;
    } else if (mDistanceToPlayer > ::sDistanceFar) {
        windAccel = ::sWindAccelFar;
        windRandomAccel = ::sWindRandomAccelFar;
    }

    for (s32 i = 0; i < mNumLeaves; i++) {
        mLeaves[i]->update(windAccel, windRandomAccel);
    }
}

CoconutTreeLeaf::~CoconutTreeLeaf() {
}

CoconutTreeLeafGroup::~CoconutTreeLeafGroup() {
}

MtxPtr CoconutTreeLeaf::getBaseMtx() const {
    return mJointMtx;
}
