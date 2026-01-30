#include "revolution/vf/pf_cache.h"
#include "revolution/vf/d_common.h"
#include "revolution/vf/pf_clib.h"
#include "revolution/vf/pf_driver.h"
#include "revolution/vf/pf_fat.h"

int VFiPFCACHE_InitPageList(PF_VOLUME* p_vol, PF_CACHE_PAGE** pp_head, PF_CACHE_PAGE* pages, u8 (*buffers)[512], u16 num, u32 size, u32 is_fat) {
    u32 bps_per_buf;
    u16 v9;
    u32 i;
    u16 v11;
    u32 v12;
    u16 v13;

    *pp_head = pages;
    bps_per_buf = (p_vol->bpb.bytes_per_sector >> 9) * size;
    if (is_fat == 1 && !(num / bps_per_buf) || !is_fat && num / bps_per_buf < 2)
        return 30;
    if (num / bps_per_buf == 1) {
        pages->p_prev = pages;
        pages->p_next = pages;
        pages->buffer = *buffers;
        pages->p_buf = *buffers;
        pages->p_mod_sbuf = 0;
        pages->p_mod_ebuf = 0;
        pages->size = size;
        pages->sector = -1;
        pages->option = 0;
        pages->signature = 0;
        pages->stat &= 0xFFFCu;
        if (is_fat)
            pages->stat |= 4u;
        else
            pages->stat &= ~4u;
        return 0;
    } else {
        pages->p_prev = &pages[num - num % bps_per_buf - bps_per_buf];
        pages->p_next = &pages[bps_per_buf];
        pages->buffer = *buffers;
        pages->p_buf = *buffers;
        pages->p_mod_sbuf = 0;
        pages->p_mod_ebuf = 0;
        pages->size = size;
        pages->sector = -1;
        pages->option = 0;
        pages->signature = 0;
        pages->stat &= 0xFFFCu;
        if (is_fat)
            v9 = pages->stat | 4;
        else
            v9 = pages->stat & 0xFFFB;
        pages->stat = v9;
        for (i = bps_per_buf; i < num - num % bps_per_buf - bps_per_buf; i += bps_per_buf) {
            pages[i].p_prev = &pages[i - bps_per_buf];
            pages[i].p_next = &pages[i + bps_per_buf];
            pages[i].buffer = &(*buffers)[512 * i];
            pages[i].p_buf = &(*buffers)[512 * i];
            pages[i].p_mod_sbuf = 0;
            pages[i].p_mod_ebuf = 0;
            pages[i].size = size;
            pages[i].sector = -1;
            pages[i].option = 0;
            pages[i].signature = 0;
            pages[i].stat &= 0xFFFCu;
            if (is_fat)
                v11 = pages[i].stat | 4;
            else
                v11 = pages[i].stat & 0xFFFB;
            pages[i].stat = v11;
        }
        pages[num - num % bps_per_buf - bps_per_buf].p_prev = &pages[num - num % bps_per_buf - bps_per_buf - bps_per_buf];
        pages[num - num % bps_per_buf - bps_per_buf].p_next = pages;
        pages[num - num % bps_per_buf - bps_per_buf].buffer = &(*buffers)[512 * (num - num % bps_per_buf - bps_per_buf)];
        pages[num - num % bps_per_buf - bps_per_buf].p_buf = &(*buffers)[512 * (num - num % bps_per_buf - bps_per_buf)];
        pages[num - num % bps_per_buf - bps_per_buf].p_mod_sbuf = 0;
        pages[num - num % bps_per_buf - bps_per_buf].p_mod_ebuf = 0;
        pages[num - num % bps_per_buf - bps_per_buf].size = size;
        pages[num - num % bps_per_buf - bps_per_buf].sector = -1;
        pages[num - num % bps_per_buf - bps_per_buf].option = 0;
        pages[num - num % bps_per_buf - bps_per_buf].signature = 0;
        v12 = num - num % bps_per_buf - bps_per_buf;
        pages[v12].stat &= 0xFFFCu;
        if (is_fat)
            v13 = pages[v12].stat | 4;
        else
            v13 = pages[v12].stat & 0xFFFB;
        pages[v12].stat = v13;
        return 0;
    }
}

static void VFiPFCACHE_ClearPageModified(PF_CACHE_PAGE* p_page) {
    p_page->stat &= ~2u;
    p_page->p_mod_sbuf = 0;
    p_page->p_mod_ebuf = 0;
}

static PF_CACHE_PAGE* VFiPFCACHE_SearchForPage(PF_VOLUME* p_vol, PF_CACHE_PAGE* p_head, u32 sector) {
    PF_CACHE_PAGE* p_page;
    if (sector == -1)
        return 0;
    p_page = p_head;
    do {
        if ((p_page->stat & 1) == 0)
            return 0;
        if (p_page->sector <= sector && p_page->sector + p_page->size - 1 >= sector) {
            p_page->p_buf = &p_page->buffer[(sector - p_page->sector) << p_vol->bpb.log2_bytes_per_sector];
            return p_page;
        }
        p_page = p_page->p_next;
    } while (p_page != p_head);
    return 0;
}

static u32 VFiPFCACHE_SearchForFreePage(PF_CACHE_PAGE* p_head, PF_CACHE_PAGE** pp_page) {
    PF_CACHE_PAGE* p_page;

    p_page = p_head;
    do {
        p_page = p_page->p_prev;
        if ((p_page->stat & 1) == 0) {
            *pp_page = p_page;
            return 1;
        }
        if (p_page->sector != -1) {
            *pp_page = p_page;
            return 0;
        }
    } while (p_page != p_head);
    *pp_page = 0;
    return 0;
}

static s32 VFiPFCACHE_FlushPageIfNeeded(PF_VOLUME* p_vol, PF_CACHE_PAGE* p_page) {
    u8* p_buf;
    u32 sector;
    u32 size;
    int err;
    u32 num_success;

    if (!p_page || (p_page->stat & 2) == 0 || p_page->sector == -1)
        return 0;
    p_buf = p_page->p_mod_sbuf;
    sector = p_page->sector + ((p_buf - p_page->buffer) >> p_vol->bpb.log2_bytes_per_sector);
    size = (*p_page->p_mod_ebuf >> p_vol->bpb.log2_bytes_per_sector) - (*p_buf >> p_vol->bpb.log2_bytes_per_sector) + 1;
    err = VFiPFDRV_lwrite(p_vol, p_buf, sector, size, &num_success);
    if (err)
        return err;
    if (num_success != size)
        return 17;
    if ((p_page->stat & 4) == 0 || (err = VFiPFFAT_UpdateAlternateFATEntry(p_vol, p_buf, sector, size)) == 0) {
        VFiPFCACHE_ClearPageModified(p_page);
        return 0;
    }
    return err;
}

static void VFiPFCACHE_MovePageToHead(PF_CACHE_PAGE** pp_head, PF_CACHE_PAGE* p_page) {
    if (p_page != *pp_head) {
        if (p_page == (*pp_head)->p_prev) {
            p_page->p_prev->p_next = p_page->p_next;
            p_page->p_next->p_prev = p_page->p_prev;
            p_page->p_next = *pp_head;
            p_page->p_prev = (*pp_head)->p_prev;
            p_page->p_next->p_prev = p_page;
            p_page->p_prev->p_next = p_page;
        }
        *pp_head = p_page;
    }
}

static void VFiPFCACHE_MovePageToTail(PF_CACHE_PAGE** pp_head, PF_CACHE_PAGE* p_page) {
    if (p_page == *pp_head) {
        *pp_head = p_page->p_next;
    } else if (p_page != (*pp_head)->p_prev) {
        p_page->p_prev->p_next = p_page->p_next;
        p_page->p_next->p_prev = p_page->p_prev;
        p_page->p_next = *pp_head;
        p_page->p_prev = (*pp_head)->p_prev;
        p_page->p_next->p_prev = p_page;
        p_page->p_prev->p_next = p_page;
    }
}

s32 VFiPFCACHE_DoAllocatePage(PF_VOLUME* p_vol, PF_CACHE_PAGE** pp_head, u32 sector, PF_CACHE_PAGE** pp_page, u32* p_is_hit) {
    int do_flush;
    int err;

    do_flush = 0;
    *pp_page = VFiPFCACHE_SearchForPage(p_vol, *pp_head, sector);
    if (*pp_page) {
        *p_is_hit = 1;
    } else {
        *p_is_hit = 0;
        if (!VFiPFCACHE_SearchForFreePage(*pp_head, pp_page)) {
            if (!*pp_page) {
                *pp_page = 0;
                return 30;
            }
            if (((*pp_page)->stat & 2) != 0)
                do_flush = (*pp_page)->sector != -1;
            err = VFiPFCACHE_FlushPageIfNeeded(p_vol, *pp_page);
            if (err) {
                *pp_page = 0;
                return err;
            }
            VFiPFCACHE_ClearPageModified(*pp_page);
            if (do_flush) {
                if (dCommon_FlushFromVol(p_vol, 1))
                    return 17;
            }
        }
        if (sector == -1) {
            (*pp_page)->sector = -1;
            (*pp_page)->p_buf = (*pp_page)->buffer;
        } else if (((*pp_page)->stat & 4) != 0) {
            (*pp_page)->sector = sector - sector % (*pp_page)->size;
            (*pp_page)->p_buf = &(*pp_page)->buffer[(sector % (*pp_page)->size) << p_vol->bpb.log2_bytes_per_sector];
        } else {
            if (p_vol->bpb.fat_type == FAT_32 && (sector < p_vol->bpb.first_data_sector || sector >= p_vol->bpb.total_sectors)) {
                if (sector != p_vol->bpb.fs_info_sector)
                    return 34;
                (*pp_page)->size = 1;
            }
            (*pp_page)->sector = sector - sector % (*pp_page)->size;
            (*pp_page)->p_buf = &(*pp_page)->buffer[(sector % (*pp_page)->size) << p_vol->bpb.log2_bytes_per_sector];
        }
        (*pp_page)->stat |= 1u;
    }
    VFiPFCACHE_MovePageToHead(pp_head, *pp_page);
    return 0;
}

static void VFiPFCACHE_DoFreeCachePage(PF_CACHE_PAGE** pp_head, PF_CACHE_PAGE* p_page) {
    VFiPFCACHE_ClearPageModified(p_page);
    p_page->stat &= ~1u;
    p_page->sector = -1;
    p_page->signature = 0;
    VFiPFCACHE_MovePageToTail(pp_head, p_page);
}

s32 VFiPFCACHE_DoReadPage(PF_VOLUME* p_vol, PF_CACHE_PAGE** pp_head, u32 sector, PF_CACHE_PAGE** pp_page, u32 set_sig) {
    int err;
    int v9;
    u32 num_success;
    u32 is_hit;

    err = VFiPFCACHE_DoAllocatePage(p_vol, pp_head, sector, pp_page, &is_hit);
    if (err) {
        return err;
    }

    if (!is_hit) {
        v9 = VFiPFDRV_lread(p_vol, (*pp_page)->buffer, (*pp_page)->sector, (*pp_page)->size, &num_success);
        if (v9) {
            VFiPFCACHE_DoFreeCachePage(pp_head, *pp_page);
            return v9;
        }
        if (num_success != (*pp_page)->size && p_vol->bpb.total_sectors != (*pp_page)->sector + num_success) {
            VFiPFCACHE_DoFreeCachePage(pp_head, *pp_page);
            return 17;
        }

        VFiPFCACHE_ClearPageModified(*pp_page);

        if (set_sig) {
            (*pp_page)->signature = p_vol->cache.signature;
        } else {
            (*pp_page)->signature = 0;
        }
    }
    return 0;
}

static PF_CACHE_PAGE* VFiPFCACHE_SearchForUsedPage(PF_VOLUME* p_vol, PF_CACHE_PAGE** pp_head, PF_CACHE_PAGE* p_page) {
    PF_CACHE_PAGE* p_next;

    if (!p_page) {
        p_next = *pp_head;
        goto LABEL_8;
    }
    p_next = p_page->p_next;
    if (p_next == *pp_head)
        return 0;
LABEL_8:
    while ((p_next->stat & 1) != 0) {
        if (p_next->sector != -1) {
            p_next->p_buf = p_next->buffer;
            return p_next;
        }
        p_next = p_next->p_next;
    }
    return 0;
}

s32 VFiPFCACHE_DoReadPageAndFlushIfNeeded(PF_VOLUME* p_vol, PF_CACHE_PAGE** pp_head, u32 sector, PF_CACHE_PAGE** pp_page, u32 set_sig) {
    int err;
    int v10;
    u32 num_success;
    u32 is_hit;

    err = VFiPFCACHE_DoAllocatePage(p_vol, pp_head, sector, pp_page, &is_hit);
    if (err) {
        return err;
    }

    if (!is_hit) {
        v10 = VFiPFDRV_lread(p_vol, (*pp_page)->buffer, (*pp_page)->sector, (*pp_page)->size, &num_success);
        if (v10) {
            VFiPFCACHE_DoFreeCachePage(pp_head, *pp_page);
            return v10;
        }
        if (num_success != (*pp_page)->size && p_vol->bpb.total_sectors != (*pp_page)->sector + num_success) {
            VFiPFCACHE_DoFreeCachePage(pp_head, *pp_page);
            return 17;
        }
        VFiPFCACHE_ClearPageModified(*pp_page);
        if (set_sig)
            (*pp_page)->signature = p_vol->cache.signature;
        else
            (*pp_page)->signature = 0;

    } else {
        if (set_sig && ((*pp_page)->stat & 2) != 0 && (p_vol->cache.mode & 2) != 0 && (*pp_page)->signature != p_vol->cache.signature) {
            err = VFiPFCACHE_FlushPageIfNeeded(p_vol, *pp_page);
            if (err) {
                *pp_page = 0;
                return err;
            }
            if (dCommon_FlushFromVol(p_vol, 1))
                return 17;
        }
    }
    return 0;
}

s32 VFiPFCACHE_DoReadNumSector(PF_VOLUME* p_vol, PF_CACHE_PAGE** pp_head, u8* p_buf, u32 sector, u32 num_sector, u32* p_num_success) {
    PF_CACHE_PAGE* p_page;
    u32 num_rest_sector;
    u32 num_success_sector;
    PF_CACHE_PAGE* v15;
    int err;
    *p_num_success = 0;

    err = VFiPFDRV_lread(p_vol, p_buf, sector, num_sector, p_num_success);
    if (err)
        return err;
    p_page = 0;
    num_rest_sector = *p_num_success;
    num_success_sector = *p_num_success;
    do {
        v15 = VFiPFCACHE_SearchForUsedPage(p_vol, pp_head, p_page);
        p_page = v15;
        if (v15 && v15->sector != -1) {
            if (v15->sector > sector || v15->sector + v15->size < sector + num_success_sector) {
                if (v15->sector < sector || v15->sector + v15->size > sector + num_success_sector) {
                    if (v15->sector <= sector || v15->sector >= sector + num_success_sector ||
                        v15->sector + v15->size < sector + num_success_sector) {
                        if (v15->sector < sector && v15->sector + v15->size > sector && v15->sector + v15->size <= sector + num_success_sector) {
                            VFipf_memcpy(p_buf, &v15->buffer[(sector - v15->sector) << p_vol->bpb.log2_bytes_per_sector],
                                         (v15->size - (sector - v15->sector)) << p_vol->bpb.log2_bytes_per_sector);
                            num_rest_sector -= p_page->size - (sector - p_page->sector);
                        }
                    } else {
                        VFipf_memcpy(&p_buf[(v15->sector - sector) << p_vol->bpb.log2_bytes_per_sector], v15->buffer,
                                     (v15->size - (v15->sector + v15->size - (sector + num_success_sector))) << p_vol->bpb.log2_bytes_per_sector);
                        num_rest_sector -= p_page->size - (p_page->sector + p_page->size - (sector + num_success_sector));
                    }
                } else {
                    VFipf_memcpy(&p_buf[(v15->sector - sector) << p_vol->bpb.log2_bytes_per_sector], v15->buffer,
                                 v15->size << p_vol->bpb.log2_bytes_per_sector);
                    num_rest_sector -= p_page->size;
                }
            } else {
                VFipf_memcpy(p_buf, &v15->buffer[(sector - v15->sector) << p_vol->bpb.log2_bytes_per_sector],
                             num_success_sector << p_vol->bpb.log2_bytes_per_sector);
                num_rest_sector -= num_success_sector;
            }
        }
    } while (p_page && num_rest_sector);
    if (*p_num_success == num_sector || p_vol->bpb.total_sectors == sector + *p_num_success)
        return 0;
    else
        return 17;
}

s32 VFiPFCACHE_DoWritePage(PF_VOLUME* p_vol, PF_CACHE_PAGE** pp_head, PF_CACHE_PAGE* p_page, u32 set_sig) {
    int do_flush;
    int err;

    do_flush = 0;
    VFiPFCACHE_MovePageToHead(pp_head, p_page);

    if (set_sig) {
        p_page->signature = p_vol->cache.signature;
    } else {
        p_page->signature = 0;
    }

    if ((p_vol->cache.mode & 1) != 0 || ((p_vol->cache.mode & 4) != 0 && (p_page->stat & 4) != 0)) {
        if (p_page && (p_page->stat & 2) != 0 && p_page->sector != -1) {
            do_flush = 1;
        }

        err = VFiPFCACHE_FlushPageIfNeeded(p_vol, p_page);

        if (err) {
            return err;
        }

        if (do_flush && dCommon_FlushFromVol(p_vol, 1)) {
            return 17;
        }
    }

    return 0;
}

static s32 VFiPFCACHE_UpdatePageModified(PF_CACHE_PAGE* p_page, u8* p_sbuf, u8* p_ebuf) {
    p_page->stat |= 2u;
    if (p_sbuf)
        p_page->p_mod_sbuf = p_sbuf;
    if (p_ebuf)
        p_page->p_mod_ebuf = p_ebuf;
    return 0;
}

static int VFiPFCACHE_ClearModified(PF_VOLUME* p_vol, PF_CACHE_PAGE* p_head) {
    PF_CACHE_PAGE* p_page;

    p_page = p_head;
    do {
        if ((p_page->stat & 1) == 0)
            break;
        VFiPFCACHE_ClearPageModified(p_page);
        p_page = p_page->p_next;
    } while (p_page != p_head);
    return 0;
}

s32 VFiPFCACHE_DoWriteNumSectorAndFreeIfNeeded(PF_VOLUME* p_vol, PF_CACHE_PAGE** pp_head, const u8* p_buf, u32 sector, u32 num_sector,
                                               u32* p_num_success) {
    PF_CACHE_PAGE* v11;
    PF_CACHE_PAGE* p_page;
    u8* v15;
    u8* p_ebuf;
    u8* v17;
    u8* p_sbuf;
    u32 num_rest_sector;
    int err;

    v11 = 0;
    num_rest_sector = num_sector;
    *p_num_success = 0;
    do {
        p_page = VFiPFCACHE_SearchForUsedPage(p_vol, pp_head, v11);
        v11 = p_page;
        if (p_page && p_page->sector != -1) {
            if (p_page->sector > sector || p_page->sector + p_page->size < sector + num_sector) {
                if (p_page->sector < sector || p_page->sector + p_page->size > sector + num_sector) {
                    if (p_page->sector <= sector || p_page->sector >= sector + num_sector || p_page->sector + p_page->size < sector + num_sector) {
                        if (p_page->sector < sector && p_page->sector + p_page->size > sector &&
                            p_page->sector + p_page->size <= sector + num_sector) {
                            VFipf_memcpy(&p_page->buffer[(sector - p_page->sector) << p_vol->bpb.log2_bytes_per_sector], (void*)p_buf,
                                         (p_page->size - (sector - p_page->sector)) << p_vol->bpb.log2_bytes_per_sector);
                            num_rest_sector -= v11->size - (sector - v11->sector);
                            *p_num_success += v11->size - (sector - v11->sector);
                            p_sbuf = &v11->buffer[(sector - v11->sector) << p_vol->bpb.log2_bytes_per_sector];
                            if (v11->p_mod_sbuf && p_sbuf >= v11->p_mod_sbuf)
                                p_sbuf = 0;
                            VFiPFCACHE_UpdatePageModified(v11, p_sbuf, &v11->buffer[(v11->size - 1) << p_vol->bpb.log2_bytes_per_sector]);
                        }
                    } else {
                        VFipf_memcpy(p_page->buffer, (void*)&p_buf[(p_page->sector - sector) << p_vol->bpb.log2_bytes_per_sector],
                                     (p_page->size - (p_page->sector + p_page->size - (sector + num_sector))) << p_vol->bpb.log2_bytes_per_sector);
                        num_rest_sector -= v11->size - (v11->sector + v11->size - (sector + num_sector));
                        *p_num_success += v11->size - (v11->sector + v11->size - (sector + num_sector));
                        p_ebuf =
                            &v11->buffer[(v11->size - (v11->sector + v11->size - (sector + num_sector)) - 1) << p_vol->bpb.log2_bytes_per_sector];
                        if (v11->p_mod_ebuf && v11->p_mod_ebuf >= p_ebuf)
                            p_ebuf = 0;
                        VFiPFCACHE_UpdatePageModified(v11, v11->buffer, p_ebuf);
                    }
                } else {
                    VFipf_memcpy(p_page->buffer, (void*)&p_buf[(p_page->sector - sector) << p_vol->bpb.log2_bytes_per_sector],
                                 p_page->size << p_vol->bpb.log2_bytes_per_sector);
                    num_rest_sector -= v11->size;
                    *p_num_success += v11->size;
                    VFiPFCACHE_UpdatePageModified(v11, v11->buffer, &v11->buffer[(v11->size - 1) << p_vol->bpb.log2_bytes_per_sector]);
                }
            } else {
                VFipf_memcpy(&p_page->buffer[(sector - p_page->sector) << p_vol->bpb.log2_bytes_per_sector], (void*)p_buf,
                             num_sector << p_vol->bpb.log2_bytes_per_sector);
                *p_num_success += num_rest_sector;
                num_rest_sector = 0;
                v17 = &v11->buffer[(sector - v11->sector) << p_vol->bpb.log2_bytes_per_sector];
                v15 = (((num_sector - 1) << p_vol->bpb.log2_bytes_per_sector) + v17);
                if (v11->p_mod_sbuf) {
                    if (v17 >= v11->p_mod_sbuf)
                        v17 = 0;
                    if (v11->p_mod_ebuf >= v15)
                        v15 = 0;
                }
                VFiPFCACHE_UpdatePageModified(v11, v17, v15);
            }
        }
    } while (v11 && num_rest_sector);
    if (!num_rest_sector && (p_vol->cache.mode & 1) == 0)
        goto LABEL_37;
    err = VFiPFDRV_lwrite(p_vol, p_buf, sector, num_sector, p_num_success);
    if (err)
        return err;
    if (dCommon_FlushFromVol(p_vol, 1))
        return 17;
LABEL_37:
    if ((p_vol->cache.mode & 1) != 0 && num_sector != num_rest_sector)
        VFiPFCACHE_ClearModified(p_vol, p_vol->cache.p_current_data);
    return 0;
}

s32 VFiPFCACHE_DoFlushCache(PF_VOLUME* p_vol, PF_CACHE_PAGE* p_head) {
    int do_flush;
    int first_err;
    struct PF_CACHE_PAGE* v6;
    u32 v8;
    int err;
    u32 v10;

    do_flush = 0;
    first_err = 0;
    v6 = p_head;
    if ((p_head->stat & 1) == 0)
        return 0;
    do {
        if ((v6->stat & 1) == 0)
            break;
        v8 = 0;
        if (v6 && (v6->stat & 2) != 0)
            v8 = v6->sector != -1;
        err = VFiPFCACHE_FlushPageIfNeeded(p_vol, v6);
        if (v8 && !err)
            do_flush = 1;
        if (err && !first_err)
            first_err = err;
        v6 = v6->p_next;
    } while (v6 != p_head);
    if (do_flush && (v10 = first_err == 0, dCommon_FlushFromVol(p_vol, v10)) && v10)
        return 17;
    else
        return first_err;
}

void VFiPFCACHE_SetCache(PF_VOLUME* p_vol, PF_CACHE_PAGE* p_cache_page, u8 (*p_cache_buf)[512], u16 num_fat_pages, u16 num_data_pages) {
    p_vol->cache.pages = p_cache_page;
    p_vol->cache.buffers = p_cache_buf;
    p_vol->cache.num_fat_pages = num_fat_pages;
    p_vol->cache.num_data_pages = num_data_pages;
}

void VFiPFCACHE_SetFATBufferSize(PF_VOLUME* p_vol, u32 size) {
    if (size)
        p_vol->cache.fat_buff_size = size;
}

void VFiPFCACHE_SetDataBufferSize(PF_VOLUME* p_vol, u32 size) {
    if (size)
        p_vol->cache.data_buff_size = size;
}

s32 VFiPFCACHE_InitCaches(PF_VOLUME* p_vol) {
    int err;

    p_vol->cache.mode &= ~1u;
    p_vol->cache.mode |= 2u;
    p_vol->cache.mode &= ~4u;
    VFipf_memset(p_vol->cache.buffers, 0, (p_vol->cache.num_fat_pages + p_vol->cache.num_data_pages) << 9);
    err = VFiPFCACHE_InitPageList(p_vol, &p_vol->cache.p_current_fat, p_vol->cache.pages, p_vol->cache.buffers, p_vol->cache.num_fat_pages,
                                  p_vol->cache.fat_buff_size, 1u);
    if (!err) {
        err = VFiPFCACHE_InitPageList(p_vol, &p_vol->cache.p_current_data, &p_vol->cache.pages[p_vol->cache.num_fat_pages],
                                      &p_vol->cache.buffers[p_vol->cache.num_fat_pages], p_vol->cache.num_data_pages, p_vol->cache.data_buff_size, 0);
        if (!err)
            return 0;
    }
    return err;
}

s32 VFiPFCACHE_UpdateModifiedSector(PF_VOLUME* p_vol, PF_CACHE_PAGE* p_page, u32 num_sector) {
    u8* p_sbuf;
    u8* p_ebuf;

    p_sbuf = p_page->p_buf;
    p_ebuf = &p_sbuf[(num_sector - 1) << p_vol->bpb.log2_bytes_per_sector];
    if (p_page->p_mod_sbuf) {
        if (p_page->p_buf >= p_page->p_mod_sbuf) {
            p_sbuf = 0;
        }
        if (p_ebuf <= p_page->p_mod_ebuf) {
            p_ebuf = 0;
        }
    }
    VFiPFCACHE_UpdatePageModified(p_page, p_sbuf, p_ebuf);
    return 0;
}

s32 VFiPFCACHE_AllocateDataPage(PF_VOLUME* p_vol, u32 sector, PF_CACHE_PAGE** pp_page) {
    int err;
    u32 is_hit;

    if (!p_vol->cache.p_current_data) {
        p_vol->cache.pages->buffer = *p_vol->cache.buffers;
        p_vol->cache.pages->p_buf = *p_vol->cache.buffers;
        *pp_page = p_vol->cache.pages;
        return 0;
    } else {
        err = VFiPFCACHE_DoAllocatePage(p_vol, &p_vol->cache.p_current_data, sector, pp_page, &is_hit);
        if (!err) {
            return 0;
        }
    }
    return err;
}

void VFiPFCACHE_FreeDataPage(PF_VOLUME* p_vol, PF_CACHE_PAGE* p_page) {
    if (p_vol->cache.p_current_data)
        VFiPFCACHE_DoFreeCachePage(&p_vol->cache.p_current_data, p_page);
}

s32 VFiPFCACHE_ReadFATPage(PF_VOLUME* p_vol, u32 sector, PF_CACHE_PAGE** pp_page) {
    int err;

    err = VFiPFCACHE_DoReadPage(p_vol, &p_vol->cache.p_current_fat, sector, pp_page, 0);
    if (!err) {
        return 0;
    }

    return err;
}

s32 VFiPFCACHE_ReadDataPage(PF_VOLUME* p_vol, u32 sector, PF_CACHE_PAGE** pp_page, u32 set_sig) {
    int err;

    err = VFiPFCACHE_DoReadPage(p_vol, &p_vol->cache.p_current_data, sector, pp_page, set_sig);
    if (!err)
        return 0;
    return err;
}

s32 VFiPFCACHE_ReadDataPageAndFlushIfNeeded(PF_VOLUME* p_vol, u32 sector, PF_CACHE_PAGE** pp_page, u32 set_sig) {
    int err;

    err = VFiPFCACHE_DoReadPageAndFlushIfNeeded(p_vol, &p_vol->cache.p_current_data, sector, pp_page, set_sig);
    if (!err)
        return 0;
    return err;
}

s32 VFiPFCACHE_ReadDataNumSector(PF_VOLUME* p_vol, u8* p_buf, u32 sector, u32 num_sector, u32* p_num_success) {
    int err;

    if (sector >= p_vol->bpb.total_sectors)
        return 16;
    err = VFiPFCACHE_DoReadNumSector(p_vol, &p_vol->cache.p_current_data, p_buf, sector, num_sector, p_num_success);
    if (!err)
        return 0;
    return err;
}

s32 VFiPFCACHE_WriteFATPage(PF_VOLUME* p_vol, PF_CACHE_PAGE* p_page) {
    int err;

    err = VFiPFCACHE_DoWritePage(p_vol, &p_vol->cache.p_current_fat, p_page, 0);
    if (!err)
        return 0;
    return err;
}

s32 VFiPFCACHE_WriteDataPage(PF_VOLUME* p_vol, PF_CACHE_PAGE* p_page, u32 set_sig) {
    int err;

    err = VFiPFCACHE_DoWritePage(p_vol, &p_vol->cache.p_current_data, p_page, set_sig);
    if (!err)
        return 0;
    return err;
}

s32 VFiPFCACHE_WriteDataNumSectorAndFreeIfNeeded(PF_VOLUME* p_vol, const u8* p_buf, u32 sector, u32 num_sector, u32* p_num_success) {
    int err;

    if (sector >= p_vol->bpb.total_sectors)
        return 16;
    err = VFiPFCACHE_DoWriteNumSectorAndFreeIfNeeded(p_vol, &p_vol->cache.p_current_data, p_buf, sector, num_sector, p_num_success);
    if (!err)
        return 0;
    return err;
}

PF_CACHE_PAGE* VFiPFCACHE_SearchDataCache(PF_VOLUME* p_vol, u32 sector) {
    return VFiPFCACHE_SearchForPage(p_vol, p_vol->cache.p_current_data, sector);
}

s32 VFiPFCACHE_FlushFATCache(PF_VOLUME* p_vol) {
    int err;

    err = VFiPFCACHE_DoFlushCache(p_vol, p_vol->cache.p_current_fat);
    if (!err)
        return 0;
    return err;
}

s32 VFiPFCACHE_FlushDataCacheSpecific(PF_VOLUME* p_vol, void* signature) {
    int do_flush;
    int err;
    PF_CACHE_PAGE* i;
    PF_CACHE_PAGE* p_head;

    do_flush = 0;
    if ((p_vol->cache.mode & 1) != 0)
        return 0;

    p_head = p_vol->cache.p_current_data;

    if (!p_head->signature || p_head->signature == signature) {
        if ((p_head->stat & 2) != 0 && p_head->sector != -1)
            do_flush = 1;

        err = VFiPFCACHE_FlushPageIfNeeded(p_vol, p_head);
        if (err)
            return err;
    }

    for (i = p_head->p_next; i != p_head; i = i->p_next) {
        if (!i->signature || i->signature == signature) {
            err = VFiPFCACHE_FlushPageIfNeeded(p_vol, i);
            if (err)
                return err;
            if (do_flush)
                dCommon_FlushFromVol(p_vol, 0);
        }
    }

    if (do_flush && dCommon_FlushFromVol(p_vol, 1))
        return 17;

    return 0;
}

s32 VFiPFCACHE_FlushAllCaches(PF_VOLUME* p_vol) {
    int err;

    err = VFiPFCACHE_DoFlushCache(p_vol, p_vol->cache.p_current_fat);
    if (err) {
        return err;
    }

    err = VFiPFCACHE_DoFlushCache(p_vol, p_vol->cache.p_current_data);
    if (!err)
        return 0;

    return err;
}

static void VFiPFCACHE_FreeFATPage(struct PF_VOLUME* p_vol, struct PF_CACHE_PAGE* p_page) {
    if (p_vol->cache.p_current_fat)
        VFiPFCACHE_DoFreeCachePage(&p_vol->cache.p_current_fat, p_page);
}

void VFiPFCACHE_FreeAllCaches(struct PF_VOLUME* p_vol) {
    while ((p_vol->cache.p_current_fat->stat & 1) != 0)
        VFiPFCACHE_FreeFATPage(p_vol, p_vol->cache.p_current_fat);
    while ((p_vol->cache.p_current_data->stat & 1) != 0)
        VFiPFCACHE_FreeDataPage(p_vol, p_vol->cache.p_current_data);
}
