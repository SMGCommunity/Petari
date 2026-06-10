#pragma once

#include "JSystem/JAudio2/JAISound.hpp"
#include "JSystem/JAudio2/JASAramStream.hpp"
#include "JSystem/JSupport/JSUList.hpp"

class JAIStreamMgr;

class JAIStream : public JAISound, public JSULink< JAIStream >, public JASPoolAllocObject< JAIStream > {
public:
    struct TInner {
        /* 0x0 */ JASAramStream aramStream;
    };

    JAIStream(JAIStreamMgr* streamMgr, JAISoundStrategyMgr< JAIStream >* soundStrategyMgr);
    void JAIStreamMgr_startID_(JAISoundID id, s32 streamFileEntry, const TVec3f* posPtr, JAIAudience* audience, int category);
    bool prepare_prepareStream_();
    void prepare_();
    void prepare_startStream_();
    void JAIStreamMgr_mixOut_(const JASSoundParams& inParams, JAISoundActivity activity);
    void die_JAIStream_();
    bool JAISound_tryDie_();
    void JAIStreamMgr_calc_();
    s32 getNumChild() const;
    JAISoundChild* getChild(int index);
    void releaseChild(int index);
    JASTrack* getTrack();
    JASTrack* getChildTrack(int);
    JAIStream* asStream();
    JAITempoMgr* getTempoMgr();

    void* JAIStreamMgr_getAramAddr_() const {
        return streamAramAddr;
    }

    void stopStream(bool stop) {
        mIsStreamStopped = stop;
    }

    static const int NUM_CHILDREN = 6;

    /* 0x0A8 */ TInner inner_;
    /* 0x290 */ int mPrepareState;
    /* 0x294 */ s32 mStreamFileEntry;
    /* 0x298 */ int mCategory;
    /* 0x29C */ void* streamAramAddr;
    /* 0x2A0 */ JAISoundChild* children[NUM_CHILDREN];
    /* 0x2B8 */ JAIStreamMgr* streamMgr;
    /* 0x2BC */ JAISoundStrategy< JAIStream >* soundStrategy;
    /* 0x2C0 */ JAISoundStrategyMgr< JAIStream >* soundStrategyMgr;
    /* 0x2C4 */ bool mIsPaused;
    /* 0x2C5 */ bool mIsStreamStopped;
    /* 0x2C6 */ bool mIsStreamStarted;
};
