#pragma once

#include "Game/NameObj/NameObj.hpp"
#include "Game/Util/Color.hpp"

class ClipFieldFillDraw : public NameObj {
public:
    /// @brief Creates a new `ClipFieldFillDraw`.
    /// @param pName A pointer to the null-terminated name of the object.
    ClipFieldFillDraw(const char* pName);

    virtual void init(const JMapInfoIter& rIter);
    virtual void draw() const;

    void setModeSubColor();
    void setUpFillScreen() const;
    void sendFillScreen(GXColor) const;

    /* 0x0C */ GXBlendMode mFillBlendMode;
    /* 0x10 */ GXBlendFactor mFillSrcFactor;
    /* 0x14 */ GXBlendFactor mFillDstFactor;
    /* 0x18 */ GXLogicOp mFillLogicOp;
    /* 0x1C */ Color8 _1C;
    /* 0x20 */ Color8 _20;
    /* 0x24 */ bool _24;
    /* 0x25 */ bool _25;
};

namespace MR {
    NameObj* createClipFieldFillDraw(const char*);
    NameObj* createVolumeDrawSpotLight(const char*);
};  // namespace MR
