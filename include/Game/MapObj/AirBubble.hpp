#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class AirBubble : public LiveActor {
public:
    AirBubble(const char*);

    virtual ~AirBubble();
    virtual void init(const JMapInfoIter&);
    virtual void initAfterPlacement();
    virtual void appear();
    virtual void kill();
    virtual void control();
    virtual bool receiveMsgPush(HitSensor*, HitSensor*);
    virtual bool receiveOtherMsg(u32, HitSensor*, HitSensor*);

    void appearMove(const TVec3f&, s32);
    void exeWait();
    void exeMove();
    void exeBreak();
    void exeKillWait();
    bool canSpinGet() const;

    TVec3f _8C;
    TVec3f _98;
    TVec3f _A4;
    f32 _B0;
    u32 _B4;
    u32 _B8;
    u32 _BC;
    f32 _C0;
    s32 _C4;
    u8 _C8;
    u8 _C9;
};
