#include "JSystem/JKernel/JKRArchive.hpp"
#include "JSystem/JKernel/JKRHeap.hpp"
#include <cstring>

#ifdef NON_MATCHING
// tolower() is inlined
const char *JKRArchive::CArcName::store(const char *pName) {
    mHash = 0;
    u32 length = 0;

    for (; *pName != 0; pName++) {
        char lowerChar = tolower(*pName);

        mHash = lowerChar + mHash * 3;

        if (length < MAX_NAME_LENGTH) {
            mName[length++] = lowerChar;
        }
    }

    mLength = static_cast<u16>(length);
    mName[length] = 0;

    return &mLength[length];
}
#endif

#ifdef NON_MATCHING
// tolower() is inlined
const char *JKRArchive::CArcName::store(const char *pName, char stopChar) {
    mHash = 0;
    u32 length = 0;

    for (; *pName != stopChar; pName++) {
        char lowerChar = tolower(*pName);

        mHash = lowerChar + mHash * 3;

        if (length < MAX_NAME_LENGTH) {
            mName[length++] = lowerChar;
        }
    }

    mLength = static_cast<u16>(length);
    mName[length] = 0;

    if (*pName == 0) {
        return nullptr;
    }

    return pName + 1;
}
#endif

JKRArchive::JKRArchive() {
    mIsMounted = false;
    mMountDir = MOUNT_DIRECTION_1;
}

JKRArchive::JKRArchive(long entryNum, EMountMode mountMode) {
    mIsMounted = false;
    mMountMode = mountMode;
    _34 = 1;
    _58 = 1;
    mHeap = JKRHeap::findFromRoot(this);

    if (mHeap == nullptr) {
        mHeap = JKRHeap::sCurrentHeap;
    }

    mEntryNum = entryNum;

    if (gCurrentFileLoader == nullptr) {
        sCurrentDirIndex = 0;
        gCurrentFileLoader = this;
    }
}

JKRArchive::~JKRArchive() {

}

void JKRArchive::setExpandSize(SDIFileEntry *pFile, unsigned long size) {
    u32 fileIndex = static_cast<u32>(pFile - mFiles);

    if (mExpandSizes == nullptr || fileIndex >= mInfoBlock->mNrFiles) {
        return;
    }

    mExpandSizes[fileIndex] = size;
}

u32 JKRArchive::getExpandSize(SDIFileEntry *pFile) const {
    u32 fileIndex = static_cast<u32>(pFile - mFiles);

    if (mExpandSizes == nullptr || fileIndex >= mInfoBlock->mNrFiles) {
        return 0;
    }

    return mExpandSizes[fileIndex];
}


bool JKRArchive::isSameName(CArcName &rName, unsigned long nameOffset, unsigned short hash) const {
    if (rName.mHash != hash) {
        return false;
    }

    return strcmp(mStringTable + nameOffset, rName.mName) == 0;
}

JKRArchive::SDIDirEntry *JKRArchive::findResType(unsigned long a1) const {
    SDIDirEntry *current = mDirs;

    for (u32 i = 0; i < mInfoBlock->mNrDirs; i++) {
        if (current->mID == a1) {
            return current;
        }

        current++;
    }

    return nullptr;
}

#ifdef NON_MATCHING
// Register mismatch
JKRArchive::SDIDirEntry *JKRArchive::findDirectory(const char *pName, unsigned long dirIndex) const {
    SDIDirEntry *dir;
    SDIFileEntry *currentFile;
    s32 i;

    if (pName == nullptr) {
        return &mDirs[dirIndex];
    }

    CArcName name;
    const char *next = name.store(pName, '/');

    dir = &mDirs[dirIndex];
    currentFile = &mFiles[dir->mFirstFileIndex];

    for (i = 0; i < dir->mNrFiles; i++) {
        if (isSameName(name, currentFile->mNameOffset, currentFile->mHash)) {
            if ((currentFile->mFlag & FILE_FLAG_FOLDER) != 0) {
                return findDirectory(next, currentFile->mDirIndex);
            }

            break;
        }

        currentFile++;
    }

    return nullptr;
}
#endif

JKRArchive::SDIFileEntry *JKRArchive::findTypeResource(unsigned long a1, const char *pName) const {
    if (a1 != 0) {
        CArcName name;
        name.store(pName);

        SDIDirEntry *dir = findResType(a1);

        if (dir != nullptr) {
            SDIFileEntry *current = &mFiles[dir->mFirstFileIndex];

            for (s32 i = 0; i < dir->mNrFiles; i++) {
                if (isSameName(name, current->mNameOffset, current->mHash)) {
                    return current;
                }

                current++;
            }
        }
    }

    return nullptr;
}

#ifdef NON_MATCHING
// Register mismatch
JKRArchive::SDIFileEntry *JKRArchive::findFsResource(const char *pName, unsigned long dirIndex) const {
    if (pName != nullptr) {
        SDIDirEntry *dir;
        SDIFileEntry *currentFile;
        s32 i;

        CArcName name;
        const char *next = name.store(pName, '/');

        dir = &mDirs[dirIndex];
        currentFile = &mFiles[dir->mFirstFileIndex];

        for (s32 i = 0; i < dir->mNrFiles; i++) {
            if (isSameName(name, currentFile->mNameOffset, currentFile->mHash)) {
                if ((currentFile->mFlag & FILE_FLAG_FOLDER) != 0) {
                    return findFsResource(next, currentFile->mDirIndex);
                }
                else if (next == nullptr) {
                    return currentFile;
                }
                else {
                    return nullptr;
                }
            }

            currentFile++;
        }
    }

    return nullptr;
}
#endif

JKRArchive::SDIFileEntry *JKRArchive::findIdxResource(unsigned long index) const {
    if (index < mInfoBlock->mNrFiles) {
        return &mFiles[index];
    }

    return nullptr;
}

JKRArchive::SDIFileEntry *JKRArchive::findNameResource(const char *pName) const {
    SDIFileEntry *current = mFiles;

    CArcName name;
    name.store(pName);

    for (s32 i = 0; i < mInfoBlock->mNrFiles; i++) {
        if (isSameName(name, current->mNameOffset, current->mHash)) {
            return current;
        }

        current++;
    }

    return nullptr;
}

JKRArchive::SDIFileEntry *JKRArchive::findPtrResource(const void *pResource) const {
    SDIFileEntry *current = mFiles;

    for (s32 i = 0; i < mInfoBlock->mNrFiles; i++) {
        if (current->mFileData == pResource) {
            return current;
        }

        current++;
    }

    return nullptr;
}

JKRArchive::SDIFileEntry *JKRArchive::findIdResource(unsigned short fileID) const {
    if (fileID != 0xFFFF) {
        SDIFileEntry *current = mFiles;
        SDIFileEntry *indexed = &mFiles[fileID];

        if (indexed->mFileID == fileID && (indexed->mFlag & FILE_FLAG_FILE) != 0) {
            return indexed;
        }

        for (s32 i = 0; i < mInfoBlock->mNrFiles; i++) {
            if (current->mFileID == fileID && (current->mFlag & FILE_FLAG_FILE) != 0) {
                return current;
            }

            current++;
        }
    }

    return nullptr;
}
