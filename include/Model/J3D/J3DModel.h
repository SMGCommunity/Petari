#ifndef J3DMODELDATA_H
#define J3DMODELDATA_H

#include "types.h"

class J3DModelData
{
public:
    J3DModelData();

    virtual ~J3DModelData();

    u16 _7C;
    u16 _7E;
    u32 _80;
    u32 _84;
};

#endif // J3DMODELDATA_H