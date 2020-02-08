#ifndef J3DMATERIALATTACH_H
#define J3DMATERIALATTACH_H

#include <revolution.h>

class J3DMaterialTable
{
public:
    J3DMaterialTable();

    virtual ~J3DMaterialTable();

    u32 _4;
    u32** _8; // J3DMaterial**
    u32 _C;
};

#endif // J3DMATERIALATTACH_H