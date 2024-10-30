#include <revolution/mem/list.h>
#include <revolution/mem/expHeap.h>

/*
static MEMiHeapHead* FindContainHeap_(MEMList *pList, const void *memBlock) __attribute__((noinline)) {
    MEMiHeapHead* v3;
    MEMiHeapHead* v5;
    MEMiHeapHead* v8;
    MEMiHeapHead* v9;
    MEMiHeapHead* v10;
    MEMList* list;

    v3 = NULL;

    do {
        v10 = MEMGetNextListObject(pList, v3);
        v3 = v10;

        if (v10 == NULL) {
            return NULL;
        }
    } while (v10->heapStart > memBlock || memBlock >= v10->heapEnd);

    v5 = NULL;

    do {
        v9 = MEMGetNextListObject(&v3->childList, v5);
        v5 = v9;

        if (v9 == NULL) {

        }
    } while (v9->heapStart > memBlock || memBlock >= v9->heapEnd);
}
*/