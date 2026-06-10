#pragma once

#include "JSystem/JAudio2/JASBasicInst.hpp"
#include "JSystem/JAudio2/JASOscillator.hpp"
#include "JSystem/JSupport/JSupport.hpp"
#include "revolution/types.h"

class JASBank;
class JASBasicBank;
class JKRHeap;

namespace JASBNKParser {
    struct TFileHeader {
        /* 0x0 */ int id;
        /* 0x4 */ u32 mSize;
        /* 0x8 */ u8 _08[4];
        /* 0xC */ u32 mVersion;
    };

    namespace Ver1 {
        struct TOsc {
            /* 0x00 */ u32 id;
            /* 0x04 */ u8 mTarget;
            /* 0x08 */ f32 _08;
            /* 0x0C */ u32 mTableOffset;
            /* 0x10 */ u32 _10;
            /* 0x14 */ f32 mScale;
            /* 0x18 */ f32 _18;
        };

        struct TPercData {
            /* 0x00 */ f32 mVolume;
            /* 0x04 */ f32 mPitch;
            /* 0x08 */ u8 mPan;
            /* 0x0A */ u16 mRelease;
            /* 0x0C */ u32 field_0xc;
        };

        struct TChunk {
            /* 0x0 */ u32 mID;
            /* 0x4 */ u32 mSize;
        };

        struct TEnvtChunk : TChunk {
            /* 0x8 */ u8 mData[0];
        };

        struct TOscChunk : TChunk {
            /* 0x8 */ u32 mCount;
            /* 0xC */ TOsc mOsc[0];
        };

        struct TListChunk : TChunk {
            /* 0x8 */ u32 count;
            /* 0xC */ u32 mOffsets[0];
        };

        struct TRandData {
            /* 0x00 */ u8 mType;
            /* 0x04 */ f32 mBase;
            /* 0x08 */ f32 mRange;
        };

        struct TSenseData {
            /* 0x00 */ u8 mType;
            /* 0x01 */ u8 mSenseType;
            /* 0x02 */ u8 mMaxPoint;
            /* 0x04 */ f32 mStartLvl;
            /* 0x08 */ f32 mEndLvl;
        };

        struct TVeloData {
            /* 0x00 */ u8 _0;
            /* 0x04 */ u32 _4;
            /* 0x08 */ f32 mVolume;
            /* 0x0C */ f32 mPitch;
        };

        struct TInstData {
            /* 0x00 */ f32 mVolume;
            /* 0x04 */ f32 mPitch;
        };

        TChunk* findChunk(void const*, u32);
        JASInstEffect* createEffector(void const*, JKRHeap*);
        JASBasicBank* createBasicBank(void const*, JKRHeap*);
    };  // namespace Ver1

    namespace Ver0 {
        template < typename T >
        struct TOffset {
            /* 0x0 */ u32 offset;
            T* ptr(void const* stream) const {
                return JSUConvertOffsetToPtr< T >(stream, offset);
            }
        };

        struct TOsc {
            /* 0x00 */ u8 mTarget;
            /* 0x04 */ f32 field_0x4;
            /* 0x08 */ TOffset< JASOscillator::Point > mPointOffset;
            /* 0x0C */ TOffset< JASOscillator::Point > field_0xc;
            /* 0x10 */ f32 mScale;
            /* 0x14 */ f32 field_0x14;
        };

        struct TVmap {
            /* 0x00 */ u8 _0;
            /* 0x04 */ u32 _4;
            /* 0x08 */ f32 mVolume;
            /* 0x0C */ f32 mPitch;
        };

        struct TRandEffect {
            /* 0x00 */ u8 mType;
            /* 0x04 */ f32 mBaseValue;
            /* 0x08 */ f32 mRange;
        };

        struct TSenseEffect {
            /* 0x00 */ u8 mType;
            /* 0x01 */ u8 mSenseType;
            /* 0x02 */ u8 mMaxPoint;
            /* 0x04 */ f32 mStartLvl;
            /* 0x08 */ f32 mEndLvl;
        };

        struct TKeymap {
            /* 0x00 */ u8 mHighKey;
            /* 0x04 */ u32 mVeloRegionCount;
            /* 0x08 */ TOffset< TVmap > mVmapOffset[0];
        };

        struct TInst {
            /* 0x00 */ u8 field_0x0[8];
            /* 0x08 */ f32 mVolume;
            /* 0x0C */ f32 mPitch;
            /* 0x10 */ TOffset< TOsc > mOscOffset[2];
            /* 0x18 */ TOffset< TRandEffect > mRandOffset[2];
            /* 0x20 */ TOffset< TSenseEffect > mSenseOffset[2];
            /* 0x28 */ u32 mKeyRegionCount;
            /* 0x2C */ TOffset< TKeymap > mKeymapOffset[0];
        };

        struct TPmap {
            /* 0x00 */ f32 mVolume;
            /* 0x04 */ f32 mPitch;
            /* 0x08 */ TOffset< TRandEffect > mRandOffset[2];
            /* 0x10 */ u32 mVeloRegionCount;
            /* 0x14 */ TOffset< TVmap > mVmapOffset[0];
        };

        struct TPerc {
            /* 0x000 */ u32 mMagic;
            /* 0x004 */ u8 _4[0x84];
            /* 0x088 */ TOffset< TPmap > mPmapOffset[0x80];
            /* 0x288 */ s8 mPan[0x80];
            /* 0x308 */ u16 mRelease[0x80];
        };

        struct TOffsetData {
            /* 0x000 */ u8 field_0x20[4];
            /* 0x004 */ TOffset< TInst > mInstOffset[0x80];
            /* 0x204 */ u8 field_0x204[0x190];
            /* 0x394 */ TOffset< TPerc > mPercOffset[12];
        };

        struct THeader {
            /* 0x00 */ u8 field_0x0[0x20];
            /* 0x20 */ TOffsetData mOffsets;
        };

        JASBasicBank* createBasicBank(void const*, JKRHeap*);
        inline JASOscillator::Data* findOscPtr(JASBasicBank*, THeader const*, TOsc const*);
        JASOscillator::Point const* getOscTableEndPtr(JASOscillator::Point const*);
    };  // namespace Ver0

    JASBank* createBank(void const*, JKRHeap*);
    JASBasicBank* createBasicBank(void const*, JKRHeap*);

    inline u32 getBankNumber(const void* param_0) {
        u32* ptr = (u32*)param_0;
        return ptr[2];
    }

    extern u32 sUsedHeapSize;
};  // namespace JASBNKParser
