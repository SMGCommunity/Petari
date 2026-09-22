#pragma once

#include "Game/LiveActor/LiveActor.hpp"
#include <JSystem/JGeometry/TMatrix.hpp>

class Mogucchi;
class MogucchiHill;

class MogucchiHillPiece : public LiveActor {
public:
    /// @brief Creates a new `MogucchiHillPiece`.
    /// @param pName A pointer to the null-terminated name of the object.
    MogucchiHillPiece(const char* pName = "モグッチ塚ピース");

    virtual void init(const JMapInfoIter& rIter);
    virtual void appear();
    virtual void control();
    virtual void calcAndSetBaseMtx();
    virtual void attackSensor(HitSensor* pSender, HitSensor* pReceiver);
    virtual bool receiveMsgPlayerAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver);
    virtual bool receiveMsgEnemyAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver);

    void initWithModelName(const char* pModelName, const char* pJointName, const char* pAutoEffectName, bool hasLightCtrl);
    void crumble();
    void destroy();
    void setSize(f32 size);
    bool isTargetGoingAway(HitSensor* pSender, HitSensor* pReceiver) const NO_INLINE;

    void exeAppear();
    void exeWait();
    void exeCrumble();

    /* 0x8C */ MogucchiHill* mHost;
    /* 0x90 */ const char* mModelName;
    /* 0x94 */ const char* mJointName;
    /* 0x98 */ const char* mAutoEffectName;
    /* 0x9C */ TPos3f _9C;
    /* 0xCC */ bool _CC;
    /* 0xCD */ bool _CD;
    /* 0xCE */ bool mHasLightCtrl;
};

class MogucchiHill : public LiveActor {
public:
    MogucchiHill(LiveActor* pHost, s32 pieceCount, const char* pName);

    virtual void init(const JMapInfoIter& rIter);

    virtual bool notifyAppearAttackToPlayer(HitSensor* pSender, HitSensor* pReceiver) {
        return false;
    }

    virtual bool notifyWaitAttackToPlayer(HitSensor* pSender, HitSensor* pReceiver) {
        return false;
    }

    void start();
    void end();
    void startNaturally();
    void endNaturally();
    void killPieces(MogucchiHillPiece* pIter);
    void pauseOff();
    void killAllPieces();
    void setAppearNum(s32 appearNum);
    void createPieces();
    void appearPiece();
    void killPieceOverAppear();
    void killPieceIfAlive();
    f32 getDistanceFromReserveMtx() const NO_INLINE;
    void reserveCurrentMtx();

    void exeMove();
    void exeWait();

    /* 0x8C */ LiveActor* mHost;
    /* 0x90 */ MogucchiHillPiece* _90;
    /* 0x94 */ s32 _94;
    /* 0x98 */ s32 _98;
    /* 0x9C */ TPos3f mReserveMtx;
    /* 0xCC */ bool _CC;
    /* 0xD0 */ f32 _D0;
    /* 0xD4 */ f32 _D4;
    /* 0xD8 */ u32 _D8;
    /* 0xDC */ const char** mModelNameTable;
    /* 0xE0 */ const char* mJointName;
    /* 0xE4 */ const char* mAutoEffectName;
    /* 0xE8 */ bool mHasLightCtrl;
    /* 0xEC */ s32 _EC;
    /* 0xF0 */ s32 _F0;
    /* 0xF4 */ s32 mAppearNum;
};
