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

class PlayerHeapHolder {
public:
    PlayerHeapHolder();

    static JKRExpHeap* createHeap(u32, JKRHeap *);
    void adjust();
    void dispose();
    void setIsDataMario(bool);

    ConditionUsePlayerHeap* mCondition;         // 0x0
    JKRExpHeap* mNapaHeap;                      // 0x4
    JKRExpHeap* mGDDRHeap;                      // 0x8
    bool mIsDataMario;                          // 0xC
};