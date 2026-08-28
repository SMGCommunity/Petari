#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class CollectTico;
class PartsModel;

class StrayTico : public LiveActor {
public:
    StrayTico(const char*, CollectTico*);

    /* 0x08 */ virtual ~StrayTico();
    /* 0x0C */ virtual void init(const JMapInfoIter&);
    /* 0x24 */ virtual void appear();
    /* 0x48 */ virtual void control();
    /* 0x4C */ virtual void calcAndSetBaseMtx();
    /* 0x54 */ virtual void attackSensor(HitSensor*, HitSensor*);
    /* 0x58 */ virtual bool receiveMsgPush(HitSensor*, HitSensor*);
    /* 0x5C */ virtual bool receiveMsgPlayerAttack(u32, HitSensor*, HitSensor*);
    /* 0x74 */ virtual bool receiveOtherMsg(u32, HitSensor*, HitSensor*);

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
