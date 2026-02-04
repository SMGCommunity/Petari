#ifndef LIST_H
#define LIST_H

#ifdef __cplusplus
extern "C" {
#endif

#include <cstddef>
#include <revolution/types.h>


typedef struct {
    void* prev;
    void* next;
} MEMLink;

typedef struct {
    void* head;
    void* tail;
    u16 num;
    u16 offs;
} MEMList;

void MEMInitList(MEMList*, u16);
void MEMAppendListObject(MEMList*, void*);
void MEMRemoveListObject(MEMList*, void*);
void* MEMGetNextListObject(MEMList*, void*);

#define MEM_INIT_LIST(list, structName, linkName) MEMInitList(list, offsetof(structName, linkName))

#ifdef __cplusplus
}
#endif

#endif  // LIST_H
