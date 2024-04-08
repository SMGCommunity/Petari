#include "size_t.h"
#include "TRK_Types.h"

static void TRK_fill_mem(u8 *dst, s32 val, u32 length) {
    u32    v = (u8)val;
    u32    i;

    union
    {
        u8 *cpd;
        u32 *lpd;
    } dstu;

    dstu.cpd = (((u8 *)dst) - 1);

    if (length >= 32)
    {
        i = ((~ (u32)dstu.cpd) & 3);

        if (i)
        {
            length -= i;

            do
                *++(dstu.cpd) = (u8)v;
            while (--i);
        }

        if (v)
            v |= ((v << 24) | (v << 16) | (v <<  8));

        dstu.lpd = (((u32 *)(dstu.cpd + 1)) - 1);

        i = (length >> 5);

        if (i)
        {
            do
            {
                *++(dstu.lpd) = v;
                *++(dstu.lpd) = v;
                *++(dstu.lpd) = v;
                *++(dstu.lpd) = v;
                *++(dstu.lpd) = v;
                *++(dstu.lpd) = v;
                *++(dstu.lpd) = v;
                *++(dstu.lpd) = v;
            } while (--i);
        }

        i = ((length & 31) >> 2);

        if (i)
        {
            do
                *++(dstu.lpd) = v;
            while (--i);
        }

        dstu.cpd = (((u8 *)(dstu.lpd + 1)) - 1);

        length &= 3;
    }

    if (length)
    {
        do
            *++(dstu.cpd) = (u8)v;
        while (--length);
    }
}