#include "revolution/nwc24/NWC24DateParser.h"
#include "revolution/nwc24/NWC24Time.h"
#include "revolution/os.h"

static inline BOOL NETGetUniversalSeconds(s64* utcSec) {
    static s64 whenCached;
    NWC24Err err;

    if (whenCached == 0 || whenCached + OSSecondsToTicks(60) < __OSGetSystemTime()) {
        NWC24iSynchronizeRtcCounter(FALSE);
        whenCached = __OSGetSystemTime();
    }

    err = NWC24iGetUniversalTime(utcSec);
    if (err < 0) {
        return FALSE;
    }
    return TRUE;
}

BOOL NETGetUniversalCalendar(OSCalendarTime* time) {
    NWC24iDate nwcDate;
    s64 utcSec;

    if (NETGetUniversalSeconds(&utcSec) && NWC24iEpochSecondsToDate(&nwcDate, utcSec) >= 0 &&
        NWC24iDateToOSCalendarTime(time, &nwcDate) >= 0) {
        return TRUE;
    }

    OSTicksToCalendarTime(OSGetTime(), time);
    return FALSE;
}
