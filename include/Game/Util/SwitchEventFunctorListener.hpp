#pragma once

#include "Game/Map/ActorAppearSwitchListener.hpp"

namespace MR {
    class FunctorBase;
};  // namespace MR

class SwitchEventFunctorListener : public SwitchEventListener {
public:
    SwitchEventFunctorListener();

    virtual void listenSwitchOnEvent();
    virtual void listenSwitchOffEvent();

    void setOnFunctor(const MR::FunctorBase&);
    void setOffFunctor(const MR::FunctorBase&);

    /* 0x4 */ MR::FunctorBase* mOnFunctor;
    /* 0x8 */ MR::FunctorBase* mOffFunctor;
};
