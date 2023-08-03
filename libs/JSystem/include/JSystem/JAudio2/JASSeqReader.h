#pragma once

#include <revolution/types.h>

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

	u8* mSeqData;
	u8* _4;
	u32 mNumStacks;
	u8* mStackPtrs[8];
	u16 _2C[8];
};