#pragma once

#include <JSystem/JGeometry/TBox.hpp>
#include <JSystem/JGeometry/TVec.hpp>

class SpiderThreadMainPoint;
class SpiderThreadPoint;

class SpiderThreadPart {
public:
    SpiderThreadPart(SpiderThreadMainPoint*, SpiderThreadMainPoint*, f32);

    void cut(SpiderThreadMainPoint*, SpiderThreadMainPoint*);
    void updatePointerPos();
    void update();
    void draw() const;
    void startHang(s32);
    void updateHang(const TVec3f&, s32);
    void endHang(s32);
    void stopAllPoints();
    f32 calcNearestPointInfo(const TVec3f**, const TVec3f**, s32*, const TVec3f&);
    void updatePointPosToLine();
    void updatePointPosAndBasePosToLine();
    bool touchActor(s32, const TVec3f&);
    void tryTouch(f32, const TVec3f&, s32);
    s32 findTouchPoint(f32, const TVec3f&, s32);
    f32 findNearestPointNo(s32*, const TVec3f&) const;
    void updatePointPosHanging(s32);
    void updatePointVelocityTouch(s32, const TVec3f&);
    void updateBasePos();
    void updateForceAttach();
    void updateCutPoints();
    void updateDrawVec();

    void drawLine() const;
    void drawPoints() const;

    inline void padBox(f32 padAmount) {
        TVec3f padding(padAmount);
        mBoundBox.i.sub(padding);
        mBoundBox.f.add(padding);
    }

    /* 0x00 */ bool mIsStill;
    /* 0x01 */ bool mIsCut;
    /* 0x02 */ bool mIsFree;
    /* 0x04 */ s16 _4;
    /* 0x08 */ s32 mNumHangingPoints;
    /* 0x0C */ s32 mStopTime;
    /* 0x10 */ s32 mTouchTimeout;
    /* 0x14 */ TVec3f mPointerPosPad0;
    /* 0x20 */ TVec3f mPointerPosPad1;
    /* 0x2C */ SpiderThreadMainPoint* mPointA;
    /* 0x30 */ SpiderThreadMainPoint* mPointB;
    /* 0x34 */ s32 mNumPoints;
    /* 0x38 */ SpiderThreadPoint** mPoints;
    /* 0x3C */ TBox3f mBoundBox;
    /* 0x54 */ TVec3f mSide;
    /* 0x60 */ TVec3f mUp;
    /* 0x6C */ TVec3f mFront;
    /* 0x78 */ f32 mDistBetweenMainPoints;
    /* 0x7C */ f32 mDistBetweenPoints;
    /* 0x80 */ f32 mTextureOffset;
};
