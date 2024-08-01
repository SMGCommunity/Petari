import glob
import subprocess
import sys
import os
import shutil
from ninja_syntax import Writer
import pathlib
import hashlib

INCLUDE_DIRS = [ "include", 
                "libs\\JSystem\\include", 
                "libs\\MetroTRK\\include", 
                "libs\\MSL_C\\include", 
                "libs\\MSL_C++\\include", 
                "libs\\nw4r\\include", 
                "libs\\Runtime\\include", 
                "libs\\RVL_SDK\\include", 
                "libs\\RVLFaceLib\\include" ]

LIBRARIES = [ "Game", "JSystem", "MetroTRK", "MSL_C", "MSL_C++", "nw4r", "Runtime", "RVL_SDK", "RVLFaceLib" ]

incdirs = ""
for dir in INCLUDE_DIRS:
    incdirs += f'-I- -i {dir} '

COMPILER_CMD = f"-c -Cpp_exceptions off -maxerrors 1 -nodefaults -proc gekko -fp hard -lang=c++ -ipa file -inline auto,level=2 -O4,s -rtti off -sdata 4 -sdata2 4 -align powerpc -enum int -msgstyle gcc {incdirs}"
COMPILER_PATH = pathlib.Path("Compilers\\GC\\3.0a3\\mwcceppc.exe")
HASHES_BASE_PATH = pathlib.Path("data\\hashes.txt")
CHANGED_PATH = pathlib.Path("data\\changed.txt")

# if we don't have this file, create it
if not os.path.exists(CHANGED_PATH):
    open(CHANGED_PATH, 'a').close()

# our hashes that we are starting out with
start_hashes = {}

if os.path.exists(HASHES_BASE_PATH):
    with open(HASHES_BASE_PATH, "r") as f:
        lines = f.readlines()

        for line in lines:
            line = line.strip("\n")
            spl = line.split("=")
            obj = spl[0]
            hash = spl[1]
            start_hashes[obj] = hash

LIBRARY_COMPILER_ARGS = {
    "Game": COMPILER_CMD,
    "JSystem": COMPILER_CMD,
    "MetroTRK": COMPILER_CMD,
    "MSL_C": f"-c -nodefaults -nostdlib -use_lmw_stmw on -proc gekko -readonlystrings -rostr -align powerpc -ipa file -enum int -fp hard -Cpp_exceptions on -rtti off -O4,p -sdata2 8 -inline auto,level=2 {incdirs}",
    "MSL_C++": COMPILER_CMD,
    "nw4r": COMPILER_CMD,
    "Runtime": f"-c -nodefaults -nostdlib -proc gekko -use_lmw_stmw on -enum int -fp hard -rtti off -Cpp_exceptions on -O4 -inline auto {incdirs}",
    "RVL_SDK": f"-c -nodefaults -proc gekko -DHOLLYWOOD_REV -DEPPC -sdata 8 -sdata2 8 -enum int -fp hard -Cpp_exceptions off -rtti off -ipa file -DEPPC -DGEKKO -align powerpc -enc SJIS -O4,p -inline auto {incdirs}",
    "RVLFaceLib": f"-c -nodefaults -nostdlib -proc gekko -align powerpc -enum int -enc SJIS -fp hard -Cpp_exceptions off -rtti off -ipa file -DEPPC -DGEKKO -O4,p -inline auto -volatileasm {incdirs}"
}

LIBRARY_COMPILER = {
    "Game": COMPILER_PATH,
    "JSystem": COMPILER_PATH,
    "MetroTRK": COMPILER_PATH,
    "MSL_C": COMPILER_PATH,
    "MSL_C++": COMPILER_PATH,
    "nw4r": COMPILER_PATH,
    "Runtime": COMPILER_PATH,
    "RVL_SDK": pathlib.Path("Compilers\\GC\\3.0\\mwcceppc.exe"),
    "RVLFaceLib": COMPILER_PATH
}

SOURCE_FILE_EXTS = [ ".c", ".cpp", ".s" ]

def cleanLibraryBuild(lib):
    if lib == "Game":
        build_path = "build"
    else:
        build_path = f"libs\\{lib}\\build"

    if os.path.exists(build_path):
        shutil.rmtree(build_path)

def fixLibName(name):
    return name.replace("++", "pp")

def genNinja(tasks):
    with open('build.ninja', 'w') as ninja_file:
        ninja_writer = Writer(ninja_file)
        for lib in LIBRARIES:
            libName = fixLibName(lib)
            ninja_writer.rule(f"compile_{libName}", command=f'{LIBRARY_COMPILER[lib]} {LIBRARY_COMPILER_ARGS[lib]} $in -o $out', description=f'Compiling $in')

        for task in tasks:
            lib, source_path, build_path = task
            libName = fixLibName(lib)
            ninja_writer.build(outputs=[build_path], rule=f"compile_{libName}", inputs=[source_path])

def collectTasks(name, path):
    compile_tasks = list()

    if name == "Game":
        path = "source"
    # fixing this lib later
    elif name == "MetroTRK":
        return compile_tasks

    for root, dirs, files in os.walk(path):
        for file in files:
            for ext in SOURCE_FILE_EXTS:
                if file.endswith(ext):
                    source_path = os.path.join(root, file)
                    build_path = source_path.replace("source", "build", 1).replace(ext, ".o")

                    os.makedirs(os.path.dirname(build_path), exist_ok=True)
                    compile_tasks.append((name, source_path, build_path))

    return compile_tasks

clean = '-clean' in sys.argv

if clean:
    for lib in LIBRARIES:
        cleanLibraryBuild(lib)

all_tasks = []
for lib in LIBRARIES:
    tasks = collectTasks(lib, f"libs\\{lib}\\source")
    all_tasks.extend(tasks)

genNinja(all_tasks)
subprocess.run(['ninja', '-f', 'build.ninja'], check=True)

obj_hashes = {}
changed_objs = []

for lib in LIBRARIES:
    objs = []

    if lib == "Game":
        objs = glob.glob(f"build\\Game\\*\\*.o", recursive=True)
    else:
        objs = glob.glob(f"libs\\{lib}\\build\\{lib}\\*\\*.o")
    
    # generate our hashes
    for obj in objs:
        obj_hashes[obj] = hashlib.md5(open(obj,'rb').read()).hexdigest()

# now we determine what objects were changed based on comparing the two MD5 hashes
for obj in obj_hashes:
    if obj in start_hashes:
        if start_hashes[obj] != obj_hashes[obj]:
            changed_objs.append(obj)

# do we have changed objs?
# if we do, then we write those changed objects to our text file
# if not, we clear the file
if len(changed_objs) > 0:
    with open(CHANGED_PATH, "w") as w:
        for obj in changed_objs:
            w.write(f"{obj}\n")
else:
    open(CHANGED_PATH, 'w').close()

# write our new hashes
with open(HASHES_BASE_PATH, "w") as w:
    for obj in obj_hashes:
        w.write(f"{obj}={obj_hashes[obj]}\n")