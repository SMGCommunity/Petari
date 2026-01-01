#pragma once

#include "Game/LiveActor/LiveActor.hpp"
#include "Game/NameObj/NameObjArchiveListCollector.hpp"

class CocoNut : public LiveActor {
public:
    CocoNut(const char*);

    virtual ~CocoNut();
    virtual void init(const JMapInfoIter&);
    virtual void initAfterPlacement();
    virtual void startClipped();
    virtual void calcAndSetBaseMtx();
    virtual void attackSensor(HitSensor*, HitSensor*);
    virtual bool receiveMsgPush(HitSensor*, HitSensor*);
    virtual bool receiveMsgPlayerAttack(u32, HitSensor*, HitSensor*);
    virtual bool receiveOtherMsg(u32, HitSensor*, HitSensor*);

    static void makeArchiveList(NameObjArchiveListCollector*, const JMapInfoIter&);

    void initMapToolInfo(const JMapInfoIter&);
    void initModel() NO_INLINE;
    void initSensor();
    void initEffect();
    void updateRotate(f32);
    void updateGravity();
    void processMove();
    bool isOnGround() const;
    f32 calcMoveSpeed() const;
    void setFrontVec(const TVec3f&);
    bool getWallNormal(TVec3f*) const;

    bool sendMsgToBindedSensor();
    bool sendMsgEnemyAttackToBindedSensor(HitSensor*);

    bool tryHit(HitSensor*, HitSensor*);
    bool isPossibleToHit(const TVec3f&, const TVec3f&, const TVec3f&) const;
    void calcHitSpeedAndFrontVec(f32*, f32*, TVec3f*, TVec3f*, const TVec3f&, const TVec3f&) const;
    void hit(const TVec3f&, f32);
    bool tryPushedFromActor(HitSensor*, HitSensor*);

    void reviseFrontVec();
    void statusToWait();

    bool isValidReceiveMsg(const HitSensor*) const;

    void statusToHide();
    void emitEffectSpinHit(const HitSensor*, const HitSensor*);

    void tryMoveEnd();

    bool tryDisappear();
    bool isContactWithOtherCocoNut() const;

    bool isValidPushedFromPlayer(const HitSensor*, const HitSensor*) const;

    void calcHitSpeedAndFrontVec(f32*, f32*, TVec3f*, TVec3f*, TVec3f*, TVec3f*) const;

    static const char* getModelName();
    static const char* getBreakEffectName();

    void exeWait();
    void exeWaitOnBind();
    void exeMove();
    void exeInWater();
    void exeBreak();
    void exeReplaceReady();

    inline f32 getSize() { return _D0; }

private:
    f32 _8C;
    f32 _90;
    TVec3f _94;
    TPos3f _A0;
    f32 _D0;
    bool _D4;
    TPos3f _D8;
    TPos3f _108;
    s32 _138;
    bool _13C;
    TVec3f mSpawnPosition;  // 0x140
    bool _14C;
    TVec3f _150;
    bool mSphericalShadow;       // 0x15C, also affects some other properties besides shadow
    bool mRespawnWhenOutOfView;  // 0x15D
    bool _15E;                   // something about clipping
    bool mContinueRolling;       // 0x15F
};
