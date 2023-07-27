#ifndef OSPLAYRECORD_H
#define OSPLAYRECORD_H

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

#endif // OSPLAYRECORD_H