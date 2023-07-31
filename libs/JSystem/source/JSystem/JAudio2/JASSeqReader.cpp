#include "JSystem/JAudio2/JASSeqReader.h"

void JASSeqReader::init() {
	_0 = nullptr;
	_4 = nullptr;
	_8 = 0;
	for(u32 i = 0; i < 8; i++) {
		_C[i] = 0;
		_2C[i] = 0;
	}
}

void JASSeqReader::init(void *buf) {
	_0 = buf;
	_4 = buf;
	_8 = 0;
	for(u32 i = 0; i < 8; i++) {
		_C[i] = 0;
		_2C[i] = 0;
	}
}

bool JASSeqReader::call(u32 num) {
	if(_8 >= 8) return false;
	else {
		_C[_8++] = _4;
		_4 = (char*)_0 + num;
		return true;
	}
}

bool JASSeqReader::loopStart(u32 num) {
	if(_8 >= 8) return false;
	else {
		_C[_8] = _4;
		_2C[_8++] = num;
		return true;
	}
}

bool JASSeqReader::loopEnd() {
	if(_8 == 0) return false;
	else {
		u16 end = _2C[_8 - 1];
		if(end > 0) end--;
		if(end == 0) {
			_8--;
			return true;
		}
		_2C[_8 - 1] = end;
		_4 = _C[_8 - 1];
		return true;
	}
}

bool JASSeqReader::ret() {
	if(_8 == 0) return false;
	else {
		_4 = _C[--_8];
		return true;
	}
}

u32 JASSeqReader::readMidiValue() {
	u8 byte = *((u8*)_4)++;
	if(!(byte & 0x80)) return byte;
	u32 byte2 = byte & 0x7f;
	s32 i = 0;
	u8 byte4;
	do {
		if(i > 2) return 0;
		byte2 = byte2 << 7;
		byte4 = *((u8*)_4)++;
		byte2 |= byte4 & 0x7f;
		if(!(byte4 & 0x80)) break;
		i++;
	} while(true);
	return byte2;
}

void* JASSeqReader::getStackPtr(u32 idx) const {
	if(idx >= _8) return nullptr;
	else return _C[idx];
}
