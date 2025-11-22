#include "Game/NameObj/NameObjAdaptor.hpp"

NameObjAdaptor::NameObjAdaptor(const char* pName)
    : NameObj(pName),
      mMovementFunc(nullptr),
      mCalcAnimFunc(nullptr),
      mCalcViewFunc(nullptr),
      mDrawAnimFunc(nullptr) {
}

NameObjAdaptor::~NameObjAdaptor() {
    delete mMovementFunc;
    delete mCalcAnimFunc;
    delete mCalcViewFunc;
    delete mDrawAnimFunc;
}

void NameObjAdaptor::movement() {
    if (mMovementFunc != nullptr) {
        (*mMovementFunc)();
    }
}

void NameObjAdaptor::calcAnim() {
    if (mCalcAnimFunc != nullptr) {
        (*mCalcAnimFunc)();
    }
}

void NameObjAdaptor::calcViewAndEntry() {
    if (mCalcViewFunc != nullptr) {
        (*mCalcViewFunc)();
    }
}

void NameObjAdaptor::draw() const {
    if (mDrawAnimFunc != nullptr) {
        (*mDrawAnimFunc)();
    }
}

void NameObjAdaptor::connectToMovement(const MR::FunctorBase& rFunctor) {
    mMovementFunc = rFunctor.clone(nullptr);
}

void NameObjAdaptor::connectToCalcAnim(const MR::FunctorBase& rFunctor) {
    mCalcAnimFunc = rFunctor.clone(nullptr);
}

void NameObjAdaptor::connectToDraw(const MR::FunctorBase& rFunctor) {
    mDrawAnimFunc = rFunctor.clone(nullptr);
}
