#ifndef SUPPORT_H
#define SUPPORT_H

#include "size_t.h"
#include "portable/dserror.h"
#include "portable/msgcmd.h"

DSError HandleOpenFileSupportRequest(DSFileHandle handle, u8 length, u32 *options, DSIOResult *io_result);

#endif // SUPPORT_H