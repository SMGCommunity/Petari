#pragma once

#include "Game/LiveActor/LiveActor.hpp"
#include <JSystem/JGeometry/TMatrix.hpp>

class KabokuriFire : public LiveActor {
public:
    KabokuriFire(const char* pName);

    /* 0x0C */ virtual void init(const JMapInfoIter& rIter);
    /* 0x38 */ virtual MtxPtr getBaseMtx() const { return const_cast<TPos3f*>(&_8C)->toMtxPtr(); }
    /* 0x48 */ virtual void control();
    /* 0x54 */ virtual void attackSensor(HitSensor* pSender, HitSensor* pReceiver);
    /* 0x74 */ virtual bool receiveOtherMsg(u32 msg, HitSensor* pSender, HitSensor* pReceiver);

    void start(const TVec3f&, bool);
    void exeWait();

private:
    /* 0x8C */ TPos3f _8C;
};
