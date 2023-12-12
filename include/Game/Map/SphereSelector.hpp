#pragma once

#include "Game/LiveActor/LiveActor.h"
#include "Game/LiveActor/LiveActorGroup.h"

class SphereSelector : public LiveActor {
public:
    SphereSelector();

    virtual ~SphereSelector();
    virtual void init(const JMapInfoIter &);
    virtual void appear();
    virtual void kill();


    void validatePointing();
    void invalidatePointing();
    void sendMsgToAllActor(u32);
    bool isMoveClickedPos() const;
    void playSelectedME();
    void playCanceledME();
    void exeConfirm();
    void exeConfirmCancel();
    void exeConfirmStart();
    void exeSelectCancel();
    void exeSelectWait();
    void exeSelectStart();

    LiveActorGroup* mSphereGroup;   // _8C

    bool mIsPointingInvalid;        // _B0
};

namespace NrvSphereSelector {
    NERVE_DECL(SphereSelectorNrvConfirmed, SphereSelector, SphereSelector::exeConfirm);
    NERVE_DECL(SphereSelectorNrvConfirmCancel, SphereSelector, SphereSelector::exeConfirmCancel);
    NERVE_DECL_NULL(SphereSelectorNrvConfirmWait);
    NERVE_DECL(SphereSelectorNrvConfirmStart, SphereSelector, SphereSelector::exeConfirmStart);
    NERVE_DECL(SphereSelectorNrvSelectCancel, SphereSelector, SphereSelector::exeSelectCancel);
    NERVE_DECL(SphereSelectorNrvSelectWait, SphereSelector, SphereSelector::exeSelectWait);
    NERVE_DECL(SphereSelectorNrvSelectStart, SphereSelector, SphereSelector::exeSelectStart);
};