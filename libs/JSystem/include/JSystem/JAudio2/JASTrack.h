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

class JASCriticalSection {
public:
	JASCriticalSection();
	~JASCriticalSection();
	u8 _0[8];
};

struct JASDefaultBankTable {
    JASDefaultBankTable();
    ~JASDefaultBankTable();
    u8 _40C[0x40C];
};

class JASTrack : public JASSeqCtrl, public JASPoolAllocObject_MultiThreaded<JASTrack> {
public:

	static const JASOscillator::Data sEnvOsc;
	static const JASOscillator::Data sPitchEnvOsc;

	static JASDefaultBankTable sDefaultBankTable;

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
        void releaseAll();
        bool noteOff(u32, u16);
        void setPauseFlag(bool);

		JASChannel* _0[8];
		JASChannelParams _20;
		u16 _38[8];
		JASSoundParams* _48;
		JASTrack *_4C;
	};

	struct Timed {
		f32 _0;
		f32 _4;
		u32 _8;
	};

	JASTrack();
	
	~JASTrack();
	
	s32 seqMain();

	void init();
	void initTimed();
	
	void startSeq();
	void stopSeq();
	
	void start();
    JASChannel* channelStart(TChannelMgr *, u32, u32, u32);

	void close();

	int tickProc();

	void inherit(const JASTrack &);

	JASTrack* openChild(u32);
	bool connectChild(u32, JASTrack *);
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
	static void channelUpdateCallback(u32, JASChannel *, JASDsp::TChannel *, void *);

	bool gateOn(u32, u32, f32, u32);
	bool noteOn(u32, u32, u32);
	bool noteOff(u32, u16);
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

	s32 getTransposeTotal() const;
	bool isMute() const;

	void overwriteOsc(JASChannel *);
	void setOscScale(u32, f32);
	void setOscTable(u32, const JASOscillator::Point *);
	void setOscAdsr(s16, s16, s16, s16, u16);

	void setFIR(const s16 *);
	void setIIR(const s16 *);
	u32 seqTimeToDspTime(f32);
	
	u16 readPortSelf(u32);
	u16 readPort(u32);
	void writePortSelf(u32, u16);
	void writePort(u32, u16);
	
	JASTrackPort mPorts; // _5C
	JASRegisterParam _80;
	u8 _84[0x18];
	Timed _9C[6];
	JASOscillator::Data _E4[2];
	//JASOscillator::Point _FC[4];
	JASOscillator::Point _114[4];
	JASTrack *mParent; // _12C
	JASTrack *mChildren[0x10]; // _130
	TChannelMgr* _170[4];
	TChannelMgr _180;
	u32 _1D0;
	JASDefaultBankTable* _1D4;
	f32 _1D8;
	f32 _1DC;
	f32 _1E0;
	f32 _1E4;
	f32 _1E8;
	f32 _1EC;
	u16 _1F0;
	u16 _1F2;
	s16 _1F4[8];
	s16 _204[8];
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
	u16 _234[6];
	volatile s32 _240;
	volatile LLFlags _244; // Volatility seems to depend upon which union field we use
	JGadget::TLinkListNode _248;

	struct TList;
	
	static TList sTrackList;
};

struct JASTrack::TList : public JGADGET_LINK_LIST(JASTrack, _248) {
    TList();
    ~TList();
    
    static s32 cbSeqMain(void *);
    void append(JASTrack *);
    void seqMain();
    u8 _0[0xC];
    bool _C;
};

namespace JGadget {
    bool operator!=(JGADGET_LINK_LIST(JASTrack, _248)::iterator, JGADGET_LINK_LIST(JASTrack, _248)::iterator);
}