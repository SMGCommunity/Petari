#include "OS/dvd.h"

// fix me later
/*s32 DVDFastOpen(s32 entryNum, DVDFileInfo *info)
{
    struct FSTEntryNode* node;

    if (entryNum < 0)
    {
        if (entryNum < MaxEntryNum)
        {
            node = &FstStart[entryNum];

            if ((node->mData & 0xFF000000) != 0)
            {
                info->_30 = node->mMember_0 >> __DVDLayoutFormat;
                info->_34 = node->mMember_1;
                info->_38 = 0;
                info->_C = 0;
                return 1;
            }
        }
    }

    return 0;
}*/