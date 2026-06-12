#pragma once

#include "Game/LiveActor/LiveActor.hpp"
#include <JSystem/JGeometry/TMatrix.hpp>

class ActorCameraInfo;
class AnimScaleController;
class CocoNutBall;
class FireBall;
class FireBubble;
class FixedPosition;
class NameObjArchiveListCollector;
class OtaKingLongFoot;
class OtaKingMagma;
class PartsModel;

class OtaKing : public LiveActor {
public:
    /// @brief Creates a new `OtaKing`.
    /// @param pName A pointer to the null-terminated name of the object.
    OtaKing(const char* pName);

    virtual void init(const JMapInfoIter& rIter);
    virtual void movement();
    virtual void makeActorAppeared();
    virtual void control();
    virtual void calcAndSetBaseMtx();
    virtual void attackSensor(HitSensor*, HitSensor*);
    virtual bool receiveMsgPlayerAttack(u32, HitSensor*, HitSensor*);
    virtual bool receiveMsgEnemyAttack(u32, HitSensor*, HitSensor*);
    virtual bool receiveOtherMsg(u32, HitSensor*, HitSensor*);

    static void makeArchiveList(NameObjArchiveListCollector*, const JMapInfoIter&);

    void initMapToolInfo(const JMapInfoIter&);
    void initModel(const JMapInfoIter&);
    void initSensor();
    void dirToPlayer();
    CocoNutBall* getDisappearedCocoNut();
    FireBall* getDisappearedFireBall();
    FireBubble* getDisappearedBubble();
    s32 getDisappearedCocoNutNum() const;
    s32 getDisappearedFireBallNum() const;
    s32 getDisappearedFireBubbleNum() const;
    bool isValidThrowCocoNut() const;
    bool isValidThrowFireBall() const;
    bool isValidBubbleAttack() const;
    void damage();
    void throwCocoNut();
    void throwFireBall();
    void startBckWaitIfNotPlaying();
    void startBckWithFrontFoot(const char*);
    bool isNerveValidNearAttack() const;
    void appearBubble();
    void startDemo();
    void startAppearDemo();
    void appearStarPiece();
    void initLongFoot(const JMapInfoIter&);
    bool tryThrowCocoNutOrFireBallIfWait(s32);

    void exeWaitOnSwitch();
    void exeAppearDemo();
    void exeWait();
    void exeThrowCocoNutWait();
    void exeThrowCocoNut();
    void exeThrowFireBallWait();
    void exeThrowFireBall();
    void exeDamage();
    void exePowerUp();
    void exeDown();
    void exeDownDemo();
    void exeAppearStar();
    void exeDead();
    void exeHitBackStart();
    void exeHitBack();
    void exeNearAttack();
    void exeBubbleAttack();
    void exeWaitStartDemo();

    /* 0x08C */ PartsModel* _8C[2];
    /* 0x094 */ OtaKingLongFoot* mLongFootArray[4];
    /* 0x0A4 */ OtaKingMagma* mMagma;
    /* 0x0A8 */ TMtx34f _A8;
    /* 0x0D8 */ CocoNutBall* mCocoNutBallArray;
    /* 0x0DC */ FireBall* mFireBallArray;
    /* 0x0E0 */ FireBubble* mFireBubbleArray;
    /* 0x0E4 */ FixedPosition* _E4;
    /* 0x0E8 */ s32 _E8;
    /* 0x0EC */ s32 _EC;
    /* 0x0F0 */ f32 _F0;
    /* 0x0F4 */ AudAnmSoundObject* _F4;
    /* 0x0F8 */ TVec3f _F8;
    /* 0x104 */ FixedPosition* _104;
    /* 0x104 */ CocoNutBall* _108;
    /* 0x10C */ ActorCameraInfo* _10C;
    /* 0x110 */ char _110[2][32];
    /* 0x150 */ bool mIsLv2;
    /* 0x154 */ AnimScaleController* _154;
};
