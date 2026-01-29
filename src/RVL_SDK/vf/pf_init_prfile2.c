#include "revolution/vf/pf_api_util.h"
#include "revolution/vf/pf_fatfs.h"
#include "revolution/vf/pf_system.h"

s32 VFipf2_init_prfile2(long config, void* param) {
    s32 err = VFiPFFATFS_initializeFATFS(config, param); 
    if (err == 0) {
        VFiPFSYS_initializeSYS();
    }
    return VFiPFAPI_convertReturnValue(err);
}
