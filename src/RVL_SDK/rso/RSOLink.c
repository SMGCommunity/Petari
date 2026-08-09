#include "revolution/rso.h"

void RSONotifyModuleLoaded() {
    return;
}

void RSONotifyPreRSOLink(RSOObjectHeader*, const RSOObjectHeader*) NO_INLINE {
    return;
}

void RSONotifyPostRSOLink(RSOObjectHeader*, const RSOObjectHeader*) NO_INLINE {
    return;
}

void RSONotifyPreRSOLinkFar() {
    return;
}

void RSONotifyPostRSOLinkFar() {
    return;
}

RSOImportTable* RSOGetImport(const RSOSymbolHeader* imp) {
    return (RSOImportTable*)imp->mTableOffset;
}

int RSOGetNumImportSymbols(const RSOSymbolHeader* imp) {
    return imp->mTableSize / sizeof(RSOImportTable);
}

BOOL RSOIsImportSymbolResolved(const RSOObjectHeader* rso, int index) {
    u32 v = RSOGetImport(&rso->mImpHeader)[index].value;

    if (v != rso->mUnresolved) {
        return TRUE;
    }

    return FALSE;
}

BOOL RSOIsImportSymbolResolvedAll(const RSOObjectHeader* rso) {
    int numSymbols, i;
    numSymbols = RSOGetNumImportSymbols(&rso->mImpHeader);
    for (i = 0; i < numSymbols; i++) {
        if (!RSOIsImportSymbolResolved(rso, i)) {
            return FALSE;
        }
    }

    return TRUE;
}

int RSOGetNumExportSymbols(const RSOSymbolHeader* exp) {
    return exp->mTableSize >> 4;
}

RSOHash RSOGetHash(const char* symbolName) {
    char v2;
    int v3;
    unsigned int v4;
    RSOHash hash = 0;

    while (*symbolName != 0) {
        v2 = *symbolName++;
        v3 = 16 * hash + v2;
        v4 = v3 & 0xF0000000;
        if (v4 != 0) {
            v3 ^= v4 >> 24;
        }

        hash = v3 & ~v4;
    }

    return hash;
}

void RSORelocateSmallDataSection(RSOObjectHeader* rsoImp, int impIndex, RSOObjectHeader* rsoExp);

void* RSOGetExportSymbolAddr(const RSOObjectHeader* rso, int index) {
    RSOExportTable* expTab = &((RSOExportTable*)rso->mExpHeader.mTableOffset)[index];
    RSOSectionInfo* expSec = &((RSOSectionInfo*)rso->mInfo.mSectionInfoOffset)[expTab->section];
    return (void*)(expSec->mOffset + expTab->value);
}

static void RSOResolveImportSymbol(RSOObjectHeader* rsoImp, int index, void* addr);

char* RSOGetExportSymbolName(const RSOSymbolHeader* exp, int index) {
    RSOExportTable* expTab = (RSOExportTable*)exp->mTableOffset + index;
    return (char*)(exp->mStringOffset + expTab->strOffset);
}

static int FindExportIndex(const RSOObjectHeader* rso, const char* name) {
    u32 a_hash = RSOGetHash(name);
    int i;
    const char* expName;
    int s_max = RSOGetNumExportSymbols(&rso->mExpHeader);
    RSOExportTable* expTab = (RSOExportTable*)rso->mExpHeader.mTableOffset;
    RSOExportTable* a_expTab;
    int a_top = 0;
    int a_last = s_max - 1;
    int a_idx = -1;

    if (s_max <= 0) {
        return -1;
    }

    while (a_idx == -1) {
        i = (a_top + a_last) >> 1;
        a_expTab = &expTab[i];

        if (a_hash > a_expTab->hash) {
            if (a_top == i) {
                a_idx = a_last;
            } else {
                a_top = i;
            }
        } else if (a_hash < a_expTab->hash) {
            if (a_top == i) {
                a_idx = a_top;
            } else {
                a_last = i;
            }
        } else {
            a_idx = i;
        }
    }

    a_expTab = &expTab[a_idx];
    if (a_hash != a_expTab->hash) {
        return -1;
    }

    expName = RSOGetExportSymbolName(&rso->mExpHeader, a_idx);
    if (!strcmp(name, expName)) {
        return a_idx;
    }

    for (i = a_idx + 1; i <= a_last; i++) {
        a_expTab = &expTab[i];
        if (a_hash == a_expTab->hash) {
            expName = RSOGetExportSymbolName(&rso->mExpHeader, i);
            if (!strcmp(name, expName)) {
                return i;
            }
        } else {
            i = a_last + 1;
        }
    }

    for (i = a_idx - 1; i >= a_top; i--) {
        a_expTab = &expTab[i];
        if (a_hash == a_expTab->hash) {
            expName = RSOGetExportSymbolName(&rso->mExpHeader, i);
            if (!strcmp(name, expName)) {
                return i;
            }
        } else {
            return -1;
        }
    }

    return -1;
}
void* RSOFindExportSymbolAddr(const RSOObjectHeader* rso, const char* name) {
    int a_idx = FindExportIndex(rso, name);

    if (a_idx == -1) {
        return NULL;
    } else {
        return RSOGetExportSymbolAddr(rso, a_idx);
    }
}

int RSOLink(RSOObjectHeader* rsoImp, const RSOObjectHeader* rsoExp) {
    RSOSymbolHeader* imp;
    int i, count, s_max;
    const char* impName;
    const u32* addr;
    RSOImportTable* impTab;

    imp = &rsoImp->mImpHeader;
    count = 0;
    s_max = RSOGetNumImportSymbols(&rsoImp->mImpHeader);
    impTab = (RSOImportTable*)imp->mTableOffset;

    RSONotifyPreRSOLink(rsoImp, rsoExp);

    for (i = 0; i < s_max; i++) {
        const char* v9 = (const char*)(impTab->strOffset + imp->mStringOffset);

        if (v9 != 0) {
            RSORelocateSmallDataSection(rsoImp, i, (RSOObjectHeader*)rsoExp);
            addr = RSOFindExportSymbolAddr(rsoExp, v9);

            if (addr != 0) {
                RSOResolveImportSymbol(rsoImp, i, (void*)addr);
                count++;
            }
        }

        impTab++;
    }

    RSONotifyPostRSOLink(rsoImp, rsoExp);
    return count;
}

static int LinkList(void* i_newRso, void* i_bss, int i_fixed_level);

BOOL RSOLinkList(void* i_newRso, void* i_bss) {
    return LinkList(i_newRso, i_bss, 0);
}

int RSOGetJumpCodeSize(const RSOObjectHeader* pHeader) {
    return RSO_FAR_JUMP_SIZE * (pHeader->mExpHeader.mTableSize >> 4);
}
