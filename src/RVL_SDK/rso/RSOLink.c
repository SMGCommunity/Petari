#include "revolution/os.h"
#include "revolution/rso.h"

struct RSORel {
    u32 offset;
    u32 info;
    u32 addend;
};

#define RSO_STATIC_LIST ((RSOObjectList*)0x80003168)

void RSONotifyModuleLoaded(RSOObjectHeader*) NO_INLINE {
    return;
}

void RSONotifyPreRSOLink(RSOObjectHeader*, const RSOObjectHeader*) NO_INLINE {
    return;
}

void RSONotifyPostRSOLink(RSOObjectHeader*, const RSOObjectHeader*) NO_INLINE {
    return;
}

void RSONotifyPreRSOLinkFar(RSOObjectHeader*, const RSOObjectHeader*) NO_INLINE {
    return;
}

void RSONotifyPostRSOLinkFar(RSOObjectHeader*, const RSOObjectHeader*, void*) NO_INLINE {
    return;
}

static RSOImportTable* RSOGetImport(const RSOSymbolHeader* imp);
static int RSOGetNumImportSymbols(const RSOSymbolHeader* imp);
void RSORelocate(RSORel* rel, int index, u32 offset);

// these are for linking the new object
extern char _f_sbss2[];
extern char _f_sdata2[];
extern char _f_sbss[];
extern char _f_sdata[];
extern char _f_bss[];
extern char _f_data[];
extern char _f_rodata[];
extern char _f_text[];
extern char _f_init[];

int LocateObject(void* newModule, void* bss, int i_fix_level) {
    u32 i;
    RSOSectionInfo* si;
    RSOObjectHeader* moduleHeader;
    RSORel* intRel;
    RSORel* extRel;
    u32 a_max;

    moduleHeader = (RSOObjectHeader*)newModule;
    moduleHeader->mBssSection = 0;
    moduleHeader->mInfo.mSectionInfoOffset += (u32)moduleHeader;
    moduleHeader->mInfo.mNameOffset += (u32)moduleHeader;
    moduleHeader->mInternalRelOffset += (u32)moduleHeader;
    moduleHeader->mExternalRelOffset += (u32)moduleHeader;
    moduleHeader->mExpHeader.mTableOffset += (u32)moduleHeader;
    moduleHeader->mExpHeader.mStringOffset += (u32)moduleHeader;
    moduleHeader->mImpHeader.mTableOffset += (u32)moduleHeader;
    moduleHeader->mImpHeader.mStringOffset += (u32)moduleHeader;

    for (i = 1; i < moduleHeader->mInfo.mNumSections; i++) {
        si = &((RSOSectionInfo*)moduleHeader->mInfo.mSectionInfoOffset)[i];
        if (si->mOffset != 0) {
            si->mOffset += (u32)moduleHeader;
        } else if (si->mSize != 0) {
            moduleHeader->mBssSection = i;
            si->mOffset = (u32)bss;
        }
    }

    if (moduleHeader->mPrologSection != 0) {
        moduleHeader->mProlog += ((RSOSectionInfo*)moduleHeader->mInfo.mSectionInfoOffset)[moduleHeader->mPrologSection].mOffset;
    }
    if (moduleHeader->mEpilogSection != 0) {
        moduleHeader->mEpilog += ((RSOSectionInfo*)moduleHeader->mInfo.mSectionInfoOffset)[moduleHeader->mEpilogSection].mOffset;
    }
    if (moduleHeader->mUnresolvedSection != 0) {
        moduleHeader->mUnresolved += ((RSOSectionInfo*)moduleHeader->mInfo.mSectionInfoOffset)[moduleHeader->mUnresolvedSection].mOffset;
    }

    a_max = moduleHeader->mInternalRelSize / sizeof(RSORel);
    for (i = 0; i < a_max; i++) {
        intRel = &((RSORel*)moduleHeader->mInternalRelOffset)[i];
        intRel->offset += (u32)moduleHeader;
        RSORelocate(intRel, 0, ((RSOSectionInfo*)moduleHeader->mInfo.mSectionInfoOffset)[intRel->info >> 8].mOffset);
    }

    if (i_fix_level >= 1) {
        moduleHeader->mInternalRelSize = 0;
    }

    a_max = moduleHeader->mExternalRelSize / sizeof(RSORel);
    for (i = 0; i < a_max; i++) {
        ((RSORel*)moduleHeader->mExternalRelOffset)[i].offset += (u32)moduleHeader;
    }

    a_max = RSOGetNumImportSymbols(&moduleHeader->mImpHeader);
    for (i = 0; i < a_max; i++) {
        RSOImportTable* impTab = &RSOGetImport(&moduleHeader->mImpHeader)[i];
        impTab->value = moduleHeader->mUnresolved;
        extRel = (RSORel*)(moduleHeader->mExternalRelOffset + impTab->relOffset);
        while ((extRel->info >> 8) == (u32)i) {
            RSORelocate(extRel, 0, impTab->value);
            extRel++;
        }
    }

    if (i_fix_level <= 1) {
        memset(bss, 0, moduleHeader->mBssSize);
    }

    RSONotifyModuleLoaded(moduleHeader);
    return 1;
}

int RSOStaticLocateObject(void* newModule) {
    RSOObjectHeader* moduleHeader;
    u32 i;
    u32 a_max;
    RSOSectionInfo* si;

    moduleHeader = (RSOObjectHeader*)newModule;
    moduleHeader->mBssSection = 0;
    // I'm not sure if  this is actually what it's doing
    moduleHeader->mInfo.mSectionInfoOffset += (u32)moduleHeader;
    moduleHeader->mInfo.mNameOffset += (u32)moduleHeader;
    moduleHeader->mInternalRelOffset += (u32)moduleHeader;
    moduleHeader->mExternalRelOffset += (u32)moduleHeader;
    moduleHeader->mExpHeader.mTableOffset += (u32)moduleHeader;
    moduleHeader->mExpHeader.mStringOffset += (u32)moduleHeader;
    moduleHeader->mImpHeader.mTableOffset += (u32)moduleHeader;
    moduleHeader->mImpHeader.mStringOffset += (u32)moduleHeader;

    for (i = 1; i < moduleHeader->mInfo.mNumSections; i++) {
        si = &((RSOSectionInfo*)moduleHeader->mInfo.mSectionInfoOffset)[i];

        switch (i) {
        case 1:
            si->mOffset = (u32)_f_init;
            break;
        case 2:
            si->mOffset = (u32)_f_text;
            break;
        case 3:
            if (si->mSize != 0) {
                OSReport("Warrning! .ctors section[%d]! size=%x\n", i, si->mSize);
            }
            si->mOffset = 0;
            break;
        case 4:
            if (si->mSize != 0) {
                OSReport("Warrning! .dtors section[%d]! size=%x\n", i, si->mSize);
            }
            si->mOffset = 0;
            break;
        case 5:
            si->mOffset = (u32)_f_rodata;
            break;
        case 6:
            si->mOffset = (u32)_f_data;
            break;
        case 7:
            si->mOffset = (u32)_f_bss;
            break;
        case 8:
            si->mOffset = (u32)_f_sdata;
            break;
        case 9:
            si->mOffset = (u32)_f_sbss;
            break;
        case 10:
            si->mOffset = (u32)_f_sdata2;
            break;
        case 11:
            si->mOffset = (u32)_f_sbss2;
            break;
        case 12:
            si->mOffset = 0;
            break;
        case 13:
            si->mOffset = 0;
            break;
        default:
            if (si->mSize != 0) {
                OSReport("Warning! unknown section[%d]! size=%x\n", i, si->mSize);
            }
            si->mOffset = 0;
            break;
        }
    }

    a_max = moduleHeader->mExternalRelSize / sizeof(RSORel);
    for (i = 0; i < a_max; i++) {
        ((RSORel*)moduleHeader->mExternalRelOffset)[i].offset += (u32)moduleHeader;
    }

    return 1;
}

static RSOImportTable* RSOGetImport(const RSOSymbolHeader* imp) {
    return (RSOImportTable*)imp->mTableOffset;
}

static int RSOGetNumImportSymbols(const RSOSymbolHeader* imp) {
    return imp->mTableSize / sizeof(RSOImportTable);
}

void RSORelocateSmallDataSection(RSOObjectHeader* rsoImp, int impIndex, RSOObjectHeader* rsoExp);

static void RSORelocateImportSymbol(RSOObjectHeader* rso, RSOImportTable* impTab, int impIndex) {
    RSORel* rel = (RSORel*)(rso->mExternalRelOffset + impTab->relOffset);

    while ((rel->info >> 8) == (u32)impIndex) {
        RSORelocate(rel, 0, impTab->value);
        rel++;
    }
}

static void RSOResolveImportSymbol(RSOObjectHeader* rsoImp, int index, void* addr) {
    RSOImportTable* impTab = &RSOGetImport(&rsoImp->mImpHeader)[index];
    impTab->value = (u32)addr;
    RSORelocateImportSymbol(rsoImp, impTab, index);
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

    for (i = 0; i < s_max; i++, impTab++) {
        impName = (const char*)(impTab->strOffset + imp->mStringOffset);

        if (impName != 0) {
            RSORelocateSmallDataSection(rsoImp, i, (RSOObjectHeader*)rsoExp);
            addr = RSOFindExportSymbolAddr(rsoExp, impName);

            if (addr != 0) {
                RSOResolveImportSymbol(rsoImp, i, (void*)addr);
                count++;
            }
        }
    }

    RSONotifyPostRSOLink(rsoImp, rsoExp);
    return count;
}

static BOOL RSOIsImportSymbolResolved(const RSOObjectHeader* rso, int index) {
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

static int RSOGetNumExportSymbols(const RSOSymbolHeader* exp) {
    return exp->mTableSize >> 4;
}

static RSOHash RSOGetHash(const char* symbolName) {
    char v2;
    int v3;
    u32 v4;
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

static void* RSOGetExportSymbolAddr(const RSOObjectHeader* rso, int index) {
    RSOExportTable* expTab = &((RSOExportTable*)rso->mExpHeader.mTableOffset)[index];
    RSOSectionInfo* expSec = &((RSOSectionInfo*)rso->mInfo.mSectionInfoOffset)[expTab->section];
    return (void*)(expSec->mOffset + expTab->value);
}

static char* RSOGetExportSymbolName(const RSOSymbolHeader* exp, int index) {
    RSOExportTable* expTab = (RSOExportTable*)exp->mTableOffset + index;
    return (char*)(exp->mStringOffset + expTab->strOffset);
}

int FindExportIndex(const RSOObjectHeader* rso, const char* name) {
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

void RSORelocate(RSORel* rel, int index, u32 offset) {
    u32* p;
    u32 x;
    u32 y;
    RSORel* targetRel = &rel[index];

    p = (u32*)targetRel->offset;

    switch ((u8)rel->info) {
    case 0:
        break;
    case 1:
        *p = offset + rel->addend;
        break;
    case 2:
        *p = (*p & 0xFC000003) | ((offset + rel->addend) & 0x03FFFFFC);
        break;
    case 3:
        *(u16*)p = (u16)(offset + rel->addend);
        break;
    case 4:
        *(u16*)p = (u16)(offset + rel->addend);
        break;
    case 5:
        *(u16*)p = (u16)((offset + rel->addend) >> 16);
        break;
    case 6:
        x = offset + rel->addend;
        *(u16*)p = (u16)((x >> 16) + ((x >> 15) & 1));
        break;
    case 7:
    case 8:
    case 9:
        *p = (*p & 0xFFFF0003) | ((offset + rel->addend) & 0xFFFC);
        break;
    case 10:
        x = (*p & 0xFC000003) | (((offset + rel->addend) - (u32)p) & 0x03FFFFFC);
        *p = x;
        y = offset + rel->addend;
        if (y != 0 && y != (((u32)p & 0xFC000003) | ((((u32)p & 0x03FFFFFC) + (x & 0x03FFFFFC)) & 0x03FFFFFC))) {
            *p = (x & 0xFC000003) | 0x03FFFFFC;
        }
        break;
    case 11:
    case 12:
    case 13:
        *p = (*p & 0xFFFF0003) | (((offset + rel->addend) - (u32)p) & 0xFFFC);
        break;
    case 109:
        break;
    default:
        OSReport("OSLink: unknown relocation type %3d\n", (u8)rel->info);
        break;
    }

    DCFlushRange(p, 0x20);
    ICInvalidateRange(p, 0x20);
}

static RSOExportTable* RSOFindExportSymbol(const RSOObjectHeader* rso, const char* name) {
    int a_idx = FindExportIndex(rso, name);

    if (a_idx == -1) {
        return NULL;
    } else {
        return &((RSOExportTable*)rso->mExpHeader.mTableOffset)[a_idx];
    }
}

void RSORelocateSmallDataSection(RSOObjectHeader* rsoImp, int impIndex, RSOObjectHeader* rsoExp) {
    u8* p;
    u32 base;
    const char* impName;
    RSOImportTable* impTab;
    RSOExportTable* expTab;
    RSOExportTable* baseTab;
    RSORel* rel;
    RSOSectionInfo* si;

    impTab = &RSOGetImport(&rsoImp->mImpHeader)[impIndex];
    rel = (RSORel*)(rsoImp->mExternalRelOffset + impTab->relOffset);
    impName = (const char*)(impTab->strOffset + rsoImp->mImpHeader.mStringOffset);

    while ((rel->info >> 8) == (u32)impIndex) {
        if ((u8)rel->info != 109) {
            rel++;
            continue;
        }

        p = (u8*)rel->offset;

        switch ((u32)p & 3) {
        case 0:
            p += 1;
            break;
        case 2:
            p -= 1;
            break;
        case 3:
            p -= 2;
            break;
        }

        expTab = RSOFindExportSymbol(rsoExp, impName);
        if (expTab == NULL) {
            rel++;
            continue;
        }

        switch (expTab->section) {
        case 8:
        case 11:
            *p = (*p & 0xE0) | 0xD;
            baseTab = RSOFindExportSymbol(rsoExp, "_SDA_BASE_");
            if (baseTab == NULL) {
                rel++;
                continue;
            }
            base = baseTab->value;
            break;
        case 9:
        case 12:
            *p = (*p & 0xE0) | 2;
            baseTab = RSOFindExportSymbol(rsoExp, "_SDA2_BASE_");
            if (baseTab == NULL) {
                rel++;
                continue;
            }
            base = baseTab->value;
            break;
        case 10:
        case 13:
            base = 0;
            *p &= 0xE0;
            break;
        case 0xF1:
        default:
            OSReport("ERROR: incorrect R_PPC_EMB_SDA21 data.\n");
            break;
        }

        si = &((RSOSectionInfo*)rsoExp->mInfo.mSectionInfoOffset)[expTab->section];
        *(u16*)(p + 1) = (u16)((si->mOffset + expTab->value + rel->addend) - base);
        DCFlushRange(p, 0x20);
        ICInvalidateRange(p, 0x20);

        rel++;
    }
}

int RSOListInit(void* i_staticRso) {
    int r;
    RSOObjectInfo* __prev;

    RSO_STATIC_LIST->mTail = NULL;
    RSO_STATIC_LIST->mHead = NULL;

    r = RSOStaticLocateObject(i_staticRso);

    __prev = RSO_STATIC_LIST->mTail;
    if (__prev == NULL) {
        RSO_STATIC_LIST->mHead = (RSOObjectInfo*)i_staticRso;
    } else {
        __prev->mLink.mNext = (RSOObjectInfo*)i_staticRso;
    }

    ((RSOObjectInfo*)i_staticRso)->mLink.mPrev = __prev;
    ((RSOObjectInfo*)i_staticRso)->mLink.mNext = NULL;
    RSO_STATIC_LIST->mTail = (RSOObjectInfo*)i_staticRso;

    return r;
}

int LinkList(void* i_newRso, void* i_bss, int i_fixed_level) {
    int r;
    RSOObjectHeader* a_rsoInfo;
    RSOObjectInfo* a_target;
    RSOObjectInfo* __prev;

    r = LocateObject(i_newRso, i_bss, i_fixed_level);
    if (r == 0) {
        return 0;
    }

    a_target = RSO_STATIC_LIST->mHead;
    while (a_target != NULL) {
        RSOLink((RSOObjectHeader*)i_newRso, (RSOObjectHeader*)a_target);
        RSOLink((RSOObjectHeader*)a_target, (RSOObjectHeader*)i_newRso);
        a_target = a_target->mLink.mNext;
    }

    __prev = RSO_STATIC_LIST->mTail;
    if (__prev == NULL) {
        RSO_STATIC_LIST->mHead = (RSOObjectInfo*)i_newRso;
    } else {
        __prev->mLink.mNext = (RSOObjectInfo*)i_newRso;
    }

    ((RSOObjectInfo*)i_newRso)->mLink.mPrev = __prev;
    ((RSOObjectInfo*)i_newRso)->mLink.mNext = NULL;
    RSO_STATIC_LIST->mTail = (RSOObjectInfo*)i_newRso;

    if (i_fixed_level >= 2) {
        a_rsoInfo = (RSOObjectHeader*)i_newRso;
        memset(i_bss, 0, a_rsoInfo->mBssSize);
        a_rsoInfo->mExternalRelSize = 0;
        a_rsoInfo->mImpHeader.mTableSize = 0;
    }

    return 1;
}

BOOL RSOLinkList(void* i_newRso, void* i_bss) {
    return LinkList(i_newRso, i_bss, 0);
}

int cnvJumpCode(const RSOObjectHeader* rso, RSOImportTable* impTab, int impIndex, u32 addr, u32* i_buff) {
    RSORel* rel;
    u32 offset;
    u32* p;
    u32 x;
    u32 y;
    int i_sw;

    rel = (RSORel*)(rso->mExternalRelOffset + impTab->relOffset);
    i_sw = 0;

    while ((rel->info >> 8) == (u32)impIndex) {
        if ((u8)rel->info == 10) {
            p = (u32*)rel->offset;
            offset = addr + rel->addend;
            x = (*p & 0xFC000003) | ((offset - (u32)p) & 0x03FFFFFC);
            *p = x;
            y = addr + rel->addend;
            if (y != 0 && y != (((u32)p & 0xFC000003) | ((((u32)p & 0x03FFFFFC) + (x & 0x03FFFFFC)) & 0x03FFFFFC))) {
                x = (x & 0xFC000003) | (((u32)i_buff - (u32)p) & 0x03FFFFFC);
                i_sw = 1;
                *p = x;
                DCFlushRange(p, 0x20);
                ICInvalidateRange(p, 0x20);
            }
        }

        rel++;
    }

    return i_sw;
}

int RSOGetJumpCodeSize(const RSOObjectHeader* pHeader) {
    return RSO_FAR_JUMP_SIZE * (pHeader->mExpHeader.mTableSize >> 4);
}

static void makeCode(u32 addr, u32* i_buff) {
    i_buff[0] = 0x91810004;
    i_buff[1] = 0x3D800000 | ((addr >> 16) + ((addr >> 15) & 1));
    i_buff[2] = 0x398C0000 | (addr & 0xFFFF);
    i_buff[3] = 0x7D8903A6;
    i_buff[4] = 0x81810004;
    i_buff[5] = 0x4E800420;
}

void RSOMakeJumpCode(const RSOObjectHeader* i_rsoExp, void* i_buff) {
    int s_max;
    u32* r_buff;
    u32 a_addr;
    int i;

    s_max = i_rsoExp->mExpHeader.mTableSize >> 4;
    r_buff = (u32*)i_buff;

    for (i = 0; i < s_max; i++) {
        a_addr = (u32)RSOGetExportSymbolAddr(i_rsoExp, i);
        makeCode(a_addr, r_buff);
        r_buff += RSO_FAR_JUMP_SIZE / sizeof(u32);
    }

    DCFlushRange(i_buff, s_max * RSO_FAR_JUMP_SIZE);
    ICInvalidateRange(i_buff, s_max * RSO_FAR_JUMP_SIZE);
}

int RSOLinkJump(RSOObjectHeader* i_rsoImp, const RSOObjectHeader* i_rsoExp, void* i_buff) {
    RSOSymbolHeader* imp;
    int i, count, s_max;
    const char* impName;
    RSOImportTable* impTab;
    int a_idx;
    u32* a_buff;

    imp = &i_rsoImp->mImpHeader;
    count = 0;
    s_max = RSOGetNumImportSymbols(&i_rsoImp->mImpHeader);
    impTab = (RSOImportTable*)imp->mTableOffset;

    RSONotifyPreRSOLinkFar(i_rsoImp, i_rsoExp);

    for (i = 0; i < s_max; i++, impTab++) {
        impName = (const char*)(impTab->strOffset + imp->mStringOffset);

        if (impName != 0) {
            a_idx = FindExportIndex(i_rsoExp, impName);

            if (a_idx >= 0) {
                a_buff = (u32*)i_buff + a_idx * (RSO_FAR_JUMP_SIZE / sizeof(u32));
                if (cnvJumpCode(i_rsoImp, impTab, i, (u32)RSOGetExportSymbolAddr(i_rsoExp, a_idx), a_buff) != 0) {
                    count++;
                }
            }
        }
    }

    RSONotifyPostRSOLinkFar(i_rsoImp, i_rsoExp, i_buff);
    return count;
}
