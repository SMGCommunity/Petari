#include "JSystem/JAudio2/JASSeqParser.h"
#include "JSystem/JAudio2/JASTrack.h"

#pragma optlevel O4,p

bool JASSeqParser::conditionCheck(JASTrack *track, BranchCondition cond) {
	u16 res = track->_80.read(JASRegisterParam::R3);
	switch(cond) {
		case JASSeqParser::COND0:
			return true;
		case JASSeqParser::COND1:
			return !res;
		case JASSeqParser::COND2:
			return res;
		case JASSeqParser::COND3:
			return !(res - 1);
		case JASSeqParser::COND4:
			return res >= 0x8000;
		case JASSeqParser::COND5:
			return res < 0x8000;
	}
	return false;
}

void JASSeqParser::writeReg(JASTrack *track, u32 reg, u32 value) {
	track->_80.write(JASRegisterParam::R3, value);
	if(reg < 0x40) track->_80.write((JASRegisterParam::RegID)reg, value);
	else switch(reg - 0x40) {
		case 0:
		case 1:
		case 2:
		case 3:
		case 4:
		case 5:
		case 6:
		case 7:
		case 8:
		case 9:
		case 0xA:
		case 0xB:
		case 0xC:
		case 0xD:
		case 0xE:
		case 0xF:
			track->_60[reg - 0x40] = value;
			break;
		case 0x22:
			track->setTimeBase(value);
			break;
		case 0x23:
			track->_22A = value;
			break;
		case 0x24:
			track->_230 = value;
			break;
		case 0x25:
			track->_233 = value;
			break;
		case 0x26:
			track->_220 = value;
			break;
		case 0x27:
			track->_22C = (u8)value;
			break;
		case 0x28:
			track->_22E = (u8)value;
			break;
		case 0x29:
			track->_218 = value;
			track->_218 /= 32767f;
			break;
		case 0x2A:
			track->_231 = value;
			track->_232 = value >> 8;
			break;
		case 0x2B:
			track->_231 = value;
			break;
		case 0x2C:
			track->_232 = value;
			break;
		case 0x2D:
			track->_224 = value;
			break;
		case 0x2E:
			track->_1E0 = value;
			track->_1E0 /= 1524f;
			break;
		case 0x2F:
			track->_1E0 = value;
			track->_1E0 /= 12192f;
			break;
		case 0x30:
			track->_1E8 = value;
			track->_1E8 *=  0.00390625f;
			break;
		case 0x31:
			track->_1E4 = value;
			track->_1E4 *= 0.015625f;
			break;
		case 0x32:
			track->_1EC = value;
			track->_1EC *= 0.015625f;
			break;
		case 0x33:
			track->_1F0 = value;
			break;
		case 0x34:
			track->_1F2 = value;
			break;
	}
}

u32 JASSeqParser::readReg(JASTrack *track, u32 reg) const {
	if(reg < 0x40) return track->_80.read((JASRegisterParam::RegID)reg);
	else {
		u32 ret = 0;
		switch(reg - 0x40) {
			case 0:
			case 1:
			case 2:
			case 3:
			case 4:
			case 5:
			case 6:
			case 7:
			case 8:
			case 9:
			case 0xA:
			case 0xB:
			case 0xC:
			case 0xD:
			case 0xE:
			case 0xF:
				ret = track->_60[reg - 0x40];
				break;
			// Create a bitmap of the current track's children
			case 0x20:
				u16 currBit = 1;
				JASTrack *child;
				for(u32 i = 0; i < 0x10; i++) {
					if(child = track->mChildren[i]) {
						if(child->_240 == 1) ret |= currBit;
					}
					currBit <<= 1;
				}
				break;
			case 0x21:
				if(!track->mNumStacks) return 0;
				ret = track->_60[track->mNumStacks - 0x20 + 5];
				break;
			case 0x22:
				ret = track->mTimerBase;
				break;
			case 0x23:
				ret = track->_22A;
				break;
			case 0x24:
				ret = track->_230;
				break;
			case 0x25:
				ret = track->_233;
				break;
			case 0x26:
				ret = track->_220;
				break;
			case 0x27:
				ret = track->_22C;
				break;
			case 0x28:
				ret = track->_22E;
				break;
			case 0x29: {
				f32 tmp = track->_218;
				ret = tmp * 32767f;
				break;
			}
			case 0x2A:
				ret = track->_232 | track->_231;
				break;
			case 0x2B:
				ret = track->_231;
				break;
			case 0x2C:
				ret = track->_232;
				break;
			case 0x2D:
				ret = track->_224;
				break;
			case 0x2E: {
				f32 tmp = track->_1E0;
				ret = tmp * 1524f;
				break;
			}
			case 0x2F: {
				f32 tmp = track->_1E0;
				ret = tmp * 12192f;
				break;
			}
			case 0x30: {
				f32 tmp = track->_1E8;
				ret = tmp * 256f;
				break;
			}
			case 0x31: {
				f32 tmp = track->_1E4;
				ret = tmp * 64f;
				break;
			}
			case 0x32: {
				f32 tmp = track->_1EC;
				ret = tmp * 64f;
				break;
			}
			case 0x33:
				ret = track->_1F0;
				break;
			case 0x34:
				ret = track->_1F2;
				break;
		}
		return ret;
	}
}

s32 JASSeqParser::cmdOpenTrack(JASTrack *track, u32 *args) {
	u32 arg0 = args[0], arg1 = args[1];
	JASTrack *child = track->openChild(arg0);
	if(!child) return 0;
	child->setSeqData(track->mSeqData, arg1);
	child->start();
	return 0;
}

s32 JASSeqParser::cmdCloseTrack(JASTrack *track, u32 *args) {
	track->closeChild(args[0]);
	return 0;
}

s32 JASSeqParser::cmdCall(JASTrack *track, u32 *args) {
	track->call(args[0]);
	return 0;
}

s32 JASSeqParser::cmdCallF(JASTrack *track, u32 *args) {
	if(conditionCheck(track, (BranchCondition)args[0])) track->call(args[1]);
	return 0;
}

s32 JASSeqParser::cmdRet(JASTrack *track, u32 *args) {
    track->ret();
    return 0;
}
s32 JASSeqParser::cmdRetF(JASTrack *track, u32 *args) {
	if(conditionCheck(track, (BranchCondition)args[0])) {
		track->ret();
		return 0;
	}
    else return 0;
}
s32 JASSeqParser::cmdJmp(JASTrack *track, u32 *args) {
    track->_4 = track->mSeqData + args[0];
    return 0;
}
s32 JASSeqParser::cmdJmpF(JASTrack *track, u32 *args) {
	if(conditionCheck(track, (BranchCondition)args[0])) track->_4 = track->mSeqData + args[1];;
	return 0;
}

inline void mult3(u32 &a) {
	a = a * 2 + a;
}

s32 JASSeqParser::cmdJmpTable(JASTrack *track, u32 *args) {
    u32 r4 = args[0];
    mult3(r4);
    track->_4 = track->mSeqData + track->calcSeekAmt(args[1] + r4);
    return 0;
}
/*s32 JASSeqParser::cmdCallTable(JASTrack *track, u32 *args) {
    track->call(track->calcSeekAmt(mull3(args[0]) + args[1]));
    return 0;
}
/*
s32 JASSeqParser::cmdLoopS(JASTrack *track, u32 *args) {
    track->loopS(args[0]);
    return 0;
}
s32 JASSeqParser::cmdLoopE(JASTrack *track, u32 *args) {
    track->loopE(args[0]);
    return 0;
}
s32 JASSeqParser::cmdNote(JASTrack *track, u32 *args) {
    track->note(args[0]);
    return 0;
}
s32 JASSeqParser::cmdNoteOn(JASTrack *track, u32 *args) {
    track->noteOn(args[0]);
    return 0;
}
s32 JASSeqParser::cmdNoteOff(JASTrack *track, u32 *args) {
    track->noteOff(args[0]);
    return 0;
}
s32 JASSeqParser::cmdReadPort(JASTrack *track, u32 *args) {
    track->readPort(args[0]);
    return 0;
}
s32 JASSeqParser::cmdWritePort(JASTrack *track, u32 *args) {
    track->writePort(args[0]);
    return 0;
}
s32 JASSeqParser::cmdParentWritePort(JASTrack *track, u32 *args) {
    track->parentWritePort(args[0]);
    return 0;
}
s32 JASSeqParser::cmdChildWritePort(JASTrack *track, u32 *args) {
    track->childWritePort(args[0]);
    return 0;
}
s32 JASSeqParser::cmdParentReadPort(JASTrack *track, u32 *args) {
    track->parentReadPort(args[0]);
    return 0;
}
s32 JASSeqParser::cmdChildReadPort(JASTrack *track, u32 *args) {
    track->childReadPort(args[0]);
    return 0;
}
s32 JASSeqParser::cmdCheckPortImport(JASTrack *track, u32 *args) {
    track->checkPortImport(args[0]);
    return 0;
}
s32 JASSeqParser::cmdCheckPortExport(JASTrack *track, u32 *args) {
    track->checkPortExport(args[0]);
    return 0;
}
s32 JASSeqParser::cmdWait(JASTrack *track, u32 *args) {
    track->wait(args[0]);
    return 0;
}
s32 JASSeqParser::cmdWaitByte(JASTrack *track, u32 *args) {
    track->waitByte(args[0]);
    return 0;
}
s32 JASSeqParser::cmdSetLastNote(JASTrack *track, u32 *args) {
    track->setLastNote(args[0]);
    return 0;
}
s32 JASSeqParser::cmdEnvScaleSet(JASTrack *track, u32 *args) {
    track->envScaleSet(args[0]);
    return 0;
}
s32 JASSeqParser::cmdEnvSet(JASTrack *track, u32 *args) {
    track->envSet(args[0]);
    return 0;
}
s32 JASSeqParser::cmdSimpleADSR(JASTrack *track, u32 *args) {
    track->simpleADSR(args[0]);
    return 0;
}
s32 JASSeqParser::cmdBusConnect(JASTrack *track, u32 *args) {
    track->busConnect(args[0]);
    return 0;
}
s32 JASSeqParser::cmdSetIntTable(JASTrack *track, u32 *args) {
    track->setIntTable(args[0]);
    return 0;
}
s32 JASSeqParser::cmdSetInterrupt(JASTrack *track, u32 *args) {
    track->setInterrupt(args[0]);
    return 0;
}
s32 JASSeqParser::cmdClrI(JASTrack *track, u32 *args) {
    track->clrI(args[0]);
    return 0;
}
s32 JASSeqParser::cmdRetI(JASTrack *track, u32 *args) {
    track->retI(args[0]);
    return 0;
}
s32 JASSeqParser::cmdIntTimer(JASTrack *track, u32 *args) {
    track->intTimer(args[0]);
    return 0;
}
s32 JASSeqParser::cmdSyncCPU(JASTrack *track, u32 *args) {
    track->syncCPU(args[0]);
    return 0;
}
s32 JASSeqParser::cmdTempo(JASTrack *track, u32 *args) {
    track->tempo(args[0]);
    return 0;
}
s32 JASSeqParser::cmdFinish(JASTrack *track, u32 *args) {
    track->finish(args[0]);
    return 0;
}
s32 JASSeqParser::cmdNop(JASTrack *track, u32 *args) {
    track->nop(args[0]);
    return 0;
}
s32 JASSeqParser::cmdFIRSet(JASTrack *track, u32 *args) {
    track->fIRSet(args[0]);
    return 0;
}
s32 JASSeqParser::cmdIIRSet(JASTrack *track, u32 *args) {
    track->iIRSet(args[0]);
    return 0;
}
s32 JASSeqParser::cmdIIRCutOff(JASTrack *track, u32 *args) {
    track->iIRCutOff(args[0]);
    return 0;
}
s32 JASSeqParser::cmdBankPrg(JASTrack *track, u32 *args) {
    track->bankPrg(args[0]);
    return 0;
}
s32 JASSeqParser::cmdBank(JASTrack *track, u32 *args) {
    track->bank(args[0]);
    return 0;
}
s32 JASSeqParser::cmdPrg(JASTrack *track, u32 *args) {
    track->prg(args[0]);
    return 0;
}
s32 JASSeqParser::cmdI(JASTrack *track, u32 *args) {
    track->I(args[0]);
    return 0;
}
s32 JASSeqParser::cmdII(JASTrack *track, u32 *args) {
    track->iI(args[0]);
    return 0;
}
s32 JASSeqParser::cmdE(JASTrack *track, u32 *args) {
    track->E(args[0]);
    return 0;
}
s32 JASSeqParser::cmdEI(JASTrack *track, u32 *args) {
    track->eI(args[0]);
    return 0;
}
s32 JASSeqParser::cmdReg(JASTrack *track, u32 *args) {
    track->reg(args[0]);
    return 0;
}
s32 JASSeqParser::cmdRegLoad(JASTrack *track, u32 *args) {
    track->regLoad(args[0]);
    return 0;
}
s32 JASSeqParser::cmdRegUni(JASTrack *track, u32 *args) {
    track->regUni(args[0]);
    return 0;
}
s32 JASSeqParser::cmdRegTblLoad(JASTrack *track, u32 *args) {
    track->regTblLoad(args[0]);
    return 0;
}
s32 JASSeqParser::cmdDump(JASTrack *track, u32 *args) {
    track->dump(args[0]);
    return 0;
}
s32 JASSeqParser::cmdPrintf(JASTrack *track, u32 *args) {
    track->printf(args[0]);
    return 0;
}*/

