#ifndef SUPPORT_H
#define SUPPORT_H

#include "size_t.h"
#include "portable/dserror.h"
#include "portable/msgcmd.h"

DSError HandleOpenFileSupportRequest(DSFileHandle handle, u8 length, u32 *options, DSIOResult *io_result);
DSError HandlePositionFileSupportRequest(DSFileHandle handle, int *options, u8 length, DSIOResult *io_result);
DSError HandleCloseFileSupportRequest(DSFileHandle handle, DSIOResult *io_result);



#endif // SUPPORT_H