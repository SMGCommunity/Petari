#pragma once

#include <JSystem/JKernel/JKRDvdRipper.hpp>
#include <revolution/types.h>

class JKRArchive;
class JKRHeap;
class JKRMemArchive;

namespace MR {
    /// @brief Determines if a file exists within the disc file system.
    /// @param[in] pFilePath A pointer to the null-terminated absolute path of the file.
    /// @param[in] considerLanguage Determines if the file is localized.
    /// @return `true` if the file exists within the disc file system, `false` otherwise.
    bool isFileExist(const char* pFilePath, bool considerLanguage) NO_INLINE;

    /// @brief Returns the size of a file, in bytes.
    /// @param[in] pFilePath A pointer to the null-terminated absolute path of the file.
    /// @param[in] considerLanguage Determines if the file is localized.
    /// @return The size of the file, in bytes.
    u32 getFileSize(const char* pFilePath, bool considerLanguage);

    /// @brief Converts a file path to its entry number, with respect to the current language.
    /// @param[in] pFilePath A pointer to the null-terminated absolute path of the file.
    /// @return The entry number if the file was found, `-1` otherwise.
    s32 convertPathToEntrynumConsideringLanguage(const char* pFilePath);

    /// @brief Loads a file into main memory synchronously.
    /// @param[in] pFilePath A pointer to the null-terminated absolute path of the file.
    /// @param[out] pDst A pointer to the target memory address, or `nullptr` to allocate.
    /// @param[in,out] pHeap An optional pointer to the target heap.
    /// @param[in] allocDir The direction for the allocation.
    /// @return A pointer to the loaded file data.
    void* loadToMainRAM(const char* pFilePath, u8* pDst, JKRHeap* pHeap, JKRDvdRipper::EAllocDirection allocDir);

    /// @brief Requests asynchronous loading of a file into main memory.
    /// @param[in] pFilePath A pointer to the null-terminated absolute path of the file.
    /// @param[out] pDst A pointer to the target memory address, or `nullptr` to allocate.
    /// @param[in,out] pHeap An optional pointer to the target heap.
    /// @param[in] allocDir The direction for the allocation.
    void loadAsyncToMainRAM(const char* pFilePath, u8* pDst, JKRHeap* pHeap, JKRDvdRipper::EAllocDirection allocDir);

    /// @brief Mounts an archive file synchronously.
    /// @param[in] pFilePath A pointer to the null-terminated absolute path of the archive file.
    /// @param[in,out] pHeap An optional pointer to the target heap.
    /// @return A pointer to the mounted archive.
    JKRMemArchive* mountArchive(const char* pFilePath, JKRHeap* pHeap);

    /// @brief Requests asynchronous mounting of an archive file.
    /// @param[in] pFilePath A pointer to the null-terminated absolute path of the archive file.
    /// @param[in,out] pHeap An optional pointer to the target heap.
    void mountAsyncArchive(const char* pFilePath, JKRHeap* pHeap);

    /// @brief Requests asynchronous mounting of an object or layout archive file.
    /// @param[in] pFilePrefix A pointer to the null-terminated name of the archive file, without the extension.
    /// @param[in,out] pHeap An optional pointer to the target heap.
    void mountAsyncArchiveByObjectOrLayoutName(const char* pFilePrefix, JKRHeap* pHeap);

    /// @brief Returns the data of a file after it has been asynchronously loaded.
    /// @param[in] pFilePath A pointer to the null-terminated absolute path of the file.
    /// @return A pointer to the loaded file data.
    void* receiveFile(const char* pFilePath);

    /// @brief Returns the archive after it has been asynchronously mounted.
    /// @param[in] pFilePath A pointer to the null-terminated absolute path of the archive file.
    /// @return A pointer to the mounted archive.
    JKRMemArchive* receiveArchive(const char* pFilePath);

    /// @brief Receives all files that have been requested for loading.
    void receiveAllRequestedFile();

    /// @brief Creates an archive from existing memory data.
    /// @param[in] pArcData A pointer to the archive file data.
    /// @param[in,out] pHeap A pointer to the target heap.
    /// @param[in] pFilePath A pointer to the null-terminated absolute path of the file.
    void createAndAddArchive(void* pArcData, JKRHeap* pHeap, const char* pFilePath);

    /// @brief Returns the mounted archive and heap for a given file by pointer.
    /// @param[in] pFilePath A pointer to the null-terminated absolute path of the archive file.
    /// @param[out] ppArchive A pointer to the archive pointer to initialize.
    /// @param[out] ppHeap A pointer to the heap pointer to initialize.
    void getMountedArchiveAndHeap(const char* pFilePath, JKRArchive** ppArchive, JKRHeap** ppHeap);

    /// @brief Removes a file from memory, with respect to the current language.
    /// @param[in] pFilePath A pointer to the null-terminated absolute path of the file.
    void removeFileConsideringLanguage(const char* pFilePath);

    /// @brief Removes resource and file holders from memory if they belong to the given heap.
    /// @param[in,out] pHeap A pointer to the heap to compare with.
    void removeResourceAndFileHolderIfIsEqualHeap(JKRHeap* pHeap);

    /// @brief Decompresses a file from an archive file.
    /// @param[in] pArchive A pointer to the target archive.
    /// @param[in] pFilePath A pointer to the null-terminated absolute path of the file within the archive file.
    /// @param[in,out] pHeap A pointer to the target heap.
    /// @param[in] align The alignment for the allocation.
    /// @return A pointer to the decompressed file data.
    void* decompressFileFromArchive(JKRArchive* pArchive, const char* pFilePath, JKRHeap* pHeap, int align);

    /// @brief Determines if an file is currently loaded.
    /// @param[in] pFilePath A pointer to the null-terminated absolute path of the file.
    /// @return `true` if the file is currently loaded, `false` otherwise.
    bool isLoadedFile(const char* pFilePath);

    /// @brief Determines if an archive file is currently mounted.
    /// @param[in] pFilePath A pointer to the null-terminated absolute path of the archive file.
    /// @return `true` if the archive file is currently mounted, `false` otherwise.
    bool isMountedArchive(const char* pFilePath);

    /// @brief Determines if an object or layout archive file is currently loaded.
    /// @param[in] pFilePrefix A pointer to the null-terminated name of the archive file, without the extension.
    /// @return `true` if the archive file is currently loaded, `false` otherwise.
    bool isLoadedObjectOrLayoutArchive(const char* pFilePrefix);

    /// @brief Creates an absolute path to a language-aware file.
    /// @param[out] pDst A pointer to the path buffer to initialize.
    /// @param[in] size The size of the path buffer, in bytes.
    /// @param[in] pFilePath A pointer to the null-terminated absolute path of the file.
    void makeFileNameConsideringLanguage(char* pDst, u32 size, const char* pFilePath);

    /// @brief Creates an absolute path to the given object's archive file.
    /// @param[out] pDst A pointer to the path buffer to initialize.
    /// @param[in] size The size of the path buffer, in bytes.
    /// @param[in] pFileName A pointer to the null-terminated name of the archive file.
    /// @return `true` if a valid archive file exists in the file system, `false` otherwise.
    bool makeObjectArchiveFileName(char* pDst, u32 size, const char* pFileName);

    /// @brief Creates an absolute path to the given object's archive file.
    /// @param[out] pDst A pointer to the path buffer to initialize.
    /// @param[in] size The size of the path buffer, in bytes.
    /// @param[in] pFilePrefix A pointer to the null-terminated name of the archive file, without the extension.
    /// @param[in] unused Unused flag.
    /// @return `true` if a valid archive file exists in the file system, `false` otherwise.
    bool makeObjectArchiveFileNameFromPrefix(char* pDst, u32 size, const char* pFilePrefix, bool unused) NO_INLINE;

    /// @brief Creates an absolute path to the given layout's archive file.
    /// @param[out] pDst A pointer to the path buffer to initialize.
    /// @param[in] size The size of the path buffer, in bytes.
    /// @param[in] pFileName A pointer to the null-terminated name of the archive file.
    /// @return `true` if a valid archive file exists in the file system, `false` otherwise.
    bool makeLayoutArchiveFileName(char* pDst, u32 size, const char* pFileName);

    /// @brief Creates an absolute path to the given layout's archive file.
    /// @param[out] pDst A pointer to the path buffer to initialize.
    /// @param[in] size The size of the path buffer, in bytes.
    /// @param[in] pFilePrefix A pointer to the null-terminated name of the archive file, without the extension.
    /// @param[in] fallback Whether to fall back if none found.
    /// @return `true` if a valid archive file exists in the file system, `false` otherwise.
    bool makeLayoutArchiveFileNameFromPrefix(char* pDst, u32 size, const char* pFilePrefix, bool fallback);

    /// @brief Creates an absolute path to the given galaxy's mission archive file.
    /// @param[out] pDst A pointer to the path buffer to initialize.
    /// @param[in] size The size of the path buffer, in bytes.
    /// @param[in] pStageName A pointer to the null-terminated name of the galaxy.
    void makeScenarioArchiveFileName(char* pDst, u32 size, const char* pStageName);
};  // namespace MR
