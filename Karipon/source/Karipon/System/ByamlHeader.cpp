#include "Karipon/System/ByamlHeader.hpp"
#include <cstring>

s32 ByamlStringTableIter::findStringIndex(const char* pStr) const {
    s32 lowerBound = 0;
    s32 upperBound = getSize();

    while (lowerBound < upperBound) {
        s32 avg = (lowerBound + upperBound) / 2;
        s32 result = strcmp(pStr, getString(avg));
        if (result == 0) {
            return avg;
        }

        if (result > 0) {
            lowerBound = avg + 1;
        }
        else {
            upperBound = avg;
        }
    }

    return -1;
}
