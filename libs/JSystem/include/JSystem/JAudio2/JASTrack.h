#pragma once

#include <revolution/types.h>

#include "JSystem/JAudio2/JASSeqRegisterParam.h"
#include "JSystem/JAudio2/JASSeqCtrl.h"
#include "JSystem/JAudio2/JASTrackPort.h"
#include "JSystem/JAudio2/JASOscillator.h"
#include "JSystem/JAudio2/JASDspInterface.h"
#include "JSystem/JAudio2/JASBank.h"
#include "JSystem/JGadget/linklist.h"
#include "JSystem/JAudio2/JASHeapCtrl.h"

class JASSoundParams;
class JASChannel;

class JASChannelParams {
public:
	JASChannelParams();
	void init();
	f32 _0[6];
};

class JASTrack : public JASSeqCtrl, public JASPoolAllocObject_MultiThreaded<JASTrack> {
public:

	static const JASOscillator::Point sEnvOsc[4];
	static const JASOscillator::Point sPitchEnvOsc[4];

	static JASBank sDefaultBankTable[0100];

	union LLFlags {
		struct {
			bool _0: 1;
			bool _1: 1;
			bool _2: 1;
			bool _3: 1;
			bool _4: 1;
			bool _5: 1;
			bool _6: 1;
		};
		u8 byteRepr;
	};

	struct TChannelMgr : public JASPoolAllocObject_MultiThreaded<TChannelMgr> {
		TChannelMgr(JASTrack *);
		void init();

		u32 _0[8];
		JASChannelParams _20;
		u16 _38[8];
		u32 _48;
		JASTrack *_4C;
	};

	JASTrack();
	
	~JASTrack();
	
	void seqMain();

	void init();
	void initTimed();
	
	void startSeq();
	void stopSeq();
	
	void start();

	void close();

	void tickProc();

	void inherit(const JASTrack &);

	JASTrack* openChild(u32);
	void connectChild(u32, JASTrack *);
	void closeChild(u32);
	JASTrack* getRootTrack();
	
	void connectBus(int, int);

	void assignExtBuffer(u32, JASSoundParams *);
	
	void call(u32);

	void updateTimedParam();
	void updateTrack(f32);
	void updateTempo();
	void updateSeq(bool, f32);
	void updateChannel(JASChannel *, JASDsp::TChannel *);
	void channelUpdateCalback(u32, JASChannel *, JASDsp::TChannel *, void *);

	void gateOn(u32, u32, f32, u32);
	void noteOn(u32, u32, u32);
	void noteOff(u32, u16);
	void noteOffAll(u16);
	bool checkNoteStop(u32) const;

	void mute(bool);
	void pause(bool);
	
	void setChannelMgrCount(u32);
	void setChannelPauseFlag(bool);
	void setTimebase(u16);
	void setSeqData(void *, u32);
	void setLatestKey(u8);
	void setTempo(u16);
	void setTempoRate(f32);
	void setParam(u32, f32, u32);

	f32 getTransposeTotal() const;
	bool isMute() const;

	void overwriteOsc(JASChannel *);
	void setOscScale(u32, f32);
	void setOscTable(u32, const JASOscillator::Point *);
	void setOscAdsr(s16, s16, s16, s16, u16);

	void setFIR(const u16 *);
	void setIIR(const u16 *);
	void seqTimeToDSPTime(f32);
	
	u16 readPortSelf(u32);
	u16 readPort(u32);
	void writePortSelf(u32, u16);
	void writePort(u32, u16);
	
	JASTrackPort mPorts; // _5C
	JASRegisterParam _80;
	u8 _84[0x60];
	JASOscillator::Point _E4[4];
	JASOscillator::Point _FC[4];
	JASOscillator::Point _114[4];
	JASTrack *mParent; // _12C
	JASTrack *mChildren[0x10]; // _130
	TChannelMgr* _170[4];
	TChannelMgr _180;
	u32 _1D0;
	JASBank* _1D4;
	f32 _1D8;
	f32 _1DC;
	f32 _1E0;
	f32 _1E4;
	f32 _1E8;
	f32 _1EC;
	u16 _1F0;
	u16 _1F2;
	u16 _1F4[8];
	u16 _204[8];
	u8 _214;
	f32 _218;
	f32 _21C;
	u32 _220;
	u16 _224;
	u16 _226;
	u16 mTimerBase; // _228
	s8 _22A;
	u8 _22B;
	u16 _22C;
	u16 _22E;
	u8 _230;
	u8 _231;
	u8 _232;
	u8 _233;
	u16 _234;
	u16 _236;
	u16 _238;
	u16 _23A;
	u16 _23C;
	u16 _23E;
	volatile s32 _240;
	volatile LLFlags _244;
	JGadget::TLinkListNode _248;
};