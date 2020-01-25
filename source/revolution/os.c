#include "revolution/os.h"
#include "revolution/os/OSError.h"

void OSRegisterVersion(const char *versionStr)
{
    OSReport("%s\n", versionStr);
}