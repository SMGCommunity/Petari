#include "Game/Util.h"

namespace MR {
    #ifdef NON_MATCHING
    // shrug
    s32 calcCheckSum(const void *pData, u32 a2) {
        u32 v2 = 0;
        u32 v3 = 0;
        
        for (int i = a2 >> 1; i; i--) {
            u16 curData = *(u16*)pData;
            pData = (s8*)pData + 2;
            v3 += curData;
            v2 += ~curData;
        }

        return (v3 << 16) | v2;
    }
    #endif
};
