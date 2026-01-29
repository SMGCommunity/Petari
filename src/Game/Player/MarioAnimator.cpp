#include "Game/Player/MarioAnimator.hpp"
#include "Game/Animation/XanimeResource.hpp"
#include "Game/Player/MarioActor.hpp"
#include "Game/Player/MarioConst.hpp"

#include "Game/Animation/XanimePlayer.hpp"
#include "Game/LiveActor/HitSensor.hpp"
#include "Game/Player/MarioAnimatorData.hpp"
#include "Game/Util/DemoUtil.hpp"
#include "Game/Util/EffectUtil.hpp"
#include "Game/Util/JointUtil.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/MathUtil.hpp"
#include "Game/Util/MtxUtil.hpp"
#include "JSystem/JMath/JMATrigonometric.hpp"
#include <cstring>

extern const char* jname_chest;
static const char sHip[] = "Hip";
static const char sRun[] = "Run\0\0\0\0\0";

MarioAnimator::MarioAnimator(MarioActor* actor) : MarioModule(actor) {
    init();
}

void MarioAnimator::init() {
    XanimeSwapTable* luigiAnimations = nullptr;
    if (gIsLuigi) {
        luigiAnimations = luigiAnimeSwapTable;
    }
    mResourceTable =
        new XanimeResourceTable(MR::getResourceHolder(mActor), marioAnimeTable, marioAnimeAuxTable, marioAnimeOfsTable, &singleAnimeTable[0].parent,
                                &doubleAnimeTable[0].parent, &tripleAnimeTable[0].parent, &quadAnimeTable[0].parent, luigiAnimations);

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
    PSMTXCopy(MR::tmpMtxRotYRad(3.14159274101f), _DC.toMtxPtr());
}

bool MarioAnimator::isAnimationStop() const {
    return mXanimePlayer->mCurrentAnimation == mXanimePlayer->mDefaultAnimation;
}

void MarioAnimator::change(const char* name) {
    if (mActor->_B90) {
        return;
    }

    if (isTeresaClear()) {
        mXanimePlayer->changeAnimation(name);
    }

    const char* bck = mXanimePlayer->getCurrentBckName();
    if (bck) {
        const XanimeGroupInfo* info = mXanimePlayer->mCurrentAnimation;
        if (info->_18 == 2) {
            f32 arg1 = info->_14, arg2 = info->_10;
            getPlayer()->startBas(bck, false, arg1, arg2);
        } else {
            getPlayer()->startBas(bck, false, 0.0f, 0.0f);
        }
        mActor->setBlink(bck);
    } else {
        getPlayer()->startBas(nullptr, false, 0.0f, 0.0f);
        mActor->setBlink(nullptr);
    }
    mActor->changeSpecialModeAnimation(name);
    mCurrBck = bck;
    entryCallback(name);
}

void MarioAnimator::changeUpper(const char* name) {
    mXanimePlayerUpper->changeAnimation(name);
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
        s32 jointID = MR::getJointIndex(mActor, "Spine1");
        XanimeCore* upperCore = mXanimePlayerUpper->mCore;
        J3DModelData* modelData = mActor->getModelData();
        XanimeCore* lowerCore = mXanimePlayer->mCore;
        upperCore->freezeCopy(modelData, lowerCore, (u16)jointID, 8);
        mXanimePlayerUpper->stopAnimation();
        _6C = false;
    }
}

void MarioAnimator::changeDefault(const char* name) {
    getPlayer()->startBas(nullptr, false, 0.0f, 0.0f);

    mXanimePlayer->setDefaultAnimation(name);
}

void MarioAnimator::changeDefaultUpper(const char* name) {
    if (name) {
        mUpperDefaultSet = true;
        mXanimePlayerUpper->setDefaultAnimation(name);
    } else {
        mUpperDefaultSet = false;
    }
}

void MarioAnimator::initWalkWeight() {
    _18 = 0.0f;
    _1C = 0.0f;
    _20 = 0.0f;
    _24 = 1.0f;
}

f32 MarioAnimator::getFrame() const {
    return mXanimePlayer->tellAnimationFrame();
}

f32 MarioAnimator::getUpperFrame() const {
    return mXanimePlayerUpper->tellAnimationFrame();
}

void MarioAnimator::setSpeed(f32 speed) {
    mXanimePlayer->changeSpeed(speed);
}

bool MarioAnimator::isDefaultAnimationRun(const char* pName) const {
    return strcmp(mXanimePlayer->getDefaultAnimationName(), pName) == 0;
}

s32 MarioAnimator::getUpperJointID() const {
    return MR::getJointIndex(mActor, "Spine1");
}

void MarioAnimator::forceSetBlendWeight(const f32* pWeights) {
    f32 weight;
    for (u32 i = 0; i < 4; i++) {
        weight = pWeights[i];
        reinterpret_cast<f32*>(&_18)[i] = weight;
        mXanimePlayer->changeTrackWeight(i, weight);
    }
}

void MarioAnimator::targetWeight(f32* pWeight, f32 target, f32 rate) {
    if (getPlayer()->isSwimming()) {
        f32 swimRate = mActor->mConst->getTable()->mWeightBlendRatioSwim;
        *pWeight = *pWeight * (1.0f - swimRate) + target * swimRate;
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

bool MarioAnimator::isLandingAnimationRun() const {
    if (isPlayerModeTeresa()) {
        return false;
    }

    if (isAnimationRun("着地") || isAnimationRun("着地B") || isAnimationRun("着地C") ||
        isAnimationRun("着地ターン") || isAnimationRun("着地幅とび")) {
        return true;
    }

    if (isAnimationRun("ハード着地")) {
        return true;
    }

    if (isAnimationRun(getPlayer()->_728)) {
        return true;
    }

    if (isAnimationRun("すべり着地")) {
        return true;
    }

    if (isAnimationRun("ショート着地")) {
        return true;
    }

    if (isAnimationRun("逆着地")) {
        return true;
    }

    return MR::isBckPlaying(mActor, "引き戻し着地");
}

bool MarioAnimator::isCancelableAnimationRun() const {
    if (isAnimationRun("着地") || isAnimationRun("着地B") || isAnimationRun("着地C") ||
        isAnimationRun("着地ターン") || isAnimationRun("着地幅とび")) {
        return true;
    }

    if (isAnimationRun("ハード着地")) {
        return true;
    }

    return MR::isBckPlaying(mActor, "引き戻し着地");
}

bool MarioAnimator::isWalkOrWaitingMotion() const {
    if (isAnimationStop()) {
        if (isAnimationRun("基本") || isAnimationRun("水泳基本")) {
            return true;
        }
    }

    if (isAnimationRun("テニス基本")) {
        return true;
    }

    if (isAnimationRun("スライダー尻")) {
        return true;
    }

    if (isAnimationRun("スライダー腹")) {
        return true;
    }

    return isAnimationRun("スライダー逆座り");
}

void MarioAnimator::setWalkMode() {
    u8 zero = 0;
    MarioAnimator* self = this;

    Mario* player = self->getPlayer();
    if (!player->mMovementStates._F) {
        player = self->getPlayer();
        if (player->mMovementStates._10) {
            player = self->getPlayer();
            if (!player->mMovementStates._A) {
                player = self->getPlayer();
                f32 stick = player->_8F0;
                if (0.0f == stick) {
                    player = self->getPlayer();
                    if ((u8)player->checkStickFrontBack() == 2) {
                        if (!self->isStatusActiveID(0x11)) {
                            if (!self->isStatusActiveID(0x1f)) {
                                player = self->getPlayer();
                                if (player->mMovementStates._35) {
                                    self->change("ブレーキ滑り床");
                                } else {
                                    self->change("ブレーキ");
                                }
                                goto end;
                            }
                        }
                    }
                }
            }
        }
    }

    if (self->isAnimationRun("ブレーキ")) {
        player = self->getPlayer();
        player->doBrakingAnimation();
        player = self->getPlayer();
        player->_71C = zero;
    } else {
        self->mXanimePlayer->stopAnimation("ブレーキ滑り床");
    }

end:
    player = self->getPlayer();
    if (player->mMovementStates._4) {
        player = self->getPlayer();
        if (!player->mMovementStates._23) {
            self->getPlayer();
        }
    }
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
    u16 jointID = MR::getJointIndex(mActor, "Spine2");
    XjointTransform* jt = core->getJointTransform(jointID);
    jt->_64 = _AC.toMtxPtr();
}

void MarioAnimator::clearAllJointTransform() {
    XanimeCore* core = mXanimePlayer->mCore;
    u16 jointIdx = MR::getJointIndex(mActor, "ShoulderL");
    XjointTransform* jt = core->getJointTransform(jointIdx);
    jt->_64 = nullptr;

    core = mXanimePlayer->mCore;
    jointIdx = MR::getJointIndex(mActor, "ShoulderR");
    jt = core->getJointTransform(jointIdx);
    jt->_64 = nullptr;

    core = mXanimePlayer->mCore;
    jointIdx = MR::getJointIndex(mActor, "Head");
    jt = core->getJointTransform(jointIdx);
    jt->_64 = nullptr;

    core = mXanimePlayer->mCore;
    jointIdx = MR::getJointIndex(mActor, "Spine2");
    jt = core->getJointTransform(jointIdx);
    jt->_64 = nullptr;

    core = mXanimePlayer->mCore;
    jointIdx = MR::getJointIndex(mActor, "ArmL1");
    jt = core->getJointTransform(jointIdx);
    jt->_30 = 0.0f;

    core = mXanimePlayer->mCore;
    jointIdx = MR::getJointIndex(mActor, "ArmR2");
    jt = core->getJointTransform(jointIdx);
    jt->_30 = 0.0f;

    core = mXanimePlayer->mCore;
    u8 chestIdx = MR::getJointIndex(mActor, jname_chest);
    jt = core->getJointTransform(chestIdx);
    jt->_64 = nullptr;

    core = mXanimePlayer->mCore;
    u8 hipIdx = MR::getJointIndex(mActor, sHip);
    jt = core->getJointTransform(hipIdx);
    jt->_64 = nullptr;

    TVec3f zeroVec(0.0f, 0.0f, 0.0f);
    jt = mXanimePlayer->mCore->getJointTransform(0);
    *reinterpret_cast<TVec3f*>(&jt->_38) = zeroVec;

    _74 = 0;
    _70 = 0.0f;
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
    *reinterpret_cast<TVec3f*>(&joint0->_38) = playerPos;


    if (_6C) {
        // Both players animation calculation
        mXanimePlayer->calcAnm(0);
        s32 spineIdx = MR::getJointIndex(mActor, "Spine1");
        mXanimePlayerUpper->calcAnm((u16)spineIdx);
        s32 headIdx = MR::getJointIndex(mActor, "Head");
        mXanimePlayerUpper->overWriteMtxCalc((u16)headIdx);
    } else {

        s32 spineIdx = MR::getJointIndex(mActor, "Spine1");
        J3DModelData* modelData = mActor->getModelData();
        void* joint = modelData->mJointTree.mJointNodePointer[spineIdx];
        *reinterpret_cast<u32*>(reinterpret_cast<u8*>(joint) + 0x54) = 0;
        mXanimePlayer->calcAnm(0);
    }


    mXanimePlayer->mCore->_6 = 1;
    MR::calcJ3DModel(mActor);

    mXanimePlayer->mCore->_6 = 2;
    MR::calcJ3DModel(mActor);


    mXanimePlayer->clearAnm(0);

    if (_6C) {
        s32 spineIdx = MR::getJointIndex(mActor, "Spine1");
        mXanimePlayer->clearAnm((u16)spineIdx);
        s32 headIdx = MR::getJointIndex(mActor, "Head");
        mXanimePlayerUpper->clearMtxCalc((u16)headIdx);
    }
}

void MarioAnimator::resetTilt() {
    PSMTXIdentity(_28.toMtxPtr());
    _58 = 0.0f;
    _5C = 0.0f;
    _60.zero();
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

void MarioAnimator::setHand() {
    TVec3f scale;
    scale.x = getPlayer()->_3F0;
    scale.y = 1.0f;
    scale.z = 1.0f;
    XanimeCore* core = mXanimePlayer->mCore;
    u8 armR = MR::getJointIndex(mActor, "ArmR1");
    *reinterpret_cast<TVec3f*>(&core->getJointTransform(armR)->_14) = scale;
    core = mXanimePlayer->mCore;
    u8 armL = MR::getJointIndex(mActor, "ArmL1");
    *reinterpret_cast<TVec3f*>(&core->getJointTransform(armL)->_14) = scale;
}

void MarioAnimator::waterToGround() {
    HitSensor* swimSensor;
    if (mActor->_468 == 0) {
        swimSensor = nullptr;
    } else {
        swimSensor = reinterpret_cast<HitSensor*>(mActor->_428[0]);
    }

    if (swimSensor == nullptr) {
        return;
    }

    u32 state = *reinterpret_cast<u32*>(swimSensor);
    switch (state) {
    case 0x0F:
    case 0x10:
        changeAnimationUpper("ひろいウエイト", nullptr);
        mActor->clearNullAnimation(0);
        MR::deleteEffect(reinterpret_cast<LiveActor*>(reinterpret_cast<u32*>(swimSensor)[9]), "SwimBubble");
        break;
    case 0x19:
        changeAnimationUpper("カブウエイト", nullptr);
        mActor->clearNullAnimation(0);
        break;
    }
}

void MarioAnimator::changePickupAnimation(const HitSensor* pSensor) {
    u32 type = *reinterpret_cast<const u32*>(pSensor);
    switch (type) {
    case 0x19:
        mActor->_494 = reinterpret_cast<u32>(mActor->_49C);
        changeAnimation("カブひろい", static_cast<const char*>(nullptr));
        mActor->changeNullAnimation("カブひろい", -2);
        getPlayer()->stopWalk();
        break;
    case 0x0F:
    case 0x10:
        mActor->_494 = reinterpret_cast<u32>(mActor->_498);
        if (!getPlayer()->isSwimming()) {
            if (mActor->_424 == reinterpret_cast<u32>(pSensor)) {
                changeAnimationUpper("ひろいクイック", nullptr);
                playEffect("ひろいクイック");
                mActor->clearNullAnimation(-3);
                startPadVib(2);
            } else if (getPlayer()->mMovementStates_LOW_WORD & 0x80000000) {
                if (!(getPlayer()->mMovementStates_LOW_WORD & 0x100000)) {
                    changeAnimation("ひろい空中", static_cast<const char*>(nullptr));
                    mActor->clearNullAnimation(-3);
                    getPlayer()->stopWalk();
                }
            } else {
                changeAnimation("ひろい", static_cast<const char*>(nullptr));
                mActor->changeNullAnimation("ひろい", -2);
                getPlayer()->stopWalk();
            }
        } else {
            mActor->clearNullAnimation(0);
            startPadVib(2);
        }
        break;
    }
}

void MarioAnimator::changeThrowAnimation(const HitSensor* pSensor) {
    u32 type = *reinterpret_cast<const u32*>(pSensor);
    switch (type) {
    case 0x19:
        stopAnimationUpper(nullptr, nullptr);
        changeAnimation("両手投げ", static_cast<const char*>(nullptr));
        startPadVib("マリオ[亀投げ]");
        break;
    case 0x0F:
    case 0x10:
        stopAnimationUpper(nullptr, nullptr);
        if (getPlayer()->isSwimming()) {
            changeAnimation("水泳亀投げ", "水泳基本");
        } else {
            changeAnimation("投げ", static_cast<const char*>(nullptr));
        }
        startPadVib("マリオ[亀投げ]");
        break;
    }
}

void MarioAnimator::updateTakingAnimation(const HitSensor* pSensor) {
    if (pSensor == nullptr) {
        return;
    }

    LiveActor* sensorActor = reinterpret_cast<LiveActor*>(reinterpret_cast<const u32*>(pSensor)[9]);
    if (MR::isDead(sensorActor)) {
        if (mActor->_468 != 0) {
            mActor->rushDropThrowMemoSensor();
        }
        return;
    }

    u32 type = *reinterpret_cast<const u32*>(pSensor);
    switch (type) {
    case 0x19:
        stopAnimation(nullptr, static_cast<const char*>(nullptr));
        changeAnimationUpper("カブウエイト", nullptr);
        mActor->clearNullAnimation(0);
        mActor->offTakingFlag();
        break;
    case 0x0F:
    case 0x10:
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
            MR::emitEffect(sensorActor, "SwimBubble");
        }
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

void MarioAnimator::switchMirrorMode() {
    J3DModel* model = MR::getJ3DModel(mActor);
    f32 one = 1.0f;
    if (isMirrorAnimation()) {

        u32* modelFlags = reinterpret_cast<u32*>(reinterpret_cast<u8*>(model) + 0x1B0);
        *modelFlags |= 1;


        XjointTransform* jt = mXanimePlayer->mCore->getJointTransform(0);
        TVec3f mirrorScale;
        mirrorScale.x = one;
        mirrorScale.y = one;
        mirrorScale.z = -one;
        *reinterpret_cast<TVec3f*>(&jt->_14) = mirrorScale;

        Mtx invBase;
        MtxPtr base = mActor->getBaseMtx();
        PSMTXInverse(base, invBase);
        PSMTXConcat(MR::tmpMtxRotYRad(3.1415927f), invBase, _DC.toMtxPtr());

        base = mActor->getBaseMtx();
        PSMTXConcat(base, _DC.toMtxPtr(), _DC.toMtxPtr());

        jt->_6C = reinterpret_cast<u32>(_DC.toMtxPtr());
    } else {

        u32* modelFlags = reinterpret_cast<u32*>(reinterpret_cast<u8*>(model) + 0x1B0);
        *modelFlags &= ~1;


        XjointTransform* jt = mXanimePlayer->mCore->getJointTransform(0);
        TVec3f normalScale;
        normalScale.x = one;
        normalScale.y = one;
        normalScale.z = one;
        *reinterpret_cast<TVec3f*>(&jt->_14) = normalScale;


        jt->_6C = 0;
    }
}
bool MarioAnimator::isMirrorAnimation() {

    if (mActor->_468) {
        return false;
    }


    TVec3f camDir(getCamDirX());

    Mario* player = getPlayer();


    f32 dot = getCamDirY().dot(player->_1FC);
    if (dot < 0.0f) {
        camDir = camDir.negateInline();
    }


    if (isAnimationRun("WallWalkL") || isAnimationRun("WallWalkR")) {
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

        if (isAnimationRun("WallSlide") || isAnimationRun("WallKeep") || isAnimationRun("WallWait")) {
            if (!_10C) {

                const TVec3f& frontVec = getPlayer()->mFrontVec;
                f32 frontDot = camDir.dot(frontVec);
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


    if (isAnimationRun("WallJump")) {
        return true;
    }

    return false;
}
void MarioAnimator::updateWalkBas(const char* pAnimName, f32 speed) {
    bool shouldStart = false;

    // Check if player has a special animation queued
    if (getPlayer()->_970) {
        Mario* player = getPlayer();
        const char* currentBck = mXanimePlayer->getCurrentBckName();

        if (strcmp(currentBck, player->_970) != 0) {
            shouldStart = true;


            if (MR::strcasecmp(sRun, getPlayer()->_970) == 0) {
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
    if (_14 == 0) {
        getPlayer()->startBas(nullptr, false, 0.0f, 0.0f);
    } else {
        getPlayer()->startBas(pAnimName, false, 0.0f, speed);
    }


    {
        f32 frame = getAnimator()->mXanimePlayer->tellAnimationFrame();
        getPlayer()->skipBas(frame);
    }

updatePrevState:
    _15 = _14;
}

void MarioAnimator::setHipSlidingTilt(f32 stickX, f32 stickY) {
    TVec2f input(stickX, stickY);
    f32 weights[4];

    TVec2f ref0(0.0f, 1.0f);
    weights[0] = (ref0 - input).length();

    TVec2f ref1(0.0f, -1.0f);
    weights[1] = (ref1 - input).length();

    TVec2f ref2(-1.0f, 0.0f);
    weights[2] = (ref2 - input).length();

    TVec2f ref3(1.0f, 0.0f);
    weights[3] = (ref3 - input).length();

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

    MarioConst* marioConst = mActor->mConst;
    MarioConstTable* table = marioConst->mTable[marioConst->mCurrentTable];
    f32 speed = table->mSliderTiltRatio;

    setBlendWeight(weights, speed);
}

void MarioAnimator::setTilt() {
    f32 tiltAngle = 0.0f;

    if (!MR::isNearZero(_60, 0.001f)) {
        f32 frontDot = _60.dot(getFrontVec());
        tiltAngle = 1.0f - frontDot;

        if (tiltAngle > 0.001f) {
            TVec3f cross;
            PSVECCrossProduct((Vec*)&_60, (Vec*)getFrontVec(), (Vec*)&cross);

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
    MarioConst* marioConst = mActor->mConst;
    MarioConstTable* table = marioConst->mTable[marioConst->mCurrentTable];

    f32 ratio = table->mTiltRatio * player->_278;
    tiltAngle *= ratio;

    const f32 maxTilt = 1.5707964f;
    f32 absTilt = __fabsf(tiltAngle);
    if (absTilt >= maxTilt) {
        tiltAngle *= (maxTilt / absTilt);
    }

    f32 absNew = __fabsf(tiltAngle);
    f32 absOld = __fabsf(_58);

    if (absNew > absOld) {
        _58 = 0.9f * _58 + 0.1f * tiltAngle;
    } else {
        _58 = 0.95f * _58 + 0.05f * tiltAngle;
    }

    marioConst = mActor->mConst;
    table = marioConst->mTable[marioConst->mCurrentTable];
    Mario* player2 = getPlayer();
    f32 lookDown = player2->_278 * table->mLookDownRatio;

    Mario* player3 = getPlayer();
    f32 vertAngle = -player3->_3F4 * 3.1415927f;
    f32 f5 = vertAngle * 0.25f;

    f32 f4 = 0.9f * _5C + 0.1f * lookDown;
    _5C = 0.97f * f4 + 0.03f * f5;

    TVec3f rotation(0.0f, _58, _5C);
    MR::orderRotateMtx(0, rotation, _28.toMtxPtr());

    XanimeCore* core = mXanimePlayer->mCore;
    u8 chestIdx = MR::getJointIndex(mActor, jname_chest);
    XjointTransform* jt = core->getJointTransform(chestIdx);
    jt->_64 = _28.toMtxPtr();
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
        if (talkActor) {
            hasTarget = 1;


            f32 height = mActor->getFaceLookHeight(talkActor->mName);


            const TVec3f& grav = getGravityVec();
            TVec3f gravScaled(grav);
            gravScaled.x *= height;
            gravScaled.y *= height;
            gravScaled.z *= height;


            targetPos = talkActor->mPosition - gravScaled;
        }
    }

    if (hasTarget) {
        TVec3f toTarget = targetPos - mActor->mPosition;

        Mario* player = getPlayer();
        angleH = MR::vecKillElement(toTarget, player->mHeadVec, &toTarget);

        f32 dist = PSVECMag((Vec*)&toTarget);
        f32 vAngle;

        if (dist < 10.0f) {
            vAngle = 0.0f;
        } else {
            f32 dist2 = PSVECMag((Vec*)&toTarget);
            vAngle = JMath::sAtanTable.atan2_(angleH, dist2);

            if (isStatusActiveID(0x22)) {
                if (vAngle > 1.0f) {
                    vAngle = 1.0f;
                }
            } else {
                MarioConst* marioConst = mActor->mConst;
                MarioConstTable* table = marioConst->mTable[marioConst->mCurrentTable];
                if (vAngle > table->mLookMaxAngleVP) {
                    vAngle = table->mLookMaxAngleVP;
                }
            }

            MarioConst* marioConst = mActor->mConst;
            MarioConstTable* table = marioConst->mTable[marioConst->mCurrentTable];
            f32 lowerLimit = -table->mLookMaxAngleVM;
            if (vAngle < lowerLimit) {
                vAngle = lowerLimit;
            }
        }

        angleH = MR::diffAngleAbs(toTarget, getFrontVec());

        MarioConst* marioConst = mActor->mConst;
        MarioConstTable* table = marioConst->mTable[marioConst->mCurrentTable];
        if (angleH > table->mLookMaxAngleH) {
            angleH = table->mLookMaxAngleH;
        }

        TVec3f cross;
        PSVECCrossProduct((Vec*)&toTarget, (Vec*)getFrontVec(), (Vec*)&cross);
        Mario* player2 = getPlayer();
        f32 gravDot = cross.dot(player2->mHeadVec);
        if (gravDot < 0.0f) {
            angleH = -angleH;
        }

        TVec3f rotAngles(0.0f, angleH, -vAngle);
        Mtx rotMtx;
        MR::orderRotateMtx(5, rotAngles, rotMtx);

        MR::blendMtx(_7C.toMtxPtr(), rotMtx, 0.1f, _7C.toMtxPtr());

        XanimeCore* core = mXanimePlayer->mCore;
        u8 spineIdx = MR::getJointIndex(mActor, "Spine1");
        XjointTransform* jt = core->getJointTransform(spineIdx);
        jt->_64 = _7C.toMtxPtr();
    } else {

        Mtx identMtx;
        PSMTXIdentity(identMtx);
        MR::blendMtx(_7C.toMtxPtr(), identMtx, 0.1f, _7C.toMtxPtr());
    }


    MarioConst* marioConst = mActor->mConst;
    MarioConstTable* table = marioConst->mTable[marioConst->mCurrentTable];

    f32 blend = 0.9f * _110 + 0.1f * angleH;
    _110 = blend;

    f32 leftShoulderRot = 0.0f;
    f32 rightShoulderRot = 0.0f;

    if (angleH > 0.0f) {
        marioConst = mActor->mConst;
        table = marioConst->mTable[marioConst->mCurrentTable];
        f32 maxH = table->mLookMaxAngleH;
        f32 maxShoulder = table->mLookShoulderMoveMax;
        rightShoulderRot = (blend / maxH) * maxShoulder;
    } else if (angleH < 0.0f) {
        marioConst = mActor->mConst;
        table = marioConst->mTable[marioConst->mCurrentTable];
        f32 maxH = table->mLookMaxAngleH;
        f32 maxShoulder = table->mLookShoulderMoveMax;
        leftShoulderRot = (-blend / maxH) * maxShoulder;
    }

    XanimeCore* core = mXanimePlayer->mCore;
    u8 leftIdx = MR::getJointIndex(mActor, "ShoulderL");
    XjointTransform* leftJt = core->getJointTransform(leftIdx);
    leftJt->_2C = leftShoulderRot;

    u8 rightIdx = MR::getJointIndex(mActor, "ShoulderR");
    XjointTransform* rightJt = core->getJointTransform(rightIdx);
    rightJt->_2C = rightShoulderRot;
}

void MarioAnimator::updateJointRumble() {
    u32 timer = _74;
    if (timer == 0) {
        _118 = 0.0f;
        return;
    }

    u32 newTimer = timer - 1;
    _74 = newTimer;

    f32 t = (f32)newTimer * 6.2831855f / 30.0f;
    f32 sineVal;
    if (t < 0.0f) {
        f32 tmp = t * -2607.5945f;
        u16 idx = (u16)tmp;
        sineVal = -JMath::sSinCosTable.table[idx & (JMath::TSinCosTable< 14, f32 >::LEN - 1)].a1;
    } else {
        f32 tmp = t * 2607.5945f;
        u16 idx = (u16)tmp;
        sineVal = JMath::sSinCosTable.table[idx & (JMath::TSinCosTable< 14, f32 >::LEN - 1)].a1;
    }

    f32 power = -_70;

    if (_74 < 30) {
        f32 fade = (f32)_74 / 30.0f;
        sineVal *= fade;
    }

    XanimeCore* core = mXanimePlayer->mCore;
    u16 larmIdx = MR::getJointIndex(mActor, "ArmL1");
    XjointTransform* larmJt = core->getJointTransform(larmIdx);
    larmJt->_30 = sineVal * power;

    u16 rarmIdx = MR::getJointIndex(mActor, "ArmR2");
    XjointTransform* rarmJt = core->getJointTransform(rarmIdx);
    rarmJt->_30 = sineVal * power;

    f32 hipRot = 0.7f * ((f32)_74 / 60.0f);

    Mario* player = getPlayer();
    if (!player->mMovementStates._1) {
        goto setIdentity;
    }

    player = getPlayer();
    if (player->mMovementStates._37) {
        goto setIdentity;
    }

    player = getPlayer();
    if (player->_71C != 0) {
        goto setIdentity;
    }

    Mario* playerVec = getPlayer();
    Mario* playerAngle = getPlayer();
    f32 angle = playerAngle->calcAngleD(playerVec->_368);
    if (angle <= 5.0f) {
        goto setIdentity;
    }

    {
        TVec3f dir;
        const TVec3f* airGrav = &getAirGravityVec();
        Mario* playerVec2 = getPlayer();
        MR::vecKillElement(playerVec2->_368, *airGrav, &dir);
        MR::normalizeOrZero(&dir);

        f32 frontDot = getFrontVec().dot(dir);
        if (frontDot < -0.707f) {
            hipRot = -hipRot;
        } else {
            f32 frontDot2 = getFrontVec().dot(dir);
            if (frontDot2 > 0.707f) {
                goto afterSlide;
            }
            hipRot = 0.9f * _118;
        }
    }

afterSlide:
    PSMTXRotRad(_AC.toMtxPtr(), 'Z', hipRot);
    goto afterRotate;

setIdentity:
    PSMTXIdentity(_AC.toMtxPtr());

afterRotate:

    _118 = hipRot;

    core = mXanimePlayer->mCore;
    u16 hipIdx = MR::getJointIndex(mActor, "Spine2");
    XjointTransform* hipJt = core->getJointTransform(hipIdx);
    hipJt->_64 = _AC.toMtxPtr();
}

void MarioAnimator::update() {
    if (mXanimePlayer->isAnimationRunSimple()) {
        if (!mActor->_EA4 && !mActor->_3C0) {
            Mario* player = getPlayer();
            if (!player->mMovementStates._22) {
                if (!mActor->_934) {
                    player = getPlayer();
                    if (player->mMovementStates._1) {
                        stopAnimation(static_cast<const char*>(nullptr), static_cast<const char*>(nullptr));
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

    u8 walkStateTable[8] = { 0, 1, 2, 3, 4, 5, 6, 7 };

    Mario* player = getPlayer();
    u8 prevWalkState = player->_71C;
    player = getPlayer();
    player->_71D = prevWalkState;

    if (isWalkOrWaitingMotion() || isAnimationRun("待機")) {
        player = getPlayer();
        _14 = walkStateTable[player->_71C];
    } else {
        _15 = 0xFF;
    }

    player = getPlayer();
    if (player->mDrawStates._A) {
        if (!isPlayerModeHopper()) {
            player = getPlayer();
            if (!player->isStatusActive(0xD)) {
                if (isLandingAnimationRun()) {
                    stopAnimation(static_cast<const char*>(nullptr), static_cast<const char*>(nullptr));
                }

                player = getPlayer();
                if (player->mMovementStates._B) {
                    player->mMovementStates.jumping = false;
                    player->mMovementStates._B = false;
                    stopAnimation(static_cast<const char*>(nullptr), "着地");
                }

                if (isAnimationStop()) {
                    changeAnimation("基本", static_cast<const char*>(nullptr));
                }
            }
        }
        goto afterBrake;
    }

    if (isAnimationRun("基本")) {
        player = getPlayer();
        if (!player->mMovementStates._1) {
            stopAnimation("基本", "待機");
            player = getPlayer();
            player->_414 = 15;
        } else {
            stopAnimation("基本", static_cast<const char*>(nullptr));
        }
    }

    if (isCancelableAnimationRun()) {
        player = getPlayer();
        player->decideWalkSpeed();

        player = getPlayer();
        if (player->_71C != 0) {
            stopAnimation(static_cast<const char*>(nullptr), static_cast<const char*>(nullptr));
        }

        player = getPlayer();
        if (player->mMovementStates._A) {
            stopAnimation(static_cast<const char*>(nullptr), static_cast<const char*>(nullptr));
        }
        goto afterBrake;
    }

    player = getPlayer();
    if (player->mMovementStates._A) {
        goto squatWalk;
    }

    player = getPlayer();
    if (player->_20 & 0x00200000) {
        player = getPlayer();
        if (!player->mMovementStates.jumping) {
            player = getPlayer();
            if (player->mMovementStates._1) {
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
        if (_14 == 0) {
            player = getPlayer();
            player->startBas(nullptr, false, 0.0f, 0.0f);
        } else {
            if (!isAnimationRun("しゃがみ歩き")) {
                player = getPlayer();
                player->startBas("しゃがみ待機", false, 0.0f, 79.0f);
            } else {
                player = getPlayer();
                player->startBas(nullptr, false, 0.0f, 0.0f);
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

        if (mActor->mAlphaEnable) {
            updateWalkBas("スケート待機", 59.0f);
        } else {
            updateWalkBas("走り待機", 59.0f);
        }
        goto afterBrake;
    }

    if (isAnimationRun("壁押し")) {
        stopAnimation(static_cast<const char*>(nullptr), static_cast<const char*>(nullptr));
        goto doBrake;
    }

    if (isAnimationRun("壁ずり", 0)) {
        player = getPlayer();
        player->decideWalkSpeed();
        player = getPlayer();
        player->checkWallPush();
        goto doBrake;
    }

    player = getPlayer();
    if (player->mMovementStates.turning || player->mMovementStates._4) {
        player = getPlayer();
        player->decideWalkSpeed();
        goto doBrake;
    }

    if (!isAnimationRun("待機")) {
        if (isAnimationRun("ターン")) {
            player = getPlayer();
            if (player->_8F0 < 1.0f) {
                stopAnimation(static_cast<const char*>(nullptr), static_cast<const char*>(nullptr));
            }
        } else {
            _15 = 0;
        }
    }

doBrake:
    player = getPlayer();
    player->updateBrakeAnimation();

afterBrake:
    if (isAnimationRun("着地") || isAnimationRun("ターン")) {
        setTilt();
    } else {
        resetTilt();
    }

    if (isAnimationRun("スライダー")) {
        setHipSliderTilt();
    }

    if (isAnimationRun("すべり", 3)) {
        setHipSlipTilt();
    }

    setHoming();
    setHand();

    if (_6C) {
        mXanimePlayerUpper->updateBeforeMovement();
        mXanimePlayerUpper->updateAfterMovement();

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

f32 XanimePlayer::tellAnimationFrame() const {
    if (isTerminate()) {
        s16 endFrame = _20->mEnd;
        return (f32)endFrame;
    }

    return _20->mFrame;
}

namespace NrvMarioActor {
    MarioActorNrvWait MarioActorNrvWait::sInstance;
    MarioActorNrvGameOver MarioActorNrvGameOver::sInstance;
    MarioActorNrvGameOverAbyss MarioActorNrvGameOverAbyss::sInstance;
    MarioActorNrvGameOverAbyss2 MarioActorNrvGameOverAbyss2::sInstance;
    MarioActorNrvGameOverFire MarioActorNrvGameOverFire::sInstance;
    MarioActorNrvGameOverBlackHole MarioActorNrvGameOverBlackHole::sInstance;
    MarioActorNrvGameOverNonStop MarioActorNrvGameOverNonStop::sInstance;
    MarioActorNrvGameOverSink MarioActorNrvGameOverSink::sInstance;
    MarioActorNrvTimeWait MarioActorNrvTimeWait::sInstance;
    MarioActorNrvNoRush MarioActorNrvNoRush::sInstance;
}
