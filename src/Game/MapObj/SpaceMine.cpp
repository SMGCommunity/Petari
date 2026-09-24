#include "Game/MapObj/SpaceMine.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/MapObj/MapObjActorInitInfo.hpp"
#include "Game/Util.hpp"

namespace {
    const f32 sRotateSpeedNormal = 1.0f;
    const f32 sRotateSpeedFast = 1.0f;
    // sDeadTime
    // sAppearValidDistance
    const f32 sSensorRadius = 100.0f;
    const f32 sDistanceNear = 1500.0f;
    const f32 sDistanceFar = 1450.0f;
    const s32 sAppearEffectTime = 120;
}  // namespace

namespace NrvSpaceMine {
    NEW_NERVE(HostTypeWait, SpaceMine, Wait);
    NEW_NERVE(HostTypeAppear, SpaceMine, Appear);
};  // namespace NrvSpaceMine

SpaceMine::SpaceMine(const char* pName)
    : MapObjActor(pName), mRotateSpeed(::sRotateSpeedNormal), mClippingRange(gZeroVec), mShadowType(-1), mIsPlayerNear() {
}

void SpaceMine::init(const JMapInfoIter& rIter) {
    MapObjActor::init(rIter);
    MapObjActorInitInfo info;
    info.setupHioNode("地形オブジェ");
    info.setupDefaultPos();
    info.setupConnectToScene();
    info.setupEffect(nullptr);
    info.setupSound(2);
    info.setupNerve(GET_NERVE(SpaceMine, HostTypeWait));
    info.setupRailMover();
    info.setupHitSensor();
    TVec3f sensorOffs;
    sensorOffs.x = 0.0f;
    sensorOffs.y = 0.0f;
    sensorOffs.z = 0.0f;
    info.setupHitSensorParam(8, ::sSensorRadius, sensorOffs);
    info.setupGroupClipping(0x10);
    MR::getJMapInfoArg0NoInit(rIter, &mShadowType);

    bool isValid = isShadowValid(mShadowType);

    if (isValid) {
        info.setupShadow(nullptr);
    }

    s32 arg1 = -1;
    MR::getJMapInfoArg1NoInit(rIter, &arg1);

    if (arg1 != -1) {
        info.setupBinder(::sSensorRadius, 0.0f);
    }

    initialize(rIter, info);

    if (isCalcShadowAlways()) {
        MR::onCalcShadow(this, nullptr);
        MR::onCalcShadowDropGravity(this, nullptr);
    }
}

bool SpaceMine::isCalcShadowAlways() const {
    return mShadowType == 1 || (isShadowValid(mShadowType) && MR::isExistRail(this));
}

void SpaceMine::kill() {
    MR::emitEffect(this, "Explosion");
    MR::startSound(this, "SE_OJ_SPACEMINE_EXPLOSION");
    MR::tryRumblePad(this, "中", 0);
    MapObjActor::kill();
}

void SpaceMine::initAfterPlacement() {
    bool isShadow = isShadowValid(mShadowType);

    if (isShadow) {
        MR::setClippingRangeIncludeShadow(this, &mClippingRange, ::sSensorRadius);
    }
}

void SpaceMine::attackSensor(HitSensor* pSender, HitSensor* pReceiver) {
    if (MR::sendMsgEnemyAttackExplosion(pReceiver, pSender)) {
        kill();
    } else {
        MR::sendMsgPush(pReceiver, pSender);
    }
}

bool SpaceMine::receiveMsgPlayerAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    if (MR::isMsgInvincibleAttack(msg)) {
        kill();
        return true;
    }

    if (MR::isMsgStarPieceAttack(msg)) {
        kill();
        return true;
    }

    return false;
}

void SpaceMine::exeWait() {
    if (isCalcShadowAlways()) {
        MR::setClippingRangeIncludeShadow(this, &mClippingRange, ::sSensorRadius);
    }

    f32 playerDist = MR::calcDistanceToPlayer(this);
    if (mIsPlayerNear) {
        if (::sDistanceFar < playerDist) {
            mIsPlayerNear = 0;
            mRotateSpeed = ::sRotateSpeedNormal;
        }
    } else if (playerDist < ::sDistanceNear) {
        mIsPlayerNear = 1;
        mRotateSpeed = ::sRotateSpeedFast;
    }

    if (MR::isBinded(this)) {
        if (MR::isBindedGround(this)) {
            MR::sendMsgEnemyAttackExplosionToBindedSensor(this, MR::getGroundSensor(this));
        }

        if (MR::isBindedWall(this)) {
            MR::sendMsgEnemyAttackExplosionToBindedSensor(this, MR::getWallSensor(this));
        }

        if (MR::isBindedRoof(this)) {
            MR::sendMsgEnemyAttackExplosionToBindedSensor(this, MR::getRoofSensor(this));
        }

        MR::offBind(this);
        kill();
    } else {
        mRotation.y += mRotateSpeed;
        mRotation.y = MR::repeatDegree(mRotation.y);
    }
}

void SpaceMine::exeAppear() {
    if (MR::isFirstStep(this)) {
        MR::emitEffect(this, "Appear");
        MR::setClippingRangeIncludeShadow(this, &mClippingRange, ::sSensorRadius);
    }

    MR::startLevelSound(this, "SE_OJ_LV_SPACEMINE_APPEAR");

    if (MR::isStep(this, ::sAppearEffectTime)) {
        MR::startBck(this, "Appear");
        MR::showModel(this);
        MR::startSound(this, "SE_OJ_SPACEMINE_APPEAR");
    }

    if (MR::isGreaterStep(this, ::sAppearEffectTime)) {
        if (MR::isBckStopped(this)) {
            setNerve(GET_NERVE(SpaceMine, HostTypeWait));
        }
    }
}

SpaceMine::~SpaceMine() {
}
