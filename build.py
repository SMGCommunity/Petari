import glob
import subprocess
import sys
import os
from pathlib import Path

flags = "-i . -I- -i include -nostdinc -Cpp_exceptions off -O4 -proc gekko -fp hard -enum int -sdata 0 -sdata2 0 -g"
as_flags = "-i . -I- -nostdinc -proc gekko -d __MWERKS__"

if not os.path.isdir("tools"):
    print("tool directory not found")
    sys.exit(1)

if not os.path.isdir("build"):
    os.mkdir("build")

path = os.path.dirname(os.path.realpath(__file__)) + "\\source\\"

c_files = [f for f in glob.glob(path + "**/*.c", recursive=True)]
cpp_files = [f for f in glob.glob(path + "**/*.cpp", recursive=True)]
assembly_files = [f for f in glob.glob(path + "**/*.s", recursive=True)]

for f in cpp_files:
    file_name = Path(f).stem
	
    print(f"Compiling {file_name}.cpp...")

    if subprocess.call(f"mwcceppc.exe {flags} -c -o build/{file_name}.o {f}", shell=True) == 1:
        sys.exit(1)

for f in c_files:
    file_name = Path(f).stem
	
    print(f"Compiling {file_name}.c...")

    if subprocess.call(f"mwcceppc.exe {flags} -c -o build/{file_name}.o {f}", shell=True) == 1:
        sys.exit(1)

for f in assembly_files:
    file_name = Path(f).stem
	
    print(f"Assembling {file_name}.s...")

    if subprocess.call(f"mwasmeppc.exe {as_flags} -o build/{file_name}.o {f}", shell=True) == 1:
        sys.exit(1)