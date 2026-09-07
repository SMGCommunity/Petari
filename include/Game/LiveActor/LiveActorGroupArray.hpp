#pragma once

#include "Game/LiveActor/LiveActorGroup.hpp"
#include "Game/Util/Array.hpp"

class HitSensor;
class JMapIdInfo;

class MsgSharedGroup : public LiveActorGroup {
public:
    MsgSharedGroup(const char*, s32, const JMapInfoIter&);

    virtual void init(const JMapInfoIter&) override;
    virtual void movement() override;

    void sendMsgToGroupMember(u32, HitSensor*, const char*);

    /* 0x18 */ JMapIdInfo* mIdInfo;
    /* 0x1C */ u32 mMsg;
    /* 0x20 */ HitSensor* mSensor;
    /* 0x24 */ const char* mSensorName;
    /* 0x28 */ char mGroupName[48];
};

class LiveActorGroupArray : public NameObj {
public:
    LiveActorGroupArray(const char* pName);

    virtual void init(const JMapInfoIter&);

    LiveActorGroup* getLiveActorGroup(const LiveActor*) const;
    LiveActorGroup* createGroup(const JMapInfoIter&, const char*, s32);
    LiveActorGroup* findGroup(const LiveActor*) const;
    LiveActorGroup* findGroup(const JMapInfoIter&) const;
    LiveActorGroup* entry(LiveActor*, const JMapInfoIter&, const char*, s32);

    /* 0x0C */ MR::Vector< MR::FixedArray< MsgSharedGroup*, 32 > > mGroups;
};
