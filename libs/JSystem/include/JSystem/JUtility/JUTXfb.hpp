#pragma once

class JUTXfb {
public:
    void* getDrawingXfb() const;

    void* _0;
    void* _4;
    void* _8;
    u8 _C;
    u8 _D;
    u8 _E;
    s32 _10;
    s16 _14;
    s16 _16;
    s16 _18;
    s32 _1C;

    static JUTXfb* sManager;
};
