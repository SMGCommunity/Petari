#include "Game/Player/MarioAnimator.hpp"
#include "Game/Animation/XanimeResource.hpp"
#include "Game/Player/MarioActor.hpp"
#include "Game/Player/MarioConst.hpp"
#include "Game/Player/MarioAnimatorData.hpp"
#include "Game/LiveActor/HitSensor.hpp"
#include "Game/Util/DemoUtil.hpp"
#include "Game/Util/EffectUtil.hpp"
#include "Game/Util/JointUtil.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/MathUtil.hpp"
#include "Game/Util/ModelUtil.hpp"
#include "Game/Util/MtxUtil.hpp"
#include "Game/Util/StringUtil.hpp"
#include "JSystem/J3DGraphAnimator/J3DJoint.hpp"
#include "JSystem/J3DGraphAnimator/J3DModelData.hpp"
#include "JSystem/J3DGraphAnimator/J3DModel.hpp"
#include <revolution/mtx.h>
#include <cmath>
#include <cstring>

namespace JMath {
    template < s32 Len, typename T >
    class TAtanTable {
    public:
        T atan2_(T, T) const;

        T mTable[Len];
        T _1000;
    };
}  // namespace JMath

extern "C" JMath::TAtanTable< 1024, f32 > lbl_8062FC80;

extern "C" {
    extern u8 sInstance__Q213NrvMarioActor17MarioActorNrvWait;
    extern u8 sInstance__Q213NrvMarioActor21MarioActorNrvGameOver;
    extern u8 sInstance__Q213NrvMarioActor26MarioActorNrvGameOverAbyss;
    extern u8 sInstance__Q213NrvMarioActor27MarioActorNrvGameOverAbyss2;
    extern u8 sInstance__Q213NrvMarioActor25MarioActorNrvGameOverFire;
    extern u8 sInstance__Q213NrvMarioActor30MarioActorNrvGameOverBlackHole;
    extern u8 sInstance__Q213NrvMarioActor28MarioActorNrvGameOverNonStop;
    extern u8 sInstance__Q213NrvMarioActor25MarioActorNrvGameOverSink;
    extern u8 sInstance__Q213NrvMarioActor21MarioActorNrvTimeWait;
    extern u8 sInstance__Q213NrvMarioActor19MarioActorNrvNoRush;

    void __ct__Q213NrvMarioActor17MarioActorNrvWaitFv(void*);
    void __ct__Q213NrvMarioActor21MarioActorNrvGameOverFv(void*);
    void __ct__Q213NrvMarioActor26MarioActorNrvGameOverAbyssFv(void*);
    void __ct__Q213NrvMarioActor27MarioActorNrvGameOverAbyss2Fv(void*);
    void __ct__Q213NrvMarioActor25MarioActorNrvGameOverFireFv(void*);
    void __ct__Q213NrvMarioActor30MarioActorNrvGameOverBlackHoleFv(void*);
    void __ct__Q213NrvMarioActor28MarioActorNrvGameOverNonStopFv(void*);
    void __ct__Q213NrvMarioActor25MarioActorNrvGameOverSinkFv(void*);
    void __ct__Q213NrvMarioActor21MarioActorNrvTimeWaitFv(void*);
    void __ct__Q213NrvMarioActor19MarioActorNrvNoRushFv(void*);
}

extern const char lbl_805BAC68[];
extern const char lbl_805C3F7E[];
extern const char lbl_805C3F89[];
extern const char lbl_805C3F8F[];
extern const char lbl_805C3F95[];
extern const char lbl_805C3F9A[];
extern const char lbl_805C3FA4[];
extern const char lbl_805C3FAE[];
extern const char lbl_805C3FBD[];
extern const char lbl_805C3FC6[];
extern const char lbl_805C3FCD[];
extern const char lbl_805C3FE1[];
extern const char lbl_805C3FE7[];
extern const char lbl_805C3FDA[];
extern const char lbl_805C4068[];
extern const char lbl_805C4075[];
extern const char lbl_805C4084[];
extern const u8 lbl_80539AA0[];
extern const f64 lbl_80539AA8;
extern const f64 lbl_80539AB0;
extern const f32 lbl_8060FC80[];

const char* jname_chest = lbl_805C3FDA;
char lbl_806B228C[] = "Hip";

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

    const char* base = lbl_805BAC68 + 0x10000;
    const char* defaultAnim = base - 0x6E18;

    getPlayer()->startBas(nullptr, false, 0.0f, 0.0f);
    mXanimePlayer->setDefaultAnimation(defaultAnim);
    change(defaultAnim);

    mXanimePlayer->getCore()->enableJointTransform(MR::getJ3DModelData(mActor));

    mActor->mModelManager->mXanimePlayer = mXanimePlayer;
    mXanimePlayerUpper = new XanimePlayer(MR::getJ3DModel(mActor), mResourceTable, mXanimePlayer);
    changeDefaultUpper(defaultAnim);
    mXanimePlayerUpper->changeAnimation(defaultAnim);
    mXanimePlayerUpper->mCore->shareJointTransform(mXanimePlayer->mCore);
    PSMTXCopy(MR::tmpMtxRotYRad(3.1415927f), _DC.toMtxPtr());
}

bool MarioAnimator::isAnimationStop() const {
    return mXanimePlayer->mCurrentAnimation == mXanimePlayer->mDefaultAnimation;
}

void MarioAnimator::targetWeight(f32* pWeight, f32 weight, f32 ratio) {
    if (getPlayer()->isSwimming()) {
        const MarioConstTable* table = mActor->mConst->getTable();
        const f32 blend = table->mWeightBlendRatioSwim;
        *pWeight = (*pWeight * (1.0f - blend)) + (weight * blend);
        return;
    }

    if (getPlayer()->_3FA != 0) {
        const f32 blend = 0.5f;
        *pWeight = (*pWeight * blend) + (weight * blend);
        return;
    }

    *pWeight = (*pWeight * (1.0f - ratio)) + (weight * ratio);
}

void MarioAnimator::setWalkWeight(const f32* pWeight) {
    f32 blend = mActor->mConst->getTable()->mWeightBlendRatio;
    if (getPlayer()->_3FA != 0) {
        blend = 0.5f;
    }

    setBlendWeight(pWeight, blend);
}

void MarioAnimator::setBlendWeight(const f32* pWeight, f32 ratio) {
    targetWeight(&_18, pWeight[0], ratio);
    targetWeight(&_1C, pWeight[1], ratio);
    targetWeight(&_20, pWeight[2], ratio);
    targetWeight(&_24, pWeight[3], ratio);

    f32* weights = &_18;
    for (u32 i = 0; i < 4; ++i) {
        mXanimePlayer->changeTrackWeight(i, weights[i]);
    }
}

void MarioAnimator::forceSetBlendWeight(const f32* pWeight) {
    f32* weights = &_18;
    for (u32 i = 0; i < 4; ++i) {
        weights[i] = pWeight[i];
        mXanimePlayer->changeTrackWeight(i, weights[i]);
    }
}

bool MarioAnimator::isLandingAnimationRun() const {
    const char* base = lbl_805BAC68 + 0x10000;

    if (isPlayerModeTeresa()) {
        return false;
    }

    if (isAnimationRun(base - 0x6E13) || isAnimationRun(base - 0x6E0E) || isAnimationRun(base - 0x6E08) ||
        isAnimationRun(base - 0x6E02) || isAnimationRun(base - 0x6DF7)) {
        return true;
    }

    if (isAnimationRun(base - 0x6DEC)) {
        return true;
    }

    if (isAnimationRun(getPlayer()->_728)) {
        return true;
    }

    if (isAnimationRun(base - 0x6DE1) || isAnimationRun(base - 0x6DD6) || isAnimationRun(base - 0x6DCF)) {
        return true;
    }

    return MR::isBckPlaying(mActor, base - 0x6DC2);
}

bool MarioAnimator::isCancelableAnimationRun() const {
    const char* base = lbl_805BAC68 + 0x10000;

    if (isAnimationRun(base - 0x6E13) || isAnimationRun(base - 0x6E0E) || isAnimationRun(base - 0x6E08) ||
        isAnimationRun(base - 0x6E02) || isAnimationRun(base - 0x6DF7)) {
        return true;
    }

    if (isAnimationRun(base - 0x6DEC)) {
        return true;
    }

    return MR::isBckPlaying(mActor, base - 0x6DC2);
}

bool MarioAnimator::isWalkOrWaitingMotion() const {
    const char* base = lbl_805BAC68 + 0x10000;

    if (isAnimationStop() && (isAnimationRun(base - 0x6E18) || isAnimationRun(base - 0x6DB2))) {
        return true;
    }

    if (isAnimationRun(base - 0x6DA7) || isAnimationRun(base - 0x6D98) || isAnimationRun(base - 0x6D89)) {
        return true;
    }

    return isAnimationRun(base - 0x6D7C);
}

void MarioAnimator::update() {
    const char* base = lbl_805BAC68 + 0x10000;

    if (mXanimePlayer->isAnimationRunSimple()) {
        if (!mActor->_EA4 && !mActor->_3C0) {
            if ((getPlayer()->mMovementStates_HIGH_WORD & 0x20000000) == 0 && !mActor->_934) {
                if (getPlayer()->mMovementStates_LOW_WORD & 0x40000000) {
                    stopAnimation(static_cast<const char*>(nullptr), static_cast<const char*>(nullptr));
                }
            }
        }
    }

    runningCallback();

    bool doSpecialMode = false;
    if (mActor->_482) {
        doSpecialMode = true;
    } else if (mActor->_483) {
        doSpecialMode = true;
    }

    if (doSpecialMode) {
        mActor->updateSpecialModeAnimation();
        return;
    }

    u8 table[8];
    table[0] = lbl_80539AA0[0];
    table[1] = lbl_80539AA0[1];
    table[2] = lbl_80539AA0[2];
    table[3] = lbl_80539AA0[3];
    table[4] = lbl_80539AA0[4];
    table[5] = lbl_80539AA0[5];
    table[6] = lbl_80539AA0[6];
    table[7] = lbl_80539AA0[7];

    u8 walkState = getPlayer()->_71C;
    getPlayer()->_71D = walkState;

    if (isWalkOrWaitingMotion() || isAnimationRun(base - 0x6D6F)) {
        _14 = table[getPlayer()->_71C];
    } else {
        _15 = 0xFF;
    }

    bool skipMain = false;

    if (getPlayer()->mDrawStates_WORD & 0x00200000) {
        if (!isPlayerModeHopper() && !getPlayer()->isStatusActive(0xD)) {
            if (isLandingAnimationRun()) {
                stopAnimation(static_cast<const char*>(nullptr), static_cast<const char*>(nullptr));
            }

            if (getPlayer()->mMovementStates_LOW_WORD & 0x00100000) {
                getPlayer()->mMovementStates_LOW_WORD &= ~(0x80000000u | 0x00100000u);
                stopAnimation(nullptr, base - 0x6E18);
            }

            if (isAnimationStop()) {
                changeAnimation(base - 0x6D62, static_cast<const char*>(nullptr));
            }
        }

        skipMain = true;
    }

    if (!skipMain) {
        if (isAnimationRun(base - 0x6D62)) {
            if ((getPlayer()->mMovementStates_LOW_WORD & 0x40000000) == 0) {
                stopAnimation(base - 0x6D62, base - 0x6D57);
                getPlayer()->_414 = 0xF;
            } else {
                stopAnimation(base - 0x6D62, static_cast<const char*>(nullptr));
            }
        }

        if (isCancelableAnimationRun()) {
            getPlayer()->decideWalkSpeed();
            if (getPlayer()->_71C != 0) {
                stopAnimation(static_cast<const char*>(nullptr), static_cast<const char*>(nullptr));
            }

            if (getPlayer()->mMovementStates_LOW_WORD & 0x00200000) {
                stopAnimation(static_cast<const char*>(nullptr), static_cast<const char*>(nullptr));
            }

            skipMain = true;
        }
    }

    if (!skipMain) {
        bool squat = false;
        if (getPlayer()->mMovementStates_LOW_WORD & 0x00200000) {
            squat = true;
        } else if ((getPlayer()->_20 & 0x00200000) && !(getPlayer()->mMovementStates_LOW_WORD & 0x80000000) &&
                   (getPlayer()->mMovementStates_LOW_WORD & 0x40000000)) {
            squat = true;
        }

        if (squat) {
            if (getPlayer()->mMovementStates_HIGH_WORD & 0x10000000) {
                skipMain = true;
            } else {
                getPlayer()->decideSquatWalkAnimation();

                if (_14 != _15) {
                    if (_14 == 0) {
                        getPlayer()->startBas(nullptr, false, 0.0f, 0.0f);
                    } else if (!isAnimationRun(base - 0x6D52)) {
                        getPlayer()->startBas(base - 0x6D4B, false, 0.0f, 79.0f);
                    } else {
                        getPlayer()->startBas(nullptr, false, 0.0f, 0.0f);
                    }

                    getPlayer()->skipBas(mXanimePlayer->tellAnimationFrame());
                    _15 = _14;
                }

                skipMain = true;
            }
        }
    }

    if (!skipMain) {
        if (getPlayer()->mMovementStates_HIGH_WORD & 0x00000800) {
            getPlayer()->decideWalkSpeed();
            getPlayer()->decideOnIceAnimation();
            skipMain = true;
        }
    }

    if (!skipMain) {
        if (isWalkOrWaitingMotion()) {
            getPlayer()->decideWalkSpeed();
            getPlayer()->decideWalkAnimation();

            if (mActor->mAlphaEnable) {
                updateWalkBas(base - 0x6D41, 59.0f);
            } else {
                updateWalkBas(base - 0x6D34, 59.0f);
            }

            skipMain = true;
        }
    }

    if (!skipMain) {
        if (isAnimationRun(base - 0x6D2F)) {
            stopAnimation(static_cast<const char*>(nullptr), static_cast<const char*>(nullptr));
        } else if (isAnimationRun(base - 0x6D22, 0)) {
            getPlayer()->decideWalkSpeed();
            getPlayer()->checkWallPush();
        } else if ((getPlayer()->mMovementStates_LOW_WORD & 0x10000000) || (getPlayer()->mMovementStates_LOW_WORD & 0x08000000)) {
            getPlayer()->decideWalkSpeed();
        } else if (!isAnimationRun(base - 0x6DE1)) {
            if (isAnimationRun(base - 0x6D1B)) {
                if (getPlayer()->_8F0 < 1.0f) {
                    stopAnimation(static_cast<const char*>(nullptr), static_cast<const char*>(nullptr));
                } else {
                    _15 = 0;
                }
            } else {
                _15 = 0;
            }
        }

        getPlayer()->updateBrakeAnimation();
    }

    if (isAnimationRun(base - 0x6E18) || isAnimationRun(base - 0x6D1B)) {
        setTilt();
    } else {
        resetTilt();
    }

    if (isAnimationRun(base - 0x6D0E)) {
        setHipSliderTilt();
    }

    if (isAnimationRun(base - 0x6D01, 3)) {
        setHipSlipTilt();
    }

    setHoming();
    setHand();

    if (_6C) {
        mXanimePlayerUpper->updateBeforeMovement();
        mXanimePlayerUpper->updateAfterMovement();

        if ((mXanimePlayerUpper->_24[mXanimePlayerUpper->_54].mFlags & 1) && (mXanimePlayerUpper->_20->mLoopMode != 1) &&
            !mUpperDefaultSet) {
            stopUpper(nullptr);
        }
    }

    updateJointRumble();
    mActor->updateSpecialModeAnimation();

    if (isAnimationStop()) {
        mCurrBck = mXanimePlayer->getCurrentBckName();
    }
}

void MarioAnimator::updateWalkBas(const char* pName, f32 step) {
    Mario* mario = getPlayer();
    bool changed = false;

    if (mario->_970) {
        if (strcmp(mXanimePlayer->getCurrentBckName(), mario->_970) != 0) {
            changed = true;
        }

        if (MR::strcasecmp(mario->_970, lbl_805C3F7E) == 0 && mActor->mHealth == 1) {
            changed = false;
        }
    }

    if (_14 != _15 || (_14 != 0 && !mario->isRunningBas(pName)) || (_14 == 0 && changed)) {
        if (_14 == 0) {
            mario->startBas(nullptr, false, 0.0f, 0.0f);
        } else {
            mario->startBas(pName, false, 0.0f, step);
        }

        mario->skipBas(mXanimePlayer->tellAnimationFrame());
        _15 = _14;
    }
}

void MarioAnimator::setHand() {
    TVec3f scale(getPlayer()->_3F0, 1.0f, 1.0f);
    XanimeCore* core = mXanimePlayer->mCore;

    XjointTransform* joint = core->getJointTransform(MR::getJointIndex(mActor, lbl_805C3F89));
    *reinterpret_cast<TVec3f*>(&joint->_14) = scale;

    joint = core->getJointTransform(MR::getJointIndex(mActor, lbl_805C3F8F));
    *reinterpret_cast<TVec3f*>(&joint->_14) = scale;
}

void MarioAnimator::setTilt() {
    f32 angle = 0.0f;

    if (!MR::isNearZero(_60, 0.001f)) {
        angle = 1.0f - _60.dot(getFrontVec());

        if (angle > 0.001f) {
            TVec3f cross;
            PSVECCrossProduct(&_60, &getFrontVec(), &cross);
            if (cross.dot(*getPlayer()->getGravityVec()) < 0.0f) {
                angle = -angle;
            }
        } else {
            angle = 0.0f;
        }
    }

    _60 = getFrontVec();

    const MarioConstTable* table = mActor->mConst->getTable();
    angle *= table->mTiltRatio * getPlayer()->_278;

    const f32 absAngle = __fabsf(angle);
    if (absAngle > 1.5707964f) {
        angle = (1.5707964f / absAngle) * angle;
    }

    if (__fabsf(angle) > __fabsf(_58)) {
        _58 = (0.9f * _58) + (0.1f * angle);
    } else {
        _58 = (0.95f * _58) + (0.05f * angle);
    }

    const f32 lookDown = getPlayer()->_278 * table->mLookDownRatio;
    const f32 lookAngle = -getPlayer()->_3F4 * 3.1415927f;
    const f32 lookTarget = (0.9f * _5C) + (0.1f * lookDown);
    _5C = (0.97f * lookTarget) + (0.03f * (0.25f * lookAngle));

    const TVec3f rot(0.0f, _58, _5C);
    MR::orderRotateMtx(0, rot, _28.toMtxPtr());

    XjointTransform* joint = mXanimePlayer->mCore->getJointTransform(MR::getJointIndex(mActor, jname_chest));
    joint->_64 = _28.toMtxPtr();
}

void MarioAnimator::resetTilt() {
    PSMTXIdentity(_28.toMtxPtr());
    _58 = 0.0f;
    _5C = 0.0f;
    _60.zero();
}

void MarioAnimator::setHoming() {
    TVec3f target;
    bool hasTarget = false;

    if (HitSensor* sensor = mActor->getLookTargetSensor()) {
        target = sensor->mPosition;
        hasTarget = true;
    } else if (MR::isNormalTalking()) {
        LiveActor* actor = MR::getTalkingActor();
        if (actor) {
            const f32 height = mActor->getFaceLookHeight(actor->mName);
            TVec3f gravity = getGravityVec();
            gravity.x *= height;
            gravity.y *= height;
            gravity.z *= height;
            target = actor->mPosition - gravity;
            hasTarget = true;
        }
    }

    f32 angleH = 0.0f;
    f32 angleV = 0.0f;

    if (hasTarget) {
        TVec3f vec = target;
        vec.sub(mActor->_2AC);

        angleV = MR::vecKillElement(vec, getPlayer()->_1FC, &vec);
        const f32 len = vec.length();

        if (len < 10.0f) {
            angleV = 0.0f;
        } else {
            angleV = lbl_8062FC80.atan2_(angleV, len);
        }

        const MarioConstTable* table = mActor->mConst->getTable();
        if (isStatusActiveID(0x22) && angleV > 1.0f) {
            angleV = 1.0f;
        } else if (angleV > table->mLookMaxAngleVP) {
            angleV = table->mLookMaxAngleVP;
        } else if (angleV < -table->mLookMaxAngleVM) {
            angleV = -table->mLookMaxAngleVM;
        }

        const TVec3f& rFront = getFrontVec();
        angleH = MR::diffAngleAbs(vec, rFront);
        if (angleH > table->mLookMaxAngleH) {
            angleH = table->mLookMaxAngleH;
        }

        TVec3f cross;
        PSVECCrossProduct(&rFront, &vec, &cross);
        if (cross.dot(getPlayer()->_1FC) < 0.0f) {
            angleH = -angleH;
        }

        const TVec3f rot(0.0f, angleH, -angleV);
        Mtx temp;
        MR::orderRotateMtx(5, rot, temp);
        MR::blendMtx(_7C.toMtxPtr(), temp, 0.1f, _7C.toMtxPtr());

        XjointTransform* joint = mXanimePlayer->mCore->getJointTransform(MR::getJointIndex(mActor, lbl_805C3F95));
        joint->_64 = _7C.toMtxPtr();
    } else {
        Mtx temp;
        PSMTXIdentity(temp);
        MR::blendMtx(_7C.toMtxPtr(), temp, 0.1f, _7C.toMtxPtr());
    }

    _110 = (0.9f * _110) + (0.1f * angleH);

    f32 left = 0.0f;
    f32 right = 0.0f;
    const MarioConstTable* table = mActor->mConst->getTable();

    if (angleH > 0.0f) {
        right = (_110 / table->mLookMaxAngleH) * table->mLookShoulderMoveMax;
    } else if (angleH < 0.0f) {
        left = ((-_110) / table->mLookMaxAngleH) * table->mLookShoulderMoveMax;
    }

    XanimeCore* core = mXanimePlayer->mCore;
    XjointTransform* joint = core->getJointTransform(MR::getJointIndex(mActor, lbl_805C3F9A));
    joint->_2C = left;

    joint = core->getJointTransform(MR::getJointIndex(mActor, lbl_805C3FA4));
    joint->_2C = right;
}

void MarioAnimator::setHipSlidingTilt(f32 x, f32 y) {
    TVec2f stick(x, y);
    f32 weights[4];

    TVec2f diff = TVec2f(0.0f, 1.0f);
    diff.sub(stick);
    weights[0] = diff.length();

    diff = TVec2f(0.0f, -1.0f);
    diff.sub(stick);
    weights[1] = diff.length();

    diff = TVec2f(-1.0f, 0.0f);
    diff.sub(stick);
    weights[2] = diff.length();

    diff = TVec2f(1.0f, 0.0f);
    diff.sub(stick);
    weights[3] = diff.length();

    const f32 limit = 1.414213f;
    for (int i = 0; i < 4; ++i) {
        if (weights[i] >= limit) {
            weights[i] = 0.0f;
        } else {
            weights[i] = (limit - weights[i]) / limit;
        }
    }

    for (int i = 0; i < 4; ++i) {
        const f32 value = weights[i];
        f32 squared = value * value;
        if (value < 0.0f) {
            squared = -squared;
        }
        weights[i] = squared;
    }

    setBlendWeight(weights, mActor->mConst->getTable()->mSliderTiltRatio);
}

void MarioAnimator::setHipSliderTilt() {
    setHipSlidingTilt(getStickX(), getStickY());
}

void MarioAnimator::setHipSlipTilt() {
    TVec3f dir = getWorldPadDir();
    const f32 front = MR::vecKillElement(dir, getFrontVec(), &dir);
    const f32 side = MR::vecKillElement(dir, getPlayer()->mSideVec, &dir);
    setHipSlidingTilt(-side, front);
}

void MarioAnimator::setWalkMode() {
    Mario* mario = getPlayer();

    if (!(mario->mMovementStates_LOW_WORD & 0x00010000) && (mario->mMovementStates_LOW_WORD & 0x00008000) &&
        !(mario->mMovementStates_LOW_WORD & 0x00200000) && mario->_8F0 == 0.0f && mario->checkStickFrontBack() == 2 &&
        !isStatusActiveID(0x11) && !isStatusActiveID(0x1F)) {
        if (mario->mMovementStates_HIGH_WORD & 0x00000400) {
            change(lbl_805C3FAE);
        } else {
            change(lbl_805C3FBD);
        }
    } else if (isAnimationRun(lbl_805C3FBD)) {
        mario->doBrakingAnimation();
        mario->_71C = 0;
    } else {
        mXanimePlayer->stopAnimation(lbl_805C3FAE);
    }

    if (getPlayer()->mMovementStates_LOW_WORD & 0x08000000) {
        if (!(getPlayer()->mMovementStates_HIGH_WORD & 0x10000000)) {
            getPlayer();
        }
    }
}

void MarioAnimator::calc() {
    if (mActor->_482 || mActor->_483) {
        return;
    }

    XanimeCore* core = mXanimePlayer->mCore;
    XjointTransform* joint = core->getJointTransform(0);
    *reinterpret_cast<TVec3f*>(&joint->_38) = getPlayer()->_13C;

    if (_6C) {
        mXanimePlayer->calcAnm(0);

        const u16 spine1 = MR::getJointIndex(mActor, lbl_805C3FC6);
        mXanimePlayerUpper->calcAnm(spine1);

        const u16 partsControl = MR::getJointIndex(mActor, lbl_805C3FCD);
        mXanimePlayerUpper->overWriteMtxCalc(partsControl);
    } else {
        const u16 spine1 = MR::getJointIndex(mActor, lbl_805C3FC6);
        J3DModelData* modelData = mActor->getModelData();
        modelData->mJointTree.mJointNode[spine1]->mMtxCalc = nullptr;
        mXanimePlayer->calcAnm(0);
    }

    mXanimePlayer->mCore->_6 = 1;
    MR::calcJ3DModel(mActor);
    mXanimePlayer->mCore->_6 = 2;
    MR::calcJ3DModel(mActor);
    mXanimePlayer->clearAnm(0);

    if (_6C) {
        const u16 spine1 = MR::getJointIndex(mActor, lbl_805C3FC6);
        mXanimePlayer->clearAnm(spine1);

        const u16 partsControl = MR::getJointIndex(mActor, lbl_805C3FCD);
        mXanimePlayerUpper->clearMtxCalc(partsControl);
    }
}

void MarioAnimator::change(const char* name) {
    if (mActor->_B90) {
        return;
    }

    if (!isPlayerModeTeresa()) {
        mXanimePlayer->changeAnimation(name);
    }

    const char* bck = mXanimePlayer->getCurrentBckName();
    if (bck) {
        const XanimeGroupInfo* info = mXanimePlayer->mCurrentAnimation;
        if (info->_18 == 2) {
            getPlayer()->startBas(bck, false, info->_14, info->_10);
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
    if (!pName) {
        shouldStop = true;
    } else if (mXanimePlayerUpper->isRun(pName)) {
        shouldStop = true;
    }

    if (shouldStop) {
        u16 index = MR::getJointIndex(mActor, lbl_805C3FC6);
        XanimeCore* upperCore = mXanimePlayerUpper->mCore;
        upperCore->freezeCopy(mActor->getModelData(), mXanimePlayer->mCore, index, 8);
        mXanimePlayerUpper->stopAnimation();
        _6C = false;
    }
}

void MarioAnimator::changeDefault(const char* name) {
    getPlayer()->startBas(nullptr, false, 0.0f, 0.0f);
    mXanimePlayer->setDefaultAnimation(name);
}

bool MarioAnimator::isDefaultAnimationRun(const char* pName) const {
    return strcmp(mXanimePlayer->getDefaultAnimationName(), pName) == 0;
}

void MarioAnimator::changeDefaultUpper(const char* name) {
    if (name) {
        mUpperDefaultSet = true;
        mXanimePlayerUpper->setDefaultAnimation(name);
    } else {
        mUpperDefaultSet = false;
    }
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

s32 MarioAnimator::getUpperJointID() const {
    return MR::getJointIndex(mActor, lbl_805C3FC6);
}

void MarioAnimator::updateJointRumble() {
    if (_74 == 0) {
        _118 = 0.0f;
        return;
    }

    u32 count = _74 - 1;
    _74 = count;

    f32 angle = (static_cast<f32>(count) * 6.2831855f) / 30.0f;
    f32 sinVal = 0.0f;

    if (angle < 0.0f) {
        s32 index = static_cast<s32>(angle * -2607.5945f);
        index &= 0x3FFF;
        sinVal = -lbl_8060FC80[index * 2];
    } else {
        s32 index = static_cast<s32>(angle * 2607.5945f);
        index &= 0x3FFF;
        sinVal = lbl_8060FC80[index * 2];
    }

    if (_74 < 0x1E) {
        sinVal *= static_cast<f32>(_74) / 30.0f;
    }

    const f32 scale = -_70;

    XanimeCore* core = mXanimePlayer->mCore;
    XjointTransform* joint = core->getJointTransform(MR::getJointIndex(mActor, lbl_805C3F8F));
    joint->_30 = sinVal * scale;

    joint = core->getJointTransform(MR::getJointIndex(mActor, lbl_805C3FE1));
    joint->_30 = sinVal * scale;

    f32 tilt = 0.7f * (static_cast<f32>(_74) / 60.0f);

    if ((getPlayer()->mMovementStates_LOW_WORD & 0x40000000) && !(getPlayer()->mMovementStates_HIGH_WORD & 0x00000100) &&
        getPlayer()->_71C == 0) {
        if (calcAngleD(getPlayer()->_368) > 5.0f) {
            TVec3f dir;
            MR::vecKillElement(getPlayer()->_368, getAirGravityVec(), &dir);
            MR::normalizeOrZero(&dir);

            const f32 dot = dir.dot(getFrontVec());
            if (dot < -0.707f) {
                tilt = -tilt;
            } else if (dot <= 0.707f) {
                tilt = 0.9f * _118;
            }

            PSMTXRotRad(_AC.toMtxPtr(), 'Z', tilt);
        } else {
            PSMTXIdentity(_AC.toMtxPtr());
        }
    } else {
        PSMTXIdentity(_AC.toMtxPtr());
    }

    _118 = tilt;

    joint = core->getJointTransform(MR::getJointIndex(mActor, lbl_805C3FE7));
    joint->_64 = _AC.toMtxPtr();
}

void MarioAnimator::addRumblePower(f32 power, u32 type) {
    if (_70 < power) {
        _70 = power;
    }
    _74 = type;
}

void MarioAnimator::setUpperRotateY(f32 angle) {
    PSMTXRotRad(_AC.toMtxPtr(), 'X', angle);

    XjointTransform* joint = mXanimePlayer->mCore->getJointTransform(MR::getJointIndex(mActor, lbl_805C3FE7));
    joint->_64 = _AC.toMtxPtr();
}

void MarioAnimator::clearAllJointTransform() {
    const char* base = lbl_805BAC68 + 0x10000;
    XanimeCore* core = mXanimePlayer->mCore;
    XjointTransform* joint = nullptr;

    joint = core->getJointTransform(MR::getJointIndex(mActor, base - 0x6CCE));
    joint->_64 = nullptr;

    joint = core->getJointTransform(MR::getJointIndex(mActor, base - 0x6CC4));
    joint->_64 = nullptr;

    joint = core->getJointTransform(MR::getJointIndex(mActor, base - 0x6CD3));
    joint->_64 = nullptr;

    joint = core->getJointTransform(MR::getJointIndex(mActor, base - 0x6C81));
    joint->_64 = nullptr;

    joint = core->getJointTransform(MR::getJointIndex(mActor, base - 0x6CD9));
    joint->_30 = 0.0f;

    joint = core->getJointTransform(MR::getJointIndex(mActor, base - 0x6C87));
    joint->_30 = 0.0f;

    joint = core->getJointTransform(MR::getJointIndex(mActor, jname_chest));
    joint->_64 = nullptr;

    joint = core->getJointTransform(MR::getJointIndex(mActor, lbl_806B228C));
    joint->_64 = nullptr;

    const TVec3f zero(0.0f, 0.0f, 0.0f);
    joint = core->getJointTransform(0);
    *reinterpret_cast<TVec3f*>(&joint->_38) = zero;

    _74 = 0;
    _70 = 0.0f;
}

bool MarioAnimator::isMirrorAnimation() {
    if (*reinterpret_cast<const u32*>(&mActor->_468) != 0) {
        return false;
    }

    TVec3f camDirX = getCamDirX();
    if (getCamDirY().dot(getPlayer()->_1FC) < 0.0f) {
        camDirX.x = -camDirX.x;
        camDirX.y = -camDirX.y;
        camDirX.z = -camDirX.z;
    }

    const char* base = lbl_805BAC68 + 0x10000;

    if (isAnimationRun(base - 0x6C7A) || isAnimationRun(base - 0x6C71)) {
        if (_10C == 0) {
            if (camDirX.dot(getPlayer()->getWallNorm()) < -0.5f) {
                _10D = 1;
            } else {
                _10D = 0;
            }
            _10C = 1;
        }
        return _10D != 0;
    }

    if ((getPlayer()->mMovementStates_HIGH_WORD & 0x00000100) &&
        (isAnimationRun(base - 0x6CAB) || isAnimationRun(base - 0x6C66) || isAnimationRun(base - 0x6C57))) {
        if (_10C == 0) {
            if (camDirX.dot(getPlayer()->mFrontVec) > 0.0f) {
                _10D = 1;
            } else {
                _10D = 0;
            }
            _10C = 1;
        }
        return _10D != 0;
    }

    _10C = 0;
    return isAnimationRun(base - 0x6C48);
}

void MarioAnimator::switchMirrorMode() {
    J3DModel* model = MR::getJ3DModel(mActor);
    const f32 unit = 1.0f;

    if (isMirrorAnimation()) {
        model->mModelData->mJointTree.mFlags |= 1;

        XjointTransform* joint = mXanimePlayer->mCore->getJointTransform(0);
        const TVec3f scale(unit, unit, -unit);
        *reinterpret_cast<TVec3f*>(&joint->_14) = scale;

        Mtx inv;
        PSMTXInverse(mActor->getBaseMtx(), inv);
        PSMTXConcat(MR::tmpMtxRotYRad(3.1415927f), inv, _DC.toMtxPtr());
        PSMTXConcat(mActor->getBaseMtx(), _DC.toMtxPtr(), _DC.toMtxPtr());
        joint->_6C = reinterpret_cast<u32>(_DC.toMtxPtr());
    } else {
        model->mModelData->mJointTree.mFlags &= ~1u;

        XjointTransform* joint = mXanimePlayer->mCore->getJointTransform(0);
        const TVec3f scale(unit, unit, unit);
        *reinterpret_cast<TVec3f*>(&joint->_14) = scale;
        joint->_6C = 0;
    }
}

void MarioAnimator::changePickupAnimation(const HitSensor* pSensor) {
    const char* base = lbl_805BAC68 + 0x10000;

    if (pSensor->mType == 0x19) {
        mActor->_494 = reinterpret_cast<u32>(mActor->_49C);
        changeAnimation(base - 0x6C3D, static_cast<const char*>(nullptr));
        mActor->changeNullAnimation(base - 0x6C34, -2);
        getPlayer()->stopWalk();
        return;
    }

    if (pSensor->mType != 0x0F) {
        return;
    }

    mActor->_494 = reinterpret_cast<u32>(mActor->_498);

    if (getPlayer()->isSwimming()) {
        mActor->clearNullAnimation(0);
        startPadVib(2);
        return;
    }

    if (mActor->_424 == reinterpret_cast<u32>(pSensor)) {
        changeAnimationUpper(base - 0x6C2C, nullptr);
        playEffect(base - 0x6C2C);
        mActor->clearNullAnimation(-3);
        startPadVib(2);
        return;
    }

    if ((getPlayer()->mMovementStates_LOW_WORD & 0x80000000) && !(getPlayer()->mMovementStates_LOW_WORD & 0x00100000)) {
        changeAnimation(base - 0x6C1D, static_cast<const char*>(nullptr));
        mActor->clearNullAnimation(-3);
        getPlayer()->stopWalk();
        return;
    }

    changeAnimation(base - 0x6C12, static_cast<const char*>(nullptr));
    mActor->changeNullAnimation(base - 0x6C0B, -2);
    getPlayer()->stopWalk();
}

void MarioAnimator::updateTakingAnimation(const HitSensor* pSensor) {
    const char* base = lbl_805BAC68 + 0x10000;

    if (!pSensor) {
        return;
    }

    if (MR::isDead(pSensor->mHost)) {
        if (*reinterpret_cast<const u32*>(&mActor->_468) != 0) {
            mActor->rushDropThrowMemoSensor();
        }
        return;
    }

    if (pSensor->mType == 0x19) {
        stopAnimation(static_cast<const char*>(nullptr), static_cast<const char*>(nullptr));
        changeAnimationUpper(base - 0x6C00, nullptr);
        mActor->clearNullAnimation(0);
        mActor->offTakingFlag();
        return;
    }

    if (pSensor->mType != 0x0F) {
        return;
    }

    if (getPlayer()->isSwimming()) {
        MR::emitEffect(pSensor->mHost, base - 0x6BE4);
        return;
    }

    bool ready = true;
    if (isAnimationRun(base - 0x6C2C)) {
        ready = isAnimationTerminateUpper(nullptr);
    }

    if (!ready) {
        return;
    }

    if (isAnimationRun(base - 0x6C1D)) {
        return;
    }

    if (!mActor->isStopNullAnimation()) {
        return;
    }

    changeAnimationUpper(base - 0x6BF3, nullptr);
    startPadVib(2);
    mActor->clearNullAnimation(0);
    mActor->offTakingFlag();
}

void MarioAnimator::changeThrowAnimation(const HitSensor* pSensor) {
    const char* base = lbl_805BAC68 + 0x10000;

    if (pSensor->mType == 0x19) {
        stopAnimationUpper(nullptr, nullptr);
        changeAnimation(base - 0x6BD9, static_cast<const char*>(nullptr));
        startPadVib(base - 0x6BD0);
        return;
    }

    if (pSensor->mType != 0x0F) {
        return;
    }

    stopAnimationUpper(nullptr, nullptr);

    if (getPlayer()->isSwimming()) {
        changeAnimation(base - 0x6BC1, base - 0x6BB6);
    } else {
        changeAnimation(base - 0x6BAD, static_cast<const char*>(nullptr));
    }

    startPadVib(base - 0x6BD0);
}

void MarioAnimator::waterToGround() {
    HitSensor* sensor = nullptr;
    if (*reinterpret_cast<const u32*>(&mActor->_468) != 0) {
        sensor = reinterpret_cast<HitSensor*>(mActor->_428);
    }

    if (!sensor) {
        return;
    }

    if (sensor->mType == 0x0F) {
        changeAnimationUpper(lbl_805C4075, nullptr);
        mActor->clearNullAnimation(0);
        MR::deleteEffect(sensor->mHost, lbl_805C4084);
        return;
    }

    if (sensor->mType == 0x19) {
        changeAnimationUpper(lbl_805C4068, nullptr);
        mActor->clearNullAnimation(0);
    }
}

void MarioAnimator::initWalkWeight() {
    _18 = 0.0f;
    _1C = 0.0f;
    _20 = 0.0f;
    _24 = 1.0f;
}

HitSensor* MarioActor::getLookTargetSensor() const {
    if (_934) {
        return nullptr;
    }

    if (isSleeping()) {
        return nullptr;
    }

    return *reinterpret_cast<HitSensor* const*>(reinterpret_cast<const u8*>(this) + 0x46C);
}

extern "C" void __sinit_MarioAnimator_cpp() {
    __ct__Q213NrvMarioActor17MarioActorNrvWaitFv(&sInstance__Q213NrvMarioActor17MarioActorNrvWait);
    __ct__Q213NrvMarioActor21MarioActorNrvGameOverFv(&sInstance__Q213NrvMarioActor21MarioActorNrvGameOver);
    __ct__Q213NrvMarioActor26MarioActorNrvGameOverAbyssFv(&sInstance__Q213NrvMarioActor26MarioActorNrvGameOverAbyss);
    __ct__Q213NrvMarioActor27MarioActorNrvGameOverAbyss2Fv(&sInstance__Q213NrvMarioActor27MarioActorNrvGameOverAbyss2);
    __ct__Q213NrvMarioActor25MarioActorNrvGameOverFireFv(&sInstance__Q213NrvMarioActor25MarioActorNrvGameOverFire);
    __ct__Q213NrvMarioActor30MarioActorNrvGameOverBlackHoleFv(&sInstance__Q213NrvMarioActor30MarioActorNrvGameOverBlackHole);
    __ct__Q213NrvMarioActor28MarioActorNrvGameOverNonStopFv(&sInstance__Q213NrvMarioActor28MarioActorNrvGameOverNonStop);
    __ct__Q213NrvMarioActor25MarioActorNrvGameOverSinkFv(&sInstance__Q213NrvMarioActor25MarioActorNrvGameOverSink);
    __ct__Q213NrvMarioActor21MarioActorNrvTimeWaitFv(&sInstance__Q213NrvMarioActor21MarioActorNrvTimeWait);
    __ct__Q213NrvMarioActor19MarioActorNrvNoRushFv(&sInstance__Q213NrvMarioActor19MarioActorNrvNoRush);
}

__declspec(section ".ctors") extern void* const __sinit_MarioAnimator_cpp_reference = __sinit_MarioAnimator_cpp;

f32 XanimePlayer::tellAnimationFrame() const {
    if (isTerminate()) {
        union {
            u64 u;
            f64 d;
        } conv;

        conv.u = (static_cast<u64>(0x43300000u) << 32) | static_cast<u32>(static_cast<s16>(_20->mEndFrame));
        return static_cast<f32>((conv.d - lbl_80539AB0) - lbl_80539AA8);
    }

    return _20->mCurrentFrame;
}
