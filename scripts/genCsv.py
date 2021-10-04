# genCsv.py -- shibboleet
# generates csv files based on a specific input file

prevObj = ""
prevArch = ""

outputFile = []

with open("KOREAN_MAP_FOR_GIT.txt", "r") as f:
    lines = f.readlines()
    
for line in lines:
    lineFix = line.strip("\n\r")
    lineSpl = lineFix.split("=")
    
    # some symbols have commas in them
    sym = lineSpl[1].split(":")[0].replace(",", "&#44;")
    lib = lineSpl[1].split("~")[0].split(":")[1]
    obj = lineSpl[1].split("~")[1]
    
    # new lib, new list
    if prevArch != lib:
        # save our old one before wiping
        
        if prevArch == "":
            prevArch = lib

        with open(f"archives/{prevArch[:-2]}.csv", "w") as w:
            w.writelines(outputFile)
    
        outputFile.clear()
        outputFile.append("Symbol Name, Object File, Library Archive, Matching, Author\n")
        prevArch = lib
    
    if prevObj != obj:
        prevObj = obj

    outputFile.append(f"{sym},{obj},{lib},false,\n")