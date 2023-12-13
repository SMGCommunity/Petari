#pragma once

#include "Game/System/StationedArchiveLoader.hpp"
#include <JSystem/JKernel/JKRHeap.hpp>
#include <JSystem/JKernel/JKRExpHeap.hpp>

class ConditionUsePlayerHeap : public StationedArchiveLoader::Condition {
public:
    ConditionUsePlayerHeap();

    virtual ~ConditionUsePlayerHeap();
    virtual bool isExecute(const MR::StationedFileInfo *) const;
    virtual JKRExpHeap* getProperHeap(const MR::StationedFileInfo *) const;

    JKRExpHeap* mNapaHeap;     // _4
    JKRExpHeap* mGDDRHeap;     // _8
    bool mIsDataMario;      // _C
};

class ConditionIfIsNotPlayer : public StationedArchiveLoader::Condition {
public:
    ConditionIfIsNotPlayer();

    virtual ~ConditionIfIsNotPlayer();
    virtual bool isExecute(const MR::StationedFileInfo *) const;
    virtual JKRExpHeap* getProperHeap(const MR::StationedFileInfo *) const;

};

class PlayerHeapHolder {
public:
    PlayerHeapHolder();

    static JKRExpHeap* createHeap(u32, JKRHeap *);
    void adjust();
    void dispose();
    void setIsDataMario(bool);

    ConditionUsePlayerHeap* mCondition;         // _0
    JKRExpHeap* mNapaHeap;                      // _4
    JKRExpHeap* mGDDRHeap;                      // _8
    bool mIsDataMario;                          // _C
};