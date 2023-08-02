# dump_stationed.py
# dumps tables from IDA when given two addresses, a entry size, and will output the table in C++ syntax

output = []

START_ADDR = 0x8053BBA8
END_ADDR = 0x8053C48C
ENTRY_SIZE = 0xC

for i in range(START_ADDR, END_ADDR, ENTRY_SIZE):
    # each entry contains 6 elements, the object name, the start SE, moving SE, ending SE, the number of steps, and the sound effect type
    heap_type = ida_bytes.get_dword(i)
    unk = ida_bytes.get_dword(i + 0x4)
    path_addr = ida_bytes.get_dword(i + 0x8)

    
    # if our addresses are 0 then we just write "0", otherwise, we get the string contents
    if path_addr == 0:
        path_str = 0
    else:
        path_str = f"\"{get_strlit_contents(path_addr).decode('utf-8')}\""
    
    # build our C++ line
    line = f"{{ {heap_type}, {unk}, {path_str}}},\n"
    output.append(line)
    
# export header
with open("output.h", "w") as w:
    w.writelines(output)