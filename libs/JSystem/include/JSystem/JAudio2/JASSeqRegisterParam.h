#pragma once

class JASRegisterParam {
public:
	enum RegID {
		R0,
		R1,
		R2,
		R3,
		R4,
		R5
	};
	u32 read(RegID);
	void write(RegID, u32);
	u32 _0;
};
