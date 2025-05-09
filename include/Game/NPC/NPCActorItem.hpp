#pragma once

#include <revolution.h>

class NPCActorItem {
public:
    inline NPCActorItem() {
        
    }

    NPCActorItem(const char *pName) {
        mActor = pName;
        _4 = "";
        _8 = "";
        mArchive = "";
        _10 = "";
    }

    const char* mActor;             // 0x00
    const char* _4;
    const char* _8;
    const char* mArchive;           // 0x0C
    const char* _10;
};
