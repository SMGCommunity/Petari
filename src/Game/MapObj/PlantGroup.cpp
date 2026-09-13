#include "Game/MapObj/PlantGroup.hpp"
#include "Game/LiveActor/HitSensor.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/Scene/SceneFunction.hpp"
#include "Game/Util.hpp"
#include "JSystem/JMath/JMATrigonometric.hpp"

namespace {
    const s32 hIsShakableStep = 20;
    const f32 sSize = 160.0f;
    const f32 sCollisionRadius = 50.0f;
    // sWindRadius
    // sMaxAnimRate
    // sMinAnimRate
    const f32 sCheckLineLength = 1000.0f;
    const f32 sCoinVel = 30.0f;
    const f32 sDPDTouchRadius = 100.0f;
    const f32 sScratchVel = 6.0f;
    const s32 hGenerateItemTouch = 1;
    const s32 sHintEffectEmitInterval = 300;
    const f32 hShakeStartFrame = 3.0f;
    const f32 hWeakRate = 0.5f;
    const f32 hMiddleRate = 1.0f;
    const f32 hStrongRate = 1.5f;
    const f32 hStrongDist = 100.0f;
    const f32 hMiddleDist = 300.0f;
    const f32 hWeakDist = 500.0f;
}  // namespace

namespace NrvPlantMember {
    NEW_NERVE(HostTypeNrvWait, PlantMember, Wait);
    NEW_NERVE(HostTypeNrvHint, PlantMember, Hint);
    NEW_NERVE(HostTypeNrvShakeWeak, PlantMember, ShakeWeak);
    NEW_NERVE(HostTypeNrvShakeMiddle, PlantMember, ShakeMiddle);
    NEW_NERVE(HostTypeNrvShakeStrong, PlantMember, ShakeStrong);
}  // namespace NrvPlantMember

PlantGroup::PlantGroup(const char* pName)
    : LiveActor(pName), mMembers(), mMemberCount(7), mPlantType(), mIsStarPiece(), mHintPosition(0.0f), mHintRotation(0.0f),
      mHintTimer(sHintEffectEmitInterval), mHintIndex() {
}

void PlantGroup::init(const JMapInfoIter& rIter) {
    MR::connectToScene(this, MR::MovementType_MapObj, -1, -1, -1);
    MR::initDefaultPos(this, rIter);
    const char* name = nullptr;
    MR::getObjectName(&name, rIter);

    if (MR::isEqualString(name, "FlowerGroup")) {
        mPlantType = 1;
    } else if (MR::isEqualString(name, "FlowerBlueGroup")) {
        mPlantType = 2;
    } else {
        mPlantType = 0;
    }

    s32 count = -1;
    MR::getJMapInfoArg0NoInit(rIter, &count);

    if (count > 0) {
        mMemberCount = count;
    } else if (count == -1) {
        mMemberCount = 7;
    }

    s32 itemCount = 0;
    MR::getJMapInfoArg1NoInit(rIter, &itemCount);

    if (itemCount < 0) {
        itemCount = 0;
    }

    s32 itemType = -1;
    MR::getJMapInfoArg2NoInit(rIter, &itemType);

    if (itemType == 1) {
        mIsStarPiece = true;
        MR::declareStarPiece(this, itemCount);
    } else {
        mIsStarPiece = false;
        MR::declareCoin(this, itemCount);
    }

    initMember(itemCount, rIter);
    initSound(4, false);
    initEffectKeeper(0, "Bush", false);
    MR::setEffectHostSRT(this, "HintShakeLeaf", &mHintPosition, &mHintRotation, nullptr);
    MR::initStarPointerTarget(this, 0.0f, TVec3f(0.0f));
    MR::useStageSwitchSleep(this, rIter);
    MR::useStageSwitchReadAppear(this, rIter);

    if (MR::isValidSwitchAppear(this)) {
        makeActorDead();
        MR::syncStageSwitchAppear(this);
    } else {
        makeActorAppeared();
    }

    MR::tryRegisterDemoCast(this, rIter);
    mHintTimer = MR::getRandom(3L, 10L) * 10;
}

void PlantGroup::makeActorAppeared() {
    LiveActor::makeActorAppeared();

    for (s32 i = 0; i < mMemberCount; i++) {
        mMembers[i]->makeActorAppeared();
    }
}

void PlantGroup::makeActorDead() {
    LiveActor::makeActorDead();

    for (s32 i = 0; i < mMemberCount; i++) {
        mMembers[i]->makeActorDead();
    }
}

void PlantGroup::initMember(s32 itemCount, const JMapInfoIter&) {
    mMembers = new PlantMember*[mMemberCount];

    for (s32 i = 0; i < mMemberCount; i++) {
        if (mPlantType == 1) {
            mMembers[i] = new PlantMember("花", "Flower", false, nullptr);
        } else if (mPlantType == 2) {
            mMembers[i] = new PlantMember("青い花", "FlowerBlue", false, nullptr);
        } else {
            mMembers[i] = new PlantMember("草", "CutBush", true, nullptr);
        }

        mMembers[i]->initWithoutIter();
        MR::invalidateClipping(mMembers[i]);

        if (i < itemCount) {
            mMembers[i]->mHasItem = true;
        } else {
            mMembers[i]->mHasItem = false;
        }
    }

    for (s32 i = 0; i < mMemberCount; i++) {
        s32 index = MR::getRandom(0L, i + 1);
        bool hasItem = mMembers[i]->mHasItem;
        mMembers[i]->mHasItem = mMembers[index]->mHasItem;
        mMembers[index]->mHasItem = hasItem;
    }

    initHitSensor(1);
    MR::addHitSensorMapObj(this, "境界球", 16, 100.0f, TVec3f(gZeroVec));
}

s32 PlantGroup::placeOnCollisionFormCircle(TVec3f* pCenter, const TVec3f& rGravity, const TVec3f& rAxisX, const TVec3f& rAxisY) {
    pCenter->set(0, 0, 0);
    s32 ring = 0;
    f32 angle = 0.0f;
    f32 angleStep = 2.0f * PI;
    f32 radius = sSize * ring;
    s32 placedCount = 0;

    for (s32 i = 0; i < mMemberCount; i++) {
        TVec3f offset(rAxisX);
        offset.scale(JMACosRadian(angle));
        offset += rAxisY * JMASinRadian(angle);
        offset.scale(radius);
        TVec3f start(mPosition);
        start += offset;
        start -= rGravity * 100.0f;

        if (MR::getFirstPolyOnLineToMap(&mMembers[i]->mPosition, nullptr, start, rGravity * sCheckLineLength)) {
            *pCenter += mMembers[i]->mPosition;
            placedCount++;
        } else {
            mMembers[i]->kill();
        }

        mMembers[i]->initPosture();
        angle += angleStep;

        if (angle >= 2.0f * PI) {
            ring++;
            angle = 0.0f;
            angleStep = 2.0f * PI / (ring * 6);
            radius = sSize * ring;
        }
    }

    pCenter->scale(1.0f / placedCount);
    return placedCount;
}

f32 PlantGroup::calcBoundingSphereRadius(const TVec3f& rCenter) {
    TVec3f min(rCenter);
    TVec3f max(rCenter);

    for (s32 i = 0; i < mMemberCount; i++) {
        if (!MR::isDead(mMembers[i])) {
            const TVec3f& pos = mMembers[i]->mPosition;

            if (pos.x < min.x) {
                min.x = pos.x;
            } else if (max.x < pos.x) {
                max.x = pos.x;
            }

            if (pos.y < min.y) {
                min.y = pos.y;
            } else if (max.y < pos.y) {
                max.y = pos.y;
            }

            if (pos.z < min.z) {
                min.z = pos.z;
            } else if (max.z < pos.z) {
                max.z = pos.z;
            }

            MR::isNoCalcAnim(mMembers[i]);
            mMembers[i]->calcAnim();
        }
    }

    mPosition.set((max + min) / 2.0f);
    TVec3f size(max - min);

    if (size.x < size.y) {
        if (size.y < size.z) {
            return size.z / 2.0f;
        }

        return size.y / 2.0f;
    }

    if (size.x < size.z) {
        return size.z / 2.0f;
    }

    return size.x / 2.0f;
}

void PlantGroup::initAfterPlacement() {
    TVec3f gravity;
    TVec3f axisX;
    axisX.set(1, 0, 0);
    TVec3f axisY;
    axisY.set(0, 1, 0);
    MR::calcGravityVector(this, mPosition, &gravity, nullptr, 0);
    MR::makeAxisCrossPlane(&axisX, &axisY, gravity);
    TVec3f center;
    placeOnCollisionFormCircle(&center, gravity, axisX, axisY);
    f32 radius = calcBoundingSphereRadius(center);
    f32 scale = mScale.y;
    HitSensor* sensor = getSensor("境界球");
    sensor->mRadius = sSize + radius * scale;
    MR::setStarPointerTargetRadius3d(this, sSize + radius * scale);
    MR::setClippingTypeSphere(this, sSize + radius * scale);
}

void PlantGroup::control() {
    bool pointing = MR::isStarPointerPointing2POnPressButton(this, nullptr, false, false);

    for (s32 i = 0; i < mMemberCount; i++) {
        mMembers[i]->animControl(this);
        mMembers[i]->movement();

        if (pointing) {
            TVec2f velocity(*MR::getStarPointerScreenVelocity(1));

            if (sScratchVel * sScratchVel < velocity.squared()) {
                TVec3f pos(*MR::getStarPointerWorldPosUsingDepth(1));

                if (mMembers[i]->tryPush(pos, sDPDTouchRadius, 2) == true) {
                    MR::tryRumblePadVeryWeak(this, 1);
                    break;
                }
            }
        }
    }

    emitHintEffect();
}

void PlantGroup::emitHintEffect() {
    mHintTimer--;

    if (mHintTimer <= 0) {
        mHintTimer = sHintEffectEmitInterval;
        s32 index = (mMemberCount + mHintIndex) % mMemberCount;
        do {
            if (mMembers[index]->mHasItem && mMembers[index]->tryEmitHint()) {
                mHintIndex = (index + mMemberCount + 1) % mMemberCount;
                mHintPosition.set(mMembers[index]->mPosition);
                mHintRotation.set(mMembers[index]->mRotation);
                MR::emitEffect(this, "HintShakeLeaf");
                break;
            }

            index = (index + mMemberCount + 1) % mMemberCount;
        } while (index != mHintIndex);
    }
}

bool PlantGroup::receiveOtherMsg(u32 msg, HitSensor* pSender, HitSensor*) {
    if (msg == ACTMES_TORNADO_ATTACK || msg == ACTMES_TORNADO_STORM_RANGE || msg == ACTMES_SPIN_STORM_RANGE) {
        for (s32 i = 0; i < mMemberCount; i++) {
            mMembers[i]->tryShake(pSender);
        }
    }

    return false;
}

void PlantGroup::attackSensor(HitSensor* pSender, HitSensor* pReceiver) {
    if (MR::isSensorPlayer(pReceiver) || MR::isSensorEnemy(pReceiver)) {
        TVec3f velocity(pReceiver->mHost->mVelocity);
        MR::vecKillElement(velocity, pReceiver->mHost->mGravity, &velocity);

        if (velocity.squared() < 2.0f) {
            return;
        }

        s32 touchType = 0;

        if (MR::isSensorPlayer(pReceiver) || MR::isSensorNpc(pReceiver)) {
            touchType = 1;
        } else if (MR::sendArbitraryMsg(ACTMES_PLANT_GROUP_EMIT_ITEM, pReceiver, pSender)) {
            touchType = 1;
        }

        for (s32 i = 0; i < mMemberCount; i++) {
            mMembers[i]->tryPush(pReceiver->mPosition, pReceiver->mRadius, touchType);
        }
    }
}

void PlantGroup::startClipped() {
    LiveActor::startClipped();

    for (s32 i = 0; i < mMemberCount; i++) {
        mMembers[i]->startClipped();
    }
}

void PlantGroup::endClipped() {
    LiveActor::endClipped();

    for (s32 i = 0; i < mMemberCount; i++) {
        mMembers[i]->endClipped();
    }
}

void PlantMember::init(const JMapInfoIter&) {
    _90 = 1.0f;
    mTouchType = 3;
    _9C = false;
    mHasItem = false;
    MR::onCalcAnim(this);
    initNerve(GET_NERVE(PlantMember, HostTypeNrvWait));
    appear();
}

bool PlantMember::tryEmitHint() {
    if (isNerve(GET_NERVE(PlantMember, HostTypeNrvWait))) {
        setNerve(GET_NERVE(PlantMember, HostTypeNrvHint));
        return true;
    }

    return false;
}

void PlantMember::exeWait() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "Wait", nullptr);
    }
}

void PlantMember::exeHint() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "HintShake", nullptr);
    }

    if (MR::isBckStopped(this)) {
        setNerve(GET_NERVE(PlantMember, HostTypeNrvWait));
    }
}

void PlantMember::exeShakeWeak() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "Shake", nullptr);
        MR::setBckFrame(this, hShakeStartFrame);
        MR::startSound(this, "SE_OJ_LEAVES_SWING");
        MR::setBckRate(this, hWeakRate);
    }

    if (MR::isBckStopped(this)) {
        setNerve(GET_NERVE(PlantMember, HostTypeNrvWait));
    }
}

void PlantMember::exeShakeMiddle() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "Shake", nullptr);
        MR::setBckFrame(this, hShakeStartFrame);
        MR::startSound(this, "SE_OJ_LEAVES_SWING");
        MR::setBckRate(this, hMiddleRate);
    }

    if (MR::isBckStopped(this)) {
        setNerve(GET_NERVE(PlantMember, HostTypeNrvWait));
    }
}

void PlantMember::exeShakeStrong() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "Shake", nullptr);
        MR::setBckFrame(this, hShakeStartFrame);
        MR::startSound(this, "SE_OJ_LEAVES_SWING");
        MR::setBckRate(this, hStrongRate);
    }

    if (MR::isBckStopped(this)) {
        setNerve(GET_NERVE(PlantMember, HostTypeNrvWait));
    }
}

bool PlantMember::generateItem(PlantGroup* pGroup) {
    if (mHasItem == true) {
        TVec3f velocity(-mGravity);
        velocity.scale(sCoinVel);

        if (pGroup->mIsStarPiece) {
            MR::startSound(pGroup, "SE_OJ_STAR_PIECE_BURST");
            MR::appearStarPiece(pGroup, mPosition, 1, 10.0f, 40.0f, false);
        } else {
            MR::appearCoinPop(pGroup, mPosition, 1);
        }

        mHasItem = false;
        return true;
    }

    return false;
}

void PlantMember::initPosture() {
    MR::calcGravityVector(this, &mGravity, nullptr, 0);
    TVec3f axis;
    axis.set(1, 0, 0);

    if (MR::isSameDirection(mGravity, axis, 0.01f)) {
        axis.set(0, 1, 0);
    }

    TPos3f posture;
    MR::calcMtxFromGravityAndZAxis(&posture, this, mGravity, axis);
    TVec3f euler;
    TPos3f rotation;
    f32 angle = MR::getRandom(-PI, PI);
    rotation.makeRotate(mGravity, angle);
    posture.concat(rotation, posture);
    posture.getEuler(euler);
    mRotation.x = _180_PI * euler.x;
    mRotation.y = _180_PI * euler.y;
    mRotation.z = _180_PI * euler.z;
    calcAnim();
}

bool PlantMember::tryShake(HitSensor* pSensor) {
    if (MR::isDead(this)) {
        return false;
    }

    if ((isNerve(GET_NERVE(PlantMember, HostTypeNrvShakeWeak)) || isNerve(GET_NERVE(PlantMember, HostTypeNrvShakeMiddle)) ||
         isNerve(GET_NERVE(PlantMember, HostTypeNrvShakeStrong))) &&
        MR::isLessStep(this, hIsShakableStep)) {
        return false;
    }

    TVec3f diff(pSensor->mPosition - mPosition);
    MR::vecKillElement(diff, mGravity, &diff);
    f32 distance = diff.length();

    if (distance < hStrongDist) {
        setNerve(GET_NERVE(PlantMember, HostTypeNrvShakeStrong));
    } else if (distance < hMiddleDist) {
        setNerve(GET_NERVE(PlantMember, HostTypeNrvShakeMiddle));
    } else if (distance < hWeakDist) {
        setNerve(GET_NERVE(PlantMember, HostTypeNrvShakeWeak));
    }

    return true;
}

bool PlantMember::tryPush(const TVec3f& rPosition, f32 radius, s32 touchType) {
    if (MR::isDead(this)) {
        return false;
    }

    if ((isNerve(GET_NERVE(PlantMember, HostTypeNrvShakeWeak)) || isNerve(GET_NERVE(PlantMember, HostTypeNrvShakeMiddle)) ||
         isNerve(GET_NERVE(PlantMember, HostTypeNrvShakeStrong))) &&
        MR::isLessStep(this, hIsShakableStep)) {
        return false;
    }

    TVec3f diff(rPosition - mPosition);
    radius += sCollisionRadius * mScale.y;
    radius *= radius;

    if (diff.squared() < radius) {
        mTouchType = touchType;

        if (isNerve(GET_NERVE(PlantMember, HostTypeNrvWait)) || isNerve(GET_NERVE(PlantMember, HostTypeNrvHint))) {
            setNerve(GET_NERVE(PlantMember, HostTypeNrvShakeWeak));
        } else if (isNerve(GET_NERVE(PlantMember, HostTypeNrvShakeWeak))) {
            setNerve(GET_NERVE(PlantMember, HostTypeNrvShakeMiddle));
        } else if (isNerve(GET_NERVE(PlantMember, HostTypeNrvShakeMiddle))) {
            setNerve(GET_NERVE(PlantMember, HostTypeNrvShakeStrong));
        } else if (isNerve(GET_NERVE(PlantMember, HostTypeNrvShakeStrong))) {
            setNerve(GET_NERVE(PlantMember, HostTypeNrvShakeStrong));
        }

        return true;
    }

    return false;
}

void PlantMember::animControl(PlantGroup* pGroup) {
    if (isNerve(GET_NERVE(PlantMember, HostTypeNrvShakeWeak))) {
        if (mTouchType == hGenerateItemTouch) {
            generateItem(pGroup);
        }
    } else if (isNerve(GET_NERVE(PlantMember, HostTypeNrvShakeMiddle)) || isNerve(GET_NERVE(PlantMember, HostTypeNrvShakeStrong))) {
        if (mTouchType == 1 || mTouchType == 2) {
            generateItem(pGroup);
        }
    }
}

PlantGroup::~PlantGroup() {
}

PlantMember::~PlantMember() {
}
