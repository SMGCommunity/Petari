#pragma once

#include <revolution/types.h>

struct J2DataManageLink {
    /* 0x00 */ void* mData;
    /* 0x04 */ char* mName;
    /* 0x08 */ J2DataManageLink* mNext;
};

class J2DDataManage {
public:
    /* 0x00 */ J2DataManageLink* mList;

public:
    void* get(char const*);
};
