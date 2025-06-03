#pragma once

class JUTXfb {
public:
    void* getDrawingXfb() const {
        if (_14 >= 0) {
            return _0[_14];
        }
        return nullptr;
    }

    static JUTXfb* getManager() {
        return sManager;
    }

    void* _0[3];
    u8 _C;
    u8 _D;
    u8 _E;
    s32 _10;
    s16 _14;
    s16 _16;
    s16 _18;
    s32 _1C;


private:
    static JUTXfb* sManager;
};
