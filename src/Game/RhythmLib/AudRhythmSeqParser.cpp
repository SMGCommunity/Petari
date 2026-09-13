#include "Game/RhythmLib/AudRhythmSeqParser.hpp"
#include "Game/RhythmLib/AudChordInfo.hpp"
#include "Game/RhythmLib/AudMeTrackCallback.hpp"
#include <JSystem/JAudio2/JASCriticalSection.hpp>
#include <JSystem/JAudio2/JASSeqCtrl.hpp>
#include <JSystem/JAudio2/JASSeqParser.hpp>
#include <JSystem/JAudio2/JASTrack.hpp>
#include <revolution/os/OSTime.h>

AudRhythmSeqParser::AudRhythmSeqParser()
    : JASSeqParser(), mTempo(120.0f), mModifiedTempo(120.0f), mChordId(), mScaleId(), mTempoAdjuster(), mCallDepth(), _1A4(), mIsUsed(true),
      mIsActive() {
    for (u32 i = 0; i < MAX_CHANNELS; i++) {
        initNoteInfo(i);
        mNoteOnInfo[i].mChannel = -1;
        mNoteOnInfo[i]._4 = 0;
        mNoteOnInfo[i].mNoteOnFunc = nullptr;
        mNoteOnInfo[i].mNoteOffFunc = nullptr;
    }

    mRhythmType0 = 0x18;
    mRhythmType1 = 0x1E;
    mRhythmType2 = 0x1B;
    mRhythmType3 = 0x19;
    mRhythmType4 = 0x1A;
}

void AudRhythmSeqParser::noteOnProcess(u32 channel, u32 a2, u32 a3, u32 noteOff) {
    for (s32 i = 0; i < _1A4; i++) {
        if (mNoteOnInfo[i]._4 == a2) {
            mNoteOnInfo[i].mChannel = channel;
            NoteOnFunc func = mNoteOnInfo[i].mNoteOnFunc;
            if (func != nullptr) {
                (func)(a3, noteOff);
            }
            return;
        }
    }
}

void AudRhythmSeqParser::noteOffProcess(u32 channel) {
    for (s32 i = 0; i < _1A4; i++) {
        if (mNoteOnInfo[i].mChannel == channel) {
            NoteOffFunc func = mNoteOnInfo[i].mNoteOffFunc;
            if (func != nullptr) {
                (func)();
            }
            mNoteOnInfo[i].mChannel = -1;
            return;
        }
    }
}

bool AudRhythmSeqParser::execNoteOnMidi(JASTrack* pTrack, u32 channel, u32 pitch, u32 velocity) {
    if (!mIsActive) {
        return false;
    }

    u32 noteOff = getNextNoteOff(pTrack, channel);
    setNoteInfo(channel, pitch, velocity, noteOff);
    noteOnProcess(channel, pitch, velocity, noteOff);

    s32 rhythmType = getRhythmType(pitch);

    if (rhythmType >= 0) {
        if (rhythmType == RhythmType_0) {
            f32 rate = mTempoAdjuster.beatProc(mTempo);
            JASTrack* mainTrack = pTrack;
            while (mainTrack->mParent != nullptr) {
                mainTrack = mainTrack->mParent;
            }
            mainTrack->setTempo(mTempo * rate);
            mModifiedTempo = mainTrack->mTempo;
        }
    }

    if (mIsUsed == true) {
        if (rhythmType >= 0) {
            if (rhythmType == RhythmType_0) {
                s32 vel = velocity - 10;
                if (vel < 100) {
                    AudMeTrackCallback::callback(RhythmType_5, noteOff * vel);
                }
            }
        }

        AudMeTrackCallback::callback(rhythmType, noteOff);
    }

    return pTrack->noteOn(channel, pitch, velocity);
}

void AudRhythmSeqParser::execNoteOff(JASTrack* pTrack, u32 channel) {
    if (!mIsActive) {
        return;
    }
    initNoteInfo(channel);
    noteOffProcess(channel);
    pTrack->noteOff(channel, 0);
}

s32 AudRhythmSeqParser::parse(JASTrack* pTrack) {
    u8 data = pTrack->getSeqCtrl()->readByte();

    if ((data & 0x80) == 0) {
        return parseNoteOn(pTrack, data);
    }

    switch (data & 0xf0) {
    case 0x80:
        return parseNoteOff(pTrack, data & 0xf);
    case 0x90:
        return parseRegCommand(pTrack, (data & 7) + 1);
    default:
        break;
    }

    switch (data) {
    case 0xE0:  // cmdTempo override
        mTempo = getTempoData(pTrack);
        mModifiedTempo = mTempo;
        break;
    case 0xE2:  // cmdBank override
        mScaleId = getScaleNumberData(pTrack);
        registScaleNumToInfo();
        break;
    case 0xE3:  // cmdPrg override
        mChordId = getChordNumberData(pTrack);
        registChordNumToInfo();
        break;
    }

    return parseCommand(pTrack, data, 0);
}

bool AudRhythmSeqParser::checkNoteOn(u32 channel, u32& rPitch, u32& rVelocity, u32& rNoteOff, s64& rTime, bool setOff) {
    JASCriticalSection crit;

    if (!mNoteInfo[channel].mIsOn) {
        return false;
    }

    rPitch = mNoteInfo[channel].mPitch;
    rVelocity = mNoteInfo[channel].mVelocity;
    rNoteOff = mNoteInfo[channel].mNoteOff;
    rTime = mNoteInfo[channel].mTime;

    if (setOff) {
        mNoteInfo[channel].mIsOn = false;
    }

    return true;
}

bool AudRhythmSeqParser::checkNoteOnKey(u32 key, u32& rVelocity, u32& rNoteOff, s64& rTime, u32& rChannel) {
    u32 pitch, velocity, noteOff;
    s64 time;

    for (u32 i = 0; i < MAX_CHANNELS; i++) {
        if (checkNoteOn(i, pitch, velocity, noteOff, time, false) && key == pitch) {
            rVelocity = velocity;
            rNoteOff = noteOff;
            rTime = time;
            rChannel = i;
            mNoteInfo[rChannel].mIsOn = false;
            return true;
        }
    }

    return false;
}

f32 AudRhythmSeqParser::getTempo() const {
    JASCriticalSection crit;
    return mTempo;
}

f32 AudRhythmSeqParser::getModifiedTempo() const {
    JASCriticalSection crit;
    return mModifiedTempo;
}

s32 AudRhythmSeqParser::getRhythmType(u32 key) {
    // TODO: clarify
    if (mRhythmType0 == key) {
        return RhythmType_0;
    }
    if (mRhythmType1 == key) {
        return RhythmType_1;
    }
    if (mRhythmType2 == key) {
        return RhythmType_2;
    }
    if (mRhythmType3 == key) {
        return RhythmType_3;
    }
    if (mRhythmType4 == key) {
        return RhythmType_4;
    }
    return -1;
}

u16 AudRhythmSeqParser::getTempoData(JASTrack* pTrack) {
    return pTrack->getSeqCtrl()->get16(pTrack->getSeqCtrl()->getOffset());
}

u8 AudRhythmSeqParser::getChordNumberData(JASTrack* pTrack) {
    return pTrack->getSeqCtrl()->getByte(pTrack->getSeqCtrl()->getOffset());
}

u8 AudRhythmSeqParser::getScaleNumberData(JASTrack* pTrack) {
    return pTrack->getSeqCtrl()->getByte(pTrack->getSeqCtrl()->getOffset());
}

s32 AudRhythmSeqParser::getNextNoteOff(JASTrack* pTrack, u32 channel) {
    // TODO: is this actually written with a goto?
    // Possible FAKEMATCH

    JASSeqCtrl* ctrl = pTrack->getSeqCtrl();
    u32 peekOff = ctrl->getOffset();
    mCallDepth = 0;
    u32 noteOff = 0;

    for (s32 i = 0; i < 1000; i++) {
        u8 data = ctrl->readByte();

        if ((data & 0x80) == 0) {
            u8 ctrlFlags = ctrl->readByte();
            data = ctrlFlags & 7;
            u8 velocity = ctrl->readByte();

            if (data == channel) {
                goto foundNote;
            }
        } else {
            switch (data & 0xf0) {
            case 0x80:
                if ((data & 7) == channel) {
                    goto foundNote;
                }
                break;
            case 0x90:
                passRegCommand(pTrack, (data & 7) + 1);
                break;
            default:
                noteOff += passCommand(pTrack, data, 0);
                break;
            }
        }
    }
    return -1;

foundNote:
    ctrl->jump(peekOff);
    return noteOff;
}

u32 AudRhythmSeqParser::passRegCommand(JASTrack* pTrack, int numArgs) {
    u8 data = pTrack->getSeqCtrl()->readByte();
    u16 width = 0;
    u16 b = 3;
    for (int i = 0; i < numArgs; i++) {
        if ((data & 0x80) != 0) {
            width |= b;
        }
        data <<= 1;
        b <<= 2;
    }
    return passCommand(pTrack, pTrack->getSeqCtrl()->readByte(), width);
}

u32 AudRhythmSeqParser::passCommand(JASTrack* pTrack, u8 cmdCode, u16 widths) {
    // TODO: this needs cleanup
    u32 ret = 0;

    Command* cmd;
    if (cmdCode != 0xB0) {
        cmd = &sCmdInfo[cmdCode - 0xA0];
    } else {
        cmd = &sExtCmdInfo[pTrack->getSeqCtrl()->readByte()];
    }

    u32 readType = cmd->argWidths | widths;
    u32 args[8];

    for (int i = 0; i < cmd->numArgs; i++) {
        u32 arg = 0;
        switch (readType & 3) {
        case 0:
            arg = pTrack->getSeqCtrl()->readByte();
            break;
        case 1:
            arg = pTrack->getSeqCtrl()->read16();
            break;
        case 2:
            arg = pTrack->getSeqCtrl()->read24();
            break;
        case 3:
            arg = readReg(pTrack, pTrack->getSeqCtrl()->readByte());
            break;
        }
        args[i] = arg;
        readType = (u16)readType >> 2;
    }

    if (cmdCode != 0xB0) {
        CommandFunc func = {nullptr};
        cmd = &sCmdInfo[cmdCode - 0xA0];

        switch (cmdCode) {
        case 0xC5: {  // cmdRet
            if (!retTmp(pTrack)) {
                pTrack->getSeqCtrl()->mReader.jump(pTrack->getSeqCtrl()->getSeqReader()->getStackPtr(pTrack->getSeqCtrl()->mReader.mNumStacks - 1));
            }
            break;
        }
        case 0xC3: {  // cmdCall
            callTmp(pTrack, args[0]);
            break;
        }
        case 0xC7: {  // cmdJump
            func = cmd->func;
            if (func == nullptr) {
                return 0;
            }
            execCommand(pTrack, func, cmd->numArgs, args);
            break;
        }
        case 0xF0:  // cmdWait
            ret = pTrack->getSeqCtrl()->mReader.readMidiValue();
            break;
        case 0xF1:  // cmdWaitByte
            ret = args[0];
            break;
        case 0xFD:  // cmdPrintf
            cmdPrintf(pTrack, nullptr);
            break;
        }
    }

    return ret;
}

bool AudRhythmSeqParser::callTmp(JASTrack* pTrack, u32 addr) {
    if (mCallDepth >= 8)
        return false;
    else {
        mCallStack[mCallDepth++] = (u8*)pTrack->getSeqCtrl()->getCur();
        pTrack->getSeqCtrl()->jump(addr);
        return true;
    }
}

bool AudRhythmSeqParser::retTmp(JASTrack* pTrack) {
    if (mCallDepth == 0)
        return false;
    else {
        pTrack->getSeqCtrl()->mReader.jump(mCallStack[--mCallDepth]);
        return true;
    }
}

void AudRhythmSeqParser::initNoteInfo(u32 channel) {
    mNoteInfo[channel].mIsOn = false;
    mNoteInfo[channel].mNoteOff = 0;
    mNoteInfo[channel].mPitch = 0;
    mNoteInfo[channel].mVelocity = 0;
    mNoteInfo[channel].mTime = 0;
}

void AudRhythmSeqParser::init() {
    JASCriticalSection crit;
    mIsActive = false;
    for (u32 i = 0; i < MAX_CHANNELS; i++) {
        initNoteInfo(i);
    }
    mCallDepth = 0;
    mTempo = 0.0f;
    mModifiedTempo = 0.0f;
}

void AudRhythmSeqParser::activate() {
    JASCriticalSection crit;
    mIsActive = true;
}

void AudRhythmSeqParser::setNoteInfo(u32 channel, u32 pitch, u32 velocity, u32 noteOff) {
    mNoteInfo[channel].mIsOn = true;
    mNoteInfo[channel].mNoteOff = noteOff;
    mNoteInfo[channel].mPitch = pitch;
    mNoteInfo[channel].mVelocity = velocity;
    mNoteInfo[channel].mTime = OSGetTime();
}

bool AudRhythmSeqParser::registChordNumToInfo() {
    if (!mIsUsed) {
        return false;
    }

    AudChordInfo* info = AudChordInfo::getInstance();
    if (info == nullptr) {
        return false;
    }

    if (!info->isAvailable()) {
        return false;
    }

    return info->setCurChord(mChordId);
}

bool AudRhythmSeqParser::registScaleNumToInfo() {
    if (!mIsUsed) {
        return false;
    }

    AudChordInfo* info = AudChordInfo::getInstance();
    if (info == nullptr) {
        return false;
    }

    if (!info->isAvailable()) {
        return false;
    }

    return info->setCurScale(mScaleId);
}

void AudRhythmSeqParser::flushChordScale() {
    registChordNumToInfo();
    registScaleNumToInfo();
}
