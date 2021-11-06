# checkObject.py
# checks a specific object for a specific function's completion, or an entire object's

import sys
from colored import fg

if len(sys.argv) < 3:
    print("Syntax: markDecompiled.py library object [mangled_symbol]")
    sys.exit(1)

lib = sys.argv[1]

if len(sys.argv) == 4:
    sym = sys.argv[3].replace(",", "&#44;").strip("\n\r")
else: 
    sym = ""

obj = sys.argv[2]

countInObj = 0.0
doneInObj = 0.0

with open(f"csv/{lib}.csv", "r") as f:
    lines = f.readlines()

for line in lines:
    newLine = line.strip("\n\r")
    spl = newLine.split(",")
    
    if spl[0] == sym and sym != "":
        status = "nonmatching"

        if spl[3] == 'true':
            status = "matching"

        print(f"Status for {sym} = {status}")
        sys.exit(1)
    else:
        # skip symbol
        if sym != "":
            continue

        status = "nonmatching"
            
        if spl[1] == obj:
            countInObj += 1

            if spl[3] == 'true':
                doneInObj += 1
                status = "matching"

            print(f"Function {spl[0]} status: {status}")

if doneInObj / countInObj == 1:
    print("Object is fully matching!")
else:
    print(f"Progress for {obj}: {doneInObj / countInObj}%")