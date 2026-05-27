# main/RVL_SDK/vf/pf_cache

**Source Path:** `src/RVL_SDK/vf/pf_cache.c`

## Functions

| Name | Address | Match % |
|------|---------|---------|
| `VFiPFCACHE_InitPageList` | `0x80471818` | :x: (88.8%) |
| `VFiPFCACHE_FlushPageIfNeeded` | `0x80471A98` | :x: (72.5%) |
| `VFiPFCACHE_DoAllocatePage` | `0x80471B98` | :x: (88.6%) |
| `VFiPFCACHE_DoReadPage` | `0x80471EE0` | :white_check_mark: (100.0%) |
| `VFiPFCACHE_DoReadPageAndFlushIfNeeded` | `0x804720E4` | :x: (85.0%) |
| `VFiPFCACHE_DoReadNumSector` | `0x80472378` | :x: (61.4%) |
| `VFiPFCACHE_DoWritePage` | `0x804725CC` | :x: (97.3%) |
| `VFiPFCACHE_DoWriteNumSectorAndFreeIfNeeded` | `0x80472700` | :x: (59.4%) |
| `VFiPFCACHE_DoFlushCache` | `0x80472B3C` | :x: (89.8%) |
| `VFiPFCACHE_SetCache` | `0x80472C3C` | :white_check_mark: (100.0%) |
| `VFiPFCACHE_SetFATBufferSize` | `0x80472C50` | :white_check_mark: (100.0%) |
| `VFiPFCACHE_SetDataBufferSize` | `0x80472C60` | :white_check_mark: (100.0%) |
| `VFiPFCACHE_InitCaches` | `0x80472C70` | :x: (92.2%) |
| `VFiPFCACHE_UpdateModifiedSector` | `0x80472D38` | :white_check_mark: (100.0%) |
| `VFiPFCACHE_AllocateDataPage` | `0x80472DA0` | :white_check_mark: (100.0%) |
| `VFiPFCACHE_FreeDataPage` | `0x80472E14` | :white_check_mark: (100.0%) |
| `VFiPFCACHE_ReadFATPage` | `0x80472EA4` | :white_check_mark: (100.0%) |
| `VFiPFCACHE_ReadDataPage` | `0x80472EE4` | :white_check_mark: (100.0%) |
| `VFiPFCACHE_ReadDataPageAndFlushIfNeeded` | `0x80472F2C` | :white_check_mark: (100.0%) |
| `VFiPFCACHE_ReadDataNumSector` | `0x80472F74` | :white_check_mark: (100.0%) |
| `VFiPFCACHE_WriteFATPage` | `0x80472FD4` | :white_check_mark: (100.0%) |
| `VFiPFCACHE_WriteDataPage` | `0x80473010` | :white_check_mark: (100.0%) |
| `VFiPFCACHE_WriteDataNumSectorAndFreeIfNeeded` | `0x8047304C` | :white_check_mark: (100.0%) |
| `VFiPFCACHE_SearchDataCache` | `0x804730AC` | :white_check_mark: (100.0%) |
| `VFiPFCACHE_FlushFATCache` | `0x80473130` | :white_check_mark: (100.0%) |
| `VFiPFCACHE_FlushDataCacheSpecific` | `0x80473164` | :x: (86.9%) |
| `VFiPFCACHE_FlushAllCaches` | `0x80473288` | :white_check_mark: (100.0%) |
| `VFiPFCACHE_FreeAllCaches` | `0x804732E0` | :white_check_mark: (100.0%) |