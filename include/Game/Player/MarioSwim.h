#pragma once

#include "Game/Player/MarioModule.h"

class MarioActor;

class MarioSwim : public MarioModule {
public:
    MarioSwim(MarioActor *);
	
	void checkWaterCube(bool);
    
    u8 _8[0x13c];
    u32 _144;
    u8 _148[0x18];
    TVec3f _160;
    TVec3f _16c;
};
