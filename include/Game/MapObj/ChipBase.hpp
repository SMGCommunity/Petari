#pragma once

#include "Game/LiveActor/LiveActor.hpp"
#include "Game/LiveActor/FlashingCtrl.hpp"
#include "Game/LiveActor/PartsModel.hpp"
#include "Game/MapObj/MapPartsRailMover.hpp"
#include "Game/NameObj/NameObjArchiveListCollector.hpp"

class ChipBase : public LiveActor {
public:
    ChipBase(const char *, s32, const char *);
    
    virtual ~ChipBase() {

    }

    virtual void init(const JMapInfoIter &);
    virtual void initAfterPlacement();
    virtual void makeActorAppeared();
    virtual void makeActorDead();
    virtual void control();
    virtual bool receiveOtherMsg(u32, HitSensor *, HitSensor *);

    void initModel(const JMapInfoIter &);
    void initSensor();
    void initShadow(const JMapInfoIter &);
    void initJMapParam(const JMapInfoIter &);
    void deactive();
    void setGroupID(s32);
    void setHost(LiveActor *);
    void appearWait();
    void appearFlashing(s32);
    bool requestGet(HitSensor *, HitSensor *);
    bool requestShow();
    bool requestHide();
    bool requestStartControl();
    bool requestEndControl();
    void exeFlashing();
    void exeGot();
    bool isGettable() const;
    static bool isNeedBubble(const JMapInfoIter &);
    static void makeArchiveList(NameObjArchiveListCollector *, const JMapInfoIter &);

    FlashingCtrl* mFlashingCtrl;    // _8C
    MapPartsRailMover* mRailMover;  // _90
    PartsModel* mAirBubble;         // _94
    const char* mChipName;          // _98
    LiveActor* mHost;               // _9C
    TVec3f mClippingRange;          // _A0
    s32 mGroupID;                   // _AC
    s32 mChipType;                  // _B0
    bool _B4;
    bool _B5;
};

namespace NrvChipBase {
    NERVE(ChipBaseNrvDeactive);
    NERVE(ChipBaseNrvWait);
    NERVE(ChipBaseNrvControled);
    NERVE(ChipBaseNrvFlashing);
    NERVE(ChipBaseNrvHide);
    NERVE(ChipBaseNrvGot);
};