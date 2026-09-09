#include "Game/LiveActor/LiveActorGroupArray.hpp"
#include "Game/LiveActor/HitSensor.hpp"
#include "Game/LiveActor/LiveActor.hpp"
#include "Game/Scene/SceneFunction.hpp"
#include "Game/Util/JMapIdInfo.hpp"
#include "Game/Util/JMapUtil.hpp"
#include "Game/Util/ObjUtil.hpp"
#include "Game/Util/StringUtil.hpp"
#include <cstdio>
#include <cstring>

MsgSharedGroup::MsgSharedGroup(const char* pName, s32 numMax, const JMapInfoIter& rIter)
    : LiveActorGroup(mGroupName, numMax), mIdInfo(), mMsg(-1), mSensor(), mSensorName() {
    MR::copyString(mGroupName, pName, sizeof(mGroupName));
    JMapIdInfo* pIdInfo = new JMapIdInfo();

    if (pIdInfo != nullptr) {
        JMapIdInfo newInfo = MR::createJMapIdInfoFromGroupId(rIter);
        pIdInfo->_0 = newInfo._0;
        pIdInfo->mZoneID = newInfo.mZoneID;
    }

    mIdInfo = pIdInfo;
}

void MsgSharedGroup::init(const JMapInfoIter& rIter) {
    MR::connectToScene(this, MR::MovementType_MsgSharedGroup, -1, -1, -1);
}

void MsgSharedGroup::movement() {
    NameObj::movement();

    if (mMsg == -1) {
        return;
    }

    for (s32 i = 0; i < getObjNum(); i++) {
        getActor(i)->getSensor(mSensorName)->receiveMessage(mMsg, mSensor);
    }

    mMsg = -1;
}

void MsgSharedGroup::sendMsgToGroupMember(u32 msg, HitSensor* pSensor, const char* pSensorName) {
    mMsg = msg;
    mSensor = pSensor;
    mSensorName = pSensorName;
}

LiveActorGroupArray::LiveActorGroupArray(const char* pName) : NameObj(pName), mGroups() {
}

void LiveActorGroupArray::init(const JMapInfoIter& rIter) {
}

LiveActorGroup* LiveActorGroupArray::getLiveActorGroup(const LiveActor* pActor) const {
    return findGroup(pActor);
}

LiveActorGroup* LiveActorGroupArray::createGroup(const JMapInfoIter& rIter, const char* pName, s32 numMax) {
    MsgSharedGroup* pGroup;

    pGroup = new MsgSharedGroup(pName, numMax, rIter);
    pGroup->initWithoutIter();

    mGroups.push_back(pGroup);

    return pGroup;
}

LiveActorGroup* LiveActorGroupArray::findGroup(const LiveActor* pActor) const {
    MsgSharedGroup* const* it;

    for (it = mGroups.begin(); it != mGroups.end(); it++) {
        if ((*it)->hasActor(pActor)) {
            break;
        }
    }

    if (it != mGroups.end()) {
        return *it;
    }

    return nullptr;
}

LiveActorGroup* LiveActorGroupArray::findGroup(const JMapInfoIter& rIter) const {
    JMapIdInfo idInfo = MR::createJMapIdInfoFromGroupId(rIter);
    MsgSharedGroup* const* it;

    for (it = mGroups.begin(); it != mGroups.end(); it++) {
        if (*(*it)->mIdInfo == idInfo) {
            break;
        }
    }

    if (it != mGroups.end()) {
        return *it;
    }

    return nullptr;
}

LiveActorGroup* LiveActorGroupArray::entry(LiveActor* pActor, const JMapInfoIter& rIter, const char* pName, s32 numMax) {
    s32 groupID = -1;
    MR::getJMapInfoGroupID(rIter, &groupID);

    if (pName == nullptr) {
        char defaultName[32];
        snprintf(defaultName, sizeof(defaultName), "group%02d", groupID);

        pName = defaultName;
    }

    LiveActorGroup* pGroup = findGroup(rIter);

    if (pGroup == nullptr) {
        pGroup = createGroup(rIter, pName, numMax);
    }

    pGroup->registerActor(pActor);

    return pGroup;
}
