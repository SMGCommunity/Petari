#pragma once

#include <revolution.h>
#include <size_t.h>

namespace MR {
    /// @brief Returns the length of the given null-terminated wide string.
    /// @param pStr The pointer to the null-terminated wide string to be examined.
    /// @return The length of the string.
    size_t strlenUTF16(const u16* pStr);
};  // namespace MR
