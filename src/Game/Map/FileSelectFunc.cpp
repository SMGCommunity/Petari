#include "Game/Map/FileSelectFunc.hpp"
#include "Game/Map/FileSelectIconID.hpp"
#include "Game/Util.hpp"
#include "RVLFaceLib.h"

namespace FileSelectFunc {
    u32 getMiiNameBufferSize() {
        return 0xB;
    }

    void copyMiiName(u16* pData, const FileSelectIconID& rIcon) {
        if (rIcon.isFellow()) {
            u32 fellowID = rIcon.getFellowID();
            const wchar_t* msg = MR::getGameMessageDirect(sIconNameMessageID[fellowID]);
            MR::copyMemory(pData, msg, 0x16);
        }
        else {
            if (rIcon.isMii()) {
                u16 miiIdx = rIcon.getMiiIndex();

                RFLAdditionalInfo info;
                RFLErrcode ret = RFLGetAdditionalInfo(&info, RFLDataSource_Official, 0, miiIdx);

                if (ret == RFLErrcode_Success) {
                    MR::copyMemory(pData, &info, 0x16);
                }
            }
        }
    }
};
