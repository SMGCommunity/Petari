#include "Actor/NameObj/NameObjAdaptor.h"

NameObjAdaptor::NameObjAdaptor(const char *pName) : NameObj(pName)
{
    MR::FunctorBase* pTemp = 0;

    mMovementFunctor = pTemp;
    mCalcAnimFunctor = pTemp;
    _14 = pTemp;
    mDrawFunctor = pTemp;
}

void NameObjAdaptor::movement()
{
    if (mMovementFunctor == 0)
        return;

    mMovementFunctor->operator();
}

void NameObjAdaptor::connectToMovement(MR::FunctorBase const &functor)
{
    mMovementFunctor = functor.clone(0);
}

void NameObjAdaptor::connectToCalcAnim(MR::FunctorBase const &functor)
{
    mCalcAnimFunctor = functor.clone(0);
}

void NameObjAdaptor::connectToDraw(MR::FunctorBase const &functor)
{
    mDrawFunctor = functor.clone(0);
}