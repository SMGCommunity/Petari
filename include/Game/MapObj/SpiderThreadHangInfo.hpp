#pragma once

#include <JSystem/JGeometry/TVec.hpp>

class LiveActor;
class SpiderThreadPart;

class SpiderThreadHangInfo {
public:
    SpiderThreadHangInfo();

    void startBind(LiveActor*, const TVec3f*, SpiderThreadPart*, s32, const TVec3f&, s32);
    void endBind();
    void startHang();
    void endHang(bool);
    void updateHang();
    void copyInfo(const SpiderThreadHangInfo*);

    /* 0x00 */ LiveActor* mBindedActor;
    /* 0x04 */ const TVec3f* mPosition;
    /* 0x08 */ SpiderThreadPart* mThreadPart;
    /* 0x0C */ s32 mHangIndex;
    /* 0x10 */ bool mIsHanging;
    /* 0x14 */ s32 _14;
};
