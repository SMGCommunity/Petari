#pragma once

#include <revolution.h>

class NPCActorItem {
public:
    inline NPCActorItem() {
    }

    // cannot be noinline because of NPCItemParameterReader constructor
    NPCActorItem(const char* pName) {
        mActor = pName;
        mGoods0 = "";
        mGoodsJoint0 = "";
        mGoods1 = "";
        mGoodsJoint1 = "";
    }

    /* 0x00 */ const char* mActor;
    /* 0x04 */ const char* mGoods0;
    /* 0x08 */ const char* mGoodsJoint0;
    /* 0x0C */ const char* mGoods1;
    /* 0x10 */ const char* mGoodsJoint1;
};
