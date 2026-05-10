#include "revolution/nwc24/NWC24DateParser.h"
#include "revolution/nwc24/NWC24Time.h"
#include "revolution/os.h"

// https://decomp.me/scratch/Gda4L
BOOL NETGetUniversalCalendar(OSCalendarTime* time) {
    static s64 whenCached = 0;
    NWC24iDate date;
    s64 universalTime;
    s64 now;

    now = __OSGetSystemTime();
    if (whenCached + OSSecondsToTicks(60) >= now) {
        goto use_cache;
    }

    if (whenCached == 0) {
        goto update;
    }

update:
    NWC24iSynchronizeRtcCounter(FALSE);
    now = __OSGetSystemTime();
    whenCached = now;
use_cache:
    if (NWC24iGetUniversalTime(&universalTime) < 0) {
        goto error;
    }
    if (NWC24iEpochSecondsToDate(&date, universalTime) < 0) {
        goto error;
    }
    if (NWC24iDateToOSCalendarTime(time, &date) < 0) {
        goto error;
    }
    return TRUE;
error:
    OSTicksToCalendarTime(OSGetTime(), time);
    return FALSE;
}
