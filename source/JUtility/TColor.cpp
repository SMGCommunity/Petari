#include "JUtility/TColor.h"

namespace JUtility
{
    TColor::TColor(u8 r, u8 g, u8 b, u8 a)
        : red(r), green(g), blue(b), alpha(a)
    { }

    void TColor::operator=(const TColor &in)
    {
        red = in.red;
        green = in.green;
        blue = in.blue;
        alpha = in.alpha;
    }
};