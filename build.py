import glob
import subprocess
import sys
import os
from pathlib import Path

def deleteDFiles():
    dirs = os.listdir(os.getcwd())

    for dire in dirs:
        if dire.endswith(".d"):
            os.remove(os.path.join(os.getcwd(), dire))

rootPath = os.path.dirname(os.path.realpath(__file__))
path = os.path.dirname(os.path.realpath(__file__)) + "\\source\\"
toolsPath = os.path.dirname(os.path.realpath(__file__)) + "\\tools\\"

flags = "-c -Cpp_exceptions off -proc gekko -fp hard -O4,p -rtti off -nodefaults -msgstyle gcc -i . -I- -i include "

if not os.path.isdir("build"):
    os.mkdir("build")

c_files = [f for f in glob.glob(path + "**/*.c", recursive=True)]
cpp_files = [f for f in glob.glob(path + "**/*.cpp", recursive=True)]

for f in cpp_files:
    file_name = Path(f).stem

    print(f"Compiling {file_name}.cpp...")

    if subprocess.call(f"{toolsPath}/mwcceppc.exe {flags} -o build/{file_name}.o {f}", shell=True) == 1:
            deleteDFiles()
            sys.exit(1)
    

for f in c_files:
    file_name = Path(f).stem
    
    print(f"Compiling {file_name}.c...")

    if subprocess.call(f"{toolsPath}/mwcceppc.exe {flags} -o build/{file_name}.o {f}", shell=True) == 1:
        deleteDFiles()
        sys.exit(1)

deleteDFiles()

print("Complete.")