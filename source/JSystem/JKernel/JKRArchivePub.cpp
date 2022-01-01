#include "JSystem/JKernel/JKRAramArchive.h"
#include "JSystem/JKernel/JKRArchive.h"
#include "JSystem/JKernel/JKRCompArchive.h"
#include "JSystem/JKernel/JKRDvdArchive.h"
#include "JSystem/JKernel/JKRFileFinder.h"
#include "JSystem/JKernel/JKRHeap.h"
#include "JSystem/JKernel/JKRMemArchive.h"
#include "revolution.h"

bool JKRArchive::becomeCurrent(const char *pName) {
    SDIDirEntry *dir;

    if (*pName == '/') {
        const char *pDir = pName + 1;

        if (*pDir == 0) {
            pDir = NULL;
        }

        dir = findDirectory(pDir, 0);
    }
    else {
        dir = findDirectory(pName, sCurrentDirIndex);
    }

    bool validDir = dir != NULL;

    if (validDir) {
        JKRFileLoader::gCurrentFileLoader = this;
        sCurrentDirIndex = static_cast<u32>(dir - mDirs);
    }

    return validDir;
}

void *JKRArchive::getResource(const char *pName) {
    SDIFileEntry *file;

    if (*pName == '/') {
        file = findFsResource(pName + 1, 0);
    }
    else {
        file = findFsResource(pName, sCurrentDirIndex);
    }

    if (file != NULL) {
        return fetchResource(file, NULL);
    }

    return NULL;
}

void *JKRArchive::getResource(unsigned long a1, const char *pName) {
    SDIFileEntry *file;

    if (a1 == NULL_MAGIC || a1 == QUESTIONMARK_MAGIC) {
        file = findNameResource(pName);
    }
    else {
        file = findTypeResource(a1, pName);
    }

    if (file != NULL) {
        return fetchResource(file, NULL);
    }

    return NULL;
}

u32 JKRArchive::readResource(void *a1, unsigned long a2, const char *pName) {
    SDIFileEntry *file;

    if (*pName == '/') {
        file = findFsResource(pName + 1, 0);
    }
    else {
        file = findFsResource(pName, sCurrentDirIndex);
    }

    if (file != NULL) {
        u32 local_18;
        fetchResource(a1, a2, file, &local_18);

        return local_18;
    }

    return 0;
}

u32 JKRArchive::readResource(void *a1, unsigned long a2, unsigned long a3, const char *pName) {
    SDIFileEntry *file;

    if (a3 == NULL_MAGIC || a3 == QUESTIONMARK_MAGIC) {
        file = findNameResource(pName);
    }
    else {
        file = findTypeResource(a3, pName);
    }

    if (file != NULL) {
        u32 local_18;
        fetchResource(a1, a2, file, &local_18);

        return local_18;
    }

    return 0;
}

void JKRArchive::removeResourceAll() {
    if (mInfoBlock != NULL && mMountMode != MOUNT_MODE_DVD) {
        SDIFileEntry *current = mFiles;

        for (u32 i = 0; i < mInfoBlock->mNrFiles; i++) {
            if (current->mFileData != NULL) {
                JKRHeap::free(current->mFileData, mHeap);
                current->mFileData = NULL;
            }

            current++;
        }
    }
}

bool JKRArchive::removeResource(void *pResource) {
    SDIFileEntry *file = findPtrResource(pResource);

    if (file == NULL) {
        return false;
    }

    file->mFileData = NULL;
    JKRHeap::free(pResource, mHeap);

    return true;
}

bool JKRArchive::detachResource(void *pResource) {
    SDIFileEntry *file = findPtrResource(pResource);

    if (file == NULL) {
        return false;
    }

    file->mFileData = NULL;

    return true;
}

s32 JKRArchive::getResSize(const void *pResource) const {
    SDIFileEntry *file = findPtrResource(pResource);

    if (file == NULL) {
        return -1;
    }

    return file->mDataSize;
}

s32 JKRArchive::countFile(const char *pName) const {
    SDIDirEntry *dir;

    if (*pName == '/') {
        pName++;

        if (*pName == 0) {
            pName = NULL;
        }

        dir = findDirectory(pName, 0);
    }
    else {
        dir = findDirectory(pName, sCurrentDirIndex);
    }

    if (dir != NULL) {
        return dir->mNrFiles;
    }

    return 0;
}

JKRArcFinder *JKRArchive::getFirstFile(const char *pName) const {
    SDIDirEntry *dir;

    if (*pName == '/') {
        pName++;

        if (*pName == 0) {
            pName = NULL;
        }

        dir = findDirectory(pName, 0);
    }
    else {
        dir = findDirectory(pName, sCurrentDirIndex);
    }

    if (dir != NULL) {
        // Bad to cast to non-const
        return new(JKRHeap::sGameHeap, 0) JKRArcFinder(const_cast<JKRArchive *>(this), dir->mFirstFileIndex, dir->mNrFiles);
    }

    return NULL;
}

JKRArchive *JKRArchive::check_mount_already(long entryNum) {
    JSUPtrLink *current = JKRFileLoader::sFileLoaderList.mHead;

    while (current != NULL) {
        JKRArchive *archive = reinterpret_cast<JKRArchive *>(current->mData);

        if (archive->mLoaderType == RARC_MAGIC && archive->mEntryNum == entryNum) {
            archive->_34++;
            return archive;
        }

        current = current->mNext;
    }

    return NULL;
}

JKRArchive *JKRArchive::check_mount_already(long entryNum, JKRHeap *pHeap) {
    if (pHeap == NULL) {
        pHeap = JKRHeap::sCurrentHeap;
    }

    JSUPtrLink *current = JKRFileLoader::sFileLoaderList.mHead;

    while (current != NULL) {
        JKRArchive *archive = reinterpret_cast<JKRArchive *>(current->mData);

        if (archive->mLoaderType == RARC_MAGIC && archive->mEntryNum == entryNum && archive->mHeap == pHeap) {
            archive->_34++;
            return archive;
        }

        current = current->mNext;
    }

    return NULL;
}

void JKRArchive::mount(const char *pName, EMountMode mountMode, JKRHeap *pHeap, EMountDirection mountDir) {
    s32 entryNum = DVDConvertPathToEntrynum(pName);

    if (entryNum >= 0) {
        mount(entryNum, mountMode, pHeap, mountDir);
    }
}

JKRArchive *JKRArchive::mount(long entryNum, EMountMode mountMode, JKRHeap *pHeap, EMountDirection mountDir) {
    JKRArchive *archive = check_mount_already(entryNum, pHeap);

    if (archive != NULL) {
        return archive;
    }

    s32 uVar1 = -4;

    if (mountDir == MOUNT_DIRECTION_1) {
        uVar1 = 4;
    }

    switch (mountMode) {
        case MOUNT_MODE_MEM:
            archive = new(pHeap, uVar1) JKRMemArchive(entryNum, mountDir);
            break;
        case MOUNT_MODE_ARAM:
            archive = new(pHeap, uVar1) JKRAramArchive(entryNum, mountDir);
            break;
        case MOUNT_MODE_DVD:
            archive = new(pHeap, uVar1) JKRDvdArchive(entryNum, mountDir);
            break;
        case MOUNT_MODE_COMP:
            archive = new(pHeap, uVar1) JKRCompArchive(entryNum, mountDir);
            break;
    }

    if (archive != NULL && archive->mMountMode == MOUNT_MODE_0) {
        delete archive;
        archive = NULL;
    }

    return archive;
}

bool JKRArchive::getDirEntry(SDirEntry *pDir, unsigned long fileIndex) const {
    SDIFileEntry *file = findIdxResource(fileIndex);

    if (file == NULL) {
        return false;
    }

    pDir->mFileFlag = file->mFlag;
    pDir->mFileID = file->mFileID;
    pDir->mName = mStringTable + file->mNameOffset;

    return true;
}

void *JKRArchive::getIdxResource(unsigned long fileIndex) {
    SDIFileEntry *file = findIdxResource(fileIndex);

    if (file != NULL) {
        return fetchResource(file, 0);
    }

    return NULL;
}

void *JKRArchive::getResource(unsigned short fileID) {
    SDIFileEntry *file = findIdResource(fileID);

    if (file != NULL) {
        return fetchResource(file, 0);
    }

    return NULL;
}

u32 JKRArchive::readResource(void *pResource, unsigned long a2, unsigned short fileID) {
    SDIFileEntry *file = findIdResource(fileID);

    if (file != NULL) {
        u32 local_18;
        fetchResource(pResource, a2, file, &local_18);

        return local_18;
    }

    return 0;
}

u32 JKRArchive::countResource() const {
    u32 count = 0;

    for (u32 i = 0; i < mInfoBlock->mNrFiles; i++) {
        if ((mFiles[i].mFlag & FILE_FLAG_FILE) != 0) {
            count++;
        }
    }

    return count;
}

u32 JKRArchive::getFileAttribute(unsigned long fileIndex) const {
    SDIFileEntry *file = findIdxResource(fileIndex);

    if (file != NULL) {
        return file->mFlag;
    }

    return 0;
}
