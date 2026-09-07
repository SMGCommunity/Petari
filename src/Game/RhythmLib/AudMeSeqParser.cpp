#include "Game/RhythmLib/AudMeSeqParser.hpp"
#include "Game/RhythmLib/AudMeTrack.hpp"
#include <JSystem/JAudio2/JASCriticalSection.hpp>

#define ARG_U8 0
#define ARG_U16 1
#define ARG_U24 2

#define FUNC_0(func)                                                                                                                                 \
    { &AudMeSeqParser::func, 0, 0 }
#define FUNC_1(func, a)                                                                                                                              \
    { &AudMeSeqParser::func, 1, (a << 0) }
#define FUNC_2(func, a, b)                                                                                                                           \
    { &AudMeSeqParser::func, 2, ((a << 0) | (b << 2)) }
#define FUNC_3(func, a, b, c)                                                                                                                        \
    { &AudMeSeqParser::func, 3, ((a << 0) | (b << 2) | (c << 4)) }
#define FUNC_4(func, a, b, c, d)                                                                                                                     \
    { &AudMeSeqParser::func, 4, ((a << 0) | (b << 2) | (c << 4) | (d << 6)) }
#define FUNC_5(func, a, b, c, d, e)                                                                                                                  \
    { &AudMeSeqParser::func, 5, ((a << 0) | (b << 2) | (c << 4) | (d << 6) | (e << 8)) }

AudMeSeqParser::Command AudMeSeqParser::sCmdInfo[0xA0] = {
    /* 0x00 */ FUNC_1(cmdBank, ARG_U8),
    /* 0x01 */ FUNC_1(cmdPrg, ARG_U8),
    /* 0x02 */ FUNC_1(cmdBProg, ARG_U16),
    /* 0x03 */ FUNC_1(cmdRelease, ARG_U16),
    /* 0x04 */ FUNC_5(cmdSimpleAdsr, ARG_U16, ARG_U16, ARG_U16, ARG_U16, ARG_U16),
    /* 0x05 */ {nullptr},
    /* 0x06 */ {nullptr},
    /* 0x07 */ {nullptr},
    /* 0x08 */ {nullptr},
    /* 0x09 */ {nullptr},
    /* 0x0A */ {nullptr},
    /* 0x0B */ {nullptr},
    /* 0x0C */ {nullptr},
    /* 0x0D */ {nullptr},
    /* 0x0E */ {nullptr},
    /* 0x0F */ {nullptr},
    /* 0x10 */ FUNC_2(cmdOpenTrack, ARG_U8, ARG_U24),
    /* 0x11 */ FUNC_1(cmdCloseTrack, ARG_U8),
    /* 0x12 */ {nullptr},
    /* 0x13 */ {nullptr},
    /* 0x14 */ {nullptr},
    /* 0x15 */ {nullptr},
    /* 0x16 */ {nullptr},
    /* 0x17 */ {nullptr},
    /* 0x18 */ {nullptr},
    /* 0x19 */ {nullptr},
    /* 0x1A */ {nullptr},
    /* 0x1B */ {nullptr},
    /* 0x1C */ {nullptr},
    /* 0x1D */ {nullptr},
    /* 0x1E */ {nullptr},
    /* 0x1F */ {nullptr},
    /* 0x20 */ FUNC_1(cmdStart, ARG_U8),
    /* 0x21 */ {nullptr},
    /* 0x22 */ {nullptr},
    /* 0x23 */ {nullptr},
    /* 0x24 */ FUNC_0(cmdEnd),
    /* 0x25 */ {nullptr},
    /* 0x26 */ {nullptr},
    /* 0x27 */ {nullptr},
    /* 0x28 */ FUNC_2(cmdWait, ARG_U8, ARG_U16),
    /* 0x29 */ {nullptr},
    /* 0x2A */ FUNC_1(cmdProgressOn, ARG_U8),
    /* 0x2B */ FUNC_0(cmdProgressOff),
    /* 0x2C */ {nullptr},
    /* 0x2D */ {nullptr},
    /* 0x2E */ {nullptr},
    /* 0x2F */ {nullptr},
    /* 0x30 */ FUNC_1(cmdJump, ARG_U24),
    /* 0x31 */ FUNC_1(cmdCall, ARG_U24),
    /* 0x32 */ FUNC_0(cmdRet),
    /* 0x33 */ FUNC_1(cmdLoopStart, ARG_U16),
    /* 0x34 */ FUNC_0(cmdLoopEnd),
    /* 0x35 */ FUNC_1(cmdJumpZ, ARG_U24),
    /* 0x36 */ FUNC_1(cmdJumpNZ, ARG_U24),
    /* 0x37 */ {nullptr},
    /* 0x38 */ FUNC_0(cmdLock),
    /* 0x39 */ FUNC_0(cmdNoWait),
    /* 0x3A */ FUNC_1(cmdCallZ, ARG_U24),
    /* 0x3B */ FUNC_1(cmdCallNZ, ARG_U24),
    /* 0x3C */ {nullptr},
    /* 0x3D */ {nullptr},
    /* 0x3E */ {nullptr},
    /* 0x3F */ {nullptr},
    /* 0x40 */ FUNC_2(cmdNote, ARG_U24, ARG_U8),
    /* 0x41 */ FUNC_3(cmdNoteL, ARG_U24, ARG_U8, ARG_U8),
    /* 0x42 */ FUNC_3(cmdNoteP, ARG_U24, ARG_U8, ARG_U8),
    /* 0x43 */ FUNC_4(cmdNoteLP, ARG_U24, ARG_U8, ARG_U8, ARG_U8),
    /* 0x44 */ {nullptr},
    /* 0x45 */ {nullptr},
    /* 0x46 */ {nullptr},
    /* 0x47 */ {nullptr},
    /* 0x48 */ FUNC_0(cmdNoteOff),
    /* 0x49 */ FUNC_0(cmdTie),
    /* 0x4A */ FUNC_0(cmdRest),
    /* 0x4B */ {nullptr},
    /* 0x4C */ {nullptr},
    /* 0x4D */ {nullptr},
    /* 0x4E */ {nullptr},
    /* 0x4F */ {nullptr},
    /* 0x50 */ FUNC_1(cmdNoteRangeUpper, ARG_U8),
    /* 0x51 */ FUNC_1(cmdNoteRangeLower, ARG_U8),
    /* 0x52 */ FUNC_1(cmdNoteRangeStart, ARG_U8),
    /* 0x53 */ FUNC_1(cmdNoteRangeStartU, ARG_U8),
    /* 0x54 */ FUNC_1(cmdNoteRangeStartD, ARG_U8),
    /* 0x55 */ FUNC_0(cmdNoStartRange),
    /* 0x56 */ {nullptr},
    /* 0x57 */ {nullptr},
    /* 0x58 */ FUNC_1(cmdPan, ARG_U8),
    /* 0x59 */ FUNC_1(cmdDolby, ARG_U8),
    /* 0x5A */ FUNC_1(cmdVolume, ARG_U16),
    /* 0x5B */ FUNC_1(cmdPitchBend, ARG_U16),
    /* 0x5C */ FUNC_1(cmdBendSense, ARG_U8),
    /* 0x5D */ FUNC_1(cmdFxMix, ARG_U16),
    /* 0x5E */ {nullptr},
    /* 0x5F */ {nullptr},
    /* 0x60 */ FUNC_1(cmdNoteReg, ARG_U8),
    /* 0x61 */ {nullptr},
    /* 0x62 */ {nullptr},
    /* 0x63 */ {nullptr},
    /* 0x64 */ {nullptr},
    /* 0x65 */ {nullptr},
    /* 0x66 */ {nullptr},
    /* 0x67 */ {nullptr},
    /* 0x68 */ FUNC_2(cmdCmpNI, ARG_U8, ARG_U8),
    /* 0x69 */ FUNC_2(cmdCmpCI, ARG_U8, ARG_U8),
    /* 0x6A */ {nullptr},
    /* 0x6B */ {nullptr},
    /* 0x6C */ {nullptr},
    /* 0x6D */ {nullptr},
    /* 0x6E */ {nullptr},
    /* 0x6F */ {nullptr},
    /* 0x70 */ FUNC_0(cmdNoteDefineStart),
    /* 0x71 */ FUNC_0(cmdNoteDefineEnd),
    /* 0x72 */ {nullptr},
    /* 0x73 */ {nullptr},
    /* 0x74 */ {nullptr},
    /* 0x75 */ {nullptr},
    /* 0x76 */ {nullptr},
    /* 0x77 */ {nullptr},
    /* 0x78 */ FUNC_1(cmdRangeExcepUpper, ARG_U24),
    /* 0x79 */ FUNC_1(cmdRangeExcepLower, ARG_U24),
    /* 0x7A */ {nullptr},
    /* 0x7B */ {nullptr},
    /* 0x7C */ {nullptr},
    /* 0x7D */ {nullptr},
    /* 0x7E */ {nullptr},
    /* 0x7F */ {nullptr},
    /* 0x80 */ FUNC_0(cmdBasenotePrev),
    /* 0x81 */ FUNC_1(cmdBasenoteChord, ARG_U24),
    /* 0x82 */ FUNC_1(cmdBasenoteScale, ARG_U24),
    /* 0x83 */ FUNC_1(cmdBasenoteChromatic, ARG_U24),
    /* 0x84 */ FUNC_1(cmdBasenotePart, ARG_U8),
    /* 0x85 */ FUNC_0(cmdBasenoteUpperPart),
    /* 0x86 */ FUNC_1(cmdBasenoteFix, ARG_U8),
    /* 0x87 */ FUNC_1(cmdBasenoteReg, ARG_U8),
    /* 0x88 */ {nullptr},
    /* 0x89 */ {nullptr},
    /* 0x8A */ FUNC_2(cmdAdjust, ARG_U8, ARG_U8),
    /* 0x8B */ {nullptr},
    /* 0x8C */ {nullptr},
    /* 0x8D */ {nullptr},
    /* 0x8E */ {nullptr},
    /* 0x8F */ {nullptr},
    /* 0x90 */ FUNC_3(cmdModify, ARG_U8, ARG_U8, ARG_U8),
    /* 0x91 */ FUNC_3(cmdModifyRnd, ARG_U8, ARG_U8, ARG_U8),
    /* 0x92 */ {nullptr},
    /* 0x93 */ {nullptr},
    /* 0x94 */ {nullptr},
    /* 0x95 */ {nullptr},
    /* 0x96 */ {nullptr},
    /* 0x97 */ {nullptr},
    /* 0x98 */ {nullptr},
    /* 0x99 */ {nullptr},
    /* 0x9A */ {nullptr},
    /* 0x9B */ {nullptr},
    /* 0x9C */ {nullptr},
    /* 0x9D */ {nullptr},
    /* 0x9E */ {nullptr},
    /* 0x9F */ {nullptr},
};

s32 AudMeSeqParser::cmdBank(AudMeTrack* pTrack, u32* pArgs) {
    pTrack->setBankNumber(pArgs[0]);
    return 0;
}

s32 AudMeSeqParser::cmdPrg(AudMeTrack* pTrack, u32* pArgs) {
    pTrack->setProgNumber(pArgs[0]);
    return 0;
}

s32 AudMeSeqParser::cmdBProg(AudMeTrack* pTrack, u32* pArgs) {
    u8 bankNo = pArgs[0] >> 8;
    u8 progNo = pArgs[0];
    pTrack->setBankNumber(bankNo);
    pTrack->setProgNumber(progNo);
    return 0;
}

s32 AudMeSeqParser::cmdRelease(AudMeTrack* pTrack, u32* pArgs) {
    pTrack->setRelease(pArgs[0]);
    return 0;
}

s32 AudMeSeqParser::cmdSimpleAdsr(AudMeTrack* pTrack, u32* pArgs) {
    pTrack->setOscAdsr(pArgs[0], pArgs[1], pArgs[2], pArgs[3], pArgs[4]);
    return 0;
}

s32 AudMeSeqParser::cmdOpenTrack(AudMeTrack* pTrack, u32* pArgs) {
    s32 trackNo = pArgs[0];
    u32 addr = pArgs[1];
    JASCriticalSection crit;
    AudMeTrack* child = pTrack->newChild(trackNo);
    if (child == nullptr) {
        return 0;
    }
    child->setSeqData(pTrack->getSeqCtrl()->getBase(), addr);
    return 0;
}

s32 AudMeSeqParser::cmdCloseTrack(AudMeTrack* pTrack, u32* pArgs) {
    pTrack->deleteChild(pArgs[0]);
    return 0;
}

s32 AudMeSeqParser::cmdStart(AudMeTrack* pTrack, u32* pArgs) {
    s32 time = pArgs[0];
    if (time == 6) {
        return 0;
    }

    pTrack->getSeqCtrl()->wait(time, 1);
    return 0;
}

s32 AudMeSeqParser::cmdEnd(AudMeTrack* pTrack, u32* pArgs) {
    return -1;
}

s32 AudMeSeqParser::cmdWait(AudMeTrack* pTrack, u32* pArgs) {
    pTrack->getSeqCtrl()->wait(pArgs[0], pArgs[1]);
    return 0;
}

s32 AudMeSeqParser::cmdProgressOn(AudMeTrack* pTrack, u32* pArgs) {
    pTrack->getSeqCtrl()->setProgress(pArgs[0]);
    return 0;
}

s32 AudMeSeqParser::cmdProgressOff(AudMeTrack* pTrack, u32* pArgs) {
    pTrack->getSeqCtrl()->setProgress(-1);
    return 0;
}

s32 AudMeSeqParser::cmdJump(AudMeTrack* pTrack, u32* pArgs) {
    u32 addr = pArgs[0];
    pTrack->getSeqCtrl()->jump(addr);
    return 0;
}

s32 AudMeSeqParser::cmdCall(AudMeTrack* pTrack, u32* pArgs) {
    u32 addr = pArgs[0];
    pTrack->getSeqCtrl()->call(addr);
    return 0;
}

s32 AudMeSeqParser::cmdJumpZ(AudMeTrack* pTrack, u32* pArgs) {
    u32 addr = pArgs[0];
    if (pTrack->getFlagZ()) {
        pTrack->getSeqCtrl()->jump(addr);
    }
    pTrack->clearJumpFlag();
    return 0;
}

s32 AudMeSeqParser::cmdJumpNZ(AudMeTrack* pTrack, u32* pArgs) {
    u32 addr = pArgs[0];
    if (pTrack->getFlagNZ()) {
        pTrack->getSeqCtrl()->jump(addr);
    }
    pTrack->clearJumpFlag();
    return 0;
}

s32 AudMeSeqParser::cmdCallZ(AudMeTrack* pTrack, u32* pArgs) {
    u32 addr = pArgs[0];
    if (pTrack->getFlagZ()) {
        pTrack->getSeqCtrl()->call(addr);
    }
    pTrack->clearJumpFlag();
    return 0;
}

s32 AudMeSeqParser::cmdCallNZ(AudMeTrack* pTrack, u32* pArgs) {
    u32 addr = pArgs[0];
    if (pTrack->getFlagNZ()) {
        pTrack->getSeqCtrl()->call(addr);
    }
    pTrack->clearJumpFlag();
    return 0;
}

s32 AudMeSeqParser::cmdRet(AudMeTrack* pTrack, u32* pArgs) {
    pTrack->getSeqCtrl()->ret();
    return 0;
}

s32 AudMeSeqParser::cmdLoopStart(AudMeTrack* pTrack, u32* pArgs) {
    pTrack->getSeqCtrl()->loopStart(pArgs[0]);
    return 0;
}

s32 AudMeSeqParser::cmdLoopEnd(AudMeTrack* pTrack, u32* pArgs) {
    pTrack->getSeqCtrl()->loopEnd();
    return 0;
}

s32 AudMeSeqParser::cmdLock(AudMeTrack* pTrack, u32* pArgs) {
    pTrack->getSeqCtrl()->lock();
    return 0;
}

s32 AudMeSeqParser::cmdNoWait(AudMeTrack* pTrack, u32* pArgs) {
    pTrack->getSeqCtrl()->mWaitTime = -1;
    pTrack->getSeqCtrl()->mTime = 0;
    return 0;
}

s32 AudMeSeqParser::cmdNote(AudMeTrack* pTrack, u32* pArgs) {
    u32 addr = pArgs[0];
    u32 note = pArgs[1];
    pTrack->setPitch(note);
    pTrack->getSeqCtrl()->call(addr);
    return 1;
}

s32 AudMeSeqParser::cmdNoteL(AudMeTrack* pTrack, u32* pArgs) {
    return 0;
}

s32 AudMeSeqParser::cmdNoteP(AudMeTrack* pTrack, u32* pArgs) {
    return 0;
}

s32 AudMeSeqParser::cmdNoteLP(AudMeTrack* pTrack, u32* pArgs) {
    return 0;
}

s32 AudMeSeqParser::cmdNoteOff(AudMeTrack* pTrack, u32* pArgs) {
    pTrack->noteOffAll(0);
    return 1;
}

s32 AudMeSeqParser::cmdTie(AudMeTrack* pTrack, u32* pArgs) {
    pTrack->getSeqCtrl()->autoWait();
    return 0;
}

s32 AudMeSeqParser::cmdRest(AudMeTrack* pTrack, u32* pArgs) {
    pTrack->noteOffAll(0);
    pTrack->getSeqCtrl()->autoWait();
    return 1;
}

s32 AudMeSeqParser::cmdNoteRangeUpper(AudMeTrack* pTrack, u32* pArgs) {
    pTrack->setNoteRangeUpper(pArgs[0]);
    return 0;
}

s32 AudMeSeqParser::cmdNoteRangeLower(AudMeTrack* pTrack, u32* pArgs) {
    pTrack->setNoteRangeLower(pArgs[0]);
    return 0;
}

s32 AudMeSeqParser::cmdNoteRangeStart(AudMeTrack* pTrack, u32* pArgs) {
    pTrack->setNoteRangeStart(pArgs[0]);
    return 0;
}

s32 AudMeSeqParser::cmdNoteRangeStartU(AudMeTrack* pTrack, u32* pArgs) {
    pTrack->setNoteRangeStart(0x7F);
    return 0;
}

s32 AudMeSeqParser::cmdNoteRangeStartD(AudMeTrack* pTrack, u32* pArgs) {
    pTrack->setNoteRangeStart(0x7E);
    return 0;
}

s32 AudMeSeqParser::cmdNoStartRange(AudMeTrack* pTrack, u32* pArgs) {
    pTrack->setNoteNoStartRange(true);
    return false;
}

s32 AudMeSeqParser::cmdPan(AudMeTrack* pTrack, u32* pArgs) {
    pTrack->setPan(pArgs[0]);
    return 0;
}

s32 AudMeSeqParser::cmdDolby(AudMeTrack* pTrack, u32* pArgs) {
    pTrack->setDolby(pArgs[0]);
    return 0;
}

s32 AudMeSeqParser::cmdVolume(AudMeTrack* pTrack, u32* pArgs) {
    pTrack->setVolume(pArgs[0]);
    return 0;
}

s32 AudMeSeqParser::cmdPitchBend(AudMeTrack* pTrack, u32* pArgs) {
    pTrack->setPitchBend(pArgs[0]);
    return 0;
}

s32 AudMeSeqParser::cmdBendSense(AudMeTrack* pTrack, u32* pArgs) {
    pTrack->setBendSense(pArgs[0]);
    return 0;
}

s32 AudMeSeqParser::cmdFxMix(AudMeTrack* pTrack, u32* pArgs) {
    pTrack->setFxMix(pArgs[0]);
    return 0;
}

s32 AudMeSeqParser::cmdNoteReg(AudMeTrack* pTrack, u32* pArgs) {
    pTrack->setNoteReg(pArgs[0]);
    return 0;
}

s32 AudMeSeqParser::cmdCmpNI(AudMeTrack* pTrack, u32* pArgs) {
    u8 reg = pArgs[0];

    if (reg == AudMeTrack::sReg[pArgs[0]]) {
        pTrack->setJumpFlag(true);
    } else {
        pTrack->setJumpFlag(false);
    }
    return 0;
}

s32 AudMeSeqParser::cmdCmpCI(AudMeTrack* pTrack, u32* pArgs) {
    u8 reg = pArgs[0];

    if (reg == pTrack->_163[pArgs[0]]) {
        pTrack->setJumpFlag(true);
    } else {
        pTrack->setJumpFlag(false);
    }
    return 0;
}

s32 AudMeSeqParser::cmdNoteDefineStart(AudMeTrack* pTrack, u32* pArgs) {
    pTrack->noteOffAll(0);
    return 1;
}

s32 AudMeSeqParser::cmdNoteDefineEnd(AudMeTrack* pTrack, u32* pArgs) {
    pTrack->startNote();
    pTrack->getSeqCtrl()->autoWait();
    pTrack->getSeqCtrl()->ret();
    return 0;
}

s32 AudMeSeqParser::cmdRangeExcepUpper(AudMeTrack* pTrack, u32* pArgs) {
    return 0;
}

s32 AudMeSeqParser::cmdRangeExcepLower(AudMeTrack* pTrack, u32* pArgs) {
    return 0;
}

s32 AudMeSeqParser::cmdBasenotePrev(AudMeTrack* pTrack, u32* pArgs) {
    pTrack->setBaseNote(pTrack->getBaseNotePrev());
    return 0;
}

s32 AudMeSeqParser::cmdBasenoteChord(AudMeTrack* pTrack, u32* pArgs) {
    u8* chordData = (u8*)pTrack->getSeqCtrl()->getAddr(pArgs[0]);
    pTrack->setBaseNoteFromChord(chordData + 2, chordData[0], chordData[1] != 0);
    return 0;
}

s32 AudMeSeqParser::cmdBasenoteScale(AudMeTrack* pTrack, u32* pArgs) {
    u8* scaleData = (u8*)pTrack->getSeqCtrl()->getAddr(pArgs[0]);
    pTrack->setBaseNoteFromScale(scaleData + 2, scaleData[0], scaleData[1] != 0);
    return 0;
}

s32 AudMeSeqParser::cmdBasenoteChromatic(AudMeTrack* pTrack, u32* pArgs) {
    u8* chromData = (u8*)pTrack->getSeqCtrl()->getAddr(pArgs[0]);
    pTrack->setBaseNoteFromChromatic(chromData + 2, chromData[0], chromData[1] != 0);
    return 0;
}

s32 AudMeSeqParser::cmdBasenotePart(AudMeTrack* pTrack, u32* pArgs) {
    pTrack->setBaseNoteFromOtherPart(pArgs[0]);
    return 0;
}

s32 AudMeSeqParser::cmdBasenoteUpperPart(AudMeTrack* pTrack, u32* pArgs) {
    pTrack->setBaseNoteFromUpperPart();
    return 0;
}

s32 AudMeSeqParser::cmdBasenoteFix(AudMeTrack* pTrack, u32* pArgs) {
    pTrack->setBaseNoteFix(pArgs[0]);
    return 0;
}

s32 AudMeSeqParser::cmdBasenoteReg(AudMeTrack* pTrack, u32* pArgs) {
    pTrack->setBaseNoteFromRegister(pArgs[0]);
    return 0;
}

s32 AudMeSeqParser::cmdAdjust(AudMeTrack* pTrack, u32* pArgs) {
    pTrack->adjustNote(pArgs[0], pArgs[1]);
    return 0;
}

s32 AudMeSeqParser::cmdModify(AudMeTrack* pTrack, u32* pArgs) {
    u8 arg0 = pArgs[0];
    u8 arg1 = pArgs[1];
    u8 arg2 = pArgs[2];
    pTrack->modifyNote(arg0, arg1, arg2);
    return 0;
}

s32 AudMeSeqParser::cmdModifyRnd(AudMeTrack* pTrack, u32* pArgs) {
    u8 arg0 = pArgs[0];
    u8 arg1 = pArgs[1];
    u8 rnd = pArgs[2];
    pTrack->modifyNoteRnd(arg0, arg1, rnd);
    return 0;
}

s32 AudMeSeqParser::parse(AudMeTrack* pTrack) {
    // FIXME: regswap in read16
    // https://decomp.me/scratch/rnZki

    if (pTrack == nullptr) {
        return 0;
    }

    Command* cmd = &sCmdInfo[pTrack->getSeqCtrl()->read8()];
    u32 readType = cmd->argWidths;
    int numArgs = cmd->numArgs;
    u32 args[8];

    for (int i = 0; i < numArgs; i++) {
        u32 arg = 0;
        switch (readType & 3) {
        case ARG_U8:
            arg = pTrack->getSeqCtrl()->read8();
            break;
        case ARG_U16:
            arg = pTrack->getSeqCtrl()->read16();
            break;
        case ARG_U24:
            arg = pTrack->getSeqCtrl()->read24();
            break;
        }
        args[i] = arg;
        readType = (u16)readType >> 2;
    }

    CommandFunc func = cmd->func;
    if (func != nullptr) {
        return (this->*func)(pTrack, args);
    }

    return 0;
}
