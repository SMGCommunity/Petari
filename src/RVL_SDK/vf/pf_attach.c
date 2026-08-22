#include "revolution/vf/pf_attach.h"
#include "revolution/vf/pf_api_util.h"
#include "revolution/vf/pf_volume.h"
#include "revolution/vf/vf_struct.h"

int VFipf2_attach(PF_DRV_TBL** drv_tbl) {
    int v2;
    PF_DRV_TBL* tbl;

    if (drv_tbl == 0 || !*drv_tbl) {
        v2 = 10;
        VFipf_vol_set.last_error = v2;
    } else {
        v2 = 0;
        while (*drv_tbl) {
            tbl = *drv_tbl;
            if (!v2) {
                v2 = VFiPFVOL_attach(tbl);
            } else {
                tbl->drive = 0;
            }

            drv_tbl++;
        }
    }

    return VFiPFAPI_convertReturnValue(v2);
}
