#include "actor/nameobj/NameObjAdaptor.h"

NameObjAdaptor::NameObjAdaptor(const char *name) : NameObj(name)
{
    MR::FunctorBase* temp = 0;

    this->mMovementFunctor = temp;
    this->mCalcAnimFunctor = temp;
    this->_14 = temp;
    this->mDrawFunctor = temp;
}

void NameObjAdaptor::connectToMovement(MR::FunctorBase const &functor)
{
    this->mMovementFunctor = functor.clone(0);
}

void NameObjAdaptor::connectToCalcAnim(MR::FunctorBase const &functor)
{
    this->mCalcAnimFunctor = functor.clone(0);
}

void NameObjAdaptor::connectToDraw(MR::FunctorBase const &functor)
{
    this->mDrawFunctor = functor.clone(0);
}