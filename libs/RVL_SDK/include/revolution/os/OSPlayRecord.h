#ifndef OSPLAYRECORD_H
#define OSPLAYRECORD_H

#include <revolution/types.h>
#include <revolution/os/OSTime.h>

typedef struct {
    u32 checkSum;
    u16 titleName[2][21];
    OSTime playStartTime;
    OSTime playTime;
    char gameCode[4];
    char companyCode[2];
    char res[18];
} OSPlayRecord;

void __OSStartPlayRecord(void);
void __OSStopPlayRecord(void);

#endif // OSPLAYRECORD_H