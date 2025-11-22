#pragma once

#include "Game/System/NerveExecutor.hpp"
#include "Game/System/StationedArchiveLoader.hpp"

class JKRHeap;
class JKRExpHeap;
class PlayerHeapHolder;

namespace MR {
    class FunctorBase;
};

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
    ConditionIsEqualType(bool b) {
        s32 var = 3;

        if (b) {
            var = 2;
        }

        _4 = var;
    }

    virtual bool isExecute(const MR::StationedFileInfo*) const;

    /* 0x4 */ s32 _4;
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
