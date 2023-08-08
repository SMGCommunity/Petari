#pragma once

class JASRegisterParam {
public:
	enum RegID {
		REG_A,
		REG_B,
		REG_S,
		REG_F,
		REG_X,
		REG_Y
	};
	u32 read(RegID);
	void write(RegID, u32);
	u32 _0;
};
