#pragma once


// temp
class BgmMgr {
public:
    u8 _0[0x10];
    u32 _10;
};

class AudWrap {
public:
    static BgmMgr* getBgmMgr();

    static void setNextIdStageBgm(u32);
};