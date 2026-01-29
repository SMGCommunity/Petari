#include "revolution/vf/pf_api_util.h"
#include "revolution/vf/pf_file.h"

int VFipf2_finfo(PF_FILE* p_file, PF_INFO* p_info) {
    return VFiPFAPI_convertReturnValue(VFiPFFILE_finfo(p_file, p_info));
}
