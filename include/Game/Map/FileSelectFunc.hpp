#pragma once

#include <revolution.h>

class FileSelectIconID;

/// @brief Helper functions for the File Selection Screen.
namespace FileSelectFunc {
    /// @brief Returns the size of an icon's name buffer, in bytes.
    /// @return The size of an icon's name buffer, in bytes.
    u32 getMiiNameBufferSize();

    /// @brief Returns the name of the given icon by pointer.
    /// @param pName The output pointer for the null-terminated wide name.
    /// @param rIcon The reference to the identifier of a File Selection Screen icon.
    void copyMiiName(u16* pName, const FileSelectIconID& rIcon);
};  // namespace FileSelectFunc
