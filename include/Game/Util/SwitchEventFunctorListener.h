#pragma once

#include "Game/Util/Functor.h"
#include <revolution.h>

class SwitchEventListener {
public:
    virtual void refresh(bool listenOn) {
        if (listenOn) {
            listenSwitchOnEvent();
        }
        else {
            listenSwitchOffEvent();
        }
    }

    virtual void listenSwitchOnEvent() = 0;
    virtual void listenSwitchOffEvent() = 0;
};

class SwitchEventFunctorListener : public SwitchEventListener {
public:
    SwitchEventFunctorListener();

    virtual void listenSwitchOnEvent();
    virtual void listenSwitchOffEvent();

    void setOnFunctor(const MR::FunctorBase &);
    void setOffFunctor(const MR::FunctorBase &);

    MR::FunctorBase* mOnFunctor;    // _4
    MR::FunctorBase* mOffFunctor;   // _8
};