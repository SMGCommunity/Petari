#include "revolution/nwc24/NWC24DateParser.h"
#include "revolution/nwc24/NWC24Time.h"
#include "revolution/os.h"

static s64 whenCached = 0;

// https://decomp.me/scratch/Gda4L
BOOL NETGetUniversalCalendar(OSCalendarTime* time) {
    NWC24iDate date;
    s64 universalTime;

    if (whenCached == 0) {
        goto update;
    }

    if (whenCached + OSSecondsToTicks(60) >= __OSGetSystemTime()) {
        goto use_cache;
    }

update:
    NWC24iSynchronizeRtcCounter(FALSE);
    whenCached = __OSGetSystemTime();
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
