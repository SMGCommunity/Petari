import subprocess
import sys
import os
import shutil

def deleteDFiles():
    dirs = os.listdir(os.getcwd())

    for dire in dirs:
        if dire.endswith(".d"):
            os.remove(os.path.join(os.getcwd(), dire))

rootPath = os.path.dirname(os.path.realpath(__file__))
path = os.path.dirname(os.path.realpath(__file__)) + "\\source\\"
toolsPath = os.path.dirname(os.path.realpath(__file__)) + "\\tools\\"

flags = "-c -Cpp_exceptions off -stdinc -nodefaults -proc gekko -fp hard -lang=c99 -O4,s -rtti off -align powerpc -enum int -enc SJIS -DRVL_SDK -DEPPC -DHOLLYWOOD_REV -DTRK_INTEGRATION -DGEKKO -DMTX_USE_PS -D_MSL_USING_MW_C_HEADERS -msgstyle gcc "
includes = "-i . -I- -i include "

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

rvl_path = os.getenv("RVLFOLDER")
cw_path = os.getenv("CWFOLDER")
nw_path = os.getenv("NW4RFOLDER")

includes += f"-i {rvl_path}\\include -I- -i {nw_path}\\include -I- -i  {cw_path}\\PowerPC_EABI_Support\\MetroTRK -I- -i  {cw_path}\\PowerPC_EABI_Support\\Runtime\\Inc -I- -i {cw_path}\\PowerPC_EABI_Support\\Msl\\MSL_C\\PPC_EABI\\Include -I- -i {cw_path}\\PowerPC_EABI_Support\\Msl\\MSL_C\\MSL_Common\\Include "
flags += includes

if os.path.exists("build"):
    shutil.rmtree("build", ignore_errors=True)

tasks = list()

# I do not think that SMG1 ever uses any C anywhere that isn't a part of the SDK / Runtime lib
for root, dirs, files in os.walk("source"):
    for file in files:
        if file.endswith(".cpp"):
            source_path = os.path.join(root, file)
            build_path = source_path.replace("source", "build").replace(".cpp", ".o")

            os.makedirs(os.path.dirname(build_path), exist_ok=True)

            tasks.append((source_path, build_path))

for task in tasks:
    source_path, build_path = task

    print(f"Compiling {source_path}...")

    if subprocess.call(f"{toolsPath}/mwcceppc.exe {flags} {source_path} -o {build_path}", shell=True) == 1:
            deleteDFiles()
            sys.exit(1)

deleteDFiles()

print("Complete.")