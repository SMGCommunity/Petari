#pragma once

#include "JSystem/JAudio2/JAISound.hpp"
#include "JSystem/JAudio2/JAISoundInfo.hpp"
#include "JSystem/JAudio2/JAUSectionHeap.hpp"
#include "JSystem/JAudio2/JAUSoundInfo.hpp"

// TODO: Not exactly sure what to call this class, or where it goes
class dummy {
public:
    virtual s32 getStreamFileEntry(JAISoundID) = 0;
    virtual ~dummy();
};

struct JAUStdSoundInfo : public JAISoundInfo, public JAUSoundInfo, public dummy {
    enum DataType {
        DATA_SE = 0x50,
        DATA_BGM = 0x60,
        DATA_STREAM = 0x70,
    };

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
