#pragma once

#include "Game/LiveActor/LiveActorGroup.hpp"
#include "Game/Util.hpp"

class HitSensor;

class MsgSharedGroup : public LiveActorGroup {
public:
    MsgSharedGroup(const char *, s32, const JMapInfoIter &);
    
    virtual ~MsgSharedGroup();

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

    virtual ~LiveActorGroupArray();

    virtual void init(const JMapInfoIter &);

    LiveActorGroup* getLiveActorGroup(const LiveActor *) const;
    MsgSharedGroup* createGroup(const JMapInfoIter &, const char *, s32);
    LiveActorGroup* findGroup(const LiveActor *) const;
    LiveActorGroup* findGroup(const JMapInfoIter &);
    LiveActorGroup* entry(LiveActor *, const JMapInfoIter &, const char *, s32);
    
    MsgSharedGroup* mGroups[0x20];  // _C
    s32 mNumGroups;                 // _8C
};