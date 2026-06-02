#pragma once

#include "revolution/types.h"

#include "JSystem/JAudio2/JASSeqParser.hpp"
#include "JSystem/JAudio2/JASSeqReader.hpp"

class JASTrack;
class JASSeqParser;

class JASSeqCtrl {
public:
    enum IntrType {
        INTRTYPE_VALUE_0,
        INTRTYPE_VALUE_1,
        INTRTYPE_VALUE_2,
        INTRTYPE_VALUE_3,
        INTRTYPE_VALUE_4,
        INTRTYPE_VALUE_5,
        INTRTYPE_VALUE_6,
    };

    JASSeqCtrl();
    void init();
    void start(void*, u32);
    int tickProc(JASTrack*);
    void interrupt(JASSeqCtrl::IntrType);
    void setIntrMask(u32);
    void clrIntrMask(u32);
    bool retIntr();
    s32 findIntr();
    void checkIntr();
    void timerProcess();

    const JASSeqReader* getSeqReader() const {
        return &mReader;
    }
    JASSeqReader* getSeqReader_no_const() {
        return &mReader;
    }
    void* getBase() {
        return mReader.getBase();
    }
    void* getAddr(u32 param_0) {
        return mReader.getAddr(param_0);
    }
    u8 getByte(u32 param_0) const {
        return mReader.getByte(param_0);
    }
    u16 get16(u32 param_0) const {
        return mReader.get16(param_0);
    }
    u32 get24(u32 param_0) const {
        return mReader.get24(param_0);
    }
    u32 get32(u32 param_0) const {
        return mReader.get32(param_0);
    }
    void* getCur() {
        return mReader.getCur();
    }
    u32 readByte() {
        return mReader.readByte();
    }
    u32 read16() {
        return mReader.read16();
    }
    u32 read24() {
        return mReader.read24();
    }
    bool call(u32 param_0) {
        return mReader.call(param_0);
    }
    bool ret() {
        return mReader.ret();
    }
    void jump(u32 param_0) {
        mReader.jump(param_0);
    }
    bool loopStart(u32 param_0) {
        return mReader.loopStart(param_0);
    }
    bool loopEnd() {
        return mReader.loopEnd();
    }
    int readMidiValue() {
        return mReader.readMidiValue();
    }
    void wait(s32 time) {
        mTimer = time;
    }
    void clrIntr() {
        mCursorSwap = NULL;
    }
    void setIntrTable(u32 param_0) {
        _48 = param_0;
    }
    void setIntrTimer(u32 time, u32 param_1) {
        mIntTimer = time;
        _54 = param_1;
        _58 = param_1;
    }
    void waitNoteFinish() {
        _51 = 1;
    }

    /* 0x00 */ JASSeqReader mReader;
    /* 0x3C */ JASSeqParser* mParser;
    /* 0x40 */ s32 mTimer;
    /* 0x44 */ void* mCursorSwap;
    /* 0x48 */ u32 _48;
    /* 0x4C */ u16 _4C;
    /* 0x4E */ u16 _4E;
    /* 0x50 */ u8 mIntTimer;
    /* 0x51 */ u8 _51;
    /* 0x52 */ u16 _52;
    /* 0x54 */ u32 _54;
    /* 0x58 */ u32 _58;
    static JASSeqParser sDefaultParser;
};
