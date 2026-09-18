#pragma once

#include "JSystem/JSupport/JSupport.hpp"

class JKRHeap;
class JASWaveBank;
struct JASBasicWaveBank;
struct JASSimpleWaveBank;

class JASWSParser {
public:
    template < class T >
    class TOffset {
    public:
        T* ptr(void const* param_0) const {
            return JSUConvertOffsetToPtr< T >(param_0, mOffset);
        }

    public:
        /* 0x0 */ u32 mOffset;
    };

    struct TCtrlWave {
        union {
            struct {
                /* 0x0 */ u16 mWaveGroupId;
                /* 0x2 */ u16 mWaveId;
            };
            /* 0x0 */ u32 mData;
        };
    };

    struct TWave {
        /* 0x00 */ u8 _00;
        /* 0x01 */ u8 mFormat;
        /* 0x02 */ u8 mBaseKey;
        /* 0x04 */ f32 mSampleRate;
        /* 0x08 */ u32 mAWStartOffs;
        /* 0x0C */ u32 mAWLength;
        /* 0x10 */ u32 mLoopFlags;
        /* 0x14 */ u32 mSampleLoopStart;
        /* 0x18 */ u32 mSampleLoopEnd;
        /* 0x1C */ u32 mSampleCount;
        /* 0x20 */ s16 mpLastSample;
        /* 0x22 */ s16 mpPenultSample;
    };

    struct TWaveArchive {
        /* 0x00 */ char mFileName[0x70];
        /* 0x70 */ int mWaveCount;
        /* 0x74 */ TOffset< TWave > mWaveOffsets[0];
    };

    struct TWaveArchiveBank {
        /* 0x0 */ char mMagic[4];
        /* 0x4 */ int mWaveGroupCount;
        /* 0x8 */ TOffset< TWaveArchive > mArchiveOffsets[0];
    };

    struct TCtrl {
        /* 0x0 */ char mMagic[4];
        /* 0x4 */ u32 mWaveCount;
        /* 0x8 */ TOffset< TCtrlWave > mCtrlWaveOffsets[0];
    };

    struct TCtrlScene {
        /* 0x0 */ char mMagic[4];
        /* 0x4 */ u8 _04[8];
        /* 0xC */ TOffset< TCtrl > mCtrlOffset;
    };

    struct TCtrlGroup {
        /* 0x0 */ char mMagic[4];
        /* 0x4 */ u8 _04[4];
        /* 0x8 */ u32 mGroupCount;
        /* 0xC */ TOffset< TCtrlScene > mCtrlSceneOffsets[0];
    };

    /** @fabricated */
    struct THeader {
        /* 0x00 */ char mMagic[4];
        /* 0x04 */ int mSize;
        /* 0x08 */ int mId;
        /* 0x0C */ u32 mWaveTableSize;
        /* 0x10 */ TOffset< TWaveArchiveBank > mArchiveBankOffset;
        /* 0x14 */ TOffset< TCtrlGroup > mCtrlGroupOffset;
    };

    static u32 getGroupCount(void const*);
    static JASWaveBank* createWaveBank(void const*, JKRHeap*);
    static JASBasicWaveBank* createBasicWaveBank(void const*, JKRHeap*);
    static JASSimpleWaveBank* createSimpleWaveBank(void const*, JKRHeap*);

    static u32 sUsedHeapSize;
};
