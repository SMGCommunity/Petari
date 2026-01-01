#include "revolution/vf/pf_api_util.h"

int VFipf_error_to_api_error[40];

int VFiPFAPI_convertError(int err) {
    if (err != 0 && err != -1) {
        if (err > 0 && err < 160) {
            return VFipf_error_to_api_error[err];
        } else {
            if (err == 4096) {
                return 5;
            }
        }
    }

    return err;
}

int VFiPFAPI_convertReturnValue(int err) {
    return (-err | err) >> 31;
}

void* VFiPFAPI_convertReturnValue2NULL(int err, void* p_stream) {
    if (err) {
        return NULL;
    } else {
        return p_stream;
    }
}

int VFiPFAPI_convertReturnValue4unmount(int err) {
    if (err == 0) {
        return 0;
    }

    if (err == 1) {
        return 1;
    }

    return -1;
}
