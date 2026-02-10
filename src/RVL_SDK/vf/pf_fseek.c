#include "revolution/vf/pf_api_util.h"
#include "revolution/vf/pf_file.h"
#include "revolution/vf/vf_struct.h"

int VFipf2_fseek(PF_FILE* p_file, int lOffset, int nOrigin) {
    return VFiPFAPI_convertReturnValue(VFiPFFILE_fseek(p_file, lOffset, nOrigin));
}
