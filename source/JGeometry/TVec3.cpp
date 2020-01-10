#include "JGeometry/TVec3.h"

namespace JGeometry
{   
    template<typename T>
    TVec3<T>::TVec3(T val)
    {
        x = val;
        y = val;
        z = val;
    }

    /*template<typename T>
    void TVec3<T>::set(T inX, T inY, T inZ)
    {
        x = inX;
        y = inY;
        z = inZ;
    }*/
};