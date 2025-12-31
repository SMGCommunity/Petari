#include "JSystem/JUtility/JUTNameTab.hpp"
#include <cstring>

JUTNameTab::JUTNameTab() {
    setResource(nullptr);
}

JUTNameTab::JUTNameTab(const ResNTAB* pNameTable) {
    setResource(pNameTable);
}

void JUTNameTab::setResource(const ResNTAB* pNameTable) {
    mResource = pNameTable;

    if (pNameTable != NULL) {
        mNameNum = pNameTable->mEntryNum;
        mStrData = (const char*)(pNameTable->mEntries + mNameNum);
    } else {
        mNameNum = 0;
        mStrData = nullptr;
    }
}

s32 JUTNameTab::getIndex(const char* pName) const {
    const ResNTAB::Entry* pEntry = mResource->mEntries;
    u16 keyCode = calcKeyCode(pName);

    for (u16 i = 0; i < mNameNum; i++) {
        if (pEntry->mKeyCode == keyCode && strcmp((mResource->mEntries[i].mOffs + ((const char*)mResource)), pName) == 0) {
            return i;
        }
        pEntry++;
    }

    return -1;
}

const char* JUTNameTab::getName(u16 index) const {
    if (index < mNameNum) {
        return ((const char*)mResource) + mResource->mEntries[index].mOffs;
    }

    return nullptr;
}

u16 JUTNameTab::calcKeyCode(const char* pName) const {
    u32 keyCode = 0;
    while (*pName) {
        keyCode = (keyCode * 3) + *pName++;
    }
    return keyCode;
}

JUTNameTab::~JUTNameTab() {}
