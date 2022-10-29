# check.py
# script that marks functions as decompiled based on matching status

import os
from typing import overload
from elftools.elf.elffile import ELFFile
from colorama import Fore, Style
import glob
import hashlib
import sys
import struct
from capstone import *
from capstone.ppc import *
import pathlib
from collections import OrderedDict

class FunctionLibrary:
    def __init__(self):
        self.libraries = dict()
        self.functions = dict()

    def load(self):
        self.libraries.clear()
        self.functions.clear()

        # Load CSV files
        for file in os.listdir("csv"):            
            library = file[0:file.rfind(".")]
            symbols = OrderedDict()

            with open(pathlib.Path(f"csv/{file}"), "r") as input:
                is_first_line = True
                for line in input:
                    if is_first_line:
                        is_first_line = False
                        continue

                    line_split = line.rstrip().split(",")

                    symbol = line_split[0]
                    obj_file = line_split[1]
                    library_name = line_split[2]
                    matches = line_split[3] == "true"

                    if (symbol, obj_file) in symbols:
                        print(f"Duplicate symbol {symbol} in .o file {obj_file}.")
                        sys.exit(1)

                    symbols[(symbol, obj_file)] = (library_name, matches)

            self.libraries[library] = symbols

        # Load addresses from symbol map
        with open("../../data/map_for_dol.map", "r") as input:
            for line in input:
                line_split = line.rstrip().split("=")

                symbol = line_split[1].split(":")[0]

                number_split = line_split[0].split(" ")
                address = int(number_split[4], 16)
                size = int(number_split[3], 16)

                self.functions[symbol] = (address, size)

    def save(self):
        for library, symbols in self.libraries.items():
            with open(f"csv/{library}.csv", "w") as output:
                output.write("Symbol Name, Object File, Library Archive, Matching\n")

                for (symbol, obj_file), values in symbols.items():
                    output.write(f"{symbol},{obj_file},{values[0]},{str(values[1]).lower()}\n")

    def get_obj_names_from_symbol(self, symbol_lookup):
        names = []

        for symbols in self.libraries.values():
            for (symbol, obj_name) in symbols.keys():
                if symbol == symbol_lookup:
                    names.append(obj_name)

        return names

    def is_marked_decompiled(self, library_name, symbol, obj_file):
        if library_name in self.libraries:
            library = self.libraries[library_name]
            
            if (symbol, obj_file) in library:
                return library[(symbol, obj_file)][1]

        return False

    def mark_symbol_decompiled(self, library_name, symbol, obj_file, decompiled):
        if library_name in self.libraries:
            library = self.libraries[library_name]
            
            if (symbol, obj_file) in library:
                library[(symbol, obj_file)] = (library[(symbol, obj_file)][0], decompiled)

    def get_library_from_symbol(self, symbol, obj_file):
        for library, symbols in self.libraries.items():
            if (symbol, obj_file) in symbols:
                return library

        return None

    def get_address_from_symbol(self, symbol):
        if symbol in self.functions:
            return self.functions[symbol][0]

        return None
        
    def get_size_from_symbol(self, symbol):
        if symbol in self.functions:
            return self.functions[symbol][1]

        return None

    def get_symbols_marked_as_decompiled(self):
        for symbols in self.libraries.values():
            for (symbol, obj_file), values in symbols.items():
                if values[1] == True:
                    yield (symbol, obj_file)

def print_help_and_exit():
    print("Usage: check.py [mangled_symbol] [flags...]")
    print("\t[mangled_symbol]: name of the symbol that should be checked.")
    print("\t[-all]: run checks on all functions which has been marked as decompiled.")
    print("\t[-compare]: compares decompiled functions.")
    print("\t[-help]: displays this help text.")
    print("\t[-only-errors]: displays only error messages.")
    print("\t[-no-hints]: don't display hint messages.")
    print("\t[-no-warnings]: don't display warning messages.")
    print("\t[-no-errors]: don't display error messages.")
    print("\t[-readonly]: don't mark or unmark any functions as decompiled.")

    sys.exit(0)

def is_dol_correct():
    with open("../../baserom.dol", "rb") as input:
        data = input.read()

        hash = hashlib.sha256(data).hexdigest().upper()
        return hash == "8B7F28D193170F998F92E02EA638107822FB72073691D0893EB18857BE0C6FCF" or hash == "69F93FCC0FA34837347B5AC05168BC783ADCACB3C02697CFDA087A3B63ABC9E0"

def get_code_from_dol(address, size):
    with open("../../baserom.dol", "rb") as input:
        data = input.read()
        
        # Grab .text offset and start offset
        txt_offset, = struct.unpack_from(">I", data, 4)
        start_address, = struct.unpack_from(">I", data, 0x4C)

        offset = address - start_address

        return data[txt_offset + offset:txt_offset + offset + size]

def print_error(message):
    global show_errors

    if show_errors:
        print(f"ERROR: {message}")

def print_warning(message):
    global show_warnings

    if show_warnings:
        print(f"WARNING: {message}")

def print_hint(message):
    global show_hints

    if show_hints:
        print(f"HINT: {message}")

def print_instruction_comparison_error(message, original, custom):
    global show_errors

    if show_errors:
        print_error(message)
        print_instruction_comparison(original, custom)

def print_instruction_comparison_warning(message, original, custom):
    global show_warnings

    if show_warnings:
        print_warning(message)
        print_instruction_comparison(original, custom)

def print_instruction_comparison_hint(message, original, custom):
    global show_hints

    if show_hints:
        print_hint(message)
        print_instruction_comparison(original, custom)

def print_instruction_comparison(original, custom):
    print(f"\tOriginal: {original}")
    print(f"\tCustom:   {custom}")

def check_symbol(function_library, mangled_symbol, obj_name, readonly):
    black_listed_instructions = {
        PPC_INS_VMSUMSHM, PPC_INS_VMHADDSHS, PPC_INS_XXSLDWI, PPC_INS_VSEL,
        PPC_INS_XVSUBSP, PPC_INS_XXSEL, PPC_INS_XVMULSP, PPC_INS_XVDIVSP,
        PPC_INS_VADDUHM, PPC_INS_XXPERMDI, PPC_INS_XVMADDASP, PPC_INS_XVMADDMSP,
        PPC_INS_XVCMPGTSP, PPC_INS_XXMRGHD, PPC_INS_XSMSUBMDP, PPC_INS_XSTDIVDP,
        PPC_INS_XVADDSP, PPC_INS_XVCMPEQSP, PPC_INS_XVMSUBASP, PPC_INS_XVCMPGESP,
        PPC_INS_MFESR, PPC_INS_MFDEAR, PPC_INS_MTESR, PPC_INS_MTDEAR, PPC_INS_MFICCR, PPC_INS_MFASR
    }

    unsupported_libraries = { "TRK_Hollywood_Revolution" }

    library = function_library.get_library_from_symbol(mangled_symbol, obj_name)
    print(library)

    if library == None:
        print("Could not find library of symbol.")
        return False

    if library in unsupported_libraries:
        print(f"Library {library} is currently not supported.")
        return False

    obj_files = glob.glob(f"build/**/{obj_name}", recursive=True)

    if len(obj_files) > 1:
        print("Found multiple .o files. This should not happen.")
        return False

    if len(obj_files) == 0:
        print("Could not find any .o files for the function.")
        return False

    with open(pathlib.Path(obj_files[0]), "rb") as input:
        elf_file = ELFFile(input)
        symtab = elf_file.get_section_by_name('.symtab')

        if symtab.get_symbol_by_name(mangled_symbol) is None:
            print("Could not find symbol in object file. This may be caused by the code not being compiled, the function being in the wrong C++ source file or the function signature being wrong.")
            return False

        compiled_symbol = symtab.get_symbol_by_name(mangled_symbol)[0]

        # Get custom code
        custom_offset = compiled_symbol["st_value"]
        custom_size = compiled_symbol['st_size']

        text = elf_file.get_section_by_name('.text')
        custom_data = text.data()[custom_offset:custom_offset + custom_size]

        # Get original code
        original_address = function_library.get_address_from_symbol(mangled_symbol)
        original_size = function_library.get_size_from_symbol(mangled_symbol)

        if original_address == None or original_size == None:
            print("Could not find address and/or size for symbol")
            return False

        original_data = get_code_from_dol(original_address, original_size)

        if original_data == None:
            print("Could not get data from DOL file.")
            return False

        cs = Cs(CS_ARCH_PPC, CS_MODE_32 | CS_MODE_BIG_ENDIAN)
        cs.detail = True
        cs.imm_unsigned = False

        original_instructions = list(cs.disasm(original_data, 0))
        custom_instructions = list(cs.disasm(custom_data, 0))

        error_count = 0
        warning_count = 0
        hint_count = 0

        # Capstone doesn't seem to handle paired single instructions
        # If any is found, it just stops disassembling
        if 4 * len(original_instructions) != original_size:
            print_warning(f"Only {len(original_instructions)} out of the {original_size // 4} original instructions were loaded.")
            warning_count += 1

        if 4 * len(custom_instructions) != custom_size:
            print_warning(f"Only {len(custom_instructions)} out of the {custom_size // 4} custom instructions were loaded.")
            warning_count += 1

        if original_size > custom_size:
            print_error("Original code contains more instructions than custom code.")
            error_count += 1
        elif original_size < custom_size:
            print_error("Original code contains less instructions than custom code.")
            error_count += 1

        for i in range(min(len(original_instructions), len(custom_instructions))):
            line = i * 4
            line_string = f"{hex(line)} (Original: {hex(original_address + line)})"

            original_instruction = original_instructions[i]            
            custom_instruction = custom_instructions[i]

            original_operands = original_instruction.operands
            custom_operands = custom_instruction.operands

            if str(original_instruction) == str(custom_instruction):
                print(f"{Fore.GREEN}{str(original_instruction):<80}{custom_instruction}{Style.RESET_ALL}")
                # Fully identical, nothing to be checked
                continue

            if original_instruction in black_listed_instructions:
                print_warning(f"Skipping blacklisted instruction at line {line_string}.")
                warning_count += 1
                continue
            
            if original_instruction.id == custom_instruction.id:
                assert(len(original_operands) == len(custom_operands))

                # First check common r2 and r13 issues
                if original_instruction.id in { PPC_INS_LBZ, PPC_INS_LWZ, PPC_INS_STW, PPC_INS_LFS }:
                    assert(len(original_operands) == 2 and len(custom_operands) == 2)

                    # lbz, lwz, stw and lfs are sometimes used with r13, which is a pointer to a read-write
                    # small data area (SDA). When compiling custom code, this SDA is not generated,
                    # so the register is set to r0 and the displacement is set to 0.

                    # Original must be (instr) rX, X(r13) and custom must be (instr) rX, 0(r0)
                    if original_operands[1].reg == PPC_REG_R13 and custom_operands[1].reg == PPC_REG_R0 and\
                            custom_operands[1].mem.disp == 0 and original_operands[0].reg == custom_operands[0].reg:
                        print(f"{Fore.YELLOW}{str(original_instruction):<80}{custom_instruction}{Style.RESET_ALL}")
                        #print_instruction_comparison_hint(f"Skipping r13 issue at line {line_string}.", original_instruction, custom_instruction)
                        hint_count += 1
                        continue
                    
                if original_instruction.id in { PPC_INS_LWZ, PPC_INS_LFS, PPC_INS_LHZ, PPC_INS_LFS }:
                    assert(len(original_operands) == 2 and len(custom_operands) == 2)

                    # Same as above, except with r2 instead of r13. r2 is a pointer to a read-only SDA.

                    # Original must be (instr) rX, X(r2) and custom must be (instr) rX, 0(0)
                    if original_operands[1].reg == PPC_REG_R2 and custom_operands[1].reg == PPC_REG_R0 and\
                            custom_operands[1].mem.disp == 0 and original_operands[0].reg == custom_operands[0].reg:
                        print(f"{Fore.YELLOW}{str(original_instruction):<80}{custom_instruction}{Style.RESET_ALL}")
                        #print_instruction_comparison_hint(f"Skipping r2 issue at line {line_string}.", original_instruction, custom_instruction)
                        hint_count += 1 
                        continue

                # Check if all registers are equal
                registers_equal = True 

                for j in range(len(original_operands)):
                    if original_operands[j].reg != custom_operands[j].reg:
                        registers_equal = False
                        break

                if registers_equal:
                    print(f"{Fore.YELLOW}{str(original_instruction):<80}{custom_instruction}{Style.RESET_ALL}")
                    #print_instruction_comparison_warning(f"Registers are identical but the instruction is not identical at line {line_string}.", original_instruction, custom_instruction)
                    warning_count += 1
                elif original_instruction.id == PPC_INS_ADDI:
                    # addi is commonly used when loading addresses
                    print(f"{Fore.YELLOW}{str(original_instruction):<80}{custom_instruction}{Style.RESET_ALL}")
                    #print_instruction_comparison_warning(f"Skipping addi instruction at line {line_string}.", original_instruction, custom_instruction)
                    warning_count += 1
                elif original_instruction.id == PPC_INS_LIS:
                    # Same as addi
                    print(f"{Fore.YELLOW}{str(original_instruction):<80}{custom_instruction}{Style.RESET_ALL}")
                    #print_instruction_comparison_warning(f"Skipping lis instruction at line {line_string}.", original_instruction, custom_instruction)
                    warning_count += 1
                elif original_instruction.id in { PPC_INS_B, PPC_INS_BL }:
                    # bl is used to call most functions, and since the functions are likely to be placed
                    # differently it's not possible to compare it
                    # If a function ends with a function call, and the returned value from the function, then b is sometimes used for branching
                    # to that function. Then it's not possible to compare this
                    print(f"{Fore.YELLOW}{str(original_instruction):<80}{custom_instruction}{Style.RESET_ALL}")
                    #print_instruction_comparison_warning(f"Skipping branch instruction at line {line_string}.", original_instruction, custom_instruction)
                    warning_count += 1                
                else:
                    print(f"{Fore.RED}{str(original_instruction):<80}{custom_instruction}{Style.RESET_ALL}")
                    #print_instruction_comparison_error(f"Instruction mismatch on line {line_string}.", original_instruction, custom_instruction)
                    error_count += 1                
            elif original_instruction.id == PPC_INS_ADDI and custom_instruction.id == PPC_INS_LI:
                assert(len(original_operands) == 3 and len(custom_operands) == 2)

                # This is caused by the read-write SDA, pointed by r13, is not generated in the custom code.

                # Original must be addi rX, r13, X and custom must be li rX, 0
                if original_operands[1].reg == PPC_REG_R13 and custom_operands[1].imm == 0 and\
                        original_operands[0].reg == custom_operands[0].reg:
                    print(f"{Fore.YELLOW}{str(original_instruction):<80}{custom_instruction}{Style.RESET_ALL}")
                    #print_instruction_comparison_hint(f"Found addi / li mismatch at line {line_string}.", original_instruction, custom_instruction)
                    hint_count += 1
                else:
                    print(f"{Fore.RED}{str(original_instruction):<80}{custom_instruction}{Style.RESET_ALL}")
                    #print_instruction_comparison_error(f"Instruction mismatch on line {line_string}.", original_instruction, custom_instruction)
                    error_count += 1
            else:
                print(f"{Fore.RED}{str(original_instruction):<80}{custom_instruction}{Style.RESET_ALL}")
                #print_instruction_comparison_error(f"Instruction mismatch on line {line_string}.", original_instruction, custom_instruction)
                error_count += 1

        print()
        print(f"Check finished with {error_count} error(s), {warning_count} warning(s) and {hint_count} hint(s).")

        is_decompiled = function_library.is_marked_decompiled(library, mangled_symbol, obj_name)
        passed = error_count == 0

        if not readonly:
            if passed:
                if is_decompiled:
                    print("Function already marked as decompiled.")
                else:
                    print("Marking as decompiled...")
                    function_library.mark_symbol_decompiled(library, mangled_symbol, obj_name, True)
            else:
                if is_decompiled:
                    print("Function is marked as decompiled, but does not match.")
                    print("Unmarking as decompiled...")
                    function_library.mark_symbol_decompiled(library, mangled_symbol, obj_name, False)
                else:
                    print("Function is not marked as decompiled, and does not match either.")

        return passed

mangled_symbol = None
check_all = False
compare = False
show_hints = True
show_warnings = True
show_errors = True
readonly = False

for i in range(1, len(sys.argv)):
    arg = sys.argv[i]

    if arg == "-all":
        check_all = True
    elif arg == "-compare":
        compare = True
    elif arg == "-help":
        print_help_and_exit()
    elif arg == "-only-errors":
        show_hints = False
        show_warnings = False
        show_errors = True
    elif arg == "-no-hints":
        show_hints = False
    elif arg == "-no-warnings":
        show_warnings = False
    elif arg == "-no-errors":
        show_errors = False
    elif arg == "-readonly":
        readonly = True
    elif mangled_symbol == None:
        mangled_symbol = arg
    else:
        print(f"Invalid argument: {arg}")
        print()
        print_help_and_exit()

if mangled_symbol == None and not check_all and not compare:
    print_help_and_exit()

if not is_dol_correct():
    print("DOL file is not valid.")
    sys.exit(1)

function_library = FunctionLibrary()
function_library.load()

if check_all:
    success_count = 0
    total_count = 0

    for (symbol, obj_name) in function_library.get_symbols_marked_as_decompiled():
        print(f"Checking {symbol}...")
        total_count += 1
        
        if check_symbol(function_library, symbol, obj_name, True):
            success_count += 1

        print()

    print(f"{success_count} function(s) out of the {total_count} function(s), which were marked as decompiled, passed the check.")
    print(f"{total_count - success_count} function(s) failed the check.")

if compare:
    success_count_a = 0
    fail_count_a = 0
    matched_a = set()
    failed_a = set()

    for (symbol, obj_name) in function_library.get_symbols_marked_as_decompiled():
        print(f"Checking {symbol}...")
        
        if check_symbol(function_library, symbol, obj_name, True):
            success_count_a += 1
            matched_a.add((symbol, obj_name))
        else:
            fail_count_a += 1
            failed_a.add((symbol, obj_name))

        print()

    print()
    print()
    print("First run of tests are now done.")
    print("Now, please make the code changes you wish to test, and then recompile everything.")
    input("Press Enter to contiue...")
    
    success_count_b = 0
    fail_count_b = 0
    matched_b = set()
    failed_b = set()

    for (symbol, obj_name) in function_library.get_symbols_marked_as_decompiled():
        print(f"Checking {symbol}...")
        
        if check_symbol(function_library, symbol, obj_name, True):
            success_count_b += 1
            matched_b.add((symbol, obj_name))
        else:
            fail_count_b += 1
            failed_b.add((symbol, obj_name))

            print()
            
    print()
    print()
    print("Second run of tests are now done.")
    print()
    
    print(f"First run: {success_count_a} matched and {fail_count_a} failed.")
    print(f"Second run: {success_count_b} matched and {fail_count_b} failed.")
    print()

    print("The following functions matched during the first run and failed during the second test:")

    for matched in matched_a:
        if matched in failed_b:
            print(f"{matched[0]} in {matched[1]}")

    print()
    print("The following functions failed during the first run and matched during the second test:")

    for failed in failed_a:
        if failed in matched_b:
            print(f"{failed[0]} in {failed[1]}")


if mangled_symbol != None:
    obj_names = function_library.get_obj_names_from_symbol(mangled_symbol)

    if len(obj_names) == 0:
        print("Could not find any .o files for the specified symbol.")
        sys.exit(1)
    elif len(obj_names) > 1:
        print("There are multiple .o files found for the specified symbol. This is currently not supported by the script.")
        sys.exit(1)

    check_symbol(function_library, mangled_symbol, obj_names[0], readonly)

function_library.save()