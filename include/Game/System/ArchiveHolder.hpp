#pragma once

#include <JSystem/JKernel/JKRMemArchive.hpp>
#include <revolution/types.h>

class ArchiveHolderArchiveEntry {
public:
    /// @brief Creates a new `ArchiveHolderArchiveEntry`.
    ArchiveHolderArchiveEntry(void *, JKRHeap *, const char *);

    /// @brief Destroys the `ArchiveHolderArchiveEntry`.
    ~ArchiveHolderArchiveEntry();

    /* 0x0 */ JKRMemArchive *mArchive;
    /* 0x4 */ JKRHeap *mHeap;
    /* 0x8 */ char *mArchiveName;
};

class ArchiveHolder {
public:
    /// @brief Creates a new `ArchiveHolder`.
    ArchiveHolder();

    /// @brief Gets the pointer to the first entry.
    /// @return The pointer to the first entry.
    ArchiveHolderArchiveEntry **first() const {
        return mEntries;
    }

    /// @brief Gets the pointer to the last entry.
    /// @return The pointer to the last entry.
    ArchiveHolderArchiveEntry **last() const {
        return &mEntries[mCurEntryNum];
    }

    /// @brief Creates and adds a new archive entry.
    /// @param pData The data for the archive.
    /// @param pHeap The heap to allocate from.
    /// @param pArchiveName The name of the archive.
    /// @return The pointer to the created entry.
    ArchiveHolderArchiveEntry *createAndAdd(void *, JKRHeap *, const char *);
    
    /// @brief Gets the archive for the given archive name.
    /// @param pArchiveName The name of the archive.
    /// @return The pointer to the found archive, or `nullptr` if not found.
    JKRMemArchive *getArchive(const char *) const;
    
    /// @brief Gets the archive and heap for the given archive name.
    /// @param pArchiveName The name of the archive.
    /// @param pArchive The output pointer for the archive.
    /// @param pHeap The output pointer for the heap.
    void getArchiveAndHeap(const char *, JKRArchive **, JKRHeap **) const;
    
    /// @brief Removes entries that match the given heap.
    /// @param pHeap The heap to compare.
    void removeIfIsEqualHeap(JKRHeap *);

    /// @brief Finds the entry with the given archive name.
    /// @param pArchiveName The name of the archive.
    /// @return The pointer to the found entry, or `nullptr` if not found.
    ArchiveHolderArchiveEntry *findEntry(const char *) const;

    /* 0x0 */ ArchiveHolderArchiveEntry **mEntries;
    /* 0x4 */ s32 mMaxEntries;
    /* 0x8 */ s32 mCurEntryNum;
    /* 0xC */ OSMutex mMutex;
};
