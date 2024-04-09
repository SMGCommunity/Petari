#include <size_t.h>
#include "portable/mslsupp.h"
#include "TRK_Types.h"

inline IOError __access_file(u32 handle, u8 *buffer,
                          size_t *count, __ref_con ref_con, u8 read);

IOError __read_file(u32 handle, u8 *buffer, size_t *count, __ref_con ref_con);

/* it will not inline __write_file but it matches otherwise */
IOError __TRK_write_console(u32 handle, u8 *buffer, size_t *count, __ref_con ref_con) {
    if ((GetUseSerialIO() & 0xFF) == 0) {
        return 1;
    }

    return __write_file(1, buffer, count, ref_con);
}

IOError __read_file(u32 handle, u8 *buffer, size_t *count, __ref_con ref_con) {
    return __access_file(handle, buffer, count, ref_con, 0xD1);
}

inline IOError __write_file(u32 handle, u8 *buffer, size_t *count, __ref_con ref_con) {
    return __access_file(handle, buffer, count, ref_con, 0xD0);
}

/* these ar*/
inline IOError __access_file(u32 handle, u8 *buffer, size_t *count, __ref_con ref_con, u8 read) {
    u8 err;
    u32 local_count;

    if (GetTRKConnected() == 0) {
        return 1;
    }

    local_count = (u32)*count;
    err = TRKAccessFile((u8)(read), (u32)handle, &local_count, (u8 *)buffer);
    *count = (size_t)local_count;

    switch (err) {
        case 0:
            err = 0;
            break;

        case 2:
            err = 2;
            break;

        default:
            err = 1;
            break;
    }

    return err;
}

/* there are odd issues going on with __read_file but I don't care enough to figure out what. it matches otherwise */
IOError __read_console(u32 handle, u8 *buffer, size_t *count, __ref_con ref_con) {
    if ((GetUseSerialIO() & 0xFF) == 0) {
        return 1;
    }

    return __read_file(0, buffer, count, ref_con);
}