#pragma once

#include "Game/NameObj/NameObj.hpp"
#include "Game/Util.hpp"

/// @brief Implementation of a NameObj that stores function pointers to movement, calcAnim, calcView, and draw functions.
class NameObjAdaptor : public NameObj {
public:
    NameObjAdaptor(const char*);

    virtual ~NameObjAdaptor();
    virtual void movement();
    virtual void draw() const;
    virtual void calcAnim();
    virtual void calcViewAndEntry();

    void connectToMovement(const MR::FunctorBase&);
    void connectToCalcAnim(const MR::FunctorBase&);
    void connectToDraw(const MR::FunctorBase&);

    MR::FunctorBase* mMovementFunc;  // 0xC
    MR::FunctorBase* mCalcAnimFunc;  // 0x10
    MR::FunctorBase* mCalcViewFunc;  // 0x14
    MR::FunctorBase* mDrawAnimFunc;  // 0x18
};
