#pragma once

#include "Game/NameObj/NameObj.hpp"

class LiveActor;
class Nerve;
class Spine;

class MapPartsFunction : public NameObj {
public:
    MapPartsFunction(LiveActor* pHost, const char* pName);

    virtual void init(const JMapInfoIter& rIter) {
    }

    virtual void movement();

    virtual bool isWorking() const;

    virtual void start();

    virtual void end() {
    }

    virtual bool receiveMsg(u32 msg) NO_INLINE {
        return false;
    }

    virtual void control() {
    }

    bool sendMsgToHost(u32 msg);
    void initNerve(const Nerve* pNerve);
    void setNerve(const Nerve* pNerve);
    s32 getStep() const;
    bool isStep(s32 step) const;
    bool isNerve(const Nerve* pNerve) const;
    bool isFirstStep() const;

    /* 0x0C */ Spine* mSpine;
    /* 0x10 */ LiveActor* mHost;
    /* 0x14 */ bool mIsActive;
};
