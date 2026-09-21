#pragma once

#include "Game/NPC/NPCActor.hpp"

class MapObjConnector;
class TakeOutStar;

class Kinopio : public NPCActor {
public:
    /// @brief Creates a new `Kinopio`.
    /// @param pName A pointer to the null-terminated name of the object.
    Kinopio(const char* pName);

    virtual void init(const JMapInfoIter& rIter);
    virtual void calcAndSetBaseMtx();
    virtual bool receiveMsgPlayerAttack(u32, HitSensor*, HitSensor*);
    virtual bool receiveOtherMsg(u32, HitSensor*, HitSensor*);

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

    inline bool isPlayerNotNearKinopio();

    /* 0x15C */ s32 mObjArg0;
    /* 0x160 */ s32 mBehavior;
    /* 0x164 */ s32 mAppearBehavior;
    /* 0x168 */ s32 mGoodsIndex;
    /* 0x16C */ TQuat4f _16C;
    /* 0x17C */ MapObjConnector* _17C;
    /* 0x180 */ TakeOutStar* mTakeOutStar;
    /* 0x184 */ bool mIsSpawnPowerStarMode;
    /* 0x185 */ bool _185;
};
