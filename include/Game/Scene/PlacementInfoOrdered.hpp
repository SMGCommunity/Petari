#pragma once

#include "Game/Util.h"

typedef NameObj* (*CreationFuncPtr)(const char *);

class PlacementInfoOrdered {
public:
    class Index {
    public:
        Index();
        ~Index();

        MR::BothDirPtrLink mLink;   // _0
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
        MR::BothDirPtrList mList;   // _C
    };

    class Identifier {
    public:
        const char* mName;      // _0
        s32 _4;
        u32 _8;
        u32 _C;
        JMapInfoIter mInfoIter; // _10
    };

    PlacementInfoOrdered(int);

    void sort();
    void requestFileLoad();
    void initPlacement();
    void insert(const Identifier &, const JMapInfoIter &);
    u32 getUsedArrayNum() const;
    SameIdSet* find(const Identifier &) const;
    SameIdSet* createSameIdSet(const Identifier &);
    Index* createIndex(const JMapInfoIter &);

    Index* mIndexArray;             // _0
    u32 _4;
    SameIdSet* mSetArray;           // _8
    Identifier** mIdentiferArray;   // _C
    int mCount;                     // _10
};

namespace {
    CreationFuncPtr getCreator(const PlacementInfoOrdered::Identifier &);
};
