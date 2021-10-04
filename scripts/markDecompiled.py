import sys

if len(sys.argv) < 4:
    print("Syntax: markDecompiled.py library mangled_symbol author")
    sys.exit(1)

lib = sys.argv[1]
sym = sys.argv[2]
auth = sys.argv[3]

with open(f"csv/{lib}.csv", "r") as f:
    lines = f.readlines()

output = []

for line in lines:
    newLine = line.strip("\n\r")
    spl = newLine.split(",")
    
    if spl[0] == sym:
        spl[4] = auth
        spl[3] = 'true'

    output.append(f"{spl[0]},{spl[1]},{spl[2]},{spl[3]},{spl[4]}\n")

with open(f"csv/{lib}.csv", "w") as w:
    w.writelines(output)

print(f"Function {sym} has been marked as decompiled!")