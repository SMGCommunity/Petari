#include "Game/NPC/NPCActor.hpp"
#include "Game/Enemy/AnimScaleController.hpp"
#include "Game/LiveActor/LiveActor.hpp"
#include "Game/LiveActor/LodCtrl.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/LiveActor/PartsModel.hpp"
#include "Game/LiveActor/Spine.hpp"
#include "Game/NPC/NPCActorItem.hpp"
#include "Game/Util.hpp"
#include "revolution/types.h"

namespace NrvNPCActor {
    NEW_NERVE(NPCActorNrvReaction, NPCActor, Reaction);
    NEW_NERVE(NPCActorNrvWait, NPCActor, Wait);
    NEW_NERVE(NPCActorNrvTalk, NPCActor, Talk);
    NEW_NERVE(NPCActorNrvNull, NPCActor, Null);
};  // namespace NrvNPCActor

namespace {
    class NPCParam : public AnimScaleParam {
    public:
        NPCParam() : AnimScaleParam() {
            _10 = 15.0f;
            _14 = 1.4;
            _20 = 0.3f;
        };
    };

    static NPCParam sParam;
}  // namespace

NPCActorCaps::NPCActorCaps(const char* pName) {
    _0 = pName;
    mMakeActor = false;
    mHostIO = false;
    mInterpole = false;
    mConnectTo = false;
    mLightCtrl = false;
    mEffect = false;
    mSound = false;
    mAttribute = false;
    mPosition = false;
    mLodCtrl = false;
    mNerve = false;
    mSensor = false;
    mBinder = false;
    mShadow = false;
    _5D = false;
    mRailRider = false;
    mSwitchDead = true;
    mSwitchAppear = false;
    _67 = false;
    mMakeActor = false;
    mHostIO = false;
    mInterpole = false;
    mConnectTo = false;
    mLightCtrl = false;
    mEffect = false;
    mSound = false;
    mAttribute = false;
    mPosition = false;
    mLodCtrl = false;
    mNerve = false;
    mSensor = false;
    mBinder = false;
    mShadow = false;
    _5D = false;
    mRailRider = false;
    mSwitchDead = true;
    mSwitchAppear = false;
    _67 = false;
    mBinderSize = 50.0f;
    mSensorSize = 50.0f;
    mShadowSize = 50.0f;
    mPointerSize = 80.0f;
    mSoundSize = 4;
    mSceneConnectionType = 0;
    mPointer = false;
    mStarPointerOffs.x = 0.0f;
    mStarPointerOffs.y = 0.0f;
    mStarPointerOffs.z = 0.0f;
    mModel = false;
    mMessage = false;
    _F = false;
    mModel = false;
    mMessage = false;
    _F = false;
    mMessageOffset.x = 0.0f;
    mMessageOffset.y = 150.0f;
    mMessageOffset.z = 0.0f;
    mSensorOffset.set< f32 >(0.0f, 50.0f, 0.0f);
    _10 = pName;
    mTalkMtx = nullptr;
    mTalkJointName = nullptr;
    mTalkMtx = nullptr;
    mTalkJointName = nullptr;
    mObjectName = pName;
    mSensorJoint = nullptr;
    mSensorJoint = nullptr;
    mSensorMax = 1;
    mWaitNerve = &NrvNPCActor::NPCActorNrvWait::sInstance;
    mTalkNerve = &NrvNPCActor::NPCActorNrvTalk::sInstance;
    mReactionNerve = &NrvNPCActor::NPCActorNrvReaction::sInstance;
    _70 = nullptr;
    _6C = nullptr;
    _70 = nullptr;
    _6C = nullptr;
}

void NPCActorCaps::setDefault() {
    mMakeActor = true;
    mHostIO = true;
    mInterpole = true;
    mConnectTo = true;
    mLightCtrl = true;
    mEffect = true;
    mSound = true;
    mAttribute = true;
    mPosition = true;
    mLodCtrl = true;
    mNerve = true;
    mSensor = true;
    mBinder = true;
    mShadow = true;
    mRailRider = true;
    mSwitchDead = true;
    mSwitchAppear = true;
    mPointer = true;
    mModel = true;
    mMessage = true;
    mMakeActor = true;
    mHostIO = true;
    mInterpole = true;
    mConnectTo = true;
    mLightCtrl = true;
    mEffect = true;
    mSound = true;
    mAttribute = true;
    mPosition = true;
    mLodCtrl = true;
    mNerve = true;
    mSensor = true;
    mBinder = true;
    mShadow = true;
    mRailRider = true;
    mSwitchDead = true;
    mSwitchAppear = true;
    mPointer = true;
    mModel = true;
    mMessage = true;
}

void NPCActorCaps::setIndirect() {
    mSceneConnectionType = 2;
}

NPCActor::NPCActor(const char* pName) : LiveActor(pName) {
    mLodCtrl = nullptr;
    mMsgCtrl = nullptr;
    _94 = nullptr;
    _98 = nullptr;
    mLodCtrl = nullptr;
    mMsgCtrl = nullptr;
    _94 = nullptr;
    _98 = nullptr;
    _9C = 0;
    _D8 = 0;
    _D9 = 0;
    _DA = 0;
    _DB = 0;
    _DC = 0;
    _DD = 0;
    _DE = 0;
    _DF = 0;
    _E0 = 0;
    _E1 = 0;
    _E2 = 0;
    _E3 = 0;
    _E4 = 0;
    _E5 = 0;
    _E6 = 0;
    mParam._0 = 1;
    mParam._1 = 1;
    mParam._4 = 2000.0f;
    mParam._8 = 4.0f;
    mParam._C = 0.0f;
    mParam._10 = 0.0f;
    mParam._14 = nullptr;
    mParam._18 = nullptr;
    mParam._1C = nullptr;
    mParam._20 = nullptr;
    _11C = nullptr;
    _120 = nullptr;
    _10C = 2.0f;
    _110 = 0.1f;
    _114 = 0.08f;
    _118 = 1.0f;
    _124 = 0;
    _128 = 1;
    _12C = 0.0f;
    _130 = 0;
    _134 = 0;
    _138 = 0;
    _13C = 0;
    mScaleController = nullptr;
    mDelegator = nullptr;
    mCurNerve = nullptr;
    _158 = 0x400;
    _A0.set< f32 >(0.0f, 0.0f, 0.0f, 1.0f);
    _B0.set< f32 >(0.0f, 0.0f, 0.0f, 1.0f);
    _C0.zero();
    _CC.zero();
    mWaitNerve = &NrvNPCActor::NPCActorNrvWait::sInstance;
    mTalkNerve = &NrvNPCActor::NPCActorNrvTalk::sInstance;
    mReactionNerve = &NrvNPCActor::NPCActorNrvReaction::sInstance;
}

void NPCActor::makeArchiveList(NameObjArchiveListCollector* pCollector, const JMapInfoIter& rIter) {
    const char* name;
    MR::getObjectName(&name, rIter);
    NPCActorItem item(name);
    s32 item_type = -1;
    MR::getJMapInfoArg7NoInit(rIter, &item_type);

    bool ret = MR::getNPCItemData(&item, item_type);

    if (ret) {
        NPCActor::addArchive(pCollector, item);
    }
}

void NPCActor::addArchive(NameObjArchiveListCollector* pCollector, const NPCActorItem& rItem) {
    if (!MR::isNullOrEmptyString(rItem.mGoods0) && MR::isNPCItemFileExist(rItem.mGoods0)) {
        pCollector->addArchive(rItem.mGoods0);
    }

    if (!MR::isNullOrEmptyString(rItem.mGoods1) && MR::isNPCItemFileExist(rItem.mGoods1)) {
        pCollector->addArchive(rItem.mGoods1);
    }
}

void NPCActor::makeArchiveListDefault(NameObjArchiveListCollector* pCollector, const JMapInfoIter& rIter) {
    const char* name;
    MR::getObjectName(&name, rIter);
    NPCActorItem item(name);
    bool ret = MR::getNPCItemData(&item, 0);

    if (ret) {
        NPCActor::addArchive(pCollector, item);
    }
}

void NPCActor::setBaseMtx(const TPos3f& rPos) {
    TVec3f trans;
    rPos.getTransInline(trans);

    TVec3f eulerXYZ;
    // inlined in the assembly
    rPos.getEulerXYZ(eulerXYZ);
    MR::makeQuatRotateRadian(&_A0, eulerXYZ);
    _A0.normalize();

    mPosition.set< f32 >(trans);
    eulerXYZ = eulerXYZ * 57.29578f;
    mRotation.set< f32 >(eulerXYZ);
    _CC.set< f32 >(eulerXYZ);

    MR::setBaseTRMtx(this, _A0);
    MR::resetPosition(this);
}

void NPCActor::setBaseMtx(MtxPtr pMtx) {
    TPos3f pos;
    pos.setInline(pMtx);
    setBaseMtx(pos);
}

void NPCActor::setInitPose() {
    _B0.x = _A0.x;
    _B0.y = _A0.y;
    _B0.z = _A0.z;
    _B0.w = _A0.w;

    _C0.set< f32 >(mPosition);
}

void NPCActor::init(const JMapInfoIter& rIter) {
    LiveActor::init(rIter);
}

void NPCActor::initAfterPlacement() {
    MR::calcGravity(this);

    if (!MR::isNullOrEmptyString(mParam._14)) {
        MR::startAction(this, mParam._14);

        if (MR::isExistBck(this, mParam._14)) {
            MR::startBckNoInterpole(this, mParam._14);
        }

        MR::setBckFrameAtRandom(this);
        MR::calcAnimDirect(this);
    }
}

void NPCActor::initialize(const JMapInfoIter& rIter, const NPCActorCaps& rCaps) {
    if (rCaps.mPosition) {
        MR::initDefaultPosAndQuat(this, rIter);
    }

    if (rCaps.mModel) {
        initModelManagerWithAnm(rCaps.mObjectName, nullptr, false);
    }

    if (rCaps.mConnectTo) {
        if (rCaps.mSceneConnectionType == 0) {
            MR::connectToSceneNpc(this);
        } else if (rCaps.mSceneConnectionType == 1) {
            MR::connectToSceneNpcMovement(this);
        } else if (rCaps.mSceneConnectionType == 2) {
            MR::connectToSceneIndirectNpc(this);
        }
    }

    if (rCaps.mLightCtrl) {
        MR::initLightCtrl(this);
    }

    if (rCaps.mSound) {
        initSound(rCaps.mSoundSize, false);
    }

    if (rCaps.mAttribute) {
        MR::addToAttributeGroupSearchTurtle(this);
    }

    if (rCaps.mNerve) {
        mWaitNerve = rCaps.mWaitNerve;
        mTalkNerve = rCaps.mTalkNerve;
        mReactionNerve = rCaps.mReactionNerve;
        if (mWaitNerve != nullptr) {
            initNerve(mWaitNerve);
        }
    }

    if (rCaps.mSensor) {
        initHitSensor(rCaps.mSensorMax);
    }

    if (rCaps.mSensorJoint != nullptr) {
        MR::addHitSensorAtJointNpc(this, "Body", rCaps.mSensorJoint, 8, rCaps.mSensorSize, rCaps.mSensorOffset);
    } else {
        MR::addHitSensorNpc(this, "Body", 8, rCaps.mSensorSize, rCaps.mSensorOffset);
    }

    if (rCaps.mBinder) {
        initBinder(rCaps.mBinderSize, rCaps.mBinderSize, 0);
        MR::onCalcGravity(this);
    }

    if (rCaps.mEffect) {
        initEffectKeeper(0, nullptr, false);
    }

    if (rCaps._5D) {
        MR::initShadowFromCSV(this, "Shadow");
    } else if (rCaps.mShadow) {
        MR::initShadowVolumeSphere(this, rCaps.mShadowSize);
        MR::onCalcShadowOneTime(this, nullptr);
    }

    if (rCaps.mRailRider && MR::isConnectedWithRail(rIter)) {
        initRailRider(rIter);
        MR::moveCoordToStartPos(this);
        if (MR::isExistShadow(this, nullptr)) {
            MR::onCalcShadow(this, nullptr);
        }
    }

    if (rCaps.mPointer) {
        if (rCaps._6C != nullptr) {
            MR::initStarPointerTargetAtJoint(this, rCaps._6C, rCaps.mPointerSize, rCaps.mStarPointerOffs);
        } else {
            MR::initStarPointerTarget(this, rCaps.mPointerSize, rCaps.mStarPointerOffs);
        }
    }

    if (rCaps.mMessage) {
        MtxPtr talkMtx;
        if (rCaps.mTalkJointName != nullptr) {
            talkMtx = MR::getJointMtx(this, rCaps.mTalkJointName);
        } else {
            talkMtx = rCaps.mTalkMtx;
        }
        if (rCaps._F) {
            mMsgCtrl = MR::createTalkCtrlDirect(this, rIter, rCaps._10, rCaps.mMessageOffset, talkMtx);
            MR::onRootNodeAutomatic(mMsgCtrl);
        } else {
            initTalkCtrl(rIter, rCaps._10, rCaps.mMessageOffset, talkMtx);
        }
    }

    if (rCaps.mLodCtrl) {
        mLodCtrl = MR::createLodCtrlNPC(this, rIter);
    }

    MR::useStageSwitchReadA(this, rIter);
    MR::useStageSwitchReadB(this, rIter);
    MR::useStageSwitchSleep(this, rIter);

    if (rCaps.mSwitchDead) {
        MR::useStageSwitchWriteDead(this, rIter);
    }

    if (rCaps._70 != nullptr) {
        mScaleController = new AnimScaleController(&::sParam);
        mDelegator = MR::createJointDelegatorWithNullChildFunc(this, &calcJointScale, rCaps._70);
    }

    if (rCaps._67) {
        makeActorDead();
    } else if (rCaps.mSwitchAppear && MR::useStageSwitchReadAppear(this, rIter)) {
        MR::syncStageSwitchAppear(this);
        if (rCaps.mMakeActor) {
            makeActorDead();
        }
    } else if (rCaps.mMakeActor) {
        makeActorAppeared();
    }
}

void NPCActor::equipment(const NPCActorItem& rItem, bool indirect) {
    if (indirect) {
        _94 = MR::createIndirectNPCGoods(this, rItem.mGoods0, rItem.mGoodsJoint0);
        _98 = MR::createIndirectNPCGoods(this, rItem.mGoods1, rItem.mGoodsJoint1);
    } else {
        _94 = MR::createNPCGoods(this, rItem.mGoods0, rItem.mGoodsJoint0);
        _98 = MR::createNPCGoods(this, rItem.mGoods1, rItem.mGoodsJoint1);
    }

    if (_94 != nullptr) {
        MR::registerDemoSimpleCastAll(_94);
    }

    if (_98 != nullptr) {
        MR::registerDemoSimpleCastAll(_98);
    }
}

void NPCActor::calcAndSetBaseMtx() {
    _A0.normalize();
    if (!(_CC == mRotation)) {
        MR::makeQuatRotateDegree(&_A0, mRotation);
        _CC = mRotation;
    }

    MR::setBaseTRMtx(this, _A0);
    if (mDelegator != nullptr) {
        mDelegator->registerCallBack();
    }
}

void NPCActor::makeActorAppeared() {
    LiveActor::makeActorAppeared();
    if (mLodCtrl != nullptr) {
        mLodCtrl->appear();
    }

    if (_94 != nullptr) {
        _94->appear();
    }

    if (_98 != nullptr) {
        _98->appear();
    }
}

void NPCActor::makeActorDead() {
    if (mLodCtrl != nullptr) {
        mLodCtrl->kill();
    }

    if (_94 != nullptr) {
        _94->kill();
    }

    if (_98 != nullptr) {
        _98->kill();
    }

    LiveActor::makeActorDead();
}

void NPCActor::kill() {
    if (MR::isValidSwitchDead(this)) {
        MR::onSwitchDead(this);
    }

    LiveActor::kill();
}

void NPCActor::control() {
    if (isPointingSe()) {
        _158 = 0;
    } else {
        _158++;
    }

    s32 min = 0x400;
    if (_158 < 0x400) {
        min = _158;
    }
    _158 = min;

    updateReaction();

    if (_9C > 0) {
        _9C--;
    }

    if (mLodCtrl != nullptr) {
        mLodCtrl->update();
    }

    if (mScaleController != nullptr) {
        mScaleController->update();
    }

    if (mModelManager != nullptr && MR::checkPlayerSwingTrigger() && MR::isNearPlayer(this, _12C)) {
        _E3 = true;
    }

    if (mStarPointerTarget != nullptr && MR::isStarPointerPointing2POnPressButton(this, "弱", true, false)) {
        _E4 = true;
    }

    if (mScaleController != nullptr && mDelegator != nullptr) {
        updateScaleCtrl();
    }
}

bool NPCActor::initTalkCtrl(const JMapInfoIter& rIter, const char* pChar, const TVec3f& rVec, MtxPtr pMtx) {
    s32 messageID;
    MR::getJMapInfoMessageID(rIter, &messageID);
    if (messageID < 0) {
        return false;
    }

    mMsgCtrl = MR::createTalkCtrl(this, rIter, pChar, rVec, pMtx);
    MR::onRootNodeAutomatic(mMsgCtrl);
    return true;
}

bool NPCActor::initTalkCtrlDirect(const JMapInfoIter& rIter, const char* pChar, const TVec3f& rVec, MtxPtr pMtx) {
    mMsgCtrl = MR::createTalkCtrlDirect(this, rIter, pChar, rVec, pMtx);
    MR::onRootNodeAutomatic(mMsgCtrl);
    return true;
}

bool NPCActor::calcJointScale(TPos3f* pPos, const JointControllerInfo&) {
    MR::preScaleMtx(pPos->toMtxPtr(), mScaleController->_C);
    return true;
}

bool NPCActor::turnToPlayer() {
    return turnToPlayer(5.0f);
}

namespace {
    void calcVecToPlayer(TVec3f* pOut, const LiveActor* pActor) {
        TVec3f playerTrans;
        MR::extractMtxTrans(MR::getPlayerBaseMtx(), &playerTrans);
        MR::normalizeOrZero(playerTrans - pActor->mPosition, pOut);
    }
}  // namespace

bool NPCActor::turnToPlayer(f32 f1) {
    TVec3f zDir;
    TVec3f toPlayer;
    ::calcVecToPlayer(&toPlayer, this);
    _A0.getZDir(zDir);

    if (MR::isOppositeDirection(-zDir, toPlayer, 0.01f)) {
        return true;
    }

    return MR::faceToVector(&_A0, toPlayer, f1);
}

bool NPCActor::turnToPlayer(f32 f1, f32 f2, f32 f3) {
    bool turned = turnToPlayer(f1);
    if (f3 == 0.0f) {
        return turned;
    }

    TVec3f toPlayer;
    TVec3f yDir;
    _B0.getYDir(yDir);
    ::calcVecToPlayer(&toPlayer, this);

    if (MR::isSameDirection(toPlayer, yDir, 0.01f)) {
        return false;
    }

    TVec3f vec1;
    TVec3f vec2;
    MR::makeAxisFrontUp(&vec1, &vec2, toPlayer, yDir);
    MR::clampVecAngleDeg(&vec2, yDir, f3);
    // probably a typo
    return MR::turnQuatYDirRad(&_A0, _A0, vec2, f2 * 0.17453294f) & turned;
}

bool NPCActor::turnToDefault(f32 f1) {
    TVec3f zDir;
    TVec3f zDir2;
    TVec3f yDir;
    _B0.getZDir(zDir);
    _B0.getYDir(yDir);
    _A0.getZDir(zDir2);

    if (MR::isOppositeDirection(-zDir, zDir2, 0.01f)) {
        return true;
    }

    f32 dot = zDir.dot(zDir2);
    dot = MR::clamp(dot, -1.0f, 1.0f);

    f32 flt = __fabsf((f1 * 0.17453294f) / JMAAcosRadian(dot));
    flt = MR::clamp(flt, 0.0f, 1.0f);

    MR::blendQuatUpFront(&_A0, yDir, zDir, flt, flt);
    return MR::isSameDirection(zDir2, zDir, 0.01f);
}

void NPCActor::setToDefault() {
    mPosition = _C0;
    _A0 = _B0;
    _A0.normalize();

    TVec3f euler;
    _A0.getEuler(euler);
    euler = euler * 57.29578f;
    mRotation = euler;
    _CC = euler;

    MR::setBaseTRMtx(this, _A0);
    MR::resetPosition(this);
}

void NPCActor::attackSensor(HitSensor* pSender, HitSensor* pReceiver) {
    if (MR::isSensorPlayerOrRide(pReceiver)) {
        MR::sendMsgPush(pReceiver, pSender);
    }

    if (MR::isSensorEnemy(pReceiver)) {
        MR::sendMsgPush(pReceiver, pSender);
    }
}

bool NPCActor::receiveMsgPlayerAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    if (MR::isMsgPlayerTrample(msg)) {
        if (_9C == 0) {
            _9C = 30;
            _E2 = true;
            return true;
        }
        return false;
    }

    if (MR::isMsgPlayerHipDrop(msg)) {
        _E2 = true;
        MR::sendMsgAwayJump(pSender, pReceiver);
        return true;
    }

    if (MR::isMsgPlayerSpinAttack(msg) && MR::isNearZero(_12C)) {
        _E3 = true;
        return false;
    }

    if (MR::isMsgStarPieceReflect(msg)) {
        _E5 = true;
        return true;
    }

    // unused
    if (MR::isMsgStarPieceAttack(msg)) {
    }

    if (MR::isMsgJetTurtleAttack(msg)) {
        _E5 = true;
        _E6 = true;
        return true;
    }
    return false;
}

void NPCActor::pushNerve(const Nerve* pNerve) {
    mCurNerve = mSpine->getCurrentNerve();
    setNerve(pNerve);
}

const Nerve* NPCActor::popAndPushNerve(const Nerve* pNerve) {
    const Nerve* poppedNerve = popNerve();
    pushNerve(pNerve);
    return poppedNerve;
}

const Nerve* NPCActor::popNerve() {
    const Nerve* currentNerve = mSpine->getCurrentNerve();
    setNerve(mCurNerve);
    mCurNerve = nullptr;
    return currentNerve;
}

bool NPCActor::tryPushNullNerve() {
    if (isEmptyNerve()) {
        pushNerve(&NrvNPCActor::NPCActorNrvNull::sInstance);
        return true;
    }

    return false;
}

bool NPCActor::isEmptyNerve() const {
    return mCurNerve == nullptr;
}

bool NPCActor::isScaleAnim() const {
    // non-matching
    AnimScaleController* scaleController = mScaleController;
    bool retval;
    if (mScaleController == nullptr) {
        return false;
    } else {
        retval = !MR::isNearZero(1.0f - scaleController->_C.x, 0.2f) || !MR::isNearZero(1.0f - scaleController->_C.y, 0.2f) ||
                 !MR::isNearZero(1.0f - scaleController->_C.z, 0.2f);
    }

    return retval;
}

bool NPCActor::isPointingSe() const {
    if (_DA && _158 >= 60) {
        return true;
    }

    return false;
}

void NPCActor::updateReaction() {
    _D8 = 0;
    _D9 = 0;
    _DA = 0;
    _DB = 0;
    _DC = 0;

    if (!_DD && _E2) {
        _D8 = true;
    } else if (!_E0 && _E5) {
        _DB = true;
        _DC = !_E1 && _E6;
    } else if (!_DE && _E3) {
        _D9 = true;
    } else if (!_DF && _E4) {
        _DA = true;
    }

    _DD = _E2;
    _DE = _E3;
    _DF = _E4;
    _E0 = _E5;
    _E1 = _E6;
    _E2 = false;
    _E3 = false;
    _E4 = false;
    _E5 = false;
    _E6 = false;
}

void NPCActor::updateScaleCtrl() {
    if (_128) {
        if (_D9 && MR::isNullOrEmptyString(_130)) {
            mScaleController->startHitReaction();
        }

        if (_D8 && MR::isNullOrEmptyString(_134)) {
            mScaleController->startHitReaction();
        }

        if (_DB && MR::isNullOrEmptyString(_13C)) {
            mScaleController->startHitReaction();
        }

        if (!_DF && _E4) {
            if (MR::isNullOrEmptyString(_138)) {
                mScaleController->startDpdHitVibration();
            }
        } else if (_DF && !_E4 && MR::isNullOrEmptyString(_138)) {
            mScaleController->startAnim();
        }
    } else {
        if (_D9 || _D8 || _DB) {
            mScaleController->startHitReaction();
        }

        if (!_DF && _E4) {
            mScaleController->startDpdHitVibration();
        } else if (_DF && !_E4) {
            mScaleController->startAnim();
        }
    }
}

void NPCActor::exeReaction() {
    MR::tryStartReactionAndPopNerve(this);
}

void NPCActor::exeWait() {
    if (!MR::tryStartReactionAndPushNerve(this, mReactionNerve)) {
        if (mMsgCtrl != nullptr) {
            if (MR::tryTalkNearPlayerAndStartTalkAction(this)) {
                setNerve(mTalkNerve);
            }
        } else {
            MR::tryStartTurnAction(this);
        }
    }
}

void NPCActor::exeTalk() {
    if (!MR::tryStartReactionAndPushNerve(this, mReactionNerve) && (MR::tryTalkNearPlayerAtEndAndStartMoveTalkAction(this))) {
        setNerve(mWaitNerve);
    }
}

void NPCActor::exeNull() {}
