#include "revolution/vf/pf_entry.h"
#include "revolution/vf/pf_clib.h"
#include "revolution/vf/pf_entry_iterator.h"
#include "revolution/vf/pf_fat.h"

#include "revolution/vf/pf_path.h"

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
    p_start_cluster = p_ffd->p_start_cluster;
    iter.ffd.start_cluster = p_ffd->start_cluster;
    iter.ffd.p_start_cluster = p_start_cluster;
    max_chain_index = p_ffd->last_cluster.max_chain_index;
    iter.ffd.last_cluster.num_last_cluster = p_ffd->last_cluster.num_last_cluster;
    iter.ffd.last_cluster.max_chain_index = max_chain_index;
    cluster = p_ffd->last_access.cluster;
    iter.ffd.last_access.chain_index = p_ffd->last_access.chain_index;
    iter.ffd.last_access.cluster = cluster;
    v11 = p_ffd->cluster_link.interval;
    iter.ffd.cluster_link.buffer = p_ffd->cluster_link.buffer;
    iter.ffd.cluster_link.interval = v11;
    max_count = p_ffd->cluster_link.max_count;
    iter.ffd.cluster_link.position = p_ffd->cluster_link.position;
    iter.ffd.cluster_link.max_count = max_count;
    p_hint = p_ffd->p_hint;
    iter.ffd.cluster_link.save_index = p_ffd->cluster_link.save_index;
    iter.ffd.p_hint = p_hint;
    iter.ffd.p_vol = p_ffd->p_vol;
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
                    while (i)
                        bit_pos = sfnbuf[sfn_baselen - i--] + 10 * bit_pos - 48;
                    VFipf_strcpy(patbuf, pattern);
                    VFiPFPATH_parseShortNameNumeric(patbuf, bit_pos);
                    if (!VFiPFPATH_cmpTailSFN(sfnbuf, patbuf) && bit_pos >= tail_index &&
                        bit_pos < tail_index + 32 * p_vol->tail_entry.tracker_size) {
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
    p_ent->create_time = ((buf[7] << 8) | buf[6]);
    p_ent->create_date = ((buf[8] << 8) | buf[9]);
    p_ent->access_date = ((buf[9] << 8) | buf[10]);
    p_ent->modify_time = ((buf[11] << 8) | buf[12]);
    p_ent->modify_date = ((buf[12] << 8) | buf[13]);
    p_ent->file_size = ((buf[7] << 24) | (buf[8] << 16) | (buf[9] << 8) | buf[10]);
    p_ent->start_cluster = ((buf[10] << 24) | (buf[11] << 16) | (buf[12] << 8) | buf[13]);
}
