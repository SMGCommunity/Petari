#include "Game/Util/SwitchEventFunctorListener.hpp"

SwitchEventFunctorListener::SwitchEventFunctorListener() {
    mOnFunctor = nullptr;
    mOffFunctor = nullptr;
}

void SwitchEventFunctorListener::setOnFunctor(const MR::FunctorBase &rFunctor) {
    mOnFunctor = rFunctor.clone(0);
}

void SwitchEventFunctorListener::setOffFunctor(const MR::FunctorBase &rFunctor) {
    mOffFunctor = rFunctor.clone(0);
}

void SwitchEventFunctorListener::listenSwitchOnEvent() {
    if (mOnFunctor != nullptr) {
        (*mOnFunctor)();
    }
}

void SwitchEventFunctorListener::listenSwitchOffEvent() {
    if (mOffFunctor != nullptr) {
        (*mOffFunctor)();
    }
}
