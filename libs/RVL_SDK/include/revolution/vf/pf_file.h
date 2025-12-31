#ifndef PF_FILE_H
#define PF_FILE_H

#include "revolution/types.h"
#include "revolution/vf/vf_struct.h"

int VFiPFFILE_fclose(PF_FILE*);
int VFiPFFILE_fread(u8*, u32, u32, PF_FILE*, u32*);
int VFiPFFILE_fwrite(u8*, u32, u32, PF_FILE*, u32*);
int VFiPFFILE_fseek(PF_FILE*, int, int);
int VFiPFFILE_finfo(PF_FILE*, PF_INFO*);

#endif  // PF_FILE_H
