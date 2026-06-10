#include "JSystem/JAudio2/JAISeqDataMgr.hpp"
#include "JSystem/JAudio2/JASGlobal.hpp"
#include "JSystem/JSupport/JSUList.hpp"

struct JAIAudience;
class JAISeq;

class JAISeqMgr : public JASGlobalInstance< JAISeqMgr >, public JAISeqDataUser, public JAISoundActivityMgr {
public:
    enum ReleaseSeqResult {
        RELEASE_SEQ_1 = 1,
        RELEASE_SEQ_2 = 2,
    };
    JAISeqMgr(bool setInstance);
    void freeDeadSeq_();
    bool startSound(JAISoundID id, JAISoundHandle* handle, const TVec3f* posPtr);
    void calc();
    void stop();
    void stop(u32 fadeTime);
    void stopSoundID(JAISoundID id);
    void mixOut();
    JAISeq* beginStartSeq_() NO_INLINE;

    void initParams() {
        mMove.init();
    }

    bool endStartSeq_(JAISeq* seq, JAISoundHandle* handle);

    virtual bool isUsingSeqData(const JAISeqDataRegion& seqDataRegion);
    virtual int releaseSeqData(const JAISeqDataRegion& seqDataRegion);
    virtual ~JAISeqMgr() {
    }

    void setAudience(JAIAudience* audience) {
        mAudience = audience;
    }
    JAIAudience* getAudience() {
        return mAudience;
    }

    JAISeqDataMgr* getSeqDataMgr() {
        return seqDataMgr;
    }

    void setSeqDataMgr(JAISeqDataMgr* seqDataMgr) {
        resetSeqDataMgr();
        seqDataMgr = seqDataMgr;
        seqDataMgr->setSeqDataUser(this);
    }

    void resetSeqDataMgr() {
        if (seqDataMgr) {
            seqDataMgr->setSeqDataUser(nullptr);
            seqDataMgr = nullptr;
        }
    }

    JAISoundParamsMove* getParams() {
        return &mMove;
    }
    bool isActive() const {
        return mSeqList.getNumLinks() != 0;
    }
    int getNumActiveSeqs() const {
        return mSeqList.getNumLinks();
    }
    void pause(bool paused) {
        mSoundActivity.pause(paused);
    }

private:
    /* 0x08 */ JAIAudience* mAudience;
    /* 0x0C */ JAISeqDataMgr* seqDataMgr;
    /* 0x10 */ JAISoundStrategyMgr< JAISeq >* soundStrategyMgr;
    /* 0x14 */ JAISoundParamsMove mMove;
    /* 0x64 */ JSUList< JAISeq > mSeqList;
    /* 0x70 */ int mNumTracks;
};
