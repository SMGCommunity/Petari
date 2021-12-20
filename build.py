import subprocess
import sys
import os
import shutil

import util
import pathlib
import shutil

def deleteDFiles():
    dirs = os.listdir(os.getcwd())

    for dire in dirs:
        if dire.endswith(".d"):
            os.remove(os.path.join(os.getcwd(), dire))

def main(argv):
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

    # Adjust this if all else fails.
    canUseNinja = not "-n" in argv  
    useNinja = shutil.which("ninja") is not None and canUseNinja

    compiler_path = pathlib.Path(f"deps/Compilers/{default_compiler_path}/mwcceppc.exe ")
    if isNotWindows:
        compiler_path = pathlib.Path(f"wine {compiler_path} ")

    if useNinja:

        # Use ninja build system to generate a build script.
        from ninja import ninja_syntax
        bo = open("build.ninja", "w")
        nw = ninja_syntax.Writer(bo)

        # Create main compiler rule and exception compilers.
        nw.rule("cc", f"{compiler_path} $flags $in -o $out", "Compiling $in...")
        exceptionsToRules = { "sample": "value" }
        cc_num = 1
        for exc in compiler_execptions.values():
            if not exc in exceptionsToRules.keys():
                exceptionsToRules[exc] = f"cc_{cc_num}"
                cc_num += 1

        nw.newline()
        for task in tasks:
            source_path, build_path = task
            rule = "cc"
            try:
                if compiler_execptions[source_path]:
                    rule = exceptionsToRules[compiler_execptions[source_path]]
            except:
                pass
            nw.build(build_path, rule, source_path, variables={ 'flags': flags })
        nw.close()
    
        # Call ninja to run said build script.
        if subprocess.call("ninja", shell=True) == 1:
            deleteDFiles()
            sys.exit(1)

    else:

        # Old method.
        if os.path.exists("build"):
            shutil.rmtree("build", ignore_errors=True)
        for task in tasks:
            source_path, build_path = task     

            try:
                if compiler_execptions[source_path]:
                    compiler_path = pathlib.Path(f"deps/Compilers/{compiler_execptions[source_path]}/mwcceppc.exe ")
                    if isNotWindows:
                        compiler_path = pathlib.Path(f"wine {compiler_path} ")
            except:
                pass

            print(f"Compiling {source_path}...")
            if subprocess.call(f"{compiler_path} {flags} {source_path} -o {build_path}", shell=True) == 1:
                    deleteDFiles()
                    sys.exit(1)

    deleteDFiles()

    print("Complete.")

if __name__ == "__main__":
    args = sys.argv[1:]
    if (len(args) == 1 and args[0] != "-n") or len(args) > 1:
        print("Usage: build.py (-n)\n\t-n: Do not use ninja even if available.")
    else:
        main(args)
