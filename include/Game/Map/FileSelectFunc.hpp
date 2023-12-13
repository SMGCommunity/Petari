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

class FileSelectFunc {
public:
    static u32 getMiiNameBufferSize();
    static void copyMiiName(u16 *, const FileSelectIconID &);
};
