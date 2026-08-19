#include "revolution/vf/d_hash.h"
#include "revolution/vf/pf_clib.h"
#include "revolution/vf/vf_struct.h"

extern PF_VOLUME_SET VFipf_vol_set;

typedef struct {
    unsigned short Name[8];
    unsigned char arg;
} VF_HashTableEntry;

static u16 l_tmpWName[8];
static VF_HashTableEntry hashTable[31];

static u16* _GetWStr() {
    return l_tmpWName;
}

static u32 _StrLen(const char* i_Name) {
    u32 len;

    for (len = 0; *i_Name != 0; i_Name++) {
        len++;
    }
    return len;
}

int _MakeWStr(const char* i_Name) {
    u16* p;
    u32 len;
    int Id;
    int next2nd;

    if (_StrLen(i_Name) < 8) {
        len = _StrLen(i_Name);
        p = l_tmpWName;
        next2nd = 0;
        for (Id = 0; Id < len; Id++) {
            if (next2nd) {
                next2nd = 0;
            } else {
                if (VFipf_vol_set.codeset.is_oem_mb_char(i_Name[Id], 1) != 0) {
                    next2nd = 1;
                    *p = ((u32)i_Name[Id + 1] & 0xFFFF) | (i_Name[Id] << 8);
                } else {
                    *p = i_Name[Id];
                }
            }
            p++;
        }
        l_tmpWName[7] = 0;
        l_tmpWName[len] = 0;
        return 1;
    }

    return 0;
}

void dHash_InitHashTable() {
    int i;

    for (i = 0; i < 30; i++) {
        hashTable[i].Name[0] = 0;
        hashTable[i].arg = 0;
    }

    hashTable[30].Name[0] = 0;
    hashTable[30].arg = 0;
}

int dHash_SearchHashW(const u16* i_Name) {
    int len;
    u32 n;
    u32 hash;
    int i;
    int firstHash;
    int len2;
    int slotLen;
    int k;
    int hashval;
    const u16* str0_p;
    const u16* str1_p;
    int success;

    for (len = 0; i_Name[len] != 0; len++) {
    }

    if (len < 8) {
        hash = 0;
        n = 0;
        for (i = 0; i < len; i++) {
            if (n > 7) {
                n = 0;
            }
            hash += i_Name[i] << (n * 4);
            n++;
        }
        firstHash = hash % 31;
    } else {
        firstHash = -1;
    }

    if (firstHash != -1) {
        for (len2 = 0; i_Name[len2] != 0; len2++) {
        }

        for (k = 0; k < 15; k++) {
            hashval = (firstHash + k * k) % 31;
            str1_p = hashTable[hashval].Name;

            for (slotLen = 0; str1_p[slotLen] != 0; slotLen++) {
            }

            if (len2 == slotLen) {
                str0_p = i_Name;
                success = 1;
                for (i = 0; i < len2; i++) {
                    if (str0_p[i] != str1_p[i]) {
                        success = 0;
                        break;
                    }
                }
                if (success) {
                    return hashval;
                }
            }
        }
    }

    return -1;
}

int dHash_GetArg(const char* i_Name) {
    int searchHash;

    if (_MakeWStr(i_Name)) {
        searchHash = dHash_SearchHashW(l_tmpWName);
        if (searchHash == -1) {
            return -1;
        } else {
            return hashTable[searchHash].arg;
        }
    }

    return -1;
}

int dHash_SetArg(const char* i_Name, u8 i_Arg) {
    int len;
    u32 n;
    u32 hash;
    int i;
    int firstHash;
    int newHash;
    int k;
    int hashval;
    VF_HashTableEntry* entry;

    if (_MakeWStr(i_Name)) {
        for (len = 0; l_tmpWName[len] != 0; len++) {
        }

        if (len < 8) {
            hash = 0;
            n = 0;
            for (i = 0; i < len; i++) {
                if (n > 7) {
                    n = 0;
                }
                hash += l_tmpWName[i] << (n * 4);
                n++;
            }
            firstHash = hash % 31;
        } else {
            firstHash = -1;
        }

        if (firstHash != -1) {
            if (hashTable[firstHash].Name[0] != 0) {
                newHash = -1;
                for (k = 1; k <= 7; k++) {
                    hashval = (firstHash + k * k) % 31;
                    if (hashTable[hashval].Name[0] == 0) {
                        newHash = hashval;
                        break;
                    }
                }
            } else {
                newHash = firstHash;
            }
        } else {
            newHash = -1;
        }

        if (newHash != -1) {
            entry = &hashTable[newHash];
            VFipf_memcpy(entry->Name, l_tmpWName, 16);
            entry->arg = i_Arg;
            return 1;
        }
    }

    return 0;
}

static void _DeleteDataByIdx(int i_Idx) {
    if (i_Idx < 0x1F) {
        VF_HashTableEntry* entry = &hashTable[i_Idx];
        entry->Name[0] = 0;
        entry->arg = 0;
    }
}

static int dHash_DeleteDataW(const u16* i_Name) {
    long Hash = dHash_SearchHashW(i_Name);
    if (Hash != -1) {
        if (Hash >= 0) {
            _DeleteDataByIdx(Hash);
        }

        return 1;
    }

    return 0;
}

int dHash_DeleteData(const char* i_Name) {
    if (_MakeWStr(i_Name)) {
        return dHash_DeleteDataW(_GetWStr());
    }

    return 0;
}
