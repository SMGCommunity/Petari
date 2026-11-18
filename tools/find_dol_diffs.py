from dataclasses import dataclass
import struct
from typing import BinaryIO

from get_symbol import get_symbol

TEXT_SECTIONS = 7
DATA_SECTIONS = 11

base_dol_path = "orig/RMGK01/sys/main.dol"
built_dol_path = "build/RMGK01/main.dol"
symbols_path = "config/RMGK01/symbols.txt"

TEXT_SECTION_NAMES = [".init", ".text", "", "", "", "", ""]
DATA_SECTION_NAMES = ["extab", "extabindex", ".ctors", ".dtors", ".rodata", ".data", ".bss", ".sdata", ".sbss", ".sdata2", ".sbss2"]


def read_u32(stream: BinaryIO) -> int:
    return struct.unpack(">I", stream.read(4))[0]


def read_u32_list(stream: BinaryIO, count: int) -> list[int]:
    return [read_u32(stream) for _ in range(count)]


@dataclass
class Section:
    name: str
    offset: int
    address: int
    size: int


def get_sections(stream: BinaryIO) -> list[Section]:
    names = TEXT_SECTION_NAMES + DATA_SECTION_NAMES
    offsets = read_u32_list(stream, TEXT_SECTIONS + DATA_SECTIONS)
    addresses = read_u32_list(stream, TEXT_SECTIONS + DATA_SECTIONS)
    sizes = read_u32_list(stream, TEXT_SECTIONS + DATA_SECTIONS)
    
    return [Section(name, offset, address, size)
            for name, offset, address, size in
            zip(names, offsets, addresses, sizes)]


def get_diff(base_stream: BinaryIO, built_stream: BinaryIO, base_section: Section, built_section: Section, threshold: int = 4, max_diffs: int = -1) -> list[int]:
    base_stream.seek(base_section.offset)
    built_stream.seek(built_section.offset)

    diffs = []
    for i, base_byte, built_byte in zip(range(base_section.size), base_stream.read(base_section.size), built_stream.read(built_section.size)):
        if base_byte != built_byte:
            diff += 1
            if diff != threshold:
                continue

            diffs.append(base_section.address + (i + 1) - diff)
            if len(diffs) == max_diffs:
                return diffs
            
        diff = 0
    return diffs


def find_symbols(path: str, addresses: list[int]) -> list[str]:
    symbols = []
    i = 0
    with open(path, "r") as f:
        for line in f:
            if (symbol := get_symbol(line)) is None:
                continue

            symbol_address = symbol.group("address")
            symbol_size = symbol.group("size")
            
            symbol_address = int(symbol_address, 16)

            if "0x" in symbol_size:
                symbol_size = int(symbol_size, 16)
            else:
                symbol_size = int(symbol_size)
            
            if symbol_address <= addresses[i] < symbol_address + symbol_size:
                i += 1
                symbols.append(symbol.group("symbol"))
                
                if i == len(addresses):
                    return symbols
    return symbols


with open(base_dol_path, "rb") as base_dol, open(built_dol_path, "rb") as built_dol:
    base_sections = get_sections(base_dol)
    built_sections = get_sections(built_dol)

    for base_section, built_section in zip(base_sections, built_sections):
        if (diff_addresses := get_diff(base_dol, built_dol, base_section, built_section, 1)) == []:
            continue

        if (symbols := find_symbols(symbols_path, diff_addresses)) == []:
            continue
        
        for symbol, diff_address in zip(symbols, diff_addresses):
            print(f"Difference at address 0x{diff_address:08X} in symbol {symbol}")
