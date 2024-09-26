#include "Game/NameObj/NameObjExecuteHolder.hpp"
#include "Game/NameObj/NameObjAdaptor.hpp"
#include "Game/System/ResourceHolder.hpp"
#include "Game/System/ResourceHolderManager.hpp"
#include "Game/Util/ObjUtil.hpp"
#include <cstdio>
#include <va_list.h>

class LiveActor;
 
namespace {
    JMapInfo* tryCreateCsvParserLocal(const ResourceHolder* pHolder, const char* pArchive, va_list pFormat) NO_INLINE {
            char buf[0x100];
            vsnprintf(buf, 0x100, pArchive, pFormat);

            if (!pHolder->mFileInfoTable->isExistRes(buf)) {
                return nullptr;
            }

            JMapInfo* inf = new JMapInfo();
            inf->attach(pHolder->mFileInfoTable->getRes(buf));
            return inf;
    }
};

namespace MR {
    void connectToScene(LiveActor *pActor, int a2, int a3, int a4, int a5) {
        MR::registerNameObjToExecuteHolder(reinterpret_cast<NameObj*>(pActor), a2, a3, a4, a5);
    }

    void connectToScene(NameObj *pObj, int a2, int a3, int a4, int a5) {
        MR::registerNameObjToExecuteHolder(pObj, a2, a3, a4, a5);
        MR::connectToSceneTemporarily(pObj);
        MR::connectToDrawTemporarily(pObj);
    }

    void connectToSceneCollisionMapObj(LiveActor *pActor) {
        MR::registerNameObjToExecuteHolder(reinterpret_cast<NameObj*>(pActor), 0x1E, 2, 8, -1);
    }

    void connectToSceneCollisionMapObjMovementCalcAnim(LiveActor *pActor) {
        MR::registerNameObjToExecuteHolder(reinterpret_cast<NameObj*>(pActor), 0x1E, 2, -1, -1);
    }

    void connectToSceneCollisionMapObjWeakLight(LiveActor *pActor) {
        MR::registerNameObjToExecuteHolder(reinterpret_cast<NameObj*>(pActor), 0x1E, 2, 9, -1);
    }

    void connectToSceneCollisionMapObjStrongLight(LiveActor *pActor) {
        MR::registerNameObjToExecuteHolder(reinterpret_cast<NameObj*>(pActor), 0x1E, 2, 0xA, -1);
    }

    void connectToSceneCollisionEnemy(LiveActor *pActor) {
        MR::registerNameObjToExecuteHolder(reinterpret_cast<NameObj*>(pActor), 0x1F, 3, 0x12, -1);
    }

    void connectToSceneCollisionEnemyMovement(NameObj *pObj) {
        MR::registerNameObjToExecuteHolder(pObj, 0x1F, -1, -1, -1);
        MR::connectToSceneTemporarily(pObj);
        MR::connectToDrawTemporarily(pObj);
    }

    void connectToSceneCollisionEnemyStrongLight(LiveActor *pActor) {
        MR::registerNameObjToExecuteHolder(reinterpret_cast<NameObj*>(pActor), 0x1F, 3, 0xA, -1);
    }

    void connectToSceneCollisionEnemyNoShadowedMapObjStrongLight(LiveActor *pActor) {
        MR::registerNameObjToExecuteHolder(reinterpret_cast<NameObj*>(pActor), 0x1F, 3, 0xC, -1);
    }

    void connectToSceneNpc(LiveActor *pActor) {
        MR::registerNameObjToExecuteHolder(reinterpret_cast<NameObj*>(pActor), 0x28, 6, 0x10, -1);
    }

    void connectToSceneNpcMovement(LiveActor *pActor) {
        MR::registerNameObjToExecuteHolder(reinterpret_cast<NameObj*>(pActor), 0x28, -1, -1, -1);
    }

    void connectToSceneRide(LiveActor *pActor) {
        MR::registerNameObjToExecuteHolder(reinterpret_cast<NameObj*>(pActor), 0x29, 7, 0x11, -1);
    }

    void connectToSceneEnemy(LiveActor *pActor) {
        MR::registerNameObjToExecuteHolder(reinterpret_cast<NameObj*>(pActor), 0x2A, 8, 0x12, -1);
    }

    void connectToSceneEnemyMovement(NameObj *pObj) {
        MR::registerNameObjToExecuteHolder(pObj, 0x2A, -1, -1, -1);
        MR::connectToSceneTemporarily(pObj);
        MR::connectToDrawTemporarily(pObj);
    }

    void connectToSceneMapObj(LiveActor *pActor) {
        MR::registerNameObjToExecuteHolder(reinterpret_cast<NameObj*>(pActor), 0x22, 5, 8, -1);
    }

    void connectToSceneMapObjMovement(NameObj *pObj) {
        MR::registerNameObjToExecuteHolder(pObj, 0x22, -1, -1, -1);
        MR::connectToSceneTemporarily(pObj);
        MR::connectToDrawTemporarily(pObj);
    }

    void connectToSceneMapObjMovementCalcAnim(NameObj *pObj) {
        MR::registerNameObjToExecuteHolder(pObj, 0x22, 5, -1, -1);
        MR::connectToSceneTemporarily(pObj);
        MR::connectToDrawTemporarily(pObj);
    }

    void connectToSceneMapObjNoMovement(LiveActor *pActor) {
        MR::registerNameObjToExecuteHolder(reinterpret_cast<NameObj*>(pActor), -1, 5, 8, -1);
    }

    void connectToSceneMapObjNoCalcAnim(LiveActor *pActor) {
        MR::registerNameObjToExecuteHolder(reinterpret_cast<NameObj*>(pActor), 0x22, -1, 8, -1);
    }

    void connectToSceneMapObjNoCalcAnimStrongLight(LiveActor *pActor) {
        MR::registerNameObjToExecuteHolder(reinterpret_cast<NameObj*>(pActor), 0x22, -1, 0xA, -1);
    }

    void connectToSceneMapObjDecoration(LiveActor *pActor) {
        MR::registerNameObjToExecuteHolder(reinterpret_cast<NameObj*>(pActor), 0x23, 0xB, 8, -1);
    }

    void connectToSceneMapObjDecorationStrongLight(LiveActor *pActor) {
        MR::registerNameObjToExecuteHolder(reinterpret_cast<NameObj*>(pActor), 0x23, 0xB, 0xA, -1);
    }

    void connectToSceneMapObjDecorationMovement(NameObj *pObj) {
        MR::registerNameObjToExecuteHolder(pObj, 0x23, -1, -1, -1);
        MR::connectToSceneTemporarily(pObj);
        MR::connectToDrawTemporarily(pObj);
    }

    void connectToSceneMapObjStrongLight(LiveActor *pActor) {
        MR::registerNameObjToExecuteHolder(reinterpret_cast<NameObj*>(pActor), 0x22, 5, 0xA, -1);
    }

    void connectToSceneMapParts(LiveActor *pActor) {
        MR::registerNameObjToExecuteHolder(reinterpret_cast<NameObj*>(pActor), 0x1C, 0, 8, -1);
    }

    void connectToScenePlanet(LiveActor *pActor) {
        if (MR::isExistIndirectTexture(pActor)) {
            MR::registerNameObjToExecuteHolder(reinterpret_cast<NameObj*>(pActor), 0x1D, 1, 0x1D, -1);
        }
        else {
            MR::registerNameObjToExecuteHolder(reinterpret_cast<NameObj*>(pActor), 0x1D, 1, 4, -1);
        }
    }

    void connectToSceneEnvironment(LiveActor *pActor) {
        MR::registerNameObjToExecuteHolder(reinterpret_cast<NameObj*>(pActor), 0x21, 4, 6, -1);
    }

    void connectToSceneEnvironmentStrongLight(LiveActor *pActor) {
        MR::registerNameObjToExecuteHolder(reinterpret_cast<NameObj*>(pActor), 0x21, 4, 7, -1);
    }

    void connectToClippedMapParts(LiveActor *pActor) {
        MR::registerNameObjToExecuteHolder(reinterpret_cast<NameObj*>(pActor), 0x1C, 0, 0, -1);
    }

    void connectToSceneEnemyDecoration(LiveActor *pActor) {
        MR::registerNameObjToExecuteHolder(reinterpret_cast<NameObj*>(pActor), 0x2B, 0xB, 0x13, -1);
    }

    void connectToSceneEnemyDecorationMovement(NameObj *pObj) {
        MR::registerNameObjToExecuteHolder(pObj, 0x2B, -1, -1, -1);
        MR::connectToSceneTemporarily(pObj);
        MR::connectToDrawTemporarily(pObj);
    }

    void connectToSceneEnemyDecorationMovementCalcAnim(NameObj *pObj) {
        MR::registerNameObjToExecuteHolder(pObj, 0x2B, 0x11, -1, -1);
        MR::connectToSceneTemporarily(pObj);
        MR::connectToDrawTemporarily(pObj);
    }

    void connectToSceneItem(LiveActor *pActor) {
        MR::registerNameObjToExecuteHolder(reinterpret_cast<NameObj*>(pActor), 0x2C, 0x10, 0xD, -1);
    }

    void connectToSceneItemStrongLight(LiveActor *pActor) {
        MR::registerNameObjToExecuteHolder(reinterpret_cast<NameObj*>(pActor), 0x2C, 0x10, 0xF, -1);
    }

    void connectToSceneIndirectEnemy(LiveActor *pActor) {
        MR::registerNameObjToExecuteHolder(reinterpret_cast<NameObj*>(pActor), 0x2A, 8, 0x1C, -1);
    }

    void connectToSceneIndirectNpc(LiveActor *pActor) {
        MR::registerNameObjToExecuteHolder(reinterpret_cast<NameObj*>(pActor), 0x28, 6, 0x1B, -1);
    }

    void connectToSceneIndirectMapObj(LiveActor *pActor) {
        MR::registerNameObjToExecuteHolder(reinterpret_cast<NameObj*>(pActor), 0x22, 5, 0x19, -1);
    }

    void connectToSceneIndirectMapObjStrongLight(LiveActor *pActor) {
        MR::registerNameObjToExecuteHolder(reinterpret_cast<NameObj*>(pActor), 0x22, 5, 0x1A, -1);
    }

    void connectToSceneScreenEffectMovement(NameObj *pObj) {
        MR::registerNameObjToExecuteHolder(pObj, 3, -1, -1, -1);
        MR::connectToSceneTemporarily(pObj);
        MR::connectToDrawTemporarily(pObj);
    }

    void connectToSceneAreaObj(NameObj *pObj) {
        MR::registerNameObjToExecuteHolder(pObj, 0xD, -1, -1, -1);
        MR::connectToSceneTemporarily(pObj);
        MR::connectToDrawTemporarily(pObj);
    }

    NameObjAdaptor* createDrawAdaptor(const char *pName, const MR::FunctorBase &rFunctor) {
        NameObjAdaptor* adaptor = new NameObjAdaptor(pName);
        adaptor->connectToDraw(rFunctor);
        return adaptor;
    }

    NameObjAdaptor* createAdaptorAndConnectToDrawBloomModel(const char *pName, const MR::FunctorBase &rFunctor) {
        NameObjAdaptor* adaptor = new NameObjAdaptor(pName);
        adaptor->connectToDraw(rFunctor);
        MR::registerNameObjToExecuteHolder(adaptor, -1, -1, -1, 0x36);
        MR::connectToSceneTemporarily(adaptor);
        MR::connectToDrawTemporarily(adaptor);
        return adaptor;
    }

    ResourceHolder* createAndAddResourceHolder(const char* pResource) {
        return SingletonHolder<ResourceHolderManager>::sInstance->createAndAdd(pResource, nullptr);
    }

    void* loadResourceFromArc(const char* pArchive, const char* pFile) {
        ResourceHolder* holder = createAndAddResourceHolder(pArchive);
        return holder->mFileInfoTable->getRes(pFile);
    }

    bool isExistResourceInArc(const char* pArchive, const char* pFile) {
        ResourceHolder* holder = createAndAddResourceHolder(pArchive);
        return holder->mFileInfoTable->isExistRes(pFile);
    }

    const ResTIMG* loadTexFromArc(const char* pArchive, const char* pFile) {
        return static_cast<const ResTIMG*>(loadResourceFromArc(pArchive, pFile));
    }

    const ResTIMG* loadTexFromArc(const char* pArchive) {
        char arcBuf[0x100];
        snprintf(arcBuf, 0x100, "%s.arc", pArchive);
        char texBuf[0x100];
        snprintf(texBuf, 0x100, "%s.bti", pArchive);
        return loadTexFromArc(arcBuf, texBuf);
    }

    JMapInfo* createCsvParser(const ResourceHolder* pHolder, const char* pFormat, ...) {
        va_list list;
        va_start(list, pFormat);
        return ::tryCreateCsvParserLocal(pHolder, pFormat, list);
    }

    JMapInfo* createCsvParser(const char *pArchive, const char *pFormat, ...) {
        ResourceHolder* holder = SingletonHolder<ResourceHolderManager>::sInstance->createAndAdd(pArchive, nullptr);
        return MR::createCsvParser(holder, pFormat);   
    }

    // MR::tryCreateCsvParser

    s32 getCsvDataElementNum(const JMapInfo *pMapInfo) {
        if (pMapInfo->mData != nullptr) {
            return pMapInfo->mData->mNumEntries;
        }

        return 0;
    }

    void getCsvDataStr(const char **pOut , const JMapInfo *pMapInfo, const char *pKey, s32 idx) {
        int v7 = pMapInfo->searchItemInfo(pKey);

        if (v7 >= 0) {
            pMapInfo->getValueFast(idx, v7, pOut);
        }
    }

    void getCsvDataStrOrNULL(const char **pOut, const JMapInfo *pMapInfo, const char *pKey, s32 idx) {
        getCsvDataStr(pOut, pMapInfo, pKey, idx);
        if (*pOut[0] == 0) {
            *pOut = nullptr;
        }
    }

    void getCsvDataS32(s32 *pOut, const JMapInfo *pMapInfo, const char *pKey, s32 idx) {
        pMapInfo->getValue<s32>(idx, pKey, pOut);
    }

    void getCsvDataU8(u8 *pOut, const JMapInfo *pMapInfo, const char *pKey, s32 idx) {
        s32 val = 0;
        pMapInfo->getValue<s32>(idx, pKey, &val);
        *pOut = val;
    }

    #ifdef NON_MATCHING
    void getCsvDataF32(f32 *pOut, const JMapInfo *pMapInfo, const char *pKey, s32 idx) {
        int v7 = pMapInfo->searchItemInfo(pKey);

        if (v7 >= 0) {
            *pOut = *((f32*)(pMapInfo->mData->mNumEntries + pMapInfo->mData->mDataOffset + idx * (pMapInfo->mData->mEntrySize + static_cast<const JMapItem*>(&pMapInfo->mData->mItems)[v7].mOffsData)));
        }
    }
    #endif
    
    // MR::getCsvDataBool

};