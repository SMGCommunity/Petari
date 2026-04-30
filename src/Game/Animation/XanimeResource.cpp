#include "Game/Animation/XanimeResource.hpp"
#include "Game/System/ResourceHolder.hpp"
#include "Game/System/ResourceInfo.hpp"
#include "Game/Util/HashUtil.hpp"
#include "Game/Util/StringUtil.hpp"
#include <cstring>

static bool unknownBool;
XanimeResourceTable::XanimeResourceTable(ResourceHolder* pArg) {
    // TODO: where is this from?

    init();

    _0 = 0;
    _10 = 0;
    mResourceHolder = pArg;
    m_14Size = 0;
    m_10Size = 0;

    _1 = 1;

    if (unknownBool) {
        _1 = unknownBool;
    }

    _1C.init();

    _1C._4 = 1.0f;
    _1C.mParent.animationName = "non-group";
    _1C._8 = 1;
    _1C._1C = 1;
    _1C._1D = 0;
}

XanimeResourceTable::XanimeResourceTable(ResourceHolder* pArg1, XanimeGroupInfo* pArg2, XanimeAuxInfo* pArg3, XanimeOfsInfo* pArg4,
                                         XanimeBckTable* pArg5, XanimeBckTable* pArg6, XanimeBckTable* pArg7, XanimeBckTable* pArg8,
                                         XanimeSwapTable* pArg9) {
    init();
    _0 = 1;
    _10 = pArg2;
    mResourceHolder = pArg1;
    _1 = 1;
    m_10Size = initGroupInfo(pArg1, pArg2, pArg3, pArg4, pArg5, pArg6, pArg7, pArg8, pArg9);
    createSortTable();

    // Hacky?
    _18 = reinterpret_cast< XanimeSingleBckTable* >(pArg5);

    int i = 0;
    while (_18 != nullptr) {
        if (_18[i].parent.animationName[0] == '\0') {
            break;
        }
        _18[i].animationHash = MR::getHashCode(_18[i].parent.animationName);
        _18[i].fileHash = MR::getHashCode(_18[i].fileName);

        i++;
    }
    m_14Size = i;

    _1C.init();
    _1C._4 = 1.0f;
    _1C.mParent.animationName = "non-group";
    _1C._8 = 1;
    _1C._1C = 1;
    _1C._1D = 0;
    _68 = new XanimeGroupInfo[0];
    m_68Size = 0;
}

void XanimeResourceTable::init() {
    _0 = 0;
    _1 = 0;
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

#pragma push
#pragma ppc_iro_level 3
const XanimeGroupInfo* XanimeResourceTable::getGroupInfo(const char* pPath, XanimeDirectory* pDir) const {
    int i = 0;
    const char* originalPath = pPath;
    while (true) {
        i++;
        if (originalPath[i] == '\0') {
            break;
        }

        if (pPath[i] == '/') {
            char pathOneDirectoryDown[48];
            MR::extractString(pathOneDirectoryDown, &pPath[i], i, 32);

            u32 index = getIndex(pDir, pathOneDirectoryDown);
            XanimeDirectory* entry = &pDir[index];

            switch (pDir[index].mDirectoryType) {
            case XanimeDirectory::Recursive:
                return getGroupInfo(&pPath[i + 1], pDir[index].mSubDirectories);

            case XanimeDirectory::Leaf:
                u32 hashcode = MR::getHashCode(&pPath[i + 1]);
                XanimeGroupInfo* infos = entry->mSubInformations;

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

    // While it technically matches, I suspect an inline
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
#pragma pop

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

bool XanimeResourceTable::search(XanimeBckTable** pArg1, const char* pArg2, u32 arg3) const {
    if (pArg1 == nullptr) {
        return false;
    }

    if (pArg1[0] == nullptr) {
        return false;
    }

    while (true) {
        const char* string = pArg1[0]->animationName;
        if (strcmp(string, pArg2) == 0) {
            return true;
        }

        if (pArg1[0]->animationName[0] == '\0') {
            return false;
        }

        // Possibly fake pointer arithmetic
        pArg1[0] = (XanimeBckTable*)((const char*)pArg1[0] + arg3);
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
    _1C = 0;
    _1D = 0;
    for (u32 i = 0; i < 4; i++) {
        _30[i] = 0.f;
        _20[i] = 0;
    }
    _40 = 0;
    mHash = 0;
}
