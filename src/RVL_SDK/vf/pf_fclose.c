#include "revolution/vf/pf_api_util.h"
#include "revolution/vf/pf_file.h"

int VFipf2_errnum(PF_FILE* p_file) {
    return VFiPFAPI_convertReturnValue(VFiPFFILE_fclose(p_file));
}
