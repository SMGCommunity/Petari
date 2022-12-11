# dump_table.py
# dumps tables from IDA when given two addresses, a entry size, and will output the table in C++ syntax

output = []

START_ADDR = 0x80533D90
END_ADDR = 0x80534C18
ENTRY_SIZE = 0x18

for i in range(START_ADDR, END_ADDR, ENTRY_SIZE):
    # each entry contains 6 elements, the object name, the start SE, moving SE, ending SE, the number of steps, and the sound effect type
    obj_name_addr = ida_bytes.get_dword(i)
    start_se_addr = ida_bytes.get_dword(i + 0x4)
    moving_se_addr = ida_bytes.get_dword(i + 0x8)
    stop_se_addr = ida_bytes.get_dword(i + 0xC)
    steps = ida_bytes.get_dword(i + 0x10)
    se_type = ida_bytes.get_dword(i + 0x14)
    
    # if our addresses are 0 then we just write "0", otherwise, we get the string contents
    if obj_name_addr == 0:
        obj_name_str = 0
    else:
        obj_name_str = f"\"{get_strlit_contents(obj_name_addr).decode('utf-8')}\""
    
    if start_se_addr == 0:
        start_se_str = 0
    else:
        start_se_str = f"\"{get_strlit_contents(start_se_addr).decode('utf-8')}\""
        
    if moving_se_addr == 0:
        moving_se_str = 0
    else:
        moving_se_str = f"\"{get_strlit_contents(moving_se_addr).decode('utf-8')}\""
        
    if stop_se_addr == 0:
        stop_se_str = 0
    else:
        stop_se_str = f"\"{get_strlit_contents(stop_se_addr).decode('utf-8')}\""
        
    # build our C++ line
    line = f"{{ {obj_name_str}, {start_se_str}, {moving_se_str}, {stop_se_str}, {hex(steps)}, {se_type} }},\n"
    output.append(line)
    
# export header
with open("output.h", "w") as w:
    w.writelines(output)