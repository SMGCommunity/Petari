#pragma once

#include "Game/Screen/LayoutActor.hpp"

class StarPointerCommandStream : public LayoutActor {
public:
    StarPointerCommandStream(const TVec2f*);

    virtual void init(const JMapInfoIter& rIter);
    virtual void control();

    void start(const TVec3f*, bool);
    void show();
    void hide();
    void clear();
    void calcPose(const TVec2f&, const TVec2f&);

    void exeWait();
    void exeSignal();
    f32 calcLineWidth(f32) const;

    inline void setPortAndColor(s32 padChannel, const GXColor* color) {
        mPadChannel = padChannel;
        mColor = color;
    }

    /* 0x20 */ bool _20;
    /* 0x24 */ s32 mOffScreenTime;
    /* 0x28 */ f32 _28;
    /* 0x2C */ const TVec2f* mScreenPos;
    /* 0x30 */ const TVec3f* mWorldPos;
    /* 0x34 */ TVec2f mFollowPos;
    /* 0x3C */ s32 mPadChannel;
    /* 0x40 */ const GXColor* mColor;
};
