#pragma once

#include <revolution/types.h>

class HashSortTable {
public:
    /// @brief Creates a new `HashSortTable` with the given maximum size.
    /// @param size the maximum possible size of the new `HashSortTable`.
    HashSortTable(u32);

    /// @brief Adds the given key/value pair to the `HashSortTable`.
    /// @param A name to be used as the key.
    /// @param An unsigned long to be used as the value.
    /// @param True if this key/value pair is added using `addOrSkip(u32, u32)`; False if using `add(u32, u32)` instead.
    /// @returns True on success.
    bool add(const char*, u32, bool);
    /// @brief Adds the given hash/value pair to the `HashSortTable`. If the given hash already exists in the table, its associated value will be
    /// overwritten.
    /// @param The hash code of the name to be used as the key.
    /// @param An unsigned long to be used as the value.
    /// @return True on success.
    bool add(u32, u32);
    /// @brief Adds the given hash/value pair to the `HashSortTable`. If the given hash already exists in the table, the table will be unchanged and
    /// this function will fail.
    /// @param The hash code of the name to be used as the key.
    /// @param An unsigned long to be used as the value.
    /// @return True on success (hash wasn't already in the table); False on failure (hash was already in the table).
    bool addOrSkip(u32, u32);
    /// @brief Sorts the key/value pairs in the hash table in ascending order by the keys' hash codes.
    void sort();
    /// @brief Searches for the given hash code in the table. If the given pointer isn't `nullptr`, changes the value at that pointer to the value
    /// associated with the given hash code, or 0 if the hash code is not in the table.
    /// @param A hash code to search for.
    /// @param A pointer to where the hash code's associated value in the table will be stored. If this pointer is `nullptr`, the value will not be
    /// stored anywhere.
    /// @return True if the given hash code was found in the table; False otherwise.
    bool search(u32, u32*);
    /// @brief Searches for the given key (name) in the table. If the given pointer isn't `nullptr`, changes the value at that pointer to the value
    /// associated with the given key, or 0 if the key is not in the table.
    /// @param A pointer to a key name (string) to search for.
    /// @param A pointer to where the key's associated value in the table will be stored. If this pointer is `nullptr`, the value will not be stored
    /// anywhere.
    /// @return True if the given key was found in the table; False otherwise.
    bool search(const char*, u32*);
    /// @brief Given two strings, adds their hash codes together and searches for the sum in the table. If the given pointer isn't `nullptr`, changes
    /// the value at that pointer to the value associated with the given hash code, or 0 if the hash code is not in the table.
    /// @param A pointer to a string.
    /// @param A pointer to a string.
    /// @param A pointer to where the hash code's associated value in the table will be stored. If this pointer is `nullptr`, the value will not be
    /// stored anywhere.
    /// @return True if the hash code was found in the table; False otherwise.
    bool search(const char*, const char*, u32*);
    /// @brief Given pointers to two key names (name1 and name2), replaces name1's hash in the table with name2's hash. If name1's hash isn't in the
    /// table, it will be appended at the end of the list of hash codes.
    void swap(const char*, const char*);

    /* 0x00 */ bool mHasBeenSorted;  ///< True if this table is sorted in ascending order by the keys' hash codes; False otherwise.
    /* 0x04 */ u32* mHashCodes;      ///< An array containing the hash codes of all the names in the name/value pairs.
    /* 0x08 */ u32* mValues;  ///< An array containing all the values in the name/value pairs. For any i, mHashCodes[i] and mValues[i] form a pair.
    /* 0x0C */ u16*
        mIndexFirstHashForUpperByte;  ///< For any byte i (0-255), indexFirstHashForUpperByte[i] is the index of the first hash code in mHashCodes
                                      ///< whose upper byte is i. If no such hash code exists, indexFirstHashForUpperByte[i] = 65535.
    /* 0x10 */ u16* mNumHashesForUpperByte;  ///< For any byte i (0-255), numHashesForUpperByte[i] is the number of hash codes in mHashCodes whose
                                             ///< upper byte is i.
    /* 0x14 */ u32 mCurrentLength;           ///< The current number of name/value pairs in the table.
    /* 0x18 */ u32 mMaxLength;               ///< The maximum possible number of name/value pairs in the table.
};

namespace MR {
    /// @brief Generates a hash code for a given string.
    /// @param A pointer to a string.
    /// @return A hash code for that string.
    u32 getHashCode(const char*);
    /// @brief Given a pointer to a string, converts the string to lowercase and then generates a hash code for it. The original string is not
    /// modified.
    /// @param A pointer to a string.
    /// @return A hash code for the string converted to lowercase.
    u32 getHashCodeLower(const char*);
};  // namespace MR
