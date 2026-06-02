#include "JSystem/JAudio2/JASSeqParser.hpp"
#include "JSystem/JAudio2/JASCalc.hpp"
#include "JSystem/JAudio2/JASReport.hpp"
#include "JSystem/JAudio2/JASSeqCtrl.hpp"
#include "JSystem/JAudio2/JASTrack.hpp"
#include "JSystem/JMath/random.hpp"
#include "JSystem/JSupport/JSupport.hpp"

extern const s16 sIIRCutoff[128][4];

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
    u16 res = track->mRegisterParam.read(JASRegisterParam::REG_F);
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
    track->writeReg(JASRegisterParam::REG_F, value);
    if (reg < 0x40)
        track->writeReg((JASRegisterParam::RegID)reg, value);
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
            track->setPort(reg - 0x40, value);
            break;
        case 0x22:
            track->setTimebase(value);
            break;
        case 0x23:
            track->setTranspose(value);
            break;
        case 0x24:
            track->setBendSense(value);
            break;
        case 0x25:
            track->setGateRate(value);
            break;
        case 0x26:
            track->setSkipSample(value);
            break;
        case 0x27:
            track->setBankNumber((u8)value);
            break;
        case 0x28:
            track->setProgNumber((u8)value);
            break;
        case 0x29:
            track->setPanPower(value / 32767.0f);
            break;
        case 0x2A:
            track->mNoteOnPrio = JSULoByte(value);
            track->mReleasePrio = JSUHiByte(value);
            break;
        case 0x2B:
            track->setNoteOnPrio(value);
            break;
        case 0x2C:
            track->setReleasePrio(value);
            break;
        case 0x2D:
            track->setDirectRelease(value);
            break;
        case 0x2E:
            track->setVibDepth(value / 1524.0f);
            break;
        case 0x2F:
            track->setVibDepth(value / 12192.0f);
            break;
        case 0x30:
            track->setTremDepth(value / 256.0f);
            break;
        case 0x31:
            track->setVibPitch(value / 64.0f);
            break;
        case 0x32:
            track->setTremPitch(value / 64.0f);
            break;
        case 0x33:
            track->setVibDelay(value);
            break;
        case 0x34:
            track->setTremDelay(value);
            break;
        }
}

u32 JASSeqParser::readReg(JASTrack* track, u32 reg) const {
    if (reg < 0x40)
        return track->mRegisterParam.read((JASRegisterParam::RegID)reg);
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
            ret = track->getPort(reg - 0x40);
            break;
        // Create a bitmap of the current track's children
        case 0x20: {
            u16 currBit = 1;
            for (u32 i = 0; i < 0x10; i++, currBit <<= 1) {
                JASTrack* child = track->getChild(i);
                if (child != nullptr && child->mStatus == 1) {
                    ret |= currBit;
                }
            }
        } break;
        case 0x21:
            if (!track->mSeqCtrl.mReader.mNumStacks) {
                return 0;
            }
            ret = track->mTrackPort.mPorts[track->mSeqCtrl.mReader.mNumStacks - 0x20 + 5];
            break;
        case 0x22:
            ret = track->getTimebase();
            break;
        case 0x23:
            ret = track->getTranspose();
            break;
        case 0x24:
            ret = track->getBendSense();
            break;
        case 0x25:
            ret = track->getGateRate();
            break;
        case 0x26:
            ret = track->getSkipSample();
            break;
        case 0x27:
            ret = track->getBankNumber();
            break;
        case 0x28:
            ret = track->getProgNumber();
            break;
        case 0x29:
            ret = track->getPanPower() * 32767.0f;
            break;
        case 0x2A:
            ret = track->mReleasePrio | track->mNoteOnPrio;
            break;
        case 0x2B:
            ret = track->getNoteOnPrio();
            break;
        case 0x2C:
            ret = track->getReleasePrio();
            break;
        case 0x2D:
            ret = track->getDirectRelease();
            break;
        case 0x2E:
            ret = track->getVibDepth() * 1524.0f;
            break;
        case 0x2F:
            ret = track->getVibDepth() * 12192.0f;
            break;
        case 0x30:
            ret = track->getTremDepth() * 256.0f;
            break;
        case 0x31:
            ret = track->getVibPitch() * 64.0f;
            break;
        case 0x32:
            ret = track->getTremPitch() * 64.0f;
            break;
        case 0x33:
            ret = track->getVibDelay();
            break;
        case 0x34:
            ret = track->getTremDelay();
            break;
        }
        return ret;
    }
}

s32 JASSeqParser::cmdOpenTrack(JASTrack* track, u32* args) {
    u32 arg0 = args[0], arg1 = args[1];
    JASTrack* child = track->openChild(arg0);
    if (child == nullptr) {
        return 0;
    }
    child->setSeqData(track->getSeqCtrl()->getBase(), arg1);
    child->start();
    return 0;
}

s32 JASSeqParser::cmdCloseTrack(JASTrack* track, u32* args) {
    track->closeChild(args[0]);
    return 0;
}

s32 JASSeqParser::cmdCall(JASTrack* track, u32* args) {
    track->getSeqCtrl()->call(args[0]);
    return 0;
}

s32 JASSeqParser::cmdCallF(JASTrack* track, u32* args) {
    if (conditionCheck(track, (BranchCondition)args[0])) {
        track->getSeqCtrl()->call(args[1]);
    }
    return 0;
}

s32 JASSeqParser::cmdRet(JASTrack* track, u32* args) {
    track->getSeqCtrl()->ret();
    return 0;
}
s32 JASSeqParser::cmdRetF(JASTrack* track, u32* args) {
    if (conditionCheck(track, (BranchCondition)args[0])) {
        track->getSeqCtrl()->ret();
        return 0;
    } else
        return 0;
}
s32 JASSeqParser::cmdJmp(JASTrack* track, u32* args) {
    track->getSeqCtrl()->jump(args[0]);
    return 0;
}
s32 JASSeqParser::cmdJmpF(JASTrack* track, u32* args) {
    if (conditionCheck(track, (BranchCondition)args[0])) {
        track->getSeqCtrl()->jump(args[1]);
    }
    return 0;
}

s32 JASSeqParser::cmdJmpTable(JASTrack* track, u32* args) {
    JASSeqCtrl* seqCtrl = track->getSeqCtrl();
    u32 idx = args[0];
    idx += idx * 2;
    seqCtrl->jump(seqCtrl->get24(args[1] + idx));
    return 0;
}

s32 JASSeqParser::cmdCallTable(JASTrack* track, u32* args) {
    u32 idx = args[0];
    idx += idx * 2;
    track->getSeqCtrl()->call(track->getSeqCtrl()->get24(args[1] + idx));
    return 0;
}

s32 JASSeqParser::cmdLoopS(JASTrack* track, u32* args) {
    track->getSeqCtrl()->loopStart(args[0]);
    return 0;
}

s32 JASSeqParser::cmdLoopE(JASTrack* track, u32* args) {
    track->getSeqCtrl()->loopEnd();
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
    u16 val = track->readPortSelf(args[0]);
    writeReg(track, args[1], val);
    return 0;
}

s32 JASSeqParser::cmdWritePort(JASTrack* track, u32* args) {
    track->writePortSelf(args[0], args[1]);
    return 0;
}

s32 JASSeqParser::cmdParentWritePort(JASTrack* track, u32* args) {
    JASTrack* parent = track->getParent();
    if (parent != nullptr) {
        parent->writePort(args[0], args[1]);
    }
    return 0;
}

s32 JASSeqParser::cmdChildWritePort(JASTrack* track, u32* args) {
    u8 trackNum = args[0];
    u8 port = args[0];
    JASTrack* child = track->getChild(JSUHiNibble(trackNum));
    if (child != nullptr) {
        child->writePort(JSULoNibble(port), args[1]);
    }
    return 0;
}

s32 JASSeqParser::cmdParentReadPort(JASTrack* track, u32* args) {
    JASTrack* parent = track->getParent();
    if (parent != nullptr) {
        u16 val = parent->readPort(args[0]);
        writeReg(track, args[1], val);
    }
    return 0;
}

s32 JASSeqParser::cmdChildReadPort(JASTrack* track, u32* args) {
    u8 trackNum = args[0];
    u8 port = args[0];
    JASTrack* child = track->getChild(JSUHiNibble(trackNum));
    if (child != nullptr) {
        u16 val = child->readPort(JSULoNibble(port));
        writeReg(track, args[1], val);
    }
    return 0;
}

s32 JASSeqParser::cmdCheckPortImport(JASTrack* track, u32* args) {
    track->writeReg(JASRegisterParam::REG_F, track->checkPortIn(args[0]));
    return 0;
}

s32 JASSeqParser::cmdCheckPortExport(JASTrack* track, u32* args) {
    track->writeReg(JASRegisterParam::REG_F, track->checkPort(args[0]));
    return 0;
}

s32 JASSeqParser::cmdWait(JASTrack* track, u32* args) {
    track->getSeqCtrl()->wait(track->getSeqCtrl()->readMidiValue());
    return 0;
}

s32 JASSeqParser::cmdWaitByte(JASTrack* track, u32* args) {
    track->getSeqCtrl()->wait(args[0]);
    return 0;
}

s32 JASSeqParser::cmdSetLastNote(JASTrack* track, u32* args) {
    if (args[0] < 0x100) {
        track->setLatestKey(args[0]);
    }
    return 0;
}

s32 JASSeqParser::cmdEnvScaleSet(JASTrack* track, u32* args) {
    track->setOscScale(args[0], args[1] / 16383.0f);
    return 0;
}

s32 JASSeqParser::cmdEnvSet(JASTrack* track, u32* args) {
    track->setOscTable(args[0], (const JASOscillator::Point*)((u8*)track->getSeqCtrl()->getAddr(args[1])));
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
    track->getSeqCtrl()->setIntrTable(args[0]);
    return 0;
}

s32 JASSeqParser::cmdSetInterrupt(JASTrack* track, u32* args) {
    track->getSeqCtrl()->setIntrMask(args[0]);
    return 0;
}

s32 JASSeqParser::cmdDisInterrupt(JASTrack* track, u32* args) {
    track->getSeqCtrl()->clrIntrMask(args[0]);
    return 0;
}

s32 JASSeqParser::cmdClrI(JASTrack* track, u32* args) {
    track->getSeqCtrl()->clrIntr();
    track->getSeqCtrl()->checkIntr();
    return 0;
}

s32 JASSeqParser::cmdRetI(JASTrack* track, u32* args) {
    track->getSeqCtrl()->retIntr();
    track->getSeqCtrl()->checkIntr();
    return 0;
}

s32 JASSeqParser::cmdIntTimer(JASTrack* track, u32* args) {
    // NOTE: likely `track->getSeqCtrl()->setIntrTimer(args[0], args[1]);`
    // but refuses to inline
    u32 arg0 = args[0], arg1 = args[1];
    JASSeqCtrl* seqCtrl = track->getSeqCtrl();
    seqCtrl->mIntTimer = arg0;
    seqCtrl->_54 = arg1;
    seqCtrl->_58 = arg1;
    return 0;
}

u16 (*JASSeqParser::sCallbackFunc)(JASTrack*, u16);

s32 JASSeqParser::cmdSyncCPU(JASTrack* track, u32* args) {
    u16 value = -1;
    if (sCallbackFunc != nullptr) {
        value = sCallbackFunc(track, args[0]);
    }
    track->writeReg(JASRegisterParam::REG_F, value);
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
    track->setFIR((s16*)track->getSeqCtrl()->getAddr(args[0]));
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
    if (args[0] < 0x80) {
        track->setIIR(sIIRCutoff[args[0]]);
    }
    return 0;
}

s32 JASSeqParser::cmdBankPrg(JASTrack* track, u32* args) {
    u16 arg0 = JSULoHalf(args[0]);
    track->setBankNumber(JSUHiByte(arg0));
    track->setProgNumber(JSULoByte(arg0));
    return 0;
}

s32 JASSeqParser::cmdBank(JASTrack* track, u32* args) {
    track->setBankNumber(JSULoByte(args[0]));
    return 0;
}

s32 JASSeqParser::cmdPrg(JASTrack* track, u32* args) {
    track->setProgNumber(JSULoByte(args[0]));
    return 0;
}

s32 JASSeqParser::cmdParamI(JASTrack* track, u32* args) {
    track->setParam(args[0], (s16)args[1] / 32767.0f, 0);
    return 0;
}

s32 JASSeqParser::cmdParamII(JASTrack* track, u32* args) {
    track->setParam(args[0], (s16)args[1] / 32767.0f, args[2]);
    return 0;
}

s32 JASSeqParser::cmdParamE(JASTrack* track, u32* args) {
    s16 arg1 = args[1] << 8;
    if ((args[1] & 0x80) == 0) {
        arg1 |= (s16)(args[1] << 1);
    }
    track->setParam(args[0], arg1 / 32767.0f, 0);
    return 0;
}

s32 JASSeqParser::cmdParamEI(JASTrack* track, u32* args) {
    s16 arg1 = args[1] << 8;
    if ((args[1] & 0x80) == 0) {
        arg1 |= (s16)(args[1] << 1);
    }
    track->setParam(args[0], arg1 / 32767.0f, args[2]);
    return 0;
}

s32 JASSeqParser::cmdReg(JASTrack* track, u32* args) {
    u32 reg = args[1], val = args[2];
    switch (args[0]) {
    case 0:
        break;
    case 1:
        val += readReg(track, reg);
        break;
    case 2:
        val = readReg(track, reg) - val;
        break;
    case 3:
        val = readReg(track, reg) - val;
        reg = 3;  // TODO: special register number?
        break;
    case 4:
        val *= readReg(track, reg);
        reg = 0x21;  // TODO: special register number?
        break;
    case 5:
        val &= readReg(track, reg);
        break;
    case 6:
        val |= readReg(track, reg);
        break;
    case 7:
        val ^= readReg(track, reg);
        break;
    case 8:
        static JMath::TRandom_fast_ oRandom(0);
        val = (oRandom.rand() >> 9) % val;
        break;
    case 9:
        val = readReg(track, reg) << val;
        break;
    case 10:
        val = readReg(track, reg) >> val;
        break;
    default:
        return 0;
    }
    writeReg(track, reg, val);
    return 0;
}

s32 JASSeqParser::cmdRegLoad(JASTrack* track, u32* args) {
    writeReg(track, args[0], args[1]);
    return 0;
}

s32 JASSeqParser::cmdRegUni(JASTrack* track, u32* args) {
    u32 reg = args[1], val;
    switch (args[0]) {
    case 0xb:
        val = -readReg(track, reg);
        break;
    default:
        return 0;
    }
    writeReg(track, reg, val);
    return 0;
}

s32 JASSeqParser::cmdRegTblLoad(JASTrack* track, u32* args) {
    u32 reg = args[1], arg2 = args[2], arg3 = args[3];
    switch (args[0]) {
    case 0xc:
        arg2 = track->getSeqCtrl()->getByte(arg2 + arg3);
        break;
    case 0xd:
        arg2 = track->getSeqCtrl()->get16(arg2 + arg3 * 2);
        break;
    case 0xe:
        arg3 += arg3 * 2;
        arg2 = track->getSeqCtrl()->get24(arg2 + arg3);
        break;
    case 0xf:
        arg2 = track->getSeqCtrl()->get32(arg2 + arg3 * 4);
        break;
    case 0x10:
        arg2 = track->getSeqCtrl()->get32(arg2 + arg3);
        break;
    default:
        return 0;
    }
    writeReg(track, reg, arg2);
    return 0;
}

s32 JASSeqParser::cmdDump(JASTrack* track, u32* args) {
    JASSeqCtrl* seqCtrl = track->getSeqCtrl();
    JASReport("--------------- JASTrack (%8x) dump ----------------", track);
    JASReport(" Base: 0x%08x Cur: 0x%08x(0x%06x)", seqCtrl->getBase(), seqCtrl->getCur(), (u8*)seqCtrl->getCur() - (u8*)seqCtrl->getBase());
    JASReport("");
    JASReport(" REG_A: 0x%04x REG_B: 0x%04x REG_S: 0x%04x", readReg(track, JASRegisterParam::REG_A), readReg(track, JASRegisterParam::REG_B),
              readReg(track, JASRegisterParam::REG_S));
    JASReport(" REG_X: 0x%04x REG_Y: 0x%04x REG_F: 0x%04x", readReg(track, JASRegisterParam::REG_X), readReg(track, JASRegisterParam::REG_Y),
              readReg(track, JASRegisterParam::REG_F));
    JASReport("");
    JASReport(" PORT 0-3: 0x%04x 0x%04x 0x%04x 0x%04x", track->getPort(0x0), track->getPort(0x1), track->getPort(0x2), track->getPort(0x3));
    JASReport(" PORT 4-7: 0x%04x 0x%04x 0x%04x 0x%04x", track->getPort(0x4), track->getPort(0x5), track->getPort(0x6), track->getPort(0x7));
    JASReport(" PORT 8-B: 0x%04x 0x%04x 0x%04x 0x%04x", track->getPort(0x8), track->getPort(0x9), track->getPort(0xA), track->getPort(0xB));
    JASReport(" PORT C-F: 0x%04x 0x%04x 0x%04x 0x%04x", track->getPort(0xC), track->getPort(0xD), track->getPort(0xE), track->getPort(0xF));
    JASReport("");

    return 0;
}

s32 JASSeqParser::cmdPrintf(JASTrack* track, u32* args) {
    char formatString[0x80];
    u32 values[4];
    u8 formatTypes[4];
    u32 numValues = 0, i;
    for (i = 0; i < sizeof(formatString); i++) {
        formatString[i] = track->getSeqCtrl()->readByte();

        if (formatString[i] == 0) {
            break;
        }
        if (formatString[i] == '\\') {
            formatString[i] = track->getSeqCtrl()->readByte();
            if (formatString[i] == 0) {
                break;
            }

            switch (formatString[i]) {
            case 'n':
                formatString[i] = '\r';
                break;
            }
        } else if (formatString[i] == '%') {
            i++;
            formatString[i] = track->getSeqCtrl()->readByte();

            if (formatString[i] == 0) {
                break;
            }

            switch (formatString[i]) {
            case 'd':
                formatTypes[numValues] = 0;
                break;
            case 'x':
                formatTypes[numValues] = 1;
                break;
            case 's':
                formatTypes[numValues] = 2;
                break;
            case 'r':
                formatTypes[numValues] = 3;
                formatString[i] = 'd';
                break;
            case 'R':
                formatTypes[numValues] = 4;
                formatString[i] = 'x';
                break;
            }
            numValues++;
        }
    }

    for (i = 0; i < numValues; i++) {
        values[i] = track->getSeqCtrl()->readByte();
        switch (formatTypes[i]) {
        case 2:
            values[i] = (u32)track->getSeqCtrl()->getAddr(values[i]);
            break;
        case 3:
        case 4:
            values[i] = readReg(track, JSULoByte(values[i]));
            break;
        }
    }

    JASReport(formatString, values[0], values[1], values[2], values[3]);
    return 0;
}

void JASSeqParser::execNoteOnGate(JASTrack* track, u32 pitch, u32 velocity, u32 time, u32 ctrlFlags) {
    u32 flags = 0;

    if (ctrlFlags >> 6 & 1) {
        flags |= 2;  // mIsMute
    }

    if (ctrlFlags >> 7 & 1) {
        flags |= 1;  // mPauseFlag
    }

    if (time == 0) {
        flags |= 4;  // mIsDirectlyPlayed
    }

    track->gateOn(pitch, velocity, time, flags);

    if (time != 0) {
        track->getSeqCtrl()->wait(time);
    } else {
        track->getSeqCtrl()->waitNoteFinish();
    }
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

s32 JASSeqParser::parseNoteOff(JASTrack* track, u8 noteId) {
    noteId &= 7;
    if (noteId != 0) {
        execNoteOff(track, noteId);
    }
    return 0;
}

s32 JASSeqParser::parseNoteOn(JASTrack* track, u8 pitch) {
    u8 ctrlFlags = track->getSeqCtrl()->readByte();
    u8 noteId = ctrlFlags & 7;
    u8 velocity = track->getSeqCtrl()->readByte();
    if (noteId == 0) {
        execNoteOnGate(track, pitch, velocity, track->getSeqCtrl()->readMidiValue(), ctrlFlags);
    } else {
        execNoteOnMidi(track, noteId, pitch, velocity);
    }
    return 0;
}

s32 JASSeqParser::parseCommand(JASTrack* track, u8 cmdCode, u16 c) {
    Command* cmd;

    if (cmdCode != 0xB0) {
        cmd = &sCmdInfo[cmdCode - 0xA0];
    } else {
        cmd = &sExtCmdInfo[track->getSeqCtrl()->readByte()];
    }

    u32 readType = cmd->argWidths | c;
    u32 args[8];

    for (int i = 0; i < cmd->numArgs; i++) {
        u32 arg = 0;
        switch (readType & 3) {
        case 0:
            arg = track->getSeqCtrl()->readByte();
            break;
        case 1:
            arg = track->getSeqCtrl()->read16();
            break;
        case 2:
            // FIXME: this should REALLY be
            // `arg = track->getSeqCtrl()->read24();`
            // but the compiler refuses to inline it
            track->getSeqCtrl()->mReader.mSeqCursor--;
#ifdef __MWERKS__
            arg = *((u32*)(track->getSeqCtrl()->mReader.mSeqCursor))++ & 0xffffff;
#else
            arg = *(u32*)track->getSeqCtrl()->mReader.mSeqCursor;
            track->getSeqCtrl()->mReader.mSeqCursor += 4;
#endif
            break;
        case 3:
            arg = readReg(track, track->getSeqCtrl()->readByte());
            break;
        }
        args[i] = arg;
        readType = (u16)readType >> 2;
    }

    CommandFunc func = cmd->func;
    if (func == nullptr) {
        return 0;
    }
    return execCommand(track, func, cmd->numArgs, args);
}

s32 JASSeqParser::parseRegCommand(JASTrack* track, int numArgs) {
    u8 data = track->getSeqCtrl()->readByte();
    u16 width = 0;
    u16 b = 3;
    for (int i = 0; i < numArgs; i++) {
        if ((data & 0x80) != 0) {
            width |= b;
        }
        data <<= 1;
        b <<= 2;
    }
    return parseCommand(track, track->getSeqCtrl()->readByte(), width);
}

s32 JASSeqParser::parse(JASTrack* track) {
    u8 data = track->getSeqCtrl()->readByte();

    if ((data & 0x80) == 0) {
        return parseNoteOn(track, data);
    }

    switch (data & 0xf0) {
    case 0x80:
        return parseNoteOff(track, data & 0xf);
    case 0x90:
        return parseRegCommand(track, (data & 7) + 1);
    default:
        return parseCommand(track, data, 0);
    }
}
