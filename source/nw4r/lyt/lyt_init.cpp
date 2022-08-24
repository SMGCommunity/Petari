#include "nw4r/lyt/lyt_init.h"
#include <revolution/os.h>

namespace {
    static const char* NW4R_LYT_Version_ = "<< NW4R    - LYT \tfinal   build: Jul 17 2007 12:25:23 (0x4199_60831) >>";
};

namespace nw4r {
    namespace lyt {
        void LytInit() {
            OSRegisterVersion(NW4R_LYT_Version_);

            __asm {
                li r3, 4
                oris r3, r3, 4
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
    };
};