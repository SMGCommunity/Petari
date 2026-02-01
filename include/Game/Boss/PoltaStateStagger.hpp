#pragma once

#include "Game/LiveActor/ActorStateBase.hpp"

class Polta;
class HitSensor;

class PoltaStateStagger : public ActorStateBase<Polta> {
public:
    PoltaStateStagger(Polta*);

    virtual ~PoltaStateStagger();
    virtual void appear();

    void exeWait();

    bool isEnableSensor() const;
    void setActionName(const char*);

    inline Polta* getPolta() { return mPoltaPtr; }

    /* 0xC */ Polta* mPoltaPtr;
    u8 padding[0xC];
    /* 0x1C */ const char* mActionName;
    bool _20;  
};
