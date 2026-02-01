#include "revolution/vf/pf_fclose.h"
#include "revolution/vf/pf_api_util.h"
#include "revolution/vf/pf_file.h"

s32 VFipf2_fclose(PF_FILE* p_file) {
    return VFiPFAPI_convertReturnValue(VFiPFFILE_fclose(p_file));
}
