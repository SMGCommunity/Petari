#pragma once

#include "JSystem/JAudio2/JAISound.hpp"
#include "JSystem/JAudio2/JASGlobal.hpp"

// TODO: this is supposed to be 6 bytes for SMG?
struct JAUSoundTableItem {
    /* 0x00 */ u8 mPriority;
    /* 0x01 */ u8 mVolume;
    /* 0x02 */ u16 mResourceId;
    /* 0x04 */ u32 _4;
};

template < typename Root, typename Section, typename Group, typename Item >
struct JAUSoundTable_ {
    JAUSoundTable_() {
        mData = nullptr;
        mRoot = nullptr;
    }

    void reset() {
        mData = nullptr;
        mRoot = nullptr;
    }

    void init(const void* pData) {
        mData = pData;
        if (*(u32*)mData != Root::magicNumber()) {
            mData = nullptr;
        } else {
            mRoot = (Root*)((u8*)mData + *((u32*)mData + 3));
        }
    }

    Section* getSection(int index) const {
        if (index < 0) {
            return nullptr;
        }
        if ((u32)index >= mRoot->mSectionNumber) {
            return nullptr;
        }
        u32 offset = mRoot->mSectionOffsets[index];
        if (offset == 0) {
            return nullptr;
        }
        return (Section*)((u8*)mData + offset);
    }

    Group* getGroup(const Section* section, int index) const {
        if (index < 0) {
            return nullptr;
        }
        if ((u32)index >= section->mNumGroups) {
            return nullptr;
        }
        u32 offset = section->getGroupOffset(index);
        if (offset == 0) {
            return nullptr;
        }
        return (Group*)((u8*)mData + offset);
    }

    Item* getItem(const Group* group, int index) const {
        u32 offset = group->getItemOffset(index);
        if (offset == 0) {
            return nullptr;
        }
        return (Item*)((u8*)mData + offset);
    }

    /* 0x00 */ const void* mData;
    /* 0x04 */ Root* mRoot;
};

struct JAUSoundTableRoot {
    static inline u32 magicNumber() {
        return 'BST ';
    }
    /* 0x00 */ u32 mSectionNumber;
    /* 0x04 */ u32 mSectionOffsets[0];
};

struct JAUSoundTableSection {
    int getGroupOffset(int index) const {
        if (index < 0) {
            return 0;
        }
        if (index >= mNumGroups) {
            return 0;
        }
        return mGroupOffsets[index];
    }

    /* 0x00 */ u32 mNumGroups;
    /* 0x04 */ u32 mGroupOffsets[0];
};

struct JAUSoundTableGroup {
    u8 getTypeID(int index) const {
        if (index < 0) {
            return 0;
        }
        if (index >= mNumItems) {
            return -1;
        }
        return ((u8*)mItemOffsets)[index * 4];
    }

    u32 getItemOffset(int index) const {
        if (index < 0) {
            return 0;
        }
        if (index >= mNumItems) {
            return 0;
        }
        return mItemOffsets[index] & 0xffffff;
    }

    /* 0x00 */ u32 mNumItems;
    /* 0x04 */ u32 mGlobalOffset;
    /* 0x08 */ u32 mItemOffsets[0];
};

struct JAUSoundTable : public JASGlobalInstance< JAUSoundTable > {
    JAUSoundTable(bool setInstance) : JASGlobalInstance< JAUSoundTable >(setInstance) {
    }
    ~JAUSoundTable() {
    }

    void init(void const*);
    u8 getTypeID(JAISoundID) const;
    JAUSoundTableItem* getData(JAISoundID) const;
    int getNumGroups_inSection(u8) const;
    int getNumItems_inGroup(u8, u8) const;

    JAUSoundTableItem* getItem(const JAUSoundTableGroup* group, int index) const {
        return (JAUSoundTableItem*)mTable.getItem(group, index);
    }

    const void* getResource() const {
        return mTable.mData;
    }
    bool isValid() const {
        return mTable.mData != nullptr;
    }

    /* 0x00 */ JAUSoundTable_< JAUSoundTableRoot, JAUSoundTableSection, JAUSoundTableGroup, void > mTable;
};

struct JAUSoundNameTableRoot {
    static inline u32 magicNumber() {
        return 'BSTN';
    }
    /* 0x00 */ u32 mSectionNumber;
    /* 0x04 */ u32 mSectionOffsets[0];
};

struct JAUSoundNameTableSection {
    int getGroupOffset(int index) const {
        if (index < 0) {
            return 0;
        }
        if (index >= mNumGroups) {
            return 0;
        }
        return mGroupOffsets[index];
    }

    /* 0x00 */ u32 mNumGroups;
    /* 0x04 */ u32 mGlobalOffset;
    /* 0x08 */ u32 mGroupOffsets[0];
};

struct JAUSoundNameTableGroup {
    u32 getItemOffset(int index) const {
        if (index < 0) {
            return 0;
        }
        if (index >= mNumItems) {
            return 0;
        }
        return mItemOffsets[index];
    }

    /* 0x00 */ u32 mNumItems;
    /* 0x04 */ u32 mGlobalOffset;
    /* 0x08 */ u32 mItemOffsets[0];
};

struct JAUSoundNameTable : public JASGlobalInstance< JAUSoundNameTable > {
    JAUSoundNameTable(bool setInstance) : JASGlobalInstance< JAUSoundNameTable >(setInstance) {
    }
    ~JAUSoundNameTable() {
    }
    int getNumGroups_inSection(u8) const;
    int getNumItems_inGroup(u8, u8) const;
    void init(void const*);
    const char* getName(JAISoundID) const;
    const char* getGroupName(JAISoundID) const;

    const char* getItem(const JAUSoundNameTableGroup* group, int index) const {
        return (const char*)mTable.getItem(group, index);
    }

    /* 0x00 */ JAUSoundTable_< JAUSoundNameTableRoot, JAUSoundNameTableSection, JAUSoundNameTableGroup, char > mTable;
};
