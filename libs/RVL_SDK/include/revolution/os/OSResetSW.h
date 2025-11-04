#ifndef OSRESETSW_H
#define OSRESETSW_H

#ifdef __cplusplus
extern "C" {
#endif

typedef void (*OSResetCallback)(void);
typedef void (*OSPowerCallback)(void);

BOOL OSGetResetButtonState(void);

OSResetCallback OSSetResetCallback(OSResetCallback);
OSPowerCallback OSSetPowerCallback(OSPowerCallback);

#ifdef __cplusplus
}
#endif

#endif // OSRESETSW_H