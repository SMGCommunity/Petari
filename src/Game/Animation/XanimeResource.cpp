#include "Game/Animation/XanimeResource.hpp"
#include "Game/System/ResourceHolder.hpp"
#include "Game/System/ResourceInfo.hpp"
#include "Game/Util/HashUtil.hpp"
#include "Game/Util/StringUtil.hpp"
#include "revolution/types.h"
#include <cstring>

static bool unknownByte;
XanimeResourceTable::XanimeResourceTable(ResourceHolder* pArg) {
    init();

    _0 = 0;
    mGroupInfos = nullptr;
    mResourceHolder = pArg;
    mAmountOfBckTables = 0;
    mAmountOfGroupInfos = 0;

    mMaxGroupInfoTableSize = 1;

    if (unknownByte != 0) {
        mMaxGroupInfoTableSize = unknownByte;
    }

    _1C.init();

    _1C.mRate = 1.0f;
    _1C.mParent.mAnimationName = "non-group";
    _1C._8 = 1;
    _1C.mBckTableVariant = 1;
    _1C._1D = 0;
}

XanimeResourceTable::XanimeResourceTable(ResourceHolder* pResourceHolder, XanimeGroupInfo* pGroupInfo, XanimeAuxInfo* pAuxInfo,
                                         XanimeOfsInfo* pOfsInfo, XanimeBckTable* pBckTable1, XanimeBckTable2* pBckTable2,
                                         XanimeBckTable3* pBckTable3, XanimeBckTable4* pBckTable4, XanimeSwapTable* pSwapTable) {
    init();
    _0 = 1;
    mGroupInfos = pGroupInfo;
    mResourceHolder = pResourceHolder;
    mMaxGroupInfoTableSize = 1;
    mAmountOfGroupInfos = initGroupInfo(pResourceHolder, pGroupInfo, pAuxInfo, pOfsInfo, pBckTable1, pBckTable2, pBckTable3, pBckTable4, pSwapTable);
    createSortTable();

    mBckTables = reinterpret_cast< XanimeBckTable1* >(pBckTable1);

    int i = 0;
    while (mBckTables != nullptr) {
        if (mBckTables[i].mParent.mAnimationName[0] == '\0') {
            break;
        }
        mBckTables[i].mAnimationHash = MR::getHashCode(mBckTables[i].mParent.mAnimationName);
        mBckTables[i].mFileHash = MR::getHashCode(mBckTables[i].mFileName);

        i++;
    }
    mAmountOfBckTables = i;

    _1C.init();
    _1C.mRate = 1.0f;
    _1C.mParent.mAnimationName = "non-group";
    _1C._8 = 1;
    _1C.mBckTableVariant = 1;
    _1C._1D = 0;
    mSimpleGroupInfos = new XanimeGroupInfo[0];
    mAmountOfSimpleGroupInfos = 0;
}

void XanimeResourceTable::init() {
    _0 = 0;
    mMaxGroupInfoTableSize = 0;
    mAmountOfGroupInfos = 0;
    mAmountOfBckTables = 0;
    mAmountOfSimpleGroupInfos = 0;
    mGroupInfos = nullptr;
    mDirectories = nullptr;
    mBckTables = nullptr;

    mResourceHolder = nullptr;
    mSortTable = nullptr;
    mSwapTable = nullptr;
}

// While pBckTable1 is an XanimeBckTable*, all seems to indicate it is actually an XanimeBckTable1*
u32 XanimeResourceTable::initGroupInfo(ResourceHolder* pResourceHolder, XanimeGroupInfo* pInfo, XanimeAuxInfo* pAuxInfo, XanimeOfsInfo* pOfsInfo,
                                       XanimeBckTable* pBckTable1, XanimeBckTable2* pBckTable2, XanimeBckTable3* pBckTable3,
                                       XanimeBckTable4* pBckTable4, XanimeSwapTable* pSwapTable) {
    mSwapTable = pSwapTable;
    int i = 0;
    u32 maxTableSize = 1;
    while (true) {
        XanimeGroupInfo* entry = &pInfo[i];
        if (entry->mParent.mAnimationName[0] == '\0') {
            break;
        }

        for (int j = 0; j < ARRAY_SIZE(entry->_20); j++) {
            entry->_20[j] = nullptr;
            entry->mWeights[j] = 0.0f;
        }

        entry->mHash = MR::getHashCode(entry->mParent.mAnimationName);

        XanimeBckTable* bckTables[4];
        bckTables[3] = reinterpret_cast< XanimeBckTable* >(pBckTable1);
        bckTables[2] = reinterpret_cast< XanimeBckTable* >(pBckTable2);
        bckTables[1] = reinterpret_cast< XanimeBckTable* >(pBckTable3);
        bckTables[0] = reinterpret_cast< XanimeBckTable* >(pBckTable4);

        entry->mBckTableVariant = 0;

        const char* firstFilename = nullptr;
        if (search(&bckTables[3], entry->mParent.mAnimationName, sizeof(XanimeBckTable1))) {
            entry->mBckTableVariant = 1;

            entry->mWeights[0] = 1.0f;

            pResourceHolder->mMotionResTable->findFileInfo(reinterpret_cast< XanimeBckTable1* >(bckTables[3])->mFileName);

            entry->_20[0] = findResMotion(reinterpret_cast< XanimeBckTable1* >(bckTables[3])->mFileName);

            firstFilename = reinterpret_cast< XanimeBckTable1* >(bckTables[3])->mFileName;
        } else if (search(&bckTables[2], entry->mParent.mAnimationName, sizeof(XanimeBckTable2))) {
            if (maxTableSize < 2) {
                maxTableSize = 2;
            }
            entry->mBckTableVariant = 2;

            entry->_20[0] = findResMotion(reinterpret_cast< XanimeBckTable2* >(bckTables[2])->mEntries[0].mFileName);
            entry->mWeights[0] = reinterpret_cast< XanimeBckTable2* >(bckTables[2])->mEntries[0]._4;

            entry->_20[1] = findResMotion(reinterpret_cast< XanimeBckTable2* >(bckTables[2])->mEntries[1].mFileName);
            entry->mWeights[1] = reinterpret_cast< XanimeBckTable2* >(bckTables[2])->mEntries[1]._4;

            firstFilename = reinterpret_cast< XanimeBckTable2* >(bckTables[2])->mEntries[0].mFileName;
        } else if (search(&bckTables[1], entry->mParent.mAnimationName, sizeof(XanimeBckTable3))) {
            if (maxTableSize < 3) {
                maxTableSize = 3;
            }
            entry->mBckTableVariant = 3;

            entry->_20[0] = findResMotion(reinterpret_cast< XanimeBckTable3* >(bckTables[1])->mEntries[0].mFileName);
            entry->mWeights[0] = reinterpret_cast< XanimeBckTable3* >(bckTables[1])->mEntries[0]._4;

            entry->_20[1] = findResMotion(reinterpret_cast< XanimeBckTable3* >(bckTables[1])->mEntries[1].mFileName);
            entry->mWeights[1] = reinterpret_cast< XanimeBckTable3* >(bckTables[1])->mEntries[1]._4;

            entry->_20[2] = findResMotion(reinterpret_cast< XanimeBckTable3* >(bckTables[1])->mEntries[2].mFileName);
            entry->mWeights[2] = reinterpret_cast< XanimeBckTable3* >(bckTables[1])->mEntries[2]._4;

            firstFilename = reinterpret_cast< XanimeBckTable3* >(bckTables[1])->mEntries[0].mFileName;
        } else if (search(&bckTables[0], entry->mParent.mAnimationName, sizeof(XanimeBckTable4))) {
            if (maxTableSize < 4) {
                maxTableSize = 4;
            }
            entry->mBckTableVariant = 4;

            entry->_20[0] = findResMotion(reinterpret_cast< XanimeBckTable4* >(bckTables[0])->mEntries[0].mFileName);
            entry->mWeights[0] = reinterpret_cast< XanimeBckTable4* >(bckTables[0])->mEntries[0]._4;

            entry->_20[1] = findResMotion(reinterpret_cast< XanimeBckTable4* >(bckTables[0])->mEntries[1].mFileName);
            entry->mWeights[1] = reinterpret_cast< XanimeBckTable4* >(bckTables[0])->mEntries[1]._4;

            entry->_20[2] = findResMotion(reinterpret_cast< XanimeBckTable4* >(bckTables[0])->mEntries[2].mFileName);
            entry->mWeights[2] = reinterpret_cast< XanimeBckTable4* >(bckTables[0])->mEntries[2]._4;

            entry->_20[3] = findResMotion(reinterpret_cast< XanimeBckTable4* >(bckTables[0])->mEntries[3].mFileName);
            entry->mWeights[3] = reinterpret_cast< XanimeBckTable4* >(bckTables[0])->mEntries[3]._4;

            firstFilename = reinterpret_cast< XanimeBckTable4* >(bckTables[0])->mEntries[0].mFileName;
        }
        entry->mBckName = firstFilename;

        XanimeBckTable* auxTables[1];
        auxTables[0] = reinterpret_cast< XanimeBckTable* >(pAuxInfo);
        if (search(auxTables, entry->mParent.mAnimationName, sizeof(XanimeAuxInfo))) {
            entry->_1D = reinterpret_cast< XanimeAuxInfo* >(auxTables[0])->_4;
            entry->_40 = &(reinterpret_cast< XanimeAuxInfo* >(auxTables[0])->_8);
        } else {
            entry->_1D = 0;
            entry->_40 = nullptr;
        }

        entry->mStart = 0.0f;
        if (entry->_20[0] == nullptr) {
            entry->mEnd = 0.0f;
            entry->mLoop = 0.0f;
            entry->mAttribute = 0;
        } else {
            // Unsure of what is happening there.
            entry->mAttribute = static_cast< u8* >(entry->_20[0])[4];
            entry->mLoop = 0.0f;
            entry->mEnd = static_cast< f32 >(reinterpret_cast< const s16* >(entry->_20[0])[3]);
        }

        XanimeBckTable* ofsTables[1];
        ofsTables[0] = reinterpret_cast< XanimeBckTable* >(pOfsInfo);

        if (search(ofsTables, entry->mParent.mAnimationName, sizeof(XanimeOfsInfo))) {
            entry->mStart = reinterpret_cast< XanimeOfsInfo* >(ofsTables[0])->_4;
            entry->mLoop = reinterpret_cast< XanimeOfsInfo* >(ofsTables[0])->_C;
            entry->mEnd = reinterpret_cast< XanimeOfsInfo* >(ofsTables[0])->_8;
            entry->mAttribute = reinterpret_cast< XanimeOfsInfo* >(ofsTables[0])->_10;
        }

        i++;
    }
    mMaxGroupInfoTableSize = maxTableSize;

    return i;
}

const XanimeGroupInfo* XanimeResourceTable::getGroupInfo(const char* pArg) const {
    switch (_0) {
    case 0:
        return nullptr;

    case 1:
        s32 groupIndex = getGroupIndex(pArg);
        if (groupIndex == -1) {
            s32 simpleIndex = getSimpleIndex(pArg);
            if (simpleIndex == -1) {
                return nullptr;
            }

            return &mSimpleGroupInfos[simpleIndex];
        }

        return &mGroupInfos[groupIndex];

    case 2:
        return getGroupInfo(pArg, mDirectories);
    }

    return nullptr;
}

const XanimeGroupInfo* XanimeResourceTable::getGroupInfo(const char* pPath, XanimeDirectory* pDir) const {
    int i = 0;
    while (true) {
        i++;

        if (pPath[i] == '\0') {
            break;
        }

        if (pPath[i] == '/') {
            char pathOneDirectoryDown[32];
            MR::extractString(pathOneDirectoryDown, &pPath[i], i, 32);

            u32 index = getIndex(pDir, pathOneDirectoryDown);

            switch (pDir[index].mDirectoryType) {
            case XanimeDirectory::Recursive:
                return getGroupInfo(&pPath[i + 1], pDir[index].mSubDirectories);

            case XanimeDirectory::Leaf:
                u32 hash = MR::getHashCode(&pPath[i + 1]);
                XanimeGroupInfo* infos = pDir[index].mSubInformations;

                for (int j = 0; j < pDir[index].mSize; j++) {
                    if (infos[j].mHash == hash) {
                        return &infos[j];
                    }
                }

                return nullptr;
            }
        }
    }

    i = 0;
    u32 hash = MR::getHashCode(pPath);
    const XanimeGroupInfo* result = nullptr;

    while (true) {
        XanimeDirectory* entry = &pDir[i];
        if (entry->mName[0] == '\0') {
            break;
        }

        switch (entry->mDirectoryType) {
        case XanimeDirectory::Recursive:
            result = getGroupInfo(pPath, entry->mSubDirectories);
            break;
        case XanimeDirectory::Leaf:
            XanimeGroupInfo* subInfo = entry->mSubInformations;
            for (int j = 0; j < entry->mSize; j++) {
                if (subInfo[j].mHash == hash) {
                    return &entry->mSubInformations[j];
                }
            }
            break;
        }

        if (result != nullptr) {
            return result;
        }
        i++;
    }

    return nullptr;
}

u32 XanimeResourceTable::getIndex(XanimeDirectory* pDir, const char* pTarget) const {
    int i = 0;
    u32 hash = MR::getHashCode(pTarget);

    while (true) {
        if (pDir[i].mName[0] == '\0') {
            return -1;
        }

        if (hash == pDir[i].mHash) {
            return i;
        }

        i++;
    }
}

u32 XanimeResourceTable::getIndexFromHash(u32 hash) const {
    if (mSortTable != nullptr) {
        u32 index;
        if (mSortTable->search(hash, &index)) {
            return index;
        }
    }

    return -1;
}

u32 XanimeResourceTable::getGroupIndex(const char* pTarget) const {
    u32 hash = MR::getHashCode(pTarget);

    switch (_0) {
    case 0:
        return -1;
    case 1:
        u32 position;
        if (mSortTable->search(hash, &position)) {
            return position;
        }
    case 2:
        break;
    }

    return -1;
}

XanimeGroupInfo* XanimeResourceTable::getGroupInfoFromHash(u32 hash) const {
    s32 index = getIndexFromHash(hash);
    if (index == -1) {
        return 0;
    }

    return &mGroupInfos[index];
}

u32 XanimeResourceTable::getSingleIndex(const char* pTarget) const {
    if (mBckTables == nullptr) {
        return -1;
    }
    u32 hash = MR::getHashCode(pTarget);

    for (int i = 0; i < mAmountOfBckTables; i++) {
        if (hash == mBckTables[i].mAnimationHash) {
            return i;
        }
    }

    return -1;
}

u32 XanimeResourceTable::getSimpleIndex(const char* pTarget) const {
    u32 hash = MR::getHashCode(pTarget);

    for (int i = 0; i < mAmountOfSimpleGroupInfos; i++) {
        if (hash == mSimpleGroupInfos[i].mHash) {
            return i;
        }
    }

    return -1;
}

void XanimeResourceTable::createSortTable() {
    mSortTable = new HashSortTable(mAmountOfGroupInfos);
    for (int i = 0; i < mAmountOfGroupInfos; i++) {
        mSortTable->add(mGroupInfos[i].mHash, i);
    }
    mSortTable->sort();
}

void* XanimeResourceTable::findResMotion(const char* pTarget) const {
    const char* newName = swapBckName(pTarget, mSwapTable);
    if (!mResourceHolder->mMotionResTable->isExistRes(newName)) {
        return nullptr;
    }
    return mResourceHolder->mMotionResTable->getRes(newName);
}

const char* XanimeResourceTable::findStringMotion(const char* pArg) const {
    return mResourceHolder->mMotionResTable->findFileInfo(pArg)->mName;
}

const char* XanimeResourceTable::swapBckName(const char* pOriginal, XanimeSwapTable* pTable) const {
    if (pTable == nullptr) {
        return pOriginal;
    }

    int i = 0;

    while (true) {
        if (pTable[i].mOriginal[0] == '\0') {
            return pOriginal;
        }

        if (MR::strcasecmp(pTable[i].mOriginal, pOriginal) == 0) {
            return pTable[i].mSwapped;
        }
        i++;
    }
}

bool XanimeResourceTable::search(XanimeBckTable** pArray, const char* pTarget, u32 sizeOfArrayElements) const {
    if (pArray == nullptr) {
        return false;
    }

    if (pArray[0] == nullptr) {
        return false;
    }

    while (true) {
        const char* string = pArray[0]->mAnimationName;
        if (strcmp(string, pTarget) == 0) {
            return true;
        }

        if (pArray[0]->mAnimationName[0] == '\0') {
            return false;
        }

        // Possibly fake pointer arithmetic. It appears this line pops the first element of the array pointed at by pArray, which has a variable size
        // equal to sizeOfArrayElements (polymorphism)
        pArray[0] = reinterpret_cast< XanimeBckTable* >(reinterpret_cast< u8* >(pArray[0]) + sizeOfArrayElements);
    }
}

void XanimeGroupInfo::init() {
    mRate = 0.0f;
    mParent.mAnimationName = "not-initialized";
    _8 = 0;
    mStart = 0.0f;
    mEnd = 0.0f;
    mLoop = 0.0f;
    mAttribute = 0;
    mBckTableVariant = 0;
    _1D = 0;
    for (u32 i = 0; i < ARRAY_SIZE(_20); i++) {
        mWeights[i] = 0.0f;
        _20[i] = nullptr;
    }
    _40 = nullptr;
    mHash = 0;
}
