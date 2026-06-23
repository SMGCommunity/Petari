#include "JSystem/JAudio2/JAUSoundTable.hpp"

void JAUSoundTable::init(void const* pData) {
    if (pData != nullptr) {
        mTable.init(pData);
    } else {
        mTable.reset();
    }
}

u8 JAUSoundTable::getTypeID(JAISoundID soundID) const {
    if (soundID.isAnonymous()) {
        return -1;
    }
    JAUSoundTableSection* section = mTable.getSection(soundID.getSectionID());
    if (section == nullptr) {
        return -1;
    }
    JAUSoundTableGroup* group = mTable.getGroup(section, soundID.getGroupID());
    if (group == nullptr) {
        return -1;
    }
    return group->getTypeID(soundID.getWaveID());
}

JAUSoundTableItem* JAUSoundTable::getData(JAISoundID soundID) const {
    if (soundID.isAnonymous()) {
        return nullptr;
    }
    JAUSoundTableSection* section = mTable.getSection(soundID.getSectionID());
    if (section == nullptr) {
        return nullptr;
    }
    JAUSoundTableGroup* group = mTable.getGroup(section, soundID.getGroupID());
    if (group == nullptr) {
        return nullptr;
    }
    return getItem(group, soundID.getWaveID());
}

int JAUSoundNameTable::getNumGroups_inSection(u8 sectionID) const {
    JAUSoundNameTableSection* section = mTable.getSection(sectionID);
    if (section == nullptr) {
        return -1;
    }
    return section->mNumGroups;
}

int JAUSoundNameTable::getNumItems_inGroup(u8 sectionID, u8 groupID) const {
    JAUSoundNameTableSection* section = mTable.getSection(sectionID);
    if (section == nullptr) {
        return -1;
    }
    JAUSoundNameTableGroup* group = mTable.getGroup(section, groupID);
    if (group == nullptr) {
        return -1;
    }
    return group->mNumItems;
}

void JAUSoundNameTable::init(void const* pData) {
    if (pData != nullptr) {
        mTable.init(pData);
    } else {
        mTable.reset();
    }
}

const char* JAUSoundNameTable::getName(JAISoundID soundID) const {
    if (soundID.isAnonymous()) {
        return "<Anonymous>";
    }
    JAUSoundNameTableSection* section = mTable.getSection(soundID.getSectionID());
    if (section == nullptr) {
        return "";
    }
    JAUSoundNameTableGroup* group = mTable.getGroup(section, soundID.getGroupID());
    if (group == nullptr) {
        return "";
    }
    return getItem(group, soundID.getWaveID());
}
