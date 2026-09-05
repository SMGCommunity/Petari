#pragma once

#include "Game/RhythmLib/AudMeSeqReader.hpp"

class AudMeTrack;

class AudMeSeqCtrl {
public:
    AudMeSeqCtrl();

    void start(void*, u32);
    void wait(s32, s32);
    void autoWait();
    void inherit(AudMeSeqCtrl*);
    void rhythmProc(AudMeTrack*, s32);

    const AudMeSeqReader* getSeqReader() const {
        return &mReader;
    }
    AudMeSeqReader* getSeqReader_no_const() {
        return &mReader;
    }
    void* getBase() {
        return mReader.getBase();
    }
    void* getAddr(u32 addr) {
        return mReader.getAddr(addr);
    }
    u8 getByte(u32 addr) const {
        return mReader.getByte(addr);
    }
    u16 get16(u32 addr) const {
        return mReader.get16(addr);
    }
    u32 get24(u32 addr) const {
        return mReader.get24(addr);
    }
    u32 get32(u32 addr) const {
        return mReader.get32(addr);
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
    bool call(u32 addr) {
        return mReader.call(addr);
    }
    bool ret() {
        return mReader.ret();
    }
    void jump(u32 addr) {
        mReader.jump(addr);
    }
    bool loopStart(u32 loopCount) {
        return mReader.loopStart(loopCount);
    }
    bool loopEnd() {
        return mReader.loopEnd();
    }

    /* 0x00 */ AudMeSeqReader mReader;
    /* 0x3C */ u8 pad[0x80 - 0x3C];
};
