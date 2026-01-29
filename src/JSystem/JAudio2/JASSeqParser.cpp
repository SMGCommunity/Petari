#include "JSystem/JAudio2/JASSeqParser.hpp"
#include "JSystem/JAudio2/JASCalc.hpp"
#include "JSystem/JAudio2/JASReport.hpp"
#include "JSystem/JAudio2/JASSeqReader.hpp"
#include "JSystem/JAudio2/JASTrack.hpp"
#include "JSystem/JMath/random.hpp"

JASSeqParser::Command JASSeqParser::sCmdInfo[0x60] = {{nullptr},
                                                      {nullptr},
                                                      {nullptr},
                                                      {nullptr},
                                                      {nullptr},
                                                      {nullptr},
                                                      {nullptr},
                                                      {nullptr},
                                                      {nullptr},
                                                      {nullptr},
                                                      {nullptr},
                                                      {nullptr},
                                                      {nullptr},
                                                      {nullptr},
                                                      {nullptr},
                                                      {nullptr},
                                                      {nullptr},
                                                      {&JASSeqParser::cmdNoteOn, 3},
                                                      {&JASSeqParser::cmdNoteOff, 1},
                                                      {&JASSeqParser::cmdNote, 4, 0x40},
                                                      {&JASSeqParser::cmdSetLastNote, 1},
                                                      {nullptr},
                                                      {nullptr},
                                                      {nullptr},
                                                      {&JASSeqParser::cmdParamE, 2},
                                                      {&JASSeqParser::cmdParamI, 2, 4},
                                                      {&JASSeqParser::cmdParamEI, 3, 0x10},
                                                      {&JASSeqParser::cmdParamII, 3, 0x14},
                                                      {nullptr},
                                                      {nullptr},
                                                      {nullptr},
                                                      {nullptr},
                                                      {nullptr},
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
                                                      {nullptr},
                                                      {nullptr},
                                                      {nullptr},
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
                                                      {nullptr},
                                                      {nullptr},
                                                      {nullptr},
                                                      {&JASSeqParser::cmdTempo, 1, 1},
                                                      {&JASSeqParser::cmdBankPrg, 1, 1},
                                                      {&JASSeqParser::cmdBank, 1},
                                                      {&JASSeqParser::cmdPrg, 1},
                                                      {nullptr},
                                                      {nullptr},
                                                      {nullptr},
                                                      {&JASSeqParser::cmdEnvScaleSet, 2, 4},
                                                      {&JASSeqParser::cmdEnvSet, 2, 8},
                                                      {&JASSeqParser::cmdSimpleADSR, 5, 0x155},
                                                      {&JASSeqParser::cmdBusConnect, 2, 4},
                                                      {&JASSeqParser::cmdIIRCutOff, 1},
                                                      {&JASSeqParser::cmdIIRSet, 4, 0x55},
                                                      {&JASSeqParser::cmdFIRSet, 1, 2},
                                                      {nullptr},
                                                      {nullptr},
                                                      {&JASSeqParser::cmdWait, 0},
                                                      {&JASSeqParser::cmdWaitByte, 1},
                                                      {nullptr},
                                                      {&JASSeqParser::cmdSetIntTable, 1, 2},
                                                      {&JASSeqParser::cmdSetInterrupt, 1, 1},
                                                      {&JASSeqParser::cmdDisInterrupt, 1, 1},
                                                      {&JASSeqParser::cmdRetI, 0},
                                                      {&JASSeqParser::cmdClrI, 0},
                                                      {&JASSeqParser::cmdIntTimer, 2, 4},
                                                      {&JASSeqParser::cmdSyncCPU, 1, 1},
                                                      {nullptr},
                                                      {nullptr},
                                                      {nullptr},
                                                      {&JASSeqParser::cmdPrintf, 0},
                                                      {&JASSeqParser::cmdNop, 0},
                                                      {&JASSeqParser::cmdFinish, 0}};

JASSeqParser::Command JASSeqParser::sExtCmdInfo[0xff] = {{nullptr}, {&JASSeqParser::cmdDump}, {nullptr}, {nullptr}, {nullptr}, {nullptr}, {nullptr}};

bool JASSeqParser::conditionCheck(JASTrack* track, BranchCondition cond) {
    u16 res = track->mRegs.read(JASRegisterParam::REG_F);
    switch (cond) {
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

void JASSeqParser::writeReg(JASTrack* track, u32 reg, u32 value) {
    track->mRegs.write(JASRegisterParam::REG_F, value);
    if (reg < 0x40)
        track->mRegs.write((JASRegisterParam::RegID)reg, value);
    else
        switch (reg - 0x40) {
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
            track->_1E8 *= 0.00390625f;
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

u32 JASSeqParser::readReg(JASTrack* track, u32 reg) const {
    if (reg < 0x40)
        return track->mRegs.read((JASRegisterParam::RegID)reg);
    else {
        u32 ret = 0;
        switch (reg - 0x40) {
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
            JASTrack* child;
            for (u32 i = 0; i < 0x10; i++) {
                if (child = track->mChildren[i]) {
                    if (child->_240 == 1)
                        ret |= currBit;
                }
                currBit <<= 1;
            }
            break;
        case 0x21:
            if (!track->mNumStacks)
                return 0;
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

s32 JASSeqParser::cmdOpenTrack(JASTrack* track, u32* args) {
    u32 arg0 = args[0], arg1 = args[1];
    JASTrack* child = track->openChild(arg0);
    if (!child)
        return 0;
    child->setSeqData(track->mSeqBuff, arg1);
    child->start();
    return 0;
}

s32 JASSeqParser::cmdCloseTrack(JASTrack* track, u32* args) {
    track->closeChild(args[0]);
    return 0;
}

s32 JASSeqParser::cmdCall(JASTrack* track, u32* args) {
    track->call(args[0]);
    return 0;
}

s32 JASSeqParser::cmdCallF(JASTrack* track, u32* args) {
    if (conditionCheck(track, (BranchCondition)args[0]))
        track->call(args[1]);
    return 0;
}

s32 JASSeqParser::cmdRet(JASTrack* track, u32* args) {
    track->ret();
    return 0;
}

s32 JASSeqParser::cmdRetF(JASTrack* track, u32* args) {
    if (conditionCheck(track, (BranchCondition)args[0])) {
        track->ret();
        return 0;
    } else
        return 0;
}

s32 JASSeqParser::cmdJmp(JASTrack* track, u32* args) {
    track->mSeqCursor = (void*)((u8*)track->mSeqBuff + args[0]);
    return 0;
}

s32 JASSeqParser::cmdJmpF(JASTrack* track, u32* args) {
    if (conditionCheck(track, (BranchCondition)args[0])) {
        track->mSeqCursor = (void*)((u8*)track->mSeqBuff + args[1]);
    }
    return 0;
}

inline void mult3(u32& a) {
    a = a * 2 + a;
}

s32 JASSeqParser::cmdJmpTable(JASTrack* track, u32* args) {
    u32 idx = args[0];
    mult3(idx);
    track->mSeqCursor = (void*)((u8*)track->mSeqBuff + track->calcSeekAmt(args[1] + idx));
    return 0;
}

s32 JASSeqParser::cmdCallTable(JASTrack* track, u32* args) {
    u32 idx = args[0];
    mult3(idx);
    track->call(track->calcSeekAmt(args[1] + idx));
    return 0;
}

s32 JASSeqParser::cmdLoopS(JASTrack* track, u32* args) {
    track->loopStart(args[0]);
    return 0;
}

s32 JASSeqParser::cmdLoopE(JASTrack* track, u32* args) {
    track->loopEnd();
    return 0;
}

s32 JASSeqParser::cmdNote(JASTrack* track, u32* args) {
    execNoteOnGate(track, args[1], args[2], args[3], args[0]);
    return 0;
}

s32 JASSeqParser::cmdNoteOn(JASTrack* track, u32* args) {
    execNoteOnMidi(track, args[1], args[0], args[2]);
    return 0;
}

s32 JASSeqParser::cmdNoteOff(JASTrack* track, u32* args) {
    execNoteOff(track, args[0]);
    return 0;
}

s32 JASSeqParser::cmdReadPort(JASTrack* track, u32* args) {
    u16 res = track->readPortSelf(args[0]);
    writeReg(track, args[1], res);
    return 0;
}

s32 JASSeqParser::cmdWritePort(JASTrack* track, u32* args) {
    track->writePortSelf(args[0], args[1]);
    return 0;
}

s32 JASSeqParser::cmdParentWritePort(JASTrack* track, u32* args) {
    if (track->mParent)
        track->mParent->writePort(args[0], args[1]);
    return 0;
}

s32 JASSeqParser::cmdChildWritePort(JASTrack* track, u32* args) {
    u8 offset = args[0] >> 4 & 0xf;
    u8 port = args[0];
    JASTrack* child = track->mChildren[offset];
    if (child)
        child->writePort(port & 0xf, args[1]);
    return 0;
}

s32 JASSeqParser::cmdParentReadPort(JASTrack* track, u32* args) {
    if (track->mParent) {
        u16 res = track->mParent->readPort(args[0]);
        writeReg(track, args[1], res);
    }
    return 0;
}

s32 JASSeqParser::cmdChildReadPort(JASTrack* track, u32* args) {
    u8 offset = args[0] >> 4 & 0xf;
    u8 port = args[0];
    JASTrack* child = track->mChildren[offset];
    if (child) {
        u16 res = child->readPort(port & 0xf);
        writeReg(track, args[1], res);
    }
    return 0;
}

s32 JASSeqParser::cmdCheckPortImport(JASTrack* track, u32* args) {
    track->mRegs.write(JASRegisterParam::REG_F, track->mPorts.checkImport(args[0]));
    return 0;
}

s32 JASSeqParser::cmdCheckPortExport(JASTrack* track, u32* args) {
    track->mRegs.write(JASRegisterParam::REG_F, track->mPorts.checkExport(args[0]));
    return 0;
}

s32 JASSeqParser::cmdWait(JASTrack* track, u32* args) {
    track->mTimer = track->readMidiValue();
    return 0;
}

s32 JASSeqParser::cmdWaitByte(JASTrack* track, u32* args) {
    track->mTimer = args[0];
    return 0;
}

s32 JASSeqParser::cmdSetLastNote(JASTrack* track, u32* args) {
    if (args[0] < 0x100)
        track->setLatestKey(args[0]);
    return 0;
}

s32 JASSeqParser::cmdEnvScaleSet(JASTrack* track, u32* args) {
    track->setOscScale(args[0], args[1] / 16383f);
    return 0;
}

s32 JASSeqParser::cmdEnvSet(JASTrack* track, u32* args) {
    track->setOscTable(args[0], (const JASOscillator::Point*)((u8*)track->mSeqBuff + args[1]));
    return 0;
}

s32 JASSeqParser::cmdSimpleADSR(JASTrack* track, u32* args) {
    track->setOscAdsr(args[0], args[1], args[2], args[3], args[4]);
    return 0;
}

s32 JASSeqParser::cmdBusConnect(JASTrack* track, u32* args) {
    track->connectBus(args[0], args[1]);
    return 0;
}

s32 JASSeqParser::cmdSetIntTable(JASTrack* track, u32* args) {
    track->_48 = args[0];
    return 0;
}

s32 JASSeqParser::cmdSetInterrupt(JASTrack* track, u32* args) {
    track->setIntrMask(args[0]);
    return 0;
}

s32 JASSeqParser::cmdDisInterrupt(JASTrack* track, u32* args) {
    track->clrIntrMask(args[0]);
    return 0;
}

s32 JASSeqParser::cmdClrI(JASTrack* track, u32* args) {
    track->mCursorSwap = nullptr;
    track->checkIntr();
    return 0;
}

s32 JASSeqParser::cmdRetI(JASTrack* track, u32* args) {
    track->retIntr();
    track->checkIntr();
    return 0;
}

s32 JASSeqParser::cmdIntTimer(JASTrack* track, u32* args) {
    u32 arg0 = args[0], arg1 = args[1];
    track->mIntTimer = arg0;
    track->_54 = arg1;
    track->_58 = arg1;
    return 0;
}

u16 (*JASSeqParser::sCallbackFunc)(JASTrack*, u16);

s32 JASSeqParser::cmdSyncCPU(JASTrack* track, u32* args) {
    u16 value = -1;
    if (sCallbackFunc)
        value = sCallbackFunc(track, args[0]);
    track->mRegs.write(JASRegisterParam::REG_F, value);
    return 0;
}

s32 JASSeqParser::cmdTempo(JASTrack* track, u32* args) {
    track->setTempo(args[0]);
    return 0;
}

s32 JASSeqParser::cmdFinish(JASTrack* track, u32* args) {
    return -1;
}

s32 JASSeqParser::cmdNop(JASTrack* track, u32* args) {
    return 0;
}

s32 JASSeqParser::cmdFIRSet(JASTrack* track, u32* args) {
    track->setFIR((const s16*)((u8*)track->mSeqBuff + args[0]));
    return 0;
}

s32 JASSeqParser::cmdIIRSet(JASTrack* track, u32* args) {
    s16 iir[4];
    for (u32 i = 0; i < 4; i++) {
        iir[i] = args[i];
    }
    track->setIIR(iir);
    return 0;
}

s32 JASSeqParser::cmdIIRCutOff(JASTrack* track, u32* args) {
    if (args[0] < 0x80)
        track->setIIR(JASCalc::CUTOFF_TO_IIR_TABLE[args[0]]);
    return 0;
}

s32 JASSeqParser::cmdBankPrg(JASTrack* track, u32* args) {
    u32 arg0 = args[0];
    track->mBank = (u8)(arg0 >> 8);
    track->mPrg = (u8)arg0;
    return 0;
}

s32 JASSeqParser::cmdBank(JASTrack* track, u32* args) {
    track->mBank = (u8)args[0];
    return 0;
}

s32 JASSeqParser::cmdPrg(JASTrack* track, u32* args) {
    track->mPrg = (u8)args[0];
    return 0;
}

s32 JASSeqParser::cmdParamI(JASTrack* track, u32* args) {
    track->setParam(args[0], (s16)args[1] / 32767f, 0);
    return 0;
}

s32 JASSeqParser::cmdParamII(JASTrack* track, u32* args) {
    track->setParam(args[0], (s16)args[1] / 32767f, args[2]);
    return 0;
}

s32 JASSeqParser::cmdParamE(JASTrack* track, u32* args) {
    s16 arg1 = args[1] << 8;
    if (!(args[1] & 0x80))
        arg1 |= (s16)(args[1] << 1);
    track->setParam(args[0], arg1 / 32767f, 0);
    return 0;
}

s32 JASSeqParser::cmdParamEI(JASTrack* track, u32* args) {
    s16 arg1 = args[1] << 8;
    if (!(args[1] & 0x80))
        arg1 |= (s16)(args[1] << 1);
    track->setParam(args[0], arg1 / 32767f, args[2]);
    return 0;
}

s32 JASSeqParser::cmdReg(JASTrack* track, u32* args) {
    u32 arg1 = args[1], arg2 = args[2];
    switch (args[0]) {
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

s32 JASSeqParser::cmdRegLoad(JASTrack* track, u32* args) {
    writeReg(track, args[0], args[1]);
    return 0;
}

s32 JASSeqParser::cmdRegUni(JASTrack* track, u32* args) {
    u32 arg1 = args[1], reg;
    switch (args[0]) {
    case 0xb:
        reg = -readReg(track, arg1);
        break;
    default:
        return 0;
    }
    writeReg(track, arg1, reg);
    return 0;
}

s32 JASSeqParser::cmdRegTblLoad(JASTrack* track, u32* args) {
    u32 arg1 = args[1], arg3 = args[2], arg2 = args[3];
    switch (args[0]) {
    case 0xc:
        arg3 = *((u8*)track->mSeqBuff + arg3 + arg2);
        break;
    case 0xd:
        arg3 = *((u16*)((u8*)track->mSeqBuff + arg3) + arg2);
        break;
    case 0xe:
        mult3(arg2);
        arg3 = track->calcSeekAmt(arg3 + arg2);
        break;
    case 0xf:
        arg3 = *((u32*)((u8*)track->mSeqBuff + arg3) + arg2);
        break;
    case 0x10:
        arg3 = *(u32*)((u8*)track->mSeqBuff + arg2 + arg3);
        break;
    default:
        return 0;
    }
    writeReg(track, arg1, arg3);
    return 0;
}

s32 JASSeqParser::cmdDump(JASTrack* track, u32* args) {
    JASReport("--------------- JASTrack (%8x) dump ----------------", track);

    JASReport(" Base: 0x%08x Cur: 0x%08x(0x%06x)", track->mSeqBuff, track->mSeqCursor, (u8*)track->mSeqCursor - (u8*)track->mSeqBuff);

    JASReport("");

    u32 ra = track->mRegs.read(JASRegisterParam::REG_A), rb = track->mRegs.read(JASRegisterParam::REG_B),
        rs = track->mRegs.read(JASRegisterParam::REG_S);
    JASReport(" REG_A: 0x%04x REG_B: 0x%04x REG_S: 0x%04x", ra, rb, rs);

    u32 rx = track->mRegs.read(JASRegisterParam::REG_X), ry = track->mRegs.read(JASRegisterParam::REG_Y),
        rf = track->mRegs.read(JASRegisterParam::REG_F);
    JASReport(" REG_X: 0x%04x REG_Y: 0x%04x REG_F: 0x%04x", rx, ry, rf);
    JASReport("");

    JASReport(" PORT 0-3: 0x%04x 0x%04x 0x%04x 0x%04x", track->mPorts.mPorts[0], track->mPorts.mPorts[1], track->mPorts.mPorts[2],
              track->mPorts.mPorts[3]);
    JASReport(" PORT 4-7: 0x%04x 0x%04x 0x%04x 0x%04x", track->mPorts.mPorts[4], track->mPorts.mPorts[5], track->mPorts.mPorts[6],
              track->mPorts.mPorts[7]);
    JASReport(" PORT 8-B: 0x%04x 0x%04x 0x%04x 0x%04x", track->mPorts.mPorts[8], track->mPorts.mPorts[9], track->mPorts.mPorts[0xA],
              track->mPorts.mPorts[0xB]);
    JASReport(" PORT C-F: 0x%04x 0x%04x 0x%04x 0x%04x", track->mPorts.mPorts[0xC], track->mPorts.mPorts[0xD], track->mPorts.mPorts[0xE],
              track->mPorts.mPorts[0xF]);
    JASReport("");

    return 0;
}

s32 JASSeqParser::cmdPrintf(JASTrack* track, u32* args) {
    char stack_1C[0x80];
    u32 stack_C[4];
    u8 stack_8[4];
    u32 r1f = 0, i;
    for (i = 0; i < sizeof(stack_1C); i++) {
        char currChar = *(((char*)track->mSeqCursor)++);
        stack_1C[i] = currChar;
        if (!currChar)
            break;
        if (currChar == '\\') {
            char currChar = *(((char*)track->mSeqCursor)++);
            if (!(stack_1C[i] = currChar))
                break;
            switch (currChar) {
            case 'n':
                stack_1C[i] = 0xd;
                continue;
            default:
                continue;
            }
        } else if (currChar == '%') {
            char currChar2 = *(((char*)track->mSeqCursor)++);
            if (!(stack_1C[++i] = currChar2))
                break;
            switch (currChar2) {
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
        } else
            continue;
        r1f++;
    }
    for (i = 0; i < r1f; i++) {
        u8 curr = stack_8[i];
        u8* tmp2 = (u8*)track->mSeqCursor;
        ((u8*)track->mSeqCursor)++;
        stack_C[i] = *tmp2;
        switch (curr) {
        case 2:
            stack_C[i] = (u32)((u8*)track->mSeqBuff + stack_C[i]);
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

void JASSeqParser::execNoteOnGate(JASTrack* track, u32 a, u32 b, u32 c, u32 d) {
    u32 arg = 0;
    if (d >> 6 & 1)
        arg |= 2;
    if (d >> 7 & 1)
        arg |= 1;
    if (!c)
        arg |= 4;
    track->gateOn(a, b, c, arg);
    if (c)
        track->mTimer = c;
    else
        track->_51 = true;
}

void JASSeqParser::execNoteOnMidi(JASTrack* track, u32 a, u32 b, u32 c) {
    track->noteOn(a, b, c);
}

void JASSeqParser::execNoteOff(JASTrack* track, u32 a) {
    track->noteOff(a, 0);
}

s32 JASSeqParser::execCommand(JASTrack* track, CommandFunc command, u32 a, u32* args) {
    return (this->*command)(track, args);
}

s32 JASSeqParser::parseNoteOff(JASTrack* track, u8 a) {
    a &= 7;
    if (a)
        execNoteOff(track, a);
    return 0;
}

s32 JASSeqParser::parseNoteOn(JASTrack* track, u8 a) {
    u8 note = *(((u8*)track->mSeqCursor)++);
    u8 lower = note & 7;
    u8 note2 = *(((u8*)track->mSeqCursor)++);
    if (!lower)
        execNoteOnGate(track, a, note2, track->readMidiValue(), note);
    else
        execNoteOnMidi(track, lower, a, note2);
    return 0;
}

struct u24 {
    u8 _0[3];
};

s32 JASSeqParser::parseCommand(JASTrack* track, u8 cmdCode, u16 c) {
    Command* cmd;

    if (cmdCode != 0xB0) {
        cmd = sCmdInfo + (cmdCode - 0xA0);
    } else {
        cmd = sExtCmdInfo + *(((u8*)track->mSeqCursor)++);
    }

    u32 b = cmd->argWidths | c;
    u32 args[8];

    for (int i = 0; i < cmd->numArgs; i++) {
        u32 arg = 0;
        switch (b & 3) {
        case 0:
            arg = *(((u8*)track->mSeqCursor)++);
            break;
        case 1:
            arg = *(((u16*)track->mSeqCursor)++);
            break;
        case 2:
            // Access a 24-bit field
            arg = *((u32*)((u8*)(((u24*)track->mSeqCursor)++) - 1)) & 0xffffff;
            break;
        case 3:
            arg = readReg(track, *(((u8*)track->mSeqCursor)++));
            break;
        }
        args[i] = arg;
        b = (u16)b >> 2;
    }
    CommandFunc func = cmd->func;
    if (func == nullptr)
        return 0;
    return execCommand(track, func, cmd->numArgs, args);
}

s32 JASSeqParser::parseRegCommand(JASTrack* track, int a) {
    u8 data = *(((u8*)track->mSeqCursor)++);
    u16 width = 0;
    u16 b = 3;
    for (int i = 0; i < a; i++) {
        if (data & 0x80)
            width |= b;
        data <<= 1;
        b <<= 2;
    }
    return parseCommand(track, *(((u8*)track->mSeqCursor)++), width);
}

s32 JASSeqParser::parse(JASTrack* track) {
    u8 data = *(((u8*)track->mSeqCursor)++);
    if (!(data & 0x80))
        return parseNoteOn(track, data);
    switch (data & 0xf0) {
    case 0x80:
        return parseNoteOff(track, data & 0xf);
    case 0x90:
        return parseRegCommand(track, (data & 7) + 1);
    default:
        return parseCommand(track, data, 0);
    }
}
