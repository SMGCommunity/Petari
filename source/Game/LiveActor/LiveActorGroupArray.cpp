#include "Game/LiveActor/LiveActorGroupArray.h"
#include "Game/LiveActor/LiveActor.h"

MsgSharedGroup::MsgSharedGroup(const char *pName, s32 a2, const JMapInfoIter &rIter) : LiveActorGroup(_28, a2) {
    mIDInfo = 0;
    _1C = -1;
    _20 = 0;
    _24 = 0;
    MR::copyString(_28, pName, 0x30);
    JMapIdInfo* inf = new JMapIdInfo();

    if (inf) {
        JMapIdInfo newInfo = MR::createJMapIdInfoFromClippingGroupId(rIter);
        inf->_0 = newInfo._0;
        inf->mZoneID = newInfo.mZoneID;
    }

    mIDInfo = inf;
}

void MsgSharedGroup::init(const JMapInfoIter &rIter) {
    MR::connectToScene(this, 6, -1, -1, -1);
}

void MsgSharedGroup::movement() {
    NameObj::movement();

    if (_1C != 0xFFFFFFFF) {
        for (s32 i = 0; i < mObjectCount; i++) {
            LiveActor* actor = getActor(i);
            HitSensor* sensor = actor->getSensor(_24);
            sensor->receiveMessage(_1C, _20);
        }

        _1C = -1;
    }
}

void MsgSharedGroup::sendMsgToGroupMember(u32 msg, HitSensor *pSensor, const char *pName) {
    _1C = msg;
    _20 = pSensor;
    _24 = pName;
}

LiveActorGroupArray::LiveActorGroupArray(const char *pName) : NameObj(pName) {
    mNumGroups = 0;
}

void LiveActorGroupArray::init(const JMapInfoIter &rIter) {

}

LiveActorGroup* LiveActorGroupArray::getLiveActorGroup(const LiveActor *pActor) const {
    return findGroup(pActor);
}

MsgSharedGroup* LiveActorGroupArray::createGroup(const JMapInfoIter &rIter, const char *pName, s32 msg) {
    MsgSharedGroup* group = new MsgSharedGroup(pName, msg, rIter);
    group->initWithoutIter();
    s32 cnt = mNumGroups;
    mNumGroups = cnt + 1;
    mGroups[cnt] = group;
    return group;
}