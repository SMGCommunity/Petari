#include "Game/Map/FileSelectFunc.h"
#include "Game/Map/FileSelectIconID.h"
#include "Game/Util.h"
#include "RVLFaceLib.h"

u32 FileSelectFunc::getMiiNameBufferSize() {
    return 0xB;
}

void FileSelectFunc::copyMiiName(u16 *pData, const FileSelectIconID &rIcon) {
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
