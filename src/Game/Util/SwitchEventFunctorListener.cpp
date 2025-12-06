#include "Game/Util/SwitchEventFunctorListener.hpp"

SwitchEventFunctorListener::SwitchEventFunctorListener() {
    mOnFunctor = nullptr;
    mOffFunctor = nullptr;
}

void SwitchEventFunctorListener::setOnFunctor(const MR::FunctorBase& rFunctor) {
    mOnFunctor = rFunctor.clone(nullptr);
}

void SwitchEventFunctorListener::setOffFunctor(const MR::FunctorBase& rFunctor) {
    mOffFunctor = rFunctor.clone(nullptr);
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
