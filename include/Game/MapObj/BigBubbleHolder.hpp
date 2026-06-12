#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class BigBubble;
class BigBubbleDrawer;
class BigBubbleMoveLimitter;

class BigBubbleHolder : public NameObj {
public:
    BigBubbleHolder(const char*);

    virtual void init(const JMapInfoIter& rIter);
    virtual void movement();
    virtual void draw() const;

    void addMoveLimitter(BigBubbleMoveLimitter*);
    BigBubble* generateBigBubble(LiveActor*, const TVec3f&, const TVec3f&, f32, bool, s32, s32, s32);
    BigBubbleMoveLimitter* getMoveLimitter(s32) const;
    void sortDrawOrder();

    /* 0x0C */ BigBubbleDrawer* mDrawer;
    /* 0x10 */ BigBubble* mBubbles[16];
    /* 0x50 */ BigBubble* mDrawBubbles[16];
    /* 0x90 */ BigBubbleMoveLimitter* mLimitters[64];
    /* 0x190 */ s32 mNumLimitters;
    /* 0x194 */ s32 mNumBubbles;
    /* 0x198 */ bool mIsMarioBinded;
};

namespace MR {
    void setUpBigBubbleDraw();
    void createBigBubbleHolder();
    BigBubble* generateBigBubble(LiveActor*, const TVec3f&, const TVec3f&, f32, bool, s32, s32, s32);
    void limitBigBubblePosition(TVec3f*, f32, s32);
    void addBigBubbleMoveLimitter(BigBubbleMoveLimitter*);
    bool isPlayerBindedBigBubble();
};  // namespace MR
