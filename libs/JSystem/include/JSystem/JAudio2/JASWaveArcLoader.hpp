#pragma once

#include "JSystem/JAudio2/JASHeapCtrl.hpp"

class JKRHeap;
class JKRSolidHeap;

struct JASDisposer {
    JASDisposer() {
    }
    virtual ~JASDisposer() {
    }
    virtual void onDispose() {
    }
};

#define DIR_MAX 64

struct JASWaveArcLoader {
    static JASHeap* getRootHeap();
    static void setCurrentDir(char const*);
    static char* getCurrentDir();

    static char sCurrentDir[DIR_MAX];
    static JASHeap* sAramHeap;
};

struct JASWaveArc : JASDisposer {
    JASWaveArc();
    bool loadSetup(u32);
    bool eraseSetup();
    static void loadToAramCallback(void*);
    bool sendLoadCmd();
    inline bool load(JASHeap*) NO_INLINE; /* overwritten in OverwriteJAudio */
    bool loadTail(JASHeap*);
    bool erase();
    void setEntryNum(s32);
    inline void setFileName(char const*) NO_INLINE; /* overwritten in OverwriteJAudio */

    virtual ~JASWaveArc();
    virtual void onDispose();
    virtual void onLoadDone() {
    }
    virtual void onEraseDone() {
    }

    s32 getStatus() const {
        return mStatus;
    }
    u32 getFileSize() const {
        return mFileLength;
    }
    JASHeap* getHeap() {
        return &mHeap;
    }

    struct loadToAramCallbackParams {
        // not official struct name
        /* 0x0 */ JASWaveArc* mWavArc;
        /* 0x4 */ s32 mEntryNum;
        /* 0x8 */ u32 mBase;
        /* 0xC */ u32 _C;
    };

    /* 0x04 */ mutable JASHeap mHeap;
    /* 0x48 */ u32 _48;
    /* 0x4C */ volatile s32 mStatus;
    /* 0x50 */ int mEntryNum;
    /* 0x54 */ u32 mFileLength;
    /* 0x58 */ u16 _58;
    /* 0x5A */ u16 _5A;
    /* 0x5C */ OSMutex mMutex;
};
