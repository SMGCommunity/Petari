#include "Game/MapObj/BeeFlowerHover.hpp"
#include "Game/LiveActor/LodCtrl.hpp"

BeeFlowerHover::BeeFlowerHover(const char *pName) : LiveActor(pName) {
    mLodCtrlPlanet = 0;
    _BC = 50.0f;
    mRailMover = 0;
    mRailPosture = 0;
    _CC.x = 0.0f;
    _CC.y = 0.0f;
    _CC.z = 0.0f;
    _8C.identity();
}

void BeeFlowerHover::init(const JMapInfoIter &rIter) {
    MR::initDefaultPos(this, rIter);
    initModelManagerWithAnm("BeeFlowerHover", 0, false);
    MR::connectToSceneMapObj(this);
    initHitSensor(1);
    MR::addBodyMessageSensorMapObj(this);
    MR::initCollisionPartsAutoEqualScale(this, "BeeFlowerHover", getSensor(0), 0);
    initEffectKeeper(0, 0, false);
    initSound(4, false);

    if (MR::isConnectedWithRail(rIter)) {
        initRailRider(rIter);
        mRailMover = new MapPartsRailMover(this);
        mRailMover->init(rIter);
        mRailMover->start();

        mRailPosture = new MapPartsRailPosture(this);
        mRailPosture->_48 = 2;
        mRailPosture->initWithoutIter();
        mRailMover->start();
    }

    MR::initShadowVolumeCylinder(this, 450.0f);
    MR::onShadowFollowHostScale(this, 0);

    if (!mRailMover) {
        MR::onCalcShadowOneTime(this, 0);
    }

    s32 drop_length = -1;
    MR::getJMapInfoArg7NoInit(rIter, &drop_length);
    if (drop_length != -1) {
        MR::setShadowDropLength(this, 0, drop_length);
    }

    if (mRailMover) {
        MR::initAndSetRailClipping(&_CC, this, 100.0f, 500.0f);
    }
    else {
        MR::setClippingTypeSphereContainsModelBoundingBox(this, 100.0f);
    }

    if (mRailMover) {
        MR::onCalcGravity(this);
    }
    else {
        MR::calcGravity(this);
    }

    MR::getJMapInfoArg0NoInit(rIter, &_BC);
    _BC /= 100.0f;
    mLodCtrlPlanet = MR::createLodCtrlPlanet(this, rIter, 100.0f, -1);
    mLodCtrlPlanet->validate();
    initNerve(&NrvBeeFlowerHover::BeeFlowerHoverNrvWait::sInstance);
    makeActorAppeared();
}

void BeeFlowerHover::exeSoftTouch() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "OnBee", 0);
        MR::startSound(this, "SE_OJ_BEE_FLOWER_BEE_ON", -1, -1);
    }

    if (MR::isBckStopped(this)) {
        setNerve(&NrvBeeFlowerHover::BeeFlowerHoverNrvSoftTouchWait::sInstance);
    }
}

void BeeFlowerHover::exeSoftTouchWait() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "Wait", 0);
    }

    if (!MR::isPlayerElementModeBee()) {
        setNerve(&NrvBeeFlowerHover::BeeFlowerHoverNrvHardTouch::sInstance);
    }
    else {
        
        if (MR::isOnPlayer(getSensor(0))) {
            setNerve(&NrvBeeFlowerHover::BeeFlowerHoverNrvWait::sInstance);
        }
    }
}

void BeeFlowerHover::exeHardTouch() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "ThroughMario", 0);
        MR::startSound(this, "SE_OJ_BEE_FLOWER_DISAPPEAR", -1, -1);
        MR::invalidateCollisionParts(this);
        MR::invalidateShadow(this, 0);
        MR::onCalcShadowOneTime(this, 0);
        MR::tryRumblePadMiddle(this, 0);
        MR::shakeCameraNormal();
    }

    if (MR::isBckStopped(this)) {
        MR::hideModel(this);
        setNerve(&NrvBeeFlowerHover::BeeFlowerHoverNrvRecover::sInstance);
    }
}

void BeeFlowerHover::exeRecover() {
    if (MR::isStep(this, 0x3C)) {
        MR::validateShadow(this, 0);
        MR::onCalcShadowOneTime(this, 0);
        MR::showModel(this);
        MR::startBck(this, "Appear", 0);
        MR::startSound(this, "SE_OJ_BEE_FLOWER_RECOVER", -1, -1);
    }

    if (MR::isGreaterEqualStep(this, 0x3C) && MR::isBckStopped(this) && !MR::isHiddenModel(this)) {
        MR::validateCollisionParts(this);
        setNerve(&NrvBeeFlowerHover::BeeFlowerHoverNrvWait::sInstance);   
    }
}

#ifdef NON_MATCHING
void BeeFlowerHover::control() { 
    if (!isNerve(&NrvBeeFlowerHover::BeeFlowerHoverNrvRecover::sInstance)) {
        mLodCtrlPlanet->update();
        
        if (mLodCtrlPlanet->isShowLowModel()) {
            return;
        }
    }

    f32 v5 = _BC;
    TVec3f v11;
    v5 = v5 * 0.017453292f;
    v11.y = 1.0f;
    v11.x = 0.0f;
    v11.z = 0.0f;

    TMtx34f v12; // stack_20
    v12.mMtx[0][3] = 0.0f;
    v12.mMtx[1][3] = 0.0f;
    v12.mMtx[2][3] = 0.0f;

    TVec3f v10;
    v10.set(v11);
    f32 mag = PSVECMag(v10.toCVec());
    PSVECNormalize(v10.toCVec(), v10.toVec());
    f32 v6 = sin(v5);
    f32 v7 = cos(v5);
    v12.mMtx[0][0] = v7 + ((1.0f - v7) * (v10.x * v10.x));
    v12.mMtx[1][1] = v7 + ((1.0f - v7) * (v10.y * v10.y));
    
    v12.mMtx[2][2] = v7 + ((1.0f - v7) * (v10.z * v10.z));

    //f32 val_3 = 
    
    v12.mMtx[0][1] = (v10.y * ((1.0f - v7) * v10.x)) - (v6 * v10.z);
    v12.mMtx[1][0] = (v10.y * ((1.0f - v7) * v10.x)) + (v6 * v10.z);
    v12.mMtx[0][2] = (v10.z * ((1.0f - v7) * v10.x)) + (v6 * v10.y);
    v12.mMtx[2][0] = (v10.z * ((1.0f - v7) * v10.x)) - (v6 * v10.y);
    v12.mMtx[1][2] = (v10.z * ((1.0f - v7) * v10.y)) - (v6 * v10.x);
    v12.mMtx[2][1] = (v10.z * ((1.0f - v7) * v10.y)) + (v6 * v10.x);
    _8C.concat(v12);

    if (mRailMover) {
        mRailMover->movement();
        mRailPosture->movement();
        mPosition.set(mRailMover->_28);
    }
}
#endif

void BeeFlowerHover::calcAndSetBaseMtx() {
    TPos3f stack_8;
    stack_8.identity();

    if (mRailPosture) {
        stack_8.concat(mRailPosture->_18);
    }
    else {
        MR::makeMtxRotate(stack_8.toMtxPtr(), mRotation);
    }

    stack_8.concat(_8C);
    stack_8.mMtx[0][3] = mPosition.x;
    stack_8.mMtx[1][3] = mPosition.y;
    stack_8.mMtx[2][3] = mPosition.z;
    MR::setBaseTRMtx(this, stack_8);
}

bool BeeFlowerHover::receiveOtherMsg(u32 msg, HitSensor *, HitSensor *) {
    if (!isNerve(&NrvBeeFlowerHover::BeeFlowerHoverNrvWait::sInstance)) {
        return 0;
    }

    if (MR::isMsgFloorTouch(msg)) {
        if (MR::isPlayerElementModeBee()) {
            setNerve(&NrvBeeFlowerHover::BeeFlowerHoverNrvSoftTouch::sInstance);
        }
        else {
            setNerve(&NrvBeeFlowerHover::BeeFlowerHoverNrvHardTouch::sInstance);
        }

        return 1;
    }

    return 0;
}

BeeFlowerHover::~BeeFlowerHover() {

}

namespace NrvBeeFlowerHover {
    INIT_NERVE(BeeFlowerHoverNrvWait);
    INIT_NERVE(BeeFlowerHoverNrvSoftTouch);
    INIT_NERVE(BeeFlowerHoverNrvSoftTouchWait);
    INIT_NERVE(BeeFlowerHoverNrvHardTouch);
    INIT_NERVE(BeeFlowerHoverNrvRecover);

    void BeeFlowerHoverNrvRecover::execute(Spine *pSpine) const {
        BeeFlowerHover* flower = reinterpret_cast<BeeFlowerHover*>(pSpine->mExecutor);
        flower->exeRecover();
    }

    void BeeFlowerHoverNrvHardTouch::execute(Spine *pSpine) const {
        BeeFlowerHover* flower = reinterpret_cast<BeeFlowerHover*>(pSpine->mExecutor);
        flower->exeHardTouch();
    }

    void BeeFlowerHoverNrvSoftTouchWait::execute(Spine *pSpine) const {
        BeeFlowerHover* flower = reinterpret_cast<BeeFlowerHover*>(pSpine->mExecutor);
        flower->exeSoftTouchWait();
    }

    void BeeFlowerHoverNrvSoftTouch::execute(Spine *pSpine) const {
        BeeFlowerHover* flower = reinterpret_cast<BeeFlowerHover*>(pSpine->mExecutor);
        flower->exeSoftTouch();
    }

    void BeeFlowerHoverNrvWait::execute(Spine *pSpine) const {
        BeeFlowerHover* flower = reinterpret_cast<BeeFlowerHover*>(pSpine->mExecutor);
        
        if (MR::isFirstStep(flower)) {
            MR::startBck(flower, "Wait", 0);
        }
    }
};
