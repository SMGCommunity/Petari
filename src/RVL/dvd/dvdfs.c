#include "RVL/dvd.h"
#include "RVL/os/OSBootInfo.h"

typedef struct FSTEntry FSTEntry;

struct FSTEntry
{
    u32 _0;
    u32 _4;
    u32 _8;
};

extern OSBootInfo* BootInfo_1;
extern FSTEntry* FstStart;
extern char* FstStringStart;
extern u32 MaxEntryNum;

void __DVDFSInit()
{
    BootInfo_1 = (OSBootInfo*)0x80000000;
    FstStart = (FSTEntry*)BootInfo_1->_38;

    if (FstStart)
    {
        MaxEntryNum = FstStart->_8;
        FstStringStart = (char*)&(FstStart[MaxEntryNum]);
    }
}