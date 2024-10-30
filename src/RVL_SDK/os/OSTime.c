#include <revolution/os/OSTime.h>
#include <revolution/os.h>

static int YearDays[] = { 0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334 };
static int LeapYearDays[] = { 0, 31, 60, 91, 121, 152, 182, 213, 244, 274, 305, 335 };

asm OSTime OSGetTime(void) {
    nofralloc

loop:
    mftbu r3
    mftb r4
    mftbu r5
    cmpw r3, r5
    bne loop
    blr
}

asm OSTick OSGetTick(void) {
    nofralloc
    mftb r3
    blr
}

OSTime __OSGetSystemTime(void) {
    BOOL en;
    OSTime* time = (OSTime*)(0x800030D8);
    OSTime res;

    en = OSDisableInterrupts();
    res = *time + OSGetTime();
    OSRestoreInterrupts(en);
    return res;
}

OSTime __OSTimeToSystemTime(OSTime time) {
    BOOL en;
    OSTime* os_time = (OSTime*)(0x800030D8);
    OSTime res;

    en = OSDisableInterrupts();
    res = *os_time + time;
    OSRestoreInterrupts(en);
    return res;
}

static BOOL IsLeapYear(int year) {
    return (year % 4 == 0 && year % 100 != 0) || year % 400 == 0;
}

static int GetLeapDays(int year) {
    if (year < 1) {
        return 0;
    }

    return (year + 3) / 4 - (year - 1) / 100 + (year - 1) / 400; 
}

static void GetDates(s32 days, OSCalendarTime *pTime) NO_INLINE;

void OSTicksToCalendarTime(OSTime ticks, OSCalendarTime *pTime) {
    int numDays;
    int numSecs;
    OSTime ticksAfter;

    ticksAfter = ticks % OSSecondsToTicks(1);

    if (ticksAfter < 0) {
        ticksAfter += OSSecondsToTicks(1);
    }

    pTime->usec = (int)(OSTicksToMicroseconds(ticksAfter) % 1000);
    pTime->msec = (int)(OSTicksToMilliseconds(ticksAfter) % 1000);

    ticks -= ticksAfter;

    numDays = (int)(OSTicksToSeconds(ticks) / 86400 + 0xB2575);
    numSecs = (int)(OSTicksToSeconds(ticks) % 86400);

    if (numSecs < 0) {
        numDays -= 1;
        numSecs += 86400;
    }

    GetDates(numDays, pTime);
    pTime->hour = numSecs / 60 / 60;
    pTime->min = (numSecs / 60) % 60;
    pTime->sec = numSecs % 60;
}

static void GetDates(s32 days, OSCalendarTime *pTime) {
    int year;
    int dayCount;
    int month;
    int* monthArr;

    pTime->wday = (days + 6) % 7;

    for (year = days / 365; days < (dayCount = GetLeapDays(year) + 365 * year); --year);

    days -= dayCount;
    pTime->year = year;
    pTime->yday = days;

    monthArr = IsLeapYear(year) ? LeapYearDays : YearDays;

    for (month = 12; days < monthArr[--month];);

    pTime->mon = month;
    pTime->mday = days - monthArr[month] + 1;
}