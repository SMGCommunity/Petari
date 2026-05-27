#pragma once

#include "Game/Boss/OtaKing.hpp"
#include "Game/Enemy/CocoNutBall.hpp"
#include "Game/LiveActor/ActorCameraInfo.hpp"
#include "Game/LiveActor/LiveActor.hpp"
#include "Game/LiveActor/PartsModel.hpp"
#include "JSystem/JGeometry/TMatrix.hpp"
#include "JSystem/JGeometry/TVec.hpp"

class AudAnmSoundObject;
class AnimScaleController;
class FireBall;
class FireBubble;
class FixedPosition;
class NameObjArchiveListCollector;
class OtaKingMagma;
class OtaKingLongFoot;


class OtaKing : public LiveActor {
public:
    OtaKing(const char*);

    virtual void init(const JMapInfoIter& rIter);
    virtual void movement();
    virtual void makeActorAppeared();
    virtual void control();
    virtual void calcAndSetBaseMtx();
    virtual void attackSensor(HitSensor* pSender, HitSensor* pReceiver);
    virtual bool receiveMsgPlayerAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver);
    virtual bool receiveMsgEnemyAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver);
    virtual bool receiveOtherMsg(u32 msg, HitSensor* pSender, HitSensor* pReceiver);

    void startAppearDemo();
    static void makeArchiveList(NameObjArchiveListCollector*, const JMapInfoIter&);
    void initMapToolInfo(const JMapInfoIter& rIter);
    void initModel(const JMapInfoIter& rIter);
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
    void appearStarPiece();
    void initLongFoot(const JMapInfoIter& rIter);
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

    bool isSensorBodyOrFace(HitSensor*);
    bool isDamageNerve();

    PartsModel* _8C[2];
    OtaKingLongFoot* _94[4];
    OtaKingMagma* _A4;
    TPos3f _A8;

    CocoNutBall* _D8;
    FireBall* _DC;
    FireBubble* _E0;
    
    FixedPosition* _E4;
    s32 _E8;
    s32 _EC;

    f32 _f0;
    AudAnmSoundObject* _f4;
    TVec3f _f8;

    FixedPosition* _104;
    CocoNutBall* _108;
    ActorCameraInfo* _10C;
    char _110[0x20];
    char _130[0x20];
    bool _150;
    AnimScaleController* _154;
};
