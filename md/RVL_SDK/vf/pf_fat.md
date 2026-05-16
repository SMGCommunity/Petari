# main/RVL_SDK/vf/pf_fat

**Source Path:** `src/RVL_SDK/vf/pf_fat.c`

## Functions

| Name | Address | Match % |
|------|---------|---------|
| `VFiPFFAT_ReadFATSector` | `0x80477460` | :x: (67.5%) |
| `VFiPFFAT_SearchForNumFreeClusters` | `0x80477590` | :x: (66.1%) |
| `VFiPFFAT_FindClusterLink` | `0x80477794` | :x: (73.2%) |
| `VFiPFFAT_FindClusterLinkPage` | `0x80477950` | :x: (71.2%) |
| `VFiPFFAT_ReadClusterPage` | `0x80477AFC` | :x: (76.6%) |
| `VFiPFFAT_WriteCluster` | `0x80477CC8` | :x: (64.8%) |
| `VFiPFFAT_WriteClusterPage` | `0x80477EEC` | :x: (64.7%) |
| `VFiPFFAT_DoAllocateChain` | `0x80478114` | :x: (91.0%) |
| `VFiPFFAT_GetClusterInChain` | `0x804783C0` | :x: (86.1%) |
| `VFiPFFAT_GetClusterContinuousSectorInChain` | `0x80478728` | :white_check_mark: (100.0%) |
| `VFiPFFAT_GetClusterAllocatedInChain` | `0x80478858` | :white_check_mark: (100.0%) |
| `VFiPFFAT_GetClusterSpecified` | `0x80478968` | :x: (92.5%) |
| `VFiPFFAT_GetClusterAllocated` | `0x80478AE8` | :x: (0.0%) |
| `VFiPFFAT_GetSector` | `0x80478C00` | :x: (32.5%) |
| `VFiPFFAT_UpdateFATEntry` | `0x80478DB8` | :white_check_mark: (100.0%) |
| `VFiPFFAT_UpdateAlternateFATEntry` | `0x80478DF4` | :x: (99.2%) |
| `VFiPFFAT_GetSectorSpecified` | `0x80478EB8` | :x: (94.5%) |
| `VFiPFFAT_GetSectorAllocated` | `0x80478F68` | :white_check_mark: (100.0%) |
| `VFiPFFAT_GetContinuousSector` | `0x80479014` | :x: (54.6%) |
| `VFiPFFAT_CountAllocatedClusters` | `0x804791A8` | :white_check_mark: (100.0%) |
| `VFiPFFAT_CountFreeClusters` | `0x804792D8` | :x: (84.0%) |
| `VFiPFFAT_FreeChain` | `0x8047951C` | :x: (72.6%) |
| `VFiPFFAT_GetBeforeChain` | `0x804798FC` | :x: (51.9%) |
| `VFiPFFAT_GetBeforeSector` | `0x80479A94` | :x: (81.0%) |
| `VFiPFFAT_InitFATRegion` | `0x80479BC4` | :x: (50.3%) |
| `VFiPFFAT_MakeRootDir` | `0x80479E74` | :x: (37.8%) |
| `VFiPFFAT_TraceClustersChain` | `0x80479F5C` | :x: (94.9%) |
| `VFiPFFAT_ReadValueToSpecifiedCluster` | `0x8047A174` | :x: (69.2%) |
| `VFiPFFAT_InitHint` | `0x8047A1F0` | :white_check_mark: (100.0%) |
| `VFiPFFAT_SetHint` | `0x8047A200` | :white_check_mark: (100.0%) |
| `VFiPFFAT_ResetFFD` | `0x8047A208` | :white_check_mark: (100.0%) |
| `VFiPFFAT_InitFFD` | `0x8047A264` | :white_check_mark: (100.0%) |
| `VFiPFFAT_FinalizeFFD` | `0x8047A2CC` | :white_check_mark: (100.0%) |
| `VFiPFFAT_SetLastAccess` | `0x8047A2EC` | :x: (57.4%) |
| `VFiPFFAT_GetValueOfEOC2` | `0x8047A300` | :white_check_mark: (100.0%) |