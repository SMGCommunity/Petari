#pragma once

#include "Game/NameObj/NameObj.hpp"

namespace MR {
    class FunctorBase;
};  // namespace MR

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

    /* 0x0C */ MR::FunctorBase* mMovementFunc;
    /* 0x10 */ MR::FunctorBase* mCalcAnimFunc;
    /* 0x14 */ MR::FunctorBase* mCalcViewFunc;
    /* 0x18 */ MR::FunctorBase* mDrawAnimFunc;
};
