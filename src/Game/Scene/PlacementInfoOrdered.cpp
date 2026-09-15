#include "Game/Scene/PlacementInfoOrdered.hpp"
#include "Game/LiveActor/AllLiveActorGroup.hpp"
#include "Game/Map/PlanetMapCreator.hpp"
#include "Game/NameObj/ModelChangableObjFactory.hpp"
#include "Game/NameObj/NameObjFactory.hpp"
#include "Game/Util/JMapUtil.hpp"
#include "Game/Util/MemoryUtil.hpp"
#include "Game/Util/SceneUtil.hpp"

namespace {
    CreationFuncPtr getCreator(const PlacementInfoOrdered::Identifier& rIdentifier) {
        if (rIdentifier.mShapeId != -1) {
            return MR::getModelChangableObjCreator(rIdentifier.mName);
        }

        return NameObjFactory::getCreator(rIdentifier.mName);
    }
}  // namespace

PlacementInfoOrdered::PlacementInfoOrdered(int count) : mIndexArray(), mIndexCount(), mSetArray(), mOrderedSetArray(), mCount(count) {
    mIndexArray = new Index[count];
    mSetArray = new SameIdSet[count];
    mOrderedSetArray = new SameIdSet*[count];
    MR::zeroMemory(mOrderedSetArray, count * sizeof(SameIdSet*));
}

void PlacementInfoOrdered::sort() {
    s32 count = getUsedArrayNum();
    for (s32 i = 0; i < count; i++) {
        SameIdSet* pSet = mOrderedSetArray[i];
        if (NameObjFactory::isPlayerArchiveLoaderObj(pSet->mName)) {
            pSet->mPriority = 0;
        } else {
            const JMapInfoIter& rIter = static_cast< Index* >(pSet->mList.mHead->mValue)->mInfoIter;
            bool fromDVD;
            if (pSet->mShapeId != -1) {
                fromDVD = MR::isReadResourceFromDVDAtModelChangableObj(pSet->mName, pSet->mShapeId);
            } else {
                fromDVD = NameObjFactory::isReadResourceFromDVD(pSet->mName, rIter);
            }

            pSet->mPriority = (fromDVD != false) + 1;
        }
    }

    s32 gap = 13;
    while (gap < count) {
        gap = gap * 3 + 1;
    }

    for (gap /= 9; gap > 0; gap /= 3) {
        for (s32 i = gap; i < count; i++) {
            SameIdSet* pSet = mOrderedSetArray[i];
            s32 j = i - gap;
            while (j >= 0 && *pSet < *mOrderedSetArray[j]) {
                mOrderedSetArray[j + gap] = mOrderedSetArray[j];
                j -= gap;
            }

            mOrderedSetArray[j + gap] = pSet;
        }
    }
}

void PlacementInfoOrdered::requestFileLoad() {
    s32 count = getUsedArrayNum();
    for (s32 i = 0; i < count; i++) {
        SameIdSet* pSet = getSameIdSet(i);
        if (getCreator(*pSet) != nullptr) {
            if (pSet->mShapeId != -1) {
                MR::requestMountModelChangableObjArchives(pSet->mName, pSet->mShapeId);
            } else {
                for (MR::BothDirList< Index >::iterator iter = pSet->mList.begin(); !iter.isEnd(); iter++) {
                    NameObjFactory::requestMountObjectArchives(pSet->mName, iter->mInfoIter);
                }
            }
        }
    }
}

void PlacementInfoOrdered::initPlacement() {
    SameIdSet* pSet;
    NameObj* pObj;
    MR::startInitLiveActorSystemInfo();

    s32 count = getUsedArrayNum();
    for (s32 i = 0; i < count; i++) {
        pSet = mOrderedSetArray[i];
        CreationFuncPtr creator = getCreator(*pSet);
        if (creator != nullptr) {
            const char* pName = MR::getJapaneseObjectName(pSet->mName);
            for (MR::BothDirList< Index >::iterator iter = pSet->mList.begin(); !iter.isEnd(); iter++) {
                const JMapInfoIter& rIter = iter->getInfoIter();
                MR::setCurrentPlacementZoneId(MR::getPlacedZoneId(rIter));
                pObj = creator(pName);
                MR::initLiveActorSystemInfo(rIter);
                pObj->init(rIter);
                MR::initLiveActorSystemInfo(rIter);
                MR::clearCurrentPlacementZoneId();
            }
        }
    }
}

void PlacementInfoOrdered::insert(const Identifier& rIdentifier, const JMapInfoIter& rIter) {
    SameIdSet* pSet = find(rIdentifier);
    if (pSet == nullptr) {
        pSet = createSameIdSet(rIdentifier);
    }

    pSet->mList.append(createIndex(rIter));
}

s32 PlacementInfoOrdered::getUsedArrayNum() const {
    s32 count = 0;
    s32 i = 0;
    while (i < mCount && mOrderedSetArray[i] != nullptr) {
        i++;
        count++;
    }

    return count;
}

PlacementInfoOrdered::SameIdSet* PlacementInfoOrdered::find(const Identifier& rIdentifier) const {
    s32 count = getUsedArrayNum();
    for (s32 i = 0; i < count; i++) {
        SameIdSet* pSet = mOrderedSetArray[i];
        if (pSet != nullptr && *pSet == rIdentifier) {
            return pSet;
        }
    }

    return nullptr;
}

PlacementInfoOrdered::SameIdSet* PlacementInfoOrdered::createSameIdSet(const Identifier& rIdentifier) {
    s32 index = getUsedArrayNum();
    SameIdSet* pSet = &mSetArray[index];
    mOrderedSetArray[index] = pSet;
    *static_cast< Identifier* >(pSet) = rIdentifier;
    return pSet;
}

PlacementInfoOrdered::Index* PlacementInfoOrdered::createIndex(const JMapInfoIter& rIter) {
    Index* pIndex = &mIndexArray[mIndexCount];
    mIndexCount++;
    pIndex->mInfoIter = rIter;
    return pIndex;
}

PlacementInfoOrdered::Index::Index() : MR::BothDirLink< Index >(this), mInfoIter() {
}

PlacementInfoOrdered::Index::~Index() {
}

PlacementInfoOrdered::SameIdSet::SameIdSet() : Identifier(), mList() {
}

PlacementInfoOrdered::SameIdSet::~SameIdSet() {
}

void PlacementInfoOrdered::attach(const JMapInfo* pInfo, PlacementInfoOrdered* pAfterScenario) {
    s32 count = pInfo->getNumEntries();
    for (s32 i = 0; i < count; i++) {
        JMapInfoIter iter(pInfo, i);
        const char* pName = "";
        MR::getObjectName(&pName, iter);
        s32 shapeId = -1;
        MR::getJMapInfoShapeIdWithInit(iter, &shapeId);
        Identifier identifier(pName, shapeId);
        if (pAfterScenario != nullptr && shapeId == -1 && PlanetMapCreatorFunction::isLoadArchiveAfterScenarioSelected(pName)) {
            pAfterScenario->insert(identifier, iter);
        } else {
            insert(identifier, iter);
        }
    }
}
