#include "Game/Map/FileSelectFunc.hpp"
#include "Game/Map/FileSelectIconID.hpp"
#include "Game/Util/MemoryUtil.hpp"
#include "Game/Util/MessageUtil.hpp"
#include <RVLFaceLib.h>

namespace {
    /// @brief The array of message identifiers corresponding to File Selection Screen icon names.
    static const char* sIconNameMessageID[] = {
        "System_FileSelect_Icon000",
        "System_FileSelect_Icon001",
        "System_FileSelect_Icon002",
        "System_FileSelect_Icon003",
        "System_FileSelect_Icon004",
    };
}; // namespace

namespace FileSelectFunc {
    u32 getMiiNameBufferSize() {
        return RFL_NAME_LEN + 1;
    }

    void copyMiiName(u16* pName, const FileSelectIconID& rIcon) {
        if (rIcon.isFellow()) {
            const char*    pMessageId = sIconNameMessageID[rIcon.getFellowID()];
            const wchar_t* pMessage = MR::getGameMessageDirect(pMessageId);

            MR::copyMemory(pName, pMessage, getMiiNameBufferSize() * sizeof(wchar_t));
        } else if (rIcon.isMii()) {
            RFLAdditionalInfo info;
            RFLErrcode        err = RFLGetAdditionalInfo(&info, RFLDataSource_Official, nullptr, rIcon.getMiiIndex());

            if (err == RFLErrcode_Success) {
                MR::copyMemory(pName, info.name, getMiiNameBufferSize() * sizeof(wchar_t));
            }
        }
    }
}; // namespace FileSelectFunc
