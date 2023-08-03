#pragma once

#include <revolution/types.h>

class JASTrack;

class JASSeqParser {
public:
	typedef s32 (JASSeqParser::*Command)(const void *, void *, JASTrack *, u32 *);

	enum BranchCondition {
		COND0 = 0,
		COND1,
		COND2,
		COND3,
		COND4,
		COND5
	};

	JASSeqParser();
	
	virtual ~JASSeqParser();
	virtual int parse(JASTrack *);
	virtual void execNoteOnMidi(JASTrack *, u32, u32, u32);
	virtual void execNoteOnGate(JASTrack *, u32, u32, u32, u32);
	virtual void execNoteOff(JASTrack *, u32);
	virtual void execCommand(JASTrack *, JASSeqParser::Command, u32, u32 *);

	bool conditionCheck(JASTrack *, BranchCondition);
	void writeReg(JASTrack *, u32, u32);
	u32 readReg(JASTrack *, u32) const;
	s32 cmdOpenTrack(JASTrack *, u32 *);
	s32 cmdCloseTrack(JASTrack *, u32 *);
	s32 cmdCall(JASTrack *, u32 *);
	s32 cmdCallF(JASTrack *, u32 *);
	s32 cmdRet(JASTrack *, u32 *);
	s32 cmdRetF(JASTrack *, u32 *);
	s32 cmdJmp(JASTrack *, u32 *);
	s32 cmdJmpF(JASTrack *, u32 *);
	s32 cmdJmpTable(JASTrack *, u32 *);
	s32 cmdCallTable(JASTrack *, u32 *);
	s32 cmdLoopS(JASTrack *, u32 *);
	s32 cmdLoopE(JASTrack *, u32 *);
	s32 cmdNote(JASTrack *, u32 *);
	s32 cmdNoteOn(JASTrack *, u32 *);
	s32 cmdNoteOff(JASTrack *, u32 *);
	s32 cmdReadPort(JASTrack *, u32 *);
	s32 cmdWritePort(JASTrack *, u32 *);
	s32 cmdParentWritePort(JASTrack *, u32 *);
	s32 cmdChildWritePort(JASTrack *, u32 *);
	s32 cmdParentReadPort(JASTrack *, u32 *);
	s32 cmdChildReadPort(JASTrack *, u32 *);
	s32 cmdCheckPortImport(JASTrack *, u32 *);
	s32 cmdCheckPortExport(JASTrack *, u32 *);
	s32 cmdWait(JASTrack *, u32 *);
	s32 cmdWaitByte(JASTrack *, u32 *);
	s32 cmdSetLastNote(JASTrack *, u32 *);
	s32 cmdEnvScaleSet(JASTrack *, u32 *);
	s32 cmdEnvSet(JASTrack *, u32 *);
	s32 cmdSimpleADSR(JASTrack *, u32 *);
	s32 cmdBusConnect(JASTrack *, u32 *);
	s32 cmdSetIntTable(JASTrack *, u32 *);
	s32 cmdSetInterrupt(JASTrack *, u32 *);
	s32 cmdClrI(JASTrack *, u32 *);
	s32 cmdRetI(JASTrack *, u32 *);
	s32 cmdIntTimer(JASTrack *, u32 *);
	s32 cmdSyncCPU(JASTrack *, u32 *);
	s32 cmdTempo(JASTrack *, u32 *);
	s32 cmdFinish(JASTrack *, u32 *);
	s32 cmdNop(JASTrack *, u32 *);
	s32 cmdFIRSet(JASTrack *, u32 *);
	s32 cmdIIRSet(JASTrack *, u32 *);
	s32 cmdIIRCutOff(JASTrack *, u32 *);
	s32 cmdBankPrg(JASTrack *, u32 *);
	s32 cmdBank(JASTrack *, u32 *);
	s32 cmdPrg(JASTrack *, u32 *);
	s32 cmdI(JASTrack *, u32 *);
	s32 cmdII(JASTrack *, u32 *);
	s32 cmdE(JASTrack *, u32 *);
	s32 cmdEI(JASTrack *, u32 *);
	s32 cmdReg(JASTrack *, u32 *);
	s32 cmdRegLoad(JASTrack *, u32 *);
	s32 cmdRegUni(JASTrack *, u32 *);
	s32 cmdRegTblLoad(JASTrack *, u32 *);
	s32 cmdDump(JASTrack *, u32 *);
	s32 cmdPrintf(JASTrack *, u32 *);
	void parseNoteOff(JASTrack *, u8);
	void parseNoteOne(JASTrack *, u8);
	void parseCommand(JASTrack *, u8, u16);
	void parseRegCommand(JASTrack *, int);
	
};