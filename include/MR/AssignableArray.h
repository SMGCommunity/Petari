#ifndef ASSIGNABLEARRAY_H
#define ASSIGNABLEARRAY_H

namespace MR
{
    template<typename T>
    class AssignableArray
    {
    public:
        void* mContent; // _0
        s32 mCount; // _4
    };
};

#endif // ASSIGNABLEARRAY_H