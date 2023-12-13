#include "Game/Util/SwitchEventFunctorListener.hpp"

SwitchEventFunctorListener::SwitchEventFunctorListener() {
    mOnFunctor = 0;
    mOffFunctor = 0;
}

void SwitchEventFunctorListener::setOnFunctor(const MR::FunctorBase &rFunctor) {
    mOnFunctor = rFunctor.clone(0);
}

void SwitchEventFunctorListener::setOffFunctor(const MR::FunctorBase &rFunctor) {
    mOffFunctor = rFunctor.clone(0);
}

void SwitchEventFunctorListener::listenSwitchOnEvent() {
    if (mOnFunctor) {
        (*mOnFunctor)();
    }
}

void SwitchEventFunctorListener::listenSwitchOffEvent() {
    if (mOffFunctor) {
        (*mOffFunctor)();
    }
}