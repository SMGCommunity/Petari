#include "Game/System/GameSystemStationedArchiveLoader.hpp"
#include "Game/System/GameSystemFunction.hpp"
#include "Game/System/HeapMemoryWatcher.hpp"
#include "Game/System/StationedArchiveLoader.hpp"
#include "Game/System/ResourceHolder.hpp"
#include "Game/Util.hpp"
#include "Game/Util/ObjUtil.hpp"
#include "Game/Util/SystemUtil.hpp"

namespace {
    NEW_NERVE(GameSystemStationedArchiveLoaderLoadAudio1stWaveData, GameSystemStationedArchiveLoader, LoadAudio1stWaveData);
    NEW_NERVE(GameSystemStationedArchiveLoaderoadStationedArchivePlayer, GameSystemStationedArchiveLoader, LoadStationedArchivePlayer);
    NEW_NERVE(GameSystemStationedArchiveLoaderLoadStationedArchiveOthers, GameSystemStationedArchiveLoader, LoadStationedArchiveOthers);
    NEW_NERVE(GameSystemStationedArchiveLoaderInitializeGameData, GameSystemStationedArchiveLoader, InitializeGameData);
    NERVE_DECL_NULL(GameSystemStationedArchiveLoaderEnd);
    NERVE_DECL_NULL(GameSystemStationedArchiveLoaderSuspend);
    NEW_NERVE(GameSystemStationedArchiveLoaderChangeArchivePlayer, GameSystemStationedArchiveLoader, ChangeArchivePlayer);
};

bool ConditionIfIsNotPlayer::isExecute(const MR::StationedFileInfo *pInfo) const {
    bool result = false;

    if (pInfo->mLoadType != 2 && pInfo->mLoadType != 3) {
        result = true;
    }

    return result;
}

ConditionUsePlayerHeap::ConditionUsePlayerHeap() {
    mNapaHeap = nullptr;
    mGDDRHeap = nullptr;
    mIsDataMario = 1;
}

bool ConditionUsePlayerHeap::isExecute(const MR::StationedFileInfo *pInfo) const {
    s32 type = 3;

    if (mIsDataMario != 0) {
        type = 2;
    }

    return pInfo->mLoadType == type;
}

JKRExpHeap* ConditionUsePlayerHeap::getProperHeap(const MR::StationedFileInfo *pInfo) const {
    switch (pInfo->mHeapType) {
        case 0:
            return mNapaHeap;
            break;
        case 1:
            return mGDDRHeap;
            break;
        case 2:
            return nullptr;
            break;
    }

    return nullptr;
}

PlayerHeapHolder::PlayerHeapHolder() {
    mCondition = nullptr;
    mNapaHeap = nullptr;
    mGDDRHeap = nullptr;
    mIsDataMario = true;
    ConditionUsePlayerHeap* condition = new ConditionUsePlayerHeap();
    mNapaHeap = PlayerHeapHolder::createHeap(0x500000, (JKRHeap*)MR::getStationedHeapNapa());
    JKRExpHeap* gddr = PlayerHeapHolder::createHeap(0x500000, (JKRHeap*)MR::getStationedHeapGDDR3());
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

void PlayerHeapHolder::setIsDataMario(bool data) {
    mIsDataMario = data;
    mCondition->mIsDataMario = data;
}

JKRExpHeap* PlayerHeapHolder::createHeap(u32 size, JKRHeap* pParent) {
    if (pParent->getMaxAllocatableSize(0x10) < size) {
        size = -1;
    }

    return JKRExpHeap::create(size, pParent, true);
}

GameSystemStationedArchiveLoader::GameSystemStationedArchiveLoader() : NerveExecutor("常駐データ初期化") {
    mHeapHolder = nullptr;
    _C = 0;
    initNerve(&::GameSystemStationedArchiveLoaderLoadAudio1stWaveData::sInstance);
}

void GameSystemStationedArchiveLoader::update() {
    updateNerve();
}

bool GameSystemStationedArchiveLoader::isDone() const {
    return isNerve(&::GameSystemStationedArchiveLoaderEnd::sInstance);
}

// ....

void GameSystemStationedArchiveLoader::requestChangeArchivePlayer(bool a1) {
    if (mHeapHolder != nullptr && !isNerve(&::GameSystemStationedArchiveLoaderChangeArchivePlayer::sInstance)) {
        if (a1 != mHeapHolder->mIsDataMario) {
            mHeapHolder->setIsDataMario(a1);
            setNerve(&::GameSystemStationedArchiveLoaderChangeArchivePlayer::sInstance);
        }
    }
}

void GameSystemStationedArchiveLoader::exeLoadAudio1stWaveData() {
    if (MR::isFirstStep(this)) {
        GameSystemFunction::loadAudioStaticWaveData();
    }

    if (GameSystemFunction::isLoadedAudioStaticWaveData()) {
        if (trySuspend()) {
            setNerve(&::GameSystemStationedArchiveLoaderSuspend::sInstance);
        }
        else {
            setNerve(&::GameSystemStationedArchiveLoaderLoadStationedArchiveOthers::sInstance);
        }
    }
}

// GameSystemStationedArchiveLoader::exeLoadStationedArchivePlayer
// GameSystemStationedArchiveLoader::exeLoadStationedArchiveOther

void GameSystemStationedArchiveLoader::exeInitializeGameData() {
    GameSystemFunction::initAfterStationedResourceLoaded();
    SingletonHolder<HeapMemoryWatcher>::get()->setCurrentHeapToGameHeap();
    SingletonHolder<HeapMemoryWatcher>::get()->adjustStationedHeaps();
    GameSystemFunction::setSceneNameObjHolderToNameObjRegister();
    MR::clearFileLoaderRequestFileInfo(false);
    setNerve(&::GameSystemStationedArchiveLoaderEnd::sInstance);
}

// GameSystemStationedArchiveLoader::exeChangeArchivePlayer

bool GameSystemStationedArchiveLoader::trySuspend() {
    if (!_C) {
        return false;
    }

    _C = 1;
    return true;
}

bool GameSystemStationedArchiveLoader::tryAsyncExecuteIfNotSuspend(const MR::FunctorBase &rFunctor, const char *pName) {
    if (trySuspend()) {
        return false;
    }

    MR::startFunctionAsyncExecute(rFunctor, 0xE, pName);
    return true;
}

void GameSystemStationedArchiveLoader::startToLoadStationedArchivePlayer(bool a1) {
    StationedArchiveLoader::loadResourcesFromTable(*mHeapHolder->mCondition);
}

void GameSystemStationedArchiveLoader::startToLoadStationedArchiveOthers() {
    ConditionIfIsNotPlayer cond;
    StationedArchiveLoader::loadResourcesFromTable(cond);
    StationedArchiveLoader::loadScenarioData(MR::getStationedHeapGDDR3());
}

ConditionIfIsNotPlayer::ConditionIfIsNotPlayer() {

}

ConditionIfIsNotPlayer::~ConditionIfIsNotPlayer() {

}

void GameSystemStationedArchiveLoader::createAndAddPlayerArchives(bool a1) {
    ConditionIsEqualType cond(a1);
    StationedArchiveLoader::createAndAddResourcesFromTable(cond);
    ResourceHolder* marioAnimeHolder = MR::createAndAddResourceHolder("MarioAnime.arc");
    if (a1) {
        ResourceHolder* marioRes = MR::createAndAddResourceHolder("Mario.arc");
        J3DModelData* model = (J3DModelData*)marioRes->mModelResTable->getRes("mario");
        marioAnimeHolder->newMaterialAnmBuffer(model);
    }
    else {
        ResourceHolder* marioRes = MR::createAndAddResourceHolder("Luigi.arc");
        J3DModelData* model = (J3DModelData*)marioRes->mModelResTable->getRes("luigi");
        marioAnimeHolder->newMaterialAnmBuffer(model);
    }
}

ConditionIsEqualType::~ConditionIsEqualType() {
    
}

void GameSystemStationedArchiveLoader::createAndAddOtherArchives() {
    ConditionIfIsNotPlayer cond;
    StationedArchiveLoader::createAndAddResourcesFromTable(cond);
}
