#---------------------------------------------------------------------------------
# Petari 3DS bring-up build (New 3DS target). Requires devkitARM + libctru + citro3d.
#   make            -> petari3ds.3dsx
#   make clean
# Add files to SRCS as they become portable. Paths are relative to the repo root.
#---------------------------------------------------------------------------------
.SUFFIXES:

ifeq ($(strip $(DEVKITARM)),)
$(error "Set DEVKITARM first, e.g. export DEVKITARM=/opt/devkitpro/devkitARM")
endif

include $(DEVKITARM)/3ds_rules

# Game version the decomp is built for: 0=JPN(RMGJ01) 1=USA(RMGE01) 2=PAL(RMGP01) 3=KOR(RMGK01)
GAME_VERSION_NUM ?= 1

TARGET   := petari3ds
BUILD    := build
ROOT     := ..
CTRULIB  := $(DEVKITPRO)/libctru

# Files from the decomp that currently build for ARM (grow this list).
SRCS := \
	$(ROOT)/src/JSystem/JMath/JMath.cpp \
	$(ROOT)/src/JSystem/JMath/JMATrigonometric.cpp \
	$(ROOT)/src/JSystem/JMath/random.cpp

LOCAL_SRCS := source/main.cpp

INCLUDE := \
	-I$(CTRULIB)/include \
	-I$(ROOT)/include \
	-I$(ROOT)/libs/JSystem/include \
	-I$(ROOT)/libs/RVL_SDK/include \
	-I$(ROOT)/libs/MSL_C/include \
	-I$(ROOT)/libs/MSL_C++/include \
	-I$(ROOT)/libs/Runtime/include \
	-I$(ROOT)/libs/nw4r/include \
	-I$(ROOT)/libs/RVLFaceLib/include \
	-Icompat -include compat/petari_compat.h

# New 3DS = ARM11 MPCore (armv6k). Hard-float VFP11.
ARCH     := -march=armv6k -mtune=mpcore -mfloat-abi=hard -mtp=soft
COMMON   := -g -O2 -Wall -mword-relocations -ffunction-sections -fdata-sections $(ARCH) -D__3DS__ $(INCLUDE)
CFLAGS   := $(COMMON) -std=gnu11
CXXFLAGS := $(COMMON) -std=gnu++14 -Wno-unknown-pragmas -Wno-register -Wno-narrowing -D__fabs=__builtin_fabs \
	-DVERSION=$(GAME_VERSION_NUM) -DRMGJ01=0 -DRMGE01=1 -DRMGP01=2 -DRMGK01=3 -DNDEBUG=1 -DREVOLUTION
LDFLAGS  := -specs=3dsx.specs -g $(ARCH) -Wl,--gc-sections
LIBS     := -L$(CTRULIB)/lib -lcitro3d -lctru -lm

OBJS := $(patsubst %.cpp,$(BUILD)/%.o,$(subst ../,up/,$(SRCS)) $(LOCAL_SRCS))

all: $(TARGET).3dsx

$(BUILD)/up/%.o: $(ROOT)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(BUILD)/%.o: %.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(TARGET).elf: $(OBJS)
	$(CXX) $(LDFLAGS) $(OBJS) $(LIBS) -o $@

clean:
	rm -rf $(BUILD) $(TARGET).elf $(TARGET).3dsx

.PHONY: all clean
