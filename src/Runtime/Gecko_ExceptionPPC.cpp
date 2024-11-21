#include <NMWException.h>

typedef struct ProcessInfo {
	__eti_init_info* exception_info;
	char* TOC;
	int active;
} ProcessInfo;

static ProcessInfo fragmentinfo[1];

int __register_fragment(struct __eti_init_info *pInfo, char *pTOC) {
    ProcessInfo* info;
    int i;

    for (i = 0, info = fragmentinfo; i < 1; ++i, ++info) {
        if (info->active == 0) {
            info->exception_info = pInfo;
            info->TOC = pTOC;
            info->active = 1;
            return i;
        }
    }
    

    return -1;
}

void __unregister_fragment(int id) {
    ProcessInfo* info;

    if (id >= 0 && id < 1) {
        info = &fragmentinfo[id];
        info->exception_info = 0;
        info->TOC = 0;
        info->active = 0;
    }
}