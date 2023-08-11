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

struct Stupid {
	u16 _0;
	u16 get() const {return _0;}
	~Stupid();
};

class JASTrack : public JASSeqCtrl, public JASPoolAllocObject_MultiThreaded<JASTrack> {
public:

	static const JASOscillator::Point sEnvOsc[4];
	static const JASOscillator::Point sPitchEnvOsc[4];

	static JASBank sDefaultBankTable[0100];

	struct Bitfield {
		s32 _240;
		bool _244_0: 1;
		u8 _244_1: 1;
		bool _244_2: 1;
		bool _244_3: 1;
		bool _244_4: 1;
		bool _244_5: 1;
		u8 _244_6: 1;
		//unsigned char _244_7: 1;

		Bitfield();

		Bitfield(Bitfield base, u8 num) {
		//	base.set(num);
			*this = base;
		}

		const Bitfield& operator=(const u8 &num) {
			*this = Bitfield(*this, num);
			return *this;
		}

		/*void set(u8 stuff) {
			const Bitfield BF = *(const Bitfield *)&stuff;
			_244_0 = BF._244_0;
			_244_1 = BF._244_1;
			_244_2 = BF._244_2;
			_244_5 = BF._244_5;
			_244_3 = BF._244_3;
			_244_4 = BF._244_4;
			_244_6 = BF._244_6;
		}*/

	void biggerScaryFunction() {

	}

	void bigScaryFunction() {
		biggerScaryFunction();
	}

		void bs();
	};

	/*inline void mutate(u32 arg) {
		 _22E = arg;
    _230 = 0xC;
    _231 = 0x40;
    _232 = 0;
    _233 = 0x64;
    _234 = 0x150;
    _236 = 0x210;
    _238 = 0x352;
    _23A = 0x412;
    _23C = 0;
    _23E = 0;
    for(u32 i = 0; i < 8; i++) {
        _1F4[i] = 0;
    }
    _1F4[0] = 0x7fff;
    for(u32 i = 0; i < 8; i++) {
        _204[i] = 0;
    }
    _204[0] = 0x7fff;
    _214 = 0;
    
    /*trueBs();
    readValue();*/
    //JASTrack::Bitfield bf = {false, false, true, false, false, true, false};
	/*_255._244_0 = false;
    _255._244_1 = false;
    _255._244_2 = true;
    _255._244_5 = true;
    _255._244_3 = false;
    _255._244_4 = false;
    _255._244_6 = false;

    _240 = 0;
	}*/

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

	/*inline void trueBs() {
    //initBitfield();
    //_244 = (((_244 & 0xffffff3f) | 0x24) & 0xffffffe7);
    //_244 &= 0xfffffffd;
    //_240 = 0;
    for(u32 i = 0; i < 8; i++) {
        _1F4[i] = 0;
    }
    _1F4[0] = 0x7fff;
    for(u32 i = 0; i < 8; i++) {
        _204[i] = 0;
    }
    _204[0] = 0x7fff;
    _214 = 0;
    
	_255._244_0 = false;
    _255._244_1 = false;
    _255._244_2 = true;
    _255._244_5 = true;
    _255._244_3 = false;
    _255._244_4 = false;
    _255._244_6 = _214;
	}*/

	void readValue() {
		//_240 = _214;
	}
	
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
	//s32 _240;
	union {
		struct {
			s32 _240;
			u8 _244;
		};
		Bitfield _255;
	};
	JGadget::TLinkListNode _248;
	//JASTrack *_250; // probably part of the link list
};