#pragma once

#include "revolution/gx/GXStruct.h"

namespace nw4r
{
    namespace ut
    {
        // I am pretty sure this inherits
        class Color : public GXColor
        {
        public:
            inline Color(u32 color)
            {
                operator =(color);
            }

            inline Color(int red, int green, int blue, int alpha)
            {
                r = static_cast<u8>(red);
                g = static_cast<u8>(green);
                b = static_cast<u8>(blue);
                a = static_cast<u8>(alpha);
            }

            inline ~Color()
            {

            }

            inline Color& operator =(u32 color)
            {
                toRef() = color;
                return *this;
            }

            u32& toRef()
            {
                return *reinterpret_cast<u32*>(this);
            }
        };
    };
};