#pragma once

#include <revolution.h>

namespace MR {
    /// @brief Appends `pChild` to `pParent` as a new component to the path.
    /// @param pDst The output pointer for the path.
    /// @param size The size of the path buffer, in bytes.
    /// @param pChild The pointer to the null-terminated path component to append.
    /// @param pParent The pointer to the null-terminated path component to be appended to.
    void addFilePrefix(char* pDst, u32 size, const char* pChild, const char* pParent);

    /// @brief Creates a localized datestamp.
    /// @param pDst The output pointer for the datestamp.
    /// @param size The size of the datestamp buffer, in bytes.
    /// @param year The number in the years place.
    /// @param month The number in the months place.
    /// @param day The number in the days place.
    void makeDateString(wchar_t* pDst, s32 size, s32 year, s32 month, s32 day);

    /// @brief Creates a localized timestamp with minute precision.
    /// @param pDst The output pointer for the timestamp.
    /// @param size The size of the timestamp buffer, in bytes.
    /// @param hour The number in the hours place.
    /// @param minute The number in the minutes place.
    void makeTimeString(wchar_t* pDst, s32 size, s32 hour, s32 minute);

    /// @brief Creates a localized timestamp with centisecond precision.
    /// @param pDst The output pointer for the timestamp.
    /// @param frame The number of frames to convert.
    void makeClearTimeString(wchar_t* pDst, u32 frame);

    /// @brief Creates a localized timestamp with second precision.
    /// @param pDst The output pointer for the timestamp.
    /// @param frame The number of frames to convert.
    void makeMinuteAndSecondString(wchar_t* pDst, u32 frame);

    wchar_t* addNumberFontTag(wchar_t* pDst, int tag);

    /// @brief Removes the file extension from the given path.
    /// @param pDst The output pointer for the path.
    /// @param size The size of the path buffer, in bytes.
    /// @param pPath The pointer to the null-terminated path.
    /// @return The pointer to the final directory separator character.
    char* removeExtensionString(char* pDst, u32 size, const char* pPath);

    /// @brief Creates a localized timestamp for the given race's best time.
    /// @param pDst The output pointer for the timestamp.
    /// @param raceId The identifier of the target race.
    void makeRaceBestTimeString(wchar_t* pDst, int raceId);

    /// @brief Creates a localized timestamp for the current race's elapsed time.
    /// @param pDst The output pointer for the timestamp.
    void makeRaceCurrentTimeString(wchar_t* pDst);

    /// @brief Copies `num` characters from `pSrc` to `pDst`.
    /// @param pDst The pointer to the null-terminated character string to copy to.
    /// @param pSrc The pointer to the null-terminated character string to copy from.
    /// @param num The number of characters to copy.
    void copyString(char* pDst, const char* pSrc, u32 num);

    /// @brief Copies `num` characters from `pSrc` to `pDst`.
    /// @param pDst The pointer to the null-terminated wide character string to copy to.
    /// @param pSrc The pointer to the null-terminated wide character string to copy from.
    /// @param num The number of characters to copy.
    void copyString(wchar_t* pDst, const wchar_t* pSrc, u32 num);

    wchar_t* addPictureFontCode(wchar_t* pDst, int code);
    wchar_t* addPictureFontTag(wchar_t* pDst, int tag);
    wchar_t* addPictureFontTagPlayerIcon(wchar_t* pDst);

    /// @brief Writes a terminated newline character at the given wide character pointer.
    /// @param pDst The pointer to the wide character.
    /// @return The pointer to the new null terminator character.
    wchar_t* addNewLine(wchar_t* pDst);

    wchar_t* addNumberFontTag(wchar_t* pDst, const wchar_t* pFmt, ...);

    /// @brief Returns the final component of the given path.
    /// @param pPath The pointer to the null-terminated path.
    /// @return The pointer to the null-terminated final component of the path.
    const char* getBasename(const char* pPath);

    /// @brief Copies `num` characters from `pSrc` to `pDst` and terminates the string.
    /// @param pDst The pointer to the null-terminated character string to copy to.
    /// @param pSrc The pointer to the null-terminated character string to copy from.
    /// @param num The number of characters to copy.
    void extractString(char* pDst, const char* pSrc, u32 num, u32);

    /// @brief Transcodes a wide character string to a character string.
    /// @param pDst The output pointer for the null-terminated character string.
    /// @param pSrc The pointer to the null-terminated wide character string to transcode.
    /// @param num The number of characters to transcode, including the null terminator character.
    /// @return The number of characters transcoded, excluding the null terminator character.
    int convertUTF16ToASCII(char* pDst, const wchar_t* pSrc, s32 num);

    /// @brief Compares two character strings for equivalence without regard to case sensitivity.
    /// @param pStr1 The pointer to the left-hand null-terminated character string.
    /// @param pStr2 The pointer to the right-hand null-terminated character string.
    /// @retval -1 The first string contains a character less than the corresponding character in the second string.
    /// @retval 0 The strings are equivalent.
    /// @retval 1 The second string contains a character greater than the corresponding character in the first string.
    int strcasecmp(const char* pStr1, const char* pStr2);

    /// @brief Determines if two character strings are equivalent.
    /// @param pStr1 The pointer to the left-hand null-terminated character string.
    /// @param pStr2 The pointer to the right-hand null-terminated character string.
    /// @return `true` if the strings are equivalent, `false` otherwise.
    bool isEqualString(const char* pStr1, const char* pStr2);

    /// @brief Determines if two character strings are equivalent without regard to case sensitivity.
    /// @param pStr1 The pointer to the left-hand null-terminated character string.
    /// @param pStr2 The pointer to the right-hand null-terminated character string.
    /// @return `true` if the strings are equivalent, `false` otherwise.
    bool isEqualStringCase(const char* pStr1, const char* pStr2);

    /// @brief Determines if the given character string contains the given substring.
    /// @param pStr The pointer to the null-terminated character string.
    /// @param pSubStr The pointer to the null-terminated character substring.
    /// @return `true` if the string contains the substring, `false` otherwise.
    bool isEqualSubString(const char* pStr, const char* pSubStr);

    /// @brief Determines if the given character string contains a space character.
    /// @param pStr The pointer to the null-terminated character string.
    /// @return `true` if the string contains a space character, `false` otherwise.
    bool hasStringSpace(const char* pStr);

    /// @brief Determines if the given character string contains `digitNum` trailing digits.
    /// @param pStr The pointer to the null-terminated character string.
    /// @param digitNum The number of trailing digits to check for.
    /// @return `true` if the string contains `digitNum` trailing digits, `false` otherwise.
    bool isDigitStringTail(const char* pStr, int digitNum);

    /// @brief Determines if the given array of character strings contains a character string equivalent to `pStr`.
    /// @param pStr The pointer to the null-terminated character string.
    /// @param ppStr The array of pointers to null-terminated character strings.
    /// @param strNum The number of pointers to null-terminated character strings.
    /// @return `true` if the array of strings contains a string equivalent to `pStr`, `false` otherwise.
    bool isExistString(const char* pStr, const char* const* ppStr, u32 strNum);

    /// @brief Determines if the pointer to the given character string is `nullptr` or contains no characters.
    /// @param pStr The pointer to the null-terminated character string.
    /// @return `true` if the pointer to the string is `nullptr` or contains no characters, `false` otherwise.
    bool isNullOrEmptyString(const char* pStr);

    bool isMessageEditorNextTag(const wchar_t*);
    int getStringLengthWithMessageTag(const wchar_t*);

    /// @brief Parses the given character string into a signed 32-bit integer.
    /// @param pSrc The pointer to the null-terminated character string to read from.
    /// @param pSubStr The pointer to the null-terminated character substring to search for.
    /// @param pDst The output pointer for the parsed value.
    void scan32(const char* pSrc, const char* pSubStr, s32* pDst);

    /// @brief Parses the given character string into an unsigned 16-bit integer.
    /// @param pSrc The pointer to the null-terminated character string to read from.
    /// @param pSubStr The pointer to the null-terminated character substring to search for.
    /// @param pDst The output pointer for the parsed value.
    void scan16(const char* pSrc, const char* pSubStr, u16* pDst);

    /// @brief Parses the given character string into an unsigned 8-bit integer.
    /// @param pSrc The pointer to the null-terminated character string to read from.
    /// @param pSubStr The pointer to the null-terminated character substring to search for.
    /// @param pDst The output pointer for the parsed value.
    void scan8(const char* pSrc, const char* pSubStr, u8* pDst);

    /// @brief Parses the given character string into a 32-bit floating-point number.
    /// @param pSrc The pointer to the null-terminated character string to read from.
    /// @param pSubStr The pointer to the null-terminated character substring to search for.
    /// @param pDst The output pointer for the parsed value.
    void scanf32(const char* pSrc, const char* pSubStr, f32* pDst);

    /// @brief Parses the given character string into a set of four unsigned 8-bit integers.
    /// @param pSrc The pointer to the null-terminated character string to read from.
    /// @param pSubStr The pointer to the null-terminated character substring to search for.
    /// @param pDst The output pointer for the parsed values.
    void scanu8x4(const char* pSrc, const char* pSubStr, u8* pDst);

    /// @brief Parses the given character string into a set of four signed 16-bit integers.
    /// @param pSrc The pointer to the null-terminated character string to read from.
    /// @param pSubStr The pointer to the null-terminated character substring to search for.
    /// @param pDst The output pointer for the parsed values.
    void scans16x4(const char* pSrc, const char* pSubStr, s16* pDst);

    /// @brief Parses the given character string into a set of four 32-bit floating-point numbers.
    /// @param pSrc The pointer to the null-terminated character string to read from.
    /// @param pSubStr The pointer to the null-terminated character substring to search for.
    /// @param pDst The output pointer for the parsed values.
    void scanf32x4(const char* pSrc, const char* pSubStr, f32* pDst);
};  // namespace MR
