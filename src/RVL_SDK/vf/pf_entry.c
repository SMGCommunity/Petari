#include "revolution/vf/pf_entry.h"
#include "revolution/vf/pf_clib.h"
#include "revolution/vf/pf_entry_iterator.h"
#include "revolution/vf/pf_fat.h"
#include "revolution/vf/pf_path.h"
#include "revolution/vf/pf_sector.h"
#include "revolution/vf/pf_service.h"
#include "revolution/vf/pf_str.h"
#include "revolution/vf/pf_volume.h"

#define GET_U16_LE(addr) ((u16)(__rlwinm(*(u16*)(addr), 24, 24, 31) | __rlwinm(*(u16*)(addr), 8, 16, 23)))

#define GET_U32_LE(addr) ((*(u32*)(addr) << 24) | __rlwinm(*(u32*)(addr), 8, 8, 15) | ((*(u32*)(addr) >> 24) | __rlwinm(*(u32*)(addr), 24, 16, 23)))

#define SET_U16_LE(addr, val) (*(u16*)(addr) = (u16)(__rlwinm((val), 24, 24, 31) | __rlwinm((val), 8, 16, 23)))

#define SET_U32_LE(addr, val)                                                                                                                        \
    (*(u32*)(addr) =                                                                                                                                 \
         (u32)(__rlwimi(__rlwinm((u32)(val), 8, 8, 15), (u32)(val), 24, 0, 7) | __rlwimi(__rlwinm((u32)(val), 24, 16, 23), (u32)(val), 8, 24, 31)))

static void VFiPFENT_storeShortNameToBuf(u8* buf, const PF_DIR_ENT* p_ent) {
    VFiPFPATH_putShortName(buf, p_ent->short_name, p_ent->attr);
    if (*buf == 229)
        *buf = 5;
}

s32 VFiPFENT_searchEmptyTailSFN(PF_FFD* p_ffd, u32 tail_index, const s8* pattern, u32* p_tail_bit) {
    s8 sfnbuf[13];
    s8 patbuf[13];

    u32* p_start_cluster;
    u32 max_chain_index;
    u32 cluster;
    int v11;
    u32 max_count;
    int j;
    u32 sfn_taillen;
    u32 sfn_baselen;
    int i;
    u32 bit_pos;
    PF_ENT_ITER iter;
    PF_VOLUME* p_vol;
    PF_FAT_HINT* p_hint;

    p_vol = p_ffd->p_vol;
    VFipf_memset(p_tail_bit, 0, 4 * p_vol->tail_entry.tracker_size);
    iter.ffd = *p_ffd;

    for (j = VFiPFENT_ITER_IteratorInitialize(&iter, 0); !VFiPFENT_ITER_IsAtLogicalEnd(&iter); j = VFiPFENT_ITER_Advance(&iter, 0)) {
        if (j)
            return j;
        if (!iter.buf[0])
            break;
        if (iter.buf[0] != 229 && (iter.buf[11] & 0xF) != 0xF && (iter.buf[11] & 8) == 0) {
            VFiPFPATH_getShortName(sfnbuf, iter.buf, 0);
            for (sfn_taillen = 1; sfnbuf[sfn_taillen] != 126 && sfnbuf[sfn_taillen] && sfn_taillen < 7; ++sfn_taillen)
                ;
            if (sfn_taillen < 7 && sfnbuf[sfn_taillen] == 126) {
                for (sfn_baselen = sfn_taillen + 1; sfnbuf[sfn_baselen] >= 48 && sfnbuf[sfn_baselen] <= 57; ++sfn_baselen)
                    ;
                if (sfnbuf[sfn_baselen] == 46 || !sfnbuf[sfn_baselen]) {
                    i = sfn_baselen - sfn_taillen - 1;
                    bit_pos = 0;
                    while (i) {
                        bit_pos = -48 + bit_pos * 10 + sfnbuf[sfn_baselen - i];
                        i--;
                    }
                    VFipf_strcpy(patbuf, pattern);
                    VFiPFPATH_parseShortNameNumeric(patbuf, bit_pos);
                    if (!VFiPFPATH_cmpTailSFN(sfnbuf, patbuf) && bit_pos >= tail_index &&
                        bit_pos < 32 * p_vol->tail_entry.tracker_size + tail_index) {
                        p_tail_bit[(bit_pos - tail_index) >> 5] |= 1 << (bit_pos - tail_index);
                    }
                }
            }
        }
    }
    return 0;
}

s32 VFiPFENT_findEmptyTailSFN(PF_DIR_ENT* p_ent_containig_dir, const s8* name, u32* p_tails) {
    PF_VOLUME* p_vol;
    u32 num;
    int err;
    u32 track_num;
    PF_FAT_HINT hint;
    PF_FFD ffd;

    p_vol = p_ent_containig_dir->p_vol;
    *p_tails = 1;
    VFiPFFAT_InitFFD(&ffd, &hint, p_ent_containig_dir->p_vol, &p_ent_containig_dir->start_cluster);
    for (num = 1; num <= 0xF423F; num += 32 * p_vol->tail_entry.tracker_size) {
        err = VFiPFENT_searchEmptyTailSFN(&ffd, num, name, p_vol->tail_entry.tracker_bits);
        if (err)
            return err;
        for (track_num = 0; track_num < p_vol->tail_entry.tracker_size; ++track_num) {
            if (p_vol->tail_entry.tracker_bits[track_num] != -1) {
                while ((p_vol->tail_entry.tracker_bits[track_num] & 1) != 0) {
                    p_vol->tail_entry.tracker_bits[track_num] >>= 1;
                    ++*p_tails;
                }
                num = 1000000;
                break;
            }
            *p_tails += 32;
        }
    }
    return 0;
}

u8 VFiPFENT_CalcCheckSum(PF_DIR_ENT* p_ent) {
    u16 i;
    u8 sum;
    u8 buf[13];

    VFiPFPATH_putShortName(buf, p_ent->short_name, 0);
    sum = 0;
    for (i = 0; i < 0xB; i++) {
        sum = (sum >> 1) + (((sum & 0x1) != 0) ? 0x80 : 0) + buf[i];
    }
    return sum;
}

void VFiPFENT_LoadShortNameFromBuf(PF_DIR_ENT* p_ent, const u8* buf) {
    VFiPFPATH_getShortName(p_ent->short_name, buf, buf[11]);
    if (p_ent->short_name[0] == 5) {
        p_ent->short_name[0] = -27;
    }
}

void VFiPFENT_loadEntryNumericFieldsFromBuf(PF_DIR_ENT* p_ent, const u8* buf) {
    p_ent->attr = buf[11];
    p_ent->small_letter_flag = buf[12];
    p_ent->create_time_ms = buf[13];

    p_ent->create_time = GET_U16_LE(&buf[0x0E]);
    p_ent->create_date = GET_U16_LE(&buf[0x10]);
    p_ent->access_date = GET_U16_LE(&buf[0x12]);
    p_ent->modify_time = GET_U16_LE(&buf[0x16]);
    p_ent->modify_date = GET_U16_LE(&buf[0x18]);

    p_ent->file_size = GET_U32_LE(&buf[0x1C]);

    p_ent->start_cluster = (u32)GET_U16_LE(&buf[0x14]) << 16 | GET_U16_LE(&buf[0x1A]);
}

s32 VFiPFENT_LoadLFNEntryFieldsFromBuf(PF_DIR_ENT* p_ent, const u8* buf, u32 is_reverse) {
    u8 ordinal;
    u8 expected_ordinal;
    u8 check_sum;
    u32 is_first;
    u32 is_last_LFN_ent;
    u16* p_name_slot;
    u16* q;
    u16* q_after;

    is_first = 1;
    is_last_LFN_ent = 0;

    check_sum = buf[13];
    ordinal = buf[0];

    if (ordinal & 0x40) {
        is_last_LFN_ent = 1;
        ordinal &= ~0x40;
    }

    if (ordinal > 20) {
        p_ent->ordinal = 0;
        p_ent->check_sum = 0;
        p_ent->num_entry_LFNs = 0;
        return 33;
    }

    if (is_reverse) {
        if (ordinal != 1) {
            expected_ordinal = p_ent->ordinal + 1;
            is_first = 0;
        }
    } else {
        if (!is_last_LFN_ent) {
            expected_ordinal = p_ent->ordinal - 1;
            is_first = 0;
        }
    }

    if (is_first) {
        p_ent->num_entry_LFNs = 0;
    } else {
        if (p_ent->num_entry_LFNs == 0) {
            return 33;
        }

        if (ordinal != expected_ordinal || check_sum != p_ent->check_sum) {
            p_ent->ordinal = 0;
            p_ent->check_sum = 0;
            p_ent->num_entry_LFNs = 0;
            return 33;
        }
    }

    p_ent->ordinal = ordinal;
    p_ent->check_sum = check_sum;

    p_name_slot = p_ent->long_name + (ordinal - 1) * 13;

    VFipf_memcpy(p_name_slot, buf + 1, 10);
    VFipf_memcpy(p_name_slot + 5, buf + 14, 12);
    VFipf_memcpy(p_name_slot + 11, buf + 28, 4);

    VFiPF_LE16_TO_U16_STR((u8*)p_name_slot, 10);
    VFiPF_LE16_TO_U16_STR((u8*)p_name_slot + 5, 12);
    VFiPF_LE16_TO_U16_STR((u8*)p_name_slot + 11, 4);

    if (is_last_LFN_ent) {
        p_name_slot[13] = 0;
        q = p_name_slot;
        q_after = p_name_slot + 13;

        while (q < q_after) {
            if (*q == 0x0000) {
                q++;
                break;
            }
            q++;
        }

        while (q < q_after) {
            if (*q != 0xFFFF) {
                p_ent->num_entry_LFNs = 0;
                return 33;
            }
            q++;
        }
    }

    p_ent->num_entry_LFNs++;
    return 0;
}

void VFiPFENT_storeLFNEntryFieldsToBuf(u8* buf, PF_DIR_ENT* p_ent, u8 ord, u8 sum, u32 is_last) {
    u8* p_seg;
    u16* p;
    u16* p_after;

    if (is_last) {
        buf[0] = ord | 0x40;
    } else {
        buf[0] = ord;
    }

    buf[11] = 0x0F;
    buf[13] = sum;
    buf[12] = 0;
    *(u16*)&buf[26] = 0;

    p_seg = (u8*)p_ent->long_name + (u32)(ord - 1) * 26;

    if (is_last) {
        p = (u16*)p_seg;
        p_after = (u16*)(p_seg + 26);

        while (p < p_after) {
            if (*p == 0) {
                p++;
                break;
            }
            p++;
        }

        while (p < p_after) {
            *p++ = 0xFFFF;
        }
    }

    VFipf_memcpy(buf + 1, p_seg, 10);
    VFipf_memcpy(buf + 14, p_seg + 10, 12);
    VFipf_memcpy(buf + 28, p_seg + 22, 4);

    VFiPF_LE16_TO_U16_STR(buf + 1, 10);
    VFiPF_LE16_TO_U16_STR(buf + 14, 12);
    VFiPF_LE16_TO_U16_STR(buf + 28, 4);
}

s32 VFiPFENT_GetParentEntryOfPath(PF_DIR_ENT* p_ent, PF_VOLUME* p_vol, PF_STR* p_path) {
    PF_ENT_ITER iter;

    if (!p_ent || !p_vol) {
        return 10;
    }

    if (!p_path->p_head) {
        return 10;
    }

    if (VFiPFSTR_StrLen(p_path) == 0) {
        return 10;
    } else {
        return VFiPFENT_ITER_GetEntryOfPath(&iter, p_ent, p_vol, p_path, 1);
    }

    return 10;
}

s32 VFiPFENT_findEntry(PF_FFD* p_ffd, PF_DIR_ENT* p_ent, u32 index_search_from, PF_STR* p_pattern, u8 attr_required, u32* p_lpos, u32* p_ppos) {
    s32 err;
    PF_ENT_ITER iter;
    u32 is_found;

    if (p_ppos) {
        *p_ppos = 999999;
    }
    if (p_ppos) {
        *p_ppos = 999999;
    }

    if (!p_ffd || !p_ent) {
        return 10;
    }

    if (!p_pattern->p_head) {
        return 10;
    }

    if (VFiPFSTR_StrLen(p_pattern) == 0) {
        return 10;
    }

    if (index_search_from >= 999999) {
        return 10;
    }

    iter.ffd = *p_ffd;

    err = VFiPFENT_ITER_IteratorInitialize(&iter, index_search_from);
    if (err != 0) {
        return err;
    }

    err = VFiPFENT_ITER_FindEntry(&iter, p_ent, p_pattern, attr_required, &is_found, p_ppos, p_lpos, 0);
    if (err != 0) {
        return err;
    }

    if (!is_found) {
        return 3;
    } else {
        return 0;
    }
}

s32 VFiPFENT_allocateEntry(PF_DIR_ENT* p_ent, u8 num_entries, PF_FFD* p_ffd, u32* p_prev_chain, PF_STR* p_filename, u8 attr_required, u32* p_pos) {
    s32 err;

    if (p_pos) {
        *p_pos = 999999;
    }
    if (!p_ffd || !p_ffd->p_vol) {
        return 10;
    }
    if (p_filename && !p_filename->p_head) {
        return 10;
    }

    if (num_entries == 0 || num_entries > 21) {
        return 10;
    }

    return VFiPFENT_ITER_AllocateEntry(p_ent, num_entries, p_ffd, p_prev_chain, p_filename, attr_required, p_pos);
}

s32 VFiPFENT_GetRootDir(PF_VOLUME* p_vol, PF_DIR_ENT* p_ent) {
    if (!p_vol) {
        return 10;
    }
    if (!p_ent) {
        return 10;
    }
    if ((p_vol->flags & 2) == 0) {
        return 9;
    }

    p_ent->long_name[0] = 92;
    p_ent->long_name[1] = 0;

    p_ent->num_entry_LFNs = 0;
    p_ent->ordinal = 0;
    p_ent->check_sum = 0;

    p_ent->short_name[0] = 92;
    p_ent->short_name[1] = 0;
    p_ent->small_letter_flag = 0;

    p_ent->attr = 0x10;

    p_ent->create_time_ms = 0;
    p_ent->create_time = 0;
    p_ent->create_date = 0;
    p_ent->access_date = 0;
    p_ent->modify_time = 0;
    p_ent->modify_date = 0;
    p_ent->file_size = 0;

    p_ent->p_vol = p_vol;
    p_ent->path_len = 3;
    p_ent->start_cluster = 1;
    p_ent->entry_sector = -1;
    p_ent->entry_offset = 0;

    return 0;
}

s32 VFiPFENT_MakeRootDir(PF_VOLUME* p_vol) {
    s32 err;

    if (!p_vol) {
        return 10;
    }

    switch (p_vol->bpb.fat_type) {
    case FAT_32:
        err = VFiPFENT_FillVoidEntryToSectors(
            p_vol, p_vol->bpb.first_data_sector + ((p_vol->bpb.root_dir_cluster - 2) << p_vol->bpb.log2_sectors_per_cluster),
            p_vol->bpb.sectors_per_cluster, 0, 0, 0);

        if (err == 0) {
            err = VFiPFFAT_MakeRootDir(p_vol);
        }
        break;
    case FAT_12:
    case FAT_16:
        err = VFiPFENT_FillVoidEntryToSectors(p_vol, p_vol->bpb.first_root_dir_sector,
                                              p_vol->bpb.first_data_sector - p_vol->bpb.first_root_dir_sector, 0, 0, 0);
        break;
    default:
        err = 7;
        break;
    }

    if (err == 0) {
        VFiPFVOL_LoadVolumeLabelFromBuf((const u8*)"NO NAME    ", p_vol);
    }

    return err;
}

u32 VFiPFENT_CompareAttr(u8 attr, u32 attr_required) {
    u32 is_valid;
    u32 attr_extra;

    attr_extra = attr & 0x7F;
    is_valid = 0;

    if (attr_extra == 0) {
        attr_extra = 0x40;
    }

    if ((attr_extra & 0x10) == 0) {
        attr_extra |= 0x100;
    }

    if (attr_required & 0x80) {
        attr_required &= ~0x80;
        if (attr_required == (attr_extra & attr_required)) {
            is_valid = 1;
        }
    } else if (attr_required & 0x1000) {
        attr_required &= ~0x1000u;
        if ((attr_required & 0x100) == 0) {
            attr_extra &= ~0x100;
        }
        if (attr_extra == attr_required) {
            is_valid = 1;
        }
    } else if (attr_extra & attr_required) {
        is_valid = 1;
    }

    return is_valid;
}

s32 VFiPFENT_compareEntryName(PF_DIR_ENT* p_ent, PF_STR* p_pattern, u8 attr) {
    s32 is_match = 1;
    u32 is_short_search;

    if (p_ent->num_entry_LFNs != 0 && p_ent->ordinal == 1 && p_ent->check_sum == VFiPFENT_CalcCheckSum(p_ent)) {
        if (VFiPFPATH_MatchFileNameWithPattern((s8*)p_ent, p_pattern, 0, 1)) {
            is_match = 0;
        } else {
            is_match = 1;
        }
    }

    if (is_match == 1) {
        if ((VFipf_vol_set.setting & 2) == 2 && (attr & 8) == 0) {
            is_short_search = 0;
        } else {
            is_short_search = 1;
        }

        if (VFiPFPATH_MatchFileNameWithPattern(p_ent->short_name, p_pattern, is_short_search, 0)) {
            is_match = 0;
        }
    }

    return is_match;
}

void VFiPFENT_StoreEntryNumericFieldsToBuf(u8* buf, const PF_DIR_ENT* p_ent) {
    buf[11] = p_ent->attr;
    buf[12] = p_ent->small_letter_flag;
    buf[13] = p_ent->create_time_ms;

    SET_U16_LE(&buf[14], p_ent->create_time);
    SET_U16_LE(&buf[16], p_ent->create_date);
    SET_U16_LE(&buf[18], p_ent->access_date);
    SET_U16_LE(&buf[22], p_ent->modify_time);
    SET_U16_LE(&buf[24], p_ent->modify_date);

    SET_U16_LE(&buf[20], (u16)(p_ent->start_cluster >> 16));
    SET_U16_LE(&buf[26], (u16)(p_ent->start_cluster));

    SET_U32_LE(&buf[28], p_ent->file_size);
}

s32 VFiPFENT_UpdateSFNEntry(PF_DIR_ENT* p_ent, u32 flag) {
    PF_VOLUME* p_vol;
    s32 err;
    u32 success_size;
    u8 buf[32];

    if (!p_ent) {
        return 10;
    }
    if (!p_ent->p_vol) {
        return 10;
    }

    p_vol = p_ent->p_vol;

    if (p_ent->entry_sector < p_vol->bpb.first_data_sector - p_vol->bpb.num_root_dir_sectors) {
        return 28;
    }
    if (p_ent->entry_sector >= p_vol->bpb.total_sectors) {
        return 16;
    }

    if (p_ent->start_cluster == 1) {
        return 14;
    }

    if (flag) {
        p_ent->attr |= 0x20;
    }

    VFiPFENT_storeShortNameToBuf(buf, p_ent);
    VFiPFENT_StoreEntryNumericFieldsToBuf(buf, p_ent);

    err = VFiPFSEC_WriteData(p_vol, buf, p_ent->entry_sector, p_ent->entry_offset, 32, &success_size, 0, 0);

    if (err) {
        return err;
    }

    if (success_size != 32) {
        return 17;
    } else {
        return 0;
    }
}
