#include "Game/Scene/StageDataHolder.hpp"
#include "Game/NameObj/NameObjFactory.hpp"
#include "Game/Scene/PlacementInfoOrdered.hpp"
#include "Game/Scene/SceneObjHolder.hpp"
#include "Game/Scene/StageResourceLoader.hpp"
#include "Game/System/GalaxyStatusAccessor.hpp"
#include "Game/System/ScenarioDataParser.hpp"
#include "Game/Util/FileUtil.hpp"
#include "Game/Util/JMapIdInfo.hpp"
#include "Game/Util/JMapUtil.hpp"
#include "Game/Util/MemoryUtil.hpp"
#include "Game/Util/MtxUtil.hpp"
#include "Game/Util/SceneUtil.hpp"
#include "Game/Util/StringUtil.hpp"
#include <JSystem/JKernel/JKRFileFinder.hpp>
#include <JSystem/JKernel/JKRMemArchive.hpp>
#include <cstdio>

template JMapInfoIter JMapInfo::findElement< s32 >(const char*, s32, int) const;
template JMapInfoIter JMapInfo::findElement< const char* >(const char*, const char*, int) const;

// for some reason these two strings only emit here
// shrug
void StageDataHolder_FORCE_MATCH_JMAP(const JMapInfo* pInfo) {
    pInfo->findElement< s32 >("", 0, 0);
    pInfo->findElement< const char* >("", "", 0);
    pInfo->end() == pInfo->end();
}

namespace {
    static const char* cLayerDirName[0x11] = {"Common", "LayerA", "LayerB", "LayerC", "LayerD", "LayerE", "LayerF", "LayerG", "LayerH",
                                              "LayerI", "LayerJ", "LayerK", "LayerL", "LayerM", "LayerN", "LayerO", "LayerP"};

    static bool isPrioPlacementObjInfo(const char* pName) NO_INLINE {
        return MR::isEqualStringCase(pName, "AreaObjInfo") || MR::isEqualStringCase(pName, "PlanetObjInfo") ||
               MR::isEqualStringCase(pName, "DemoObjInfo") || MR::isEqualStringCase(pName, "CameraCubeInfo");
    }

    u32 getAllLayers(const char* pStageName, s32 scenarioNo) {
        u32 commonLayers = ScenarioDataFunction::getCurrentCommonLayers(pStageName);
        u32 scenarioLayers = ScenarioDataFunction::getCurrentScenarioLayers(pStageName, scenarioNo);
        return commonLayers | scenarioLayers;
    }

    static void calcPlacementInfoNum(int* pPriorityCount, int* pNormalCount, const MR::AssignableArray< JMapInfo >& rArray) NO_INLINE {
        *pPriorityCount = 0;
        *pNormalCount = 0;

        for (const JMapInfo* pInfo = rArray.begin(); pInfo != rArray.end(); pInfo++) {
            if (::isPrioPlacementObjInfo(pInfo->getName())) {
                int size;

                if (pInfo->mData != nullptr) {
                    size = pInfo->mData->mNumEntries;
                } else {
                    size = 0;
                }

                *pPriorityCount += size;
            } else {
                int size;

                if (pInfo->mData != nullptr) {
                    size = pInfo->mData->mNumEntries;
                } else {
                    size = 0;
                }

                *pNormalCount += size;
            }
        }
    }

    static void attachJmpInfoToPlacementInfoOrdered(PlacementInfoOrdered* pPriorityInfo, PlacementInfoOrdered* pNormalInfo,
                                                    PlacementInfoOrdered* pDeferredInfo, const MR::AssignableArray< JMapInfo >& rArray) NO_INLINE {
        for (const JMapInfo* pInfo = rArray.begin(); pInfo != rArray.end(); pInfo++) {
            if (::isPrioPlacementObjInfo(pInfo->getName())) {
                pPriorityInfo->attach(pInfo, nullptr);
            } else {
                pNormalInfo->attach(pInfo, pDeferredInfo);
            }
        }
    }
};  // namespace

StageDataHolder::StageDataHolder(const char* pStageName, int zoneID, bool loadCommon)
    : NameObj("StageDataHolder"), mObjNameTbl(), mStageDataHolderCount(), mArchive(MR::getStageArchive(pStageName)), _A8(pStageName), mZoneID(zoneID),
      _E0(loadCommon), _E4(), _E8(), _FC(), _100(), _104(), _108(), _10C() {
    MR::zeroMemory(mStageDataArray, sizeof(mStageDataArray));
    mPlacementMtx.identity();
}

void StageDataHolder::init(const JMapInfoIter& rIter) {
    if (!mZoneID) {
        initTableData();
    }

    if (_E0) {
        u32 commonLayer = ScenarioDataFunction::getCurrentCommonLayers(_A8);
        initLayerJmpInfo(&_EC, "/jmp/Placement", "/jmp/MapParts", commonLayer);
    }

    createLocalStageDataHolder(_EC, true);

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

    if (isValidGeneralPosCount > 0) {
        initAllLayerJmpInfo(&mGeneralPosObjs, "/jmp/GeneralPos");
    }

    s32 childObjFileCount = mArchive->countFile("/jmp/ChildObj");
    childObjFileCount -= 2;
    s32 isValidChildObjCount = (childObjFileCount > 0) ? childObjFileCount : 0;

    if (isValidChildObjCount > 0) {
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

JMapInfoIter StageDataHolder::getCommonPathPointInfo(const JMapInfo** ppOut, int idx) const {
    const JMapInfo* pInfo = findJmpInfoFromArray(&mPathObjs, "CommonPathInfo");
    JMapInfoIter pathIter = pInfo->findElement< s32 >("l_id", idx, 0);
    return getCommonPathPointInfoFromRailDataIndex(ppOut, pathIter.mIndex);
}

JMapInfoIter StageDataHolder::getCommonPathPointInfoFromRailDataIndex(const JMapInfo** ppInfo, int idx) const {
    const JMapInfo* pInfo = findJmpInfoFromArray(&mPathObjs, "CommonPathInfo");
    char buf[128];
    snprintf(buf, sizeof(buf), "CommonPathPointInfo.%d", idx);
    *ppInfo = findJmpInfoFromArray(&mPathObjs, buf);
    return JMapInfoIter(pInfo, idx);
}

void StageDataHolder_FORCE_MATCH_DATA(const char* pName) {
    MR::isEqualStringCase(pName, "StageInfo");
    MR::isEqualStringCase(pName, "MultiFileName");
    MR::isEqualStringCase(pName, "All_mul");
    MR::isEqualStringCase(pName, "ChangeSceneListInfo");
    MR::isEqualStringCase(pName, "SceneNo");
}

s32 StageDataHolder::getCommonPathInfoElementNum() const {
    return findJmpInfoFromArray(&mPathObjs, "CommonPathInfo")->getNumEntries();
}

s32 StageDataHolder::getStartPosNum() const {
    s32 cur = 0;

    for (const JMapInfo* i = &mStartObjs[0]; i != mStartObjs.end(); i++) {
        cur += i->getNumEntries();
    }

    for (s32 i = 0; i < mStageDataHolderCount; i++) {
        cur += mStageDataArray[i]->getStartPosNum();
    }

    return cur;
}

s32 StageDataHolder::getCurrentStartZoneId() const {
    return findPlacedStageDataHolder(makeCurrentMarioJMapInfoIter())->mZoneID;
}

s32 StageDataHolder::getCurrentStartCameraId() const {
    JMapInfoIter marioIter = makeCurrentMarioJMapInfoIter();
    s32 cameraID;
    bool ret = marioIter.getValue< s32 >("Camera_id", &cameraID);

    if (ret) {
        return cameraID;
    }

    return -1;
}

void StageDataHolder::getStartCameraIdInfoFromStartDataIndex(JMapIdInfo* pInfo, int startDataIdx) const {
    JMapInfoIter startIter = getStartJMapInfoIterFromStartDataIndex(startDataIdx);
    s32 cameraID;
    startIter.getValue< s32 >("Camera_id", &cameraID);
    pInfo->initialize(cameraID, startIter);
}

s32 StageDataHolder::getGeneralPosNum() const {
    s32 count = 0;

    if (mGeneralPosObjs.size() > 0) {
        for (const JMapInfo* pInfo = &mGeneralPosObjs[0]; pInfo != mGeneralPosObjs.end(); pInfo++) {
            count += pInfo->getNumEntries();
        }
    }

    for (s32 i = 0; i < mStageDataHolderCount; i++) {
        count += mStageDataArray[i]->getGeneralPosNum();
    }

    return count;
}

JMapInfoIter StageDataHolder::getGeneralPosInfoFromDataIndex(int idx_) const {
    int idx = idx_;

    for (JMapInfo* pInfo = mGeneralPosObjs.mArr; pInfo != mGeneralPosObjs.end(); pInfo++) {
        const JMapData* curData = pInfo->mData;
        bool isValid = curData;
        int curIdx = isValid ? curData->mNumEntries : 0;

        if (idx < curIdx) {
            return JMapInfoIter(pInfo, idx);
        }

        curIdx = isValid ? curData->mNumEntries : 0;

        idx -= curIdx;
    }

    for (s32 i = 0; i < mStageDataHolderCount; i++) {
        StageDataHolder* pHolder = mStageDataArray[i];
        int startPosNum = pHolder->getGeneralPosNum();

        if (idx < startPosNum) {
            return pHolder->getGeneralPosInfoFromDataIndex(idx);
        }

        idx -= startPosNum;
    }

    return JMapInfoIter();
}

s32 StageDataHolder::getChildObjNum(const JMapInfoIter& rIter) const {
    s32 linkID;
    s32 count = 0;
    MR::getJMapInfoLinkID(rIter, &linkID);

    if (mChildObjs.size() > 0) {
        for (const JMapInfo* pInfo = mChildObjs.begin(); pInfo != mChildObjs.end(); pInfo++) {
            for (s32 i = 0; i < pInfo->getNumEntries(); i++) {
                s32 parentID;
                pInfo->getValue(i, "ParentID", &parentID);
                if (parentID == linkID) {
                    count++;
                }
            }
        }
    }

    return count;
}

JMapInfoIter StageDataHolder::getChildObjInfoFromDataIndex(const JMapInfoIter& rIter, int index) const {
    s32 linkID;
    s32 count = 0;
    MR::getJMapInfoLinkID(rIter, &linkID);

    for (const JMapInfo* pInfo = mChildObjs.begin(); pInfo != mChildObjs.end(); pInfo++) {
        for (s32 i = 0; i < pInfo->getNumEntries(); i++) {
            s32 parentID;
            pInfo->getValue(i, "ParentID", &parentID);
            if (parentID == linkID) {
                if (count == index) {
                    return JMapInfoIter(pInfo, i);
                }

                count++;
            }
        }
    }

    return JMapInfoIter();
}

const StageDataHolder* StageDataHolder::findPlacedStageDataHolder(const JMapInfoIter& rIter) const {
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

StageDataHolder* StageDataHolder::getStageDataHolderFromZoneId(int zoneID) {
    return const_cast< StageDataHolder* >(static_cast< const StageDataHolder* >(this)->getStageDataHolderFromZoneId(zoneID));
}

bool StageDataHolder::isPlacedZone(int zoneID) const {
    if (!zoneID) {
        return true;
    }

    for (s32 i = 0; i < mStageDataHolderCount; i++) {
        if (zoneID == mStageDataArray[i]->mZoneID) {
            return true;
        }
    }

    return false;
}

const char* StageDataHolder::getJapaneseObjectName(const char* pName) const {
    const JMapInfoIter englishName = mObjNameTbl->findElement< const char* >("en_name", pName, 0);

    if (englishName == mObjNameTbl->end()) {
        return nullptr;
    }

    const char* japaneseName;
    englishName.getValue< const char* >("jp_name", &japaneseName);
    return japaneseName;
}

void* StageDataHolder::getStageArchiveResource(const char* pName) {
    return mArchive->getResource('????', pName);
}

s32 StageDataHolder::getStageArchiveResourceSize(void* pData) {
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

JMapInfoIter StageDataHolder::makeMarioJMapInfoIter(const JMapIdInfo& rInfo) const {
    const StageDataHolder* hldr = getStageDataHolderFromZoneId(rInfo.mZoneID);
    s32 id = rInfo._0;
    const JMapInfo* i = hldr->mStartObjs.begin();

    while (i != hldr->mStartObjs.end()) {
        JMapInfoIter iter = i->findElement< s32 >("MarioNo", id, 0);
        bool res = !(iter == i->end());

        if (res) {
            return iter;
        }

        i++;
    }

    return JMapInfoIter();
}

JMapInfoIter StageDataHolder::makeCurrentMarioJMapInfoIter() const {
    JMapIdInfo startID = MR::getCurrentMarioStartIdInfo();
    JMapInfoIter iter = makeMarioJMapInfoIter(startID);

    if (iter.isValid()) {
        return iter;
    }

    return JMapInfoIter();
}

void StageDataHolder::initJmpInfo(MR::AssignableArray< JMapInfo >* pInfo, const char* pName) {
    s32 fileCount = mArchive->countFile(pName) - 2;
    s32 count = fileCount > 0 ? fileCount : 0;

    if (count != 0) {
        JMapInfo* pInfoArray = new JMapInfo[count];
        pInfo->mArr = pInfoArray;
        pInfo->mMaxSize = count;
        attachJmpInfoToArray(pInfoArray, pName);
    }
}

void StageDataHolder::initAllLayerJmpInfo(MR::AssignableArray< JMapInfo >* pInfo, const char* pName) {
    s32 count = 0;
    u32 mask = getAllLayers(_A8, MR::getCurrentScenarioNo());
    char buf[64];

    for (u32 i = 0; i < 0x11; i++) {
        if ((mask & (1 << i)) != 0) {
            snprintf(buf, sizeof(buf), "%s/%s", pName, cLayerDirName[i]);
            s32 fileCount = mArchive->countFile(buf) - 2;
            count += fileCount > 0 ? fileCount : 0;
        }
    }

    if (count != 0) {
        JMapInfo* pInfoArray = new JMapInfo[count];
        pInfo->mMaxSize = count;
        pInfo->mArr = pInfoArray;

        for (u32 i = 0; i < 0x11; i++) {
            if ((mask & (1 << i)) != 0) {
                snprintf(buf, sizeof(buf), "%s/%s", pName, cLayerDirName[i]);
                pInfoArray = attachJmpInfoToArray(pInfoArray, buf);
            }
        }
    }
}

void StageDataHolder::initAllLayerJmpInfo(MR::AssignableArray< JMapInfo >* pInfo, const char* pPlacementDir, const char* pMapPartsDir) {
    u32 mask = getAllLayers(_A8, MR::getCurrentScenarioNo());
    initLayerJmpInfo(pInfo, pPlacementDir, pMapPartsDir, mask);
}

void StageDataHolder::initLayerJmpInfo(MR::AssignableArray< JMapInfo >* pInfo, const char* pPlacementDir, const char* pMapPartsDir, u32 layerMask) {
    s32 count = 0;
    char path[64];

    for (u32 i = 0; i < 0x11; i++) {
        if (layerMask & (1 << i)) {
            snprintf(path, sizeof(path), "%s/%s", pPlacementDir, cLayerDirName[i]);
            s32 fileCount = mArchive->countFile(path) - 2;
            count += fileCount > 0 ? fileCount : 0;
            snprintf(path, sizeof(path), "%s/%s", pMapPartsDir, cLayerDirName[i]);
            fileCount = mArchive->countFile(path) - 2;
            count += fileCount > 0 ? fileCount : 0;
        }
    }

    if (count != 0) {
        JMapInfo* pCurrent = new JMapInfo[count];
        pInfo->mMaxSize = count;
        pInfo->mArr = pCurrent;
        for (u32 i = 0; i < 0x11; i++) {
            if (layerMask & (1 << i)) {
                snprintf(path, sizeof(path), "%s/%s", pPlacementDir, cLayerDirName[i]);
                pCurrent = attachJmpInfoToArray(pCurrent, path);
                snprintf(path, sizeof(path), "%s/%s", pMapPartsDir, cLayerDirName[i]);
                pCurrent = attachJmpInfoToArray(pCurrent, path);
            }
        }
    }
}

JMapInfo* StageDataHolder::attachJmpInfoToArray(JMapInfo* pInfo, const char* pDirectory) {
    s32 fileCount = mArchive->countFile(pDirectory) - 2;
    s32 count = fileCount > 0 ? fileCount : 0;

    if (count == 0) {
        return pInfo;
    }

    JKRArcFinder* pFinder = mArchive->getFirstFile(pDirectory);

    for (u32 i = 0; i < count; i++, pInfo++, pFinder->findNextFile()) {
        pInfo->attach(mArchive->getIdxResource(pFinder->mDirIndex));
        pInfo->setName(pFinder->mName);
    }

    delete pFinder;
    return pInfo;
}

void StageDataHolder::initTableData() {
    void* tableFile = MR::receiveArchive("/StageData/ObjNameTable.arc")->getResource('????', "ObjNameTable.tbl");

    mObjNameTbl = new JMapInfo();
    mObjNameTbl->attach(tableFile);
}

void StageDataHolder::initPlacementInfoOrderedCommon() {
    int priorityCount, normalCount;
    ::calcPlacementInfoNum(&priorityCount, &normalCount, _EC);

    for (s32 i = 0; i < mStageDataHolderCount; i++) {
        int localPriorityCount, localNormalCount;
        ::calcPlacementInfoNum(&localPriorityCount, &localNormalCount, mStageDataArray[i]->_EC);
        priorityCount += localPriorityCount;
        normalCount += localNormalCount;
    }

    _FC = new PlacementInfoOrdered(priorityCount);
    _100 = new PlacementInfoOrdered(normalCount);
    _10C = new PlacementInfoOrdered(0x20);
    ::attachJmpInfoToPlacementInfoOrdered(_FC, _100, _10C, _EC);

    for (s32 i = 0; i < mStageDataHolderCount; i++) {
        ::attachJmpInfoToPlacementInfoOrdered(_FC, _100, _10C, mStageDataArray[i]->_EC);
    }

    _FC->sort();
    _100->sort();
}

void StageDataHolder::initPlacementInfoOrderedScenario() {
    int priorityCount, normalCount;
    ::calcPlacementInfoNum(&priorityCount, &normalCount, _F4);

    for (s32 i = 0; i < mStageDataHolderCount; i++) {
        int localPriorityCount, localNormalCount;
        ::calcPlacementInfoNum(&localPriorityCount, &localNormalCount, mStageDataArray[i]->_F4);
        priorityCount += localPriorityCount;
        normalCount += localNormalCount;
    }

    _104 = new PlacementInfoOrdered(priorityCount);
    _108 = new PlacementInfoOrdered(normalCount);
    ::attachJmpInfoToPlacementInfoOrdered(_104, _108, nullptr, _F4);

    for (s32 i = 0; i < mStageDataHolderCount; i++) {
        ::attachJmpInfoToPlacementInfoOrdered(_104, _108, nullptr, mStageDataArray[i]->_F4);
    }

    _104->sort();
    _108->sort();
    _10C->sort();
}

const JMapInfo* StageDataHolder::findJmpInfoFromArray(const MR::AssignableArray< JMapInfo >* pInfoArr, const char* pName) const {
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
            return JMapInfoIter(pInfo, idx);
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

    return JMapInfoIter();
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

void StageDataHolder::updateDataAddress(const MR::AssignableArray< JMapInfo >* pInfoArray) {
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

void StageDataHolder::createLocalStageDataHolder(const MR::AssignableArray< JMapInfo >& rInfoArray, bool loadCommon) {
    for (const JMapInfo* pInfo = rInfoArray.begin(); pInfo != rInfoArray.end(); pInfo++) {
        if (MR::isEqualStringCase(pInfo->getName(), "StageObjInfo")) {
            for (s32 i = 0; i < pInfo->getNumEntries(); i++) {
                JMapInfoIter iter(pInfo, i);
                const char* pStageName = "";
                MR::getObjectName(&pStageName, iter);
                s32 zoneID = MR::makeCurrentGalaxyStatusAccessor().getZoneId(pStageName);
                mStageDataArray[mStageDataHolderCount] = new StageDataHolder(pStageName, zoneID, loadCommon);
                mStageDataArray[mStageDataHolderCount]->initWithoutIter();
                mStageDataArray[mStageDataHolderCount]->calcPlacementMtx(iter);
                mStageDataHolderCount++;
            }
        }
    }
}

void StageDataHolder::calcPlacementMtx(const JMapInfoIter& rIter) {
    TVec3f pos;
    rIter.getValue< f32 >("pos_x", &pos.x);
    rIter.getValue< f32 >("pos_y", &pos.y);
    rIter.getValue< f32 >("pos_z", &pos.z);

    TVec3f rot;
    rIter.getValue< f32 >("dir_x", &rot.x);
    rIter.getValue< f32 >("dir_y", &rot.y);
    rIter.getValue< f32 >("dir_z", &rot.z);

    MR::makeMtxTR(mPlacementMtx, pos, rot);
}

namespace MR {
    StageDataHolder* getStageDataHolder() {
        return getSceneObj< StageDataHolder >(SceneObj_StageDataHolder);
    }
};  // namespace MR
