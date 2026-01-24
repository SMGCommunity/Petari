#include "Game/NPC/NPCActor.hpp"
#include "Game/NPC/NPCActorItem.hpp"
#include "Game/Util.hpp"
#include "Game/Util/NPCUtil.hpp"

namespace NrvNPCActor {
    NEW_NERVE(NPCActorNrvReaction, NPCActor, Reaction);
    NEW_NERVE(NPCActorNrvWait, NPCActor, Wait);
    NEW_NERVE(NPCActorNrvTalk, NPCActor, Talk);
    NEW_NERVE(NPCActorNrvNull, NPCActor, Null);
};  // namespace NrvNPCActor

NPCActorCaps::NPCActorCaps(const char* pName) {
    _0 = pName;
    mMakeActor = 0;
    mHostIO = 0;
    mInterpole = 0;
    mConnectTo = 0;
    mLightCtrl = 0;
    mEffect = 0;
    mSound = 0;
    mAttribute = 0;
    mPosition = 0;
    mLodCtrl = 0;
    mNerve = 0;
    mSensor = 0;
    mBinder = 0;
    mShadow = 0;
    _5D = 0;
    mRailRider = 0;
    mSwitchDead = 1;
    mSwitchAppear = 0;
    _67 = 0;
    mBinderSize = 50.0f;
    mSensorSize = 50.0f;
    mShadowSize = 50.0f;
    mPointerSize = 80.0f;
    mSoundSize = 4;
    mSceneConnectionType = 0;
    mPointer = 0;
    mStarPointerOffs.x = 0.0f;
    mStarPointerOffs.y = 0.0f;
    mStarPointerOffs.z = 0.0f;
    mModel = 0;
    mMessage = 0;
    _F = 0;
    mMessageOffset.x = 0.0f;
    mMessageOffset.y = 150.0f;
    mMessageOffset.z = 0.0f;
    mSensorOffset.set(0.0f, 50.0f, 0.0f);
    _10 = pName;
    mTalkMtx = 0;
    mTalkJointName = 0;
    mObjectName = pName;
    mSensorJoint = 0;
    mSensorMax = 1;
    mWaitNerve = &NrvNPCActor::NPCActorNrvWait::sInstance;
    mTalkNerve = &NrvNPCActor::NPCActorNrvTalk::sInstance;
    mReactionNerve = &NrvNPCActor::NPCActorNrvReaction::sInstance;
    _70 = 0;
    _6C = 0;
}

void NPCActorCaps::setDefault() {
    mMakeActor = 1;
    mHostIO = 1;
    mInterpole = 1;
    mConnectTo = 1;
    mLightCtrl = 1;
    mEffect = 1;
    mSound = 1;
    mAttribute = 1;
    mPosition = 1;
    mLodCtrl = 1;
    mNerve = 1;
    mSensor = 1;
    mBinder = 1;
    mShadow = 1;
    mRailRider = 1;
    mSwitchDead = 1;
    mSwitchAppear = 1;
    mPointer = 1;
    mModel = 1;
    mMessage = 1;
}

void NPCActorCaps::setIndirect() {
    mSceneConnectionType = 2;
}

NPCActor::NPCActor(const char* pName) : LiveActor(pName) {
    mLodCtrl = 0;
    mMsgCtrl = 0;
    _94 = 0;
    _98 = 0;
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
    mParam._14 = 0;
    // todo -- find me
    //_100 = 0;
    //_104 = 0;
    //_108 = 0;
    _11C = 0;
    _120 = 0;
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
    //_140 = 0;
    _144 = 0;
    //_148 = 0;
    _158 = 0x400;
    _A0.set(0.0f, 0.0f, 0.0f, 1.0f);
    _B0.set(0.0f, 0.0f, 0.0f, 1.0f);
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
    if (!MR::isNullOrEmptyString(rItem.mActor) && MR::isNPCItemFileExist(rItem.mActor)) {
        pCollector->addArchive(rItem.mActor);
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

void NPCActor::exeReaction() {
    MR::tryStartReactionAndPopNerve(this);
}

// void NPCActor::exeWait()
// void NPCActor::exeTalk()

void NPCActor::exeNull() {}
