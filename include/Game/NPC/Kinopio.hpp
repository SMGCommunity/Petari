#pragma once

#include "Game/NPC/NPCActor.hpp"

class MapObjConnector;
class TakeOutStar;

class Kinopio : public NPCActor {
public:
    /// @brief Creates a new `Kinopio`.
    /// @param pName A pointer to the null-terminated name of the object.
    Kinopio(const char* pName);

    /* 0x08 */ virtual ~Kinopio();
    /* 0x0C */ virtual void init(const JMapInfoIter& rIter);
    /* 0x4C */ virtual void calcAndSetBaseMtx();
    /* 0x5C */ virtual bool receiveMsgPlayerAttack(u32, HitSensor*, HitSensor*);
    /* 0x74 */ virtual bool receiveOtherMsg(u32, HitSensor*, HitSensor*);

    void faceToVector(const TVec3f&, f32);
    void faceToPlayer(f32);
    void faceToDefault(f32);
    void setDefaultNerve();
    bool tryStartArgs();
    bool tryCancelEvent();
    bool eventFunc(u32);

    void exeReaction();
    void exeTakeOutStar();
    void exeJump();
    void exeWait();
    void exeStop();
    void exeAction();
    void exeAppear();
    void exeEscape();
    void exeStarWait();
    void exeMount();
    void exeEvent();
    void exeEventAndTalk();
    void exeEventFall();

private:
    /* 0x15C */ s32 mObjArg1;
    /* 0x160 */ s32 mObjArg2;
    /* 0x164 */ s32 mObjArg4;
    /* 0x168 */ s32 mObjArg7;
    /* 0x16C */ TQuat4f _16C;
    /* 0x17C */ MapObjConnector* _17C;
    /* 0x180 */ TakeOutStar* mTakeOutStar;
    /* 0x184 */ bool mObjArg3;
    /* 0x185 */ bool _185;
};
