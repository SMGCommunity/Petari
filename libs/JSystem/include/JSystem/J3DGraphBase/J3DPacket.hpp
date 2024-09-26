#pragma once

#include <revolution.h>

class J3DDrawBuffer;

class J3DPacket {
public:
    inline J3DPacket() {

    }

    virtual u32 entry(J3DDrawBuffer *);

    void addChildPacket(J3DPacket *);

    J3DPacket* mNext;   // 0x4
    J3DPacket* mFirst;  // 0x8
};

class J3DDisplayListObj {
public:
    J3DDisplayListObj();

    void newDisplayList(u32);
    void newSingleDisplayList(u32);
    bool single_to_Double();
    void setSingleDisplayList(void *, u32);
    void swapBuffer();
    void callDL() const;
    void beginDL();
    void endDL();
    void beginPatch();
    void endPatch();

    u32 _0;
    u32 _4;
    u32 mSize;      // 0x8
    u32 mMaxSize;   // 0xC
};
