#pragma once

#include "Game/NameObj/NameObj.hpp"
#include <JSystem/JGeometry/THex.hpp>

class ClippingJudge : public NameObj {
public:
    ClippingJudge(const char*);

    virtual ~ClippingJudge();
    virtual void init(const JMapInfoIter&);

    virtual void movement();

    bool isJudgedToClipFrustum(const TVec3f&, f32) const;
    bool isJudgedToClipFrustum(const TVec3f&, f32, s32) const;
    void calcViewingVolume(THex3f*, f32);

    /* 0x0C */ THex3f mFrustum;
    /* 0x06C */ THex3f mClipFrustums[0x8];
    /* 0x36C */ f32 mClipDistances[0x8];
};

namespace MR {
    ClippingJudge* getClippingJudge();
};  // namespace MR
