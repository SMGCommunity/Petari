#include <revolution.h>

#define TRUNC(n, a) (((u32) (n)) & ~((a) - 1))
#define ROUND(n, a) (((u32) (n) + (a) - 1) & ~((a) - 1))

volatile int __OSCurrHeap = -1;
typedef struct Cell Cell;
typedef struct HeapDesc HeapDesc;

struct Cell {
    Cell* prev;     // 0x0
    Cell* next;     // 0x4
    long size;      // 0x8
};

struct HeapDesc {
    long size;
    Cell* free;
    Cell* allocated;
};

static HeapDesc* HeapArray;
static int NumHeaps;
static void* ArenaStart;
static void* ArenaEnd;

static Cell* DLAddFront(Cell* list, Cell* cell) {
    cell->next = list;
    cell->prev = 0;

    if (list != 0) {
        list->prev = cell;
    }

    return cell;
}

static Cell* DLExtract(Cell* list, Cell* cell) {
    if (cell->next) {
        cell->next->prev = cell->prev;
    }

    if (cell->prev == 0) {
        return cell->next;
    }
    else {
        cell->prev->next = cell->next;
        return list;
    }
}

static Cell* DLInsert(Cell* list, Cell* cell) {
    Cell* prev;
    Cell* next;

    for (next = list, prev = 0; next; prev = next, next = next->next) {
        if (cell <= next) {
            break;
        }
    }

    cell->next = next;
    cell->prev = prev;

    if (next != 0) {
        next->prev = cell;

        if ((char*)cell + cell->size == (char*)next) {
            cell->size += next->size;
            cell->next = next = next->next;

            if (next != 0) {
                next->prev = cell;
            }
        }
    }
    
    if (prev != 0) {
        prev->next = cell;

        if ((char*)prev + prev->size == (char*)cell) {
            prev->size += cell->size;
            prev->next = next;

            if (next != 0) {
                next->prev = prev;
            }
        }

        return list;
    }
    else {
        return cell;
    }
}

void* OSAllocFromHeap(int heap, u32 size) {
    HeapDesc* desc;
    Cell* cell;
    Cell* newCell;
    long leftSize;

    desc = &HeapArray[heap];
    size += 0x3F;
    size = ROUND(size, 32);

    for (cell = desc->free; cell != 0; cell = cell->next) {
        if ((long)size <= cell->size) {
            break;
        }
    }

    if (cell == 0) {
        return 0;
    }

    leftSize = cell->size - size;

    // cast is probably from a macro that they have defined that likely defines a max size of some sort
    if (leftSize < (u32)0x40) {
        // inlined
        desc->free = DLExtract(desc->free, cell);
    }
    else {
        cell->size = size;
        newCell = (Cell*)((char*)cell + size);
        newCell->size = leftSize;
        newCell->prev = cell->prev;
        newCell->next = cell->next;

        if (newCell->next != 0) {
            newCell->next->prev = newCell;
        }

        if (newCell->prev != 0) {
            newCell->prev->next = newCell;
        }
        else {
            desc->free = newCell;
        }
    }

    desc->allocated = DLAddFront(desc->allocated, cell);
    return (void*)((char*)cell + 0x20);
}

void OSFreeToHeap(int heap, void *ptr) {
    HeapDesc* desc;
    Cell* cell;

    cell = (Cell*)((char*)ptr - 0x20);
    desc = &HeapArray[heap];
    desc->allocated = DLExtract(desc->allocated, cell);
    desc->free = DLInsert(desc->free, cell);
}

int OSSetCurrentHeap(int heap) {
    int prev;
    prev = __OSCurrHeap;
    __OSCurrHeap = heap;
    return prev;
}

void* OSInitAlloc(void* arenaStart, void* arenaEnd, int max) {
    u32 size;
    int i;
    size = max * 0xC;
    HeapArray = arenaStart;
    NumHeaps = max;

    for (i = 0; i < NumHeaps; i++) {
        HeapDesc* desc = &HeapArray[i];
        desc->size = -1;
        desc->free = desc->allocated = 0;
    }

    __OSCurrHeap = -1;
    arenaStart = (void*)((char*)HeapArray + size);
    arenaStart = (void*)ROUND(arenaStart, 0x20);
    ArenaStart = arenaStart;
    ArenaEnd = (void*)TRUNC(arenaEnd, 0x20);
    return arenaStart;
}

int OSCreateHeap(void *start, void *end) {
    int heap;
    HeapDesc* desc;
    Cell* cell;
    start = (void*)ROUND(start, 0x20);
    end = (void*)TRUNC(end, 0x20);

    for (heap = 0; heap < NumHeaps; heap++) {
        desc = &HeapArray[heap];

        // insert the new heap into a heap with size of -1
        if (desc->size < 0) {
            desc->size = (char*)end - (char*)start;
            cell = (Cell*)start;
            cell->prev = 0;
            cell->next = 0;
            cell->size = desc->size;
            desc->free = cell;
            desc->allocated = 0;
            return heap;
        }
    }

    return -1;
}