#include "Game/System/FileRipper.hpp"

s32 FileRipper::checkCompressed(const u8 *pData)
{
    if (pData[0] == 'Y' && pData[1] == 'a' && pData[3] == '0') {
        if (pData[2] == 'y') {
            return 1;
        }

        if (pData[2] == 'z') {
            return 2;
        }
    }

    return 0;
}
