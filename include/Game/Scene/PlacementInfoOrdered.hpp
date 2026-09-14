#pragma once

#include "Game/Util/BothDirList.hpp"
#include "Game/Util/JMapInfo.hpp"
#include "Game/Util/StringUtil.hpp"

class NameObj;

typedef NameObj* (*CreationFuncPtr)(const char*);

class PlacementInfoOrdered {
public:
    class Identifier {
    public:
        Identifier() : mName(), mShapeId(-1) {
        }

        Identifier(const char* pName, s32 shapeId) : mName(pName), mShapeId(shapeId) {
        }

        bool operator==(const Identifier& rOther) const {
            return MR::isEqualString(mName, rOther.mName) && mShapeId == rOther.mShapeId;
        }

        /* 0x00 */ const char* mName;
        /* 0x04 */ s32 mShapeId;
    };

    class Index : public MR::BothDirLink< Index > {
    public:
        Index();

        ~Index();

        const JMapInfoIter& getInfoIter() const {
            return mInfoIter;
        }

        /* 0x10 */ JMapInfoIter mInfoIter;
    };

    class SameIdSet : public Identifier {
    public:
        SameIdSet();

        ~SameIdSet();

        bool operator<(const SameIdSet& rOther) const {
            if (mPriority == rOther.mPriority) {
                return rOther.mList.mCount < mList.mCount;
            }

            return mPriority < rOther.mPriority;
        }

        /* 0x08 */ s32 mPriority;
        /* 0x0C */ MR::BothDirList< Index > mList;
    };

    PlacementInfoOrdered(int);

    void sort();
    void requestFileLoad();
    void initPlacement();
    void insert(const Identifier&, const JMapInfoIter&);
    SameIdSet* getSameIdSet(int index) const {
        return mOrderedSetArray[index];
    }

    s32 getUsedArrayNum() const;
    SameIdSet* find(const Identifier&) const;
    SameIdSet* createSameIdSet(const Identifier&);
    Index* createIndex(const JMapInfoIter&);
    void attach(const JMapInfo*, PlacementInfoOrdered*);

    /* 0x00 */ Index* mIndexArray;
    /* 0x04 */ u32 mIndexCount;
    /* 0x08 */ SameIdSet* mSetArray;
    /* 0x0C */ SameIdSet** mOrderedSetArray;
    /* 0x10 */ int mCount;
};
