#include "JSystem/JAudio2/JASSeqReader.h"

void JASSeqReader::init() {
	mSeqData = nullptr;
	_4 = nullptr;
	mNumStacks = 0;
	for(u32 i = 0; i < 8; i++) {
		mStackPtrs[i] = 0;
		_2C[i] = 0;
	}
}

void JASSeqReader::init(void *buf) {
	mSeqData = (u8*)buf;
	_4 = (u8*)buf;
	mNumStacks = 0;
	for(u32 i = 0; i < 8; i++) {
		mStackPtrs[i] = 0;
		_2C[i] = 0;
	}
}

bool JASSeqReader::call(u32 num) {
	if(mNumStacks >= 8) return false;
	else {
		mStackPtrs[mNumStacks++] = _4;
		_4 = mSeqData + num;
		return true;
	}
}

bool JASSeqReader::loopStart(u32 num) {
	if(mNumStacks >= 8) return false;
	else {
		mStackPtrs[mNumStacks] = _4;
		_2C[mNumStacks++] = num;
		return true;
	}
}

bool JASSeqReader::loopEnd() {
	if(mNumStacks == 0) return false;
	else {
		u16 end = _2C[mNumStacks - 1];
		if(end > 0) end--;
		if(end == 0) {
			mNumStacks--;
			return true;
		}
		_2C[mNumStacks - 1] = end;
		_4 = mStackPtrs[mNumStacks - 1];
		return true;
	}
}

bool JASSeqReader::ret() {
	if(mNumStacks == 0) return false;
	else {
		_4 = mStackPtrs[--mNumStacks];
		return true;
	}
}

u32 JASSeqReader::readMidiValue() {
	u8 byte = *_4++;
	if(!(byte & 0x80)) return byte;
	u32 byte2 = byte & 0x7f;
	s32 i = 0;
	u8 byte4;
	while(true) {
		if(i > 2) return 0;
		byte2 = byte2 << 7;
		byte4 = *_4++;
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
