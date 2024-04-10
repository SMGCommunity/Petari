#include "TRK_Types.h"
#include "portable/msgbuf.h"
#include "portable/msgcmd.h"
#include "portable/dserror.h"
#include "portable/targimpl_ppc.h"
#include "portable/usr_put.h"

static s32 IsTRKConnected = 0;

DSError TRKDoSetOption(MessageBuffer *b) {
    u8 msg_reply = DSFetch_u8(&b->fData[12]);

    if (b->fData[8] == 1) {
        usr_puts_serial("\nMetroTRK Option : SerialIO - ");
        if (msg_reply != 0) {
            usr_puts_serial("Enable\n");
        }
        else {
            usr_puts_serial("Disable\n");
        }

        SetUseSerialIO(msg_reply);
    }


}

s32 GetTRKConnected(void) {
    return IsTRKConnected;
}