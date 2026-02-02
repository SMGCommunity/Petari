#include "revolution/vf/pf_entry.h"
#include "revolution/vf/pf_clib.h"
#include "revolution/vf/pf_entry_iterator.h"
#include "revolution/vf/pf_path.h"

static void VFiPFENT_storeShortNameToBuf(u8* buf, const struct PF_DIR_ENT* p_ent) {
    VFiPFPATH_putShortName(buf, p_ent->short_name, p_ent->attr);

    if (*buf == 0xE5) {
        *buf = 0x05;
    }
}

s32 VFiPFENT_searchEmptyTailSFN(PF_FFD* p_ffd, u32 tail_index, const s8* pattern, u32* p_tail_bit) {
    PF_ENT_ITER iter;
    s8 attr;
    s32 err;
    u32 bit_pos;
    u32 sfn_taillen;
    u32 sfn_baselen;
    u32 i;
    s8 sfnbuf[13];
    s8 patbuf[13];
    PF_VOLUME* p_vol;

    p_vol = p_ffd->p_vol;

    VFipf_memset(p_tail_bit, 0, p_vol->tail_entry.tracker_size * sizeof(u32));

    iter.ffd = *p_ffd;

    for (err = VFiPFENT_ITER_IteratorInitialize(&iter, 0); !VFiPFENT_ITER_IsAtLogicalEnd(&iter); err = VFiPFENT_ITER_Advance(&iter, 0)) {
        if (err != 0) {
            return err;
        }

        if (iter.buf[0] == 0) {
            break;
        }

        if (iter.buf[0] != 0xE5 && (iter.buf[11] & 0x0F) != 0x0F && (iter.buf[11] & 0x08) == 0) {
            VFiPFPATH_getShortName(sfnbuf, iter.buf, 0);

            for (sfn_baselen = 1; sfnbuf[sfn_baselen] != '~' && sfnbuf[sfn_baselen] != 0 && sfn_baselen < 7; ++sfn_baselen)
                ;

            if (sfn_baselen < 7 && sfnbuf[sfn_baselen] == '~') {
                for (sfn_taillen = sfn_baselen + 1; sfnbuf[sfn_taillen] >= '0' && sfnbuf[sfn_taillen] <= '9'; ++sfn_taillen)
                    ;

                if (sfnbuf[sfn_taillen] == '.' || sfnbuf[sfn_taillen] == 0) {
                    i = sfn_taillen - sfn_baselen - 1;
                    bit_pos = 0;

                    while (i > 0) {
                        bit_pos = (sfnbuf[sfn_taillen - i] - '0') + (10 * bit_pos);
                        i--;
                    }

                    VFipf_strcpy(patbuf, pattern);
                    VFiPFPATH_parseShortNameNumeric(patbuf, bit_pos);

                    if (VFiPFPATH_cmpTailSFN(sfnbuf, patbuf) == 0) {
                        if (bit_pos >= tail_index && bit_pos < (tail_index + (32 * p_vol->tail_entry.tracker_size))) {
                            p_tail_bit[(bit_pos - tail_index) >> 5] |= (1 << (bit_pos - tail_index));
                        }
                    }
                }
            }
        }
    }

    return 0;
}
