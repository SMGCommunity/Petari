#ifndef VECTOR_H
#define VECTOR_H

namespace MR
{
    template<typename T>
    class Vector
    {
    public:
        Vector();
        ~Vector();

        void push_back(const T &);

        u32 _0;
        u32 _4;
        u32 _8;
    };
};

#endif // VECTOR_H
