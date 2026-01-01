#include "revolution/vf/pf_detach.h"
#include "revolution/vf/pf_api_util.h"
#include "revolution/vf/pf_volume.h"

int VFipf2_detach(s8 drive) {
    return VFiPFAPI_convertReturnValue(VFiPFVOL_detach(drive));
}
