#ifndef OSALARM_H
#define OSALARM_H

#include <revolution/types.h>
#include <revolution/os/OSContext.h>
#include <revolution/os/OSTime.h>

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

void OSCreateAlarm(OSAlarm *);
void OSSetAlarm(OSAlarm *, OSTime, OSAlarmHandler);
void OSSetPeriodicAlarm(OSAlarm *, OSTime, OSTime, OSAlarmHandler);
void OSCancelAlarm(OSAlarm *);
void OSSetAlarmTag(OSAlarm *, u32);
void OSSetAlarmUserData(OSAlarm *, void *);
void* OSGetAlarmUserData(const OSAlarm *);

#ifdef __cplusplus
}
#endif

#endif // OSALARM_H