#ifndef OSINTERRUPT_H
#define OSINTERRUPT_H

#ifdef __cplusplus
extern "C" {
#endif

void OSDisableInterrupts();
void OSRestoreInterrupts();

#ifdef __cplusplus
}
#endif

#endif // OSINERRUPT_H