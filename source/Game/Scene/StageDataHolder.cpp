#include "Game/Scene/StageDataHolder.h"
#include "Game/Scene/StageResourceLoader.h"
#include "Game/System/ScenarioDataParser.h"
#include "JSystem/JKernel/JKRArchive.h"

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

#ifdef NON_MATCHING
JMapInfo StageDataHolder::getCommonPathPointInfo(const JMapInfo **pOut, int idx) const {
    JMapInfo* inf = findJmpInfoFromArray(&mPathObjs, "CommonPathInfo");
    JMapInfoIter pathIter = inf->findElement<s32>("l_id", idx, 0);
    return getCommonPathPointInfoFromRailDataIndex(pOut, pathIter._4);
}
#endif

s32 StageDataHolder::getCurrentStartCameraId() const {
    JMapInfoIter marioIter = makeCurrentMarioJMapInfoIter();
    s32 cameraID;
    bool ret = marioIter.mInfo->getValue<s32>(marioIter._4, "Camera_id", &cameraID);

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
    copy.mInfo->getValue<s32>(startIter._4, "Camera_id", &cameraID);
    pInfo->initalize(cameraID, copy);
}

#ifdef NON_MATCHING
const StageDataHolder* StageDataHolder::findPlacedStageDataHolder(const JMapInfoIter &rIter) const {
    s32 data = (s32)rIter.mInfo->mData + rIter.mInfo->mData->mDataOffset + rIter.mInfo->mData->_C * rIter._4;

    if (_E4 > data || data >= _E8) {
        for (s32 i = 0; i < mStageDataHolderCount; i++) {
            const StageDataHolder* holder = mStageDataArray[i]->findPlacedStageDataHolder(rIter);

            if (holder) {
                return holder;
            }
        }

        return 0;
    }

    return this;
}
#endif

const StageDataHolder* StageDataHolder::getStageDataHolderFromZoneId(int zoneID) const {
    if (zoneID == 0) {
        return this;
    }

    for (s32 i = 0; i < mStageDataHolderCount; i++) {
        StageDataHolder* holder = mStageDataArray[i];
        s32 curZoneID = holder->mZoneID;

        if (zoneID == curZoneID) {
            return holder;
        }
    }

    return 0;
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

#ifdef NON_MATCHING
// stack nonsense
const char* StageDataHolder::getJapaneseObjectName(const char *pName) const {
    const JMapInfoIter englishName = mObjNameTbl->findElement<const char *>("en_name", pName, 0);
    const JMapInfoIter copy = englishName;
    const JMapInfoIter last = mObjNameTbl->end();

    if (copy == last) {
        return 0;
    }

    const char* japaneseName;
    last.mInfo->getValue<const char *>(englishName._4, "jp_name", &japaneseName);
    return japaneseName;
}
#endif

void* StageDataHolder::getStageArchiveResource(const char *pName) {
    return mArchive->getResource(0x3F3F3F3F, pName);
}

s32 StageDataHolder::getStageArchiveResourceSize(void *pData) {
    return mArchive->getResSize(pData);
}

void StageDataHolder::initTableData() {
    JKRArchive* archive = (JKRArchive*)MR::receiveArchive("/StageData/ObjTableTable.arc");
    void* tableFile = archive->getResource(0x3F3F3F3F, "ObjNameTable.tbl");

    mObjNameTbl = new JMapInfo();
    mObjNameTbl->attach(tableFile);
}

JMapInfo* StageDataHolder::findJmpInfoFromArray(const MR::AssignableArray<JMapInfo> *pInfoArr, const char *pName) const {
    JMapInfo* inf = pInfoArr->mArr;
    while (inf != pInfoArr->mArr + pInfoArr->mMaxSize) {
        if (MR::isEqualStringCase(inf->getName(), pName)) {
            return inf;
        }

        inf++;
    }

    return 0;
}

JMapInfoIter StageDataHolder::getStartJMapInfoIterFromStartDataIndex(int idx_) const {
    int idx = idx_;
    int curIdx;
    JMapInfo* pEnd = mStartObjs.mArr + mStartObjs.mMaxSize;
    bool isValid;
    s32 i;
    JMapInfo* inf = mStartObjs.mArr;
    StageDataHolder* locHolder;
    
    while (inf != pEnd)
    {
        const JMapData * curData = inf->mData;
        isValid = curData;

        curIdx = isValid ? curData->mNumEntries : 0;
    
        if (idx < curIdx) {
            JMapInfoIter iter;
            iter.mInfo = inf;
            iter._4 = idx;
            return iter;
        }

        curIdx = isValid ? curData->mNumEntries : 0;

        idx -= curIdx;

        inf++;
    }

    for (s32 i = 0; i < mStageDataHolderCount; i++) {
        locHolder = mStageDataArray[i];
        int startPosNum = locHolder->getStartPosNum();

        if (idx < startPosNum) {
            return locHolder->getStartJMapInfoIterFromStartDataIndex(idx);
        }
        idx -= startPosNum;
    }

    JMapInfoIter iter;
    iter.mInfo = 0;
    iter._4 = -1;
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
    JMapInfo* inf = pInfoArray->mArr;

    while (inf != pInfoArray->mArr + pInfoArray->mMaxSize) {
        if ((u32)inf->mData < _E4) {
            _E4 = (u32)inf->mData;
        }

        u32 addr = (inf->mData->mEntrySize * inf->mData->mNumEntries) + ((s32)inf->mData + inf->mData->mDataOffset);

        if (_E8 < addr) {
            _E8 = addr;
        }

        inf++;
    }
}