#include "Game/NameObj/NameObjFactory.hpp"
#include "Game/Scene/PlacementInfoOrdered.hpp"
#include "Game/Scene/StageDataHolder.hpp"
#include "Game/Scene/StageResourceLoader.hpp"
#include "Game/System/ScenarioDataParser.hpp"
#include "JSystem/JKernel/JKRArchive.hpp"
#include <cstdio>

JMapInfoIter JMapInfo::end() const {
    return JMapInfoIter(this, getNumEntries());
}

namespace {
    static bool isPrioPlacementObjInfo(const char *pName) NO_INLINE {
        return MR::isEqualStringCase(pName, "AreaObjInfo") 
            || MR::isEqualStringCase(pName, "PlanetObjInfo")
            || MR::isEqualStringCase(pName, "DemoObjInfo")
            || MR::isEqualStringCase(pName, "CameraCubeInfo");
    }

    static void calcPlacementInfoNum(int *a1, int *a2, const MR::AssignableArray<JMapInfo> &rArray) NO_INLINE {
        *a1 = 0;
        *a2 = 0;

        for (const JMapInfo* pInfo = rArray.begin(); pInfo != rArray.end(); pInfo++) {
            if (::isPrioPlacementObjInfo(pInfo->getName())) {
                int size;
                
                if (pInfo->mData != nullptr) {
                    size = pInfo->mData->mNumEntries;
                }
                else {
                    size = 0;
                }

                *a1 += size;
            }
            else {
                int size;

                if (pInfo->mData != nullptr) {
                    size = pInfo->mData->mNumEntries;
                }
                else {
                    size = 0;
                }

                *a2 += size;
            }
        }
    }

    static void attachJmpInfoToPlacementInfoOrdered(PlacementInfoOrdered *a1, PlacementInfoOrdered *a2, PlacementInfoOrdered *a3, const MR::AssignableArray<JMapInfo> &rArray) NO_INLINE {
        for (const JMapInfo* pInfo = rArray.begin(); pInfo != rArray.end(); pInfo++) {
            if (::isPrioPlacementObjInfo(pInfo->getName())) {
                a1->attach(pInfo, nullptr);
            }
            else {
                a2->attach(pInfo, a3);
            }
        }
    }
};

void StageDataHolder::init(const JMapInfoIter &rIter) {
    if (!mZoneID) {
        initTableData();
    }

    if (_E0) {
        u32 commonLayer = ScenarioDataFunction::getCurrentCommonLayers(_A8);
        initLayerJmpInfo(&_EC, "/jmp/Placement", "/jmp/MapParts", commonLayer);
    }

    createLocalStageDataHolder(_EC, 1);

    if (!mZoneID) {
        initPlacementInfoOrderedCommon();
    }
}

void StageDataHolder::initAfterScenarioSelected() {
    s32 curScenarioNo = MR::getCurrentScenarioNo();
    u32 curLayers = ScenarioDataFunction::getCurrentScenarioLayers(_A8, curScenarioNo);

    if (!_E0) {
        curLayers |= ScenarioDataFunction::getCurrentCommonLayers(_A8);
    }

    initLayerJmpInfo(&_F4, "/jmp/Placement", "/jmp/MapParts", curLayers);
    createLocalStageDataHolder(_F4, false);

    for (s32 i = 0; i < mStageDataHolderCount; i++) {
        mStageDataArray[i]->initAfterScenarioSelected();
    }

    if (!mZoneID) {
        initPlacementInfoOrderedScenario();
    }

    initAllLayerJmpInfo(&mPlacementObjs, "/jmp/Placement", "/jmp/MapParts");
    initAllLayerJmpInfo(&mStartObjs, "/jmp/Start");

    s32 generalPosFileCount = mArchive->countFile("/jmp/GeneralPos");
    generalPosFileCount -= 2;
    s32 isValidGeneralPosCount = (generalPosFileCount > 0) ? generalPosFileCount : 0;

    if (isValidGeneralPosCount) {
        initAllLayerJmpInfo(&mGeneralPosObjs, "/jmp/GeneralPos");
    }

    s32 childObjFileCount = mArchive->countFile("/jmp/ChildObj");
    childObjFileCount -= 2;
    s32 isValidChildObjCount = (childObjFileCount > 0) ? childObjFileCount : 0;

    if (isValidChildObjCount) {
        initAllLayerJmpInfo(&mChildObjs, "/jmp/ChildObj");
    }

    initJmpInfo(&mListObjs, "/jmp/List");
    initJmpInfo(&mPathObjs, "/jmp/Path");
    calcDataAddress();
}

void StageDataHolder::requestFileLoadCommon() {
    if (!MR::tryRequestLoadStageResource()) {
        _FC->requestFileLoad();
        _100->requestFileLoad();
    }
}

void StageDataHolder::requestFileLoadScenario() {
    if (MR::isLoadStageScenarioResource()) {
        _104->requestFileLoad();
        _108->requestFileLoad();
        _10C->requestFileLoad();
    }
}

void StageDataHolder::initPlacement() {
    MR::setInitializeStatePlacementPlayer();
    initPlacementMario();
    MR::setInitializeStatePlacementHighPriority();
    _FC->initPlacement();
    _104->initPlacement();
    MR::setInitializeStatePlacement();
    _100->initPlacement();
    _108->initPlacement();
    _10C->initPlacement();
}

JMapInfo StageDataHolder::getCommonPathPointInfo(const JMapInfo **ppOut, int idx) const {
    const JMapInfo* pInfo = findJmpInfoFromArray(&mPathObjs, "CommonPathInfo");
    JMapInfoIter pathIter = pInfo->findElement<s32>("l_id", idx, 0);
    return getCommonPathPointInfoFromRailDataIndex(ppOut, pathIter.mIndex);
}

JMapInfo StageDataHolder::getCommonPathPointInfoFromRailDataIndex(const JMapInfo **ppInfo, int idx) const {
    const JMapInfo* pInfo = findJmpInfoFromArray(&mPathObjs, "CommonPathInfo");
    char buf[0x80];
    snprintf(buf, sizeof(buf), "CommonPathPointInfo.%d", idx);
    *ppInfo = findJmpInfoFromArray(&mPathObjs, buf);
    return *pInfo;
}

s32 StageDataHolder::getCurrentStartCameraId() const {
    JMapInfoIter marioIter = makeCurrentMarioJMapInfoIter();
    s32 cameraID;
    bool ret = marioIter.mInfo->getValue<s32>(marioIter.mIndex, "Camera_id", &cameraID);

    if (ret) {
        return cameraID;
    } 

    return -1;
}

void StageDataHolder::getStartCameraIdInfoFromStartDataIndex(JMapIdInfo *pInfo, int startDataIdx) const {
    JMapInfoIter copy;
    JMapInfoIter startIter = getStartJMapInfoIterFromStartDataIndex(startDataIdx);
    copy = startIter;
    s32 cameraID;
    copy.mInfo->getValue<s32>(startIter.mIndex, "Camera_id", &cameraID);
    pInfo->initalize(cameraID, copy);
}

const StageDataHolder* StageDataHolder::findPlacedStageDataHolder(const JMapInfoIter &rIter) const {
    s32 data = (s32)rIter.mInfo->mData + rIter.mInfo->mData->mDataOffset + rIter.mInfo->mData->mEntrySize * rIter.mIndex;

    if (_E4 <= data && data < _E8) {
        return this;
    }

    for (s32 i = 0; i < mStageDataHolderCount; i++) {
        const StageDataHolder* pHolder = mStageDataArray[i]->findPlacedStageDataHolder(rIter);

        if (pHolder != nullptr) {
            return pHolder; 
        }
    }

    return nullptr;
}

const StageDataHolder* StageDataHolder::getStageDataHolderFromZoneId(int zoneID) const {
    if (zoneID == 0) {
        return this;
    }

    for (s32 i = 0; i < mStageDataHolderCount; i++) {
        StageDataHolder* pHolder = mStageDataArray[i];
        s32 curZoneID = pHolder->mZoneID;

        if (zoneID == curZoneID) {
            return pHolder;
        }
    }

    return nullptr;
}

const StageDataHolder* StageDataHolder::getStageDataHolderFromZoneId(int zoneID) {
    return getStageDataHolderFromZoneId(zoneID);
}

bool StageDataHolder::isPlacedZone(int zoneID) const {
    if (!zoneID) {
        return true;
    }

    for (s32 i = 0; i < mStageDataHolderCount; i++) {
        if (zoneID != mStageDataArray[i]->mZoneID) {
            return true;
        }
    }

    return false;
}

const char* StageDataHolder::getJapaneseObjectName(const char *pName) const {
    const JMapInfoIter englishName = mObjNameTbl->findElement<const char *>("en_name", pName, 0);

    if (englishName == mObjNameTbl->end()) {
        return 0;
    }

    const char* japaneseName;
    englishName.mInfo->getValue<const char *>(englishName.mIndex, "jp_name", &japaneseName);
    return japaneseName;
}

void* StageDataHolder::getStageArchiveResource(const char *pName) {
    return mArchive->getResource(0x3F3F3F3F, pName);
}

s32 StageDataHolder::getStageArchiveResourceSize(void *pData) {
    return mArchive->getResSize(pData);
}

void StageDataHolder::initPlacementMario() {
    JMapInfoIter iter = makeCurrentMarioJMapInfoIter();
    MR::setCurrentPlacementZoneId(MR::getPlacedZoneId(iter));
    const char* objName = "";
    MR::getObjectName(&objName, iter);
    CreationFuncPtr funcPtr = NameObjFactory::getCreator(objName);

    NameObj* obj = funcPtr("マリオアクター"); 
    obj->init(iter);
    MR::clearCurrentPlacementZoneId();
}

void StageDataHolder::initTableData() {
    JKRArchive* archive = (JKRArchive*)MR::receiveArchive("/StageData/ObjTableTable.arc");
    void* tableFile = archive->getResource(0x3F3F3F3F, "ObjNameTable.tbl");

    mObjNameTbl = new JMapInfo();
    mObjNameTbl->attach(tableFile);
}

void StageDataHolder::initPlacementInfoOrderedCommon() {
    int v12, v11;
    ::calcPlacementInfoNum(&v12, &v11, _EC);

    s32 v2 = 0;
    s32 v3 = 0;

    for (s32 i = 0; i < mStageDataHolderCount; i++) {
        int v10, v9;
        ::calcPlacementInfoNum(&v10, &v9, mStageDataArray[i]->_EC);
        v12 += v10;
        v11 += v9;
    }

    _FC = new PlacementInfoOrdered(v12);
    _100 = new PlacementInfoOrdered(v11);
    _10C = new PlacementInfoOrdered(0x20);
    ::attachJmpInfoToPlacementInfoOrdered(_FC, _100, _10C, _EC);

    s32 v7 = 0;
    s32 v8 = 0;

    for (s32 i = 0; i < mStageDataHolderCount; i++) {
        ::attachJmpInfoToPlacementInfoOrdered(_FC, _100, _10C, mStageDataArray[i]->_EC);
        i++;
    }

    _FC->sort();
    _100->sort();
}

const JMapInfo* StageDataHolder::findJmpInfoFromArray(const MR::AssignableArray<JMapInfo> *pInfoArr, const char *pName) const {
    for (const JMapInfo* pInfo = pInfoArr->begin(); pInfo != pInfoArr->end(); pInfo++) {
        if (MR::isEqualStringCase(pInfo->getName(), pName)) {
            return pInfo;
        }
    }

    return nullptr;
}

JMapInfoIter StageDataHolder::getStartJMapInfoIterFromStartDataIndex(int idx_) const {
    int idx = idx_;
    
    for (JMapInfo* pInfo = mStartObjs.mArr; pInfo != mStartObjs.end(); pInfo++) {
        const JMapData* curData = pInfo->mData;
        bool isValid = curData;
        int curIdx = isValid ? curData->mNumEntries : 0;
    
        if (idx < curIdx) {
            JMapInfoIter iter;
            iter.mInfo = pInfo;
            iter.mIndex = idx;

            return iter;
        }

        curIdx = isValid ? curData->mNumEntries : 0;

        idx -= curIdx;
    }

    for (s32 i = 0; i < mStageDataHolderCount; i++) {
        StageDataHolder* pHolder = mStageDataArray[i];
        int startPosNum = pHolder->getStartPosNum();

        if (idx < startPosNum) {
            return pHolder->getStartJMapInfoIterFromStartDataIndex(idx);
        }

        idx -= startPosNum;
    }

    JMapInfoIter iter;
    iter.mInfo = nullptr;
    iter.mIndex = -1;

    return iter;
}

void StageDataHolder::calcDataAddress() {
    _E4 = -1;
    _E8 = 0;
    updateDataAddress(&mPlacementObjs);
    updateDataAddress(&mStartObjs);
    updateDataAddress(&mGeneralPosObjs);
    updateDataAddress(&mChildObjs);
    updateDataAddress(&mPathObjs);
}

void StageDataHolder::calcPlacementMtx(const JMapInfoIter &rIter) {
    TVec3f pos;
    rIter.getValue<f32>("pos_x", &pos.x);
    rIter.getValue<f32>("pos_y", &pos.y);
    rIter.getValue<f32>("pos_z", &pos.z);

    TVec3f rot;
    rIter.getValue<f32>("dir_x", &rot.x);
    rIter.getValue<f32>("dir_y", &rot.y);
    rIter.getValue<f32>("dir_z", &rot.z);

    MR::makeMtxTR(mPlacementMtx, pos, rot);
}

void StageDataHolder::updateDataAddress(const MR::AssignableArray<JMapInfo> *pInfoArray) {
    for (const JMapInfo* pInfo = pInfoArray->begin(); pInfo != pInfoArray->end(); pInfo++) {
        if ((u32)pInfo->mData < _E4) {
            _E4 = (u32)pInfo->mData;
        }

        u32 addr = (pInfo->mData->mEntrySize * pInfo->mData->mNumEntries) + ((s32)pInfo->mData + pInfo->mData->mDataOffset);

        if (_E8 < addr) {
            _E8 = addr;
        }
    }
}
