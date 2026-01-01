#include "revolution/vf/d_hash.h"

typedef struct {
    unsigned short Name[8];
    unsigned char arg;
} VF_HashTableEntry;

static u16 l_tmpWName[8];
static VF_HashTableEntry hashTable[31];

u16* _GetWStr() {
    return l_tmpWName;
}

// _MakeWStr

void dHash_InitHashTable() {
    int i;

    for (i = 0; i < 30; i++) {
        hashTable[i].Name[0] = 0;
        hashTable[i].arg = 0;
    }

    hashTable[30].Name[0] = 0;
    hashTable[30].arg = 0;
}

// dHash_SearchHashW

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

// dHash_SetArg

static int _DeleteDataByIdx(int i_Idx) {
    if (i_Idx < 0x1F) {
        VF_HashTableEntry* entry = &hashTable[i_Idx];
        entry->Name[0] = 0;
        entry->arg = 0;
    }
}

int dHash_DeleteDataW(const u16* i_Name) {
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
