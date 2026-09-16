#pragma once

#include "Game/System/NerveExecutor.hpp"
#include "Game/System/StationedArchiveLoader.hpp"
#include "Game/System/StationedFileInfo.hpp"

class JKRExpHeap;
class JKRHeap;
class PlayerHeapHolder;

namespace MR {
    class FunctorBase;
};  // namespace MR

class GameSystemStationedArchiveLoader : public NerveExecutor {
public:
    GameSystemStationedArchiveLoader();

    void update();
    bool isDone() const;
    bool isPreparedReset() const;
    void prepareReset();
    void requestChangeArchivePlayer(bool);
    void exeLoadAudio1stWaveData();
    void exeLoadStationedArchivePlayer();
    void exeLoadStationedArchiveOthers();
    void exeInitializeGameData();
    void exeEnd();
    void exeSuspended();
    void exeChangeArchivePlayer();
    bool trySuspend();
    bool tryAsyncExecuteIfNotSuspend(const MR::FunctorBase&, const char*);
    void startToLoadStationedArchivePlayer(bool);
    void startToLoadStationedArchiveOthers();
    void createAndAddPlayerArchives(bool);
    void createAndAddOtherArchives();

    /* 0x8 */ PlayerHeapHolder* mHeapHolder;
    /* 0xc */ bool _C;
};

class ConditionUsePlayerHeap : public StationedArchiveLoader::Condition {
public:
    ConditionUsePlayerHeap();

    virtual bool isExecute(const MR::StationedFileInfo*) const;
    virtual JKRHeap* getProperHeap(const MR::StationedFileInfo*) const;

    /* 0x4 */ JKRExpHeap* mNapaHeap;
    /* 0x8 */ JKRExpHeap* mGDDRHeap;
    /* 0xC */ bool mIsDataMario;
};

class ConditionIfIsNotPlayer : public StationedArchiveLoader::Condition {
public:
    virtual bool isExecute(const MR::StationedFileInfo*) const;
};

class ConditionIsEqualType : public StationedArchiveLoader::Condition {
public:
    ConditionIsEqualType(bool isDataMario) {
        MR::StationedFileInfo::LoadType var = MR::StationedFileInfo::LOAD_TYPE_MOUNT_RESOURCE_LUIGI;

        if (isDataMario) {
            var = MR::StationedFileInfo::LOAD_TYPE_MOUNT_RESOURCE_MARIO;
        }

        mLoadType = var;
    }

    virtual bool isExecute(const MR::StationedFileInfo*) const;

    /* 0x4 */ MR::StationedFileInfo::LoadType mLoadType;
};

class PlayerHeapHolder {
public:
    PlayerHeapHolder();

    void adjust();
    void dispose();
    void setIsDataMario(bool) NO_INLINE;

    static JKRExpHeap* createHeap(u32, JKRHeap*);

    /* 0x0 */ ConditionUsePlayerHeap* mCondition;
    /* 0x4 */ JKRExpHeap* mNapaHeap;
    /* 0x8 */ JKRExpHeap* mGDDRHeap;
    /* 0xC */ bool mIsDataMario;
};
