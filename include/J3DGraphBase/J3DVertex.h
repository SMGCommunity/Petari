#ifndef J3DVERTEX_H
#define J3DVERTEX_H

#include <revolution.h>

class J3DVertexData
{
public:
    J3DVertexData();

    u8 _0[0x5C];
};

class J3DVertexBuffer
{
public:
    ~J3DVertexBuffer();

    void setVertexData(J3DVertexData *);
    void init();
    void setArray() const;
    void frameInit();

    u8 _0[0x38];
};

#endif // J3DVERTEX_H