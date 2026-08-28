#pragma once

#include "Game/NameObj/NameObj.hpp"

class LiveActor;
class Nerve;
class Spine;

class MapPartsFunction : public NameObj {
public:
    MapPartsFunction(LiveActor*, const char*);

    /* 0x0C */ virtual void init(const JMapInfoIter&) {
    }

    /* 0x14 */ virtual void movement();

    /* 0x24 */ virtual bool isWorking() const {
        return true;
    }

    /* 0x28 */ virtual void start() {
    }

    /* 0x2C */ virtual void end() {
    }

    /* 0x30 */ virtual bool receiveMsg(u32) {
        return false;
    }

    /* 0x34 */ virtual void control() {
    }

    bool sendMsgToHost(u32);
    void initNerve(const Nerve*);
    void setNerve(const Nerve*);
    s32 getStep() const;
    bool isStep(s32) const;
    bool isNerve(const Nerve*) const;
    bool isFirstStep() const;

    /* 0x0C */ Spine* mSpine;
    /* 0x10 */ LiveActor* mHost;
    /* 0x14 */ bool mIsActive;
};
