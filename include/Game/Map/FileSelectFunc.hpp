#pragma once

#include <revolution.h>

class FileSelectIconID;

namespace {
    /// @brief The array of message identifiers corresponding to File Selection Screen icon names.
    static const char* sIconNameMessageID[] = {
        "System_FileSelect_Icon000",
        "System_FileSelect_Icon001",
        "System_FileSelect_Icon002",
        "System_FileSelect_Icon003",
        "System_FileSelect_Icon004"
    };
};

/// @brief Helper functions for the File Selection Screen.
namespace FileSelectFunc {
    /// @brief Returns the size of an icon's name buffer, in bytes.
    /// @return The size of an icon's name buffer, in bytes.
    u32 getMiiNameBufferSize();

    /// @brief Returns the name of the given icon by pointer.
    /// @param pName The output pointer for the null-terminated wide name.
    /// @param rIcon The reference to the File Selection Screen icon.
    void copyMiiName(u16* pName, const FileSelectIconID& rIcon);
};
