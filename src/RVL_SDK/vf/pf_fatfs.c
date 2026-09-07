#include "revolution/vf/pf_fatfs.h"
#include "revolution/vf/pf_volume.h"

int VFiPFFATFS_initializeFATFS(u32 config, void* param) {
    return VFiPFVOL_InitModule(config, param);
}
