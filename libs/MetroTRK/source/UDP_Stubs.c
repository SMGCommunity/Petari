#include "TRK_Types.h"

typedef void (*DBCallback)(s32, void *);

s32 udp_cc_post_stop(void)
{
    return -1;
}

s32 udp_cc_pre_continue(void)
{
    return -1;
}

s32 udp_cc_peek(void)
{
    return 0;
}

s32 udp_cc_write(u8 *buffer, s32 length) {
    return 0;
}

s32 udp_cc_read(u8 *buffer, s32 length) {
    return 0;   
}

s32 udp_cc_close(void) {
    return -1;
}

s32 udp_cc_open(void) {
    return -1;
}

s32 udp_cc_shutdown(void) {
    return -1;
}

s32 udp_cc_initialize(volatile u8 **inputPendingPtrRef, DBCallback cb) {
    return -1;
}