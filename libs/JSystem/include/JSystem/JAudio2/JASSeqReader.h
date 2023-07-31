#pragma once

#include <revolution/types.h>

	struct int24 {
		unsigned num: 24;
	};

class JASSeqReader {
public:
	void init();
	void init(void *);
	bool call(u32);
	bool loopStart(u32);
	bool loopEnd();
	bool ret();
	u32 readMidiValue();
	void* getStackPtr(u32) const;

	void* _0;
	void* _4;
	u32 _8;
	void* _C[8];
	u16 _2C[8];
};