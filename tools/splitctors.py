# splitctors.py -- shibboleet
# takes the blob of ctors.s and splits it into labels for each sinit

with open("asm/ctors.s", "r") as f:
    lines = f.readlines()

output = []

starting_lbl = ""
starting_addr = 0
size = 0

# there are really only two lines we have to iterate through
for line in lines:
    # skip, dont care
    if ".section" in line:
        continue

    if ".global" in line:
        starting_lbl = line.strip()

    if ".incbin" in line:
        new_line = line.strip()

        split = new_line.split(", ")
        starting_addr = split[1]
        size = int(split[2], 16)

# now we can make our new list!

# each ctor ptr is 4 bytes
# we subtract 0x20 because there are 0x20 bytes of padding after the last ctor ptr
num_ctors = (size - 0x20) // 0x4
base_data_addr = 0x529380

for i in range(num_ctors):
    # 0x8052D280 is the base address for ctors
    cur_addr = (i * 4) + 0x8052D280
    cur_base_data_addr = (i * 4) + base_data_addr
    cur_addr_lbl = hex(cur_addr)[2:].upper()
    print(cur_addr_lbl)

    global_lbl = f".global lbl_{cur_addr_lbl}\n"
    reg_lbl = f"lbl_{cur_addr_lbl}:\n"
    inc_bin = f"\t.incbin \"baserom.dol\", {hex(cur_base_data_addr)}, 0x4\n"

    output.append(global_lbl)
    output.append(reg_lbl)
    output.append(inc_bin)
    output.append("\n")

with open("ctors_new.s", "w") as w:
    w.write(".section .ctors\n\n")
    for o in output:
        w.write(o)

    for i in range(7):
        w.write(".long 0\n")