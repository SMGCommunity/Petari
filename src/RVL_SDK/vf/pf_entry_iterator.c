#include "revolution/vf/pf_entry_iterator.h"
#include "revolution/vf/pf_cache.h"
#include "revolution/vf/pf_clib.h"
#include "revolution/vf/pf_entry.h"
#include "revolution/vf/pf_fat.h"
#include "revolution/vf/pf_path.h"
#include "revolution/vf/pf_sector.h"
#include "revolution/vf/pf_str.h"
#include "revolution/vf/pf_volume.h"
#include "revolution/vf/vf_struct.h"

extern struct PF_VOLUME_SET VFipf_vol_set;

static s32 VFiPFENT_RecalcEntryIterator(struct PF_ENT_ITER* p_iter, u32 may_allocate) {
    s32 err;
    u32 is_initialize;
    u32 file_sector_index;
    struct PF_CACHE_PAGE* p_page;
    u32 num_sectors;
    u32 success_size;

    is_initialize = p_iter->file_sector_index;
    file_sector_index = p_iter->index >> p_iter->log2_entries_per_sector;
    if (file_sector_index != is_initialize) {
        p_iter->file_sector_index = file_sector_index;
        err = VFiPFFAT_GetSectorSpecified(&p_iter->ffd, file_sector_index, 0, &p_iter->sector);
        if (err != 0) {
            return err;
        }
        if (p_iter->sector == 0xFFFFFFFF && may_allocate != 0) {
            err = VFiPFFAT_GetSectorSpecified(&p_iter->ffd, file_sector_index, 1, &p_iter->sector);
            if (err != 0) {
                return err;
            }
            if (p_iter->sector != 0xFFFFFFFF && is_initialize != 0xFFFFFFFF) {
                err = VFiPFCACHE_AllocateDataPage(p_iter->p_vol, 0xFFFFFFFF, &p_page);
                if (err != 0) {
                    return err;
                }
                VFipf_memset(p_page->p_buf, 0, p_iter->p_vol->bpb.bytes_per_sector);
                for (num_sectors = 0; num_sectors < p_iter->p_vol->bpb.sectors_per_cluster; num_sectors++) {
                    err = VFiPFSEC_WriteData(p_iter->p_vol, p_page->p_buf, p_iter->sector + num_sectors, 0, p_iter->p_vol->bpb.bytes_per_sector,
                                             &success_size, 0, 0);
                    if (err != 0) {
                        VFiPFCACHE_FreeDataPage(p_iter->p_vol, p_page);
                        return err;
                    }
                    if (success_size != p_iter->p_vol->bpb.bytes_per_sector) {
                        VFiPFCACHE_FreeDataPage(p_iter->p_vol, p_page);
                        return 0x11;
                    }
                }
                VFiPFCACHE_FreeDataPage(p_iter->p_vol, p_page);
            }
        }
    }
    p_iter->offset = (p_iter->index & p_iter->offset_mask) << 5;
    return 0;
}

static s32 VFiPFENT_ITER_LoadEntry(struct PF_ENT_ITER* p_iter) {
    u32 success_size;
    s32 err;

    if (p_iter->sector == 0xFFFFFFFF) {
        return 0x10;
    }
    err = VFiPFSEC_ReadData(p_iter->p_vol, p_iter->buf, p_iter->sector, p_iter->offset, 0x20, &success_size, 0);
    if (success_size != 0x20) {
        return 0x11;
    }
    return err;
}

static s32 VFiPFENT_ITER_DoMoveTo(struct PF_ENT_ITER* p_iter, u32 index, u32 may_allocate) {
    u32 prev_index;
    s32 err;

    prev_index = p_iter->index;
    p_iter->index = index;
    err = VFiPFENT_RecalcEntryIterator(p_iter, may_allocate);
    if (err) {
        p_iter->index = prev_index;
        VFiPFENT_RecalcEntryIterator(p_iter, 0);
    } else {
        err = VFiPFENT_ITER_LoadEntry(p_iter);
        // idk
        return err & ((-err | err) >> 31);
    }
    return err;
}

static u32 VFiPFENT_ITER_IsAtPhysicalEnd(struct PF_ENT_ITER* p_iter) {
    return p_iter->sector == 0xFFFFFFFF;
}

s32 VFiPFENT_ITER_FindCluster(struct PF_ENT_ITER* p_iter, struct PF_DIR_ENT* p_ent, u32 cluster, u32* p_is_found) {
    u8 attr;
    s32 err;
    s32 lengthName;
    s8 filename[13];

    err = 0;
    if (p_iter == NULL || p_ent == NULL) {
        return 0xA;
    }

    *p_is_found = 0;
    p_ent->num_entry_LFNs = 0;
    p_ent->ordinal = 0;
    p_ent->check_sum = 0;

    while (!VFiPFENT_ITER_IsAtLogicalEnd(p_iter)) {
        if (err != 0) {
            return err;
        }

        if (p_iter->buf[0] == 0xE5) {
            p_ent->num_entry_LFNs = 0;
            p_ent->long_name[0] = 0;
        } else {
            attr = p_iter->buf[11];
            if ((attr & 0xF) == 0xF) {
                err = VFiPFENT_LoadLFNEntryFieldsFromBuf(p_ent, p_iter->buf, 0);
                if (err != 0) {
                    p_ent->num_entry_LFNs = 0;
                    p_ent->long_name[0] = 0;
                }
            } else if ((attr & 0x8) != 0) {
                p_ent->num_entry_LFNs = 0;
                p_ent->long_name[0] = 0;
            } else if ((attr & 0x10) == 0) {
                p_ent->num_entry_LFNs = 0;
                p_ent->long_name[0] = 0;
            } else {
                if (cluster != ((((((u32)((u16*)p_iter->buf)[10] >> 8) & 0xFF) | (((u32)((u16*)p_iter->buf)[10] << 8) & 0xFF00)) << 16) |
                                ((((u32)((u16*)p_iter->buf)[13] << 8) & 0xFF00) | (((u32)((u16*)p_iter->buf)[13] >> 8) & 0xFF)))) {
                    p_ent->num_entry_LFNs = 0;
                    p_ent->long_name[0] = 0;
                } else {
                    *p_is_found = 1;
                    VFiPFENT_LoadShortNameFromBuf(p_ent, p_iter->buf);

                    if (p_ent->num_entry_LFNs != 0 && p_ent->ordinal == 1 && p_ent->check_sum == VFiPFENT_CalcCheckSum(p_ent)) {
                        VFiPFENT_loadEntryNumericFieldsFromBuf(p_ent, p_iter->buf);
                        p_ent->entry_sector = p_iter->sector;
                        p_ent->entry_offset = p_iter->offset;
                        p_ent->p_vol = p_iter->ffd.p_vol;

                        if ((p_ent->attr & 0x10) != 0 && p_ent->start_cluster != cluster) {
                            p_ent->num_entry_LFNs = 0;
                            p_ent->long_name[0] = 0;
                            goto next_entry;
                        }
                        if ((p_ent->attr & 0x10) != 0 && p_ent->start_cluster == 0) {
                            p_ent->start_cluster = 1;
                        }
                    }

                    if ((p_iter->buf[12] & 0x18) != 0) {
                        VFiPFPATH_getLongNameformShortName(p_ent->short_name, filename, p_iter->buf[12]);
                        lengthName = VFiPFPATH_transformInUnicode(p_ent->long_name, filename);
                        p_ent->num_entry_LFNs = (lengthName / 13) + ((lengthName % 13) != 0);
                        p_ent->check_sum = VFiPFENT_CalcCheckSum(p_ent);
                        p_ent->ordinal = 1;
                    }

                    VFiPFENT_loadEntryNumericFieldsFromBuf(p_ent, p_iter->buf);
                    p_ent->entry_sector = p_iter->sector;
                    p_ent->entry_offset = p_iter->offset;
                    p_ent->p_vol = p_iter->ffd.p_vol;

                    return 0;
                }
            }
        }
    next_entry:
        err = VFiPFENT_ITER_Advance(p_iter, 0);
    }

    return 0;
}

s32 VFiPFENT_ITER_DoFindEntry(struct PF_ENT_ITER* p_iter, struct PF_DIR_ENT* p_ent, struct PF_STR* p_pattern, u8 attr_required, u8 attr_unwanted,
                              u32* p_is_found, u32 is_skip) {
    u8 attr;
    s32 err;
    s32 lengthName;
    s8 filename[13];
    u32 index_search_from;
    u32 is_extsfn;

    err = 0;
    *p_is_found = 0;
    is_extsfn = 0;

    if (*p_iter->ffd.p_start_cluster == 1 &&
        (VFiPFSTR_StrNCmp(p_pattern, (const s8*)"..\\", 1, 0, 3) == 0 || VFiPFSTR_StrNCmp(p_pattern, (const s8*)"../", 1, 0, 3) == 0 ||
         VFiPFSTR_StrCmp(p_pattern, (const s8*)"..") == 0)) {
        return 2;
    } else {
        if (VFiPFSTR_StrNCmp(p_pattern, (const s8*)".\\", 1, 0, 2) == 0 || VFiPFSTR_StrCmp(p_pattern, (const s8*)".") == 0) {
            if (is_skip == 0 && (*p_iter->ffd.p_start_cluster == 1 || (p_iter->ffd.p_vol->bpb.fat_type == FAT_32 &&
                                                                       *p_iter->ffd.p_start_cluster == p_iter->ffd.p_vol->bpb.root_dir_cluster))) {
                err = VFiPFENT_GetRootDir(p_iter->p_vol, p_ent);
                if (err != 0) {
                    return err;
                }
                is_skip = 1;
            }
            if (is_skip == 1) {
                *p_is_found = 1;
                return 0;
            }
        }

        p_ent->num_entry_LFNs = 0;
        p_ent->ordinal = 0;
        p_ent->check_sum = 0;
        p_ent->long_name[0] = 0;

        if ((VFipf_vol_set.setting & 2) == 2) {
            is_extsfn = VFiPFPATH_GetExtShortNameIndex(p_pattern, &index_search_from);
        }

        if (is_extsfn == 1) {
            err = VFiPFENT_ITER_IteratorInitialize(p_iter, index_search_from - 1);

            while (1) {
                if (err) {
                    return err;
                }

                attr = p_iter->buf[11];
                if ((attr & 0xF) == 0xF) {
                    err = VFiPFENT_LoadLFNEntryFieldsFromBuf(p_ent, p_iter->buf, 0);
                    if (err != 0) {
                        p_ent->num_entry_LFNs = 0;
                        p_ent->long_name[0] = 0;
                        goto retreat;
                    }
                    if ((p_iter->buf[0] & 0x40) == 0) {
                        goto retreat;
                    }
                } else {
                    return 0;
                }

                err = VFiPFENT_ITER_IteratorInitialize(p_iter, index_search_from);
                if (err != 0) {
                    return err;
                }

                attr = p_iter->buf[11];
                if (attr == 0) {
                    attr = 0x40;
                }

                if ((attr_required & 0x80) != 0) {
                    attr_required &= 0x7F;
                    attr_unwanted &= 0x7F;
                    if ((attr_required != 0 && attr_required != (attr_required & attr)) ||
                        (attr_unwanted != 0 && attr_unwanted == (attr_unwanted & attr))) {
                        err = -1;
                    }
                } else {
                    if (attr_required != 0x7F && attr != attr_required && ((attr & attr_required) == 0 || (attr & attr_unwanted) != 0)) {
                        err = -1;
                    }
                }

                if (err == -1) {
                    return 0;
                } else if ((attr & 8) != 0) {
                    return 3;
                } else {
                    VFiPFENT_LoadShortNameFromBuf(p_ent, p_iter->buf);
                    if (!VFiPFPATH_MatchFileNameWithPattern(p_ent->short_name, p_pattern, 0, 0)) {
                        break;
                    }
                    VFiPFENT_loadEntryNumericFieldsFromBuf(p_ent, p_iter->buf);
                    p_ent->entry_sector = p_iter->sector;
                    p_ent->entry_offset = p_iter->offset;
                    p_ent->p_vol = p_iter->ffd.p_vol;

                    if ((p_ent->attr & 0x10) != 0 && p_ent->start_cluster == 0) {
                        p_ent->start_cluster = 1;
                    }
                    *p_is_found = 1;
                    return 0;
                }

            retreat:
                err = VFiPFENT_ITER_Retreat(p_iter, 0);
            }
        } else {
            while (!VFiPFENT_ITER_IsAtLogicalEnd(p_iter)) {
                if (err != 0) {
                    return err;
                }
                if (p_iter->buf[0] == 0) {
                    break;
                }
                if (p_iter->buf[0] == 0xE5) {
                    p_ent->num_entry_LFNs = 0;
                    p_ent->long_name[0] = 0;
                } else {
                    attr = p_iter->buf[11];
                    if ((attr & 0xF) == 0xF) {
                        if (VFiPFENT_LoadLFNEntryFieldsFromBuf(p_ent, p_iter->buf, 0) != 0) {
                            p_ent->num_entry_LFNs = 0;
                            p_ent->long_name[0] = 0;
                        }
                    } else {
                        if (attr == 0) {
                            attr = 0x40;
                        }

                        if ((attr_required & 0x80) != 0) {
                            attr_required &= 0x7F;
                            attr_unwanted &= 0x7F;
                            if ((attr_required != 0 && attr_required != (attr_required & attr)) ||
                                (attr_unwanted != 0 && attr_unwanted == (attr_unwanted & attr))) {
                                err = -1;
                            }
                        } else {
                            if (attr_required != 0x7F && attr != attr_required && ((attr & attr_required) == 0 || (attr & attr_unwanted) != 0)) {
                                err = -1;
                            }
                        }

                        if (err == -1) {
                            p_ent->num_entry_LFNs = 0;
                            p_ent->long_name[0] = 0;
                        } else {
                            if ((attr & 8) != 0) {
                                p_ent->num_entry_LFNs = 0;
                                p_ent->long_name[0] = 0;
                            }
                            VFiPFENT_LoadShortNameFromBuf(p_ent, p_iter->buf);

                            if (p_ent->num_entry_LFNs != 0 && p_ent->ordinal == 1 && p_ent->check_sum == VFiPFENT_CalcCheckSum(p_ent) &&
                                (err = VFiPFPATH_MatchFileNameWithPattern((const s8*)p_ent->long_name, p_pattern, 1, 0)) != 0) {
                                VFiPFENT_loadEntryNumericFieldsFromBuf(p_ent, p_iter->buf);
                                p_ent->entry_sector = p_iter->sector;
                                p_ent->entry_offset = p_iter->offset;
                                p_ent->p_vol = p_iter->ffd.p_vol;

                                if ((p_ent->attr & 0x10) != 0 && p_ent->start_cluster == 0) {
                                    p_ent->start_cluster = 1;
                                }
                                *p_is_found = 1;
                                return 0;
                            }

                            err = VFiPFPATH_MatchFileNameWithPattern(p_ent->short_name, p_pattern, 0, 0);
                            if (err != 0) {
                                if ((p_iter->buf[12] & 0x18) != 0) {
                                    VFiPFPATH_getLongNameformShortName(p_ent->short_name, filename, p_iter->buf[12]);
                                    lengthName = VFiPFPATH_transformInUnicode(p_ent->long_name, filename);
                                    p_ent->num_entry_LFNs = (lengthName / 13) + ((lengthName % 13) != 0);
                                    p_ent->check_sum = VFiPFENT_CalcCheckSum(p_ent);
                                    p_ent->ordinal = 1;
                                }
                                VFiPFENT_loadEntryNumericFieldsFromBuf(p_ent, p_iter->buf);
                                p_ent->entry_sector = p_iter->sector;
                                p_ent->entry_offset = p_iter->offset;
                                p_ent->p_vol = p_iter->ffd.p_vol;

                                if ((p_ent->attr & 0x10) != 0 && p_ent->start_cluster == 0) {
                                    p_ent->start_cluster = 1;
                                }
                                *p_is_found = 1;
                                return 0;
                            }

                            p_ent->num_entry_LFNs = 0;
                            p_ent->long_name[0] = 0;
                        }
                    }
                }
                err = VFiPFENT_ITER_Advance(p_iter, 0);
            }
        }
    }
    return 0;
}

s32 VFiPFENT_ITER_DoGetEntry(struct PF_ENT_ITER* p_iter, struct PF_DIR_ENT* p_ent, struct PF_VOLUME* p_vol, struct PF_STR* p_path, u32 wildcard,
                             u32 no_look_last_token) {
    s8* p;
    struct PF_STR token;
    u32 is_found;
    struct PF_FAT_HINT hint;
    u32 start_cluster;
    u32 name_len;
    struct PF_DIR_ENT current_ent;
    s32 err;
    u32 code_mode;

    err = VFiPFSTR_StrNCmp(p_path, (const s8*)":", 1, 1, 1);
    if (err == 0) {
        VFiPFSTR_MoveStrPos(p_path, 2);
    }

    err = VFiPFSTR_StrNCmp(p_path, (const s8*)"\\", 1, 0, 1);
    if (err == 0 || VFiPFSTR_StrNCmp(p_path, (const s8*)"/", 1, 0, 1) == 0) {
        err = VFiPFENT_GetRootDir(p_vol, p_ent);
        if (err != 0) {
            return err;
        }
        if (VFiPFSTR_StrNumChar(p_path, 1) == 1 && VFiPFSTR_StrNCmp(p_path, (const s8*)"\0\0", 2, 0, 1) == 0) {
            return 0;
        }
    } else {
        err = VFiPFVOL_GetCurrentDir(p_vol, p_ent);
        if (err != 0) {
            return err;
        }
    }

    p = VFiPFSTR_GetStrPos(p_path, 1);
    VFiPFFAT_InitFFD(&p_iter->ffd, &hint, p_vol, &p_iter->ffd.start_cluster);
    p_iter->ffd.start_cluster = p_ent->start_cluster;
    code_mode = VFiPFSTR_GetCodeMode(p_path);
    VFiPFPATH_InitTokenOfPath(&token, p, code_mode);
    err = VFiPFPATH_GetNextTokenOfPath(&token, wildcard);

    while (*(void**)&token != NULL) {
        if (err != 0) {
            return err;
        }
        if (no_look_last_token != 0 && VFiPFSTR_StrNCmp(&token, (const s8*)"\0\0", 2, 0, 1) == 0) {
            break;
        }
        if ((p_ent->attr & 0x10) == 0) {
            return 2;
        }
        if (VFiPFSTR_StrLen(&token) == 0) {
            return 2;
        }

        p_iter->ffd.start_cluster = p_ent->start_cluster;
        err = VFiPFFAT_ResetFFD(&p_iter->ffd, &p_iter->ffd.start_cluster);
        err = VFiPFENT_ITER_IteratorInitialize(p_iter, 0);
        if (err != 0) {
            return err;
        }

        start_cluster = *p_iter->ffd.p_start_cluster;
        name_len = 0;
        if ((VFiPFSTR_StrNCmp(&token, (const s8*)"..\\", 1, 0, 3) == 0 || VFiPFSTR_StrNCmp(&token, (const s8*)"../", 1, 0, 3) == 0 ||
             VFiPFSTR_StrCmp(&token, (const s8*)"..") == 0) &&
            VFipf_strcmp(p_ent->short_name, (const s8*)"..") != 0) {
            if (p_ent->num_entry_LFNs != 0) {
                name_len = VFiPFPATH_GetLengthFromUnicode(p_ent->long_name) + 1;
            } else {
                name_len = VFiPFPATH_GetLengthFromShortname(p_ent->short_name) + 1;
            }
        }

        err = VFiPFENT_ITER_DoFindEntry(p_iter, p_ent, &token, 0x7F, 0, &is_found, 1);
        if (err != 0) {
            return err;
        }

        if (is_found == 0) {
            err = VFiPFPATH_GetNextTokenOfPath(&token, wildcard);
            if (err != 0) {
                return err;
            }
            return (*(void**)&token == NULL) ? 3 : 2;
        }

        if (wildcard == 0) {
            if ((p_ent->attr & 0x10) != 0 && p_ent->start_cluster == 1) {
                p_ent->path_len = 3;
            } else if ((p_ent->attr & 0x10) != 0 && VFipf_strcmp(p_ent->short_name, (const s8*)"..") == 0) {
                if (name_len != 0) {
                    p_ent->path_len -= name_len;
                } else {
                    current_ent.start_cluster = p_ent->start_cluster;
                    err = VFiPFFAT_ResetFFD(&p_iter->ffd, &current_ent.start_cluster);
                    err = VFiPFENT_ITER_IteratorInitialize(p_iter, 0);
                    if (err != 0) {
                        return err;
                    }
                    err = VFiPFENT_ITER_FindCluster(p_iter, &current_ent, start_cluster, &is_found);
                    if (err != 0) {
                        return err;
                    }
                    if (is_found == 0) {
                        return 2;
                    }
                    if (current_ent.num_entry_LFNs != 0) {
                        p_ent->path_len -= VFiPFPATH_GetLengthFromUnicode(current_ent.long_name) + 1;
                    } else {
                        p_ent->path_len -= VFiPFPATH_GetLengthFromShortname(current_ent.short_name) + 1;
                    }
                }
            } else if ((p_ent->attr & 0x10) == 0 || p_ent->short_name[0] != '.') {
                p_ent->path_len += VFiPFSTR_StrNumChar(&token, 1) + 1;
                if (VFiPFSTR_StrNCmp(&token, (const s8*)"\0\0", 2, 0, 1) != 0) {
                    p_ent->path_len -= VFiPFSTR_StrNumChar(&token, 2);
                }
            }
        }

        err = VFiPFPATH_GetNextTokenOfPath(&token, wildcard);
    }

    return 0;
}

s32 VFiPFENT_ITER_IteratorInitialize(struct PF_ENT_ITER* p_iter, u32 index_start_from) {
    s32 err;

    p_iter->p_vol = p_iter->ffd.p_vol;
    if (p_iter->ffd.p_vol == NULL) {
        return 0xA;
    }
    p_iter->log2_entries_per_sector = p_iter->ffd.p_vol->bpb.log2_bytes_per_sector - 5;
    p_iter->offset_mask = (1 << p_iter->log2_entries_per_sector) - 1;
    p_iter->file_sector_index = 0xFFFFFFFF;
    err = VFiPFENT_ITER_DoMoveTo(p_iter, index_start_from, 0);
    return err;
}

u32 VFiPFENT_ITER_IsAtLogicalEnd(struct PF_ENT_ITER* p_iter) {
    s32 err;
    u32 save_index;
    u32 save_fsindex;

    if (p_iter->sector == 0xFFFFFFFF || p_iter->buf[0] == 0 || p_iter->index >= 999999) {
        save_index = p_iter->index;
        save_fsindex = p_iter->file_sector_index;
        p_iter->index = (p_iter->file_sector_index + 1) << p_iter->log2_entries_per_sector;
        err = VFiPFENT_RecalcEntryIterator(p_iter, 0);
        if (err != 0) {
            p_iter->index = save_index;
            p_iter->file_sector_index = save_fsindex;
            return 1;
        }
        if (p_iter->sector != 0xFFFFFFFF) {
            err = VFiPFENT_ITER_LoadEntry(p_iter);
            return (-err | (u32)err) >> 31;
        }
        return 1;
    }

    return 0;
}

static s32 VFiPFENT_ITER_MoveTo(struct PF_ENT_ITER* p_iter, u32 index, u32 may_allocate) {
    s32 err;

    if (p_iter->index != index || !VFiPFENT_ITER_IsAtPhysicalEnd(p_iter) || may_allocate == 0) {
        err = VFiPFENT_ITER_DoMoveTo(p_iter, index, may_allocate);
        if (err != 0) {
            return err;
        }
    }

    return 0;
}

s32 VFiPFENT_ITER_Advance(struct PF_ENT_ITER* p_iter, u32 may_allocate) {
    return VFiPFENT_ITER_MoveTo(p_iter, p_iter->index + 1, may_allocate);
}

s32 VFiPFENT_ITER_Retreat(struct PF_ENT_ITER* p_iter, u32 may_allocate) {
    u32 before_cluster;
    u32 cur_cluster;
    s32 err;

    if (p_iter == NULL) {
        return 0xA;
    }
    if (p_iter->index == 0) {
        return 0;
    }
    if (p_iter->file_sector_index != (p_iter->index - 1) >> p_iter->log2_entries_per_sector) {
        cur_cluster = ((p_iter->sector - p_iter->p_vol->bpb.first_data_sector) >> p_iter->p_vol->bpb.log2_sectors_per_cluster) + 2;
        if ((p_iter->file_sector_index % p_iter->p_vol->bpb.sectors_per_cluster == 0) || p_iter->p_vol->bpb.sectors_per_cluster == 1) {
            if (p_iter->p_vol->bpb.fat_type == FAT_32 ||
                (*p_iter->ffd.p_start_cluster > 1 && p_iter->p_vol->bpb.first_data_sector <= p_iter->sector)) {
                err = VFiPFFAT_GetBeforeChain(p_iter->p_vol, cur_cluster, cur_cluster, &before_cluster);
                if (err != 0) {
                    return err;
                }
                if (before_cluster == 0xFFFFFFFF) {
                    return 0xE;
                }
            } else {
                before_cluster = *p_iter->ffd.p_start_cluster;
            }
        } else {
            before_cluster = cur_cluster;
        }

        p_iter->index--;
        p_iter->file_sector_index--;

        if (p_iter->p_vol->bpb.fat_type == FAT_32 || (*p_iter->ffd.p_start_cluster > 1 && p_iter->p_vol->bpb.first_data_sector <= p_iter->sector)) {
            p_iter->sector = p_iter->p_vol->bpb.first_data_sector + ((before_cluster - 2) << p_iter->p_vol->bpb.log2_sectors_per_cluster) +
                             (p_iter->file_sector_index & (p_iter->p_vol->bpb.sectors_per_cluster - 1));
        } else {
            p_iter->sector--;
        }

        p_iter->offset = (p_iter->index & p_iter->offset_mask) << 5;
        return VFiPFENT_ITER_MoveTo(p_iter, p_iter->index, may_allocate);
    } else {
        return VFiPFENT_ITER_MoveTo(p_iter, p_iter->index - 1, may_allocate);
    }
}

s32 VFiPFENT_ITER_GetEntryOfPath(struct PF_ENT_ITER* p_iter, struct PF_DIR_ENT* p_ent, struct PF_VOLUME* p_vol, struct PF_STR* p_path,
                                 u32 no_look_last_token) {
    return VFiPFENT_ITER_DoGetEntry(p_iter, p_ent, p_vol, p_path, 0, no_look_last_token);
}
