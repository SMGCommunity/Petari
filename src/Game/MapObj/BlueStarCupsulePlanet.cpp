#include "Game/MapObj/BlueStarCupsulePlanet.hpp"
#include "Game/LiveActor/LiveActor.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/MapObj/GCapture.hpp"
#include "Game/MapObj/MapPartsRailMover.hpp"
#include "Game/MapObj/MapPartsRotator.hpp"
#include "Game/Util/ActorMovementUtil.hpp"
#include "Game/Util/ActorSensorUtil.hpp"
#include "Game/Util/ActorSwitchUtil.hpp"
#include "Game/Util/BaseMatrixFollowTargetHolder.hpp"
#include "Game/Util/DemoUtil.hpp"
#include "Game/Util/EffectUtil.hpp"
#include "Game/Util/Functor.hpp"
#include "Game/Util/JMapUtil.hpp"
#include "Game/Util/JointUtil.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/ObjUtil.hpp"
#include "Game/Util/PlayerUtil.hpp"
#include "Game/Util/SoundUtil.hpp"
#include "Game/Util/StarPointerUtil.hpp"
#include "Game/Util/StringUtil.hpp"
#include "JSystem/JGeometry/TMatrix.hpp"
#include "JSystem/JGeometry/TVec.hpp"
#include "JSystem/JMath/JMath.hpp"
#include "revolution/mtx.h"
#include "revolution/types.h"

namespace {
    static const BlueStarCupsulePlanetParam sParams[] = {{"BlueStarCupsulePlanet", 500.0f}, {"UFOBlueStarCupsule", 200.0f}};

    const BlueStarCupsulePlanetParam* getParam(const char* pName) {
        for (u32 i = 0; i < ARRAY_SIZE(sParams); i++) {
            if (MR::isEqualString(pName, ::sParams[i].mBlueStarName)) {
                return &sParams[i];
            }
        }

        return nullptr;
    }
};  // namespace

BlueStarCupsulePlanet::BlueStarCupsulePlanet(const char* pName) : LiveActor(pName) {
    mRailMover = nullptr;
    mRotator = nullptr;
    mName = nullptr;
    mIsStepUpdatable = false;
    MR::createGCapture();
}

namespace NrvBlueStarCupsulePlanet {
    NEW_NERVE(BlueStarCupsulePlanetNrvWait, BlueStarCupsulePlanet, Wait);
    NEW_NERVE(BlueStarCupsulePlanetNrvPointable, BlueStarCupsulePlanet, Pointable);
    NEW_NERVE(BlueStarCupsulePlanetNrvHitPointer, BlueStarCupsulePlanet, HitPointer);
    NEW_NERVE(BlueStarCupsulePlanetNrvActive, BlueStarCupsulePlanet, Active);
};  // namespace NrvBlueStarCupsulePlanet

BlueStarCupsulePlanet::~BlueStarCupsulePlanet() {
}

void BlueStarCupsulePlanet::init(const JMapInfoIter& rIter) {
    MR::initDefaultPos(this, rIter);
    MR::getObjectName(&mName, rIter);
    initModelManagerWithAnm(mName, nullptr, false);
    MR::connectToScene(this, 29, 1, 4, -1);
    initEffectKeeper(0, nullptr, false);
    initSound(4, false);
    initNerve(&NrvBlueStarCupsulePlanet::BlueStarCupsulePlanetNrvWait::sInstance);
    initHitSensor(1);
    TVec3f offsetPointer, vec, offsetMap;
    TPos3f pos;
    offsetMap.x = 0.0f;
    offsetMap.y = 0.0f;
    offsetMap.z = 0.0f;
    MR::addHitSensorMapObj(this, "body", 16, 0.0f, offsetMap);
    MR::initCollisionParts(this, mName, getSensor("body"), nullptr);
    MR::copyJointPos(this, "Root", &vec);
    JMath::gekko_ps_copy12(&pos, getBaseMtx());
    pos.invert(pos);
    pos.mult(vec, offsetPointer);
    MR::initStarPointerTarget(this, ::getParam(mName)->mRadius, offsetPointer);
    MR::setClippingTypeSphere(this, 600.0f);

    if (MR::isConnectedWithRail(rIter)) {
        initRailRider(rIter);
        mRailMover = new MapPartsRailMover(this);
        mRailMover->init(rIter);
        mRailMover->start();
    }

    mRotator = new MapPartsRotator(this);
    mRotator->init(rIter);
    mRotator->start();
    MR::addBaseMatrixFollowTarget(this, rIter, nullptr, nullptr);

    if (MR::useStageSwitchReadB(this, rIter)) {
        MR::listenStageSwitchOnOffB(this, MR::Functor(this, &BlueStarCupsulePlanet::activateMapPartsFunction),
                                    MR::Functor(this, &BlueStarCupsulePlanet::deactivateMapPartsFunction));
    } else if (mRailMover || mRotator) {
        activateMapPartsFunction();
    }

    makeActorAppeared();
    if (MR::useStageSwitchReadAppear(this, rIter)) {
        MR::syncStageSwitchAppear(this);
        makeActorDead();
    }

    if (MR::tryRegisterDemoCast(this, rIter) && MR::isRegisteredDemoActionAppear(this)) {
        makeActorDead();
    }

    MR::startAllAnim(this, "Wait");
}

void BlueStarCupsulePlanet::control() {
    updatePose();
    mIsStepUpdatable = false;
}

void BlueStarCupsulePlanet::startClipped() {
    LiveActor::startClipped();
    MR::deleteEffectAll(this);
}

void BlueStarCupsulePlanet::endClipped() {
    LiveActor::endClipped();
    emitNerveEffect();
}

void BlueStarCupsulePlanet::updatePose() {
    if (!mIsStepUpdatable) {
        if (mRailMover) {
            mRailMover->movement();
            if (mRailMover->isWorking()) {
                if (MR::isEqualString(mName, "UFOBlueStarCupsule")) {
                    MR::startLevelSound(this, "SE_OJ_LV_UFO_BLUE_STAR_MOVE");
                }
                mPosition.x = mRailMover->_28.x;
                mPosition.y = mRailMover->_28.y;
                mPosition.z = mRailMover->_28.z;
            }
        }

        if (mRotator) {
            mRotator->movement();
        }
        calcAnim();
        mIsStepUpdatable = true;
    }
}

void BlueStarCupsulePlanet::decidedTarget() {
    if (!isNerve(&NrvBlueStarCupsulePlanet::BlueStarCupsulePlanetNrvActive::sInstance)) {
        setNerve(&NrvBlueStarCupsulePlanet::BlueStarCupsulePlanetNrvActive::sInstance);
    }
}

void BlueStarCupsulePlanet::releasedTarget() {
    if (!isNerve(&NrvBlueStarCupsulePlanet::BlueStarCupsulePlanetNrvWait::sInstance)) {
        setNerve(&NrvBlueStarCupsulePlanet::BlueStarCupsulePlanetNrvWait::sInstance);
        MR::startAllAnim(this, "Wait");
    }
}

void BlueStarCupsulePlanet::getTargetPosition(TVec3f* pPos) {
    MR::copyJointPos(this, "Root", pPos);
}

bool BlueStarCupsulePlanet::isReleaseForce() const {
    return MR::isNearPlayer(this, 0.0f);
}

f32 BlueStarCupsulePlanet::getPointableRange() const {
    return 3400.0f;
}

void BlueStarCupsulePlanet::exeWait() {
    if (MR::isFirstStep(this)) {
        MR::validateClipping(this);
        emitNerveEffect();
    }
    updatePose();

    if (isPointable()) {
        setNerve(&NrvBlueStarCupsulePlanet::BlueStarCupsulePlanetNrvPointable::sInstance);
    }
}

void BlueStarCupsulePlanet::exePointable() {
    if (MR::isFirstStep(this)) {
        emitNerveEffect();
    }
    MR::noticeInTouchableRange();
    updatePose();

    if (!isPointable()) {
        setNerve(&NrvBlueStarCupsulePlanet::BlueStarCupsulePlanetNrvWait::sInstance);
    } else if (MR::isStarPointerPointing(this, 0, true, "弱")) {
        if (MR::requestGCaptureTarget(this)) {
            MR::invalidateClipping(this);
            setNerve(&NrvBlueStarCupsulePlanet::BlueStarCupsulePlanetNrvHitPointer::sInstance);
        }
    }
}

void BlueStarCupsulePlanet::exeHitPointer() {
    if (!MR::isRequestedGCaptureTarget(this)) {
        setNerve(&NrvBlueStarCupsulePlanet::BlueStarCupsulePlanetNrvWait::sInstance);
    } else {
        if (MR::isFirstStep(this)) {
            emitNerveEffect();
        }

        if (MR::isStep(this, 3)) {
            MR::startCSSound("CS_STAR_POWER", nullptr, 0);
        }

        updatePose();
        MR::noticeInTouchableRange();
        MR::startLevelSound(this, "SE_OJ_LV_MAGIC_PNT_G_POINT");

        bool pointable = isPointable();
        if (MR::isStarPointerPointing(this, 0, true, "弱") && pointable) {
            MR::requestGCaptureTarget(this);
        }

        if (!pointable) {
            MR::unrequestGCaptureTarget(this);
        }
    }
}

void BlueStarCupsulePlanet::exeActive() {
    if (MR::isFirstStep(this)) {
        emitNerveEffect();
        MR::startAllAnim(this, "React");
        MR::startSound(this, "SE_OJ_MAGIC_PNT_G_ON");
    }

    updatePose();
    MR::noticeInTouchableRange();
    MR::startLevelSound(this, "SE_OJ_LV_MAGIC_PNT_G_PULL", MR::calcDistanceToPlayer(this));
    MR::startLevelSound(this, "SE_OJ_LV_MAGIC_PNT_G_PULL");
}

void BlueStarCupsulePlanet::calcAndSetBaseMtx() {
    if (mRotator && mRotator->isWorking()) {
        TPos3f mtx;
        mtx.identity();
        JMath::gekko_ps_copy12(mtx, mRotator->getRotateMtx());
        mtx[0][3] = mPosition.x;
        mtx[1][3] = mPosition.y;
        mtx[2][3] = mPosition.z;
        MR::setBaseTRMtx(this, mtx);
    } else {
        LiveActor::calcAndSetBaseMtx();
    }
}

bool BlueStarCupsulePlanet::isPointable() const {
    TVec3f pos, vec1;
    MR::copyJointPos(this, "Root", &pos);
    f32 distance = PSVECDistance(pos, MR::getPlayerPos());

    if (MR::isEqualString(mName, "UFOBlueStarCupsule")) {
        MR::calcUpVec(&vec1, this);
        TVec3f vec2 = *MR::getPlayerPos() - mPosition;
        if (vec1.dot(vec2) <= 0.0f) {
            return false;
        }
    }

    return 0.0f < distance && distance < 3400.0f;
}

void BlueStarCupsulePlanet::emitNerveEffect() {
    if (isNerve(&NrvBlueStarCupsulePlanet::BlueStarCupsulePlanetNrvWait::sInstance)) {
        MR::deleteEffect(this, "TargetLight");
        MR::deleteEffect(this, "Touch");
        MR::deleteEffect(this, "Active");
    } else if (isNerve(&NrvBlueStarCupsulePlanet::BlueStarCupsulePlanetNrvPointable::sInstance)) {
        MR::emitEffect(this, "TargetLight");
        MR::emitEffect(this, "TouchAble");
        MR::deleteEffect(this, "Touch");
        MR::deleteEffect(this, "Active");
    } else if (isNerve(&NrvBlueStarCupsulePlanet::BlueStarCupsulePlanetNrvHitPointer::sInstance)) {
        MR::emitEffect(this, "TargetLight");
        MR::emitEffect(this, "Touch");
        MR::deleteEffect(this, "Active");
    } else if (isNerve(&NrvBlueStarCupsulePlanet::BlueStarCupsulePlanetNrvActive::sInstance)) {
        MR::emitEffect(this, "TargetLight");
        MR::deleteEffect(this, "Touch");
        MR::emitEffect(this, "Active");
    }
}

void BlueStarCupsulePlanet::activateMapPartsFunction() {
    if (mRailMover) {
        mRailMover->mIsActive = true;
    }

    if (mRotator) {
        mRotator->mIsActive = true;
    }
}

void BlueStarCupsulePlanet::deactivateMapPartsFunction() {
    if (mRailMover) {
        mRailMover->mIsActive = false;
    }

    if (mRotator) {
        mRotator->mIsActive = false;
    }
}

bool GCaptureTargetable::canEndHold() const {
    return true;
}

bool GCaptureTargetable::isReleaseForce() const {
    return false;
}

f32 GCaptureTargetable::releaseDistance() const {
    return -1.0f;
}

f32 GCaptureTargetable::getPointableRange() const {
    return 3000.0f;
}
