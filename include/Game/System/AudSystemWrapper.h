#pragma once

#include <JSystem/JKernel/JKRArchive.h>

class AudSystemWrapper {
public:
    JKRArchive* mSMRArchive;            // _C
    JKRArchive* mJaiSeqArchive;         // _10
    JKRArchive* mJaiCoordArchive;       // _14
    JKRArchive* mJaiMeArchive;          // _18
    JKRArchive* mJaiRemixArchive;       // _1C
    u32 _20;
    JKRArchive* mSpkResArchive;         // _24
};