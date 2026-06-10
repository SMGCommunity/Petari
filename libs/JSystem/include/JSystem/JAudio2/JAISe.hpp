#pragma once

#include "JSystem/JAudio2/JAISeqDataMgr.hpp"
#include "JSystem/JAudio2/JASTrack.hpp"

class JAISeMgr;

class JAISe : public JSULink< JAISe >, public JAISound, public JASPoolAllocObject< JAISe > {
public:
    class TInner {
    public:
        TInner() : mSeqData(nullptr, 0) {
        }

        /* 0x000 */ u32 mProperPriority;
        /* 0x004 */ u32 mPriority;
        /* 0x008 */ JASSoundParams mSoundParams;
        /* 0x01c */ JASTrack track;
        /* 0x264 */ JAISeqData mSeqData;
        /* 0x26c */ bool _26C;
        /* 0x270 */ JAISeMgr* seMgr;
        /* 0x274 */ JAISoundStrategyMgr< JAISe >* mSoundStrategyMgr;
        /* 0x278 */ JAISoundStrategy< JAISe >* mSoundStrategy;
        /* 0x27c */ JAITempoMgr mTempoMgr;
    };

    virtual s32 getNumChild() const;
    virtual JAISoundChild* getChild(int);
    virtual void releaseChild(int);
    virtual JASTrack* getTrack();
    virtual JASTrack* getChildTrack(int);
    virtual JAISe* asSe();
    virtual JAITempoMgr* getTempoMgr();
    virtual bool JAISound_tryDie_();

    JAISe(JAISeMgr* seMgr, JAISoundStrategyMgr< JAISe >* soundStrategyMgr, u32 priority);
    void mixOut_(const JASSoundParams& params);
    void stopTrack_();
    void startTrack_(const JASSoundParams& params);
    void JAISeCategoryMgr_mixOut_(bool, const JASSoundParams& params, JAISoundActivity activity);
    void JAISeCategoryMgr_calc_();
    void JAISeMgr_startID_(JAISoundID id, const TVec3f* posPtr, JAIAudience* audience);
    bool prepare_getSeqData_();
    void prepare_();

    const JAISeqData* getSeqData() const {
        return &inner_.mSeqData;
    }
    u32 JAISeCategoryMgr_getProperPriority_() const {
        return inner_.mProperPriority;
    }
    u32 JAISeCategoryMgr_getPriority_() const {
        return inner_.mPriority;
    }
    bool isFarAway() const {
        return inner_.mPriority == -1;
    }

    /* 0x0ac */ TInner inner_;
};
