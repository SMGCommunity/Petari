#pragma once

#include "Game/Map/ActorAppearSwitchListener.hpp"

namespace MR {
    class FunctorBase;
};  // namespace MR

class SwitchEventFunctorListener : public SwitchEventListener {
public:
    SwitchEventFunctorListener();

    /* 0x0C */ virtual void listenSwitchOnEvent();
    /* 0x10 */ virtual void listenSwitchOffEvent();

    void setOnFunctor(const MR::FunctorBase&);
    void setOffFunctor(const MR::FunctorBase&);

    MR::FunctorBase* mOnFunctor;   // 0x4
    MR::FunctorBase* mOffFunctor;  // 0x8
};
