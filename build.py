import subprocess
import sys
import os
import shutil
import util
import pathlib

def deleteDFiles():
    dirs = os.listdir(os.getcwd())

    for dire in dirs:
        if dire.endswith(".d"):
            os.remove(os.path.join(os.getcwd(), dire))

if not os.path.exists("deps"):
    print("deps folder not created, please run setup.py!")
    sys.exit(1)

isNotWindows = os.name != "nt"

rootPath = os.path.dirname(os.path.realpath(__file__))
path = os.path.dirname(os.path.realpath(__file__)) + "/source/"
toolsPath = os.path.dirname(os.path.realpath(__file__)) + "/tools/"

flags = "-c -Cpp_exceptions off -stdinc -nodefaults -proc gekko -fp hard -lang=c++ -inline auto,level=2 -ipa file -O4,s -rtti off -sdata 4 -sdata2 4 -align powerpc -enum int -DRVL_SDK -DEPPC -DHOLLYWOOD_REV -DTRK_INTEGRATION -DGEKKO -DMTX_USE_PS -D_MSL_USING_MW_C_HEADERS -msgstyle gcc "
includes = "-i . -I- -i include "

default_compiler_path = pathlib.Path("GC/3.0a3/")

compiler_execptions = {
    #"source\JSystem\JKernel\JKRThread.cpp": "GC/2.5/"
}

compiler_flags = {
    #"GC/2.5", flags
}

if "-nonmatching" in sys.argv:
    print("Using nonmatching functions")
    flags = flags + " -DNON_MATCHING "

rvl_sdk_path = pathlib.Path("deps/RVL_SDK/include")
nw4r_path = pathlib.Path("deps/NW4R/Library/include")
trk_path = pathlib.Path("deps/EABI/PowerPC_EABI_Support/MetroTRK")
runtime_path = pathlib.Path("deps/EABI/PowerPC_EABI_Support/Runtime/Inc")
msl_c_path = pathlib.Path("deps/EABI/PowerPC_EABI_Support/MSL/MSL_C/PPC_EABI/Include")
msl_cpp_path = pathlib.Path("deps/EABI/PowerPC_EABI_Support/MSL/MSL_C++/MSL_Common/Include")
msl_c_common_path = pathlib.Path("deps/EABI/PowerPC_EABI_Support/MSL/MSL_C/MSL_Common/Include")
facelib_path = pathlib.Path("deps/RVLFaceLib/include")

includes += f"-i {rvl_sdk_path} -I- -i {nw4r_path} -I- -i {trk_path} -I- -i {runtime_path} -I- -i {msl_c_path} -I- -i {msl_cpp_path} -I- -i {msl_c_common_path} -I- -i {facelib_path} "
flags += includes

if os.path.exists("build"):
    shutil.rmtree("build", ignore_errors=True)

tasks = list()

for root, dirs, files in os.walk("source"):
    for file in files:
        if file.endswith(".cpp"):
            source_path = os.path.join(root, file)
            build_path = source_path.replace("source", "build", 1).replace(".cpp", ".o")

            os.makedirs(os.path.dirname(build_path), exist_ok=True)

            tasks.append((source_path, build_path))
        elif file.endswith(".c"):
            source_path = os.path.join(root, file)
            build_path = source_path.replace("source", "build", 1).replace(".c", ".o")

            os.makedirs(os.path.dirname(build_path), exist_ok=True)

            tasks.append((source_path, build_path))

for task in tasks:
    source_path, build_path = task

    compiler_path = pathlib.Path(f"deps/Compilers/{default_compiler_path}/mwcceppc.exe ")

    try:
        if compiler_execptions[source_path]:
            compiler_path = pathlib.Path(f"{compiler_execptions[source_path]}/mwcceppc.exe ")
    except:
        pass

    if isNotWindows:
        compiler_path = pathlib.Path(f"wine {compiler_path} ")


    print(f"Compiling {source_path}...")
    if subprocess.call(f"{compiler_path} {flags} {source_path} -o {build_path}", shell=True) == 1:
            deleteDFiles()
            sys.exit(1)

deleteDFiles()

print("Complete.")