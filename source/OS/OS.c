#include "OS/OS.h"
#include "OS/OSError.h"

void OSRegisterVersion(const char *versionStr)
{
    OSReport("%s\n", versionStr);
}