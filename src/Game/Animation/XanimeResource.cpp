#include "Game/Animation/XanimeResource.hpp"
#include "Game/System/ResourceHolder.hpp"
#include "Game/System/ResourceInfo.hpp"
#include "Game/Util/HashUtil.hpp"
#include "Game/Util/StringUtil.hpp"
#include <cstring>

static bool unknownByte;
XanimeResourceTable::XanimeResourceTable(ResourceHolder* pArg) {
    init();

    _0 = 0;
    _10 = 0;
    mResourceHolder = pArg;
    m_14Size = 0;
    m_10Size = 0;

    mMaxGroupInfoTableSize = 1;

    if (unknownByte) {
        mMaxGroupInfoTableSize = unknownByte;
    }

    _1C.init();

    _1C._4 = 1.0f;
    _1C.mParent.animationName = "non-group";
    _1C._8 = 1;
    _1C.mTableSize = 1;
    _1C._1D = 0;
}

XanimeResourceTable::XanimeResourceTable(ResourceHolder* pArg1, XanimeGroupInfo* pArg2, XanimeAuxInfo* pArg3, XanimeOfsInfo* pArg4,
                                         XanimeBckTable* pArg5, XanimeBckTable2* pArg6, XanimeBckTable3* pArg7, XanimeBckTable4* pArg8,
                                         XanimeSwapTable* pArg9) {
    init();
    _0 = 1;
    _10 = pArg2;
    mResourceHolder = pArg1;
    mMaxGroupInfoTableSize = 1;
    m_10Size = initGroupInfo(pArg1, pArg2, pArg3, pArg4, pArg5, pArg6, pArg7, pArg8, pArg9);
    createSortTable();

    _18 = reinterpret_cast< XanimeBckTable1* >(pArg5);

    int i = 0;
    while (_18 != nullptr) {
        if (_18[i].animationName[0] == '\0') {
            break;
        }
        _18[i].animationHash = MR::getHashCode(_18[i].animationName);
        _18[i].fileHash = MR::getHashCode(_18[i].fileName);

        i++;
    }
    m_14Size = i;

    _1C.init();
    _1C._4 = 1.0f;
    _1C.mParent.animationName = "non-group";
    _1C._8 = 1;
    _1C.mTableSize = 1;
    _1C._1D = 0;
    _68 = new XanimeGroupInfo[0];
    m_68Size = 0;
}

void XanimeResourceTable::init() {
    _0 = 0;
    mMaxGroupInfoTableSize = 0;
    m_10Size = 0;
    m_14Size = 0;
    m_68Size = 0;
    _10 = nullptr;
    _14 = nullptr;
    _18 = nullptr;

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
        if (entry->mParent.animationName[0] == '\0') {
            break;
        }

        for (int j = 0; j < 4; j++) {
            entry->_20[j] = nullptr;
            entry->_30[j] = 0.0f;
        }

        entry->mHash = MR::getHashCode(entry->mParent.animationName);

        XanimeBckTable* bckTables[4];
        bckTables[3] = pBckTable1;
        bckTables[2] = pBckTable2;
        bckTables[1] = pBckTable3;
        bckTables[0] = pBckTable4;

        entry->mTableSize = 0;

        const char* firstFilename = nullptr;
        if (search(&bckTables[3], entry->mParent.animationName, sizeof(XanimeBckTable1))) {
            entry->mTableSize = 1;

            entry->_30[0] = 1.0f;

            pResourceHolder->mMotionResTable->findFileInfo(static_cast< XanimeBckTable1* >(bckTables[3])->fileName);

            entry->_20[0] = findResMotion(static_cast< XanimeBckTable1* >(bckTables[3])->fileName);

            firstFilename = static_cast< XanimeBckTable1* >(bckTables[3])->fileName;
        } else if (search(&bckTables[2], entry->mParent.animationName, sizeof(XanimeBckTable2))) {
            if (maxTableSize < 2) {
                maxTableSize = 2;
            }
            entry->mTableSize = 2;

            entry->_20[0] = findResMotion(static_cast< XanimeBckTable2* >(bckTables[2])->fileName1);
            entry->_30[0] = static_cast< XanimeBckTable2* >(bckTables[2])->_8;

            entry->_20[1] = findResMotion(static_cast< XanimeBckTable2* >(bckTables[2])->fileName2);
            entry->_30[1] = static_cast< XanimeBckTable2* >(bckTables[2])->_10;

            firstFilename = static_cast< XanimeBckTable2* >(bckTables[2])->fileName1;
        } else if (search(&bckTables[1], entry->mParent.animationName, sizeof(XanimeBckTable3))) {
            if (maxTableSize < 3) {
                maxTableSize = 3;
            }
            entry->mTableSize = 3;

            entry->_20[0] = findResMotion(static_cast< XanimeBckTable3* >(bckTables[1])->fileName1);
            entry->_30[0] = static_cast< XanimeBckTable3* >(bckTables[1])->_8;

            entry->_20[1] = findResMotion(static_cast< XanimeBckTable3* >(bckTables[1])->fileName2);
            entry->_30[1] = static_cast< XanimeBckTable3* >(bckTables[1])->_10;

            entry->_20[2] = findResMotion(static_cast< XanimeBckTable3* >(bckTables[1])->fileName3);
            entry->_30[2] = static_cast< XanimeBckTable3* >(bckTables[1])->_18;

            firstFilename = static_cast< XanimeBckTable3* >(bckTables[1])->fileName1;
        } else if (search(&bckTables[0], entry->mParent.animationName, sizeof(XanimeBckTable4))) {
            if (maxTableSize < 4) {
                maxTableSize = 4;
            }
            entry->mTableSize = 4;

            entry->_20[0] = findResMotion(static_cast< XanimeBckTable4* >(bckTables[0])->fileName1);
            entry->_30[0] = static_cast< XanimeBckTable4* >(bckTables[0])->_8;

            entry->_20[1] = findResMotion(static_cast< XanimeBckTable4* >(bckTables[0])->fileName2);
            entry->_30[1] = static_cast< XanimeBckTable4* >(bckTables[0])->_10;

            entry->_20[2] = findResMotion(static_cast< XanimeBckTable4* >(bckTables[0])->fileName3);
            entry->_30[2] = static_cast< XanimeBckTable4* >(bckTables[0])->_18;

            entry->_20[3] = findResMotion(static_cast< XanimeBckTable4* >(bckTables[0])->fileName4);
            entry->_30[3] = static_cast< XanimeBckTable4* >(bckTables[0])->_20;

            firstFilename = static_cast< XanimeBckTable4* >(bckTables[0])->fileName1;
        }
        entry->_48 = firstFilename;

        XanimeBckTable* auxTables[1];
        auxTables[0] = pAuxInfo;
        if (search(auxTables, entry->mParent.animationName, sizeof(XanimeAuxInfo))) {
            entry->_1D = static_cast< XanimeAuxInfo* >(auxTables[0])->_4;
            entry->_40 = &(static_cast< XanimeAuxInfo* >(auxTables[0])->_8);
        } else {
            entry->_1D = 0;
            entry->_40 = nullptr;
        }

        entry->_C = 0.0f;
        if (entry->_20[0] == nullptr) {
            entry->_10 = 0.0f;
            entry->_14 = 0.0f;
            entry->_18 = 0;
        } else {
            // Unsure of what is happening there.
            entry->_18 = static_cast< u8 >(entry->_20[0][4]);
            entry->_14 = 0.0f;
            entry->_10 = static_cast< f32 >(reinterpret_cast< const s16* >(entry->_20[0])[3]);
        }

        XanimeBckTable* ofsTables[1];
        ofsTables[0] = pOfsInfo;

        if (search(ofsTables, entry->mParent.animationName, sizeof(XanimeOfsInfo))) {
            entry->_C = static_cast< XanimeOfsInfo* >(ofsTables[0])->_4;
            entry->_14 = static_cast< XanimeOfsInfo* >(ofsTables[0])->_C;
            entry->_10 = static_cast< XanimeOfsInfo* >(ofsTables[0])->_8;
            entry->_18 = static_cast< XanimeOfsInfo* >(ofsTables[0])->_10;
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

            return &_68[simpleIndex];
        }

        return &_10[groupIndex];

    case 2:
        return getGroupInfo(pArg, _14);
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
                u32 hashcode = MR::getHashCode(&pPath[i + 1]);
                XanimeGroupInfo* infos = pDir[index].mSubInformations;

                for (int j = 0; j < pDir[index].mSize; j++) {
                    if (infos[j].mHash == hashcode) {
                        return &infos[j];
                    }
                }

                return nullptr;
            }
        }
    }

    i = 0;
    u32 hashcode = MR::getHashCode(pPath);
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
                if (subInfo[j].mHash == hashcode) {
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

u32 XanimeResourceTable::getIndex(XanimeDirectory* pArg1, const char* pArg2) const {
    int i = 0;
    u32 hash = MR::getHashCode(pArg2);

    while (true) {
        if (pArg1[i].mName[0] == '\0') {
            return -1;
        }

        if (hash == pArg1[i].mHash) {
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

u32 XanimeResourceTable::getGroupIndex(const char* pArg) const {
    u32 hash = MR::getHashCode(pArg);

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

    return &_10[index];
}

u32 XanimeResourceTable::getSingleIndex(const char* pArg) const {
    if (_18 == nullptr) {
        return -1;
    }
    u32 hash = MR::getHashCode(pArg);

    for (int i = 0; i < m_14Size; i++) {
        if (hash == _18[i].animationHash) {
            return i;
        }
    }

    return -1;
}

u32 XanimeResourceTable::getSimpleIndex(const char* pArg) const {
    u32 hash = MR::getHashCode(pArg);

    for (int i = 0; i < m_68Size; i++) {
        if (hash == _68[i].mHash) {
            return i;
        }
    }

    return -1;
}

void XanimeResourceTable::createSortTable() {
    mSortTable = new HashSortTable(m_10Size);
    for (int i = 0; i < m_10Size; i++) {
        mSortTable->add(_10[i].mHash, i);
    }
    mSortTable->sort();
}

char* XanimeResourceTable::findResMotion(const char* pArg) const {
    const char* newName = swapBckName(pArg, mSwapTable);
    if (!mResourceHolder->mMotionResTable->isExistRes(newName)) {
        return nullptr;
    }
    mResourceHolder->mMotionResTable->getRes(newName);
}

char* XanimeResourceTable::findStringMotion(const char* pArg) const {
    return mResourceHolder->mMotionResTable->findFileInfo(pArg)->mName;
}

const char* XanimeResourceTable::swapBckName(const char* pOriginal, XanimeSwapTable* pTable) const {
    if (pTable == nullptr) {
        return pOriginal;
    }

    int i = 0;

    while (true) {
        if (pTable[i].original[0] == '\0') {
            return pOriginal;
        }

        if (MR::strcasecmp(pTable[i].original, pOriginal) == 0) {
            return pTable[i].swapped;
        }
        i++;
    }
}

// First argument should be XanimeBckTable
bool XanimeResourceTable::search(XanimeBckTable** pArray, const char* pTarget, u32 sizeOfArrayElements) const {
    if (pArray == nullptr) {
        return false;
    }

    if (pArray[0] == nullptr) {
        return false;
    }

    while (true) {
        const char* string = pArray[0]->animationName;
        if (strcmp(string, pTarget) == 0) {
            return true;
        }

        if (pArray[0]->animationName[0] == '\0') {
            return false;
        }

        // Possibly fake pointer arithmetic.
        pArray[0] = (XanimeBckTable*)((const char*)pArray[0] + sizeOfArrayElements);
    }
}

void XanimeGroupInfo::init() {
    _4 = 0.f;
    mParent.animationName = "not-initialized";
    _8 = 0;
    _C = 0.f;
    _10 = 0.f;
    _14 = 0.f;
    _18 = 0;
    mTableSize = 0;
    _1D = 0;
    for (u32 i = 0; i < 4; i++) {
        _30[i] = 0.f;
        _20[i] = nullptr;
    }
    _40 = nullptr;
    mHash = 0;
}
