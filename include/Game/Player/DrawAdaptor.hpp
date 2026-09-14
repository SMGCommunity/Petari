#pragma once

#include "Game/NameObj/NameObj.hpp"
#include "Game/Util/Functor.hpp"

class DrawAdaptor : public NameObj {
public:
    DrawAdaptor(const MR::FunctorBase& rFunc, int drawType);

    virtual void draw() const;

private:
    /* 0x0C */ const MR::FunctorBase* mFunc;
};
