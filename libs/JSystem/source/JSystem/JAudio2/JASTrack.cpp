#include "JSystem/JAudio2/JASTrack.h"
#include <cstring>

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
    _1D8 = 1f;
    _1DC = 1f;
    _1E0 = 0f;
    _1E4 = 1f;
    _1E8 = 0f;
    _1EC = 1f;
    _1F0 = 0;
    _1F2 = 0;
    _218 = 1f;
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
    _214 = 0;
    LLFlags tmp;
    tmp.byteRepr = _244.byteRepr;
    tmp._0 = false;
    tmp._1 = false;
    tmp._2 = true;
    tmp._5 = true;
    tmp._3 = false;
    tmp._4 = false;
    tmp._6 = false;
    _244.byteRepr = tmp.byteRepr;
    _240 = 0;

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
    _0[0] = 1f;
    _0[1] = 1f;
    _0[2] = 0f;
    _0[3] = 0.5f;
    _0[4] = 0f;
    _0[5] = 0f;
}
