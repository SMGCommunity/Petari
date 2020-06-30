#pragma once

#include "JMap/JMapInfoIter.h"
#include "Actor/LiveActor/LiveActorGroup.h"

class MsgSharedGroup : public LiveActorGroup
{
public:
    MsgSharedGroup(const char *, s64, const JMapInfoIter &);

    virtual ~MsgSharedGroup();

    virtual void init(const JMapInfoIter &);
    virtual void movement();

    const JMapInfoIter* mInfoIter; // _18
    s32 _1C;
    u32 _20; // HitSensor*
    const char* mSensorName; // _24
};