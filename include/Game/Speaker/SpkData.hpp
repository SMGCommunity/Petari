#pragma once

#include "Game/Speaker/SpkTable.hpp"
#include "Game/Speaker/SpkWave.hpp"
#include <JSystem/JKernel/JKRArchive.hpp>

class SpkData {
public:
    SpkData(JKRArchive*);

    void loadTable(u16);
    void loadWave(u16);
    bool isValid() const;

    SpkTable* getSpkTable() {
        return &mTable;
    }

    SpkWave* getWave() {
        return &mWave;
    }

    /* 0x00 */ SpkTable mTable;
    /* 0x10 */ SpkWave mWave;
    /* 0x20 */ JKRArchive* mArchive;
};
