import subprocess
import sys
import os
import shutil

import pathlib
import shutil

def makeArchive(dir):
    fileList = ""
    for root, dirs, files in os.walk(f"build/nw4r/{dir}"):
        for f in files:
            if f.endswith(".o"):
                fileList += f"build/nw4r/{dir}/{f} "

    default_compiler_path = pathlib.Path("GC/3.0/")
    linker_path = pathlib.Path(f"../../Compilers/{default_compiler_path}/mwldeppc.exe ")
    linker_flags = f"-nodefaults -xm l -o lib/{dir}.a {fileList}"

    if subprocess.call(f"{linker_path} {linker_flags}", shell=True) == 1:
        print("Library creation failed.")

def makeLibArchive():
    if not os.path.isdir("lib"):
        os.mkdir("lib")

    for root, dirs, files in os.walk("build/nw4r"):
        for dir in dirs:
            makeArchive(dir)

def deleteDFiles():
    dirs = os.listdir(os.getcwd())

    for dire in dirs:
        if dire.endswith(".d"):
            os.remove(os.path.join(os.getcwd(), dire))

def main(compile_non_matching, use_ninja, clean_ninja, link):
    if not os.path.exists("../../Compilers"):
        print("Compilers folder not created, please run setup.py!")
        sys.exit(1)

    isNotWindows = os.name != "nt"

    flags = "-c -ipa file -nodefaults -nostdlib -proc gekko -align powerpc -enum int -enc SJIS -fp hard -Cpp_exceptions off -rtti off -DEPPC -DGEKKO -O4,p -inline auto -g "
    includes = "-i . -I- -i include "

    default_compiler_path = pathlib.Path("../../Compilers/GC/3.0/")

    compiler_exceptions = {
        #"source\JSystem\JKernel\JKRHeap.cpp": pathlib.Path("GC/1.2.5/")
    }

    compiler_flags = {
        #"GC/2.5", flags
    }

    if compile_non_matching:
        print("Using nonmatching functions")
        flags = flags + " -DNON_MATCHING "

    tasks = list()

    rvl_path =      pathlib.Path("../RVL_SDK/include")
    trk_path =      pathlib.Path("../MetroTRK/include")
    runtime_path =  pathlib.Path("../Runtime/include")
    msl_path =      pathlib.Path("../MSL_C/include")
    facelib_path =  pathlib.Path("../RVLFaceLib/include")

    includes += f"-i {rvl_path} -i {trk_path} -I- -i {runtime_path} -I- -i {msl_path} -I- -i {facelib_path} "
    flags += includes

    ninjaFound = shutil.which("ninja") is not None
    if not ninjaFound and use_ninja:
        print("Ninja was not found in your PATH. Compilation will be slow!")
    useNinja = ninjaFound and use_ninja
    if not useNinja:
        if os.path.exists("build"):
                shutil.rmtree("build", ignore_errors=True)

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

    compiler_path = pathlib.Path(f"{default_compiler_path}/mwcceppc.exe ")
    if isNotWindows:
        compiler_path = pathlib.Path(f"wine {compiler_path} ")

    if useNinja:
        # Use ninja build system to generate a build script.
        from ninja import ninja_syntax
        bo = open("build.ninja", "w")
        nw = ninja_syntax.Writer(bo)

        # Create main compiler rule and exception compilers.
        nw.rule("cc", f"{compiler_path} $flags $in -o $out", "Compiling $in...")
        exceptionsToRules = { }
        cc_num = 1
        for exc in compiler_exceptions.values():
            if not exc in exceptionsToRules.keys():
                exceptionsToRules[exc] = f"cc_{cc_num}"
                cc_num += 1

        nw.newline()
        for task in tasks:
            source_path, build_path = task
            rule = "cc"
            try:
                if compiler_exceptions[source_path]:
                    rule = exceptionsToRules[compiler_exceptions[source_path]]
                    path = f"../../Compilers/{compiler_exceptions[source_path]}/mwcceppc.exe "
                    nw.rule(f"{rule}", f"{path} $flags $in -o $out", "Compiling $in [With different compiler]...")
            except:
                pass
            nw.build(build_path, rule, source_path, variables={ 'flags': flags })
        nw.close()

        # Run clean
        if clean_ninja:
            subprocess.call("ninja -t clean", shell=True)
    
        # Call ninja to run said build script.
        if subprocess.call("ninja", shell=True) == 1:
            deleteDFiles()
            sys.exit(1)

    else:   
        for task in tasks:
            source_path, build_path = task     

            try:
                if compiler_exceptions[source_path]:
                    compiler_path = pathlib.Path(f"../../Compilers/{compiler_exceptions[source_path]}/mwcceppc.exe ")
                    if isNotWindows:
                        compiler_path = pathlib.Path(f"wine {compiler_path} ")
            except:
                pass

            print(f"Compiling {source_path}...")
            if subprocess.call(f"{compiler_path} {flags} {source_path} -o {build_path}", shell=True) == 1:
                    deleteDFiles()
                    sys.exit(1)

    deleteDFiles()

    if link:
        print("Creating library archives...")
        makeLibArchive()
    print("Complete.")

def print_help_and_exit():
    print("Usage: build.py [flags...]")
    print("\t-archive: Create the library archive.")
    print("\t-non-matching: Compile non-matching code.")
    print("\t-no-ninja: Do not use ninja even if available.")
    print("\t-clean: Clean old build files before building new when using ninja.")
    print("\t-help: Displays this help text")

    sys.exit(0)

if __name__ == "__main__":
    compile_non_matching = False
    use_ninja = True
    clean_ninja = False
    link = False

    for arg in sys.argv[1:]:
        if arg == "-non-matching":
            compile_non_matching = True
        elif arg == "-no-ninja":
            use_ninja = False
        elif arg == "-clean":
            clean_ninja = True
        elif arg == "-help":
            print_help_and_exit()
        elif arg == "-archive":
            link = True
        else:
            print(f"Invalid argument: {arg}")
            print()
            print_help_and_exit()
            
    main(compile_non_matching, use_ninja, clean_ninja, link)
