#include "nw4r/lyt/init.h"
#include <revolution/os.h>
#include <revolution/os/OSFastCast.h>

namespace {
    #if (VERSION == RMGJ01 || VERSION == RMGE01 || VERSION == RMGP01)
    const char* NW4R_LYT_Version_ = "<< NW4R    - LYT \tfinal   build: Jul 31 2007 17:22:11 (0x4199_60831) >>";
#elif (VERSION == RMGK01)
    const char* NW4R_LYT_Version_ = "<< NW4R    - LYT \tfinal   build: Jul 17 2007 12:25:23 (0x4199_60831) >>";
#endif
};  // namespace

namespace nw4r {
    namespace lyt {
        void LytInit() {
            OSRegisterVersion(NW4R_LYT_Version_);
            OSInitFastCast();
        }
    };  // namespace lyt
};  // namespace nw4r
