from get_symbol import get_symbol

SYMBOLS_PATH = "config/RMGE01/symbols.txt"
GHIDRA_SYMBOLS_PATH = "ghidra_symbols.txt"


with open(SYMBOLS_PATH, "r") as rf, open(GHIDRA_SYMBOLS_PATH, "w") as wf:
    for line in rf:
        if (symbol := get_symbol(line)) is None:
            continue

        symbol_name = symbol.group("symbol")
        symbol_section = symbol.group("section")
        symbol_address = symbol.group("address")
        symbol_type = symbol.group("type")
        symbol_size = symbol.group("size")
        symbol_other = symbol.group("other")

        symbol_address = int(symbol_address, 16)

        if "0x" in symbol_size:
            symbol_size = int(symbol_size, 16)
        else:
            symbol_size = int(symbol_size)

        if symbol_section == ".text":
            wf.write(f"{symbol_name} 0x{symbol_address:08X} f\n")
        elif symbol_section == ".init":
            wf.write(f"{symbol_name} 0x{symbol_address:08X} f\n")
        else:
            wf.write(f"{symbol_name} 0x{symbol_address:08X} l\n")
