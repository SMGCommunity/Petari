#pragma once

#include "Game/NameObj/NameObj.hpp"
#include "Game/Util/Array.hpp"
#include <JSystem/JGeometry.hpp>

class JKRArchive;
class JMapIdInfo;
class PlacementInfoOrdered;

class StageDataHolder : public NameObj {
public:
    StageDataHolder(const char* pStageName, int zoneID, bool loadCommon);

    virtual void init(const JMapInfoIter& rIter);

    void initAfterScenarioSelected();
    void requestFileLoadCommon();
    void requestFileLoadScenario();
    void initPlacement();
    JMapInfoIter getCommonPathPointInfo(const JMapInfo** ppInfo, int index) const;
    JMapInfoIter getCommonPathPointInfoFromRailDataIndex(const JMapInfo** ppInfo, int index) const;
    s32 getCommonPathInfoElementNum() const;
    s32 getStartPosNum() const;
    s32 getCurrentStartZoneId() const;
    s32 getCurrentStartCameraId() const;
    void getStartCameraIdInfoFromStartDataIndex(JMapIdInfo* pInfo, int index) const;
    s32 getGeneralPosNum() const;
    JMapInfoIter getGeneralPosInfoFromDataIndex(int index) const;
    s32 getChildObjNum(const JMapInfoIter& rIter) const;
    JMapInfoIter getChildObjInfoFromDataIndex(const JMapInfoIter& rIter, int index) const;

    JMapInfoIter makeMarioJMapInfoIter(const JMapIdInfo& rInfo) const;

    const StageDataHolder* findPlacedStageDataHolder(const JMapInfoIter& rIter) const;
    const StageDataHolder* getStageDataHolderFromZoneId(int zoneID) const;
    StageDataHolder* getStageDataHolderFromZoneId(int zoneID);
    bool isPlacedZone(int zoneID) const;
    const char* getJapaneseObjectName(const char* pName) const;
    void* getStageArchiveResource(const char* pName);
    s32 getStageArchiveResourceSize(void* pData);

    JMapInfoIter makeCurrentMarioJMapInfoIter() const;

    void initPlacementMario();

    void initTableData();

    void calcDataAddress();
    void updateDataAddress(const MR::AssignableArray< JMapInfo >* pInfoArray);

    void initPlacementInfoOrderedScenario();
    void initPlacementInfoOrderedCommon();

    void createLocalStageDataHolder(const MR::AssignableArray< JMapInfo >& rInfoArray, bool loadCommon);

    void initJmpInfo(MR::AssignableArray< JMapInfo >* pInfo, const char* pName);

    void initLayerJmpInfo(MR::AssignableArray< JMapInfo >* pInfo, const char* pPlacementDir, const char* pMapPartsDir, u32 layerMask);
    void initAllLayerJmpInfo(MR::AssignableArray< JMapInfo >* pInfo, const char* pPlacementDir, const char* pMapPartsDir);
    void initAllLayerJmpInfo(MR::AssignableArray< JMapInfo >* pInfo, const char* pName);

    const JMapInfo* findJmpInfoFromArray(const MR::AssignableArray< JMapInfo >* pInfo, const char* pName) const;
    JMapInfoIter getStartJMapInfoIterFromStartDataIndex(int index) const;

    void calcPlacementMtx(const JMapInfoIter& rIter);

    JMapInfo* attachJmpInfoToArray(JMapInfo* pInfo, const char* pDirectory);

    /* 0x0C */ MR::AssignableArray< JMapInfo > mPlacementObjs;
    /* 0x14 */ MR::AssignableArray< JMapInfo > mStartObjs;
    /* 0x1C */ MR::AssignableArray< JMapInfo > mGeneralPosObjs;
    /* 0x24 */ MR::AssignableArray< JMapInfo > mChildObjs;
    /* 0x2C */ MR::AssignableArray< JMapInfo > mListObjs;
    /* 0x34 */ MR::AssignableArray< JMapInfo > mPathObjs;
    /* 0x3C */ JMapInfo* mObjNameTbl;
    /* 0x40 */ s32 mStageDataHolderCount;
    /* 0x44 */ StageDataHolder* mStageDataArray[0x18];
    /* 0xA4 */ JKRArchive* mArchive;
    /* 0xA8 */ const char* _A8;
    /* 0xAC */ TPos3f mPlacementMtx;
    /* 0xDC */ s32 mZoneID;
    /* 0xE0 */ u8 _E0;
    /* 0xE1 */ u8 _E1;
    /* 0xE2 */ u8 _E2;
    /* 0xE3 */ u8 _E3;
    /* 0xE4 */ u32 _E4;
    /* 0xE8 */ u32 _E8;
    /* 0xEC */ MR::AssignableArray< JMapInfo > _EC;
    /* 0xF4 */ MR::AssignableArray< JMapInfo > _F4;
    /* 0xFC */ PlacementInfoOrdered* _FC;
    /* 0x100 */ PlacementInfoOrdered* _100;
    /* 0x104 */ PlacementInfoOrdered* _104;
    /* 0x108 */ PlacementInfoOrdered* _108;
    /* 0x10C */ PlacementInfoOrdered* _10C;
};

namespace MR {
    StageDataHolder* getStageDataHolder();
};  // namespace MR
