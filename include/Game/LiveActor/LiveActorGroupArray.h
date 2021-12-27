#pragma once

#include "Game/LiveActor/LiveActorGroup.h"
#include "Game/Util.h"

class HitSensor;

class MsgSharedGroup : public LiveActorGroup {
public:
    MsgSharedGroup(const char *, s32, const JMapInfoIter &);

    virtual void init(const JMapInfoIter &);
    virtual void movement();

    void sendMsgToGroupMember(u32, HitSensor *, const char *);

    JMapIdInfo* mIDInfo;    // _18
    s32 _1C;
    HitSensor* _20;
    const char* _24;
    char _28[0x30];
};

class LiveActorGroupArray : NameObj {
public:
    LiveActorGroupArray(const char *pName);

    virtual void init(const JMapInfoIter &);

    LiveActorGroup* getLiveActorGroup(const LiveActor *) const;
    MsgSharedGroup* createGroup(const JMapInfoIter &, const char *, s32);
    LiveActorGroup* findGroup(const LiveActor *) const;
    
    MsgSharedGroup* mGroups[0x20];  // _C
    s32 mNumGroups;                 // _8C
};