#pragma once

#include "Game/LiveActor/LiveActor.hpp"
#include <JSystem/JGeometry/TMatrix.hpp>

class NameObjArchiveListCollector;

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
    bool isInGroundGracePeriod() const {
        return _138 < 10;
    }
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

    inline f32 getSize() {
        return _D0;
    }

private:
    /* 0x8C */ f32 _8C;
    /* 0x90 */ f32 _90;
    /* 0x94 */ TVec3f _94;
    /* 0xA0 */ TPos3f _A0;
    /* 0xD0 */ f32 _D0;
    /* 0xD4 */ bool _D4;
    /* 0xD8 */ TPos3f _D8;
    /* 0x108 */ TPos3f _108;
    /* 0x138 */ s32 _138;
    /* 0x13C */ bool _13C;
    /* 0x140 */ TVec3f mSpawnPosition;
    /* 0x14C */ bool _14C;
    /* 0x150 */ TVec3f _150;
    /* 0x15C */ bool mSphericalShadow;  // also affects some other properties besides shadow
    /* 0x15D */ bool mRespawnWhenOutOfView;
    /* 0x15E */ bool _15E;  // something about clipping
    /* 0x15F */ bool mContinueRolling;
};
