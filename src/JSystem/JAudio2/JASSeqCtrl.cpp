#include "JSystem/JAudio2/JASSeqCtrl.hpp"
#include "JSystem/JAudio2/JASTrack.hpp"

JASSeqParser JASSeqCtrl::sDefaultParser = JASSeqParser();

JASSeqCtrl::JASSeqCtrl() {
	JASSeqReader::init();
	mParser = &sDefaultParser;
	mTimer = 0;
	mCursorSwap = nullptr;
	_48 = 0;
	_4C = 0;
	_4E = 0;
	mIntTimer = 0;
	_51 = false;
	_54 = 0;
	_58 = 0;
	JASSeqReader::init();
}

void JASSeqCtrl::init() {
	JASSeqReader::init();
	mParser = &sDefaultParser;
	mTimer = 0;
	mCursorSwap = nullptr;
	_48 = 0;
	_4C = 0;
	_4E = 0;
	mIntTimer = 0;
	_54 = 0;
	_58 = 0;
	_51 = false;
}

void JASSeqCtrl::start(void *, u32 offset) {
	JASSeqReader::init();
	mSeqCursor = (void *)((u8 *)mSeqBuff + offset);
}

int JASSeqCtrl::tickProc(JASTrack *track) {
	if(!mSeqBuff) return 0;
	interrupt(JASSeqCtrl::INTR_6);
	timerProcess();
	if(_51) {
		if(!track->checkNoteStop(0)) return 0;
		_51 = false;
	}
	if(mTimer > 0) mTimer--;
	checkIntr();
	
	while((!mTimer || mCursorSwap) && !_51) {
		if(mParser->parse(track) < 0) return -1;
	}

	return 0;
}

void JASSeqCtrl::interrupt(JASSeqCtrl::IntrType interrupt) {
	u32 mask = 1 << interrupt;
	if(!(_4E & mask)) return;
	_4C |= mask;
}

void JASSeqCtrl::setIntrMask(u32 mask) {
	_4E |= mask;
}

void JASSeqCtrl::clrIntrMask(u32 mask) {
	_4E &= ~mask;
}

bool JASSeqCtrl::retIntr() {
	if(!mCursorSwap) return false;
	mSeqCursor = mCursorSwap;
	mCursorSwap = nullptr;
	return true;
}

s32 JASSeqCtrl::findIntr() {
	u32 intr = _4E & _4C;
	for(u32 i = 0; intr != 0; i++) {
		if(intr & 1) {
			_4C &= ~(1 << i);
			return i;
		}
		else intr >>= 1;
	}
	return -1;
}

void JASSeqCtrl::checkIntr() {
	s32 intr;
	if(!mCursorSwap && (intr = findIntr()) >= 0) {
		intr = intr * 3 + _48;
		u32 offset = calcSeekAmt(intr);
		mCursorSwap = mSeqCursor;
		mSeqCursor = (void *)((u8 *)mSeqBuff + offset);
	}
}

void JASSeqCtrl::timerProcess() {
	if(_54 != 0) {
		if(!--_54) {
			interrupt(INTR_5);
			if(mIntTimer) {
				if(--mIntTimer) {
					_54 = _58;
				}
			}
			else _54 = _58;
		}
	}
}

JASSeqParser::JASSeqParser() {}
