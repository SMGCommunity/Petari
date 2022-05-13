#include "Game/MapObj/BigFan.h"
#include "Game/MapObj/BigFanHolder.h"
#include "Game/LiveActor/ModelObj.h"
#include "JSystem/JMath/JMath.h"

BigFan::BigFan(const char *pName) : LiveActor(pName) {
    mWindModel = 0;
    _90.x = 0.0f;
    _90.y = 0.0f;
    _90.z = 0.0f;
    mWindLength = 4000.0f;
    _A0 = 100.0f;
    mIsTeresaGalaxy = false;
    BigFanFunction::createBigFanHolder();
    BigFanFunction::registerBigFan(this);
}

void BigFan::init(const JMapInfoIter &rIter) {
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
    MR::getJMapInfoArg1NoInit(rIter, &_A0);
    initWindModel();
    TVec3f front;
    MR::calcFrontVec(&front, this);
    JMAVECScaleAdd(front.toCVec(), mPosition.toCVec(), _90.toVec(), 0.5f * mWindLength);
    MR::setClippingTypeSphere(this, 400.0f + mWindLength, &_90);
    initSound(4, false);

    if (MR::isEqualStageName("TeresaMario2DGalaxy")) {
        mIsTeresaGalaxy = true;
    }
    else {
        mIsTeresaGalaxy = false;
    }

    initNerve(&NrvBigFan::BigFanNrvWait::sInstance);
    if (MR::useStageSwitchReadAppear(this, rIter)) {
        void (BigFan::*startFunc)(void) = &BigFan::start;
        MR::listenStageSwitchOnAppear(this, MR::Functor(this, startFunc));
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

// BigFan::calcWindInfo

void BigFan::control() {

}

void BigFan::start() {
    if (isNerve(&NrvBigFan::BigFanNrvStop::sInstance)) {
        setNerve(&NrvBigFan::BigFanNrvStart::sInstance);
    }
}

void BigFan::exeStart() {
    if (MR::isFirstStep(this)) {
        mWindModel->appear();
        MR::startAction(this, "Appear");
        MR::startAction(mWindModel, "Appear");

        if (mIsTeresaGalaxy) {
            MR::startSound(this, "SE_OJ_BIG_FAN_START_FAR", -1, -1);
        }
        else {
            MR::startSound(this, "SE_OJ_BIG_FAN_START", -1, -1);
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
        MR::startLevelSound(this, "SE_OJ_LV_BIG_FAN_FAR", -1, -1, -1);
    }
    else {
        MR::startLevelSound(this, "SE_OJ_LV_BIG_FAN", -1, -1, -1);
    }
}

namespace NrvBigFan {
    INIT_NERVE(BigFanNrvStop);
    INIT_NERVE(BigFanNrvStart);
    INIT_NERVE(BigFanNrvWait);

    void BigFanNrvWait::execute(Spine *pSpine) const { 
        BigFan* fan = reinterpret_cast<BigFan*>(pSpine->mExecutor);
        fan->exeWait();
    }

    void BigFanNrvStart::execute(Spine *pSpine) const { 
        BigFan* fan = reinterpret_cast<BigFan*>(pSpine->mExecutor);
        fan->exeStart();
    }

    void BigFanNrvStop::execute(Spine *pSpine) const { 
        BigFan* fan = reinterpret_cast<BigFan*>(pSpine->mExecutor);
        
        if (MR::isFirstStep(fan)) {
            MR::startAction(fan, "Appear");
            MR::stopBck(fan);
        }
    }
};

BigFan::~BigFan() {
    
}