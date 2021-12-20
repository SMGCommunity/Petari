import subprocess
import sys
import os
import shutil
import util

def deleteDFiles():
    dirs = os.listdir(os.getcwd())

    for dire in dirs:
        if dire.endswith(".d"):
            os.remove(os.path.join(os.getcwd(), dire))

rootPath = os.path.dirname(os.path.realpath(__file__))
path = os.path.dirname(os.path.realpath(__file__)) + "\\source\\"
toolsPath = os.path.dirname(os.path.realpath(__file__)) + "\\tools\\"

flags = "-c -Cpp_exceptions off -stdinc -nodefaults -proc gekko -fp hard -lang=c++ -inline auto, level=2 -ipa file -O4,s -rtti off -sdata 4 -sdata2 4 -align powerpc -enum int -DRVL_SDK -DEPPC -DHOLLYWOOD_REV -DTRK_INTEGRATION -DGEKKO -DMTX_USE_PS -D_MSL_USING_MW_C_HEADERS -msgstyle gcc "
includes = "-i . -I- -i include "

default_compiler_path = "GC/3.0a3/"

compiler_execptions = {
    #"source\JSystem\JKernel\JKRThread.cpp": "GC/2.5/"
}

compiler_flags = {
    #"GC/2.5", flags
}

if "-nonmatching" in sys.argv:
    print("Using nonmatching functions")
    flags = flags + " -DNON_MATCHING "

if "RVLFOLDER" not in os.environ:
    print("RVLFOLDER not set in PATH.")
    sys.exit(1)

if "CWFOLDER" not in os.environ:
    print("CWFOLDER not set in PATH.")
    sys.exit(1)

if "NW4RFOLDER" not in os.environ:
    print("NW4RFOLDER not set in PATH.")
    sys.exit(1)

if "MWFOLDER" not in os.environ:
    print("MWFOLDER not set in PATH.")
    sys.exit(1)

if "RFLFOLDER" not in os.environ:
    print("RFLFOLDER not set in PATH.")
    sys.exit(1)

rvl_path = os.getenv("RVLFOLDER")
cw_path = os.getenv("CWFOLDER")
nw_path = os.getenv("NW4RFOLDER")
mw_path = os.getenv("MWFOLDER")
rfl_path = os.getenv("RFLFOLDER")

includes += f"-i \"{rvl_path}\\include\" -I- -i \"{nw_path}\\include\" -I- -i  \"{mw_path}\\PowerPC_EABI_Support\\MetroTRK\" -I- -i  \"{mw_path}\\PowerPC_EABI_Support\\Runtime\\Inc\" -I- -i \"{mw_path}\\PowerPC_EABI_Support\\MSL\\MSL_C\\PPC_EABI\\Include\" -I- -i \"{mw_path}\\PowerPC_EABI_Support\\MSL\\MSL_C++\\MSL_Common\\Include\" -I- -i \"{mw_path}\\PowerPC_EABI_Support\\MSL\\MSL_C\\MSL_Common\\Include\" -I- -i \"{rfl_path}\\include\" "
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

    compiler_path = f"{cw_path}/{default_compiler_path}"

    try:
        if compiler_execptions[source_path]:
            compiler_path = f"{cw_path}/{compiler_execptions[source_path]}"
    except:
        pass

    print(f"Compiling {source_path}...")
    if subprocess.call(f"{compiler_path}/mwcceppc.exe {flags} {source_path} -o {build_path}", shell=True) == 1:
            deleteDFiles()
            sys.exit(1)

deleteDFiles()

print("Complete.")