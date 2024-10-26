#include "Game/NameObj/NameObjAdaptor.hpp"

NameObjAdaptor::NameObjAdaptor(const char *pName) : NameObj(pName) {
    mMovementFunc = 0;
    mCalcAnimFunc = 0;
    mCalcViewFunc = 0;
    mDrawAnimFunc = 0;
}

NameObjAdaptor::~NameObjAdaptor() {
    delete mMovementFunc;
    delete mCalcAnimFunc;
    delete mCalcViewFunc;
    delete mDrawAnimFunc;
}

void NameObjAdaptor::movement() {
    if (mMovementFunc) {
        (*mMovementFunc)();
    }
}

void NameObjAdaptor::calcAnim() {
    if (mCalcAnimFunc) {
        (*mCalcAnimFunc)();
    }
}

void NameObjAdaptor::calcViewAndEntry() {
    if (mCalcViewFunc) {
        (*mCalcViewFunc)();
    }
}

void NameObjAdaptor::draw() const {
    if (mDrawAnimFunc) {
        (*mDrawAnimFunc)();
    }
}

void NameObjAdaptor::connectToMovement(const MR::FunctorBase &rFunctor) {
    mMovementFunc = rFunctor.clone(0);
}

void NameObjAdaptor::connectToCalcAnim(const MR::FunctorBase &rFunctor) {
    mCalcAnimFunc = rFunctor.clone(0);
}

void NameObjAdaptor::connectToDraw(const MR::FunctorBase &rFunctor) {
    mDrawAnimFunc = rFunctor.clone(0);
}