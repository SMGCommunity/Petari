#!/usr/bin/env python3


from argparse import ArgumentParser
from elftools.elf.constants import P_FLAGS
from elftools.elf.elffile import ELFFile
import io


def segment_is_text(segment):
    return segment['p_flags'] & P_FLAGS.PF_X == P_FLAGS.PF_X

def segment_is_data(segment):
    return not segment_is_text(segment) and not segment_is_bss(segment)

def segment_is_bss(segment):
    return segment['p_filesz'] == 0

def write_to_dol_header(file, offset, val):
    file.seek(offset)
    file.write(val.to_bytes(4, byteorder = 'big'))
    file.seek(0, io.SEEK_END)

def write_segment_to_dol(idx, segment, dol_file):
    write_to_dol_header(dol_file, 0x00 + 0x04 * idx, dol_file.tell())
    write_to_dol_header(dol_file, 0x48 + 0x04 * idx, segment['p_vaddr'])
    # align filesz to 0x20
    filesz = ((segment['p_filesz'] + 0x1F) >> 5) << 5
    write_to_dol_header(dol_file, 0x90 + 0x04 * idx, filesz)

    dol_file.write(segment.data())
    # align current dol size to 0x20
    size = 0x20 - dol_file.tell() & 0x1F
    dol_file.write(bytes([0x00] * size))

parser = ArgumentParser()
parser.add_argument('in_path')
parser.add_argument('out_path')
args = parser.parse_args()

with open(args.in_path, 'rb') as elf_file, open(args.out_path, 'wb') as dol_file:
    elf_file = ELFFile(elf_file)
    num_segments = elf_file.num_segments()

    dol_file.write(bytes([0x00] * 0x100))

    idx = 0
    for i in range(num_segments):
        segment = elf_file.get_segment(i)
        if not segment_is_text(segment):
            continue
        write_segment_to_dol(idx, segment, dol_file)
        idx += 1

    idx = 7
    for i in range(num_segments):
        segment = elf_file.get_segment(i)
        if not segment_is_data(segment):
            continue
        write_segment_to_dol(idx, segment, dol_file)
        idx += 1

    bss_start = 0
    bss_end = 0
    for i in range(num_segments):
        segment = elf_file.get_segment(i)
        if not segment_is_bss(segment):
            continue
        if bss_start == 0:
            bss_start = segment['p_vaddr']
        bss_end = segment['p_vaddr'] + segment['p_memsz']
    write_to_dol_header(dol_file, 0xD8, bss_start)
    bss_size = bss_end - bss_start
    write_to_dol_header(dol_file, 0xDC, bss_size)

    write_to_dol_header(dol_file, 0xE0, elf_file['e_entry'])
