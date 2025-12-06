#pragma once

#include "Game/NameObj/NameObj.hpp"
#include "JSystem/JGeometry/THex.hpp"

class ClippingJudge : public NameObj {
public:
    ClippingJudge(const char*);

    virtual ~ClippingJudge();
    virtual void init(const JMapInfoIter&);

    virtual void movement();

    bool isJudgedToClipFrustum(const TVec3f&, f32) const;
    bool isJudgedToClipFrustum(const TVec3f&, f32, s32) const;
    void calcViewingVolume(THex3f*, f32);

    THex3f mFrustum;  // 0xC
    THex3f mClipFrustums[0x8];
    f32 mClipDistances[0x8];
};

namespace MR {
    ClippingJudge* getClippingJudge();
};