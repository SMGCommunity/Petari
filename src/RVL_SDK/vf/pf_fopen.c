#include "revolution/vf/pf_str.h"
#include "revolution/vf/pf_file.h"
#include "revolution/vf/pf_api_util.h"
#include "revolution/vf/vf_struct.h"

PF_FILE * VFipf2_fopen(const char *path, const char *mode) {
    u32 open_mode_fopen;
    PF_FILE *p_ofile;
    PF_STR path_str;
    s32 err;

    open_mode_fopen = VFiPFAPI_ParseOpenModeString(mode);

    if (open_mode_fopen == 0) {
        VFipf_vol_set.last_error = 10;
        p_ofile = NULL;
    } 
    else {
        err = VFiPFSTR_InitStr(&path_str, path, 1);
        
        if (err == 0) {
            err = VFiPFFILE_fopen(&path_str, open_mode_fopen, &p_ofile);
        } 
        else {
            p_ofile = NULL;
            VFipf_vol_set.last_error = err;
        }
        
        p_ofile = VFiPFAPI_convertReturnValue2NULL(err, p_ofile);
    }

    return p_ofile;
}
