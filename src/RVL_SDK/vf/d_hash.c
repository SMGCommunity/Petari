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

    for (len = 0; *i_Name != 0; len++) {
        i_Name++;
    }
    return len;
}

static int _MakeWStr(const char* i_Name) {
    u32 len;
    int Id;
    int next2nd;

    if (_StrLen(i_Name) < 8) {
        len = _StrLen(i_Name);
        for (Id = 0, next2nd = 0; Id < len; Id++) {
            if (next2nd) {
                next2nd = 0;
            } else {
                if (VFipf_vol_set.codeset.is_oem_mb_char(i_Name[Id], 1) != 0) {
                    next2nd = 1;
                    l_tmpWName[Id] = i_Name[Id + 1];
                    l_tmpWName[Id] |= i_Name[Id] << 8;
                } else {
                    l_tmpWName[Id] = i_Name[Id];
                }
            }
        }
        l_tmpWName[7] = 0;
        l_tmpWName[Id] = 0;
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

static u32 _StrLenW(const u16* i_Name) {
    u32 len;

    for (len = 0; *i_Name != 0; len++) {
        i_Name++;
    }
    return len;
}

static s32 dHash_CalcFirstHashW(const u16* i_Name) {
    int len = _StrLenW(i_Name);
    u32 n;
    u32 hash;
    u32 weight;

    if (len < 8) {
        for (n = weight = hash = 0; n < len; n++, weight++) {
            if (weight > 7) {
                weight = 0;
            }
            hash += i_Name[n] << (weight * 4);
        }
        return hash % 31;
    }
    return -1;
}

static s32 dHash_CalcRehash(u32 i_FirstHash) {
    u32 hashval;
    u32 k;

    for (k = 1; k < 15; k++) {
        hashval = (i_FirstHash + k * k) % 31;
        if (hashTable[hashval].Name[0] == 0) {
            return hashval;
        }
    }
    return -1;
}

int dHash_SearchHashW(const u16* i_Name) {
    s32 firstHash = dHash_CalcFirstHashW(i_Name);
    int len2;
    int slotLen;
    int i;
    int hashval;
    int k;
    const u16* str0_p;
    const u16* str1_p;
    int success;

    if (firstHash != -1) {
        len2 = _StrLenW(i_Name);

        for (k = 0; k < 15; k++) {
            hashval = (firstHash + k * k) % 31;
            str1_p = hashTable[hashval].Name;

            slotLen = _StrLenW(str1_p);

            if (len2 == slotLen) {
                str0_p = i_Name;
                success = 1;
                for (i = 0; i < len2; i++, str0_p++, str1_p++) {
                    if (*str0_p != *str1_p) {
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

static s32 dHash_GetNewHashW(const u16* i_Name) {
    s32 firstHash = dHash_CalcFirstHashW(i_Name);
    s32 newHash;

    if (firstHash != -1) {
        if (hashTable[firstHash].Name[0] != 0) {
            newHash = dHash_CalcRehash(firstHash);
        } else {
            newHash = firstHash;
        }
    } else {
        newHash = -1;
    }
    return newHash;
}

static int dHash_SetArgW(const u16* i_Name, u8 i_Arg) {
    s32 newHash = dHash_GetNewHashW(i_Name);

    if (newHash != -1) {
        VFipf_memcpy(hashTable[newHash].Name, i_Name, 16);
        hashTable[newHash].arg = i_Arg;
        return 1;
    }
    return 0;
}

int dHash_SetArg(const char* i_Name, u8 i_Arg) {
    if (_MakeWStr(i_Name)) {
        return dHash_SetArgW(_GetWStr(), i_Arg);
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
