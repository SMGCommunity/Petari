#pragma once

#include "Game/NameObj/NameObj.hpp"
#include "Game/Scene/PlacementInfoOrdered.hpp"
#include "Game/Scene/PlacementStateChecker.hpp"
#include "Game/Util.hpp"

class StageDataHolder : public NameObj {
public:
    StageDataHolder(const char*, int, bool);

    virtual void init(const JMapInfoIter&);

    void initAfterScenarioSelected();
    void requestFileLoadCommon();
    void requestFileLoadScenario();
    void initPlacement();
    JMapInfo getCommonPathPointInfo(const JMapInfo**, int) const;
    JMapInfo getCommonPathPointInfoFromRailDataIndex(const JMapInfo**, int) const;
    s32 getCommonPathInfoElementNum() const;
    s32 getStartPosNum() const;
    s32 getCurrentStartZoneId() const;
    s32 getCurrentStartCameraId() const;
    void getStartCameraIdInfoFromStartDataIndex(JMapIdInfo*, int) const;
    s32 getGeneralPosNum() const;

    const StageDataHolder* findPlacedStageDataHolder(const JMapInfoIter&) const;
    const StageDataHolder* getStageDataHolderFromZoneId(int) const;
    const StageDataHolder* getStageDataHolderFromZoneId(int);
    bool isPlacedZone(int) const;
    const char* getJapaneseObjectName(const char*) const;
    void* getStageArchiveResource(const char*);
    s32 getStageArchiveResourceSize(void*);

    JMapInfoIter makeCurrentMarioJMapInfoIter() const;

    void initPlacementMario();

    void initTableData();

    void calcDataAddress();
    void updateDataAddress(const MR::AssignableArray< JMapInfo >*);

    void initPlacementInfoOrderedScenario();
    void initPlacementInfoOrderedCommon();

    void createLocalStageDataHolder(const MR::AssignableArray< JMapInfo >&, bool);

    void initJmpInfo(MR::AssignableArray< JMapInfo >*, const char*);

    void initLayerJmpInfo(MR::AssignableArray< JMapInfo >*, const char*, const char*, u32);
    void initAllLayerJmpInfo(MR::AssignableArray< JMapInfo >*, const char*, const char*);
    void initAllLayerJmpInfo(MR::AssignableArray< JMapInfo >*, const char*);

    const JMapInfo* findJmpInfoFromArray(const MR::AssignableArray< JMapInfo >*, const char*) const;
    JMapInfoIter getStartJMapInfoIterFromStartDataIndex(int) const;

    void calcPlacementMtx(const JMapInfoIter&);

    MR::AssignableArray< JMapInfo > mPlacementObjs;   // 0xC
    MR::AssignableArray< JMapInfo > mStartObjs;       // 0x14
    MR::AssignableArray< JMapInfo > mGeneralPosObjs;  // 0x1C
    MR::AssignableArray< JMapInfo > mChildObjs;       // 0x24
    MR::AssignableArray< JMapInfo > mListObjs;        // 0x2C
    MR::AssignableArray< JMapInfo > mPathObjs;        // 0x34
    JMapInfo* mObjNameTbl;                            // 0x3C
    s32 mStageDataHolderCount;                        // 0x40
    StageDataHolder* mStageDataArray[0x18];           // 0x44
    JKRArchive* mArchive;                             // 0xA4
    const char* _A8;
    Mtx mPlacementMtx;  // 0xAC
    s32 mZoneID;        // 0xDC
    u8 _E0;
    u8 _E1;
    u8 _E2;
    u8 _E3;
    u32 _E4;
    u32 _E8;
    MR::AssignableArray< JMapInfo > _EC;
    MR::AssignableArray< JMapInfo > _F4;
    PlacementInfoOrdered* _FC;
    PlacementInfoOrdered* _100;
    PlacementInfoOrdered* _104;
    PlacementInfoOrdered* _108;
    PlacementInfoOrdered* _10C;
};

namespace MR {
    StageDataHolder* getStageDataHolder();
};
