#pragma once

#include "Game/NameObj/NameObj.hpp"

class LiveActor;
class Spine;
class Nerve;

class MapPartsFunction : public NameObj {
public:
    MapPartsFunction(LiveActor*, const char*);

    inline virtual ~MapPartsFunction() {}

    virtual void init(const JMapInfoIter&) {}

    virtual void movement();

    virtual bool isWorking() const { return true; }

    virtual void start() {}

    virtual void end() {}

    virtual bool receiveMsg(u32) { return 0; }

    virtual void control() {}

    void sendMsgToHost(u32);
    void initNerve(const Nerve*);
    void setNerve(const Nerve*);
    s32 getStep() const;
    bool isStep(s32) const;
    bool isNerve(const Nerve*) const;
    bool isFirstStep() const;

    Spine* mSpine;     // 0xC
    LiveActor* mHost;  // 0x10
    u8 _14;
};