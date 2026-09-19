#include "Game/Player/MarioAnimator.hpp"
#include "Game/Animation/XanimeCore.hpp"
#include "Game/Animation/XanimePlayer.hpp"
#include "Game/Animation/XanimeResource.hpp"
#include "Game/LiveActor/HitSensor.hpp"
#include "Game/LiveActor/ModelManager.hpp"
#include "Game/Player/J3DModelX.hpp"
#include "Game/Player/MarioActor.hpp"
#include "Game/Player/MarioAnimatorData.hpp"
#include "Game/Player/MarioConst.hpp"
#include "Game/Player/MarioState.hpp"
#include "Game/Util/ActorSensorUtil.hpp"
#include "Game/Util/DemoUtil.hpp"
#include "Game/Util/EffectUtil.hpp"
#include "Game/Util/JointUtil.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/MathUtil.hpp"
#include "Game/Util/ModelUtil.hpp"
#include "Game/Util/MtxUtil.hpp"
#include "Game/Util/StringUtil.hpp"
#include <JSystem/JMath/JMATrigonometric.hpp>
#include <cstring>

MarioAnimator::MarioAnimator(MarioActor* pActor) : MarioModule(pActor) {
    init();
}

void MarioAnimator_FORCE_MATCH_SDATA2() {
    (void)1.0f;
    (void)0.0f;
    (void)0.5f;
    (void)-1.0f;
    (void)3.14159274f;
    (void)1.57079637f;
    (void)79.0f;
    (void)59.0f;
    (void)0.00100000005f;
    (void)0.899999976f;
    (void)0.100000001f;
    (void)0.949999988f;
    (void)0.0500000007f;
    (void)0.25f;
    (void)0.970000029f;
    (void)0.0299999993f;
    (void)10.0f;
    (void)1.41421294f;
    (void)6.28318548f;
    (void)30.0f;
    (void)-2607.59448f;
    (void)2607.59448f;
    (void)0.699999988f;
    (void)60.0f;
    (void)5.0f;
    (void)-0.707000017f;
    (void)0.707000017f;
    (void)-0.5f;
}

void MarioAnimator::init() {
    XanimeSwapTable* luigiAnimations = nullptr;
    if (gIsLuigi) {
        luigiAnimations = luigiAnimeSwapTable;
    }

    mResourceTable = new XanimeResourceTable(MR::getResourceHolder(mActor), marioAnimeTable, marioAnimeAuxTable, marioAnimeOfsTable,
                                             reinterpret_cast< XanimeBckTable* >(singleAnimeTable), doubleAnimeTable, tripleAnimeTable,
                                             quadAnimeTable, luigiAnimations);

    _14 = 0;
    _15 = 0;
    _16 = 1;

    _18 = 0.0f;
    _1C = 0.0f;
    _20 = 0.0f;
    _24 = 1.0f;

    PSMTXIdentity(_28.toMtxPtr());
    PSMTXIdentity(_7C.toMtxPtr());
    PSMTXIdentity(_AC.toMtxPtr());

    _58 = 0.0f;
    _5C = 0.0f;
    _60.zero();

    _6C = false;
    _10C = 0;
    _10D = 0;
    mUpperDefaultSet = false;
    mCurrBck = nullptr;
    _118 = 0.0f;
    _70 = 0.0f;
    _74 = 0;
    _78 = 0;
    _110 = 0.0f;

    initCallbackTable();

    mXanimePlayer = new XanimePlayer(MR::getJ3DModel(mActor), mResourceTable);

    changeDefault("基本");
    change("基本");

    mXanimePlayer->getCore()->enableJointTransform(MR::getJ3DModelData(mActor));

    mActor->mModelManager->mXanimePlayer = mXanimePlayer;
    mXanimePlayerUpper = new XanimePlayer(MR::getJ3DModel(mActor), mResourceTable, mXanimePlayer);
    changeDefaultUpper("基本");
    mXanimePlayerUpper->changeAnimation("基本");
    mXanimePlayerUpper->mCore->shareJointTransform(mXanimePlayer->mCore);
    PSMTXCopy(MR::tmpMtxRotYRad(PI), _DC.toMtxPtr());
}

bool MarioAnimator::isAnimationStop() const {
    return mXanimePlayer->mCurrentAnimation == mXanimePlayer->mDefaultAnimation;
}

void MarioAnimator::targetWeight(f32* pWeight, f32 target, f32 rate) {
    if (getPlayer()->isSwimming()) {
        *pWeight = *pWeight * (1.0f - mActor->mConst->getTable()->mWeightBlendRatioSwim) + target * mActor->mConst->getTable()->mWeightBlendRatioSwim;
    } else if (getPlayer()->_3FA != 0) {
        *pWeight = *pWeight * 0.5f + target * 0.5f;
    } else {
        *pWeight = *pWeight * (1.0f - rate) + target * rate;
    }
}

void MarioAnimator::setWalkWeight(const f32* pWeights) {
    f32 rate = mActor->mConst->getTable()->mWeightBlendRatio;
    if (getPlayer()->_3FA != 0) {
        rate = 0.5f;
    }

    setBlendWeight(pWeights, rate);
}

void MarioAnimator::setBlendWeight(const f32* pWeights, f32 rate) {
    targetWeight(&_18, pWeights[0], rate);
    targetWeight(&_1C, pWeights[1], rate);
    targetWeight(&_20, pWeights[2], rate);
    targetWeight(&_24, pWeights[3], rate);

    for (u32 i = 0; i < 4; i++) {
        mXanimePlayer->changeTrackWeight(i, *(&_18 + i));
    }
}

void MarioAnimator::forceSetBlendWeight(const f32* pWeights) {
    for (u32 i = 0; i < 4; i++) {
        (&_18)[i] = pWeights[i];
        mXanimePlayer->changeTrackWeight(i, (&_18)[i]);
    }
}

void MarioAnimator::initWalkWeight() {
    _18 = 0.0f;
    _1C = 0.0f;
    _20 = 0.0f;
    _24 = 1.0f;
}

bool MarioAnimator::isLandingAnimationRun() const {
    if (isPlayerModeTeresa()) {
        return false;
    }

    if (isAnimationRun("着地") || isAnimationRun("着地B") || isAnimationRun("着地C") || isAnimationRun("着地ターン") ||
        isAnimationRun("着地幅とび")) {
        return true;
    }

    if (isAnimationRun("すべり着地")) {
        return true;
    }

    if (isAnimationRun(getPlayer()->_728)) {
        return true;
    }

    if (isAnimationRun("ハード着地")) {
        return true;
    }

    if (isAnimationRun("逆着地")) {
        return true;
    }

    if (isAnimationRun("ショート着地")) {
        return true;
    }

    if (MR::isBckPlaying(mActor, "pulloutgraspend")) {
        return true;
    }

    return false;
}

bool MarioAnimator::isCancelableAnimationRun() const {
    if (isAnimationRun("着地") || isAnimationRun("着地B") || isAnimationRun("着地C") || isAnimationRun("着地ターン") ||
        isAnimationRun("着地幅とび")) {
        return true;
    }

    if (isAnimationRun("すべり着地")) {
        return true;
    }

    if (MR::isBckPlaying(mActor, "pulloutgraspend")) {
        return true;
    }

    return false;
}

bool MarioAnimator::isWalkOrWaitingMotion() const {
    if (isAnimationStop()) {
        if (isAnimationRun("基本") || isAnimationRun("テニス基本")) {
            return true;
        }
    }

    if (isAnimationRun("坂前後ウエイト")) {
        return true;
    }

    if (isAnimationRun("坂左右ウエイト")) {
        return true;
    }

    if (isAnimationRun("カリカリ限界")) {
        return true;
    }

    if (isAnimationRun("飛び込み準備")) {
        return true;
    }

    return false;
}

void MarioAnimator::update() {
    if (mXanimePlayer->isAnimationRunSimple()) {
        if (!mActor->_EA4 && !mActor->_3C0) {
            Mario* player = getPlayer();
            if (!player->mMovementStates._22) {
                if (!mActor->_934) {
                    player = getPlayer();
                    if (player->mMovementStates._1) {
                        stopAnimation(nullptr);
                    }
                }
            }
        }
    }

    runningCallback();

    bool specialMode = false;
    if (mActor->_482 || mActor->_483) {
        specialMode = true;
    }

    if (specialMode) {
        mActor->updateSpecialModeAnimation();
        return;
    }

    u8 walkStateTable[] = {0, 1, 1, 2, 2, 2, 3, 3};

    Mario* player = getPlayer();
    u8 prevWalkState = player->mTargetWalkSpeedIndex;
    player = getPlayer();
    player->_71D = prevWalkState;

    if (isWalkOrWaitingMotion() || isAnimationRun("しゃがみ基本")) {
        player = getPlayer();
        _14 = walkStateTable[player->mTargetWalkSpeedIndex];
    } else {
        _15 = 0xFF;
    }

    player = getPlayer();
    if (player->mDrawStates._A) {
        bool normalMode = !isPlayerModeHopper();
        if (normalMode) {
            player = getPlayer();
            if (!player->isStatusActive(MarioStatus_Freeze)) {
                if (isLandingAnimationRun()) {
                    stopAnimation(nullptr);
                }

                player = getPlayer();
                if (player->mMovementStates._B) {
                    getPlayer()->mMovementStates.jumping = false;
                    getPlayer()->mMovementStates._B = false;
                    stopAnimation(nullptr, "基本");
                }

                if (isAnimationStop()) {
                    changeAnimation("崖ふんばり", static_cast< const char* >(nullptr));
                }
            }
        }

        goto afterBrake;
    }

    if (isAnimationRun("崖ふんばり")) {
        player = getPlayer();
        if (!player->mMovementStates._1) {
            stopAnimation("崖ふんばり", "落下");
            player = getPlayer();
            player->_414 = 15;
        } else {
            stopAnimation("崖ふんばり", static_cast< const char* >(nullptr));
        }
    }

    if (isCancelableAnimationRun()) {
        player = getPlayer();
        player->decideWalkSpeed();

        player = getPlayer();
        if (player->mTargetWalkSpeedIndex != 0) {
            stopAnimation(nullptr);
        }

        player = getPlayer();
        if (player->mMovementStates._A) {
            stopAnimation(nullptr);
        }

        goto afterBrake;
    }

    player = getPlayer();
    if (player->mMovementStates._A) {
        goto squatWalk;
    }

    player = getPlayer();
    if (player->_20._A) {
        player = getPlayer();
        if (!player->mMovementStates.jumping) {
            player = getPlayer();
            if (player->mMovementStates._1 == 1) {
                goto squatWalk;
            }
        }
    }

    goto notSquat;

squatWalk:
    player = getPlayer();
    if (player->mMovementStates._23) {
        goto afterBrake;
    }

    player = getPlayer();
    player->decideSquatWalkAnimation();

    if (_14 != _15) {
        switch (_14) {
        case 0:
            player = getPlayer();
            player->startBas(nullptr, false, 0.0f, 0.0f);
            break;
        default: {
            if (!isAnimationRun("幅とび")) {
                player = getPlayer();
                player->startBas("SquatWalk", false, 0.0f, 79.0f);
            } else {
                player = getPlayer();
                player->startBas(nullptr, false, 0.0f, 0.0f);
            }
        }
        }

        f32 frame = getAnimator()->mXanimePlayer->tellAnimationFrame();
        player = getPlayer();
        player->skipBas(frame);
    }

    _15 = _14;
    goto afterBrake;

notSquat:
    player = getPlayer();
    if (player->mMovementStates._34) {
        player = getPlayer();
        player->decideWalkSpeed();
        player = getPlayer();
        player->decideOnIceAnimation();
        goto afterBrake;
    }

    if (isWalkOrWaitingMotion()) {
        player = getPlayer();
        player->decideWalkSpeed();
        player = getPlayer();
        player->decideWalkAnimation();

        if (mActor->mBeeWallWalk != 0) {
            updateWalkBas("BeeCreepWalk", 59.0f);
        } else {
            updateWalkBas("Walk", 59.0f);
        }

        goto afterBrake;
    }

    if (isAnimationRun("氷上慣性走行")) {
        stopAnimation(nullptr);
        goto doBrake;
    }

    if (isAnimationRun("壁押し", 0)) {
        player = getPlayer();
        player->decideWalkSpeed();
        player = getPlayer();
        player->checkWallPush();
        goto doBrake;
    }

    player = getPlayer();
    if (player->mMovementStates.turning || getPlayer()->mMovementStates._4) {
        player = getPlayer();
        player->decideWalkSpeed();
        goto doBrake;
    }

    if (!isAnimationRun("ハード着地")) {
        if (isAnimationRun("がんばり走り")) {
            player = getPlayer();
            if (player->_8F0 < 1.0f) {
                stopAnimation(nullptr);
            }
        } else {
            _15 = 0;
        }
    }

doBrake:
    player = getPlayer();
    player->updateBrakeAnimation();

afterBrake:
    if (isAnimationRun("基本") || isAnimationRun("がんばり走り")) {
        setTilt();
    } else {
        resetTilt();
    }

    if (isAnimationRun("スライダー尻")) {
        setHipSliderTilt();
    }

    if (isAnimationRun("坂すべり下向きあおむけ", 3)) {
        setHipSlipTilt();
    }

    setHoming();
    setHand();

    if (_6C) {
        XanimePlayer* upperPlayer = mXanimePlayerUpper;
        upperPlayer->updateBeforeMovement();
        upperPlayer->updateAfterMovement();

        u8 idx = mXanimePlayerUpper->_54;
        XanimeFrameCtrl& track = mXanimePlayerUpper->_24[idx];
        if (track.mState & 1) {
            XanimeFrameCtrl* ctrl = mXanimePlayerUpper->_20;
            if (ctrl->mAttribute != 1) {
                if (!mUpperDefaultSet) {
                    stopUpper(nullptr);
                }
            }
        }
    }

    updateJointRumble();
    mActor->updateSpecialModeAnimation();

    if (isAnimationStop()) {
        mCurrBck = mXanimePlayer->getCurrentBckName();
    }
}

void MarioAnimator::updateWalkBas(const char* pAnimName, f32 speed) {
    bool shouldStart = false;

    // Check if player has a special animation queued
    if (getPlayer()->_970) {
        Mario* player = getPlayer();
        const char* currentBck = mXanimePlayer->getCurrentBckName();

        if (strcmp(currentBck, player->_970) != 0) {
            shouldStart = true;
            if (MR::strcasecmp("DamageWait", getPlayer()->_970) == 0) {
                if (mActor->mHealth == 1) {
                    shouldStart = false;
                }
            }
        }
    }

    // Check animation state transition
    if (_14 == _15) {
        if (_14 != 0) {
            if (!getPlayer()->isRunningBas(pAnimName)) {
                goto startBas;
            }
        }

        if (_14 != 0) {
            goto updatePrevState;
        }

        if (!shouldStart) {
            goto updatePrevState;
        }
    }

    // Start or continue animation
startBas:
    switch (_14) {
    case 0:
        getPlayer()->startBas(nullptr, false, 0.0f, 0.0f);
        break;
    default:
        getPlayer()->startBas(pAnimName, false, 0.0f, speed);
        break;
    }

    {
        f32 frame = getAnimator()->mXanimePlayer->tellAnimationFrame();
        getPlayer()->skipBas(frame);
    }

updatePrevState:
    _15 = _14;
}

void MarioAnimator::setHand() {
    TVec3f scale;
    scale.x = getPlayer()->_3F0;
    scale.y = 1.0f;
    scale.z = 1.0f;
    XanimeCore* core = mXanimePlayer->mCore;
    u8 armR = MR::getJointIndex(mActor, "ArmR1");
    XjointTransform* armRJoint = core->getJointTransform(armR);
    armRJoint->_14 = scale;
    core = mXanimePlayer->mCore;
    u8 armL = MR::getJointIndex(mActor, "ArmL1");
    XjointTransform* armLJoint = core->getJointTransform(armL);
    armLJoint->_14 = scale;
}

extern const char* jname_chest;

void MarioAnimator::setTilt() {
    f32 tiltAngle = 0.0f;

    if (!MR::isNearZero(_60, 0.001f)) {
        f32 frontDot = _60.dot(getFrontVec());
        tiltAngle = 1.0f - frontDot;

        if (tiltAngle > 0.001f) {
            TVec3f cross = _60.cross(getFrontVec());

            f32 gravDot = cross.dot(*getPlayer()->getGravityVec());
            if (gravDot < 0.0f) {
                tiltAngle = -tiltAngle;
            }
        } else {
            tiltAngle = 0.0f;
        }
    }

    _60 = getFrontVec();

    Mario* player = getPlayer();
    MarioConstTable* table = mActor->mConst->getTable();

    f32 speed = player->mWalkSpeed;
    f32 ratio = table->mTiltRatio * speed;
    tiltAngle *= ratio;

    const f32 maxTilt = HALF_PI;
    f32 absTilt = MR::abs(tiltAngle);
    if (absTilt >= maxTilt) {
        tiltAngle *= (maxTilt / absTilt);
    }

    f32 absOld = MR::abs(_58);
    f32 absNew = MR::abs(tiltAngle);

    if (absNew > absOld) {
        _58 = 0.9f * _58 + 0.1f * tiltAngle;
    } else {
        _58 = 0.95f * _58 + 0.05f * tiltAngle;
    }

    table = mActor->mConst->getTable();
    Mario* player2 = getPlayer();
    f32 lookDown = player2->mWalkSpeed * table->mLookDownRatio;

    Mario* player3 = getPlayer();
    f32 vertAngle = -player3->_3F4;
    vertAngle *= PI;
    f32 f4 = 0.9f * _5C + 0.1f * lookDown;
    f32 f5 = vertAngle / 4.0f;
    _5C = 0.97f * f4 + 0.03f * f5;

    TVec3f rotation(0.0f, _58, _5C);
    MR::orderRotateMtx(0, rotation, _28.toMtxPtr());

    XanimeCore* core = mXanimePlayer->mCore;
    u8 chestIdx = MR::getJointIndex(mActor, jname_chest);
    XjointTransform* jt = core->getJointTransform(chestIdx);
    jt->_64 = _28.toMtxPtr();
}

void MarioAnimator::resetTilt() {
    PSMTXIdentity(_28.toMtxPtr());
    _58 = 0.0f;
    _5C = 0.0f;
    _60.zero();
}

void MarioAnimator::setHoming() {
    f32 angleH = 0.0f;
    s32 hasTarget = 0;
    TVec3f targetPos;
    if (mActor->getLookTargetSensor()) {
        HitSensor* lookTarget = mActor->getLookTargetSensor();
        targetPos = lookTarget->mPosition;
        hasTarget = 1;
    } else if (MR::isNormalTalking()) {
        LiveActor* talkActor = MR::getTalkingActor();
        if (talkActor != nullptr) {
            hasTarget = 1;
            f32 height = mActor->getFaceLookHeight(talkActor->mName);
            targetPos = talkActor->mPosition - getGravityVec() * height;
        }
    }

    if (hasTarget) {
        TVec3f toTarget = targetPos - mActor->_2AC;

        Mario* player = getPlayer();
        angleH = MR::vecKillElement(toTarget, player->_1FC, &toTarget);

        f32 dist = toTarget.length();
        f32 vAngle;

        if (dist < 10.0f) {
            vAngle = 0.0f;
        } else {
            f32 dist2 = toTarget.length();
            vAngle = JMath::sAtanTable.atan2_(angleH, dist2);

            if (isStatusActiveID(MarioStatus_Talk)) {
                if (vAngle > 1.0f) {
                    vAngle = 1.0f;
                }
            } else {
                MarioConstTable* table = mActor->mConst->getTable();
                if (vAngle > table->mLookMaxAngleVP) {
                    vAngle = table->mLookMaxAngleVP;
                }
            }

            MarioConstTable* table = mActor->mConst->getTable();
            f32 lowerLimit = -table->mLookMaxAngleVM;
            if (vAngle < lowerLimit) {
                vAngle = lowerLimit;
            }
        }

        angleH = MR::diffAngleAbs(toTarget, getFrontVec());

        MarioConstTable* table = mActor->mConst->getTable();
        if (angleH > table->mLookMaxAngleH) {
            angleH = table->mLookMaxAngleH;
        }

        TVec3f cross = toTarget.cross(getFrontVec());
        Mario* player2 = getPlayer();
        f32 gravDot = cross.dot(player2->_1FC);
        if (gravDot < 0.0f) {
            angleH = -angleH;
        }

        Mtx rotMtx;
        MR::orderRotateMtx(5, TVec3f(0.0f, angleH, -vAngle), rotMtx);

        MR::blendMtx(_7C.toMtxPtr(), rotMtx, 0.1f, _7C.toMtxPtr());

        XanimeCore* core = mXanimePlayer->getCore();
        u8 headIdx = MR::getJointIndex(mActor, "Head");
        XjointTransform* jt = core->getJointTransform(headIdx);
        jt->_64 = _7C.toMtxPtr();
    } else {
        Mtx identMtx;
        PSMTXIdentity(identMtx);
        MR::blendMtx(_7C.toMtxPtr(), identMtx, 0.1f, _7C.toMtxPtr());
    }

    MarioConstTable* table = mActor->mConst->getTable();

    f32 blend = 0.9f * _110 + 0.1f * angleH;
    _110 = blend;

    f32 leftShoulderRot = 0.0f;
    f32 rightShoulderRot = 0.0f;

    if (angleH > 0.0f) {
        table = mActor->mConst->getTable();
        f32 ratio = blend / table->mLookMaxAngleH;
        rightShoulderRot = ratio * table->mLookShoulderMoveMax;
    } else if (angleH < 0.0f) {
        table = mActor->mConst->getTable();
        f32 ratio = -blend / table->mLookMaxAngleH;
        leftShoulderRot = ratio * table->mLookShoulderMoveMax;
    }

    XanimeCore* core = mXanimePlayer->getCore();
    u8 leftIdx = MR::getJointIndex(mActor, "ShoulderL");
    core->getJointTransform(leftIdx)->_2C.x = leftShoulderRot;

    core = mXanimePlayer->getCore();
    u8 rightIdx = MR::getJointIndex(mActor, "ShoulderR");
    core->getJointTransform(rightIdx)->_2C.x = rightShoulderRot;
}

void MarioAnimator::setHipSlidingTilt(f32 stickX, f32 stickY) {
    TVec2f input(stickX, stickY);
    f32 weights[4];

    weights[0] = (TVec2f(0.0f, 1.0f) - input).length();

    weights[1] = (TVec2f(0.0f, -1.0f) - input).length();

    weights[2] = (TVec2f(-1.0f, 0.0f) - input).length();

    weights[3] = (TVec2f(1.0f, 0.0f) - input).length();

    const f32 maxDist = 1.414213f;
    for (s32 i = 0; i < 4; i++) {
        if (weights[i] >= maxDist) {
            weights[i] = 0.0f;
        } else {
            weights[i] = (maxDist - weights[i]) / maxDist;
        }
    }

    for (s32 i = 0; i < 4; i++) {
        f32 w = weights[i];
        f32 w2 = w * w;
        if (w < 0.0f) {
            weights[i] = -w2;
        } else {
            weights[i] = w2;
        }
    }

    MarioConstTable* table = mActor->mConst->getTable();
    f32 speed = table->mSliderTiltRatio;

    setBlendWeight(weights, speed);
}

void MarioAnimator::setHipSliderTilt() {
    f32 stickX = getStickX();
    f32 stickY = getStickY();
    setHipSlidingTilt(stickX, stickY);
}

void MarioAnimator::setHipSlipTilt() {
    TVec3f worldPadDir(getWorldPadDir());
    f32 frontDot = MR::vecKillElement(worldPadDir, getFrontVec(), &worldPadDir);
    Mario* player = getPlayer();
    f32 gravDot = MR::vecKillElement(worldPadDir, player->mSideVec, &worldPadDir);
    setHipSlidingTilt(-gravDot, frontDot);
}

void MarioAnimator::setSpeed(f32 speed) {
    mXanimePlayer->changeSpeed(speed);
}

f32 MarioAnimator::getFrame() const {
    return mXanimePlayer->tellAnimationFrame();
}

f32 MarioAnimator::getUpperFrame() const {
    return mXanimePlayerUpper->tellAnimationFrame();
}

void MarioAnimator::setWalkMode() {
    if (!getPlayer()->mMovementStates._F && getPlayer()->mMovementStates._10 && !getPlayer()->mMovementStates._A && 0.0f == getPlayer()->_8F0 &&
        static_cast< u8 >(getPlayer()->checkStickFrontBack()) == 2) {
        if (!isStatusActiveID(MarioStatus_Magic) && !isStatusActiveID(MarioStatus_Skate)) {
            if (getPlayer()->mMovementStates._35) {
                change("ブレーキ滑り床");
            } else {
                change("ブレーキ");
            }
        }
    } else if (isAnimationRun("ブレーキ")) {
        getPlayer()->doBrakingAnimation();
        getPlayer()->mTargetWalkSpeedIndex = 0;
    } else {
        mXanimePlayer->stopAnimation("ブレーキ滑り床");
    }

    if (getPlayer()->mMovementStates._4 && !getPlayer()->mMovementStates._23) {
        getPlayer();
    }
}

void MarioAnimator::calc() {
    bool specialMode = false;
    if (mActor->_482 || mActor->_483) {
        specialMode = true;
    }

    if (specialMode) {
        return;
    }

    Mario* player = getPlayer();
    TVec3f playerPos(player->_13C);
    XjointTransform* joint0 = mXanimePlayer->mCore->getJointTransform(0);
    joint0->_38 = playerPos;
    if (_6C) {
        // Both players animation calculation
        mXanimePlayer->calcAnm(0);
        u16 spineIdx = MR::getJointIndex(mActor, "Spine1");
        mXanimePlayerUpper->calcAnm((u16)spineIdx);
        u16 partsIdx = MR::getJointIndex(mActor, "PartsControl");
        mXanimePlayerUpper->overWriteMtxCalc((u16)partsIdx);
    } else {
        u16 spineIdx = MR::getJointIndex(mActor, "Spine1");
        J3DModelData* modelData = mActor->getModelData();
        J3DJoint* joint = modelData->mJointTree.mJointNodePointer[spineIdx];
        joint->setMtxCalc(nullptr);
        mXanimePlayer->calcAnm(0);
    }

    mXanimePlayer->mCore->_6 = 1;
    MR::calcJ3DModel(mActor);

    mXanimePlayer->mCore->_6 = 2;
    MR::calcJ3DModel(mActor);
    mXanimePlayer->clearAnm(0);

    if (_6C) {
        u16 spineIdx = MR::getJointIndex(mActor, "Spine1");
        mXanimePlayer->clearAnm((u16)spineIdx);
        u16 partsIdx = MR::getJointIndex(mActor, "PartsControl");
        mXanimePlayerUpper->clearMtxCalc((u16)partsIdx);
    }
}

void MarioAnimator::change(const char* pName) {
    if (mActor->_B90) {
        return;
    }

    if (isTeresaClear()) {
        mXanimePlayer->changeAnimation(pName);
    }

    const char* bck = mXanimePlayer->getCurrentBckName();
    if (bck != nullptr) {
        const XanimeGroupInfo* info = mXanimePlayer->mCurrentAnimation;
        if (info->mAttribute == 2) {
            f32 arg1 = info->mLoop, arg2 = info->mEnd;
            getPlayer()->startBas(bck, false, arg1, arg2);
        } else {
            getPlayer()->startBas(bck, false, 0.0f, 0.0f);
        }

        mActor->setBlink(bck);
    } else {
        getPlayer()->startBas(nullptr, false, 0.0f, 0.0f);
        mActor->setBlink(nullptr);
    }

    mActor->changeSpecialModeAnimation(pName);
    mCurrBck = bck;
    entryCallback(pName);
}

void MarioAnimator::changeUpper(const char* pName) {
    mXanimePlayerUpper->changeAnimation(pName);
    _6C = true;
}

void MarioAnimator::stopUpper(const char* pName) {
    if (!_6C) {
        return;
    }

    bool shouldStop = false;
    if (pName == nullptr) {
        shouldStop = true;
    } else if (mXanimePlayerUpper->isRun(pName)) {
        shouldStop = true;
    }

    if (shouldStop) {
        u16 jointID = MR::getJointIndex(mActor, "Spine1");
        XanimeCore* upperCore = mXanimePlayerUpper->getCore();
        J3DModelData* modelData = mActor->getModelData();
        XanimeCore* lowerCore = mXanimePlayer->getCore();
        upperCore->freezeCopy(modelData, lowerCore, (u16)jointID, 8);
        mXanimePlayerUpper->stopAnimation();
        _6C = false;
    }
}

void MarioAnimator::changeDefault(const char* pName) {
    getPlayer()->startBas(nullptr, false, 0.0f, 0.0f);

    mXanimePlayer->setDefaultAnimation(pName);
}

bool MarioAnimator::isDefaultAnimationRun(const char* pName) const {
    return strcmp(mXanimePlayer->getDefaultAnimationName(), pName) == 0;
}

void MarioAnimator::changeDefaultUpper(const char* pName) {
    if (pName != nullptr) {
        mUpperDefaultSet = true;
        mXanimePlayerUpper->setDefaultAnimation(pName);
    } else {
        mUpperDefaultSet = false;
    }
}

u16 MarioAnimator::getUpperJointID() const {
    return MR::getJointIndex(mActor, "Spine1");
}

const char* jname_chest = "Spine1";

void MarioAnimator::updateJointRumble() {
    u32 timer = _74;
    if (timer == 0) {
        _118 = 0.0f;
        return;
    }

    u32 newTimer = timer - 1;
    _74 = newTimer;

    f32 t = (f32)newTimer * JMath::TAngleConstant_< f32 >::RADIAN_DEG360() / 30.0f;
    f32 sineVal = JMASinRadian(t);

    f32 power = -_70;

    if (_74 < 30) {
        f32 fade = (f32)_74 / 30.0f;
        sineVal *= fade;
    }

    XanimeCore* core = mXanimePlayer->mCore;
    XjointTransform* larmJt = core->getJointTransform(static_cast< u16 >(MR::getJointIndex(mActor, "ArmL1")));
    larmJt->_2C.y = sineVal * power;

    core = mXanimePlayer->mCore;
    XjointTransform* rarmJt = core->getJointTransform(static_cast< u16 >(MR::getJointIndex(mActor, "ArmR2")));
    rarmJt->_2C.y = sineVal * power;

    f32 hipRot = 0.7f * ((f32)_74 / 60.0f);

    if (getPlayer()->mMovementStates._1 && !getPlayer()->mMovementStates._37 && getPlayer()->mTargetWalkSpeedIndex == 0 &&
        getPlayer()->calcAngleD(getPlayer()->_368) > 5.0f) {
        TVec3f dir;
        const TVec3f* airGrav = &getAirGravityVec();
        Mario* playerVec = getPlayer();
        MR::vecKillElement(playerVec->_368, *airGrav, &dir);
        MR::normalizeOrZero(&dir);

        if (getFrontVec().dot(dir) < -0.707f) {
            hipRot = -hipRot;
        } else {
            hipRot = getFrontVec().dot(dir) > 0.707f ? hipRot : 0.9f * _118;
        }

        PSMTXRotRad(_AC.toMtxPtr(), 'Z', hipRot);
    } else {
        PSMTXIdentity(_AC.toMtxPtr());
    }

    _118 = hipRot;

    core = mXanimePlayer->mCore;
    XjointTransform* hipJt = core->getJointTransform(static_cast< u16 >(MR::getJointIndex(mActor, "Spine2")));
    hipJt->_64 = _AC.toMtxPtr();
}

void MarioAnimator::addRumblePower(f32 power, u32 time) {
    if (_70 < power) {
        _70 = power;
    }

    _74 = time;
}

void MarioAnimator::setUpperRotateY(f32 angle) {
    PSMTXRotRad(_AC.toMtxPtr(), 'X', angle);
    XanimeCore* core = mXanimePlayer->mCore;
    XjointTransform* jt = core->getJointTransform(static_cast< u16 >(MR::getJointIndex(mActor, "Spine2")));
    jt->_64 = _AC.toMtxPtr();
}

void MarioAnimator::clearAllJointTransform() {
    {
        XanimeCore* core = mXanimePlayer->mCore;
        XjointTransform* jt = core->getJointTransform(static_cast< u16 >(MR::getJointIndex(mActor, "ShoulderL")));
        jt->_64 = nullptr;
    }

    {
        XanimeCore* core = mXanimePlayer->mCore;
        XjointTransform* jt = core->getJointTransform(static_cast< u16 >(MR::getJointIndex(mActor, "ShoulderR")));
        jt->_64 = nullptr;
    }

    {
        XanimeCore* core = mXanimePlayer->mCore;
        XjointTransform* jt = core->getJointTransform(static_cast< u16 >(MR::getJointIndex(mActor, "Head")));
        jt->_64 = nullptr;
    }

    {
        XanimeCore* core = mXanimePlayer->mCore;
        XjointTransform* jt = core->getJointTransform(static_cast< u16 >(MR::getJointIndex(mActor, "Spine2")));
        jt->_64 = nullptr;
    }

    {
        XanimeCore* core = mXanimePlayer->mCore;
        XjointTransform* jt = core->getJointTransform(static_cast< u16 >(MR::getJointIndex(mActor, "ArmL1")));
        jt->_2C.y = 0.0f;
    }

    {
        XanimeCore* core = mXanimePlayer->mCore;
        XjointTransform* jt = core->getJointTransform(static_cast< u16 >(MR::getJointIndex(mActor, "ArmR2")));
        jt->_2C.y = 0.0f;
    }

    {
        XanimeCore* core = mXanimePlayer->mCore;
        u8 chestIdx = MR::getJointIndex(mActor, jname_chest);
        XjointTransform* jt = core->getJointTransform(chestIdx);
        jt->_64 = nullptr;
    }

    {
        XanimeCore* core = mXanimePlayer->mCore;
        u8 hipIdx = MR::getJointIndex(mActor, "Hip");
        XjointTransform* jt = core->getJointTransform(hipIdx);
        jt->_64 = nullptr;
    }

    TVec3f zeroVec(0.0f, 0.0f, 0.0f);
    XjointTransform* jt = mXanimePlayer->mCore->getJointTransform(0);
    jt->_38 = zeroVec;

    _74 = 0;
    _70 = 0.0f;
}

bool MarioAnimator::isMirrorAnimation() {
    if (mActor->_468) {
        return false;
    }

    TVec3f camDir = getCamDirX();
    Mario* player = getPlayer();
    f32 dot = player->_1FC.dot(getCamDirY());
    if (dot < 0.0f) {
        camDir = -camDir;
    }

    if (isAnimationRun("壁すべり") || isAnimationRun("壁くっつき")) {
        if (!_10C) {
            const TVec3f& wallNorm = getPlayer()->getWallNorm();
            f32 wallDot = camDir.dot(wallNorm);
            if (wallDot < -0.5f) {
                _10D = true;
            } else {
                _10D = false;
            }

            _10C = true;
        }

        return _10D;
    }

    // Check if walling (player mMovementStates._37 bit check)
    if (getPlayer()->mMovementStates._37) {
        if (isAnimationRun("ブレーキ") || isAnimationRun("ターンブレーキ") || isAnimationRun("ターンジャンプ")) {
            if (!_10C) {
                f32 frontDot = camDir.dot(getPlayer()->mFrontVec);
                if (frontDot > 0.0f) {
                    _10D = true;
                } else {
                    _10D = false;
                }

                _10C = true;
            }

            return _10D;
        }
    }

    // Clear cache flag
    _10C = false;
    if (isAnimationRun("左空パンチ")) {
        return true;
    }

    return false;
}

void MarioAnimator::switchMirrorMode() {
    f32 scale = 1.0f;
    J3DModelX* model = static_cast< J3DModelX* >(MR::getJ3DModel(mActor));
    if (isMirrorAnimation()) {
        u32* modelFlags = (u32*)&model->mFlags;
        *modelFlags |= 1;
        XjointTransform* jt = mXanimePlayer->mCore->getJointTransform(0);
        TVec3f mirrorScale;
        mirrorScale.x = scale;
        mirrorScale.y = scale;
        mirrorScale.z = -scale;
        jt->_14 = mirrorScale;

        Mtx invBase;
        MtxPtr base = mActor->getBaseMtx();
        PSMTXInverse(base, invBase);
        MR::multMtx(_DC, invBase, MR::tmpMtxRotYRad(PI));

        base = mActor->getBaseMtx();
        MR::multMtx(_DC, _DC, base);

        jt->_6C = _DC;
    } else {
        u32* modelFlags = (u32*)&model->mFlags;
        *modelFlags &= ~1;
        XjointTransform* jt = mXanimePlayer->mCore->getJointTransform(0);
        TVec3f normalScale;
        normalScale.x = scale;
        normalScale.y = scale;
        normalScale.z = scale;
        jt->_14 = normalScale;
        jt->_6C = 0;
    }
}

void MarioAnimator::changePickupAnimation(const HitSensor* pSensor) {
    u32 type = pSensor->mType;
    switch (type) {
    case ACTMES_STAR_PIECE_GIFT_MAX:
        mActor->_494 = mActor->_49C;
        changeAnimation("カブ抜き", static_cast< const char* >(nullptr));
        mActor->changeNullAnimation("PullOut", -2);
        getPlayer()->stopWalk();
        break;
    case ACTMES_STAR_PIECE_GIFT:
    case ACTMES_STAR_PIECE_GIFT_1:
        mActor->_494 = mActor->_498;
        if (!getPlayer()->isSwimming()) {
            if (mActor->_424 == pSensor) {
                changeAnimationUpper("ひろいクイック", nullptr);
                playEffect("ひろいクイック");
                mActor->clearNullAnimation(-3);
                startPadVib(2);
            } else if (getPlayer()->mMovementStates.jumping && !getPlayer()->mMovementStates._B) {
                changeAnimation("ひろい空中", static_cast< const char* >(nullptr));
                mActor->clearNullAnimation(-3);
                getPlayer()->stopWalk();
            } else {
                changeAnimation("ひろい", static_cast< const char* >(nullptr));
                mActor->changeNullAnimation("CarryStart", -2);
                getPlayer()->stopWalk();
            }
        } else {
            mActor->clearNullAnimation(0);
            startPadVib(2);
        }

        break;
    }
}

void MarioAnimator::updateTakingAnimation(const HitSensor* pSensor) {
    if (pSensor == nullptr) {
        return;
    }

    LiveActor* sensorActor = pSensor->mHost;
    if (MR::isDead(sensorActor)) {
        if (mActor->_468 != 0) {
            mActor->rushDropThrowMemoSensor();
        }

        return;
    }

    u32 type = pSensor->mType;
    switch (type) {
    case ACTMES_STAR_PIECE_GIFT_MAX:
        stopAnimation(nullptr);
        changeAnimationUpper("カブウエイト", nullptr);
        mActor->clearNullAnimation(0);
        mActor->offTakingFlag();
        break;
    case ACTMES_STAR_PIECE_GIFT:
    case ACTMES_STAR_PIECE_GIFT_1:
        if (!getPlayer()->isSwimming()) {
            s32 shouldChange = 1;
            if (isAnimationRun("ひろいクイック")) {
                shouldChange = isAnimationTerminateUpper(nullptr);
            }

            if (shouldChange) {
                if (!isAnimationRun("ひろい空中") && mActor->isStopNullAnimation()) {
                    changeAnimationUpper("ひろいウエイト", nullptr);
                    startPadVib(2);
                    mActor->clearNullAnimation(0);
                    mActor->offTakingFlag();
                }
            }
        } else {
            MR::emitEffect(pSensor->mHost, "SwimBubble");
        }

        break;
    }
}

void MarioAnimator::changeThrowAnimation(const HitSensor* pSensor) {
    u32 type = pSensor->mType;
    switch (type) {
    case ACTMES_STAR_PIECE_GIFT_MAX:
        stopAnimationUpper(nullptr, nullptr);
        changeAnimation("両手投げ", static_cast< const char* >(nullptr));
        startPadVib("マリオ[亀投げ]");
        break;
    case ACTMES_STAR_PIECE_GIFT:
    case ACTMES_STAR_PIECE_GIFT_1:
        stopAnimationUpper(nullptr, nullptr);
        if (getPlayer()->isSwimming()) {
            changeAnimation("水泳亀投げ", "水泳基本");
        } else {
            changeAnimation("投げ", static_cast< const char* >(nullptr));
        }

        startPadVib("マリオ[亀投げ]");
        break;
    }
}

void MarioAnimator::waterToGround() {
    HitSensor* swimSensor;
    if (mActor->_468 == 0) {
        swimSensor = nullptr;
    } else {
        swimSensor = mActor->_428[0];
    }

    if (swimSensor == nullptr) {
        return;
    }

    u32 state = swimSensor->mType;
    switch (state) {
    case ACTMES_STAR_PIECE_GIFT:
    case ACTMES_STAR_PIECE_GIFT_1:
        changeAnimationUpper("ひろいウエイト", nullptr);
        mActor->clearNullAnimation(0);
        MR::deleteEffect(swimSensor->mHost, "SwimBubble");
        break;
    case ACTMES_STAR_PIECE_GIFT_MAX:
        changeAnimationUpper("カブウエイト", nullptr);
        mActor->clearNullAnimation(0);
        break;
    }
}

HitSensor* MarioActor::getLookTargetSensor() const {
    if (_934) {
        return nullptr;
    }

    if (isSleeping()) {
        return nullptr;
    }

    return _46C;
}

f32 XanimePlayer::tellAnimationFrame() const {
    if (isTerminate()) {
        return _20->getEnd();
    }

    return _20->getFrame();
}
