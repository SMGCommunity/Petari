#include "Game/MapObj/BigFan.hpp"
#include "Game/LiveActor/ModelObj.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/MapObj/BigFanHolder.hpp"
#include "Game/Util.hpp"
#include <JSystem/JMath/JMath.hpp>

void BigFan_FORCE_MATCH_SDATA2() {
    (void)1.0f;
    (void)0.0f;
    (void)0.5f;
}

namespace {
    static const f32 sBaseWindWidth = 400.0f;
};  // namespace

namespace NrvBigFan {
    NEW_NERVE(BigFanNrvStop, BigFan, Stop);
    NEW_NERVE(BigFanNrvStart, BigFan, Start);
    NEW_NERVE(BigFanNrvWait, BigFan, Wait);
};  // namespace NrvBigFan

BigFan::BigFan(const char* pName)
    : LiveActor(pName), mWindModel(), mClippingCenter(0.0f, 0.0f, 0.0f), mWindLength(4000.0f), mWindSpeed(100.0f), mIsTeresaGalaxy() {
    BigFanFunction::createBigFanHolder();
    BigFanFunction::registerBigFan(this);
}

void BigFan::init(const JMapInfoIter& rIter) {
    MR::initDefaultPos(this, rIter);
    const char* object_name;
    MR::getObjectName(&object_name, rIter);
    initModelManagerWithAnm(object_name, 0, false);
    MR::connectToSceneNoSilhouettedMapObj(this);

    if (MR::isExistCollisionResource(this, object_name)) {
        initHitSensor(1);
        MR::addBodyMessageSensorReceiver(this);
        MR::initCollisionParts(this, object_name, getSensor(0), 0);
    }

    MR::getJMapInfoArg0NoInit(rIter, &mWindLength);
    MR::getJMapInfoArg1NoInit(rIter, &mWindSpeed);
    initWindModel();
    TVec3f front;
    MR::calcFrontVec(&front, this);
    mClippingCenter.scaleAdd(0.5f * mWindLength, front, mPosition);
    MR::setClippingTypeSphere(this, 400.0f + mWindLength, &mClippingCenter);
    initSound(4, false);

    if (MR::isEqualStageName("TeresaMario2DGalaxy")) {
        mIsTeresaGalaxy = true;
    } else {
        mIsTeresaGalaxy = false;
    }

    initNerve(&NrvBigFan::BigFanNrvWait::sInstance);
    if (MR::useStageSwitchReadAppear(this, rIter)) {
        MR::listenStageSwitchOnAppear(this, MR::Functor_Inline(this, &BigFan::start));
        setNerve(&NrvBigFan::BigFanNrvStop::sInstance);
        mWindModel->kill();
    }

    MR::registerDemoSimpleCastAll(this);
    makeActorAppeared();
}

void BigFan::initWindModel() {
    mWindModel = MR::createModelObjMapObj("風モデル", "BigFanWind", getBaseMtx());
    mWindModel->initWithoutIter();
    MR::invalidateClipping(mWindModel);
    MR::startBtk(mWindModel, "BigFanWind");
    MR::registerDemoSimpleCastAll(mWindModel);
    mWindModel->mScale.z = mWindLength / 2000.0f;
}

void BigFan::calcWindInfo(TVec3f* pWindInfo, const TVec3f& rPos) {
    if (MR::isDead(this) || !isStartOrWait()) {
        pWindInfo->zero();
        return;
    }

    if (mWindLength <= 0.0f) {
        pWindInfo->zero();
        return;
    }

    TVec3f front_vec;
    MR::calcFrontVec(&front_vec, this);
    MR::normalize(&front_vec);
    TVec3f offset = rPos - mPosition;
    f32 dot = front_vec.dot(offset);

    if (dot < 0.0f) {
        pWindInfo->zero();
        return;
    }

    TVec3f ortho;
    ortho = offset - (front_vec * dot);
    f32 mag = ortho.length();

    if (mag >= ::sBaseWindWidth * mScale.x) {
        pWindInfo->zero();
        return;
    }

    f32 scalar = (1.0f - (dot / mWindLength));
    if (scalar < 0.0f) {
        pWindInfo->zero();
        return;
    }

    pWindInfo->set(front_vec * scalar);
    return;
}

void BigFan::control() {
}

void BigFan::start() {
    if (isNerve(&NrvBigFan::BigFanNrvStop::sInstance)) {
        setNerve(&NrvBigFan::BigFanNrvStart::sInstance);
    }
}

void BigFan::exeStop() {
    if (MR::isFirstStep(this)) {
        MR::startAction(this, "Appear");
        MR::stopBck(this);
    }
}

void BigFan::exeStart() {
    if (MR::isFirstStep(this)) {
        mWindModel->appear();
        MR::startAction(this, "Appear");
        MR::startAction(mWindModel, "Appear");

        if (mIsTeresaGalaxy) {
            MR::startSound(this, "SE_OJ_BIG_FAN_START_FAR");
        } else {
            MR::startSound(this, "SE_OJ_BIG_FAN_START");
        }
    }

    if (MR::isActionEnd(this)) {
        setNerve(&NrvBigFan::BigFanNrvWait::sInstance);
    }
}

void BigFan::exeWait() {
    if (MR::isFirstStep(this)) {
        MR::startAction(this, "Wait");
        MR::startAction(mWindModel, "Wait");
    }

    if (mIsTeresaGalaxy) {
        MR::startLevelSound(this, "SE_OJ_LV_BIG_FAN_FAR");
    } else {
        MR::startLevelSound(this, "SE_OJ_LV_BIG_FAN");
    }
}

inline bool BigFan::isStartOrWait() {
    bool flag = false;
    if (isNerve(&NrvBigFan::BigFanNrvStart::sInstance) || isNerve(&NrvBigFan::BigFanNrvWait::sInstance)) {
        flag = true;
    }

    return flag;
}
