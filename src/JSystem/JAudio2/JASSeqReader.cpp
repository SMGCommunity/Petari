#include "JSystem/JAudio2/JASSeqReader.hpp"

void JASSeqReader::init() {
	mSeqBuff = nullptr;
	mSeqCursor = nullptr;
	mNumStacks = 0;
	for(u32 i = 0; i < 8; i++) {
		mStackPtrs[i] = nullptr;
		mLoopTimers[i] = nullptr;
	}
}

void JASSeqReader::init(void *buf) {
	mSeqBuff = buf;
	mSeqCursor = buf;
	mNumStacks = 0;
	for(u32 i = 0; i < 8; i++) {
		mStackPtrs[i] = nullptr;
		mLoopTimers[i] = nullptr;
	}
}

bool JASSeqReader::call(u32 num) {
	if(mNumStacks >= 8) return false;
	else {
		mStackPtrs[mNumStacks++] = mSeqCursor;
		mSeqCursor = (void *)((u8 *)mSeqBuff + num);
		return true;
	}
}

bool JASSeqReader::loopStart(u32 loopTimer) {
	if(mNumStacks >= 8) return false;
	else {
		mStackPtrs[mNumStacks] = mSeqCursor;
		mLoopTimers[mNumStacks++] = loopTimer;
		return true;
	}
}

bool JASSeqReader::loopEnd() {
	if(mNumStacks == 0) return false;
	else {
		u16 loopTimer = mLoopTimers[mNumStacks - 1];
		if(loopTimer > 0) loopTimer--;
		if(loopTimer == 0) {
			mNumStacks--;
			return true;
		}
        mLoopTimers[mNumStacks - 1] = loopTimer;
		mSeqCursor = mStackPtrs[mNumStacks - 1];
		return true;
	}
}

bool JASSeqReader::ret() {
	if(mNumStacks == 0) return false;
	else {
		mSeqCursor = mStackPtrs[--mNumStacks];
		return true;
	}
}

u32 JASSeqReader::readMidiValue() {
	u8 byte = *(((u8 *)mSeqCursor)++);
	if(!(byte & 0x80)) return byte;
	u32 byte2 = byte & 0x7f;
	s32 i = 0;
	u8 byte4;
	while(true) {
		if(i > 2) return 0;
		byte2 = byte2 << 7;
		byte4 = *(((u8 *)mSeqCursor)++);
		byte2 |= byte4 & 0x7f;
		if(!(byte4 & 0x80)) break;
		i++;
	}
	return byte2;
}

void* JASSeqReader::getStackPtr(u32 idx) const {
	if(idx >= mNumStacks) return nullptr;
	else return mStackPtrs[idx];
}
