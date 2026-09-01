#pragma once

#include "JSystem/JAudio2/JAISound.hpp"
#include "JSystem/JAudio2/JAISoundInfo.hpp"
#include "JSystem/JAudio2/JAIStreamDataMgr.hpp"
#include "JSystem/JAudio2/JAUSectionHeap.hpp"
#include "JSystem/JAudio2/JAUSoundInfo.hpp"
#include "JSystem/JSupport/JSUList.hpp"

// TODO: Not exactly sure what to call this class, or where it goes
class dummy {
public:
    virtual s32 getStreamFileEntry(JAISoundID) = 0;
    virtual ~dummy();
};

class JAUStdSoundInfo : public JAISoundInfo,
                        public JAUSoundInfo,
                        public JAIStreamDataMgr  // TODO: this may not be correct.
{
public:
    JAUStdSoundInfo(bool b) : JAISoundInfo(b), JAUSoundInfo(b){};

    virtual int getSoundType(JAISoundID) const;
    virtual int getCategory(JAISoundID) const;
    virtual u32 getPriority(JAISoundID) const;
    virtual void getSeInfo(JAISoundID, JAISe*) const;
    virtual void getSeqInfo(JAISoundID, JAISeq*) const;
    virtual void getStreamInfo(JAISoundID, JAIStream*) const;
    virtual ~JAUStdSoundInfo();

    virtual u16 getAudibleSw(JAISoundID) const;
    virtual u16 getBgmSeqResourceID(JAISoundID) const;
    virtual s32 getStreamFileEntry(JAISoundID);

    void getSoundInfo_(JAISoundID, JAISound*) const;
    const char* getStreamFilePath(JAISoundID);
};

class JAUDisposer_ {
public:
    JAUDisposer_() {
    }
    virtual ~JAUDisposer_(){};
};

template < class T >
class JAUDisposerObject_ : public T, public JAUDisposer_, public JSULink< JAUDisposerObject_< T > > {
public:
    JAUDisposerObject_(bool set) : T(set), JSULink< JAUDisposerObject_< T > >(this){};
    virtual ~JAUDisposerObject_();
};
