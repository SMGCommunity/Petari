#ifndef DB_H
#define DB_H

#ifdef __cplusplus
extern "C" {
#endif

#include <revolution/types.h>

typedef struct DBInterface {
    u32 _0;
    u32 mask;
    void (*exceptionDestination)(void);
    void* exceptionReturn;
} DBInterface;

void DBInit(void);
void DBPrintf(char *, ...);

#ifdef __cplusplus
}
#endif

#endif // DB_H