#pragma once

#include "Game/Util/Array.hpp"
#include <revolution/types.h>
#include <revolution/os/OSMessage.h>

class JKRHeap;

class FileHolderFileEntry {
public:
    /// @brief Creates a new `FileHolderFileEntry`.
    /// @param pFilePath The path to the file.
    /// @param pHeap The heap to allocate from.
    /// @param pContext The context data for the file.
    FileHolderFileEntry(const char *, JKRHeap *, void *);
    
    /// @brief Destroys the `FileHolderFileEntry`.
    ~FileHolderFileEntry();
    
    /// @brief Waits until the file read is done.
    void waitReadDone();

    /// @brief Sets the context data for the file.
    /// @param pContext The context data.
    /// @param pHeap The heap to allocate from.
    void setContext(void *, JKRHeap *);

    /* 0x00 */ s32 mEntryNum;
    /* 0x04 */ void *mContext;
    /* 0x08 */ JKRHeap *mHeap;
    /* 0x0C */ s32 mState;
    /* 0x10 */ OSMessageQueue mQueue;
    /* 0x30 */ OSMessage mMessage;
    /* 0x34 */ bool mContextSet;
};

class FileHolder {
public:
    /// @brief Creates a new `FileHolder`.
    FileHolder();

    /// @brief Adds a new file entry.
    /// @param pFilePath The path to the file.
    /// @param pHeap The heap to allocate from.
    /// @param pContext The context data for the file.
    FileHolderFileEntry *add(const char *, JKRHeap *, void *);

    /// @brief Checks if a file entry exists.
    /// @param pFilePath The path to the file.
    /// @return `true` if the file entry exists, `false` otherwise.
    bool isExist(const char *) const;

    /// @brief Gets the context data for the given file.
    /// @param pFilePath The path to the file.
    void *getContext(const char *) const;

    /// @brief Removes file entries that match the given heap.
    /// @param pHeap The heap to compare.
    void removeIfIsEqualHeap(JKRHeap *);

    /// @brief Removes the file entry for the given file.
    /// @param pFilePath The path to the file.
    /// @return The pointer to the removed entry.
    FileHolderFileEntry **removeFile(const char *);

    /// @brief Finds the file entry for the given file.
    /// @param pFilePath The path to the file.
    /// @return The pointer to the found entry, or `nullptr` if not found.
    FileHolderFileEntry *findEntry(const char *) const;

    /* 0x0 */ MR::Vector<MR::AssignableArray<FileHolderFileEntry *> > mEntries;
};
