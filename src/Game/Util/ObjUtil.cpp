#include "Game/Effect/EffectSystemUtil.hpp"
#include "Game/Map/NamePosHolder.hpp"
#include "Game/NameObj/MovementOnOffGroupHolder.hpp"
#include "Game/NameObj/NameObjAdaptor.hpp"
#include "Game/NameObj/NameObjExecuteHolder.hpp"
#include "Game/NameObj/NameObjFinder.hpp"
#include "Game/NameObj/NameObjListExecutor.hpp"
#include "Game/Scene/SceneObjHolder.hpp"
#include "Game/Screen/LayoutActor.hpp"
#include "Game/System/GameSystem.hpp"
#include "Game/System/GameSystemSceneController.hpp"
#include "Game/System/ResourceHolder.hpp"
#include "Game/System/ResourceHolderManager.hpp"
#include "Game/Util/ObjUtil.hpp"
#include "Game/SingletonHolder.hpp"
#include <cstdio>
#include <va_list.h>

namespace {
    JMapInfo* tryCreateCsvParserLocal(const ResourceHolder* pHolder, const char* pArchive, va_list pFormat) NO_INLINE {
        char buf[0x100];
        vsnprintf(buf, sizeof(buf), pArchive, pFormat);

        if (!pHolder->mFileInfoTable->isExistRes(buf)) {
            return nullptr;
        }

        JMapInfo* inf = new JMapInfo();
        inf->attach(pHolder->mFileInfoTable->getRes(buf));

        return inf;
    }
}; // namespace

namespace MR {
    void connectToScene(LiveActor* pActor, int a2, int a3, int a4, int a5) {
        MR::registerNameObjToExecuteHolder(reinterpret_cast<NameObj*>(pActor), a2, a3, a4, a5);
    }

    void connectToScene(NameObj* pObj, int a2, int a3, int a4, int a5) {
        MR::registerNameObjToExecuteHolder(pObj, a2, a3, a4, a5);
        MR::connectToSceneTemporarily(pObj);
        MR::connectToDrawTemporarily(pObj);
    }

    void connectToSceneCollisionMapObj(LiveActor* pActor) {
        MR::registerNameObjToExecuteHolder(reinterpret_cast<NameObj*>(pActor), 0x1E, 2, 8, -1);
    }

    void connectToSceneCollisionMapObjMovementCalcAnim(LiveActor* pActor) {
        MR::registerNameObjToExecuteHolder(reinterpret_cast<NameObj*>(pActor), 0x1E, 2, -1, -1);
    }

    void connectToSceneCollisionMapObjWeakLight(LiveActor* pActor) {
        MR::registerNameObjToExecuteHolder(reinterpret_cast<NameObj*>(pActor), 0x1E, 2, 9, -1);
    }

    void connectToSceneCollisionMapObjStrongLight(LiveActor* pActor) {
        MR::registerNameObjToExecuteHolder(reinterpret_cast<NameObj*>(pActor), 0x1E, 2, 0xA, -1);
    }

    void connectToSceneCollisionEnemy(LiveActor* pActor) {
        MR::registerNameObjToExecuteHolder(reinterpret_cast<NameObj*>(pActor), 0x1F, 3, 0x12, -1);
    }

    void connectToSceneCollisionEnemyMovement(NameObj* pObj) {
        MR::registerNameObjToExecuteHolder(pObj, 0x1F, -1, -1, -1);
        MR::connectToSceneTemporarily(pObj);
        MR::connectToDrawTemporarily(pObj);
    }

    void connectToSceneCollisionEnemyStrongLight(LiveActor* pActor) {
        MR::registerNameObjToExecuteHolder(reinterpret_cast<NameObj*>(pActor), 0x1F, 3, 0xA, -1);
    }

    void connectToSceneCollisionEnemyNoShadowedMapObjStrongLight(LiveActor* pActor) {
        MR::registerNameObjToExecuteHolder(reinterpret_cast<NameObj*>(pActor), 0x1F, 3, 0xC, -1);
    }

    void connectToSceneNpc(LiveActor* pActor) {
        MR::registerNameObjToExecuteHolder(reinterpret_cast<NameObj*>(pActor), 0x28, 6, 0x10, -1);
    }

    void connectToSceneNpcMovement(LiveActor* pActor) {
        MR::registerNameObjToExecuteHolder(reinterpret_cast<NameObj*>(pActor), 0x28, -1, -1, -1);
    }

    void connectToSceneRide(LiveActor* pActor) {
        MR::registerNameObjToExecuteHolder(reinterpret_cast<NameObj*>(pActor), 0x29, 7, 0x11, -1);
    }

    void connectToSceneEnemy(LiveActor* pActor) {
        MR::registerNameObjToExecuteHolder(reinterpret_cast<NameObj*>(pActor), 0x2A, 8, 0x12, -1);
    }

    void connectToSceneEnemyMovement(NameObj* pObj) {
        MR::registerNameObjToExecuteHolder(pObj, 0x2A, -1, -1, -1);
        MR::connectToSceneTemporarily(pObj);
        MR::connectToDrawTemporarily(pObj);
    }

    void connectToSceneMapObj(LiveActor* pActor) {
        MR::registerNameObjToExecuteHolder(reinterpret_cast<NameObj*>(pActor), 0x22, 5, 8, -1);
    }

    void connectToSceneMapObjMovement(NameObj* pObj) {
        MR::registerNameObjToExecuteHolder(pObj, 0x22, -1, -1, -1);
        MR::connectToSceneTemporarily(pObj);
        MR::connectToDrawTemporarily(pObj);
    }

    void connectToSceneMapObjMovementCalcAnim(NameObj* pObj) {
        MR::registerNameObjToExecuteHolder(pObj, 0x22, 5, -1, -1);
        MR::connectToSceneTemporarily(pObj);
        MR::connectToDrawTemporarily(pObj);
    }

    void connectToSceneMapObjNoMovement(LiveActor* pActor) {
        MR::registerNameObjToExecuteHolder(reinterpret_cast<NameObj*>(pActor), -1, 5, 8, -1);
    }

    void connectToSceneMapObjNoCalcAnim(LiveActor* pActor) {
        MR::registerNameObjToExecuteHolder(reinterpret_cast<NameObj*>(pActor), 0x22, -1, 8, -1);
    }

    void connectToSceneMapObjNoCalcAnimStrongLight(LiveActor* pActor) {
        MR::registerNameObjToExecuteHolder(reinterpret_cast<NameObj*>(pActor), 0x22, -1, 0xA, -1);
    }

    void connectToSceneMapObjDecoration(LiveActor* pActor) {
        MR::registerNameObjToExecuteHolder(reinterpret_cast<NameObj*>(pActor), 0x23, 0xB, 8, -1);
    }

    void connectToSceneMapObjDecorationStrongLight(LiveActor* pActor) {
        MR::registerNameObjToExecuteHolder(reinterpret_cast<NameObj*>(pActor), 0x23, 0xB, 0xA, -1);
    }

    void connectToSceneMapObjDecorationMovement(NameObj* pObj) {
        MR::registerNameObjToExecuteHolder(pObj, 0x23, -1, -1, -1);
        MR::connectToSceneTemporarily(pObj);
        MR::connectToDrawTemporarily(pObj);
    }

    void connectToSceneMapObjStrongLight(LiveActor* pActor) {
        MR::registerNameObjToExecuteHolder(reinterpret_cast<NameObj*>(pActor), 0x22, 5, 0xA, -1);
    }

    void connectToSceneMapParts(LiveActor* pActor) {
        MR::registerNameObjToExecuteHolder(reinterpret_cast<NameObj*>(pActor), 0x1C, 0, 8, -1);
    }

    void connectToScenePlanet(LiveActor* pActor) {
        if (MR::isExistIndirectTexture(pActor)) {
            MR::registerNameObjToExecuteHolder(reinterpret_cast<NameObj*>(pActor), 0x1D, 1, 0x1D, -1);
        } else {
            MR::registerNameObjToExecuteHolder(reinterpret_cast<NameObj*>(pActor), 0x1D, 1, 4, -1);
        }
    }

    void connectToSceneEnvironment(LiveActor* pActor) {
        MR::registerNameObjToExecuteHolder(reinterpret_cast<NameObj*>(pActor), 0x21, 4, 6, -1);
    }

    void connectToSceneEnvironmentStrongLight(LiveActor* pActor) {
        MR::registerNameObjToExecuteHolder(reinterpret_cast<NameObj*>(pActor), 0x21, 4, 7, -1);
    }

    void connectToClippedMapParts(LiveActor* pActor) {
        MR::registerNameObjToExecuteHolder(reinterpret_cast<NameObj*>(pActor), 0x1C, 0, 0, -1);
    }

    void connectToSceneEnemyDecoration(LiveActor* pActor) {
        MR::registerNameObjToExecuteHolder(reinterpret_cast<NameObj*>(pActor), 0x2B, 0xB, 0x13, -1);
    }

    void connectToSceneEnemyDecorationMovement(NameObj* pObj) {
        MR::registerNameObjToExecuteHolder(pObj, 0x2B, -1, -1, -1);
        MR::connectToSceneTemporarily(pObj);
        MR::connectToDrawTemporarily(pObj);
    }

    void connectToSceneEnemyDecorationMovementCalcAnim(NameObj* pObj) {
        MR::registerNameObjToExecuteHolder(pObj, 0x2B, 0xB, -1, -1);
        MR::connectToSceneTemporarily(pObj);
        MR::connectToDrawTemporarily(pObj);
    }

    void connectToSceneItem(LiveActor* pActor) {
        MR::registerNameObjToExecuteHolder(reinterpret_cast<NameObj*>(pActor), 0x2C, 0x10, 0xD, -1);
    }

    void connectToSceneItemStrongLight(LiveActor* pActor) {
        MR::registerNameObjToExecuteHolder(reinterpret_cast<NameObj*>(pActor), 0x2C, 0x10, 0xF, -1);
    }

    void connectToSceneIndirectEnemy(LiveActor* pActor) {
        MR::registerNameObjToExecuteHolder(reinterpret_cast<NameObj*>(pActor), 0x2A, 8, 0x1C, -1);
    }

    void connectToSceneIndirectNpc(LiveActor* pActor) {
        MR::registerNameObjToExecuteHolder(reinterpret_cast<NameObj*>(pActor), 0x28, 6, 0x1B, -1);
    }

    void connectToSceneIndirectMapObj(LiveActor* pActor) {
        MR::registerNameObjToExecuteHolder(reinterpret_cast<NameObj*>(pActor), 0x22, 5, 0x19, -1);
    }

    void connectToSceneIndirectMapObjStrongLight(LiveActor* pActor) {
        MR::registerNameObjToExecuteHolder(reinterpret_cast<NameObj*>(pActor), 0x22, 5, 0x1A, -1);
    }

    void connectToSceneScreenEffectMovement(NameObj* pObj) {
        MR::registerNameObjToExecuteHolder(pObj, 3, -1, -1, -1);
        MR::connectToSceneTemporarily(pObj);
        MR::connectToDrawTemporarily(pObj);
    }

    void connectToSceneAreaObj(NameObj* pObj) {
        MR::registerNameObjToExecuteHolder(pObj, 0xD, -1, -1, -1);
        MR::connectToSceneTemporarily(pObj);
        MR::connectToDrawTemporarily(pObj);
    }

    void connectToScene3DModelFor2D(LiveActor* pActor) {
        MR::registerNameObjToExecuteHolder(pActor, 0xE, 0xD, 0x24, -1);
    }

    void connectToSceneLayout(NameObj* pObj) {
        MR::registerNameObjToExecuteHolder(pObj, 0xE, 0xD, -1, 0x3C);
        MR::connectToSceneTemporarily(pObj);
        MR::connectToDrawTemporarily(pObj);
    }

    void connectToSceneLayoutMovementCalcAnim(NameObj* pObj) {
        MR::registerNameObjToExecuteHolder(pObj, 0xE, 0xD, -1, -1);
        MR::connectToSceneTemporarily(pObj);
        MR::connectToDrawTemporarily(pObj);
    }

    void connectToSceneLayoutDecoration(NameObj* pObj) {
        MR::registerNameObjToExecuteHolder(pObj, 0xF, 0xE, -1, 0x3D);
        MR::connectToSceneTemporarily(pObj);
        MR::connectToDrawTemporarily(pObj);
    }

    void connectToSceneTalkLayout(NameObj* pObj) {
        MR::registerNameObjToExecuteHolder(pObj, 0xE, 0xD, -1, 0x43);
        MR::connectToSceneTemporarily(pObj);
        MR::connectToDrawTemporarily(pObj);
    }

    void connectToSceneTalkLayoutNoMovement(NameObj* pObj) {
        MR::registerNameObjToExecuteHolder(pObj, -1, 0xD, -1, 0x43);
        MR::connectToSceneTemporarily(pObj);
        MR::connectToDrawTemporarily(pObj);
    }

    void connectToSceneWipeLayout(NameObj* pObj) {
        MR::registerNameObjToExecuteHolder(pObj, 0x11, 0xD, -1, 0x42);
        MR::connectToSceneTemporarily(pObj);
        MR::connectToDrawTemporarily(pObj);
    }

    void connectToSceneLayoutOnPause(NameObj* pObj) {
        MR::registerNameObjToExecuteHolder(pObj, 0x12, 0xD, -1, 0x3E);
        MR::connectToSceneTemporarily(pObj);
        MR::connectToDrawTemporarily(pObj);
    }

    void connectToSceneLayoutOnPauseNoMovement(NameObj* pObj) {
        MR::registerNameObjToExecuteHolder(pObj, -1, 0xD, -1, 0x3E);
        MR::connectToSceneTemporarily(pObj);
        MR::connectToDrawTemporarily(pObj);
    }

    void connectToSceneLayoutOnPauseMovementCalcAnim(NameObj* pObj) {
        MR::registerNameObjToExecuteHolder(pObj, 0x12, 0xD, -1, -1);
        MR::connectToSceneTemporarily(pObj);
        MR::connectToDrawTemporarily(pObj);
    }

    void connectToSceneLayoutMovement(NameObj* pObj) {
        MR::registerNameObjToExecuteHolder(pObj, 0xE, -1, -1, -1);
        MR::connectToSceneTemporarily(pObj);
        MR::connectToDrawTemporarily(pObj);
    }

    void connectToSceneMovie(NameObj* pObj) {
        MR::registerNameObjToExecuteHolder(pObj, 0x10, -1, -1, 0x3F);
        MR::connectToSceneTemporarily(pObj);
        MR::connectToDrawTemporarily(pObj);
    }

    void connectToSceneMirrorMapObj(LiveActor* pActor) {
        MR::registerNameObjToExecuteHolder(pActor, 0x22, 0xC, 0x27, -1);
    }

    void connectToSceneMirrorMapObjDecoration(LiveActor* pActor) {
        MR::registerNameObjToExecuteHolder(pActor, 0x23, 0xC, 0x27, -1);
    }

    void connectToSceneMirrorMapObjNoMovement(LiveActor* pActor) {
        MR::registerNameObjToExecuteHolder(pActor, -1, 0xC, 0x27, -1);
    }

    void connectToSceneCamera(NameObj* pObj) {
        MR::registerNameObjToExecuteHolder(pObj, 0x2, -1, -1, -1);
        MR::connectToSceneTemporarily(pObj);
        MR::connectToDrawTemporarily(pObj);
    }

    void connectToSceneNoShadowedMapObj(LiveActor* pActor) {
        MR::registerNameObjToExecuteHolder(pActor, 0x22, 0x5, 0xB, -1);
    }

    void connectToSceneNoShadowedMapObjStrongLight(LiveActor* pActor) {
        MR::registerNameObjToExecuteHolder(pActor, 0x22, 0x5, 0xC, -1);
    }

    void connectToSceneNoSilhouettedMapObj(LiveActor* pActor) {
        MR::registerNameObjToExecuteHolder(pActor, 0x22, 0x5, 0xD, -1);
    }

    void connectToSceneNoSilhouettedMapObjStrongLight(LiveActor* pActor) {
        MR::registerNameObjToExecuteHolder(pActor, 0x22, 0x5, 0xF, -1);
    }

    void connectToSceneNoSilhouettedMapObjWeakLightNoMovement(LiveActor* pActor) {
        MR::registerNameObjToExecuteHolder(pActor, -1, 0x5, 0xE, -1);
    }

    void connectToSceneSky(LiveActor* pActor) {
        MR::registerNameObjToExecuteHolder(pActor, 0x24, 0x5, 0x1, -1);
    }

    void connectToSceneAir(LiveActor* pActor) {
        MR::registerNameObjToExecuteHolder(pActor, 0x24, 0x5, 0x2, -1);
    }

    void connectToSceneSun(LiveActor* pActor) {
        MR::registerNameObjToExecuteHolder(pActor, 0x24, 0x5, 0x3, -1);
    }

    void connectToSceneCrystal(LiveActor* pActor) {
        MR::registerNameObjToExecuteHolder(pActor, 0x22, 0x5, 0x20, -1);
    }

    void connectToSceneNormalMapObj(LiveActor* pActor) {
        MR::registerNameObjToExecuteHolder(pActor, 0x22, 0x5, -1, 0x18);
    }

    NameObjAdaptor* createDrawAdaptor(const char* pName, const MR::FunctorBase& rFunctor) {
        NameObjAdaptor* adaptor = new NameObjAdaptor(pName);
        adaptor->connectToDraw(rFunctor);
        return adaptor;
    }

    void requestMovementOn(NameObj* pObj) {
        NameObjFunction::requestMovementOn(pObj);
    }

    void requestMovementOn(LiveActor* pActor) {
        NameObjFunction::requestMovementOn(pActor);
        if (MR::isExistEffectKeeper(pActor)) {
            MR::Effect::requestMovementOn(pActor->mEffectKeeper);
        }
    }

    void requestMovementOn(LayoutActor* pActor) {
        NameObjFunction::requestMovementOn(pActor);
    }

    void requestMovementOff(NameObj* pObj) {
        NameObjFunction::requestMovementOff(pObj);
    }

    NameObjGroup* joinToNameObjGroup(NameObj* pObj, const char* pGroupName) {
        NameObjGroup* pObjGroup = static_cast<NameObjGroup*>(NameObjFinder::find(pGroupName));

        pObjGroup->registerObj(pObj);

        return pObjGroup;
    }

    NameObjGroup* joinToMovementOnOffGroup(const char* pName, NameObj* pObj, u32 a3) {
        MovementOnOffGroupHolder* pGroupHolder = MR::getSceneObj<MovementOnOffGroupHolder>(SceneObj_MovementOnOffGroupHolder);

        return pGroupHolder->joinToGroup(pName, pObj, a3);
    }

    void onMovementOnOffGroup(const char* pGroupName) {
        MR::getSceneObj<MovementOnOffGroupHolder>(SceneObj_MovementOnOffGroupHolder)->onMovementGroup(pGroupName);
    }

    void registerPreDrawFunction(const MR::FunctorBase& rFunc, int a2) {
        SingletonHolder<GameSystem>::get()->mSceneController->getNameObjListExecutor()->registerPreDrawFunction(rFunc, a2);
    }

    NameObjAdaptor* createAdaptorAndConnectToDrawBloomModel(const char* pName, const MR::FunctorBase& rFunctor) {
        NameObjAdaptor* adaptor = new NameObjAdaptor(pName);
        adaptor->connectToDraw(rFunctor);
        MR::registerNameObjToExecuteHolder(adaptor, -1, -1, -1, 0x36);
        MR::connectToSceneTemporarily(adaptor);
        MR::connectToDrawTemporarily(adaptor);
        return adaptor;
    }

    ResourceHolder* createAndAddResourceHolder(const char* pResource) {
        return SingletonHolder<ResourceHolderManager>::get()->createAndAdd(pResource, nullptr);
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
        snprintf(arcBuf, sizeof(arcBuf), "%s.arc", pArchive);
        char texBuf[0x100];
        snprintf(texBuf, sizeof(texBuf), "%s.bti", pArchive);
        return loadTexFromArc(arcBuf, texBuf);
    }

    JMapInfo* createCsvParser(const ResourceHolder* pHolder, const char* pFormat, ...) {
        va_list list;
        va_start(list, pFormat);
        return ::tryCreateCsvParserLocal(pHolder, pFormat, list);
    }

    JMapInfo* createCsvParser(const char* pArchive, const char* pFormat, ...) {
        ResourceHolder* holder = SingletonHolder<ResourceHolderManager>::get()->createAndAdd(pArchive, nullptr);
        return MR::createCsvParser(holder, pFormat);
    }

    // MR::tryCreateCsvParser

    s32 getCsvDataElementNum(const JMapInfo* pMapInfo) {
        if (pMapInfo->mData != nullptr) {
            return pMapInfo->mData->mNumEntries;
        }

        return 0;
    }

    void getCsvDataStr(const char** pOut, const JMapInfo* pMapInfo, const char* pKey, s32 idx) {
        int v7 = pMapInfo->searchItemInfo(pKey);

        if (v7 >= 0) {
            pMapInfo->getValueFast(idx, v7, pOut);
        }
    }

    void getCsvDataStrOrNULL(const char** pOut, const JMapInfo* pMapInfo, const char* pKey, s32 idx) {
        getCsvDataStr(pOut, pMapInfo, pKey, idx);
        if (*pOut[0] == 0) {
            *pOut = nullptr;
        }
    }

    void getCsvDataS32(s32* pOut, const JMapInfo* pMapInfo, const char* pKey, s32 idx) {
        pMapInfo->getValue<s32>(idx, pKey, pOut);
    }

    void getCsvDataU8(u8* pOut, const JMapInfo* pMapInfo, const char* pKey, s32 idx) {
        s32 val = 0;
        pMapInfo->getValue<s32>(idx, pKey, &val);
        *pOut = val;
    }

#ifdef NON_MATCHING
    void getCsvDataF32(f32* pOut, const JMapInfo* pMapInfo, const char* pKey, s32 idx) {
        int v7 = pMapInfo->searchItemInfo(pKey);

        if (v7 >= 0) {
            *pOut = *((f32*)(pMapInfo->mData->mNumEntries + pMapInfo->mData->mDataOffset + idx * (pMapInfo->mData->mEntrySize + static_cast<const JMapItem*>(&pMapInfo->mData->mItems)[v7].mOffsData)));
        }
    }
#endif

    // MR::getCsvDataBool

    bool isName(const NameObj* pObj, const char* pName) {
        return strcmp(pObj->mName, pName) == 0;
    }

    bool isSame(const NameObj* a1, const NameObj* a2) {
        return a1 == a2;
    }

    bool tryRegisterNamePosLinkObj(const NameObj* pObj, const JMapInfoIter& rIter) {
        return MR::getNamePosHolder()->tryRegisterLinkObj(pObj, rIter);
    }

    void findNamePos(const char* pName, MtxPtr mtx) {
        MR::tryFindLinkNamePos(nullptr, pName, mtx);
    }

    void findNamePos(const char* pName, TVec3f* a2, TVec3f* a3) {
        MR::getNamePosHolder()->find(nullptr, pName, a2, a3);
    }

    // MR::findNamePosOnGround
}; // namespace MR
