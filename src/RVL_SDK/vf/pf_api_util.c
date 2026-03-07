#include "revolution/vf/pf_api_util.h"

int VFipf_error_to_api_error[40];

#include <revolution/types.h>

u32 VFiPFAPI_ParseOpenModeString(const char* mode_str) {
    u32 open_mode_util;
    u32 i;

    if (mode_str == 0) {
        return 10;
    }

    i = 1;
    switch (mode_str[0]) {
    case 'r':
        open_mode_util = 2;
        break;
    case 'w':
        open_mode_util = 1;
        break;
    case 'a':
        open_mode_util = 4;
        break;
    default:
        return 0;
    }

    if (mode_str[1] == 'b') {
        i = 2;
    }

    switch (mode_str[i++]) {
    case 0:
        return open_mode_util;
    case 't':
    default:
        return 0;
    case '+':
        if (mode_str[i] == 0) {
            return open_mode_util | 8;
        }
        return 0;
    }
}

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
