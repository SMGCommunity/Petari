#pragma once

#include "Game/LiveActor/LiveActor.hpp"
#include <JSystem/JGeometry/TVec.hpp>

class SpiderThreadHangInfo;
class SpiderThreadMainPoint;
class SpiderThreadPart;
class SpiderThreadPoint;
class SpiderThreadRadialLine;
class SpiderThreadWindCtrl;

struct CutPointEntry {
    s32 mRadialIdx;
    s32 mPointIdx;
};

class SpiderThread : public LiveActor {
public:
    SpiderThread(const char*);

    virtual void init(const JMapInfoIter& rIter);
    virtual void movement();
    virtual void draw() const;
    virtual bool receiveOtherMsg(u32 msg, HitSensor* pSender, HitSensor* pReceiver);

    void initThread(const TVec3f&);
    void startActorBind(LiveActor*, const TVec3f**, const TVec3f**, const TVec3f**, const TVec3f&, const TVec3f&, s32);
    bool touchActor(const TVec3f&, const TVec3f&);
    void tryPush(const TVec3f&, f32);
    void stopAllPartsPoint();
    void cutSpiderThread();
    void initMainThreads();
    void connectMainPoint(SpiderThreadPoint*, SpiderThreadPoint*, bool);
    void updatePointerInfo();
    void updateHangedPoint();
    void tryTouchPoint(s32);
    void findNearestPointPos(const TVec3f**, const TVec3f**, s32*, s32*, const TVec3f&) const;
    SpiderThreadHangInfo* findHangInfo(LiveActor*) const;
    void initDraw() const;

    /* 0x8C */ SpiderThreadMainPoint* mMainPoint;
    /* 0x90 */ SpiderThreadHangInfo** mHangInfos;
    /* 0x94 */ s32 mNumHangInfos;
    /* 0x98 */ SpiderThreadHangInfo* mShootHangInfo;
    /* 0x9C */ s32 mNumRadialLines;
    /* 0xA0 */ SpiderThreadRadialLine** mRadialLines;
    /* 0xA4 */ SpiderThreadMainPoint** mAnchorPoints;
    /* 0xA8 */ s32 mMainPartsBufferSize;
    /* 0xAC */ s32 mNumMainParts;
    /* 0xB0 */ SpiderThreadPart** mMainParts;
    /* 0xB4 */ s32 mSubPartsBufferSize;
    /* 0xB8 */ s32 mNumSubParts;
    /* 0xBC */ SpiderThreadPart** mSubParts;
    /* 0xC0 */ s32 mCutPointsBufferSize;
    /* 0xC4 */ s32 mNumCutPoints;
    /* 0xC8 */ SpiderThreadMainPoint** mCutPoints;
    /* 0xCC */ TVec3f mFront;
    /* 0xD8 */ SpiderThreadWindCtrl* mWindCtrl;
    /* 0xDC */ bool mIsBloomOn;
    /* 0xE0 */ JUTTexture* mThreadTexture;
    /* 0xE4 */ JUTTexture* mIndirectTexture;
};

namespace MR {
    void initSpiderThread(const TVec3f&);
    void appearSpiderThread();
    void startSpiderThreadBattle();
    void startSpiderThreadChance();
    void startSpiderThreadBattleEnd();
    void pauseOffSpiderThread();
    f32 getSpiderThreadPosZ();
    void startActorBindToSpiderThread(LiveActor*, const TVec3f**, const TVec3f**, const TVec3f**, const TVec3f&, const TVec3f&);
    bool sendMsgToSpiderThread(u32, HitSensor*);
    bool touchActorToSpiderThread(const TVec3f&, const TVec3f&);
    void tryPushSpiderThread(const TVec3f&, f32);
    void onSpiderThreadBloom();
    void drawSpiderThreadBloom();
};  // namespace MR
