#pragma once

#include "Game/LiveActor/LiveActor.hpp"
#include "Game/LiveActor/LiveActorGroup.hpp"
#include "revolution/types.h"

class LiveActorGroup;
class SphereSelectorHandle;

enum HandlePointingPriority { Unknown_0, Unknown_1, Unknown_2 };

class SphereSelector : public LiveActor {
public:
    SphereSelector();

    virtual ~SphereSelector();
    virtual void init(const JMapInfoIter&);
    virtual void appear();
    virtual void kill();

    void registerPointingTarget(LiveActor*, HandlePointingPriority);
    void validatePointing();
    void invalidatePointing();
    void sendMsgToAllActor(u32);
    bool isMoveClickedPos() const;
    void playSelectedME();
    void playCanceledME();
    void exeSelectStart();
    void exeSelectWait();
    void exeSelectCancel();
    void exeConfirmStart();
    void exeConfirmWait();
    void exeConfirmCancel();
    void exeConfirm();

    LiveActorGroup* mSphereGroup;  // 0x8C

    bool mIsPointingInvalid;  // 0xB0
};

class SphereSelectorFunction {
public:
    static bool registerTarget(LiveActor*);
    static bool isPadButton();
    static s32 getSelectStartFrame();
    static s32 getConfirmStartCancelFrame();
    static void selectStart();
    static void selectCancel(bool);
    static void selectEnd();
    static void confirmStart();
    static void confirmCancel(bool);
    static void confirmed();
    static bool isSelectWait();
    static bool isConfirmWait();

    static bool isMsgSelectStart(u32);
    static bool isMsgSelectEnd(u32);
    static bool isMsgConfirmStart(u32);
    static bool isMsgConfirmCancel(u32);

    static bool isMsgConfirmed(u32);
    static bool isMsgTargetSelected(u32);
    static bool trySyncAppearMsgSelectStart(LiveActor*, u32);
    static bool trySyncKillMsgSelectStart(LiveActor*, u32);
    static void validatePointing();
    static void invalidatePointing();
    static bool isValidPointing();
    static void setHandle(SphereSelectorHandle*);
    static void calcHandledTrans(const TVec3f&, TVec3f*);
    static void calcHandledRotateMtx(const TVec3f&, TPos3f*);
    static TVec3f& getHandleTrans();
    static f32 getHandleRotateSpeed();
    static bool isHandleHolding();
    static void registerPointingTarget(LiveActor*, HandlePointingPriority);
    static bool tryRegisterPointingTarget(LiveActor*, HandlePointingPriority);
    static bool isPointingTarget();
    static bool isPointingTarget(const LiveActor*);
    static LiveActor* getPointingTarget();
    static LiveActor* getSelectedTarget();
    static TVec3f& getSelectedActorTrans();
    static void calcOffsetPos(TVec3f*, const TVec3f&, const TVec3f&, const TVec3f&, const TVec3f&);
};
