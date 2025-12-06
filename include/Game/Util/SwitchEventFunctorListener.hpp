#pragma once

#include "Game/Map/ActorAppearSwitchListener.hpp"
#include "Game/Util/Functor.hpp"
#include <revolution.h>

class SwitchEventFunctorListener : public SwitchEventListener {
public:
    SwitchEventFunctorListener();

    virtual void listenSwitchOnEvent();
    virtual void listenSwitchOffEvent();

    void setOnFunctor(const MR::FunctorBase&);
    void setOffFunctor(const MR::FunctorBase&);

    MR::FunctorBase* mOnFunctor;   // 0x4
    MR::FunctorBase* mOffFunctor;  // 0x8
};
