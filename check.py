# check.py
# script that marks functions as decompiled based on matching status

from elftools.elf.elffile import ELFFile
import glob
import sys
import dol
import struct
import util
from capstone import *
from capstone.ppc import *

blacklistedInsns = {
    # Unsupported instructions
    PPC_INS_VMSUMSHM, PPC_INS_VMHADDSHS, PPC_INS_XXSLDWI, PPC_INS_VSEL,
    PPC_INS_XVSUBSP, PPC_INS_XXSEL, PPC_INS_XVMULSP, PPC_INS_XVDIVSP,
    PPC_INS_VADDUHM, PPC_INS_XXPERMDI, PPC_INS_XVMADDASP, PPC_INS_XVMADDMSP,
    PPC_INS_XVCMPGTSP, PPC_INS_XXMRGHD, PPC_INS_XSMSUBMDP, PPC_INS_XSTDIVDP,
    PPC_INS_XVADDSP, PPC_INS_XVCMPEQSP, PPC_INS_XVMSUBASP, PPC_INS_XVCMPGESP,
    PPC_INS_MFESR, PPC_INS_MFDEAR, PPC_INS_MTESR, PPC_INS_MTDEAR, PPC_INS_MFICCR, PPC_INS_MFASR
}

if len(sys.argv) < 1:
    print("Syntax: check.py [mangled_sym]")
    sys.exit(1)

if len(sys.argv) == 1:
    sym = None
else:
    sym = sys.argv[1].replace(",", "&#44;").strip("\n\r")

lib = util.getLibFromSym(sym)

if lib == "":
    print("library not found, symbol must not exist")
    sys.exit(1)

if lib == "TRK_Hollywood_Revolution":
    print("this library is not supported at the moment")
    sys.exit(1)

with open(f"csv/{lib}.csv", "r") as f:
    lines = f.readlines()

if sym is not None:
    obj = ""

    # now we find the object file this is associated with
    for line in lines:
        newLine = line.strip("\n\r")
        spl = newLine.split(",")

        if spl[0] == sym:
            obj = spl[1]

    if obj == "":
        print("Could not find object from symbol.")
        sys.exit(1)

    files = glob.glob(f"build/**/{obj}", recursive=True)

    if len(files) > 1:
        print("There are multiple .o files for the same lib...this should not happen")
        sys.exit(1)

    if len(files) == 0:
        print("There are no .o files for this specific function in the specified library.")
        sys.exit(1)

    obj_file = files[0]

    with open(obj_file, "rb") as f:
        elf_file = ELFFile(f) 
        symtab = elf_file.get_section_by_name('.symtab')

        if symtab.get_symbol_by_name(sym) is None:
            print("could not find symbol, did you forget to compile?")
            sys.exit(1)

        symbol = symtab.get_symbol_by_name(sym)[0]

        offs = symbol['st_value']
        code = dol.getCodeFromDOL(sym, symbol['st_size'])

        # now we can get our custom code
        text = elf_file.get_section_by_name('.text')
        dat = text.data()[offs:offs + symbol['st_size']]

        cs = Cs(CS_ARCH_PPC, CS_MODE_32 | CS_MODE_BIG_ENDIAN)
        cs.detail = True
        cs.imm_unsigned = False

        instr_original = list(cs.disasm(code, 0))
        instr_new = list(cs.disasm(dat, 0))

        curInst = 0
        totalInsts = len(instr_new)

        error_count = 0
        
        for i in range(totalInsts):
            curOrigInstr = instr_original[i]
            curNewInstr = instr_new[i]

            if curOrigInstr.id != curNewInstr.id:
                # one of the common capstone issues is addi / li
                # most likely it's a correct instruction but marked as wrong
                if curOrigInstr.id == PPC_INS_ADDI and curNewInstr.id == PPC_INS_LI:
                    print("found addi / li mismatch, but continuing without error")
                else:
                    print(f"ERROR: Instruction mismatch on line {i * 4}")
                    print("Original:")
                    print(curOrigInstr)
                    print("Yours:")
                    print(curNewInstr)
                    error_count += 1

            # these are a lot of instructions that have different operands because of linking / SDA
            if curOrigInstr.id in {PPC_INS_ADDI, PPC_INS_LIS, PPC_INS_B, PPC_INS_BL, PPC_INS_BC, PPC_INS_BDZ, PPC_INS_BDNZ}:
                continue

            # skip any blacklisted instructions
            if curOrigInstr.id in blacklistedInsns:
                continue

            curOrigOps = curOrigInstr.operands
            curNewOps = curNewInstr.operands

            curOp = 0
            totalOps = len(list(curOrigOps))

            for j in range(totalOps):
                curOpOg = curOrigOps[j]
                curOpNw = curNewOps[j]

                if curOpOg.reg != curOpNw.reg:
                    if curOpOg.reg == PPC_REG_R2 and curOpNw.reg == PPC_REG_R0:
                        print("skipping r2 issue")
                        continue
                    
                    if curOpOg.reg == PPC_REG_R13 and curOpNw.reg == PPC_REG_R0:
                        print("skipping r13 issue")
                        continue
                    # this is a reoccuring issue with r13 as well
                    #if curOrigInstr.id == PPC_INS_LWZ or curOrigInstr.id == PPC_INS_LHZ or curOrigInstr.id == PPC_INS_STW:
                    #    if curOpOg.reg == PPC_REG_R13 and curOpNw.reg == PPC_REG_R0 or curOpOg.reg == PPC_REG_R2 and curOpNw.reg == PPC_REG_R0:
                    #        print("skipping r2/r13 issue with SDA")
                    #        continue

                    #if curOrigInstr.id == PPC_INS_LFS:
                    #    if curOpOg.reg == PPC_REG_R2 and curOpNw.reg == PPC_REG_R0:
                    #        print("skipping r2 issue with lfs")
                    #        continue

                    print(f"ERROR: Operand mismatch on line {i * 4}")
                    print("Original:")
                    print(curOrigInstr)
                    print("Yours:")
                    print(curNewInstr)
                    error_count += 1

        print(f"Error check completed with {error_count} error(s) found.")

        if util.checkSymForMarked(lib, sym) and error_count != 0:
            print("This function is marked as decompiled, but is throwing matching errors! Unmarking as decompiled...")
            util.unmarkAsDecompiled(lib, sym)
            sys.exit(1)

        if error_count == 0 and not util.checkSymForMarked(lib, sym):
            print("This function is marked as not decompiled, but matches. Marking as decompiled...")
            util.markAsDecompiled(lib, sym)
            sys.exit(1)
        
        if error_count == 0 and util.checkSymForMarked(lib, sym):
            print("This function is marked as decompiled, and still has no errors. Nothing to be done.")
            sys.exit(1)
else:
    print("todo -- implement me")