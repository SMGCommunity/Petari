#pragma once

#include "Game/NameObj/NameObj.hpp"
#include "Game/Util/Functor.hpp"

class DrawAdaptor : public NameObj {
public:
    DrawAdaptor(const MR::FunctorBase& rFunc, int drawType);
    virtual ~DrawAdaptor();

    virtual void draw() const;

private:
    /* 0xC */ const MR::FunctorBase* mFunc;
};
