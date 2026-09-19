#include "Game/System/GameSystemStationedArchiveLoader.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/System/GameSystemFunction.hpp"
#include "Game/System/HeapMemoryWatcher.hpp"
#include "Game/System/ResourceHolder.hpp"
#include "Game/System/StationedFileInfo.hpp"
#include "Game/Util/FileUtil.hpp"
#include "Game/Util/Functor.hpp"
#include "Game/Util/MemoryUtil.hpp"
#include "Game/Util/NerveUtil.hpp"
#include "Game/Util/ObjUtil.hpp"
#include "Game/Util/SingletonHolder.hpp"
#include "Game/Util/SystemUtil.hpp"
#include <JSystem/JKernel/JKRExpHeap.hpp>

namespace {
    NEW_NERVE(GameSystemStationedArchiveLoaderLoadAudio1stWaveData, GameSystemStationedArchiveLoader, LoadAudio1stWaveData);
    NEW_NERVE(GameSystemStationedArchiveLoaderLoadStationedArchivePlayer, GameSystemStationedArchiveLoader, LoadStationedArchivePlayer);
    NEW_NERVE(GameSystemStationedArchiveLoaderLoadStationedArchiveOthers, GameSystemStationedArchiveLoader, LoadStationedArchiveOthers);
    NEW_NERVE(GameSystemStationedArchiveLoaderInitializeGameData, GameSystemStationedArchiveLoader, InitializeGameData);
    NEW_NERVE(GameSystemStationedArchiveLoaderEnd, GameSystemStationedArchiveLoader, End);
    NEW_NERVE(GameSystemStationedArchiveLoaderSuspended, GameSystemStationedArchiveLoader, Suspended);
    NEW_NERVE(GameSystemStationedArchiveLoaderChangeArchivePlayer, GameSystemStationedArchiveLoader, ChangeArchivePlayer);
};  // namespace

bool ConditionIfIsNotPlayer::isExecute(const MR::StationedFileInfo* pInfo) const {
    return pInfo->mLoadType != MR::StationedFileInfo::LOAD_TYPE_MOUNT_RESOURCE_MARIO &&
           pInfo->mLoadType != MR::StationedFileInfo::LOAD_TYPE_MOUNT_RESOURCE_LUIGI;
}

ConditionUsePlayerHeap::ConditionUsePlayerHeap() : mNapaHeap(nullptr), mGDDRHeap(nullptr), mIsDataMario(true) {
}

bool ConditionUsePlayerHeap::isExecute(const MR::StationedFileInfo* pInfo) const {
    MR::StationedFileInfo::LoadType type = MR::StationedFileInfo::LOAD_TYPE_MOUNT_RESOURCE_LUIGI;

    if (mIsDataMario) {
        type = MR::StationedFileInfo::LOAD_TYPE_MOUNT_RESOURCE_MARIO;
    }

    return pInfo->mLoadType == type;
}

JKRHeap* ConditionUsePlayerHeap::getProperHeap(const MR::StationedFileInfo* pInfo) const {
    switch (pInfo->mHeapType) {
    case MR::StationedFileInfo::HEAP_TYPE_NAPA:
        return mNapaHeap;
    case MR::StationedFileInfo::HEAP_TYPE_GDDR:
        return mGDDRHeap;
    case MR::StationedFileInfo::HEAP_TYPE_NONE:
        return nullptr;
    default:
        return nullptr;
    }
}

PlayerHeapHolder::PlayerHeapHolder() : mCondition(nullptr), mNapaHeap(nullptr), mGDDRHeap(nullptr), mIsDataMario(true) {
    ConditionUsePlayerHeap* condition = new ConditionUsePlayerHeap();
    mNapaHeap = PlayerHeapHolder::createHeap(0x500000, MR::getStationedHeapNapa());
    JKRExpHeap* gddr = PlayerHeapHolder::createHeap(0x500000, MR::getStationedHeapGDDR3());
    mGDDRHeap = gddr;
    condition->mNapaHeap = mNapaHeap;
    condition->mGDDRHeap = gddr;
    mCondition = condition;
    condition->mIsDataMario = mIsDataMario;
}

void PlayerHeapHolder::adjust() {
    JKRExpHeap* napa = mNapaHeap;
    if (!napa->isEmpty()) {
        napa->alloc(0x10000, nullptr);
    }

    JKRExpHeap* gddr = mGDDRHeap;
    if (!gddr->isEmpty()) {
        gddr->alloc(0x10000, nullptr);
    }

    MR::adjustHeapSize(mNapaHeap, nullptr);
    MR::adjustHeapSize(mGDDRHeap, nullptr);
}

void PlayerHeapHolder::dispose() {
    MR::removeResourceAndFileHolderIfIsEqualHeap(mNapaHeap);
    MR::removeResourceAndFileHolderIfIsEqualHeap(mGDDRHeap);

    if (mNapaHeap->mSize != 0) {
        mNapaHeap->freeAll();
    }

    if (mGDDRHeap->mSize != 0) {
        mGDDRHeap->freeAll();
    }
}

void PlayerHeapHolder::setIsDataMario(bool isDataMario) {
    mIsDataMario = isDataMario;
    mCondition->mIsDataMario = isDataMario;
}

JKRExpHeap* PlayerHeapHolder::createHeap(u32 size, JKRHeap* pParent) {
    if (pParent->getMaxAllocatableSize(0x10) < size) {
        size = -1;
    }

    return JKRExpHeap::create(size, pParent, true);
}

GameSystemStationedArchiveLoader::GameSystemStationedArchiveLoader() : NerveExecutor("常駐データ初期化"), mHeapHolder(nullptr), _C(false) {
    initNerve(GET_NERVE_ANON(GameSystemStationedArchiveLoaderLoadAudio1stWaveData));
}

void GameSystemStationedArchiveLoader::update() {
    updateNerve();
}

bool GameSystemStationedArchiveLoader::isDone() const {
    return isNerve(GET_NERVE_ANON(GameSystemStationedArchiveLoaderEnd));
}

bool GameSystemStationedArchiveLoader::isPreparedReset() const {
    return isNerve(GET_NERVE_ANON(GameSystemStationedArchiveLoaderEnd)) || isNerve(GET_NERVE_ANON(GameSystemStationedArchiveLoaderSuspended));
}

void GameSystemStationedArchiveLoader::prepareReset() {
    if (_C) {
        return;
    }

    if (isNerve(GET_NERVE_ANON(GameSystemStationedArchiveLoaderEnd)) || isNerve(GET_NERVE_ANON(GameSystemStationedArchiveLoaderSuspended)) ||
        isNerve(GET_NERVE_ANON(GameSystemStationedArchiveLoaderChangeArchivePlayer)) ||
        isNerve(GET_NERVE_ANON(GameSystemStationedArchiveLoaderInitializeGameData))) {
        return;
    }

    _C = true;
}

void GameSystemStationedArchiveLoader::requestChangeArchivePlayer(bool isDataMario) {
    if (mHeapHolder == nullptr) {
        return;
    }

    if (isNerve(GET_NERVE_ANON(GameSystemStationedArchiveLoaderChangeArchivePlayer))) {
        return;
    }

    if (isDataMario == mHeapHolder->mIsDataMario) {
        return;
    }

    mHeapHolder->setIsDataMario(isDataMario);
    setNerve(GET_NERVE_ANON(GameSystemStationedArchiveLoaderChangeArchivePlayer));
}

void GameSystemStationedArchiveLoader::exeLoadAudio1stWaveData() {
    if (MR::isFirstStep(this)) {
        GameSystemFunction::loadAudioStaticWaveData();
    }

    if (GameSystemFunction::isLoadedAudioStaticWaveData()) {
        if (trySuspend()) {
            setNerve(GET_NERVE_ANON(GameSystemStationedArchiveLoaderSuspended));
        } else {
            setNerve(GET_NERVE_ANON(GameSystemStationedArchiveLoaderLoadStationedArchiveOthers));
        }
    }
}

void GameSystemStationedArchiveLoader::exeLoadStationedArchivePlayer() {
    if (MR::isFirstStep(this)) {
        if (trySuspend()) {
            setNerve(GET_NERVE_ANON(GameSystemStationedArchiveLoaderSuspended));
            return;
        }

        mHeapHolder = new PlayerHeapHolder();

        MR::startFunctionAsyncExecute(MR::Functor(this, &GameSystemStationedArchiveLoader::startToLoadStationedArchivePlayer, true), 14,
                                      "常駐リソース読み込み");
    } else if (trySuspend()) {
        MR::suspendAsyncExecuteThread("常駐リソース読み込み");
        setNerve(GET_NERVE_ANON(GameSystemStationedArchiveLoaderSuspended));
        return;
    }

    if (MR::tryEndFunctionAsyncExecute("常駐リソース読み込み")) {
        createAndAddPlayerArchives(mHeapHolder->mIsDataMario);

        if (mHeapHolder != nullptr) {
            mHeapHolder->adjust();
        }

        setNerve(GET_NERVE_ANON(GameSystemStationedArchiveLoaderInitializeGameData));
    }
}

void GameSystemStationedArchiveLoader::exeLoadStationedArchiveOthers() {
    if (MR::isFirstStep(this)) {
        if (!tryAsyncExecuteIfNotSuspend(MR::Functor(this, &GameSystemStationedArchiveLoader::startToLoadStationedArchiveOthers),
                                         "常駐リソース読み込み")) {
            setNerve(GET_NERVE_ANON(GameSystemStationedArchiveLoaderSuspended));
            return;
        }
    } else if (trySuspend()) {
        MR::suspendAsyncExecuteThread("常駐リソース読み込み");
        setNerve(GET_NERVE_ANON(GameSystemStationedArchiveLoaderSuspended));
        return;
    }

    if (MR::tryEndFunctionAsyncExecute("常駐リソース読み込み")) {
        createAndAddOtherArchives();
        setNerve(GET_NERVE_ANON(GameSystemStationedArchiveLoaderLoadStationedArchivePlayer));
    }
}

void GameSystemStationedArchiveLoader::exeInitializeGameData() {
    GameSystemFunction::initAfterStationedResourceLoaded();
    SingletonHolder< HeapMemoryWatcher >::get()->setCurrentHeapToGameHeap();
    SingletonHolder< HeapMemoryWatcher >::get()->adjustStationedHeaps();
    GameSystemFunction::setSceneNameObjHolderToNameObjRegister();
    MR::clearFileLoaderRequestFileInfo(false);
    setNerve(GET_NERVE_ANON(GameSystemStationedArchiveLoaderEnd));
}

void GameSystemStationedArchiveLoader::exeEnd() {
}

void GameSystemStationedArchiveLoader::exeSuspended() {
}

void GameSystemStationedArchiveLoader::exeChangeArchivePlayer() {
    if (MR::isFirstStep(this)) {
        mHeapHolder->dispose();
        MR::startFunctionAsyncExecute(
            MR::Functor(this, &GameSystemStationedArchiveLoader::startToLoadStationedArchivePlayer, mHeapHolder->mIsDataMario), 14,
            "プレイヤーリソース読み込み");
    }

    if (MR::tryEndFunctionAsyncExecute("プレイヤーリソース読み込み")) {
        createAndAddPlayerArchives(mHeapHolder->mIsDataMario);
        setNerve(GET_NERVE_ANON(GameSystemStationedArchiveLoaderEnd));
    }
}

bool GameSystemStationedArchiveLoader::trySuspend() {
    if (!_C) {
        return false;
    }

    _C = true;

    return true;
}

bool GameSystemStationedArchiveLoader::tryAsyncExecuteIfNotSuspend(const MR::FunctorBase& rFunctor, const char* pName) {
    if (trySuspend()) {
        return false;
    }

    MR::startFunctionAsyncExecute(rFunctor, 14, pName);

    return true;
}

void GameSystemStationedArchiveLoader::startToLoadStationedArchivePlayer(bool a1) {
    StationedArchiveLoader::loadResourcesFromTable(*mHeapHolder->mCondition);
}

void GameSystemStationedArchiveLoader::startToLoadStationedArchiveOthers() {
    StationedArchiveLoader::loadResourcesFromTable(ConditionIfIsNotPlayer());
    StationedArchiveLoader::loadScenarioData(MR::getStationedHeapGDDR3());
}

void GameSystemStationedArchiveLoader::createAndAddPlayerArchives(bool isDataMario) {
    ConditionIsEqualType cond(isDataMario);
    StationedArchiveLoader::createAndAddResourcesFromTable(cond);

    ResourceHolder* pMarioAnimeHolder = MR::createAndAddResourceHolder("MarioAnime.arc");

    if (isDataMario) {
        pMarioAnimeHolder->newMaterialAnmBuffer(
            static_cast< J3DModelData* >(MR::createAndAddResourceHolder("Mario.arc")->mModelResTable->getRes("mario")));
    } else {
        pMarioAnimeHolder->newMaterialAnmBuffer(
            static_cast< J3DModelData* >(MR::createAndAddResourceHolder("Luigi.arc")->mModelResTable->getRes("luigi")));
    }
}

void GameSystemStationedArchiveLoader::createAndAddOtherArchives() {
    ConditionIfIsNotPlayer cond;
    StationedArchiveLoader::createAndAddResourcesFromTable(cond);
}

bool ConditionIsEqualType::isExecute(const MR::StationedFileInfo* pStationedFileInfo) const {
    return pStationedFileInfo->mLoadType == mLoadType;
}
