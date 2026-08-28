#pragma once

#include "Game/LiveActor/HitSensor.hpp"
#include "Game/NPC/Tico.hpp"

class TicoEat : public Tico {
public:
    TicoEat(const char*);

    /* 0x0C */ virtual void init(const JMapInfoIter&);
    /* 0x48 */ virtual void control();
    /* 0x5C */ virtual bool receiveMsgPlayerAttack(u32, HitSensor*, HitSensor*);
    /* 0x78 */ virtual void startReactionSound();

    void initStarPiece(s32);
    bool tryEat();
    bool hasEnoughStarPiece() const;
    bool eventFunc(u32);
    void exeReaction();
    void exeEatPre();
    void exeEatNow();
    void exeEatPst();
    void exeEatEnd();

    u32 _190;
    s32 _194;
    s32 _198;
};

class TicoComet : public TicoEat {
public:
    TicoComet(const char*);

    /* 0x0C */ virtual void init(const JMapInfoIter&);
    /* 0x78 */ virtual void startReactionSound();

    bool branchFunc(u32);
    bool tryDemo();
    bool eventFunc(u32);
    void exeDemoAnim();
    void exeDemoFade();
    void exeDemoEnd();
};
