# check.py
# unfinished script that will probably never see the light of day

from elftools.elf.elffile import ELFFile
import glob
import sys

if len(sys.argv) < 3:
    print("Syntax: check.py lib mangled_sym")
    sys.exit(1)

lib = sys.argv[1]
sym = sys.argv[2].replace(",", "&#44;").strip("\n\r")

if lib == "TRK_Hollywood_Revolution":
    print("this library is not supported at the moment")
    sys.exit(1)

with open(f"csv/{lib}.csv", "r") as f:
    lines = f.readlines()

obj = ""

# now we find the object file this is associated with
for line in lines:
    newLine = line.strip("\n\r")
    spl = newLine.split(",")

    if spl[0] == sym:
        obj = spl[1]

if obj == "":
    print("Could not find object from symbol.")
    sys.exit(1)

files = glob.glob(f"build/**/{obj}", recursive=True)

if len(files) > 1:
    print("There are multiple .o files for the same lib...this should not happen")
    sys.exit(1)

if len(files) == 0:
    print("There are no .o files for this specific library.")
    sys.exit(1)

obj_file = files[0]

with open(obj_file, "rb") as f:
    elf_file = ELFFile(f) 
    symtab = elf_file.get_section_by_name('.symtab')

    symbol = symtab.get_symbol_by_name(sym)[0]
    print(symbol['st_value'])