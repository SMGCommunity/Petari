import glob
import subprocess
import sys
import os
import shutil
from ninja_syntax import Writer
import pathlib

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

def genNinja(lib, tasks):
    with open(f'build_{lib}.ninja', 'w') as ninja_file:
        ninja_writer = Writer(ninja_file)
        ninja_writer.rule("compile", command=f'{LIBRARY_COMPILER[lib]} {LIBRARY_COMPILER_ARGS[lib]} $in -o $out', description=f'Compiling $in')

        for task in tasks:
            source_path, build_path = task
            ninja_writer.build(outputs=[build_path], rule="compile", inputs=[source_path])

def compileLibrary(name, path, clean):
    if clean:
        cleanLibraryBuild(name)
    
    compile_tasks = list()

    if name == "Game":
        path = "source"
    # fixing this lib later
    elif name == "MetroTRK":
        return

    for root, dirs, files in os.walk(path):
        for file in files:
            for ext in SOURCE_FILE_EXTS:
                if file.endswith(ext):
                    source_path = os.path.join(root, file)
                    build_path = source_path.replace("source", "build", 1).replace(ext, ".o")

                    os.makedirs(os.path.dirname(build_path), exist_ok=True)
                    compile_tasks.append((source_path, build_path))

    genNinja(name, compile_tasks)
    subprocess.run(['ninja', '-f', f'build_{name}.ninja'], check=True)

clean = '-clean' in sys.argv

for lib in LIBRARIES:
    compileLibrary(lib, f"libs\\{lib}\\source", clean)
