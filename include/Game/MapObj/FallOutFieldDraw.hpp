#pragma once

#include "Game/NameObj/NameObj.hpp"

class FallOutFieldDraw : public NameObj {
public:
    FallOutFieldDraw(const char*);

    virtual ~FallOutFieldDraw();
    virtual void init(const JMapInfoIter&);
    virtual void draw() const;

    void setUpFillScreen() const;
    void setUpEdgeAndClearAlpha() const;

    u8 _C;
};