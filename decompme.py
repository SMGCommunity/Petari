# decompme.py
# uploads a "scratch" to https://decomp.me for help matching

import subprocess
import sys
from capstone import *
from capstone.arm64 import *
import urllib.request
import urllib.parse
from typing import Dict
import json
import helpers
import glob
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

LIBRARIES = [ "Game", "JSystem", "MetroTRK", "MSL_C", "nw4r", "Runtime", "RVL_SDK", "RVLFaceLib" ]

incdirs = " ".join([f'-I- -i {dir}' for dir in INCLUDE_DIRS])
PREPROC_CMD = f"-EP {incdirs}"
COMPILER_PATH = pathlib.Path("Compilers\\GC\\3.0a3\\mwcceppc.exe")
COMPILER_CMD = f"-c -Cpp_exceptions off -maxerrors 1 -nodefaults -proc gekko -fp hard -lang=c++ -ipa file -inline auto,level=2 -O4,s -rtti off -sdata 4 -sdata2 4 -align powerpc -enum int -msgstyle gcc"

if len(sys.argv) < 2:
    print("decompme.py -- Generates a decomp.me scratch based on a given symbol.")
    print("decompme.py [symbol] {-test}")
    sys.exit(1)

sym = sys.argv[1]
isTest = False

if "-test" in sys.argv:
    isTest = True

functionAddr = 0
functionSize = 0
objName = None
containedLib = None

for lib in LIBRARIES:
    function_library = helpers.FunctionLibrary(lib)
    function_library.load()
    
    objs = function_library.get_obj_names_from_symbol(sym)
    
    if len(objs) > 0:
        functionSize = function_library.get_size_from_symbol(sym)
        functionAddr = function_library.get_address_from_symbol(sym)
        objName = objs[0]
        containedLib = lib
        break

if objName is None:
    print("failed to find object file")
    sys.exit(1)

# time to find our filepath
filename = objName.replace(".o", ".cpp")

if containedLib == "Game":
    glob_path = f"source\\Game\\**\\{filename}"
else:
    glob_path = f"libs\\{containedLib}\\**\\{filename}"

for n in glob.glob(glob_path, recursive=True):
    path = n

# we found it!
# now let's run our preproc

preproc_cmd = [COMPILER_PATH, "-EP"] + incdirs.split() + [path]
procOut = subprocess.check_output(preproc_cmd).decode("utf-8")

funcData = helpers.get_code_from_dol(functionAddr, functionSize)

cs = Cs(CS_ARCH_PPC, CS_MODE_32 | CS_MODE_BIG_ENDIAN | CS_MODE_PS)
cs.detail = True
cs.imm_unsigned = False

if funcData == b'':
    print("Failed to fetch function data.")
    sys.exit(1)

instrs = list(cs.disasm(funcData, 0))
instrs_text = ""
instrs_text += f".global {sym}\n"
instrs_text += f"{sym}:\n"

for instr in instrs:
    instr_line = ("{}\t{}\n".format(instr.mnemonic, instr.op_str))
    instrs_text += instr_line

api_base = "https://decomp.me"

if not isTest:
    print("Uploading...")

    try:
        post_data = urllib.parse.urlencode(
            {
                "name": sym,
                "target_asm": instrs_text,
                "context": procOut,
                "source_code": "",
                "compiler": "mwcc_41_60209",
                "compiler_flags": COMPILER_CMD,
                "diff_label": sym
            }
        ).encode("ascii")

        with urllib.request.urlopen(f"{api_base}/api/scratch", post_data) as f:
            resp = f.read()
            json_data: Dict[str, str] = json.loads(resp)

            if "slug" in json_data:
                slug = json_data["slug"]
                token = json_data.get("claim_token")
                if token:
                    print(f"https://decomp.me/scratch/{slug}/claim?token={token}")
                else:
                    print(f"https://decomp.me/scratch/{slug}")
            else:
                error = json_data.get("error", resp)
                print(f"Server error: {error}")
    except Exception as e:
        print(e)