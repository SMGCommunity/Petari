#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class MeteoContainer : public LiveActor {
public:
    MeteoContainer(const char*);

    /* 0x08 */ virtual ~MeteoContainer();
    /* 0x0C */ virtual void init(const JMapInfoIter&);
    /* 0x24 */ virtual void appear();
    /* 0x2C */ virtual void kill();
    /* 0x48 */ virtual void control();
    /* 0x54 */ virtual void attackSensor(HitSensor*, HitSensor*);
    /* 0x58 */ virtual bool receiveMsgPush(HitSensor*, HitSensor*);
    /* 0x74 */ virtual bool receiveOtherMsg(u32, HitSensor*, HitSensor*);

    void exeWait();
    void exeDestroy();
};
