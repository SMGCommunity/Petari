#pragma once

#include <revolution/types.h>

class HashSortTable {
public:
    /// @brief Creates a new `HashSortTable` with the given maximum size.
    /// @param numMax The maximum number of key-value pairs that can be stored.
    HashSortTable(u32 numMax);

    /// @brief Appends the given key-value pair to the table.
    /// @param[in] pName A pointer to the null-terminated string to hash for use as the key.
    /// @param value An unsigned 32-bit integer for use as the value.
    /// @param isSkip Determines if duplicate keys should be considered before adding the key-value pair to the table.
    /// @return `true` if the key-value pair was added to the table, `false` otherwise.
    bool add(const char* pName, u32 value, bool isSkip);

    /// @brief Appends the given key-value pair to the table without considering duplicate keys.
    /// @param hash The hash code of the name for use as the key.
    /// @param value An unsigned 32-bit integer for use as the value.
    /// @return `true` if the key-value pair was added to the table, `false` otherwise.
    bool add(u32 hash, u32 value);

    /// @brief Appends the given key-value pair to the table if the key does not already exist in it.
    /// @param hash The hash code of the name for use as the key.
    /// @param value An unsigned 32-bit integer for use as the value.
    /// @return `true` if the key-value pair was added to the table, `false` otherwise.
    bool addOrSkip(u32 hash, u32 value);

    /// @brief Sorts the key-value pairs in ascending order by its keys.
    void sort();

    /// @brief Finds the given key in the table.
    /// @param hash The key to find.
    /// @param[out] pDst An optional pointer to initialize with the given key's associated value.
    /// @return `true` if the key was found in the table, `false` otherwise.
    bool search(u32 hash, u32* pDst);

    /// @brief Finds the given key in the table.
    /// @param[in] pName A pointer to the null-terminated string to hash and find.
    /// @param[out] pDst An optional pointer to initialize with the given key's associated value.
    /// @return `true` if the key was found in the table, `false` otherwise.
    bool search(const char* pName, u32* pDst);

    /// @brief Finds the sum of two keys in the table.
    /// @param[in] pName1 A pointer to the left-hand null-terminated string to hash.
    /// @param[in] pName2 A pointer to the right-hand null-terminated string to hash.
    /// @param[out] pDst An optional pointer to initialize with the given key's associated value.
    /// @return `true` if the key was found in the table, `false` otherwise.
    bool search(const char* pName1, const char* pName2, u32* pDst);

    /// @brief Replaces `pOldName`'s hashed counterpart with `pNewName`'s hashed counterpart, or appends the former to the end of the table.
    /// @param[in] pNewName A pointer to the null-terminated string to hash and be replaced in the table.
    /// @param[in] pOldName A pointer to the null-terminated string to hash and replace the old one in the table.
    void swap(const char* pOldName, const char* pNewName);

    /// @brief Returns `true` if the table was sorted by its keys.
    bool isSorted() const {
        return mIsSorted;
    }

private:
    /// @brief Determines if the table was sorted by its keys.
    /* 0x00 */ bool mIsSorted;

    /// @brief A pointer to an array of all the keys.
    /* 0x04 */ u32* mHashArray;

    /// @brief A pointer to an array of all the values.
    /* 0x08 */ u32* mValueArray;

    /// @brief A pointer to an array of indices corresponding to the start of each bucket.
    /* 0x0C */ u16* mBucketArray;

    /// @brief A pointer to an array of sizes for each bucket.
    /* 0x10 */ u16* mBucketSizeArray;

    /// @brief The current number of key-value pairs in the table.
    /* 0x14 */ u32 mTableSize;

    /// @brief The maximum number of key-value pairs that can be stored in the table.
    /* 0x18 */ u32 mTableSizeMax;
};

namespace MR {
    /// @brief Computes a hash code for a given string.
    /// @param[in] pStr A pointer to the null-terminated string to hash.
    /// @return The hashed string.
    u32 getHashCode(const char* pStr);

    /// @brief Computes a hash code for a given string, converting each character to its ASCII lowercase equivalent.
    /// @param[in] pStr A pointer to the null-terminated string to hash.
    /// @return The hashed string.
    u32 getHashCodeLower(const char* pStr);
};  // namespace MR
