#include "JSystem/JAudio2/JASTrack.h"
#include <cstring>
#pragma opt_loop_invariants on

static const JASOscillator::Point data[4] = {
    {0, 0, 0x7fff},
    {0, 0, 0x7fff},
    {0, 0, 0},
    {0xe, 0, 0}
};

const JASOscillator::Point JASTrack::sEnvOsc[4] = {
    {0, 0, 0x3f80},
    {0, 0, 0},
    {0, 0, 0x3f80},
    {0, 0, 0}
};

const JASOscillator::Point JASTrack::sPitchEnvOsc[4] = {
    {0, 1, 0x3f80},
    {0, 0, 0},
    {0, 0, 0x3f80},
    {0, 0, 0}
};


/*JASTrack::JASTrack() : JASSeqCtrl(), _80(), _180(this), _1D0(1), _255._240(0), _248() {
    _170[0] = &_180;
    for(u32 i = 1; i < 4; i++) {
        _170[i] = nullptr;
    }
    init();
}*/

JASTrack::~JASTrack() {
    for(int i = 1; i < 4; i++) {
        delete _170[i];
    }
}

void JASTrack::setChannelMgrCount(u32 count) {
    _1D0 = 1;
    for(u32 i = 1; i < count; i++, _1D0++) {
        if(_170[i]) continue;
        if(!(_170[i] = new TChannelMgr(this))) return;
    }
    for(u32 i = _1D0; i < 4; i++) {
        if(_170[i]) {
            delete _170[i];
            _170[i] = nullptr;
        }
    }
}

struct GoofyStruct {
    s32 &rStupid;
    inline void doDumb() {
        rStupid = 0;
    }
    //inline ~GoofyStruct() {}
};

/*inline JASTrack::Bitfield reallyBadBS(JASTrack *trackFake) {
    JASTrack::Bitfield track = trackFake->_255;
    track._244_0 = false;
    track._244_1 = false;
    track._244_2 = true;
    track._244_5 = true;
    track._244_3 = false;
    track._244_4 = false;
    track._244_6 = false;
    return track;

}*/

u16 death(const JASTrack *, JASTrack::Bitfield, u32);
void stupidIdiot(volatile u32 *dumb, volatile u32 arg) {
    *dumb = 10;
    for(u32 i = 0; i < arg; i++) {
        *dumb *= i;
    }
}

inline void a(u32 &rTmp) {
    rTmp = ( 0xffffff7f & 0xffffffbf & rTmp | 0x20 | 4) & 0xffffffef & 0xfffffff7;
}

void b(u32 &rTmp) {
 rTmp &= 0xfffffffd;
}

inline s32 getAssignment(volatile JASTrack *track) {
    return 0;
}

void finish(JASTrack *track) {
        track->_240 = 0;
    
}

void c(u32 i) {}
void JASTrack::init() {
    JASSeqCtrl::init();
    mPorts.init();
    initTimed();
    _80.init();
    memcpy(&_E4, sEnvOsc, 0x18);
    memcpy(&_FC, sPitchEnvOsc, 0x18);
    for(int i = 0; i < 4; i++) {
        _114[i] = data[i];
    }
    mParent = nullptr;
    for(u32 i = 0; i < 0x10; i++) {
        mChildren[i] = nullptr;
    }
    _170[0]->init();
    _1D0 = 1;
    for(int i = 1; i < 4; i++) {
        if(_170[i]) {
            delete _170[i];
            _170[i] = nullptr;
        }
    }
    _1D4 = sDefaultBankTable;
    _1D8 = 1.0f;
    _1DC = 1.0f;
    _1E0 = 0.0f;
    _1E4 = 1.0f;
    _1E8 = 0.0f;
    _1EC = 1.0f;
    _1F0 = 0;
    _1F2 = 0;
    _218 = 1.0f;
    _224 = 0;
    _220 = 0;
    _226 = 0x78;
    mTimerBase = 0x30;
    _21C = 1.0f;
    updateTempo();
    _22A = 0;
    _22B = 0x3C;
    _22C = 0;
    _22E = 0xF0;
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
    /*u32 &b = a;
    b = 1;
    if(a == 1) return;*/
    u32 tmp = 0, tmp3 = 0;
    
    _214 = tmp;
    u32 tmp2 = _244;
    a(tmp2);
    b(tmp2);
    *(volatile u8 *)&_244 = tmp2;
    _240 = 0;
        /*JASTrack::Bitfield &tmp2 = _255;
        tmp2._244_0 = false;
        tmp2._244_1 = false;
        tmp2._244_2 = true;
        tmp2._244_5 = true;
        tmp2._244_3 = false;
        tmp2._244_4 = false;
        tmp2._244_6 = false;
     _240 = tmp2._244_6;*/
    
    //if(_23E) return;
    //else return;
    //if(false) for(u32 i = 0; i < 8; i++) _204[i] = 1;
    //_204[0] = 0x7fff;
    //_214 = 0;
    
    /*trueBs();
    readValue();*/
    //const JASTrack::Bitfield bf = {false, false, true, false, false, true, false};
    //_255 = 0x24;
	/*_255._244_0 = false;
    _255._244_1 = false;
    _255._244_2 = true;
    _255._244_5 = true;
    _255._244_3 = false;
    _255._244_4 = false;
    _255._244_6 = tmp;*/

    //unsigned int dumb = ;

    

}
inline u16 death(const JASTrack *track, JASTrack::Bitfield BF, u32 a) {
    return a;
}


inline Stupid::~Stupid() {}

inline void JASTrack::Bitfield::bs() {
			_244_0 = false;
		    _244_1 = false;
		    _244_2 = true;
		    _244_5 = true;
		    _244_3 = false;
		    _244_4 = false;
		    _244_6 = false;
		    bigScaryFunction();
		}
    
JASTrack::TChannelMgr::TChannelMgr(JASTrack *track) : _20(), _48(0), _4C(track) {
    for(u32 i = 0; i < 8; i++) {
        _0[i] = 0;
    }
    for(u32 i = 0; i < 8; i++) {
        _38[i] = 0;
    }
}

JASChannelParams::JASChannelParams() {
    init();
}

inline void JASChannelParams::init() {
    _0[0] = 1.0f;
    _0[1] = 1.0f;
    _0[2] = 0.0f;
    _0[3] = 0.5f;
    _0[4] = 0.0f;
    _0[5] = 0.0f;
}
