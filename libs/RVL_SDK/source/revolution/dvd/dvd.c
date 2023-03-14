#include <revolution/os.h>

static OSAlarm ResetAlarm;

BOOL __DVDLowTestAlarm(const OSAlarm *);

BOOL __DVDTestAlarm(const OSAlarm* alarm) {
    if (alarm == &ResetAlarm) {
        return TRUE;
    }

    return __DVDLowTestAlarm(alarm);
}

BOOL __DVDStopMotorAsync(DVDCommandBlock* block, DVDCBCallback callback) {
    return TRUE;
}

void __DVDRestartMotor(void) {
    return;
}