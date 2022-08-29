#include <revolution/os/OSLink.h>

void __OSModuleInit(void) {
    /* SMG1 doesn't use REL so these are never used */
    *(int*)0x800030CC = 0; // last loaded REL
    *(int*)0x800030C8 = 0; // first loaded REL
    *(int*)0x800030D0 = 0; // REL name table
}