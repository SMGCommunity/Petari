#include "revolution/rso.h"

void RSONotifyModuleLoaded() {
    return;
}

void RSONotifyPreRSOLink() {
    return;
}

void RSONotifyPostRSOLink() {
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

int RSOGetJumpCodeSize(const RSOObjectHeader* pHeader) {
    return RSO_FAR_JUMP_SIZE * (pHeader->mExpHeader.mTableSize >> 4);
}
