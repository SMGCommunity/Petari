#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class CollectTico;
class PartsModel;

/// @brief Silver Star
class StrayTico : public LiveActor {
public:
    StrayTico(const char*, CollectTico*);

    virtual void init(const JMapInfoIter&);
    virtual void appear();
    virtual void control();
    virtual void calcAndSetBaseMtx();
    virtual void attackSensor(HitSensor*, HitSensor*);
    virtual bool receiveMsgPush(HitSensor*, HitSensor*);
    virtual bool receiveMsgPlayerAttack(u32, HitSensor*, HitSensor*);
    virtual bool receiveOtherMsg(u32, HitSensor*, HitSensor*);

    void initSensor();
    void initShadow() NO_INLINE;
    void startGlad();
    bool requestCompleteDemo(const TVec3f&, const TVec3f&, f32);
    void updateWarp();
    void updateChase();
    bool isEnableSpin() const;
    bool isRescued() const;
    bool isCompleteDemoEnd() const;
    void noticeEnd();

    void exeWait();
    void exeGlad();
    void exeChase();
    void exeLauncherCamera();
    void exeCompleteDemo();

    /* 0x8C */ CollectTico* mHost;
    /* 0x90 */ PartsModel* mItemBubbleModel;
    /* 0x94 */ TQuat4f mRotateQuat;
    /* 0xA4 */ TVec3f mFrontVec;
    /* 0xB0 */ TVec3f _B0;
    /* 0xBC */ TVec3f _BC;
    /* 0xC8 */ TVec3f _C8;
    /* 0xD4 */ TVec3f _D4;
    /* 0xE0 */ TVec3f _E0;
    /* 0xEC */ f32 _EC;
    /* 0xF0 */ f32 _F0;
};
