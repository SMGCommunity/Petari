from elftools.elf.elffile import ELFFile
import glob
import math
import os

dataSections = [".ctors", ".dtors", ".rodata", ".data", ".bss", ".sdata", ".sbss", ".sdata2", ".sbss2"]

buildPath = os.path.dirname(os.path.realpath(__file__)) + "\\build\\"
o_files = [f for f in glob.glob(buildPath + "**/*.o", recursive=True)]

fullSize = 0
dataSize = 0

for o_file in o_files:
    with open(o_file, 'rb') as f:
        elfFile = ELFFile(f)

        for section in elfFile.iter_sections():
            if section.name == ".text" or section.name == ".init":
                fullSize += section['sh_size']
            elif section.name in dataSections:
                dataSize += section['sh_size']

codeCompletionPcnt = (fullSize / 0x527760)
dataCompletionPcnt = (dataSize / 0x194130)
bytesPerStar = 0x527760 / 120
starCount = math.floor(fullSize / bytesPerStar)

print("Progress:")
print("\tCode sections: {} / {} bytes in src ({:%})".format(fullSize, 0x527760, codeCompletionPcnt))
print("\tData sections: {} / {} bytes in src ({:%})".format(dataSize, 0x194130, dataCompletionPcnt))
print("\nYou have {} out of 120 star(s).".format(starCount))