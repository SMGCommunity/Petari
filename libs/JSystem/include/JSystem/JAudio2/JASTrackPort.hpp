#pragma once

#include <revolution/types.h>

class JASTrackPort {
public:
    static const int MAX_PORTS = 16;

    void init();
    u16 readImport(u32);
    u16 readExport(u32);
    void writeImport(u32, u16);
    void writeExport(u32, u16);
    bool checkImport(u32) const;
    bool checkExport(u32) const;

    u16 get(u32 idx) const {
        return mPorts[idx];
    }
    void set(u32 idx, u16 val) {
        mPorts[idx] = val;
    }

    u16 mImport;
    u16 mExport;
    u16 mPorts[MAX_PORTS];
};
