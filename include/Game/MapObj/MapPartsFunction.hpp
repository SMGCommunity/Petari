#pragma once

#include "Game/NameObj/NameObj.hpp"

class LiveActor;
class Nerve;
class Spine;

class MapPartsFunction : public NameObj {
public:
    MapPartsFunction(LiveActor*, const char*);

    virtual void init(const JMapInfoIter&) {
    }

    virtual void movement();

    virtual bool isWorking() const {
        return true;
    }

    virtual void start() {
    }

    virtual void end() {
    }

    virtual bool receiveMsg(u32) {
        return false;
    }

    virtual void control() {
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
