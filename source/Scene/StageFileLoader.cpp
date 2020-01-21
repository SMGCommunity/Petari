#include "Scene/StageFileLoader.h"
#include "System/Galaxy/GalaxyStatusAccessor.h"
#include "arch/printf.h"

void StageFileLoader::makeStageArchiveName(char *pOut, u32 len, const char *pStageName)
{
    snprintf(pOut, len, "/StageData/%s.arc", pStageName);
}