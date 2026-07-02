#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class KirairaChain : public LiveActor {
public:
    KirairaChain(const LiveActor*);
    virtual ~KirairaChain();

    virtual void init(const JMapInfoIter& rIter);
    virtual void draw() const;
    virtual void control();

    /* 0x8C */ s32 _8C;
    /* 0x90 */ bool _90;
    /* 0x94 */ TVec3f _94;
    /* 0xA0 */ u8 _A0[0xCC];
};
