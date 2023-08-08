#pragma once

#include <revolution/types.h>

#include "JSystem/JAudio2/JASSeqRegisterParam.h"
#include "JSystem/JAudio2/JASSeqCtrl.h"
#include "JSystem/JAudio2/JASTrackPort.h"
#include "JSystem/JAudio2/JASOscillator.h"

class JASTrack : public JASSeqCtrl {
public:
	bool checkNoteStop(u32) const;
	void setTimeBase(u16);
	JASTrack* openChild(u32);
	void closeChild(u32);
	void setSeqData(void *, u32);
	void start();
	void call(u32);
	u16 readPortSelf(u32);
	u16 readPort(u32);
	void writePortSelf(u32, u16);
	void writePort(u32, u16);
	void setLatestKey(u8);
	void setOscScale(u32, f32);
	void setOscTable(u32, const JASOscillator::Point *);
	void setOscAdsr(s16, s16, s16, s16, u16);
	void connectBus(int, int);
	void setTempo(u16);
	void setFIR(const u16 *);
	void setIIR(const u16 *);
	void setParam(u32, f32, u32);
	void gateOn(u32, u32 , f32, u32);
	void noteOn(u32, u32, u32);
	void noteOff(u32, u16);
	
	JASTrackPort _5C;
	JASRegisterParam _80;
	u8 _84[0xA8];
	JASTrack *mParent;
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