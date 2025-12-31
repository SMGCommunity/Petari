#ifndef D_HASH_H
#define D_HASH_H

#include <revolution.h>

int _MakeWStr(const char*);
void dHash_InitHashTable();
int dHash_SearchHashW(const u16*);
int dHash_GetArg(const char*);
int dHash_SetArg(const char*, u8);
int dHash_DeleteData(const char*);

#endif  // D_HASH_H
