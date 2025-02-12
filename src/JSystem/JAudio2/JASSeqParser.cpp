#include "JSystem/JAudio2/JASSeqParser.hpp"
#include "JSystem/JAudio2/JASTrack.hpp"
#include "JSystem/JMath/random.hpp"
#include "JSystem/JAudio2/JASReport.hpp"

// At address 80560cb8
const static s16 sIIRCutoff[0x208] = {
    0x0000, 0x0000, 0x0000, 0x8048, 0x947C, 0x8048, 0x96D0, 0x8048,
    0x94C8, 0x8048, 0x95D4, 0x8048, 0x9534, 0x8048, 0x954C, 0x8048,
    0x9590, 0x8048, 0x9708, 0x0F5C, 0x0A3D, 0x4665, 0x1E73, 0x0F5E,
    0x0A3D, 0x4664, 0x1E73, 0x0F63, 0x0A3C, 0x4661, 0x1E71, 0x0F6C,
    0x0A3C, 0x465B, 0x1E6F, 0x0F79, 0x0A3A, 0x4653, 0x1E6B, 0x0F89,
    0x0A39, 0x4649, 0x1E67, 0x0F9C, 0x0A37, 0x463D, 0x1E62, 0x0FB4,
    0x0A35, 0x462E, 0x1E5B, 0x0FCE, 0x0A33, 0x461D, 0x1E54, 0x0FED,
    0x0A30, 0x460A, 0x1E4C, 0x100F, 0x0A2D, 0x45F5, 0x1E43, 0x1034,
    0x0A29, 0x45DE, 0x1E39, 0x105D, 0x0A26, 0x45C4, 0x1E2D, 0x108A,
    0x0A22, 0x45A8, 0x1E21, 0x10BA, 0x0A1D, 0x458A, 0x1E14, 0x10EE,
    0x0A18, 0x456A, 0x1E06, 0x1126, 0x0A13, 0x4547, 0x1DF7, 0x1161,
    0x0A0E, 0x4522, 0x1DE7, 0x119F, 0x0A08, 0x44FB, 0x1DD6, 0x11E1,
    0x0A02, 0x44D2, 0x1DC5, 0x1227, 0x09FC, 0x44A6, 0x1DB2, 0x1270,
    0x09F5, 0x4478, 0x1D9E, 0x12BD, 0x09EE, 0x4448, 0x1D89, 0x130E,
    0x09E7, 0x4416, 0x1D73, 0x1362, 0x09DF, 0x43E1, 0x1D5D, 0x13B9,
    0x09D7, 0x43AB, 0x1D45, 0x1415, 0x09CF, 0x4372, 0x1D2C, 0x1473,
    0x09C7, 0x4336, 0x1D13, 0x14D6, 0x09BE, 0x42F9, 0x1CF8, 0x153C,
    0x09B4, 0x42B9, 0x1CDD, 0x15A5, 0x09AB, 0x4277, 0x1CC0, 0x1612,
    0x09A1, 0x4233, 0x1CA3, 0x1683, 0x0997, 0x41ED, 0x1C84, 0x16F7,
    0x098C, 0x41A4, 0x1C65, 0x176F, 0x0981, 0x4159, 0x1C44, 0x17EA,
    0x0976, 0x410C, 0x1C23, 0x1869, 0x096A, 0x40BD, 0x1C01, 0x18EB,
    0x095F, 0x406B, 0x1BDD, 0x1972, 0x0952, 0x4018, 0x1BB9, 0x19FB,
    0x0946, 0x3FC2, 0x1B94, 0x1A88, 0x0939, 0x3F69, 0x1B6E, 0x1B19,
    0x092C, 0x3F0F, 0x1B47, 0x1BAE, 0x091E, 0x3EB2, 0x1B1E, 0x1C46,
    0x0911, 0x3E53, 0x1AF5, 0x1CE1, 0x0902, 0x3DF2, 0x1ACB, 0x1D80,
    0x08F4, 0x3D8E, 0x1AA0, 0x1E23, 0x08E5, 0x3D29, 0x1A74, 0x1EC9,
    0x08D6, 0x3CC1, 0x1A47, 0x1F73, 0x08C7, 0x3C57, 0x1A19, 0x2020,
    0x08B7, 0x3BEA, 0x19EB, 0x20D1, 0x08A7, 0x3B7C, 0x19BB, 0x2186,
    0x0896, 0x3B0B, 0x198A, 0x223E, 0x0886, 0x3A98, 0x1958, 0x22FA,
    0x0875, 0x3A22, 0x1925, 0x23B9, 0x0863, 0x39AB, 0x18F2, 0x247C,
    0x0851, 0x3931, 0x18BD, 0x2542, 0x083F, 0x38B5, 0x1887, 0x260C,
    0x082D, 0x3837, 0x1851, 0x26DA, 0x081A, 0x37B6, 0x1819, 0x27AB,
    0x0807, 0x3734, 0x17E1, 0x2880, 0x07F4, 0x36AF, 0x17A7, 0x2958,
    0x07E0, 0x3628, 0x176D, 0x2A34, 0x07CC, 0x359E, 0x1731, 0x2B14,
    0x07B8, 0x3512, 0x16F5, 0x2BF7, 0x07A3, 0x3485, 0x16B7, 0x2CDD,
    0x078E, 0x33F4, 0x1679, 0x2DC8, 0x0779, 0x3362, 0x163A, 0x2EB5,
    0x0764, 0x32CD, 0x15FA, 0x2FA7, 0x074E, 0x3237, 0x15B8, 0x309C,
    0x0737, 0x319E, 0x1576, 0x3194, 0x0721, 0x3102, 0x1533, 0x3290,
    0x070A, 0x3065, 0x14EF, 0x3390, 0x06F3, 0x2FC5, 0x14AA, 0x3493,
    0x06DB, 0x2F23, 0x1464, 0x359A, 0x06C3, 0x2E7F, 0x141C, 0x36A4,
    0x06AB, 0x2DD8, 0x13D4, 0x37B2, 0x0692, 0x2D2F, 0x138C, 0x38C4,
    0x067A, 0x2C85, 0x1342, 0x39D9, 0x0660, 0x2BD7, 0x12F7, 0x3AF1,
    0x0647, 0x2B28, 0x12AB, 0x3C0E, 0x062D, 0x2A76, 0x125E, 0x3D2E,
    0x0613, 0x29C2, 0x1210, 0x3E51, 0x05F8, 0x290C, 0x11C1, 0x3F78,
    0x05DE, 0x2854, 0x1172, 0x40A3, 0x05C2, 0x2799, 0x1121, 0x41D1,
    0x05A7, 0x26DC, 0x10CF, 0x4302, 0x058B, 0x261D, 0x107D, 0x4438,
    0x056F, 0x255C, 0x1029, 0x4571, 0x0553, 0x2499, 0x0FD4, 0x46AD,
    0x0536, 0x23D3, 0x0F7F, 0x47ED, 0x0519, 0x230B, 0x0F28, 0x4931,
    0x04FB, 0x2241, 0x0ED1, 0x4A78, 0x04DE, 0x2174, 0x0E78, 0x4BC2,
    0x04C0, 0x20A5, 0x0E1F, 0x4D11, 0x04A1, 0x1FD4, 0x0DC5, 0x4E63,
    0x0482, 0x1F01, 0x0D69, 0x4FB8, 0x0463, 0x1E2C, 0x0D0D, 0x5111,
    0x0444, 0x1D54, 0x0CB0, 0x526E, 0x0424, 0x1C7A, 0x0C51, 0x53CE,
    0x0404, 0x1B9E, 0x0BF2, 0x5532, 0x03E4, 0x1AC0, 0x0B92, 0x5699,
    0x03C3, 0x19DF, 0x0B31, 0x5804, 0x03A2, 0x18FD, 0x0ACF, 0x5972,
    0x0381, 0x1818, 0x0A6C, 0x5AE5, 0x035F, 0x1730, 0x0A08, 0x5C5A,
    0x033D, 0x1647, 0x09A3, 0x5DD3, 0x031B, 0x155B, 0x093D, 0x5F50,
    0x02F9, 0x146D, 0x08D6, 0x60D1, 0x02D6, 0x137D, 0x086E, 0x6255,
    0x02B2, 0x128A, 0x0805, 0x63DC, 0x028F, 0x1196, 0x079B, 0x6567,
    0x026B, 0x109F, 0x0730, 0x66F6, 0x0247, 0x0FA6, 0x06C5, 0x6888,
    0x0222, 0x0EAA, 0x0658, 0x6A1E, 0x01FD, 0x0DAD, 0x05EA, 0x6BB7,
    0x01D8, 0x0CAD, 0x057B, 0x6D54, 0x01B2, 0x0BAB, 0x050C, 0x6EF5,
    0x018D, 0x0AA6, 0x049B, 0x7099, 0x0166, 0x09A0, 0x042A, 0x7241,
    0x0140, 0x0897, 0x03B7, 0x73EC, 0x0119, 0x078C, 0x0344, 0x759B,
    0x00F2, 0x067F, 0x02CF, 0x774D, 0x00CA, 0x056F, 0x025A, 0x7903,
    0x00A3, 0x045D, 0x01E3, 0x7ABD, 0x007A, 0x0349, 0x016C, 0x7C7A
};

JASSeqParser::Command JASSeqParser::sCmdInfo[0x60] = {
	{nullptr}, {nullptr}, {nullptr}, {nullptr}, {nullptr}, {nullptr}, 
	{nullptr}, {nullptr}, {nullptr}, {nullptr}, {nullptr}, {nullptr},
	{nullptr}, {nullptr}, {nullptr}, {nullptr}, {nullptr},
	{&JASSeqParser::cmdNoteOn, 3},
	{&JASSeqParser::cmdNoteOff, 1},
	{&JASSeqParser::cmdNote, 4, 0x40},
	{&JASSeqParser::cmdSetLastNote, 1},
	{nullptr}, {nullptr}, {nullptr},
	{&JASSeqParser::cmdParamE, 2},
	{&JASSeqParser::cmdParamI, 2, 4},
	{&JASSeqParser::cmdParamEI, 3, 0x10},
	{&JASSeqParser::cmdParamII, 3, 0x14},
	{nullptr}, {nullptr}, {nullptr}, {nullptr}, {nullptr}, 
	{&JASSeqParser::cmdOpenTrack, 2, 8},
	{&JASSeqParser::cmdCloseTrack, 1},
	{&JASSeqParser::cmdCall, 1, 2},
	{&JASSeqParser::cmdCallF, 2, 8},
	{&JASSeqParser::cmdRet, 0},
	{&JASSeqParser::cmdRetF, 1},
	{&JASSeqParser::cmdJmp, 1, 2},
	{&JASSeqParser::cmdJmpF, 2, 8},
	{&JASSeqParser::cmdJmpTable, 2, 0xb},
	{&JASSeqParser::cmdCallTable, 2, 0xb},
	{&JASSeqParser::cmdLoopS, 1, 1},
	{&JASSeqParser::cmdLoopE, 0},
	{nullptr}, {nullptr}, {nullptr},
	{&JASSeqParser::cmdReadPort, 2},
	{&JASSeqParser::cmdWritePort, 2, 0xc},
	{&JASSeqParser::cmdCheckPortImport, 1},
	{&JASSeqParser::cmdCheckPortExport, 1},
	{&JASSeqParser::cmdParentWritePort, 2, 0xc},
	{&JASSeqParser::cmdChildWritePort, 2, 0xc},
	{&JASSeqParser::cmdParentReadPort, 2},
	{&JASSeqParser::cmdChildReadPort, 2},
	{&JASSeqParser::cmdRegLoad, 2, 4},
	{&JASSeqParser::cmdReg, 3, 0x30},
	{&JASSeqParser::cmdReg, 3, 0x10},
	{&JASSeqParser::cmdRegUni, 2},
	{&JASSeqParser::cmdRegTblLoad, 4, 0xe0},
	{nullptr}, {nullptr}, {nullptr},
	{&JASSeqParser::cmdTempo, 1, 1},
	{&JASSeqParser::cmdBankPrg, 1, 1},
	{&JASSeqParser::cmdBank, 1},
	{&JASSeqParser::cmdPrg, 1},
	{nullptr}, {nullptr}, {nullptr},
	{&JASSeqParser::cmdEnvScaleSet, 2, 4},
	{&JASSeqParser::cmdEnvSet, 2, 8},
	{&JASSeqParser::cmdSimpleADSR, 5, 0x155},
	{&JASSeqParser::cmdBusConnect, 2, 4},
	{&JASSeqParser::cmdIIRCutOff, 1},
	{&JASSeqParser::cmdIIRSet, 4, 0x55},
	{&JASSeqParser::cmdFIRSet, 1, 2},
	{nullptr}, {nullptr},
	{&JASSeqParser::cmdWait, 0},
	{&JASSeqParser::cmdWaitByte, 1},
	{nullptr},
	{&JASSeqParser::cmdSetIntTable, 1, 2},
	{&JASSeqParser::cmdSetInterrupt, 1, 1},
	{&JASSeqParser::cmdDisInterrupt, 1, 1},
	{&JASSeqParser::cmdClrI, 0},
	{&JASSeqParser::cmdRetI, 0},
	{&JASSeqParser::cmdIntTimer, 2, 4},
	{&JASSeqParser::cmdSyncCPU, 1, 1},
	{nullptr}, {nullptr}, {nullptr},
	{&JASSeqParser::cmdPrintf, 0},
	{&JASSeqParser::cmdNop, 0},
	{&JASSeqParser::cmdFinish, 0}
};

JASSeqParser::Command JASSeqParser::sExtCmdInfo[0xff] = {
	{nullptr},
	{&JASSeqParser::cmdDump},
    {nullptr}, {nullptr}, {nullptr}, {nullptr}, {nullptr}
};

bool JASSeqParser::conditionCheck(JASTrack *track, BranchCondition cond) {
	u16 res = track->mRegs.read(JASRegisterParam::REG_F);
	switch(cond) {
		case JASSeqParser::COND_ALWAYS:
			return true;
		case JASSeqParser::COND_NULL:
			return !res;
		case JASSeqParser::COND_NONNULL:
			return res;
		case JASSeqParser::COND_1:
			return !(res - 1);
		case JASSeqParser::COND_NEGATIVE:
			return res >= 0x8000;
		case JASSeqParser::COND_POSITIVE:
			return res < 0x8000;
	}
	return false;
}

void JASSeqParser::writeReg(JASTrack *track, u32 reg, u32 value) {
	track->mRegs.write(JASRegisterParam::REG_F, value);
	if(reg < 0x40) track->mRegs.write((JASRegisterParam::RegID)reg, value);
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
			track->mPorts.mPorts[reg - 0x40] = value;
			break;
		case 0x22:
			track->setTimebase(value);
			break;
		case 0x23:
			track->mTransposeAmt = value;
			break;
		case 0x24:
			track->_230 = value;
			break;
		case 0x25:
			track->mGateLatency = value;
			break;
		case 0x26:
			track->_220 = value;
			break;
		case 0x27:
			track->mBank = (u8)value;
			break;
		case 0x28:
			track->mPrg = (u8)value;
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
	if(reg < 0x40) return track->mRegs.read((JASRegisterParam::RegID)reg);
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
				ret = track->mPorts.mPorts[reg - 0x40];
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
				ret = track->mPorts.mPorts[track->mNumStacks - 0x20 + 5];
				break;
			case 0x22:
				ret = track->mTimebase;
				break;
			case 0x23:
				ret = track->mTransposeAmt;
				break;
			case 0x24:
				ret = track->_230;
				break;
			case 0x25:
				ret = track->mGateLatency;
				break;
			case 0x26:
				ret = track->_220;
				break;
			case 0x27:
				ret = track->mBank;
				break;
			case 0x28:
				ret = track->mPrg;
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
	child->setSeqData(track->mSeqBuff, arg1);
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
    track->mSeqCursor = (void *)((u8 *)track->mSeqBuff + args[0]);
    return 0;
}
s32 JASSeqParser::cmdJmpF(JASTrack *track, u32 *args) {
	if(conditionCheck(track, (BranchCondition)args[0])) {
		track->mSeqCursor = (void *)((u8 *)track->mSeqBuff + args[1]);
	}
	return 0;
}

inline void mult3(u32 &a) {
	a = a * 2 + a;
}

s32 JASSeqParser::cmdJmpTable(JASTrack *track, u32 *args) {
    u32 idx = args[0];
    mult3(idx);
    track->mSeqCursor = (void *)((u8 *)track->mSeqBuff + track->calcSeekAmt(args[1] + idx));
    return 0;
}

s32 JASSeqParser::cmdCallTable(JASTrack *track, u32 *args) {
    u32 idx = args[0];
    mult3(idx);
    track->call(track->calcSeekAmt(args[1] + idx));
    return 0;
}

s32 JASSeqParser::cmdLoopS(JASTrack *track, u32 *args) {
    track->loopStart(args[0]);
    return 0;
}
s32 JASSeqParser::cmdLoopE(JASTrack *track, u32 *args) {
    track->loopEnd();
    return 0;
}
s32 JASSeqParser::cmdNote(JASTrack *track, u32 *args) {
	execNoteOnGate(track, args[1], args[2], args[3], args[0]);
    return 0;
}
s32 JASSeqParser::cmdNoteOn(JASTrack *track, u32 *args) {
    execNoteOnMidi(track, args[1], args[0], args[2]);
    return 0;
}
s32 JASSeqParser::cmdNoteOff(JASTrack *track, u32 *args) {
    execNoteOff(track, args[0]);
    return 0;
}
s32 JASSeqParser::cmdReadPort(JASTrack *track, u32 *args) {
	u16 res = track->readPortSelf(args[0]);
    writeReg(track, args[1], res);
    return 0;
}
s32 JASSeqParser::cmdWritePort(JASTrack *track, u32 *args) {
    track->writePortSelf(args[0], args[1]);
    return 0;
}
s32 JASSeqParser::cmdParentWritePort(JASTrack *track, u32 *args) {
    if(track->mParent) track->mParent->writePort(args[0], args[1]);
    return 0;
}
s32 JASSeqParser::cmdChildWritePort(JASTrack *track, u32 *args) {
	u8 offset = args[0] >> 4 & 0xf;
	u8 port = args[0];
	JASTrack *child = track->mChildren[offset];
	if(child) child->writePort(port & 0xf, args[1]);
    return 0;
}
s32 JASSeqParser::cmdParentReadPort(JASTrack *track, u32 *args) {
    if(track->mParent) {
		u16 res = track->mParent->readPort(args[0]);
		writeReg(track, args[1], res);
	}
    return 0;
}
s32 JASSeqParser::cmdChildReadPort(JASTrack *track, u32 *args) {
	u8 offset = args[0] >> 4 & 0xf;
	u8 port = args[0];
	JASTrack *child = track->mChildren[offset];
    if(child) {
		u16 res = child->readPort(port & 0xf);
		writeReg(track, args[1], res);
	}
    return 0;
}
s32 JASSeqParser::cmdCheckPortImport(JASTrack *track, u32 *args) {
    track->mRegs.write(JASRegisterParam::REG_F, track->mPorts.checkImport(args[0]));
    return 0;
}
s32 JASSeqParser::cmdCheckPortExport(JASTrack *track, u32 *args) {
    track->mRegs.write(JASRegisterParam::REG_F, track->mPorts.checkExport(args[0]));
    return 0;
}
s32 JASSeqParser::cmdWait(JASTrack *track, u32 *args) {
    track->mTimer = track->readMidiValue();
    return 0;
}
s32 JASSeqParser::cmdWaitByte(JASTrack *track, u32 *args) {
    track->mTimer = args[0];
    return 0;
}
s32 JASSeqParser::cmdSetLastNote(JASTrack *track, u32 *args) {
    if(args[0] < 0x100) track->setLatestKey(args[0]);
    return 0;
}
s32 JASSeqParser::cmdEnvScaleSet(JASTrack *track, u32 *args) {
    track->setOscScale(args[0], args[1] / 16383f);
    return 0;
}
s32 JASSeqParser::cmdEnvSet(JASTrack *track, u32 *args) {
    track->setOscTable(args[0], (const JASOscillator::Point *)((u8 *)track->mSeqBuff + args[1]));
    return 0;
}
s32 JASSeqParser::cmdSimpleADSR(JASTrack *track, u32 *args) {
    track->setOscAdsr(args[0], args[1], args[2], args[3], args[4]);
    return 0;
}
s32 JASSeqParser::cmdBusConnect(JASTrack *track, u32 *args) {
    track->connectBus(args[0], args[1]);
    return 0;
}
s32 JASSeqParser::cmdSetIntTable(JASTrack *track, u32 *args) {
    track->_48 = args[0];
    return 0;
}
s32 JASSeqParser::cmdSetInterrupt(JASTrack *track, u32 *args) {
    track->setIntrMask(args[0]);
    return 0;
}
s32 JASSeqParser::cmdDisInterrupt(JASTrack *track, u32 *args) {
	track->clrIntrMask(args[0]);
	return 0;
}
s32 JASSeqParser::cmdClrI(JASTrack *track, u32 *args) {
	track->mCursorSwap = nullptr;
    track->checkIntr();
    return 0;
}
s32 JASSeqParser::cmdRetI(JASTrack *track, u32 *args) {
    track->retIntr();
    track->checkIntr();
    return 0;
}
s32 JASSeqParser::cmdIntTimer(JASTrack *track, u32 *args) {
	u32 arg0 = args[0], arg1 = args[1];
    track->mIntTimer = arg0;
    track->_54 = arg1;
    track->_58 = arg1; 
    return 0;
}
s32 JASSeqParser::cmdSyncCPU(JASTrack *track, u32 *args) {
	u16 value = -1;
	if(sCallbackFunc) value = sCallbackFunc(track, args[0]);
    track->mRegs.write(JASRegisterParam::REG_F, value);
    return 0;
}
s32 JASSeqParser::cmdTempo(JASTrack *track, u32 *args) {
    track->setTempo(args[0]);
    return 0;
}
s32 JASSeqParser::cmdFinish(JASTrack *track, u32 *args) {
    return -1;
}
s32 JASSeqParser::cmdNop(JASTrack *track, u32 *args) {
    return 0;
}
s32 JASSeqParser::cmdFIRSet(JASTrack *track, u32 *args) {
    track->setFIR((const s16 *)((u8 *)track->mSeqBuff + args[0]));
    return 0;
}
s32 JASSeqParser::cmdIIRSet(JASTrack *track, u32 *args) {
	s16 iir[4];
	for(u32 i = 0; i < 4; i++) {
		iir[i] = args[i];
	}
	track->setIIR(iir);
    return 0;
}
s32 JASSeqParser::cmdIIRCutOff(JASTrack *track, u32 *args) {
	if(args[0] < 0x80) track->setIIR(sIIRCutoff + args[0] * 4);
    return 0;
}
s32 JASSeqParser::cmdBankPrg(JASTrack *track, u32 *args) {
	u32 arg0 = args[0];
    track->mBank = (u8)(arg0 >> 8);
    track->mPrg = (u8)arg0;
    return 0;
}
s32 JASSeqParser::cmdBank(JASTrack *track, u32 *args) {
    track->mBank = (u8)args[0];
    return 0;
}
s32 JASSeqParser::cmdPrg(JASTrack *track, u32 *args) {
    track->mPrg = (u8)args[0];
    return 0;
}
s32 JASSeqParser::cmdParamI(JASTrack *track, u32 *args) {
    track->setParam(args[0], (s16)args[1] / 32767f, 0);
    return 0;
}
s32 JASSeqParser::cmdParamII(JASTrack *track, u32 *args) {
    track->setParam(args[0], (s16)args[1] / 32767f, args[2]);
    return 0;
}
s32 JASSeqParser::cmdParamE(JASTrack *track, u32 *args) {
	s16 arg1 = args[1] << 8;
	if(!(args[1] & 0x80)) arg1 |= (s16)(args[1] << 1);
	track->setParam(args[0], arg1 / 32767f, 0);
	return 0;
}
s32 JASSeqParser::cmdParamEI(JASTrack *track, u32 *args) {
	s16 arg1 = args[1] << 8;
	if(!(args[1] & 0x80)) arg1 |= (s16)(args[1] << 1);
	track->setParam(args[0], arg1 / 32767f, args[2]);
	return 0;
}
s32 JASSeqParser::cmdReg(JASTrack *track, u32 *args) {
	u32 arg1 = args[1], arg2 = args[2];
	switch(args[0]) {
		case 0:
			break;
		case 1:
			arg2 += readReg(track, arg1);
			break;
		case 2:
			arg2 = readReg(track, arg1) - arg2;
			break;
		case 3:
			arg2 = readReg(track, arg1) - arg2;
			arg1 = 3;
			break;
		case 4:
			arg2 *= readReg(track, arg1);
			arg1 = 0x21;
			break;
		case 5:
			arg2 &= readReg(track, arg1);
			break;
		case 6:
			arg2 |= readReg(track, arg1);
			break;
		case 7:
			arg2 ^= readReg(track, arg1);
			break;
		case 8:
			static JMath::TRandom_fast_ oRandom(0);
			arg2 = (oRandom.rand() >> 9) % arg2;
			break;
		case 9:
			arg2 = readReg(track, arg1) << arg2;
			break;
		case 10:
			arg2 = readReg(track, arg1) >> arg2;
			break;
		default:
			return 0;
	}
	writeReg(track, arg1, arg2);
    return 0;
}
s32 JASSeqParser::cmdRegLoad(JASTrack *track, u32 *args) {
    writeReg(track, args[0], args[1]);
    return 0;
}
s32 JASSeqParser::cmdRegUni(JASTrack *track, u32 *args) {
	u32 arg1 = args[1], reg;
	switch(args[0]) {
		case 0xb:
			reg = -readReg(track, arg1);
			break;
		default:
			return 0;
	}
	writeReg(track, arg1, reg);
    return 0;
}
s32 JASSeqParser::cmdRegTblLoad(JASTrack *track, u32 *args) {
	u32 arg1 = args[1], arg3 = args[2], arg2 = args[3];
    switch(args[0]) {
		case 0xc:
			arg3 = *((u8 *)track->mSeqBuff + arg3 + arg2);
			break;
		case 0xd:
			arg3 = *((u16 *)((u8 *)track->mSeqBuff + arg3) + arg2);
			break;
		case 0xe:
			mult3(arg2);
			arg3 = track->calcSeekAmt(arg3 + arg2);
			break;
		case 0xf:
			arg3 = *((u32 *)((u8 *)track->mSeqBuff + arg3) + arg2);
			break;
		case 0x10:
			arg3 = *(u32 *)((u8 *)track->mSeqBuff + arg2 + arg3);
			break;
		default:
			return 0;
	}
	writeReg(track, arg1, arg3);
    return 0;
}

s32 JASSeqParser::cmdDump(JASTrack *track, u32 *args) {
	JASReport("--------------- JASTrack (%8x) dump ----------------", track);

	JASReport (
		" Base: 0x%08x Cur: 0x%08x(0x%06x)",
		track->mSeqBuff,
		track->mSeqCursor,
		(u8 *)track->mSeqCursor - (u8 *)track->mSeqBuff
	);

	JASReport("");


	u32 ra = track->mRegs.read(JASRegisterParam::REG_A),
		rb = track->mRegs.read(JASRegisterParam::REG_B),
		rs = track->mRegs.read(JASRegisterParam::REG_S);
	JASReport(" REG_A: 0x%04x REG_B: 0x%04x REG_S: 0x%04x", ra, rb, rs);

	u32 rx = track->mRegs.read(JASRegisterParam::REG_X),
		ry = track->mRegs.read(JASRegisterParam::REG_Y),
		rf = track->mRegs.read(JASRegisterParam::REG_F);
	JASReport(" REG_X: 0x%04x REG_Y: 0x%04x REG_F: 0x%04x", rx, ry, rf);
	JASReport("");


	JASReport(
		" PORT 0-3: 0x%04x 0x%04x 0x%04x 0x%04x",
		track->mPorts.mPorts[0],
		track->mPorts.mPorts[1],
		track->mPorts.mPorts[2],
		track->mPorts.mPorts[3]
	);
	JASReport(
		" PORT 4-7: 0x%04x 0x%04x 0x%04x 0x%04x",
		track->mPorts.mPorts[4],
		track->mPorts.mPorts[5],
		track->mPorts.mPorts[6],
		track->mPorts.mPorts[7]
	);
	JASReport(
		" PORT 8-B: 0x%04x 0x%04x 0x%04x 0x%04x",
		track->mPorts.mPorts[8],
		track->mPorts.mPorts[9],
		track->mPorts.mPorts[0xA],
		track->mPorts.mPorts[0xB]
	);
	JASReport(
		" PORT C-F: 0x%04x 0x%04x 0x%04x 0x%04x",
		track->mPorts.mPorts[0xC],
		track->mPorts.mPorts[0xD],
		track->mPorts.mPorts[0xE],
		track->mPorts.mPorts[0xF]
	);
	JASReport("");

    return 0;
}

s32 JASSeqParser::cmdPrintf(JASTrack *track, u32 *args) {
	char stack_1C[0x80];
	u32 stack_C[4];
	u8 stack_8[4];
	u32 r1f = 0, i;
	for(i = 0; i < 0x80; i++) {
		char currChar = *(((char *)track->mSeqCursor)++);
		stack_1C[i] = currChar;
		if(!currChar) break; 
		if(currChar == '\\') {
			char currChar = *(((char *)track->mSeqCursor)++);
			if(!(stack_1C[i] = currChar)) break;
			switch(currChar) {
				case 'n':
					stack_1C[i] = 0xd;
					continue;
				default:
					continue;
			}
		}
		else if(currChar == '%') {
			char currChar2 = *(((char *)track->mSeqCursor)++);
			if(!(stack_1C[++i] = currChar2)) break;
			switch(currChar2) {
				case 'd':
					stack_8[r1f] = 0;
					break;
				case 'x':
					stack_8[r1f] = 1;
					break;
				case 's':
					stack_8[r1f] = 2;
					break;
				case 'r':
					stack_8[r1f] = 3;
					stack_1C[i] = 'd';
					break;
				case 'R':
					stack_8[r1f] = 4;
					stack_1C[i] = 'x';
					break;
			}
		}
		else continue;
		r1f++;
	}
	for(i = 0; i < r1f; i++) {
		u8 curr = stack_8[i];
		u8 *tmp2 = (u8 *)track->mSeqCursor;
		((u8 *)track->mSeqCursor)++;
		stack_C[i] = *tmp2;
		switch(curr) {
			case 2:
				stack_C[i] = (u32)((u8 *)track->mSeqBuff + stack_C[i]);
				break;
			case 3:
			case 4:
				stack_C[i] = readReg(track, (u8)stack_C[i]);
				break;
		}
	}
	JASReport(stack_1C, stack_C[0], stack_C[1], stack_C[2], stack_C[3]);
    return 0;
}

void JASSeqParser::execNoteOnGate(JASTrack *track, u32 a, u32 b, u32 c, u32 d) {
	u32 arg = 0;
	if(d >> 6 & 1) arg |= 2;
	if(d >> 7 & 1) arg |= 1;
	if(!c) arg |= 4;
	track->gateOn(a, b, c, arg);
	if(c) track->mTimer = c;
	else track->_51 = true;
}

void JASSeqParser::execNoteOnMidi(JASTrack *track, u32 a, u32 b, u32 c) {
	track->noteOn(a, b, c);
}

void JASSeqParser::execNoteOff(JASTrack *track, u32 a) {
	track->noteOff(a, 0);
}

s32 JASSeqParser::execCommand(JASTrack *track, CommandFunc command, u32 a, u32 *args) {
	return (this->*command)(track, args);
}

s32 JASSeqParser::parseNoteOff(JASTrack *track, u8 a) {
	a &= 7;
	if(a) execNoteOff(track, a);
	return 0;
}

s32 JASSeqParser::parseNoteOn(JASTrack *track, u8 a) {
	u8 note = *(((u8 *)track->mSeqCursor)++);
	u8 lower = note & 7;
	u8 note2 = *(((u8 *)track->mSeqCursor)++);
	if(!lower) execNoteOnGate(track, a, note2, track->readMidiValue(), note);
	else execNoteOnMidi(track, lower, a, note2);
	return 0;
}

struct u24 {
	u8 _0[3];
};

s32 JASSeqParser::parseCommand(JASTrack *track, u8 cmdCode, u16 c) {
	Command *cmd;
	
	if(cmdCode != 0xB0) {
		cmd = sCmdInfo + (cmdCode - 0xA0);
	}
	else {
		cmd = sExtCmdInfo + *(((u8 *)track->mSeqCursor)++); 
	}
	
	u32 b = cmd->argWidths | c;
	u32 args[8];
	
	for(int i = 0; i < cmd->numArgs; i++) {
		u32 arg = 0;
		switch(b & 3) {
			case 0:
				arg = *(((u8 *)track->mSeqCursor)++);
				break;
			case 1:
				arg = *(((u16 *)track->mSeqCursor)++);
				break;
			case 2:
				// Access a 24-bit field
				arg = *((u32 *)((u8 *)(((u24 *)track->mSeqCursor)++) - 1)) & 0xffffff;
				break;
			case 3:
				arg = readReg(track, *(((u8 *)track->mSeqCursor)++));
				break;
		}
		args[i] = arg;
		b = (u16)b >> 2;
	}
	CommandFunc func = cmd->func;
	if(func == nullptr) return 0;
	return execCommand(track, func, cmd->numArgs, args); 
}

s32 JASSeqParser::parseRegCommand(JASTrack *track, int a) {
	u8 data = *(((u8 *)track->mSeqCursor)++);
	u16 width = 0;
	u16 b = 3;
	for(int i = 0; i < a; i++) {
		if(data & 0x80) width |= b;
		data <<= 1;
		b <<= 2;
	}
	return parseCommand(track, *(((u8 *)track->mSeqCursor)++), width);
}

s32 JASSeqParser::parse(JASTrack *track) {
	u8 data = *(((u8 *)track->mSeqCursor)++);
	if(!(data & 0x80)) return parseNoteOn(track, data);
	switch(data & 0xf0) {
		case 0x80:
			return parseNoteOff(track, data & 0xf);
		case 0x90:
			return parseRegCommand(track, (data & 7) + 1);
		default:
			return parseCommand(track, data, 0);
	}
}
