#pragma once

#include "Game/NameObj/NameObj.h"
#include "Game/Scene/PlacementStateChecker.h"
#include "Game/Scene/PlacementInfoOrdered.h"
#include "Game/Util.h"

class StageDataHolder : public NameObj {
public:
    StageDataHolder(const char *, int, bool);

    virtual void init(const JMapInfoIter &);

    void initAfterScenarioSelected();
    void requestFileLoadCommon();
    void requestFileLoadScenario();
    void initPlacement();

    bool isPlacedZone(int) const;
    const char* getJapaneseObjectName(const char *) const;
    void* getStageArchiveResource(const char *);
    s32 getStageArchiveResourceSize(void *);

    void initPlacementMario();

    void initTableData();

    void calcDataAddress();
    void updateDataAddress(const MR::AssignableArray<JMapInfo> *);

    void initPlacementInfoOrderedScenario();
    void initPlacementInfoOrderedCommon();

    void createLocalStageDataHolder(const MR::AssignableArray<JMapInfo> &, bool);

    void initJmpInfo(MR::AssignableArray<JMapInfo> *, const char *);

    void initLayerJmpInfo(MR::AssignableArray<JMapInfo> *, const char *, const char *, u32);
    void initAllLayerJmpInfo(MR::AssignableArray<JMapInfo> *, const char *, const char *);
    void initAllLayerJmpInfo(MR::AssignableArray<JMapInfo> *, const char *);

    JMapInfo* findJmpInfoFromArray(const MR::AssignableArray<JMapInfo> *, const char *) const;
    JMapInfoIter getStartJMapInfoIterFromStartDataIndex(int) const;

    s32 getStartPosNum() const;

    void calcPlacementMtx(const JMapInfoIter &);

    MR::AssignableArray<JMapInfo> mPlacementObjs;   // _C
    MR::AssignableArray<JMapInfo> mStartObjs;       // _14
    MR::AssignableArray<JMapInfo> mGeneralPosObjs;  // _1C
    MR::AssignableArray<JMapInfo> mChildObjs;       // _24
    MR::AssignableArray<JMapInfo> mListObjs;        // _2C
    MR::AssignableArray<JMapInfo> mPathObjs;        // _34
    JMapInfo* mObjNameTbl;                          // _3C
    s32 mStageDataHolderCount;                      // _40
    StageDataHolder* mStageDataArray[0x18];         // _44
    JKRArchive* mArchive;                           // _A4
    const char* _A8;
    Mtx mPlacementMtx;                              // _AC
    s32 mZoneID;                                    // _DC
    u8 _E0;
    u8 _E1;
    u8 _E2;
    u8 _E3;
    s32 _E4;
    u32 _E8;
    MR::AssignableArray<JMapInfo> _EC;
    MR::AssignableArray<JMapInfo> _F4;
    PlacementInfoOrdered* _FC;
    PlacementInfoOrdered* _100;
    PlacementInfoOrdered* _104;
    PlacementInfoOrdered* _108;
    PlacementInfoOrdered* _10C;
};