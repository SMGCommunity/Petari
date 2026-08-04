#pragma once

#include "Game/LiveActor/ModelObj.hpp"

class ActorCameraInfo;
class NameObjArchiveListCollector;

class SunakazeKunTrap : public ModelObj {
public:
    SunakazeKunTrap(LiveActor*, const TVec3f&);

    virtual void init(const JMapInfoIter& rIter);
    virtual void attackSensor(HitSensor*, HitSensor*);

    void exeTrap();

private:
    /* 0x90 */ LiveActor* mHost;
    /* 0x94 */ TVec3f _94;
};

class Sandstorm : public LiveActor {
public:
    enum Type {
        /* 0x00 */ Type_TwisterSan,
        /* 0x01 */ Type_SunakazeKun,
        /* 0x02 */ Type_TwisterSanSlim,
    };

    /// @brief Creates a new `Sandstorm`.
    /// @param pName A pointer to the null-terminated name of the object.
    Sandstorm(const char* pName);

    virtual void init(const JMapInfoIter& rIter);
    virtual void startClipped();
    virtual void endClipped();
    virtual void control();
    virtual void calcAndSetBaseMtx();
    virtual void updateHitSensor(HitSensor*);
    virtual bool receiveMsgPlayerAttack(u32, HitSensor*, HitSensor*);
    virtual bool receiveOtherMsg(u32, HitSensor*, HitSensor*);

    static void makeArchiveList(NameObjArchiveListCollector*, const JMapInfoIter&);

    void initSandstormParam();
    void initTrapModel();
    void updateBindActorMtx();
    void endBind();
    void updateSpiral();
    bool tryStartBind(HitSensor*);
    bool tryTornadoJumpPlayer();
    bool tryNonActive();
    bool isSunakazeKun() const;

    void exeNonActive();
    void exeWait();
    void exeStorm();
    void exeStormAdjust();
    void exeStormFix();
    void exeStormEnd();

private:
    /* 0x8C */ ActorCameraInfo* _8C;
    /* 0x90 */ LiveActor* mBindActor;
    /* 0x94 */ TVec3f _94;
    /* 0xA0 */ TVec3f _A0;
    /* 0xAC */ s32 mType;
    /* 0xB0 */ f32 mSpiralRot;
    /* 0xB4 */ f32 mSpiralRadius;
    /* 0xB8 */ f32 _B8;
    /* 0xBC */ f32 _BC;
    /* 0xC0 */ f32 _C0;
    /* 0xC4 */ f32 _C4;
    /* 0xC8 */ s32 _C8;
    /* 0xCC */ f32 _CC;
    /* 0xD0 */ f32 mSensorHeight;
    /* 0xD4 */ f32 mSizeUpRate;
    /* 0xD8 */ f32 mSensorMin;
    /* 0xDC */ s32 mStepStorm;
    /* 0xE0 */ s32 mStepAdjust;
    /* 0xE4 */ s32 mStepFix;
    /* 0xE8 */ s32 mStepEnd;
    /* 0xEC */ f32 mBindHeight;
    /* 0xF0 */ SunakazeKunTrap** mTrapModel;
};
