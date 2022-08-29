#ifndef OSALARM_H
#define OSALARM_H

#include <revolution/types.h>
#include <revolution/os/OSContext.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OSAlarm OSAlarm;
typedef void (*OSAlarmHandler)(OSAlarm *, OSContext *);

struct OSAlarm {
    OSAlarmHandler handler;
    u32 tag;
    OSTime fire;
    OSAlarm* prev;
    OSAlarm* next;
    OSTime period;
    OSTime start;
    void* userData;
};

#ifdef __cplusplus
}
#endif

#endif // OSALARM_H