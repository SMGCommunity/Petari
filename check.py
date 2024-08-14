# check.py
# script that marks functions as decompiled based on matching status

from typing import overload
from elftools.elf.elffile import ELFFile
from colorama import Fore, Style
import glob
import sys
from capstone import *
from capstone.ppc import *
import pathlib
import progress
import helpers

# MSL_C++ has no real source files to check
LIBRARIES = [ "Game", "JSystem", "MetroTRK", "MSL_C", "nw4r", "Runtime", "RVL_SDK", "RVLFaceLib" ]

    
def check_symbol(function_library, mangled_symbols, printInstrs):
    symbol_rets = []

    black_listed_instructions = {
        PPC_INS_VMSUMSHM, PPC_INS_VMHADDSHS, PPC_INS_XXSLDWI, PPC_INS_VSEL,
        PPC_INS_XVSUBSP, PPC_INS_XXSEL, PPC_INS_XVMULSP, PPC_INS_XVDIVSP,
        PPC_INS_VADDUHM, PPC_INS_XXPERMDI, PPC_INS_XVMADDASP, PPC_INS_XVMADDMSP,
        PPC_INS_XVCMPGTSP, PPC_INS_XXMRGHD, PPC_INS_XSMSUBMDP, PPC_INS_XSTDIVDP,
        PPC_INS_XVADDSP, PPC_INS_XVCMPEQSP, PPC_INS_XVMSUBASP, PPC_INS_XVCMPGESP,
        PPC_INS_MFESR, PPC_INS_MFDEAR, PPC_INS_MTESR, PPC_INS_MTDEAR, PPC_INS_MFICCR, PPC_INS_MFASR
    }

    objs = { }

    for sym in mangled_symbols:
        names = function_library.get_obj_names_from_symbol(sym)

        if len(names) != 0:
            objs[sym] = names[0]

    if len(objs) == 0:
        return []
    
    obj_files = {}

    # associate the symbols with their relative object files
    for key in objs:
        if function_library.parent == "Game":
            obj_files[key] = glob.glob(f"build\\Game\\*\\{objs[key]}", recursive=True)
        else:
            obj_files[key] = glob.glob(f"libs\\{function_library.parent}\\build\\{function_library.parent}\\*\\{objs[key]}")

    for key in obj_files:
        if printInstrs:
            print(f"{key} ===============================================================")
        if len(obj_files[key]) == 0:
            print(f"Failed to find object file for symbol {key}")
            continue
        elif len(obj_files[key]) > 1:
            print(f"There are multiple objects associated with {key}...")
            continue

        with open(pathlib.Path(obj_files[key][0]), "rb") as input:
            elf_file = ELFFile(input)
            symtab = elf_file.get_section_by_name('.symtab')

            if symtab.get_symbol_by_name(key) is None:
                print(f"Could not find symbol in object file. This may be caused by the code not being compiled, the function being in the wrong C++ source file or the function signature being wrong. File: {obj_files}")
                symbol_rets.append(1)
                continue
            
            compiled_symbol = symtab.get_symbol_by_name(key)[0]
            custom_offset = compiled_symbol["st_value"]
            custom_size = compiled_symbol['st_size']

            text = elf_file.get_section_by_name('.text')
            custom_data = text.data()[custom_offset:custom_offset + custom_size]

            original_address = function_library.get_address_from_symbol(key)
            original_size = function_library.get_size_from_symbol(key)

            if original_address == None or original_size == None:
                print("Could not find address and/or size for symbol")
                symbol_rets.append(1)
                continue
            
            original_data = helpers.get_code_from_dol(original_address, original_size)

            if original_data == None:
                print("Could not get data from DOL file.")
                symbol_rets.append(1)
                continue
            
            cs = Cs(CS_ARCH_PPC, CS_MODE_32 | CS_MODE_BIG_ENDIAN | CS_MODE_PS)
            cs.detail = True
            cs.imm_unsigned = False

            original_instructions = list(cs.disasm(original_data, 0))
            custom_instructions = list(cs.disasm(custom_data, 0))

            error_count = 0
            warning_count = 0
            hint_count = 0

            instr_count = len(original_instructions)
            custom_count = len(custom_instructions)

            if instr_count != custom_count:
                print(f"[{Fore.YELLOW}{key}{Style.RESET_ALL}] Original instruction count is not the same as custom instruction count.")
                symbol_rets.append(1)
                continue
            
            for i in range(instr_count):
                orig = original_instructions[i]
                cust = custom_instructions[i]

                orig_operands = orig.operands
                cust_operands = cust.operands

                if str(orig) == str(cust):
                    if printInstrs:
                        print(f"{Fore.GREEN}{str(orig):<80}{cust}{Style.RESET_ALL}")
                    # Fully identical, nothing to be checked
                    continue

                if orig.id == cust.id:
                    assert(len(orig_operands) == len(cust_operands))

                    # First check common r2 and r13 issues
                    if orig.id in { PPC_INS_LBZ, PPC_INS_LWZ, PPC_INS_STB, PPC_INS_STW, PPC_INS_LFS }:

                        # lbz, lwz, stb, stw and lfs are sometimes used with r13, which is a pointer to a read-write
                        # small data area (SDA). When compiling custom code, this SDA is not generated,
                        # so the register is set to r0 and the displacement is set to 0.

                        # Original must be (instr) rX, X(r13) and custom must be (instr) rX, 0(r0)
                        if orig_operands[1].reg == PPC_REG_R13 and cust_operands[1].reg == PPC_REG_R0 and\
                                cust_operands[1].mem.disp == 0 and orig_operands[0].reg == cust_operands[0].reg:
                            if printInstrs:
                                print(f"{Fore.YELLOW}{str(orig):<80}{cust}{Style.RESET_ALL}")
                            hint_count += 1
                            continue
                        
                    if orig.id in { PPC_INS_LWZ, PPC_INS_LFS, PPC_INS_LHZ, PPC_INS_LFS }:

                        # Same as above, except with r2 instead of r13. r2 is a pointer to a read-only SDA.

                        # Original must be (instr) rX, X(r2) and custom must be (instr) rX, 0(0)
                        if orig_operands[1].reg == PPC_REG_R2 and cust_operands[1].reg == PPC_REG_R0 and\
                                cust_operands[1].mem.disp == 0 and orig_operands[0].reg == cust_operands[0].reg:
                            if printInstrs:
                                print(f"{Fore.YELLOW}{str(orig):<80}{cust}{Style.RESET_ALL}")
                            hint_count += 1 
                            continue

                    # Check if all registers are equal
                    registers_equal = True 

                    for j in range(len(orig_operands)):
                        if orig_operands[j].reg != cust_operands[j].reg:
                            registers_equal = False
                            break

                    if registers_equal:
                        if printInstrs:
                            print(f"{Fore.YELLOW}{str(orig):<80}{cust}{Style.RESET_ALL}")
                        warning_count += 1
                    elif orig.id == PPC_INS_ADDI:
                        if printInstrs:
                            print(f"{Fore.YELLOW}{str(orig):<80}{cust}{Style.RESET_ALL}")
                        warning_count += 1
                    elif orig.id == PPC_INS_LIS:
                        if printInstrs:
                            print(f"{Fore.YELLOW}{str(orig):<80}{cust}{Style.RESET_ALL}")
                        warning_count += 1
                    elif orig.id in { PPC_INS_B, PPC_INS_BL }:
                        if printInstrs:
                            print(f"{Fore.YELLOW}{str(orig):<80}{cust}{Style.RESET_ALL}")
                        warning_count += 1
                    elif orig.id in { PPC_INS_LFS, PPC_INS_LWZ }:
                        if (cust_operands[j].reg == 0):
                            if printInstrs:
                                print(f"{Fore.YELLOW}{str(orig):<80}{cust}{Style.RESET_ALL}")
                    elif orig.id in { PPC_INS_LFD }:
                        if cust_operands[j].type == 2:
                            print(f"{Fore.YELLOW}{str(orig):<80}{cust}{Style.RESET_ALL}")
                    else:
                        if printInstrs:
                            print(f"{Fore.RED}{str(orig):<80}{cust}{Style.RESET_ALL}")
                        error_count += 1                
                elif orig.id == PPC_INS_ADDI and cust.id == PPC_INS_LI:
                    assert(len(orig_operands) == 3 and len(cust_operands) == 2)
                    if orig_operands[1].reg == PPC_REG_R13 and cust_operands[1].imm == 0 and\
                            orig_operands[0].reg == cust_operands[0].reg:
                        if printInstrs:
                            print(f"{Fore.YELLOW}{str(orig):<80}{cust}{Style.RESET_ALL}")
                        hint_count += 1
                    else:
                        if printInstrs:
                            print(f"{Fore.RED}{str(orig):<80}{cust}{Style.RESET_ALL}")
                        error_count += 1
                else:
                    if printInstrs:
                        print(f"{Fore.RED}{str(orig):<80}{cust}{Style.RESET_ALL}")
                    error_count += 1

            print(f"[{Fore.YELLOW}{key}{Style.RESET_ALL}] Check finished with {Fore.RED}{error_count} error(s){Style.RESET_ALL}, {Fore.YELLOW}{warning_count} warning(s){Style.RESET_ALL} and {Fore.BLUE}{hint_count} hint(s).{Style.RESET_ALL}")
            parent_lib = function_library.get_library_from_symbol(key, objs[key])
            is_decompiled = function_library.get_function_status(parent_lib, key, objs[key])
            passed = error_count == 0

            if passed:
                if is_decompiled == "true":
                    print(f"[{Fore.YELLOW}{key}{Style.RESET_ALL}] Function already marked as decompiled.")
                else:
                    print("Marking as decompiled...")
                    function_library.mark_symbol_status(parent_lib, key, objs[key], "true")
                    symbol_rets.append(0)
                    continue
            else:
                if is_decompiled == "true":
                    print(f"[{Fore.YELLOW}{key}{Style.RESET_ALL}] Function is marked as decompiled, but does not match.")
                    print("Unmarking as decompiled...")
                    function_library.mark_symbol_status(parent_lib, key, objs[key], "false")
                    symbol_rets.append(0)
                    continue
                else:
                    print(f"[{Fore.YELLOW}{key}{Style.RESET_ALL}] Function is not marked as decompiled, and does not match either.")

            symbol_rets.append(1)

    return symbol_rets

if not helpers.is_dol_correct():
    print("DOL file is not valid.")
    sys.exit(1)

objs_to_check = []
funcs_to_check = []
printInstrs = False

function_libraries = {}

# no args means we are checking the recently compiled files
if len(sys.argv) == 1:
    printInstrs = True
    obj_files = []

    with open("data\\changed.txt", "r") as f:
        lines = f.readlines()

        for line in lines:
            objs_to_check.append(line.strip("\n"))

    if len(objs_to_check) == 0:
        print("There are no functions to check.")
        sys.exit(1)

    for obj in objs_to_check:
        with open(obj, "rb") as input:
            elf_file = ELFFile(input)
            symtab = elf_file.get_section_by_name('.symtab')

            for symbol in symtab.iter_symbols():
                section = symbol['st_shndx']
                if isinstance(section, int) and symbol.name != '':
                    section_name = elf_file.get_section(section).name

                    if section_name == '.text':
                        funcs_to_check.append(symbol.name)
else:
    for i in range(1, len(sys.argv)):
        arg = sys.argv[i]

        if arg.endswith(".o"):
            objs_to_check.append(arg)
        elif arg == "-print":
            printInstrs = True
        else:
            funcs_to_check.append(arg)

for lib in LIBRARIES:
    function_library = helpers.FunctionLibrary(lib)
    function_library.load()
    function_libraries[lib] = function_library

libsChanged = []

for lib in LIBRARIES:
    if len(funcs_to_check) > 0:
        # do we even need to check this library?
        isAnyInLib = False

        for func in funcs_to_check:
            if function_libraries[lib].get_obj_names_from_symbol(func) != []:
                isAnyInLib = True
                break

        if isAnyInLib:
            rets = check_symbol(function_libraries[lib], funcs_to_check, printInstrs)

            if len(rets) > 0 and 0 in rets:
                libsChanged.append(lib)

for lib in libsChanged:
    function_libraries[lib].save()
    
if len(libsChanged) > 0:
    progress.genProgress()