#pragma once

#include "types.h"
#include "actor/nameobj/NameObj.h"
#include "MR/functor/Functor.h"

class NameObjAdaptor : public NameObj
{
public:
    NameObjAdaptor(const char *);

    virtual ~NameObjAdaptor();

    virtual void movement();
    virtual void draw() const;
    virtual void clacAnim();
    virtual void calcViewAndEntry();

    void connectToMovement(MR::FunctorBase const &);
    void connectToCalcAnim(MR::FunctorBase const &);
    void connectToDraw(MR::FunctorBase const &);

    // TODO: properly label functors
    MR::FunctorBase* mMovementFunctor; // _C
    MR::FunctorBase* mCalcAnimFunctor; // _10
    MR::FunctorBase* _14;
    MR::FunctorBase* mDrawFunctor; // _18
};