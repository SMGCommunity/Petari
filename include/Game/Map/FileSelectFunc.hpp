#pragma once

#include "Game/Map/FileSelectIconID.hpp"

namespace { 
    static const char* sIconNameMessageID[5] = {
        "System_FileSelect_Icon000",
        "System_FileSelect_Icon001",
        "System_FileSelect_Icon002",
        "System_FileSelect_Icon003",
        "System_FileSelect_Icon004"
    };
};

namespace FileSelectFunc {
    u32 getMiiNameBufferSize();
    void copyMiiName(u16*, const FileSelectIconID&);
};
