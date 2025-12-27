#pragma once

#include "Game/Util.hpp"

typedef NameObj* (*CreationFuncPtr)(const char*);

class PlacementInfoOrdered {
public:
    class Index {
    public:
        Index();
        ~Index();

        MR::BothDirPtrLink mLink;  // 0x0
        u32 _10;
        s32 _14;
    };

    class SameIdSet {
    public:
        SameIdSet();
        ~SameIdSet();

        u32 _0;
        s32 _4;
        u32 _8;
        MR::BothDirPtrList mList;  // 0xC
    };

    class Identifier {
    public:
        const char* mName;  // 0x0
        s32 _4;
        u32 _8;
        u32 _C;
        JMapInfoIter mInfoIter;  // 0x10
    };

    PlacementInfoOrdered(int);

    void sort();
    void requestFileLoad();
    void initPlacement();
    void insert(const Identifier&, const JMapInfoIter&);
    u32 getUsedArrayNum() const;
    SameIdSet* find(const Identifier&) const;
    SameIdSet* createSameIdSet(const Identifier&);
    Index* createIndex(const JMapInfoIter&);

    void attach(const JMapInfo*, PlacementInfoOrdered*);

    Index* mIndexArray;  // 0x0
    u32 _4;
    SameIdSet* mSetArray;          // 0x8
    Identifier** mIdentiferArray;  // 0xC
    int mCount;                    // 0x10
};
