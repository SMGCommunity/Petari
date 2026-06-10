#pragma once

#include "JSystem/JAudio2/JASBasicInst.hpp"

struct JKRHeap;

const int PERC_MAX = 0x80;

struct JASDrumSet : public JASInst {
    struct TVelomap {
        /* 0x00 */ s32 _0;
        /* 0x04 */ s32 _4;
        /* 0x08 */ f32 mVolume;
        /* 0x0C */ f32 mPitch;
    };

    struct TPerc {
        TPerc();
        void setRelease(u32);

        void setVolume(f32 volume) {
            mVolume = volume;
        }
        void setPitch(f32 pitch) {
            mPitch = pitch;
        }
        void setPan(f32 pan) {
            mPan = pan;
        }

        void setVeloRegionCount(u32, JKRHeap*);
        TVelomap* getVeloRegion(int);
        const TVelomap* getVeloRegion(int) const;

        u32 getVeloRegionCount() const {
            return mVelomapCount;
        }

        void appendEffect(JASPtrListAdaptor< JASInstEffect >*);

        /* 0x00 */ f32 mVolume;
        /* 0x04 */ f32 mPitch;
        /* 0x08 */ f32 mPan;
        /* 0x0C */ u16 mRelease;
        /* 0x0E */ u16 field_0xe;
        /* 0x10 */ JASPtrList< JASInstEffect > mEffects;
        /* 0x14 */ s32 mVelomapCount;
        /* 0x18 */ TVelomap* mVelomap;
    };

    JASDrumSet();
    virtual ~JASDrumSet();
    void newPercArray(u8, JKRHeap*);
    virtual bool getParam(int, int, JASInstParam*) const;
    void setPerc(int, JASDrumSet::TPerc*);
    virtual u32 getType() const {
        return 'PERC';
    }

    /* 0x4 */ TPerc* mPercArray[PERC_MAX];
    /* 0x8 */  // u8 mPercNumMax;
};
