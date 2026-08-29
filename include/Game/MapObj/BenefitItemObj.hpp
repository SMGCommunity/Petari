#pragma once

#include "Game/LiveActor/LiveActor.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include <JSystem/JGeometry/TMatrix.hpp>

class ActorCameraInfo;
class FlashingCtrl;

class BenefitItemObj : public LiveActor {
public:
    /// @brief Creates a new `BenefitItemObj`.
    /// @param pName A pointer to the null-terminated name of the object.
    /// @param pModelName A pointer to the null-terminated name of the model.
    BenefitItemObj(const char* pName, const char* pModelName);

    virtual ~BenefitItemObj();
    virtual void init(const JMapInfoIter&);
    virtual void initAfterPlacement();
    virtual void appear();
    virtual void makeActorAppeared();
    virtual void control();
    virtual void calcAndSetBaseMtx();
    virtual bool receiveMsgPlayerAttack(u32, HitSensor*, HitSensor*);
    virtual bool receiveOtherMsg(u32, HitSensor*, HitSensor*);
    virtual void appearGround();
    virtual void appearThrowUp();
    virtual void appearThrowUpQuestionBox();
    virtual void initModelAndEfx();
    virtual void exeCatch();
    virtual void runBck(const char*);
    virtual void runEfx(const char*);
    virtual void stopEfx(const char*);
    virtual void doRotateY();
    virtual void initEscape();
    virtual void doEscape();

    void shoot(const TVec3f&, const TVec3f&, bool);
    void exeShoot();
    void exeWait();
    void exeAppearGround();
    void exePreEscape();
    void exeEscape();
    void setFollowMtx(MtxPtr);
    void calcAndSetBaseMtxInMovement();

    /* 0x08C */ u32 _8C;
    /* 0x090 */ u32 _90;
    /* 0x094 */ u32 _94;
    /* 0x098 */ TVec3f _98;
    /* 0x0A4 */ TVec3f _A4;
    /* 0x0B0 */ s16 _B0;
    /* 0x0B2 */ s16 _B2;
    /* 0x0B4 */ s16 _B4;
    /* 0x0B6 */ s16 _B6;
    /* 0x0B8 */ s16 _B8;
    /* 0x0BA */ s16 _BA;
    /* 0x0BC */ TVec3f _BC;
    /* 0x0C8 */ TVec3f _C8;
    /* 0x0D4 */ MtxPtr mFollowMtx;
    /* 0x0D8 */ u8 _D8;
    /* 0x0D9 */ u8 _D9;
    /* 0x0DA */ u8 _DA;
    /* 0x0DB */ u8 _DB;
    /* 0x0DC */ u8 _DC;
    /* 0x0DD */ u8 _DD;
    /* 0x0DE */ u8 _DE;
    /* 0x0DF */ u8 _DF;
    /* 0x0E0 */ u8 _E0;
    /* 0x0E1 */ u8 _E1;
    /* 0x0E2 */ u8 _E2;
    /* 0x0E3 */ u8 _E3;
    /* 0x0E4 */ u8 _E4;
    /* 0x0E5 */ u8 _E5;
    /* 0x0E6 */ u8 _E6;
    /* 0x0E7 */ u8 _E7;
    /* 0x0E8 */ f32 _E8;
    /* 0x0EC */ f32 _EC;
    /* 0x0F0 */ TMtx34f _F0;
    /* 0x120 */ const char* _120;
    /* 0x124 */ FlashingCtrl* mFlashCtrl;
    /* 0x128 */ ActorCameraInfo* mCameraInfo;
    /* 0x12C */ u32 _12C;
    /* 0x130 */ u32 _130;
    /* 0x134 */ LiveActor* mHitSensorActor;
    /* 0x138 */ u8 _138;
    /* 0x139 */ u8 _139;
    /* 0x13A */ u16 _13A;
};

class BenefitItemOneUp : public BenefitItemObj {
public:
    /// @brief Creates a new `BenefitItemOneUp`.
    /// @param pName A pointer to the null-terminated name of the object.
    /// @param pModelName A pointer to the null-terminated name of the model.
    BenefitItemOneUp(const char* pName, const char* pModelName = "KinokoOneUp") : BenefitItemObj(pName, pModelName) {
    }
};

class ShadowClipActor : public LiveActor {
public:
    virtual void endClipped();
    virtual void control();

    /* 0x8C */ LiveActor* _8C;
};

namespace NrvBenefitItemObj {
    NERVE_DECL_EXE(HostTypeNrvWait, BenefitItemObj, Wait);
    NERVE_DECL_EXE(HostTypeNrvShoot, BenefitItemObj, Shoot);
    NERVE_DECL_EXE(HostTypeNrvCatch, BenefitItemObj, Catch);
    NERVE_DECL_EXE(HostTypeNrvAppearGround, BenefitItemObj, AppearGround);
    NERVE_DECL_EXE(HostTypeNrvPreEscape, BenefitItemObj, PreEscape);
    NERVE_DECL_EXE(HostTypeNrvEscape, BenefitItemObj, Escape);
};  // namespace NrvBenefitItemObj
