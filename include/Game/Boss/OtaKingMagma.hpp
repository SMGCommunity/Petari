#pragma once

#include "Game/LiveActor/PartsModel.hpp"

class ProjmapEffectMtxSetter;

class OtaKingMagma : public PartsModel {
public:
    OtaKingMagma(LiveActor*, s32);

    virtual void init(const JMapInfoIter& rIter);
    virtual void control();

    void appearDemo();
    void attack();
    void fireAttack();
    void hitBack();
    void damage();
    void down();

    void exeWait();
    void exeAppearDemo();
    void exeAttack();
    void exeFireAttack();
    void exeHitBack();
    void exeDamage();
    void exeDown();
    void exeEndWait();

    void startBckThisAndBloomModel(const char*);

    /* 0x9C */ PartsModel* mBloomModel;
    /* 0xA0 */ ProjmapEffectMtxSetter* _A0;
    /* 0xA4 */ ProjmapEffectMtxSetter* _A4;
};
