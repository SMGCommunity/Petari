#include "Game/NPC/NPCActor.h"
#include "Game/Util.h"

NPCActorCaps::NPCActorCaps(const char *pName) {
    _0 = pName;
    _C = 0;
    _D = 0;
    _28 = 0;
    _29 = 0;
    _2A = 0;
    _2B = 0;
    _2C = 0;
    _34 = 0;
    _35 = 0;
    _36 = 0;
    _37 = 0;
    _40 = 0;
    _38 = 0;
    _5C = 0;
    _5D = 0;
    _64 = 0;
    _65 = 1;
    _66 = 0;
    _67 = 0;
    _3C = 50.0f;
    _48 = 50.0f;
    _60 = 50.0f;
    _80 = 80.0f;
    _30 = 4;
    _84 = 0;
    _68 = 0;
    _74 = 0.0f;
    _78 = 0.0f;
    _7C = 0.0f;
    _4 = 0;
    _E = 0;
    _F = 0;
    _14 = 0.0f;
    _18 = 150.0f;
    _1C = 0.0f;
    _4C.set<f32>(0.0f, 50.0f, 0.0f);
    _10 = pName;
    _20 = 0;
    _24 = 0;
    _8 = pName;
    _44 = 0;
    _58 = 1;
    mWaitNerve = &NrvNPCActor::NPCActorNrvWait::sInstance;
    mTalkNerve = &NrvNPCActor::NPCActorNrvTalk::sInstance;
    mReactionNerve = &NrvNPCActor::NPCActorNrvReaction::sInstance;
    _70 = 0;
    _6C = 0;
}

void NPCActorCaps::setDefault() {
    _C = 1;
    _D = 1;
    _28 = 1;
    _29 = 1;
    _2A = 1;
    _2B = 1;
    _2C = 1;
    _34 = 1;
    _35 = 1;
    _36 = 1;
    _37 = 1;
    _40 = 1;
    _38 = 1;
    _5C = 1;
    _64 = 1;
    _65 = 1;
    _66 = 1;
    _68 = 1;
    _4 = 1;
    _E = 1;
}

void NPCActorCaps::setIndirect() {
    _84 = 2;
}

#ifdef NON_MATCHING
NPCActor::NPCActor(const char *pName) : LiveActor(pName) {
    _8C = 0;
    _90 = 0;
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
    _E8 = 1;
    _E9 = 1;
    _EC = 2000.0f;
    _F0 = 4.0f;
    _F4 = 0.0f;
    _F8 = 0.0f;
    _FC = 0;
    _100 = 0;
    _104 = 0;
    _108 = 0;
    _11C = 0;
    _120 = 0;
    _10C = 2.0f;
    _110 = 0.1f;
    _114 = 0.079999998f;
    _118 = 1.0f;
    _124 = 0;
    _128 = 1;
    _12C = 0.0f;
    _130 = 0;
    _134 = 0;
    _138 = 0;
    _13C = 0;
    _140 = 0;
    _144 = 0;
    _148 = 0;
    _158 = 0x400;
    _A0.set(0.0f, 0.0f, 0.0f, 1.0f);
    _B0.set(0.0f, 0.0f, 0.0f, 1.0f);
    _C0.zero();
    _CC.zero();
    mWaitNerve = &NrvNPCActor::NPCActorNrvWait::sInstance;
    mTalkNerve = &NrvNPCActor::NPCActorNrvTalk::sInstance;
    mReactionNerve = &NrvNPCActor::NPCActorNrvReaction::sInstance;
}
#endif

void NPCActor::makeArchiveList(NameObjArchiveListCollector *pCollector, const JMapInfoIter &rIter) {
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

void NPCActor::addArchive(NameObjArchiveListCollector *pCollector, const NPCActorItem &rItem) {
    if (!MR::isNullOrEmptyString(rItem.mName) && MR::isNPCItemFileExist(rItem.mName)) {
        pCollector->addArchive(rItem.mName);
    }

    if (!MR::isNullOrEmptyString(rItem._C) && MR::isNPCItemFileExist(rItem._C)) {
        pCollector->addArchive(rItem._C);
    }
}

void NPCActor::makeArchiveListDefault(NameObjArchiveListCollector *pCollector, const JMapInfoIter &rIter) {
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
    _B0.h = _A0.h;
    _C0.set<f32>(mPosition);
}

void NPCActor::init(const JMapInfoIter &rIter) {
    LiveActor::init(rIter);
}

void NPCActor::initAfterPlacement() {
    MR::calcGravity(this);

    if (!MR::isNullOrEmptyString(_FC)) {
        MR::startAction(this, _FC);

        if (MR::isExistBck(this, _FC)) {
            MR::startBckNoInterpole(this, _FC);
        }

        MR::setBckFrameAtRandom(this);
        MR::calcAnimDirect(this);
    }
}