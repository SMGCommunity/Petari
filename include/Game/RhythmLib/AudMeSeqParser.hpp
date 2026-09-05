#pragma once

#include <revolution.h>

class AudMeTrack;

class AudMeSeqParser {
public:
    typedef s32 (AudMeSeqParser::*CommandFunc)(AudMeTrack*, u32*);
    typedef u16 (*Callback)(AudMeTrack*, u16);

    enum BranchCondition { COND_ALWAYS = 0, COND_NULL, COND_NONNULL, COND_1, COND_NEGATIVE, COND_POSITIVE };

    static Callback sCallbackFunc;
    struct Command {
        CommandFunc func;
        u16 numArgs;
        u16 argWidths;
    };

    static Command sCmdInfo[0xA0];

    AudMeSeqParser();

    s32 cmdBank(AudMeTrack*, u32*);
    s32 cmdPrg(AudMeTrack*, u32*);
    s32 cmdBProg(AudMeTrack*, u32*);
    s32 cmdRelease(AudMeTrack*, u32*);
    s32 cmdSimpleAdsr(AudMeTrack*, u32*);
    s32 cmdOpenTrack(AudMeTrack*, u32*);
    s32 cmdCloseTrack(AudMeTrack*, u32*);
    s32 cmdStart(AudMeTrack*, u32*);
    s32 cmdEnd(AudMeTrack*, u32*);
    s32 cmdWait(AudMeTrack*, u32*);
    s32 cmdProgressOn(AudMeTrack*, u32*);
    s32 cmdProgressOff(AudMeTrack*, u32*);
    s32 cmdJump(AudMeTrack*, u32*);
    s32 cmdCall(AudMeTrack*, u32*);
    s32 cmdJumpZ(AudMeTrack*, u32*);
    s32 cmdJumpNZ(AudMeTrack*, u32*);
    s32 cmdCallZ(AudMeTrack*, u32*);
    s32 cmdCallNZ(AudMeTrack*, u32*);
    s32 cmdRet(AudMeTrack*, u32*);
    s32 cmdLoopStart(AudMeTrack*, u32*);
    s32 cmdLoopEnd(AudMeTrack*, u32*);
    s32 cmdLock(AudMeTrack*, u32*);
    s32 cmdNoWait(AudMeTrack*, u32*);
    s32 cmdNote(AudMeTrack*, u32*);
    s32 cmdNoteL(AudMeTrack*, u32*);
    s32 cmdNoteP(AudMeTrack*, u32*);
    s32 cmdNoteLP(AudMeTrack*, u32*);
    s32 cmdNoteOff(AudMeTrack*, u32*);
    s32 cmdTie(AudMeTrack*, u32*);
    s32 cmdRest(AudMeTrack*, u32*);
    s32 cmdNoteRangeUpper(AudMeTrack*, u32*);
    s32 cmdNoteRangeLower(AudMeTrack*, u32*);
    s32 cmdNoteRangeStart(AudMeTrack*, u32*);
    s32 cmdNoteRangeStartU(AudMeTrack*, u32*);
    s32 cmdNoteRangeStartD(AudMeTrack*, u32*);
    s32 cmdNoStartRange(AudMeTrack*, u32*);
    s32 cmdPan(AudMeTrack*, u32*);
    s32 cmdDolby(AudMeTrack*, u32*);
    s32 cmdVolume(AudMeTrack*, u32*);
    s32 cmdPitchBend(AudMeTrack*, u32*);
    s32 cmdBendSense(AudMeTrack*, u32*);
    s32 cmdFxMix(AudMeTrack*, u32*);
    s32 cmdNoteReg(AudMeTrack*, u32*);
    s32 cmdCmpNI(AudMeTrack*, u32*);
    s32 cmdCmpCI(AudMeTrack*, u32*);
    s32 cmdNoteDefineStart(AudMeTrack*, u32*);
    s32 cmdNoteDefineEnd(AudMeTrack*, u32*);
    s32 cmdRangeExcepUpper(AudMeTrack*, u32*);
    s32 cmdRangeExcepLower(AudMeTrack*, u32*);
    s32 cmdBasenotePrev(AudMeTrack*, u32*);
    s32 cmdBasenoteChord(AudMeTrack*, u32*);
    s32 cmdBasenoteScale(AudMeTrack*, u32*);
    s32 cmdBasenoteChromatic(AudMeTrack*, u32*);
    s32 cmdBasenotePart(AudMeTrack*, u32*);
    s32 cmdBasenoteUpperPart(AudMeTrack*, u32*);
    s32 cmdBasenoteFix(AudMeTrack*, u32*);
    s32 cmdBasenoteReg(AudMeTrack*, u32*);
    s32 cmdAdjust(AudMeTrack*, u32*);
    s32 cmdModify(AudMeTrack*, u32*);
    s32 cmdModifyRnd(AudMeTrack*, u32*);

    s32 parse(AudMeTrack*);
};
