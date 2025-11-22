#pragma once

#include "Game/LiveActor/HitSensor.hpp"
#include "Game/LiveActor/LiveActor.hpp"

class CollectTico;

class StrayTico : public LiveActor {
public:
    StrayTico(const char*, CollectTico*);

    virtual ~StrayTico();
    virtual void init(const JMapInfoIter&);
    virtual void appear();
    virtual void control();
    virtual void calcAndSetBaseMtx();
    virtual void attackSensor(HitSensor*, HitSensor*);
    virtual bool receiveMsgPush(HitSensor*, HitSensor*);
    virtual bool receiveMsgPlayerAttack(u32, HitSensor*, HitSensor*);
    virtual bool receiveOtherMsg(u32, HitSensor*, HitSensor*);

    bool requestCompleteDemo(const TVec3f&, const TVec3f&, f32);
    void exeWait();
    void exeGlad();
    void exeChase();
    void exeLauncherCamera();
    void updateWarp();
    void updateChase();
    void exeCompleteDemo();
    bool isEnableSpin() const;
    bool isRescued() const;
    bool isCompleteDemoEnd() const;
    void noticeEnd();

    CollectTico* mParent;      // 0x8C
    PartsModel* mBubbleModel;  // 0x90
    TQuat4f _94;
    TVec3f _A4;
    TVec3f _B0;
    TVec3f _BC;
    TVec3f _C8;
    TVec3f _D4;
    TVec3f _E0;
    f32 _EC;
    f32 _F0;
};
