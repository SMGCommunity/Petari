#include "nw4r/lyt/init.h"
#include "revolution/os.h"
#include "revolution/os/OSFastCast.h"

namespace {
    const char* NW4R_LYT_Version_ = "<< NW4R    - LYT \tfinal   build: Jul 17 2007 12:25:23 (0x4199_60831) >>";
};

namespace nw4r {
    namespace lyt {
        void LytInit() {
            OSRegisterVersion(NW4R_LYT_Version_);
            OSInitFastCast();
        }
    };
};
