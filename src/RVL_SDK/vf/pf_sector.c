#include "revolution/vf/pf_sector.h"
#include "revolution/vf/pf_cache.h"
#include "revolution/vf/pf_clib.h"
#include "revolution/vf/pf_driver.h"

s32 VFiPFSEC_ReadFAT(PF_VOLUME* p_vol, u8* p_buf, u32 sector, u16 offset, u16 size) {
    int err;
    PF_CACHE_PAGE* p_page;

    if (!p_vol)
        return 10;
    if (!p_buf)
        return 10;
    if ((p_vol->flags & 1) == 0 || !VFiPFDRV_IsInserted(p_vol))
        return 9;
    err = VFiPFCACHE_ReadFATPage(p_vol, sector, &p_page);
    if (err) {
        return err;
    }

    VFipf_memcpy(p_buf, &p_page->p_buf[offset], size);
    return 0;
}

s32 VFiPFSEC_WriteFAT(PF_VOLUME* p_vol, const u8* p_buf, u32 sector, u16 offset, u16 size) {
    int err;
    PF_CACHE_PAGE* p_page;

    if (!p_vol)
        return 10;
    if (!p_buf)
        return 10;
    if ((p_vol->flags & 1) == 0 || !VFiPFDRV_IsInserted(p_vol) || VFiPFDRV_IsWProtected(p_vol))
        return 9;
    err = VFiPFCACHE_ReadFATPage(p_vol, sector, &p_page);
    if (err) {
        return err;
    }

    VFipf_memcpy(&p_page->p_buf[offset], (void*)p_buf, size);
    VFiPFCACHE_UpdateModifiedSector(p_vol, p_page, 1);
    err = VFiPFCACHE_WriteFATPage(p_vol, p_page);
    if (err) {
        return err;
    }

    return 0;
}

s32 VFiPFSEC_ReadData(PF_VOLUME* p_vol, u8* p_buf, u32 sector, u16 offset, u32 size, u32* p_success_size, u32 set_sig) {
    int err;
    PF_CACHE_PAGE* p_page;

    if (!p_vol || !p_buf || !p_success_size)
        return 10;
    if (!size || size >= p_vol->bpb.bytes_per_sector)
        return 10;
    *p_success_size = 0;
    if ((p_vol->flags & 1) == 0 || !VFiPFDRV_IsInserted(p_vol))
        return 9;
    err = VFiPFCACHE_ReadDataPage(p_vol, sector, &p_page, set_sig);
    if (err) {
        return err;
    }

    VFipf_memcpy(p_buf, &p_page->p_buf[offset], size);
    *p_success_size = size;
    return 0;
}

s32 VFiPFSEC_ReadDataSector(PF_VOLUME* p_vol, u8* p_buf, u32 sector, u32 size, u32* p_success_size, u32 set_sig) {
    u32 v9;
    int err;
    u32 v12;
    u32 v13;
    u32 adjust_sector;
    PF_CACHE_PAGE* p_page;
    u32 num_success;
    u32 rest_sector;

    v9 = size;
    if (!p_vol || !p_buf || !p_success_size)
        return 10;
    if (!size || size < p_vol->bpb.bytes_per_sector)
        return 10;
    *p_success_size = 0;
    if ((p_vol->flags & 1) == 0 || !VFiPFDRV_IsInserted(p_vol))
        return 9;
    if ((v9 & (p_vol->bpb.bytes_per_sector - 1)) == 0) {
        err = VFiPFCACHE_ReadDataNumSector(p_vol, p_buf, sector, v9 >> p_vol->bpb.log2_bytes_per_sector, &num_success);
        if (err)
            return err;
        *p_success_size += num_success << p_vol->bpb.log2_bytes_per_sector;
        if (num_success != v9 >> p_vol->bpb.log2_bytes_per_sector)
            return 17;
    } else {
        p_page = VFiPFCACHE_SearchDataCache(p_vol, sector);
        if (p_page) {
            v12 = p_page->sector + p_page->size - sector;
            if (v12 <= v9 >> p_vol->bpb.log2_bytes_per_sector) {
                VFipf_memcpy(p_buf, p_page->p_buf, v12 << p_vol->bpb.log2_bytes_per_sector);
                *p_success_size += v12 << p_vol->bpb.log2_bytes_per_sector;
                v9 -= v12 << p_vol->bpb.log2_bytes_per_sector;
                sector += v12;
            } else {
                VFipf_memcpy(p_buf, p_page->p_buf, v9);
                *p_success_size += v9;
                v9 = 0;
            }
        }
        if (v9) {
            v13 = v9 >> p_vol->bpb.log2_bytes_per_sector;
            rest_sector = (sector + v13) % p_vol->cache.data_buff_size;
            if (v13 > rest_sector) {
                adjust_sector = v13 - rest_sector;
                err = VFiPFCACHE_ReadDataNumSector(p_vol, &p_buf[*p_success_size], sector, v13 - rest_sector, &num_success);
                if (err)
                    return err;
                *p_success_size += num_success << p_vol->bpb.log2_bytes_per_sector;
                if (num_success != adjust_sector)
                    return 17;
                sector += adjust_sector;
                v13 = rest_sector;
            }
            if (v13) {
                err = VFiPFCACHE_ReadDataPage(p_vol, sector, &p_page, set_sig);
                if (err)
                    return err;
                VFipf_memcpy(&p_buf[*p_success_size], p_page->p_buf, v13 << p_vol->bpb.log2_bytes_per_sector);
                *p_success_size += v13 << p_vol->bpb.log2_bytes_per_sector;
            }
        }
    }
    return 0;
}

s32 VFiPFSEC_WriteData(PF_VOLUME* p_vol, const u8* p_buf, u32 sector, u16 offset, u32 size, u32* p_success_size, u32 is_direct, u32 set_sig) {
    int result;
    int v15;
    PF_CACHE_PAGE* p_page;

    if (!p_vol || !p_buf || !p_success_size)
        return 10;
    if (!size || size >= p_vol->bpb.bytes_per_sector)
        return 10;
    *p_success_size = 0;
    if ((p_vol->flags & 1) == 0 || !VFiPFDRV_IsInserted(p_vol) || VFiPFDRV_IsWProtected(p_vol))
        return 9;
    if (is_direct)
        v15 = VFiPFCACHE_AllocateDataPage(p_vol, sector, &p_page);
    else
        v15 = VFiPFCACHE_ReadDataPageAndFlushIfNeeded(p_vol, sector, &p_page, set_sig);
    if (v15)
        return v15;
    VFipf_memcpy(&p_page->p_buf[offset], (void*)p_buf, size);
    VFiPFCACHE_UpdateModifiedSector(p_vol, p_page, 1);
    result = VFiPFCACHE_WriteDataPage(p_vol, p_page, set_sig);
    if (result) {
        return result;
    }

    *p_success_size = size;
    return 0;
}

s32 VFiPFSEC_WriteDataSector(PF_VOLUME* p_vol, const u8* p_buf, u32 sector, u32 size, u32* p_success_size, u32 is_direct, u32 set_sig) {
    int err;
    u32 num_sector;
    u32 adjust_sector;
    int v16;
    PF_CACHE_PAGE* p_page;
    u32 num_success;
    u32 rest_sector;

    if (!p_vol || !p_buf || !p_success_size)
        return 10;
    if (!size || size < p_vol->bpb.bytes_per_sector)
        return 10;
    *p_success_size = 0;
    if ((p_vol->flags & 1) == 0 || !VFiPFDRV_IsInserted(p_vol) || VFiPFDRV_IsWProtected(p_vol))
        return 9;
    if ((size & (p_vol->bpb.bytes_per_sector - 1)) == 0) {
        err = VFiPFCACHE_WriteDataNumSectorAndFreeIfNeeded(p_vol, p_buf, sector, size >> p_vol->bpb.log2_bytes_per_sector, &num_success);
        if (err)
            return err;
        *p_success_size = num_success << p_vol->bpb.log2_bytes_per_sector;
        if (num_success != size >> p_vol->bpb.log2_bytes_per_sector)
            return 17;
        return 0;
    }
    num_sector = size >> p_vol->bpb.log2_bytes_per_sector;
    rest_sector = (sector + num_sector) % p_vol->cache.data_buff_size;
    if (num_sector > rest_sector) {
        adjust_sector = num_sector - rest_sector;
        err = VFiPFCACHE_WriteDataNumSectorAndFreeIfNeeded(p_vol, p_buf, sector, num_sector - rest_sector, &num_success);
        if (err)
            return err;
        *p_success_size = num_success << p_vol->bpb.log2_bytes_per_sector;
        if (num_success != adjust_sector)
            return 17;
        sector += adjust_sector;
        num_sector = rest_sector;
    }
    if (!num_sector)
        return 0;
    if (is_direct)
        v16 = VFiPFCACHE_AllocateDataPage(p_vol, sector, &p_page);
    else
        v16 = VFiPFCACHE_ReadDataPageAndFlushIfNeeded(p_vol, sector, &p_page, set_sig);
    if (v16)
        return v16;
    VFipf_memcpy(p_page->p_buf, (void*)&p_buf[*p_success_size], num_sector << p_vol->bpb.log2_bytes_per_sector);
    VFiPFCACHE_UpdateModifiedSector(p_vol, p_page, num_sector);
    err = VFiPFCACHE_WriteDataPage(p_vol, p_page, set_sig);
    if (err) {
        return err;
    }

    *p_success_size += num_sector << p_vol->bpb.log2_bytes_per_sector;
    return 0;
}
