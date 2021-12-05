#include "Game/Scene/StageDataHolder.h"
#include "Game/Scene/StageResourceLoader.h"
#include "Game/System/ScenarioDataParser.h"
#include "JSystem/JKernel/JKRArchive.h"

void StageDataHolder::init(const JMapInfoIter &rIter) {
    if (!_DC) {
        initTableData();
    }

    if (_E0) {
        u32 commonLayer = ScenarioDataFunction::getCurrentCommonLayers(_A8);
        initLayerJmpInfo(&_EC, "/jmp/Placement", "/jmp/MapParts", commonLayer);
    }

    createLocalStageDataHolder(_EC, 1);

    if (!_DC) {
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

    if (!_DC) {
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

#ifdef NON_MATCHING
// some small register usage issues
JMapInfoIter StageDataHolder::getStartJMapInfoIterFromStartDataIndex(int idx) const {
    JMapInfo* inf = mStartObjs.mArr;

    int curIdx = 0;

    while (inf != inf + mStartObjs.mMaxSize) {
        bool isValid = inf->mData != 0;

        curIdx = isValid ? inf->mData->_0 : 0;
    
        if (idx < curIdx) {
            JMapInfoIter iter;
            iter.mInfo = inf;
            iter._4 = idx;
            return iter;
        }

        curIdx = isValid ? inf->mData->_0 : 0;

        idx -= curIdx;
        inf++;
    }

    for (s32 i = 0; i < mStageDataHolderCount; i++) {
        StageDataHolder* locHolder = mStageDataArray[i];
        s32 startPosNum = locHolder->getStartPosNum();

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
#endif

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

        u32 addr = (inf->mData->_C * inf->mData->_0) + ((s32)inf->mData + inf->mData->mDataOffset);

        if (_E8 < addr) {
            _E8 = addr;
        }

        inf++;
    }
}