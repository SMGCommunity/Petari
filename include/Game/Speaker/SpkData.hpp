#pragma once

#include "JSystem/JKernel/JKRArchive.h"
#include "Game/Speaker/SpkTable.h"
#include "Game/Speaker/SpkWave.h"
#include "revolution.h"

// This is something JKernel related, but idk what it is for the time being.
extern void* FUN_8040f918(JKRArchive * pArchive, u16 id);

class SpkData {
public:
    SpkData(JKRArchive *);

    void loadTable(u16);
    void loadWave(u16);
    bool isValid() const;

    SpkTable mTable;        // _0
    SpkWave mWave;          // _4
    JKRArchive* mArchive;   // _14
};