#ifndef MUTEX_TRK_H
#define MUTEX_TRK_H

#include "TRK_Types.h"

typedef unsigned int DSMutex;

TRKResult TRKReleaseMutex(void *);
TRKResult TRKAcquireMutex(void *);
TRKResult TRKInitializeMutex(void *);


#endif // MUTEX_TRK_H