#pragma once

#include "JSystem/JAudio2/JAISeqDataMgr.hpp"
#include "JSystem/JAudio2/JASTrack.hpp"
#include "JSystem/JSupport/JSUList.hpp"

class JAISeqMgr;
struct JAISoundChild;

class JAISeq : public JAISound, public JSULink< JAISeq >, public JASPoolAllocObject< JAISeq > {
public:
    static const int MAX_OUTPUT_TRACK_CHILDREN = 2;
    static const int NUM_CHILDREN = MAX_OUTPUT_TRACK_CHILDREN * JASTrack::MAX_CHILDREN;

    class TInner {
    public:
        TInner() : mSeqData(nullptr, 0) {
        }

        /* 0x000 */ JASTrack outputTrack;
        /* 0x248 */ JAISoundChild* mSoundChild[NUM_CHILDREN];
        /* 0x2C8 */ JAITempoMgr mTempoMgr;
        /* 0x2D8 */ JASSoundParams mSoundParams;
        /* 0x2EC */ JAISeqData mSeqData;
        /* 0x2F4 */ s32 mCategory;
        /* 0x2F8 */ JAISeqMgr* seqMgr;
        /* 0x2FC */ JAISoundStrategyMgr< JAISeq >* strategyMgr;
    };

    virtual s32 getNumChild() const;
    virtual JAISoundChild* getChild(int index);
    virtual void releaseChild(int index);
    virtual JAISeq* asSeq();
    virtual JASTrack* getTrack();
    virtual JASTrack* getChildTrack(int);
    virtual JAITempoMgr* getTempoMgr();
    virtual bool JAISound_tryDie_();

    JAISeq(JAISeqMgr* seqMgr, JAISoundStrategyMgr< JAISeq >* soundStrategyMgr);
    void JAISeqMgr_startID_(JAISoundID id, const TVec3f* posPtr, JAIAudience* audience, int category, int);
    void playSeqData_(const JASSoundParams& params, JAISoundActivity activity);
    void reserveChildTracks_(int);
    void releaseChildTracks_();
    bool prepare_getSeqData_();
    bool prepare_(const JASSoundParams& params, JAISoundActivity activity);
    void JAISeqMgr_calc_();
    void die_();
    void mixOut_(const JASSoundParams& params, JAISoundActivity activity);
    void JAISeqMgr_mixOut_(const JASSoundParams& params, JAISoundActivity activity);

    const JAISeqData& getSeqData() const {
        return inner_.mSeqData;
    }
    s32 getCategory() const {
        return inner_.mCategory;
    }

    /* 0x0A8 */ TInner inner_;
    /* 0x3A8 */ JAISoundStrategy< JAISeq >* soundStrategy;
};
