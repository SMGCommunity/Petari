#include "revolution/vf/pf_errnum.h"
#include "revolution/vf/pf_api_util.h"
#include "revolution/vf/pf_volume.h"

int VFipf2_errnum(void) {
    return VFiPFAPI_convertError(VFiPFVOL_errnum());
}
