#pragma once

#include <Inline.hpp>

#include "revolution.h"
#include "JSystem/JKernel/JKRDvdRipper.hpp"

class JKRArchive;
class JKRHeap;

namespace MR { 
    
    /// @brief Checks whether a file exists.
    /// @param pFile Path to the file.
    /// @param considerLanguage Whether to consider language prefix.
    /// @return true if file exists, false otherwise.
    bool isFileExist(const char *pFile, bool considerLanguage) NO_INLINE;

    
    /// @brief Gets the size of a file.
    /// @param pFile Path to the file.
    /// @param considerLanguage Whether to consider language prefix.
    /// @return Size of the file in bytes.
    u32 getFileSize(const char *pFile, bool considerLanguage);

    
    /// @brief Converts a file path to its DVD entry number, considering the language prefix.
    /// @param pFile Path to the file.
    /// @return The DVD entry number, or -1 if not found.
    s32 convertPathToEntrynumConsideringLanguage(const char *pFile);

    
    /// @brief Loads a file to main RAM synchronously.
    /// @param pFile Path to the file.
    /// @param pData Target memory address, or nullptr to allocate.
    /// @param pHeap Memory heap to use.
    /// @param allocDir Allocation direction.
    /// @return Pointer to the loaded data.
    void* loadToMainRAM(const char *pFile, u8 *pData, JKRHeap *pHeap, JKRDvdRipper::EAllocDirection allocDir);

    
    /// @brief Requests asynchronous file load to main RAM.
    /// @param pFile Path to the file.
    /// @param pData Target memory address, or nullptr to allocate.
    /// @param pHeap Memory heap to use.
    /// @param allocDir Allocation direction.
    void loadAsyncToMainRAM(const char *pFile, u8 *pData, JKRHeap *pHeap, JKRDvdRipper::EAllocDirection allocDir);

    
    /// @brief Mounts an archive synchronously.
    /// @param pFile Path to the archive file.
    /// @param pHeap Memory heap to use.
    /// @return Pointer to the mounted archive.
    void* mountArchive(const char *pFile, JKRHeap *pHeap);

    
    /// @brief Requests asynchronous mounting of an archive.
    /// @param pFile Path to the archive file.
    /// @param pHeap Memory heap to use.
    void mountAsyncArchive(const char *pFile, JKRHeap *pHeap);

    
    /// @brief Mounts an object or layout archive based on the provided file name.
    /// @param pFile Object or layout name.
    /// @param pHeap Memory heap to use (optional). 
    void mountAsyncArchiveByObjectOrLayoutName(const char *pFile, JKRHeap *pHeap);

    
    /// @brief Receives a file after it has been asynchronously loaded.
    /// @param pFile Path to the file.
    /// @return Pointer to the loaded data.
    void* receiveFile(const char *pFile);

    
    /// @brief Receives an archive after it has been asynchronously mounted.
    /// @param pFile Path to the archive file.
    /// @return Pointer to the mounted archive. 
    void* receiveArchive(const char *pFile);

    
    /// @brief Receives all files that have been requested for loading.
    void receiveAllRequestedFile();

    
    /// @brief Creates and adds an archive from existing memory data.
    /// @param pData Pointer to the archive data.
    /// @param pHeap Heap to add the archive to.
    /// @param pFile File name for identification.
    /// @return Pointer to the created archive. 
    void* createAndAddArchive(void *pData, JKRHeap *pHeap, const char *pFile);

    
    /// @brief Gets the mounted archive and heap for a given file.
    /// @param pFile Archive file path.
    /// @param pArchive Output pointer to archive.
    /// @param pHeap Output pointer to heap.
    void getMountedArchiveAndHeap(const char *pFile, JKRArchive **pArchive, JKRHeap **pHeap);

    
    /// @brief Removes a file from memory, considering language prefix.
    /// @param pFile Path to the file.
    void removeFileConsideringLanguage(const char *pFile);

    
    /// @brief Removes resources and file holders from memory if they belong to the specified heap.
    /// @param heap The heap to check.
    void removeResourceAndFileHolderIfIsEqualHeap(JKRHeap* heap);

    
    /// @brief Decompresses a file from an archive into RAM.
    /// @param archive Archive containing the file.
    /// @param filename File name within the archive.
    /// @param heap Heap to allocate memory from.
    /// @param align Alignment for the allocation.
    /// @return Pointer to the decompressed file data.
    void* decompressFileFromArchive(JKRArchive *archive, const char *filename, JKRHeap *heap, int align);

    
    /// @brief Checks if a file is currently loaded.
    /// @param pFile File path.
    /// @return true if the file is loaded, false otherwise.
    bool isLoadedFile(const char *pFile);

    
    /// @brief Checks if an archive is currently mounted.
    /// @param pFile Archive path.
    /// @return true if the archive is mounted, false otherwise.
    bool isMountedArchive(const char *pFile);

    
    /// @brief Checks if an object or layout archive is loaded.
    /// @param pFile Object or layout name.
    /// @return true if loaded, false otherwise.
    bool isLoadedObjectOrLayoutArchive(const char *pFile);

    
    /// @brief Builds a language-aware file path.
    /// @param pName Output buffer for the file name.
    /// @param length Maximum length of the buffer.
    /// @param pFile Base file path.
    void makeFileNameConsideringLanguage(char *pName, u32 length, const char *pFile);

    
    /// @brief Constructs an object archive file name from the provided name.
    /// @param pName Output buffer for the archive name.
    /// @param length Buffer length.
    /// @param pFile Object name.
    /// @return true if a valid archive was found, false otherwise.
    bool makeObjectArchiveFileName(char *pName, u32 length, const char *pFile);

    
    /// @brief Constructs an object archive file name using a prefix.
    /// @param pName Output buffer for the archive name.
    /// @param length Buffer length.
    /// @param pFile Object name.
    /// @param unused Unused flag.
    /// @return true if a valid archive was found, false otherwise.
    bool makeObjectArchiveFileNameFromPrefix(char *pName, u32 length, const char *pFile, bool unused) NO_INLINE;

    
    /// @brief Constructs a layout archive file name from the provided name.
    /// @param pName Output buffer for the archive name.
    /// @param length Buffer length.
    /// @param pFile Layout name.
    /// @return true if a valid archive was found, false otherwise.
    bool makeLayoutArchiveFileName(char *pName, u32 length, const char *pFile);

    
    /// @brief Constructs a layout archive file name using a prefix, with optional fallback.
    /// @param pName Output buffer for the archive name.
    /// @param length Buffer length.
    /// @param pFile Layout name.
    /// @param fallback Whether to fall back if none found.
    /// @return true if a valid archive was found, false otherwise.
    bool makeLayoutArchiveFileNameFromPrefix(char *pName, u32 length, const char *pFile, bool fallback);

    
    /// @brief Creates a scenario archive file name from the given stage name.
    /// @param pName Output buffer for the file name.
    /// @param length Buffer length.
    /// @param pFile Stage name.
    void makeScenarioArchiveFileName(char *pName, u32 length, const char *pFile);
};
