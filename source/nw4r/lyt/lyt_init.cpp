#include "nw4r/lyt/lyt_init.h"
#include "revolution/OS.h"

namespace nw4r
{
    namespace lyt
    {
        void LytInit()
        {
            OSRegisterVersion(NW4R_LYT_Version);

            __asm
            {
                li r3, 4
                ori r3, r3, 4
                mtspr 0x392, r3
                li r3, 5
                oris r3, r3, 5
                mtspr 0x393, r3
                li r3, 6
                oris r3, r3, 6
                mtspr 0x394, r3
                li r3, 7
                oris r3, r3, 7
                mtspr 0x395, r3
            };
        }
    }
};