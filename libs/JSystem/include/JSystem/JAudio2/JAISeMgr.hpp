#pragma once

#include "JSystem/JAudio2/JAISe.hpp"
#include "JSystem/JAudio2/JAISound.hpp"
#include "JSystem/JAudio2/JASGlobal.hpp"
#include "JSystem/JAudio2/JASHeapCtrl.hpp"

struct JAIAudience;
struct JASSoundParams;

// TODO: is this actually correct?
enum SeqDataStatus {
    SeqDataStatus_FAIL = 0,
    SeqDataStatus_SUCCESS = 1,
    SeqDataStatus_NONE = 2,
};

template < class T >
struct JASNonCopyable {
    JASNonCopyable() {
    }
    ~JASNonCopyable() {
    }

    /* 0x0 */ T* data;
};

struct JAISeCategoryArrangementItem {
    u8 mMaxActiveSe;
    u8 mMaxInactiveSe;
};

struct JAISeCategoryArrangement {
    JAISeCategoryArrangementItem mItems[16];
};

class JAISeMgr;

class JAISeCategoryMgr : public JAISeqDataUser, public JASNonCopyable< JAIAudience > {
public:
    void JAISeMgr_calc_();
    void JAISeMgr_freeDeadSe_();
    bool JAISeMgr_acceptsNewSe_(u32 priority) const;
    void sortByPriority_();
    void stop(u32 fadeTime);
    void stop();
    void stopSoundID(JAISoundID id);
    void pause(bool);
    void JAISeMgr_mixOut_(const JAISoundParamsMove& params, JAISoundActivity activity);
    JAISeCategoryMgr() {
        mParams.init();
        mMaxActiveSe = 0;
        mMaxInactiveSe = 0;
        data = nullptr;
    }

    virtual bool isUsingSeqData(const JAISeqDataRegion& seqDataRegion);
    virtual int releaseSeqData(const JAISeqDataRegion& seqDataRegion);

    JAISoundParamsMove* getParams() {
        return &mParams;
    }
    int getMaxSe() const {
        return mMaxActiveSe == 0 ? 0 : mMaxActiveSe + mMaxInactiveSe;
    }
    int getMaxInactiveSe() const {
        return mMaxInactiveSe;
    }
    int getMaxActiveSe() const {
        return mMaxActiveSe;
    }
    void setMaxActiveSe(int num) {
        mMaxActiveSe = num;
    }
    void setMaxInactiveSe(int num) {
        mMaxInactiveSe = num;
    }
    const JSUList< JAISe >* getSeList() const {
        return &mSeList;
    }
    int getNumSe() const {
        return mSeList.getNumLinks();
    }
    JAIAudience* getAudience() {
        return data;
    }
    void JAISeMgr_appendSe_(JAISe* se) {
        mSeList.append(se);
    }

    /* 0x08 */ JAISoundParamsMove mParams;
    /* 0x58 */ JSUList< JAISe > mSeList;
    /* 0x64 */ int mMaxInactiveSe;
    /* 0x68 */ int mMaxActiveSe;
};

class JAISeMgr : public JASGlobalInstance< JAISeMgr >, public JAISeqDataUser, public JAISoundActivityMgr {
public:
    JAISeMgr(bool setInstance);
    void setCategoryArrangement(const JAISeCategoryArrangement& arrangement);
    void getCategoryArrangement(JAISeCategoryArrangement*);
    void stop();
    void stop(u32 fadeTime);
    void stopSoundID(JAISoundID id);
    void initParams();
    void setAudience(JAIAudience* audience);
    void setStrategyMgr(JAISoundStrategyMgr< JAISe >* soundStrategyMgr);
    void setSeqDataMgr(JAISeqDataMgr* seqDataMgr);
    void resetSeqDataMgr();
    JAISe* newSe_(int category, u32 priority);
    void calc();
    void mixOut();
    bool startSound(JAISoundID id, JAISoundHandle* handle, const TVec3f* posPtr);
    int getNumActiveSe() const;

    virtual bool isUsingSeqData(const JAISeqDataRegion& seqDataRegion);
    virtual int releaseSeqData(const JAISeqDataRegion& seqDataRegion);

    JAISeCategoryMgr* getCategory(int categoryIndex) {
        return &mCategoryMgrs[categoryIndex];
    }
    JAIAudience* getAudience() {
        return mAudience;
    }
    JAIAudience* getAudience(int index) NO_INLINE {
        if (index >= 0 && index < NUM_CATEGORIES) {
            JAIAudience* rv = mCategoryMgrs[index].getAudience();
            if (rv != nullptr) {
                return rv;
            }
        }
        return mAudience;
    }
    JAISeqDataMgr* getSeqDataMgr() {
        return mSeqDataMgr;
    }
    JAISoundParamsMove* getParams() {
        return &mParams;
    }

    bool isActive() const {
        return getNumActiveSe() > 0;
    }

    static const int NUM_CATEGORIES = 16;

    /* 0x00C */ JAIAudience* mAudience;
    /* 0x010 */ JAISeqDataMgr* mSeqDataMgr;
    /* 0x014 */ JAISoundStrategyMgr< JAISe >* mStrategyMgr;
    /* 0x018 */ JAISeCategoryMgr mCategoryMgrs[NUM_CATEGORIES];
    /* 0x6D8 */ JAISoundParamsMove mParams;
};
