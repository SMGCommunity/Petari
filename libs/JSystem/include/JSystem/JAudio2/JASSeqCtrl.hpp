#pragma once

#include "revolution/types.h"

#include "JSystem/JAudio2/JASSeqParser.hpp"
#include "JSystem/JAudio2/JASSeqReader.hpp"

class JASTrack;
class JASSeqParser;
struct int24 {
    u8 padding;
    unsigned num : 24;
};

class JASSeqCtrl : public JASSeqReader {
public:
    enum IntrType { INTR_0 = 0, INTR_1, INTR_2, INTR_3, INTR_4, INTR_5, INTR_6 };
    static JASSeqParser sDefaultParser;

    JASSeqCtrl();
    void init();
    void start(void*, u32);
    int tickProc(JASTrack*);
    void interrupt(JASSeqCtrl::IntrType);
    void setIntrMask(u32);
    void clrIntrMask(u32);
    bool retIntr();
    s32 findIntr() NO_INLINE;
    void checkIntr();
    void timerProcess();

    u32 calcSeekAmt(u32 intr) const { return ((int24*)(intr + (u8*)mSeqBuff - 1))->num; }

    JASSeqParser* mParser;
    s32 mTimer;
    void* mCursorSwap;
    s32 _48;
    u16 _4C;
    u16 _4E;
    u8 mIntTimer;
    bool _51;
    u32 _54;
    u32 _58;
};