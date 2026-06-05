#pragma once

#include "JSystem/JAudio2/JASBasicWaveBank.hpp"
#include "JSystem/JAudio2/JASWaveInfo.hpp"
#include "JSystem/JKernel/JKRHeap.hpp"

struct JASSimpleWaveBank : JASWaveBank, JASWaveArc {
    struct TWaveHandle : JASWaveHandle {
        TWaveHandle();

        int getWavePtr() const;
        const JASWaveInfo* getWaveInfo() const;

        /* 0x04 */ JASWaveInfo mWaveInfo;
        /* 0x28 */ JASHeap* mHeap;
    };

    JASSimpleWaveBank();
    ~JASSimpleWaveBank();
    void setWaveTableSize(u32, JKRHeap*);
    JASWaveHandle* getWaveHandle(u32) const;
    void setWaveInfo(u32, JASWaveInfo const&);
    JASWaveArc* getWaveArc(u32);
    u32 getArcCount() const;

    /* 0x78 */ TWaveHandle* mWaveTable;
    /* 0x7C */ u32 mWaveTableSize;
};
