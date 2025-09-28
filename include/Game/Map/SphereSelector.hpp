#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class LiveActorGroup;
class SphereSelectorHandle;

enum HandlePointingPriority {
    Unknown_0,
    Unknown_1,
    Unknown_2
};

class SphereSelector : public LiveActor {
public:
    SphereSelector();

    virtual ~SphereSelector();
    virtual void init(const JMapInfoIter &);
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

    LiveActorGroup* mSphereGroup;   // 0x8C

    bool mIsPointingInvalid;        // 0xB0
};

namespace SphereSelectorFunction {
    void registerTarget(LiveActor*);
    bool isPadButton();
    s32 getSelectStartFrame();
    s32 getConfirmStartCancelFrame();
    void selectStart();
    void selectCancel(bool);
    void selectEnd();
    void confirmStart();
    void confirmCancel(bool);
    void confirmed();
    bool isSelectWait();
    bool isConfirmWait();
    bool isMsgSelectStart(u32);
    bool isMsgSelectEnd(u32);
    bool isMsgConfirmStart(u32);
    bool isMsgConfirmCancel(u32);
    bool isMsgConfirmed(u32);
    bool isMsgTargetSelected(u32);
    bool trySyncAppearMsgSelectStart(LiveActor*, u32);
    bool trySyncKillMsgSelectStart(LiveActor*, u32);
    void validatePointing();
    void invalidatePointing();
    bool isValidPointing();
    void setHandle(SphereSelectorHandle*);
    void calcHandledTrans(const TVec3f&, TVec3f*);
    void calcHandledRotateMtx(const TVec3f&, TPos3f*);
    TVec3f* getHandleTrans();
    f32 getHandleRotateSpeed();
    bool isHandleHolding();
    void registerPointingTarget(LiveActor*, HandlePointingPriority);
    bool tryRegisterPointingTarget(LiveActor*, HandlePointingPriority);
    bool isPointingTarget();
    bool isPointingTarget(const LiveActor*);
    LiveActor* getPointingTarget();
    LiveActor* getSelectedTarget();
    TVec3f* getSelectedActorTrans();
    void calcOffsetPos(TVec3f*, const TVec3f&, const TVec3f&, const TVec3f&, const TVec3f&);
};
