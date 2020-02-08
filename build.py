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
    test_cmd = "which" if platform.system() != "Windows" else "where"

    try:
        rc = subprocess.call([test_cmd, cmd], stdout=open(os.devnull, 'wb'), stderr=open(os.devnull, 'wb'))
        return rc == 0
    except:
        return False

numFilesCompiled = 0

SDK_ROOT_PATH = getPath("SDK_PATH")

if SDK_ROOT_PATH == "":
    print("SDK_PATH variable not defined.")
    sys.exit(1)

SDK_BASE_PATH = f"{SDK_ROOT_PATH}/RVL_SDK/RVL"
SDK_LIB_PATH = f"{SDK_BASE_PATH}/RVL/lib"
SDK_INC_PATH = f"{SDK_ROOT_PATH}/RVL_SDK/include"

NW_BASE_PATH = f"{SDK_ROOT_PATH}/NintendoWare/Revolution/Library"
NW_INC_PATH = f"{NW_BASE_PATH}/include"
NW_LIB_PATH = f"{NW_BASE_PATH}/lib/RVL/Release/"

MW_BASE_PATH = getPath("MW_BASE_PATH")

if MW_BASE_PATH == "":
    print("MW_BASE_PATH variable not defined.")
    sys.exit(1)

MW_BIN_PATH = f"{MW_BASE_PATH}/PowerPC_EABI_TOOLS/Command_Line_Tools"
MW_LIB_PATH = f"{MW_BASE_PATH}/PowerPC_EABI_Support"
MW_TOOLS_PATH = f"{MW_BASE_PATH}/PowerPC_EABI_Tools/Command_Line_Tools"

MW_LIBS =   [
                f"{MW_LIB_PATH}/MSL/MSL_C/PPC_EABI/Lib/MSL_C.PPCEABI.bare.H.a",
                f"{MW_LIB_PATH}/MetroTRK/TRK_Hollywood_Revolution.a",
                f"{MW_LIB_PATH}/Runtime/Lib/Runtime.PPCEABI.H.a"
            ]

SDK_LIBS =  [
                f"{SDK_ROOT_PATH}/NDEV/lib/NdevExi2A.a"
            ]
    
NW_LIBS =   [
                f"{NW_LIB_PATH}/libnw4r_db.a",
                f"{NW_LIB_PATH}/libnw4r_dw.a",
                f"{NW_LIB_PATH}/libnw4r_ef.a",
                f"{NW_LIB_PATH}/libnw4r_g3d.a",
                f"{NW_LIB_PATH}/libnw4r_lyt.a",
                f"{NW_LIB_PATH}/libnw4r_math.a",
                f"{NW_LIB_PATH}/libnw4r_mcs",
                f"{NW_LIB_PATH}/libnw4r_snd.a",
                f"{NW_LIB_PATH}/libnw4r_ut.a"
            ]

MW_INC =    [
                f"-ir {MW_LIB_PATH}/MetroTRK",
                f"-ir {MW_LIB_PATH}/Runtime/Include",
                f"-ir {MW_LIB_PATH}/MSL/MSL_C",
                f"-ir {SDK_INC_PATH}",
                f"-ir {NW_INC_PATH}"
            ]

rootPath = os.path.dirname(os.path.realpath(__file__))
path = os.path.dirname(os.path.realpath(__file__)) + "\\source\\"

flags = "-nodefaults -proc gekko -DRELEASE -Cpp_exceptions off -O4,s -fp hard -enum int -DTRK_INTEGRATION -DGEKKO -DMTX_USE_PS -MMD -rtti off "
includes = "-i . -I- -i include "

for inc in MW_INC:
    includes += f"{inc} "

flags += f"{includes} -nosyspath"

ld_flags = f"-nodefaults -fp hard -proc gekko -map out.map -lcf {rootPath}/linker/linker.lcf"
ld_flags += f" -lr {SDK_LIB_PATH} -lr {MW_LIB_PATH} -lr {NW_LIB_PATH} -lr {rootPath}/build"
as_flags = "-i . -I- -proc gekko -d __MWERKS__"

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
assembly_files = [f for f in glob.glob(path + "**/*.s", recursive=True)]

for f in cpp_files:
    file_name = Path(f).stem

    print(f"Compiling {file_name}.cpp...")

    if subprocess.call(f"{MW_TOOLS_PATH}/mwcceppc.exe {flags} -o build/{file_name}.o {f}", shell=True) == 1:
        sys.exit(1)

    numFilesCompiled += 1

for f in c_files:
    file_name = Path(f).stem
    
    print(f"Compiling {file_name}.c...")

    if subprocess.call(f"{MW_TOOLS_PATH}/mwcceppc.exe {flags} -o build/{file_name}.o {f}", shell=True) == 1:
        sys.exit(1)

    numFilesCompiled += 1

for f in assembly_files:
    file_name = Path(f).stem

    print(f"Assembling {file_name}.s...")

    if subprocess.call(f"{MW_TOOLS_PATH}/mwasmeppc.exe {as_flags} -o build/{file_name}.o {f}", shell=True) == 1:
        sys.exit(1)

    numFilesCompiled += 1

if numFilesCompiled == 0:
    print("No changes to compile.")

archives = ""

with open("archives.txt", "r") as f:
    lines = f.readlines()

for line in lines:
    split = line.split("=")
    archive = split[0]
    objs = split[1].split(" ")

    out = ""

    for obj in objs:
        out += f"build/{obj} "

    print(f"Creating {archive}.a...")

    if subprocess.call(f"{MW_TOOLS_PATH}/mwldeppc.exe -library -l, {out} -o {archive}.a", shell=True) == 1:
        sys.exit(1)

    archives += f"{archive}.a "

dirs = os.listdir(os.getcwd())

for dire in dirs:
    if dire.endswith(".d"):
        os.remove(os.path.join(os.getcwd(), dire))

if "-nolink" in sys.argv:
    sys.exit(1)

print("Linking...")

if subprocess.call(f"{MW_TOOLS_PATH}/mwldeppc.exe {ld_flags} output.a -o Petari", shell=True) == 1:
    sys.exit(1)

print("Complete.")