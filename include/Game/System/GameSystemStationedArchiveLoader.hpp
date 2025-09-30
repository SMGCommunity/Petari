#pragma once

#include "Game/System/StationedArchiveLoader.hpp"
#include "Game/System/NerveExecutor.hpp"
#include "Game/Util/Functor.hpp"
#include <JSystem/JKernel/JKRHeap.hpp>
#include <JSystem/JKernel/JKRExpHeap.hpp>

class ConditionUsePlayerHeap : public StationedArchiveLoader::Condition {
public:
    ConditionUsePlayerHeap();

    virtual ~ConditionUsePlayerHeap();
    virtual bool isExecute(const MR::StationedFileInfo *) const;
    virtual JKRExpHeap* getProperHeap(const MR::StationedFileInfo *) const;

    JKRExpHeap* mNapaHeap;     // 0x4
    JKRExpHeap* mGDDRHeap;     // 0x8
    bool mIsDataMario;      // 0xC
};

class ConditionIfIsNotPlayer : public StationedArchiveLoader::Condition {
public:
    ConditionIfIsNotPlayer();

    virtual ~ConditionIfIsNotPlayer();
    virtual bool isExecute(const MR::StationedFileInfo *) const;
    virtual JKRExpHeap* getProperHeap(const MR::StationedFileInfo *) const;

};

class ConditionIsEqualType : public StationedArchiveLoader::Condition {
public:
    inline ConditionIsEqualType(bool val) {
        s32 var = 3;
        if (val) {
            var = 2;
        }

        _4 = var;
    }

    virtual ~ConditionIsEqualType();
    virtual bool isExecute(const MR::StationedFileInfo *) const;

    s32 _4;
};

class PlayerHeapHolder {
public:
    PlayerHeapHolder();

    static JKRExpHeap* createHeap(u32, JKRHeap *);
    void adjust();
    void dispose();
    void setIsDataMario(bool) NO_INLINE;

    ConditionUsePlayerHeap* mCondition;         // 0x0
    JKRExpHeap* mNapaHeap;                      // 0x4
    JKRExpHeap* mGDDRHeap;                      // 0x8
    bool mIsDataMario;                          // 0xC
};

class GameSystemStationedArchiveLoader : public NerveExecutor {
public:
    GameSystemStationedArchiveLoader();

    virtual ~GameSystemStationedArchiveLoader();

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
    bool tryAsyncExecuteIfNotSuspend(const MR::FunctorBase &, const char *);
    void startToLoadStationedArchivePlayer(bool);
    void startToLoadStationedArchiveOthers();
    void createAndAddPlayerArchives(bool);
    void createAndAddOtherArchives();

    PlayerHeapHolder* mHeapHolder;              // 0x8
    u8 _C;
};
