#pragma once

#include "Game/NameObj/NameObj.hpp"
#include "Game/Util/Functor.hpp"

class DrawAdaptor : public NameObj {
public:
    DrawAdaptor(const MR::FunctorBase& rFunc, int drawType);
    /* 0x08 */ virtual ~DrawAdaptor();

    /* 0x18 */ virtual void draw() const;

private:
    /* 0xC */ const MR::FunctorBase* mFunc;
};
