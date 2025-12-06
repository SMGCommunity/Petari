#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class FlashingCtrl;
class MapPartsRailMover;
class NameObjArchiveListCollector;
class PartsModel;

class ChipBase : public LiveActor {
public:
    ChipBase(const char*, s32, const char*);

    virtual ~ChipBase() {}

    virtual void init(const JMapInfoIter&);
    virtual void initAfterPlacement();
    virtual void makeActorAppeared();
    virtual void makeActorDead();
    virtual void control();
    virtual bool receiveOtherMsg(u32, HitSensor*, HitSensor*);

    void initModel(const JMapInfoIter&);
    void initSensor();
    void initShadow(const JMapInfoIter&);
    void initJMapParam(const JMapInfoIter&);
    void deactive();
    void setGroupID(s32);
    void setHost(LiveActor*);
    void appearWait();
    void appearFlashing(s32);
    bool requestGet(HitSensor*, HitSensor*);
    bool requestShow();
    bool requestHide();
    bool requestStartControl();
    bool requestEndControl();
    void exeDeactive();
    void exeWait();
    void exeControled();
    void exeFlashing();
    void exeHide();
    void exeGot();
    bool isGettable() const;
    static bool isNeedBubble(const JMapInfoIter&);
    static void makeArchiveList(NameObjArchiveListCollector*, const JMapInfoIter&);

    FlashingCtrl* mFlashingCtrl;    // 0x8C
    MapPartsRailMover* mRailMover;  // 0x90
    PartsModel* mAirBubble;         // 0x94
    const char* mChipName;          // 0x98
    LiveActor* mHost;               // 0x9C
    TVec3f mClippingRange;          // 0xA0
    s32 mGroupID;                   // 0xAC
    s32 mChipType;                  // 0xB0
    bool _B4;
    bool _B5;
};
