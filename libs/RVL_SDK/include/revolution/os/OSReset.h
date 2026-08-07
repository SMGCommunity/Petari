#ifndef OSRESET_H
#define OSRESET_H

#include <revolution/types.h>

#ifdef __cplusplus
extern "C" {
#endif

#define OS_SETTING_CALENDAR 1u
#define OS_SETTING_DISPLAY 2u
#define OS_SETTING_SOUND 3u
#define OS_SETTING_PARENTAL 4u
#define OS_SETTING_INTERNET 5u
#define OS_SETTING_WC24 6u
#define OS_SETTING_UPDATE 7u

#define OS_SYSLAUNCH_URL_CALENDAR "Calendar/Calendar_index.html"
#define OS_SYSLAUNCH_URL_DISPLAY "Display/Display_index.html"
#define OS_SYSLAUNCH_URL_SOUND "Sound/Sound_index.html"
#define OS_SYSLAUNCH_URL_PARENTAL "Parental_Control/Parental_Control_index.html"
#define OS_SYSLAUNCH_URL_INTERNET "Internet/Internet_index.html"
#define OS_SYSLAUNCH_URL_WC24 "WiiConnect24/Wiiconnect24_index.html"
#define OS_SYSLAUNCH_URL_UPDATE "Update/Update_index.html"

typedef BOOL (*OSShutdownFunction)(BOOL, u32);
typedef struct OSShutdownFunctionInfo OSShutdownFunctionInfo;

struct OSShutdownFunctionInfo {
    OSShutdownFunction func;
    u32 priority;
    OSShutdownFunctionInfo* next;
    OSShutdownFunctionInfo* prev;
};

void OSRegisterShutdownFunction(OSShutdownFunctionInfo*);

void OSRebootSystem(void);
void OSShutdownSystem(void);
void OSRestart(u32);
void OSReturnToMenu(void);

u32 OSGetResetCode(void);

void OSResetSystem(int, u32, BOOL);

BOOL __OSCallShutdownFunctions(BOOL, u32);
void __OSReturnToMenuForError(void);

u8 __OSGetDiscState(u8);
void __OSShutdownDevices(u32);
void __OSHotResetForError(void);

void __OSReturnToMenul(u32, const char*, ...);

#define OSIsRestart() ((OSGetResetCode() & 0x80000000) ? TRUE : FALSE)

#ifdef __cplusplus
}
#endif

#endif  // OSRESET_H
