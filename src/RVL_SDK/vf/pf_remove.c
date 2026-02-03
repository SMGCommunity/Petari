#include "revolution/vf/pf_str.h"
#include "revolution/vf/pf_file.h"
#include "revolution/vf/vf_struct.h"
#include "revolution/vf/pf_api_util.h"

int VFipf2_remove(const char *path) {
    long err;
    PF_STR path_str;

    err = VFiPFSTR_InitStr(&path_str, (const s8*)path, 1);
    if (err == 0) {
        err = VFiPFFILE_remove(&path_str);
    } else {
        VFipf_vol_set.last_error = err; 
    }

    return VFiPFAPI_convertReturnValue(err);
}
