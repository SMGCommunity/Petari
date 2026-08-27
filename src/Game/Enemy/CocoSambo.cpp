#include "Game/Enemy/CocoSambo.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/Util.hpp"

namespace NrvCocoSamboHead {
    NEW_NERVE(CocoSamboHeadNrvHeadConnectedBody, CocoSamboHead, HeadConnectedBody);
    NEW_NERVE(CocoSamboHeadNrvHeadFall, CocoSamboHead, Fall);
    NEW_NERVE(CocoSamboHeadNrvHeadFallLand, CocoSamboHead, FallLand);
    NEW_NERVE(CocoSamboHeadNrvHeadSwoon, CocoSamboHead, Swoon);
    NEW_NERVE(CocoSamboHeadNrvHeadSwoonEnd, CocoSamboHead, SwoonEnd);
    NEW_NERVE(CocoSamboHeadNrvHeadBlow, CocoSamboHead, Blow);
};  // namespace NrvCocoSamboHead

namespace {
    const Vec cHeadSensorOffset = {60.0f, 0.0f, 0.0f};
    const Vec cBlowVelocity = {0.0f, 20.0f, -100.0f};
};  // namespace

CocoSamboHead::CocoSamboHead(LiveActor* pHost) : PartsModel(pHost, "ココサンボ[頭]", "CocoSamboHead", 0, 18, 0) {
    mFrontVec.set(0.0f, 0.0f, 1.0f);
}

void CocoSamboHead::init(const JMapInfoIter& rIter) {
    TVec3f sensorOffs(cHeadSensorOffset);
    sensorOffs *= mScale;
    initHitSensor(2);
    MR::addHitSensorAtJoint(this, "body", "Head", 37, 8, (100.0f * mScale.x), sensorOffs);
    MR::addHitSensorAtJoint(this, "trample", "Head", 37, 8, (150.0f * mScale.x), sensorOffs);
    initFixedPosition(TVec3f(0.0f, 0.0f, 0.0f), TVec3f(0.0f, -90.0f, -90.0f), "Head");
    initNerve(&NrvCocoSamboHead::CocoSamboHeadNrvHeadConnectedBody::sInstance);
    PartsModel::init(rIter);
}

void CocoSamboHead::kill() {
    MR::startSound(this, "SE_EM_EXPLODE_S");
    LiveActor::kill();
    MR::emitEffect(this, "DeathSmokeHead");
}

void CocoSamboHead::calcAndSetBaseMtx() {
    if (mCalcOwnMtx != false) {
        PartsModel::calcAndSetBaseMtx();
    } else {
        TVec3f grav;
        MR::calcGravityVector(this, &grav, nullptr, 0);
        grav = -grav;

        TPos3f mtx;

        if (MR::isSameDirection(mFrontVec, grav)) {
            MR::makeMtxUpNoSupportPos(&mtx, grav, mPosition);
        } else {
            MR::makeMtxUpFrontPos(&mtx, grav, mFrontVec, mPosition);
        }

        MR::setBaseTRMtx(this, mtx);
    }
}

// CocoSamboHead::attackSensor
// CocoSamboHead::receiveMsgPlayerAttack
// CocoSamboHead::receiveMsgEnemyAttack

void CocoSamboHead::updateFrontVecToPlayer(const TVec3f& rVec) {
    TVec3f v4 = -rVec;
    TVec3f v5 = *MR::getPlayerPos() - mPosition;

    if (MR::isNearZero(v5) || MR::isSameDirection(v4, v5)) {
        if (MR::getMaxAbsElementIndex(v4) == 2) {
            v5.set< f32 >(0.0f, 1.0f, 0.0f);
        } else {
            v5.set< f32 >(0.0f, 0.0f, 1.0f);
        }
    }

    MR::normalize(&v5);
    MR::vecKillElement(v5, v4, &mFrontVec);
    MR::normalize(&mFrontVec);
}

void CocoSamboHead::exeFall() {
    TVec3f gravity(mHost->mGravity);
    TVec3f v16 = gravity * 2.0f;

    if (MR::isFirstStep(this)) {
        mFixedPos->copyTrans(&mPosition);
        mRotation.zero();
        mCalcOwnMtx = false;
        MR::startBck(this, "Fall", nullptr);
        updateFrontVecToPlayer(gravity);

        TVec3f v15 = mHost->mPosition - mPosition;
        TVec3f up;
        up = -gravity;

        MR::vecKillElement(v15, up, &v15);
        mVelocity = v15 * (1.0f / 45.0f) + ((up * up.dot(v15)) * 2.0f - ((v16 * 45.0f)) * (1.0f / 90.0f));
    }

    if (MR::isStep(this, 45)) {
        mPosition.set< f32 >(mHost->mPosition);
        mVelocity.zero();
        setNerve(&NrvCocoSamboHead::CocoSamboHeadNrvHeadFallLand::sInstance);
    } else {
        mVelocity += v16;
    }
}

void CocoSamboHead::exeFallLand() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "Land", nullptr);
        MR::startSound(this, "SE_EM_SFSAMBO_HEAD_LAND");
        MR::tryRumblePadWeak(this, 0);
        MR::shakeCameraWeak();
    }

    MR::setNerveAtBckStopped(this, &NrvCocoSamboHead::CocoSamboHeadNrvHeadSwoon::sInstance);
}

void CocoSamboHead::exeSwoon() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "Swoon", nullptr);
    }

    if (MR::isStep(this, 0xB4)) {
        setNerve(&NrvCocoSamboHead::CocoSamboHeadNrvHeadSwoonEnd::sInstance);
    }
}

void CocoSamboHead::exeSwoonEnd() {
    if (MR::isFirstStep(this)) {
        mVelocity.zero();
        MR::startBck(this, "SwoonEnd", nullptr);
    }

    if (MR::isStep(this, 0x5F)) {
        MR::invalidateShadow(mHost, nullptr);
        MR::tryRumblePadMiddle(this, 0);
        MR::shakeCameraNormal();
    }

    MR::setNerveAtBckStopped(this, &NrvCocoSamboHead::CocoSamboHeadNrvHeadConnectedBody::sInstance);
}

void CocoSamboHead::exeBlow() {
    if (MR::isFirstStep(this)) {
        MR::calcGravity(this);
        updateFrontVecToPlayer(mGravity);
        TVec3f v6 = -mGravity;
        TPos3f mtx;
        mtx.identity();
        MR::makeMtxUpFront(&mtx, v6, mFrontVec);
        TVec3f velocity(::cBlowVelocity);
        mtx.mult33(mVelocity);
        MR::startBck(this, "Blow", nullptr);
    }

    mVelocity += (mGravity * 1.0f);
}

CocoSambo::CocoSambo(const char* pName) : LiveActor(pName) {
}
CocoSambo::~CocoSambo() {
}
