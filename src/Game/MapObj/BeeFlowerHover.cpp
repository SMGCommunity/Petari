#include "Game/MapObj/BeeFlowerHover.hpp"
#include "Game/LiveActor/LodCtrl.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/MapObj/MapPartsRailMover.hpp"
#include "Game/MapObj/MapPartsRailPosture.hpp"
#include "Game/Util.hpp"
#include "Game/Util/MathUtil.hpp"

void BeeFlowerHover_FORCE_MATCH_SDATA2() {
    (void)1.0f;
    (void)0.0f;
    (void)50.0f;
}

namespace {
    static const f32 sDefaultRotateSpeed = 50.0f;
    static const s32 sStepToRecoverStart = 60;
};  // namespace

namespace NrvBeeFlowerHover {
    NEW_NERVE(BeeFlowerHoverNrvWait, BeeFlowerHover, Wait);
    NEW_NERVE(BeeFlowerHoverNrvSoftTouch, BeeFlowerHover, SoftTouch);
    NEW_NERVE(BeeFlowerHoverNrvSoftTouchWait, BeeFlowerHover, SoftTouchWait);
    NEW_NERVE(BeeFlowerHoverNrvHardTouch, BeeFlowerHover, HardTouch);
    NEW_NERVE(BeeFlowerHoverNrvRecover, BeeFlowerHover, Recover);
};  // namespace NrvBeeFlowerHover

BeeFlowerHover::BeeFlowerHover(const char* pName)
    : LiveActor(pName), mLodCtrlPlanet(), _BC(::sDefaultRotateSpeed), mRailMover(), mRailPosture(), _CC(0.0f, 0.0f, 0.0f) {
    _8C.identity();
}

void BeeFlowerHover::init(const JMapInfoIter& rIter) {
    MR::initDefaultPos(this, rIter);
    initModelManagerWithAnm("BeeFlowerHover", 0, false);
    MR::connectToSceneMapObj(this);
    initHitSensor(1);
    MR::addBodyMessageSensorMapObj(this);
    MR::initCollisionPartsAutoEqualScale(this, "BeeFlowerHover", getSensor(nullptr), 0);
    initEffectKeeper(0, nullptr, false);
    initSound(4, false);

    if (MR::isConnectedWithRail(rIter)) {
        initRailRider(rIter);
        mRailMover = new MapPartsRailMover(this);
        mRailMover->init(rIter);
        mRailMover->start();

        mRailPosture = new MapPartsRailPosture(this);
        mRailPosture->mMovePosture = 2;
        mRailPosture->initWithoutIter();
        mRailPosture->start();
    }

    MR::initShadowVolumeCylinder(this, 450.0f);
    MR::onShadowFollowHostScale(this, 0);

    if (mRailMover == nullptr) {
        MR::onCalcShadowOneTime(this, 0);
    }

    s32 drop_length = -1;
    MR::getJMapInfoArg7NoInit(rIter, &drop_length);
    if (drop_length != -1) {
        MR::setShadowDropLength(this, 0, drop_length);
    }

    if (mRailMover != nullptr) {
        MR::initAndSetRailClipping(&_CC, this, 100.0f, 500.0f);
    } else {
        MR::setClippingTypeSphereContainsModelBoundingBox(this, 100.0f);
    }

    if (mRailMover != nullptr) {
        MR::onCalcGravity(this);
    } else {
        MR::calcGravity(this);
    }

    MR::getJMapInfoArg0NoInit(rIter, &_BC);
    _BC /= 100.0f;
    mLodCtrlPlanet = MR::createLodCtrlPlanet(this, rIter, 100.0f, -1);
    mLodCtrlPlanet->validate();
    initNerve(GET_NERVE(BeeFlowerHover, BeeFlowerHoverNrvWait));
    makeActorAppeared();
}

void BeeFlowerHover::exeWait() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "Wait");
    }
}

void BeeFlowerHover::exeSoftTouch() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "OnBee");
        MR::startSound(this, "SE_OJ_BEE_FLOWER_BEE_ON");
    }

    if (MR::isBckStopped(this)) {
        setNerve(GET_NERVE(BeeFlowerHover, BeeFlowerHoverNrvSoftTouchWait));
    }
}

void BeeFlowerHover::exeSoftTouchWait() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "Wait");
    }

    if (!MR::isPlayerElementModeBee()) {
        setNerve(GET_NERVE(BeeFlowerHover, BeeFlowerHoverNrvHardTouch));
    } else if (!MR::isOnPlayer(getSensor(nullptr))) {
        setNerve(GET_NERVE(BeeFlowerHover, BeeFlowerHoverNrvWait));
    }
}

void BeeFlowerHover::exeHardTouch() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "ThroughMario");
        MR::startSound(this, "SE_OJ_BEE_FLOWER_DISAPPEAR");
        MR::invalidateCollisionParts(this);
        MR::invalidateShadow(this, 0);
        MR::onCalcShadowOneTime(this, 0);
        MR::tryRumblePadMiddle(this, WPAD_CHAN0);
        MR::shakeCameraNormal();
    }

    if (MR::isBckStopped(this)) {
        MR::hideModel(this);
        setNerve(GET_NERVE(BeeFlowerHover, BeeFlowerHoverNrvRecover));
    }
}

void BeeFlowerHover::exeRecover() {
    if (MR::isStep(this, ::sStepToRecoverStart)) {
        MR::validateShadow(this, 0);
        MR::onCalcShadowOneTime(this, 0);
        MR::showModel(this);
        MR::startBck(this, "Appear");
        MR::startSound(this, "SE_OJ_BEE_FLOWER_RECOVER");
    }

    if (MR::isGreaterEqualStep(this, ::sStepToRecoverStart) && MR::isBckStopped(this) && !MR::isHiddenModel(this)) {
        MR::validateCollisionParts(this);
        setNerve(GET_NERVE(BeeFlowerHover, BeeFlowerHoverNrvWait));
    }
}

void BeeFlowerHover::control() {
    if (!isNerve(GET_NERVE(BeeFlowerHover, BeeFlowerHoverNrvRecover))) {
        mLodCtrlPlanet->update();

        if (mLodCtrlPlanet->isShowLowModel()) {
            return;
        }
    }

    TPos3f mtx;
    mtx.makeRotate(TVec3f(0.0f, 1.0f, 0.0f), MR::toRadian(_BC));
    _8C.concat(mtx);

    if (mRailMover != nullptr) {
        mRailMover->movement();
        mRailPosture->movement();
        mPosition.set(mRailMover->_28);
    }
}

void BeeFlowerHover::calcAndSetBaseMtx() {
    TPos3f stack_8;
    stack_8.identity();

    if (mRailPosture) {
        stack_8.concat(mRailPosture->_18);
    } else {
        MR::makeMtxRotate(stack_8.toMtxPtr(), mRotation);
    }

    stack_8.concat(_8C);
    stack_8.mMtx[0][3] = mPosition.x;
    stack_8.mMtx[1][3] = mPosition.y;
    stack_8.mMtx[2][3] = mPosition.z;
    MR::setBaseTRMtx(this, stack_8);
}

bool BeeFlowerHover::receiveOtherMsg(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    if (!isNerve(GET_NERVE(BeeFlowerHover, BeeFlowerHoverNrvWait))) {
        return false;
    }

    if (MR::isMsgFloorTouch(msg)) {
        if (MR::isPlayerElementModeBee()) {
            setNerve(GET_NERVE(BeeFlowerHover, BeeFlowerHoverNrvSoftTouch));
        } else {
            setNerve(GET_NERVE(BeeFlowerHover, BeeFlowerHoverNrvHardTouch));
        }

        return true;
    }

    return false;
}

BeeFlowerHover::~BeeFlowerHover() {
}
