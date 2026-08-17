#pragma once

#include "Game/LiveActor/LiveActor.hpp"
#include <JSystem/JGeometry/TMatrix.hpp>

class KabokuriFire : public LiveActor {
public:
    KabokuriFire(const char* pName);

    virtual void init(const JMapInfoIter& rIter);
    virtual MtxPtr getBaseMtx() const { return const_cast<TPos3f*>(&_8C)->toMtxPtr(); }
    virtual void control();
    virtual void attackSensor(HitSensor* pSender, HitSensor* pReceiver);
    virtual bool receiveOtherMsg(u32 msg, HitSensor* pSender, HitSensor* pReceiver);

    void start(const TVec3f&, bool);
    void exeWait();

private:
    /* 0x8C */ TPos3f _8C;
};
