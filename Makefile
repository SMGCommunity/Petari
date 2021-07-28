ifneq ($(findstring MINGW,$(shell uname)),)
  WINDOWS := 1
endif
ifneq ($(findstring MSYS,$(shell uname)),)
  WINDOWS := 1
endif

#-------------------------------------------------------------------------------
# Files
#-------------------------------------------------------------------------------

# Used for elf2dol
TARGET_COL := wii

TARGET := smg1_us

BUILD_DIR := build/$(TARGET)

SRC_DIRS := src src/runtime_libs/debugger/embedded/MetroTRK/Os/dolphin

ASM_DIRS := asm asm/runtime_libs/debugger/embedded/MetroTRK/Portable  \
			asm/runtime_libs/debugger/embedded/MetroTRK/Export \
			asm/runtime_libs/debugger/embedded/MetroTRK/Os/dolphin \
			asm/runtime_libs/debugger/embedded/MetroTRK/Processor/ppc/Generic \
			asm/runtime_libs/gamedev/cust_connection/cc/exi2/GCN/EXI2_GDEV_GCN \
			asm/runtime_libs/gamedev/cust_connection/utils/common \
			asm/runtime_libs/gamedev/cust_connection/utils/gc \
			asm/ndev \
			asm/MSL/MSL_C/MSL_Common_Embedded/Math/Double_precision \
			asm/MSL/MSL_C/PPC_EABI/Runtime \
			asm/MSL/MSL_C/PPC_EABI/SRC \
			asm/MSL/MSL_C/PPC_EABI \
			asm/RVL \
			asm/RevoEx \
			asm/Game/JAudio2 \
			asm/nw4r  \
			asm/Game/J2D \
			asm/Game/J3D \
			asm/Game/JGadget \
			asm/Game/JKernel \
			asm/Game/JMath \
			asm/Game/JParticle \
			asm/Game/JSupport \
			asm/Game/JUtility \
			asm/Game/NWC24

# Inputs
S_FILES := $(foreach dir,$(ASM_DIRS),$(wildcard $(dir)/*.s))
C_FILES := $(foreach dir,$(SRC_DIRS),$(wildcard $(dir)/*.c))
CPP_FILES := $(foreach dir,$(SRC_DIRS),$(wildcard $(dir)/*.cpp))
LDSCRIPT := $(BUILD_DIR)/ldscript.lcf

# Outputs
DOL     := $(BUILD_DIR)/smg1_us.dol
ELF     := $(DOL:.dol=.elf)
MAP     := $(BUILD_DIR)/smg1_us.map

include obj_files.mk

O_FILES := $(INIT_O_FILES) $(EXTAB_O_FILES) $(EXTABINDEX_O_FILES) $(TEXT_O_FILES) \
           $(CTORS_O_FILES) $(DTORS_O_FILES) $(RODATA_O_FILES) $(DATA_O_FILES)    \
           $(BSS_O_FILES) $(SDATA_O_FILES) $(SDATA2_O_FILES) $(SBSS2_O_FILES)

#-------------------------------------------------------------------------------
# Tools
#-------------------------------------------------------------------------------

# Programs
ifeq ($(WINDOWS),1)
  WINE :=
else
  WINE := wine
endif
AS      := $(WINE) tools/powerpc-eabi-as.exe
OBJCOPY := $(WINE) tools/powerpc-eabi-objcopy.exe
CPP     := cpp -P
CC      := $(WINE) tools/mwcceppc.exe
LD      := $(WINE) tools/mwldeppc.exe
ELF2DOL := tools/elf2dol
SHA1SUM := sha1sum
PYTHON  := python3

#POSTPROC := tools/postprocess.py

# Options
INCLUDES := -i . -I- -i include

ASFLAGS := -mgekko -I include
LDFLAGS := -map $(MAP) -fp hard -nodefaults
CFLAGS  := -Cpp_exceptions off -proc gekko -fp hard -Os -nodefaults -msgstyle gcc $(INCLUDES)

# for postprocess.py
PROCFLAGS := -fprologue-fixup=old_stack

# elf2dol needs to know these in order to calculate sbss correctly.
SDATA_PDHR := 9
SBSS_PDHR := 10

#-------------------------------------------------------------------------------
# Recipes
#-------------------------------------------------------------------------------

### Default target ###

default: all

all: $(DOL)

ALL_DIRS := build $(BUILD_DIR) $(addprefix $(BUILD_DIR)/,$(SRC_DIRS) $(ASM_DIRS))

# Make sure build directory exists before compiling anything
DUMMY != mkdir -p $(ALL_DIRS)

.PHONY: tools

$(LDSCRIPT): ldscript.lcf
	$(CPP) -MMD -MP -MT $@ -MF $@.d -I include/ -I . -DBUILD_DIR=$(BUILD_DIR) -o $@ $<

$(DOL): $(ELF) | tools
	$(ELF2DOL) $< $@ $(SDATA_PDHR) $(SBSS_PDHR) $(TARGET_COL)
	$(SHA1SUM) -c $(TARGET).sha1

clean:
	rm -f -d -r build
	$(MAKE) -C tools clean

tools:
	$(MAKE) -C tools

$(ELF): $(O_FILES) $(LDSCRIPT)
	$(LD) $(LDFLAGS) -o $@ -lcf $(LDSCRIPT) $(O_FILES)
# The Metrowerks linker doesn't generate physical addresses in the ELF program headers. This fixes it somehow.
	$(OBJCOPY) $@ $@

$(BUILD_DIR)/%.o: %.s
	$(AS) $(ASFLAGS) -o $@ $<

$(BUILD_DIR)/%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

$(BUILD_DIR)/%.o: %.cpp
	$(CC) $(CFLAGS) -c -o $@ $<
#$(PYTHON) $(POSTPROC) $(PROCFLAGS) $@