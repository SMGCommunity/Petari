import sys
#from colored import fg

def findAddrFromSym(sym):
    with open("data/map_for_dol.map", "r") as f:
        lines = f.readlines()

        for line in lines:
            new_line = line.strip("\n")
            spl = new_line.split("=")

            if spl[1].split(":")[0] == sym:
                return spl[0].split(" ")[4]

    return 0

def findAddrSizeFromSym(sym):
    with open("data/map_for_dol.map", "r") as f:
        lines = f.readlines()

        for line in lines:
            new_line = line.strip("\n")
            spl = new_line.split("=")

            if spl[1].split(":")[0] == sym:
                return spl[0].split(" ")[3]

    return 0

def markAsDecompiled(lib, sym):
    with open(f"csv/{lib}.csv", "r") as f:
        lines = f.readlines()

    output = []

    for line in lines:
        newLine = line.strip("\n\r")
        spl = newLine.split(",")
        
        if spl[0] == sym:
            #spl[4] = auth

            #if spl[3] == 'true':
                #color = fg('red')
                #print(f"{color} Function is already marked as decompiled!")
                # reset color
                #print("\033[0;0m")
                #sys.exit(1)
            spl[3] = 'true'
            found = True

        output.append(f"{spl[0]},{spl[1]},{spl[2]},{spl[3]}\n")

    if found:
        with open(f"csv/{lib}.csv", "w") as w:
            w.writelines(output)
    else:
        print(f"Symbol {sym} not found!")

def unmarkAsDecompiled(lib, sym):
    with open(f"csv/{lib}.csv", "r") as f:
        lines = f.readlines()

    output = []

    for line in lines:
        newLine = line.strip("\n\r")
        spl = newLine.split(",")
        
        if spl[0] == sym:
            #spl[4] = auth

            if spl[3] == 'true':
                spl[3] = 'false'
            
            found = True

        output.append(f"{spl[0]},{spl[1]},{spl[2]},{spl[3]}\n")

    if found:
        #color = fg('green')
        #print(f"{color} Function {sym} has been marked as decompiled!")
        # reset color
        #print("\033[0;0m")

        with open(f"csv/{lib}.csv", "w") as w:
            w.writelines(output)
    else:
        print(f"Symbol {sym} not found!")

def checkSymForMarked(lib, sym):
    with open(f"csv/{lib}.csv", "r") as f:
        lines = f.readlines()

    output = []

    for line in lines:
        newLine = line.strip("\n\r")
        spl = newLine.split(",")
        
        if spl[0] == sym:
            if spl[3] == 'true':
                return True

    # if we got here then it's not checked
    return False