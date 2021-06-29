import glob
import subprocess
import sys
import os
from pathlib import Path

def getPath(var):
    with open("sources.txt", "r") as f:
        lines = f.readlines()

    for line in lines:
        if line.split("=")[0] == var:
            return line.split("=")[1].strip("\n\r")
    
    return ""

def isCmdAvailable(cmd):
    return True

def deleteDFiles():
    dirs = os.listdir(os.getcwd())

    for dire in dirs:
        if dire.endswith(".d"):
            os.remove(os.path.join(os.getcwd(), dire))

numFilesCompiled = 0

SDK_ROOT_PATH = getPath("SDK_PATH")

if SDK_ROOT_PATH == "":
    print("SDK_PATH variable not defined.")
    sys.exit(1)

SDK_BASE_PATH = f"{SDK_ROOT_PATH}/RVL_SDK/RVL"
SDK_LIB_PATH = f"{SDK_BASE_PATH}/lib"
SDK_INC_PATH = f"{SDK_ROOT_PATH}/RVL_SDK/include"

NW_BASE_PATH = f"{SDK_ROOT_PATH}/NintendoWare/Revolution/Library"
NW_INC_PATH = f"{NW_BASE_PATH}/include"
NW_LIB_PATH = f"{NW_BASE_PATH}/lib/RVL/Release"

MW_BASE_PATH = getPath("MW_BASE_PATH")

if MW_BASE_PATH == "":
    print("MW_BASE_PATH variable not defined.")
    sys.exit(1)

MW_BIN_PATH = f"{MW_BASE_PATH}/PowerPC_EABI_TOOLS/Command_Line_Tools"
MW_LIB_PATH = f"{MW_BASE_PATH}/PowerPC_EABI_Support"
MW_TOOLS_PATH = f"{MW_BASE_PATH}/PowerPC_EABI_Tools/Command_Line_Tools"

MW_INC =    [
                f"-ir {MW_LIB_PATH}",
                f"-ir {MW_LIB_PATH}/Runtime/Include",
                f"-ir {MW_LIB_PATH}/MSL/MSL_C/MSL_Common/Include",
                f"-ir {SDK_INC_PATH}",
                f"-ir {NW_INC_PATH}"
            ]

rootPath = os.path.dirname(os.path.realpath(__file__))
path = os.path.dirname(os.path.realpath(__file__)) + "\\source\\"
asm_path = os.path.dirname(os.path.realpath(__file__)) + "\\asm\\"

gc_compiler_path = os.path.dirname(os.path.realpath(__file__)) + "\\compiler\\GC\\1.2.5\\"

flags = "-nodefaults -proc gekko -DRELEASE -Cpp_exceptions off -gccinc -O4,s -fp hardware -enum int -sdata 4 -sdata2 4 -lang=c99 -align powerpc -inline all,auto -W noimplicitconv -DEPPC -DHOLLYWOOD_REV -DTRK_INTEGRATION -DGEKKO -DMTX_USE_PS -D_MSL_USING_MW_C_HEADERS -MMD -rtti off -ipa file "
nw4r_flags = "-nodefaults -proc gekko -DRELEASE -Cpp_exceptions off -gccinc -O4,p -fp hardware -enum int -sdata 4 -sdata2 4 -lang=c99 -align powerpc -inline all,auto -W noimplicitconv -DEPPC -DHOLLYWOOD_REV -DTRK_INTEGRATION -DGEKKO -DMTX_USE_PS -D_MSL_USING_MW_C_HEADERS -MMD -rtti off -ipa file "
includes = "-i . -I- -i include "
gc_flags = f"-Cpp_exceptions off -proc gekko -fp hard -O4 -nodefaults -msgstyle gcc -enum int -D__USING_IEEE_MATH__ "

for inc in MW_INC:
    includes += f"{inc} "

flags += f"{includes} -nosyspath"
nw4r_flags += f"{includes} -nosyspath"
gc_flags += f"{includes} -nosyspath"

as_flags = "-c -i . -I- -i include -proc gekko -d __MWERKS__"

req_commands = ['mwcceppc', 'mwasmeppc', 'mwldeppc']

# first check our tools folder
if not os.path.isdir("tools"):
    # don't have it? let's check $PATH
    for cmd in req_commands:
        if isCmdAvailable(cmd):
            continue
        else:
             print(f"Command {cmd} is not available!")
             sys.exit(1)

if not os.path.isdir("build"):
    os.mkdir("build")

c_files = [f for f in glob.glob(path + "**/*.c", recursive=True)]
cpp_files = [f for f in glob.glob(path + "**/*.cpp", recursive=True)]
assembly_files = [f for f in glob.glob(asm_path + "**/*.s", recursive=True)]

for f in cpp_files:
    file_name = Path(f).stem

    print(f"Compiling {file_name}.cpp...")

    compilerFlags = ""

    if "nw4r" in f:
        compilerFlags = nw4r_flags
    else:
        compilerFlags = flags

    if subprocess.call(f"{MW_TOOLS_PATH}/mwcceppc.exe {compilerFlags} -o build/{file_name}.o {f}", shell=True) == 1:
            deleteDFiles()
            sys.exit(1)

for f in c_files:
    file_name = Path(f).stem
    
    print(f"Compiling {file_name}.c...")

    if subprocess.call(f"{MW_TOOLS_PATH}/mwcceppc.exe {flags} -o build/{file_name}.o {f}", shell=True) == 1:
        deleteDFiles()
        sys.exit(1)

for f in assembly_files:
    file_name = Path(f).stem

    print(f"Assembling {file_name}.s...")

    if subprocess.call(f"{MW_TOOLS_PATH}/mwasmeppc.exe {as_flags} -o build/{file_name}.o {f}", shell=True) == 1:
        deleteDFiles()
        sys.exit(1)

deleteDFiles()

print("Complete.")