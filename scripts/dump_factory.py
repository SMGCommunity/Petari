# dump_factory.py
# dumps the NameObjFactory structure entries, result can be seen in Game/NameObj/NameObjFactory.h

output = []

START_ADDR = 0x80534F00
END_ADDR = 0x80538674
ENTRY_SIZE = 0xC

for i in range(START_ADDR, END_ADDR, ENTRY_SIZE):
    obj_name = get_strlit_contents(ida_bytes.get_dword(i)).decode("utf-8")
    other_name = get_strlit_contents(ida_bytes.get_dword(i + 0x8))
    
    if other_name == None:
        string = '\t\t{ "' + obj_name + '", NULL, 0 },\n'
    else:
        other_name = other_name.decode("utf-8")
        string = '\t\t{ "' + obj_name + '", NULL, "' + other_name + '" },\n'

    output.append(string)
    
with open("output.h", "w") as w:
    w.writelines(output)
   