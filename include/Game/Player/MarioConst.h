#pragma once

class MarioActor;

struct Constants {
	u8 _0[0x426]; // dummy variable
	u16 _426;
	u8 _428[0x2a0];
	u16 _6c8;
};

class MarioConst {
public:
	MarioConst();

	const Constants* _0[2];
	unsigned long _8;
};
