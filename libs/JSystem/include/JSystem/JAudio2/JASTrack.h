#pragma once

#include <revolution/types.h>

#include "JSystem/JAudio2/JASSeqRegisterParam.h"
#include "JSystem/JAudio2/JASSeqCtrl.h"

class JASTrack : public JASSeqCtrl {
public:
	bool checkNoteStop(u32) const;
	void setTimeBase(u16);
	JASTrack* openChild(u32);
	void closeChild(u32);
	void setSeqData(void *, u32);
	void start();
	void call(u32);
	
	u32 _5C;
	u16 _60[0x10];
	JASRegisterParam _80;
	u8 _84[0xAC];
	JASTrack *mChildren[0x10];
	u8 _170[0x70];
	f32 _1E0;
	f32 _1E4;
	f32 _1E8;
	f32 _1EC;
	u16 _1F0;
	u16 _1F2;
	u8 _1F4[0x24];
	f32 _218;
	u32 _21C;
	u32 _220;
	u16 _224;
	u16 _226;
	u16 mTimerBase;
	s8 _22A;
	u16 _22C;
	u16 _22E;
	u8 _230;
	u8 _231;
	u8 _232;
	u8 _233;
	u8 _234[0xC];
	s32 _240;
};