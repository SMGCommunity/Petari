#pragma once

#include "Inline.hpp"
#include <revolution.h>

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
