#pragma once

#include "Game/NameObj/NameObj.h"
#include "Game/Util.h"

/// @brief Implementation of a NameObj that stores function pointers to movement, calcAnim, calcView, and draw functions.
class NameObjAdaptor : public NameObj {
public:
    NameObjAdaptor(const char *);

    virtual ~NameObjAdaptor();
    virtual void movement();
    virtual void draw() const;
    virtual void calcAnim();
    virtual void calcViewAndEntry();

    void connectToMovement(const MR::FunctorBase &);
    void connectToCalcAnim(const MR::FunctorBase &);
    void connectToDraw(const MR::FunctorBase &);

    MR::FunctorBase* mMovementFunc;     // _C
    MR::FunctorBase* mCalcAnimFunc;     // _10
    MR::FunctorBase* mCalcViewFunc;     // _14
    MR::FunctorBase* mDrawAnimFunc;     // _18
};