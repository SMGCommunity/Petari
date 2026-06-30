#pragma once

#include "Game/NameObj/NameObj.hpp"

class FallOutFieldDraw : public NameObj {
public:
    /// @brief Creates a new `FallOutFieldDraw`.
    /// @param pName A pointer to the null-terminated name of the object.
    FallOutFieldDraw(const char* pName);

    virtual void init(const JMapInfoIter&);
    virtual void draw() const;

    void setUpFillScreen() const;
    void setUpEdgeAndClearAlpha() const;
    void activate();
    void deactivate();

    /* 0x0C */ bool _C;
};

namespace MR {
    NameObj* createFallOutFieldDraw(const char*);
};  // namespace MR
