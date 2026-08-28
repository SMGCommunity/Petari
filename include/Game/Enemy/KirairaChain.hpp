#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class KirairaChain : public LiveActor {
public:
    KirairaChain(const LiveActor*);
    /* 0x08 */ virtual ~KirairaChain();

    /* 0x0C */ virtual void init(const JMapInfoIter& rIter);
    /* 0x18 */ virtual void draw() const;
    /* 0x48 */ virtual void control();

    /* 0x8C */ s32 _8C;
    /* 0x90 */ bool _90;
    /* 0x94 */ TVec3f _94;
    /* 0xA0 */ u8 _A0[0xCC];
};
