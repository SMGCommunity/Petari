#pragma once

#include "Game/LiveActor/LiveActor.hpp"

/// @brief Faceless Amp
class Birikyu : public LiveActor {
public:
    Birikyu(const char*);

    virtual void init(const JMapInfoIter&);
    virtual void initAfterPlacement();
    virtual void appear();
    virtual void startClipped();
    virtual void endClipped();
    virtual void attackSensor(HitSensor* pSender, HitSensor* pReceiver);
    virtual bool receiveMsgPlayerAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver);
    virtual bool receiveOtherMsg(u32 msg, HitSensor* pSender, HitSensor* pReceiver);
    virtual f32 getHitRadius() const;
    virtual char* getCenterJointName() const;

    void initFromJmpArgs(const JMapInfoIter&);
    void initRail(const JMapInfoIter&);
    void initCollision();
    void initShadow();
    bool tryStopPointing();
    void goMove();
    bool isMoving() const;
    void exeMove();
    void exeMoveCircle();
    void exeWaitAtEdge();
    void exeAttack();
    void exeAttackWait();
    void exeStopPointing();

    /* 0x8C */ const char* mObjectName;
    /* 0x90 */ TVec3f mRailClipCenter;
    /* 0x9C */ TVec3f mOrbitCenter;

    /// @brief JMapInfo arg 2.
    /* 0xA8 */ bool mHasShadow;

    /* 0xA9 */ bool mIsConnected;
    /* 0xAC */ TVec3f mUpVec;
    /* 0xB8 */ TVec3f mFrontVec;

    /// @brief In radians.
    /* 0xC4 */ f32 mOrbitAngle;

    /// @brief Used for both rail movement and orbit speed.
    /* 0xC8 */ f32 mMoveSpeed;
};

/// @brief Amp
class BirikyuWithFace : public Birikyu {
public:
    BirikyuWithFace(const char*);

    virtual void init(const JMapInfoIter&);
    virtual void calcAndSetBaseMtx();
    virtual f32 getHitRadius() const;
    virtual char* getCenterJointName() const;

    /// @brief JMapInfo arg 1.
    /* 0xCC */ bool _CC;
};
