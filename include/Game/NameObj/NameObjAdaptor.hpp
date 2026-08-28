#pragma once

#include "Game/NameObj/NameObj.hpp"

namespace MR {
    class FunctorBase;
};  // namespace MR

/// @brief Implementation of a NameObj that stores function pointers to movement, calcAnim, calcView, and draw functions.
class NameObjAdaptor : public NameObj {
public:
    NameObjAdaptor(const char*);

    /* 0x08 */ virtual ~NameObjAdaptor();
    /* 0x14 */ virtual void movement();
    /* 0x18 */ virtual void draw() const;
    /* 0x1C */ virtual void calcAnim();
    /* 0x20 */ virtual void calcViewAndEntry();

    void connectToMovement(const MR::FunctorBase&);
    void connectToCalcAnim(const MR::FunctorBase&);
    void connectToDraw(const MR::FunctorBase&);

    MR::FunctorBase* mMovementFunc;  // 0xC
    MR::FunctorBase* mCalcAnimFunc;  // 0x10
    MR::FunctorBase* mCalcViewFunc;  // 0x14
    MR::FunctorBase* mDrawAnimFunc;  // 0x18
};
