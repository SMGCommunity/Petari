#pragma once

#include <revolution/types.h>

#define REGION_EU 0
#define REGION_JP 1
#define REGION_US 2
#define REGION_CN 3
#define REGION_KR 4
#define REGION_SHIFT 0x4
#define REGION_SHIFT_EU (REGION_EU << REGION_SHIFT)
#define REGION_SHIFT_JP (REGION_JP << REGION_SHIFT)
#define REGION_SHIFT_US (REGION_US << REGION_SHIFT)
#define REGION_SHIFT_CN (REGION_CN << REGION_SHIFT)
#define REGION_SHIFT_KR (REGION_KR << REGION_SHIFT)
#define REGION_MASK (0xF << REGION_SHIFT)

#define LANGUAGE_JPJAPANESE (REGION_SHIFT_JP | SC_LANG_JAPANESE)
#define LANGUAGE_USENGLISH (REGION_SHIFT_US | SC_LANG_ENGLISH)
#define LANGUAGE_USSPANISH (REGION_SHIFT_US | SC_LANG_SPANISH)
#define LANGUAGE_USFRENCH (REGION_SHIFT_US | SC_LANG_FRENCH)
#define LANGUAGE_EUENGLISH (REGION_SHIFT_EU | SC_LANG_ENGLISH)
#define LANGUAGE_EUSPANISH (REGION_SHIFT_EU | SC_LANG_SPANISH)
#define LANGUAGE_EUFRENCH (REGION_SHIFT_EU | SC_LANG_FRENCH)
#define LANGUAGE_EUGERMAN (REGION_SHIFT_EU | SC_LANG_GERMAN)
#define LANGUAGE_EUITALIAN (REGION_SHIFT_EU | SC_LANG_ITALIAN)
#define LANGUAGE_EUDUTCH (REGION_SHIFT_EU | SC_LANG_DUTCH)
#define LANGUAGE_CNSIMPCHINESE (REGION_SHIFT_CN | SC_LANG_SIMP_CHINESE)
#define LANGUAGE_KRKOREAN (REGION_SHIFT_KR | SC_LANG_KOREAN)
#define LANGUAGE_MASK (~REGION_MASK)

/// @brief A regional variant of a language.
struct Language {
    /// @brief The region-language identifier.
    u32 mId;

    /// @brief The region-language label.
    const char* mName;
};

namespace MR {
    /// @brief Returns the region-language identifier based on the console's language setting.
    /// @return The region-language identifier.
    u32 getDecidedLanguageFromIPL();

    /// @brief Returns the working region-language identifier.
    /// @return The region-language identifier.
    u32 getLanguage() NO_INLINE;

    /// @brief Extracts the console's language setting from the working region-language identifier.
    /// @return The console's language setting.
    u32 getLanguageFromIPL();

    /// @brief Returns the region-language label.
    /// @return The pointer to the null-terminated label.
    const char* getCurrentLanguagePrefix();

    /// @brief Returns the region label.
    /// @return The pointer to the null-terminated label.
    const char* getCurrentRegionPrefix();

    /// @brief Returns the number of region-language pairs.
    /// @return The number of region-language pairs.
    u32 getLanguageNum();

    /// @brief Returns the region-language label by index.
    /// @param index The position in the lookup table.
    /// @return The pointer to the null-terminated label.
    const char* getLanguagePrefixByIndex(u32 index);
};  // namespace MR
