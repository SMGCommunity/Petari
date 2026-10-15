#pragma once

#include <revolution/types.h>

class HashSortTable {
public:
    /// @brief Creates a new `HashSortTable` with the given maximum size.
    /// @param numMax The maximum possible size of the new `HashSortTable`.
    HashSortTable(u32 numMax);

    /// @brief Adds the given key/value pair to the `HashSortTable`.
    /// @param pName A name to be used as the key.
    /// @param value An unsigned integer to be used as the value.
    /// @param isValidSkip True if this key/value pair should only be added if the name isn't already in the table; False if this key/value pair
    /// should be added regardless.
    /// @returns True if the given key/value pair was successfully added to the table. If `isValidSkip` is True, this function returns True IFF the
    /// name doesn't already exist in the table; if `isValidSkip` is False, this function always returns True.
    bool add(const char* pName, u32 value, bool isValidSkip);

    /// @brief Adds the given hash/value pair to the `HashSortTable`. If the given hash already exists in the table, its associated value will be
    /// overwritten.
    /// @param hash The hash code of the name to be used as the key.
    /// @param value An unsigned integer to be used as the value.
    /// @return True, always.
    bool add(u32 hash, u32 value);

    /// @brief Adds the given hash/value pair to the `HashSortTable`. If the given hash already exists in the table, the table will be unchanged and
    /// this function will fail.
    /// @param hash The hash code of the name to be used as the key.
    /// @param value An unsigned integer to be used as the value.
    /// @return True on success (hash wasn't already in the table); False on failure (hash was already in the table).
    bool addOrSkip(u32 hash, u32 value);

    /// @brief Sorts the key/value pairs in the hash table in ascending order by the keys' hash codes. Also organizes the hash codes into "buckets" by
    /// their most significant bytes, in order to achieve much quicker searches.
    void sort();

    /// @brief Searches for the given hash code in the table. If the given pointer isn't `nullptr`, changes the value at that pointer to the value
    /// associated with the given hash code, or 0 if the hash code is not in the table.
    /// @param hashCode A hash code to search for.
    /// @param pDst A pointer to where the hash code's associated value in the table will be stored. If this pointer is `nullptr`, the value will not
    /// be stored anywhere.
    /// @return True if the given hash code was found in the table; False otherwise.
    bool search(u32 hashCode, u32* pDst);

    /// @brief Searches for the given key (name) in the table. If the given pointer isn't `nullptr`, changes the value at that pointer to the value
    /// associated with the given key, or 0 if the key is not in the table.
    /// @param pName A pointer to a key name (string) to search for.
    /// @param pDst A pointer to where the key's associated value in the table will be stored. If this pointer is `nullptr`, the value will not be
    /// stored anywhere.
    /// @return True if the given key was found in the table; False otherwise.
    bool search(const char* pName, u32* pDst);

    /// @brief Given two strings, adds their hash codes together and searches for the sum in the table. If the given pointer isn't `nullptr`, changes
    /// the value at that pointer to the value associated with the given hash code, or 0 if the hash code is not in the table.
    /// @param pName1 A pointer to a string.
    /// @param pName2 A pointer to a string.
    /// @param pDst A pointer to where the hash code's associated value in the table will be stored. If this pointer is `nullptr`, the value will not
    /// be stored anywhere.
    /// @return True if the hash code was found in the table; False otherwise.
    bool search(const char* pName1, const char* pName2, u32* pDst);

    /// @brief Given pointers to two key names (name1 and name2), replaces name1's hash in the table with name2's hash. If name1's hash isn't in the
    /// table, it will be appended at the end of the list of hash codes.
    /// @param pNameOld Pointer to the name that will be replaced in the table.
    /// @param pNameNew Pointer to the name that will replace the old one in the table.
    void swap(const char* pNameOld, const char* pNameNew);

    /* 0x00 */ bool mHasBeenSorted;  ///< True if this table is sorted in ascending order by the keys' hash codes; False otherwise.
    /* 0x04 */ u32* mHashCodes;      ///< An array containing the hash codes of all the names in the name/value pairs.
    /* 0x08 */ u32* mValues;  ///< An array containing all the values in the name/value pairs. For any i, mHashCodes[i] and mValues[i] form a pair.
    /* 0x0C */ u16* mBucketStartIndices;  ///< For any byte i (0-255), mBucketStartIndices[i] is the index of the first hash code in mHashCodes
                                          ///< whose upper byte is i. If no such hash code exists, mBucketStartIndices[i] = 0xFFFF. This facilitates
                                          ///< much faster searches by allowing the game to skip a bunch of hash codes.
    /* 0x10 */ u16* mBucketSizes;         ///< For any byte i (0-255), mBucketSizes[i] is the number of hash codes in mHashCodes whose
                                   ///< upper byte is i. This facilitates much faster searches by allowing the game to skip a bunch of hash codes.
    /* 0x14 */ u32 mTableSize;     ///< The current number of name/value pairs in the table.
    /* 0x18 */ u32 mTableMaxSize;  ///< The maximum possible number of name/value pairs in the table.
};

namespace MR {
    /// @brief Generates a hash code for a given string.
    /// @param pStr A pointer to a string.
    /// @return A hash code for that string.
    u32 getHashCode(const char* pStr);

    /// @brief Given a pointer to a string, converts the string to lowercase and then generates a hash code for it. The original string is not
    /// modified.
    /// @param pStr A pointer to a string.
    /// @return A hash code for the string converted to lowercase.
    u32 getHashCodeLower(const char* pStr);
};  // namespace MR
