#pragma once

#include "Game/MapObj/MapObjActor.hpp"

class AstroCore : public MapObjActor {
public:
    AstroCore(const char*);

    virtual ~AstroCore();
    virtual void init(const JMapInfoIter&);
    virtual void attackSensor(HitSensor*, HitSensor*);
    virtual bool receiveOtherMsg(u32, HitSensor*, HitSensor*);

    void exeGrow();

    inline void exeWait() {
        if (MR::isFirstStep(this)) {
            setStateBeforeGrow();
        }
    }

    void startDemo();
    void startAnimGrow();
    void setStateBeforeGrow();
};
