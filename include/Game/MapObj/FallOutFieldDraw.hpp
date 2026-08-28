#pragma once

#include "Game/NameObj/NameObj.hpp"

class FallOutFieldDraw : public NameObj {
public:
    /// @brief Creates a new `FallOutFieldDraw`.
    /// @param pName A pointer to the null-terminated name of the object.
    FallOutFieldDraw(const char* pName);

    /* 0x0C */ virtual void init(const JMapInfoIter&);
    /* 0x18 */ virtual void draw() const;

    void setUpFillScreen() const;
    void setUpEdgeAndClearAlpha() const;
    void activate();
    void deactivate();

    /* 0x0C */ bool _C;
};

namespace MR {
    NameObj* createFallOutFieldDraw(const char*);
};  // namespace MR
