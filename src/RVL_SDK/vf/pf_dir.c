#include "revolution/vf/pf_dir.h"
#include "revolution/vf/pf_fat.h"

static void VFiPFDIR_FinalizeUDD(PF_DIR* p_dir) {
    p_dir->stat &= ~1u;
}

static void VFiPFDIR_FinalizeSDD(PF_SDD* p_sdd) {
    p_sdd->stat &= 0xFFFFFFFC;
    VFiPFFAT_FinalizeFFD(&p_sdd->ffd);
}

void VFiPFDIR_FinalizeAllDirs(PF_VOLUME* p_vol) {
    u16 i;
    u16 j;

    for (i = 0; i < 3u; ++i) {
        VFiPFDIR_FinalizeSDD(&p_vol->sdds[i]);
    }

    for (j = 0; j < 3u; ++j) {
        VFiPFDIR_FinalizeUDD(&p_vol->udds[j]);
    }

    p_vol->num_opened_directories = 0;
}
