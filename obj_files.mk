INIT_O_FILES := 						            \
	$(BUILD_DIR)/asm/init.o

EXTAB_O_FILES :=                                    \
    $(BUILD_DIR)/asm/extab.o

EXTABINDEX_O_FILES :=                               \
    $(BUILD_DIR)/asm/extabindex.o

TEXT_O_FILES := 					            \
    $(BUILD_DIR)/asm/nw4r/libnw4r_ut.o   \
    $(BUILD_DIR)/asm/nw4r/libnw4r_db.o   \
    $(BUILD_DIR)/asm/nw4r/libnw4r_math.o   \
    $(BUILD_DIR)/asm/nw4r/libnw4r_lyt.o   \
	$(BUILD_DIR)/asm/text1.o                        \
    $(BUILD_DIR)/asm/Game/NWC24/NWC24.o \
    $(BUILD_DIR)/asm/Game/JKernel/JKernel.o \
    $(BUILD_DIR)/asm/Game/JSupport/JSupport.o \
    $(BUILD_DIR)/asm/Game/JGadget/JGadget.o \
    $(BUILD_DIR)/asm/Game/JUtility/JUtility.o \
    $(BUILD_DIR)/asm/Game/J2D/J2DGraph.o \
    $(BUILD_DIR)/asm/Game/J3D/J3DGraphBase.o \
    $(BUILD_DIR)/asm/Game/J3D/J3DGraphAnimator.o \
    $(BUILD_DIR)/asm/Game/J3D/J3DGraphLoader.o \
    $(BUILD_DIR)/asm/Game/JMath/JMath.o \
    $(BUILD_DIR)/asm/Game/JParticle/JParticle.o \
    $(BUILD_DIR)/asm/RVL/gd.o   \
    $(BUILD_DIR)/asm/RVL/kpad.o   \
    $(BUILD_DIR)/asm/RVL/thp.o   \
    $(BUILD_DIR)/asm/RVL/tpl.o   \
    $(BUILD_DIR)/asm/RVL/wenc.o   \
    $(BUILD_DIR)/asm/RVL/rso.o   \
    $(BUILD_DIR)/asm/RVL/RVLFaceLib.o   \
    $(BUILD_DIR)/asm/RevoEx/net.o   \
    $(BUILD_DIR)/asm/RevoEx/nwc24.o   \
    $(BUILD_DIR)/asm/RevoEx/vf.o   \
    $(BUILD_DIR)/asm/Game/JAudio2/JAudio2.o   \
    $(BUILD_DIR)/asm/RVL/aralt.o   \
    $(BUILD_DIR)/asm/RVL/base.o   \
    $(BUILD_DIR)/asm/RVL/os.o   \
    $(BUILD_DIR)/asm/RVL/exi.o   \
    $(BUILD_DIR)/asm/RVL/si.o   \
    $(BUILD_DIR)/asm/RVL/db.o   \
    $(BUILD_DIR)/asm/RVL/vi.o   \
    $(BUILD_DIR)/asm/RVL/mtx.o   \
    $(BUILD_DIR)/asm/RVL/gx.o   \
    $(BUILD_DIR)/asm/RVL/dvd.o   \
    $(BUILD_DIR)/asm/RVL/ai.o   \
    $(BUILD_DIR)/asm/RVL/ax.o   \
    $(BUILD_DIR)/asm/RVL/axfx.o   \
    $(BUILD_DIR)/asm/RVL/mem.o   \
    $(BUILD_DIR)/asm/RVL/code_80CA34C.o   \
    $(BUILD_DIR)/asm/RVL/dsp.o   \
    $(BUILD_DIR)/asm/RVL/nand.o   \
    $(BUILD_DIR)/asm/RVL/sc.o   \
    $(BUILD_DIR)/asm/RVL/arc.o   \
    $(BUILD_DIR)/asm/RVL/esp.o   \
    $(BUILD_DIR)/asm/RVL/ipc.o   \
    $(BUILD_DIR)/asm/RVL/fs.o   \
    $(BUILD_DIR)/asm/RVL/pad.o   \
    $(BUILD_DIR)/asm/RVL/wpad.o   \
    $(BUILD_DIR)/asm/RVL/euart.o   \
    $(BUILD_DIR)/asm/RVL/usb.o   \
    $(BUILD_DIR)/asm/RVL/wud.o   \
    $(BUILD_DIR)/asm/RVL/bte.o   \
    $(BUILD_DIR)/asm/MSL/MSL_C/PPC_EABI/Runtime/__mem.o   \
    $(BUILD_DIR)/asm/MSL/MSL_C/PPC_EABI/Runtime/__va_arg.o   \
    $(BUILD_DIR)/asm/MSL/MSL_C/PPC_EABI/Runtime/global_destructor_chain.o   \
    $(BUILD_DIR)/asm/MSL/MSL_C/PPC_EABI/Runtime/NMWException.o   \
    $(BUILD_DIR)/asm/MSL/MSL_C/PPC_EABI/Runtime/ptmf.o   \
    $(BUILD_DIR)/asm/MSL/MSL_C/PPC_EABI/Runtime/runtime.o   \
    $(BUILD_DIR)/asm/MSL/MSL_C/PPC_EABI/Runtime/__init_cpp_exceptions.o   \
    $(BUILD_DIR)/asm/MSL/MSL_C/PPC_EABI/Runtime/Gecko_ExceptionPPC.o   \
    $(BUILD_DIR)/asm/MSL/MSL_C/PPC_EABI/Runtime/GCN_mem_alloc.o   \
    $(BUILD_DIR)/asm/MSL/MSL_C/PPC_EABI/alloc.o   \
    $(BUILD_DIR)/asm/MSL/MSL_C/PPC_EABI/ansi_files.o   \
    $(BUILD_DIR)/asm/MSL/MSL_C/PPC_EABI/ansi_fp.o   \
    $(BUILD_DIR)/asm/MSL/MSL_C/PPC_EABI/arith.o   \
    $(BUILD_DIR)/asm/MSL/MSL_C/PPC_EABI/buffer_io.o   \
    $(BUILD_DIR)/asm/MSL/MSL_C/PPC_EABI/ctype.o   \
    $(BUILD_DIR)/asm/MSL/MSL_C/PPC_EABI/direct_io.o   \
    $(BUILD_DIR)/asm/MSL/MSL_C/PPC_EABI/file_io.o   \
    $(BUILD_DIR)/asm/MSL/MSL_C/PPC_EABI/FILE_POS.o   \
    $(BUILD_DIR)/asm/MSL/MSL_C/PPC_EABI/mbstring.o   \
    $(BUILD_DIR)/asm/MSL/MSL_C/PPC_EABI/mem.o   \
    $(BUILD_DIR)/asm/MSL/MSL_C/PPC_EABI/mem_funcs.o   \
    $(BUILD_DIR)/asm/MSL/MSL_C/PPC_EABI/math_api.o   \
    $(BUILD_DIR)/asm/MSL/MSL_C/PPC_EABI/misc_io.o   \
    $(BUILD_DIR)/asm/MSL/MSL_C/PPC_EABI/printf.o   \
    $(BUILD_DIR)/asm/MSL/MSL_C/PPC_EABI/scanf.o   \
    $(BUILD_DIR)/asm/MSL/MSL_C/PPC_EABI/signal.o   \
    $(BUILD_DIR)/asm/MSL/MSL_C/PPC_EABI/string.o   \
    $(BUILD_DIR)/asm/MSL/MSL_C/PPC_EABI/strtold.o   \
    $(BUILD_DIR)/asm/MSL/MSL_C/PPC_EABI/strtoul.o   \
    $(BUILD_DIR)/asm/MSL/MSL_C/PPC_EABI/wmem.o   \
    $(BUILD_DIR)/asm/MSL/MSL_C/PPC_EABI/wprintf.o   \
    $(BUILD_DIR)/asm/MSL/MSL_C/PPC_EABI/wstring.o   \
    $(BUILD_DIR)/asm/MSL/MSL_C/PPC_EABI/wchar_io.o   \
    $(BUILD_DIR)/asm/MSL/MSL_C/PPC_EABI/uart_console_io_gcn.o   \
    $(BUILD_DIR)/asm/MSL/MSL_C/PPC_EABI/abort_exit_ppc_eabi.o   \
    $(BUILD_DIR)/asm/MSL/MSL_C/PPC_EABI/math_sun.o   \
    $(BUILD_DIR)/asm/MSL/MSL_C/PPC_EABI/extras.o   \
    $(BUILD_DIR)/asm/MSL/MSL_C/MSL_Common_Embedded/Math/Double_precision/e_acos.o   \
    $(BUILD_DIR)/asm/MSL/MSL_C/MSL_Common_Embedded/Math/Double_precision/e_asin.o   \
    $(BUILD_DIR)/asm/MSL/MSL_C/MSL_Common_Embedded/Math/Double_precision/e_atan2.o   \
    $(BUILD_DIR)/asm/MSL/MSL_C/MSL_Common_Embedded/Math/Double_precision/e_fmod.o   \
    $(BUILD_DIR)/asm/MSL/MSL_C/MSL_Common_Embedded/Math/Double_precision/e_log.o   \
    $(BUILD_DIR)/asm/MSL/MSL_C/MSL_Common_Embedded/Math/Double_precision/e_log10.o   \
    $(BUILD_DIR)/asm/MSL/MSL_C/MSL_Common_Embedded/Math/Double_precision/e_pow.o   \
    $(BUILD_DIR)/asm/MSL/MSL_C/MSL_Common_Embedded/Math/Double_precision/e_rem_pio2.o   \
    $(BUILD_DIR)/asm/MSL/MSL_C/MSL_Common_Embedded/Math/Double_precision/k_cos.o   \
    $(BUILD_DIR)/asm/MSL/MSL_C/MSL_Common_Embedded/Math/Double_precision/k_rem_pio2.o   \
    $(BUILD_DIR)/asm/MSL/MSL_C/MSL_Common_Embedded/Math/Double_precision/k_sin.o   \
    $(BUILD_DIR)/asm/MSL/MSL_C/MSL_Common_Embedded/Math/Double_precision/k_tan.o   \
    $(BUILD_DIR)/asm/MSL/MSL_C/MSL_Common_Embedded/Math/Double_precision/s_atan.o   \
    $(BUILD_DIR)/asm/MSL/MSL_C/MSL_Common_Embedded/Math/Double_precision/s_ceil.o   \
    $(BUILD_DIR)/asm/MSL/MSL_C/MSL_Common_Embedded/Math/Double_precision/s_copysign.o   \
    $(BUILD_DIR)/asm/MSL/MSL_C/MSL_Common_Embedded/Math/Double_precision/s_cos.o   \
    $(BUILD_DIR)/asm/MSL/MSL_C/MSL_Common_Embedded/Math/Double_precision/s_floor.o   \
    $(BUILD_DIR)/asm/MSL/MSL_C/MSL_Common_Embedded/Math/Double_precision/s_frexp.o   \
    $(BUILD_DIR)/asm/MSL/MSL_C/MSL_Common_Embedded/Math/Double_precision/s_ldexp.o   \
    $(BUILD_DIR)/asm/MSL/MSL_C/MSL_Common_Embedded/Math/Double_precision/s_sin.o   \
    $(BUILD_DIR)/asm/MSL/MSL_C/MSL_Common_Embedded/Math/Double_precision/s_tan.o   \
    $(BUILD_DIR)/asm/MSL/MSL_C/MSL_Common_Embedded/Math/Double_precision/w_acos.o   \
    $(BUILD_DIR)/asm/MSL/MSL_C/MSL_Common_Embedded/Math/Double_precision/w_asin.o   \
    $(BUILD_DIR)/asm/MSL/MSL_C/MSL_Common_Embedded/Math/Double_precision/w_atan2.o   \
    $(BUILD_DIR)/asm/MSL/MSL_C/MSL_Common_Embedded/Math/Double_precision/w_fmod.o   \
    $(BUILD_DIR)/asm/MSL/MSL_C/MSL_Common_Embedded/Math/Double_precision/w_log10.o   \
    $(BUILD_DIR)/asm/MSL/MSL_C/MSL_Common_Embedded/Math/Double_precision/w_pow.o   \
    $(BUILD_DIR)/asm/MSL/MSL_C/MSL_Common_Embedded/Math/Double_precision/e_sqrt.o   \
    $(BUILD_DIR)/asm/MSL/MSL_C/PPC_EABI/SRC/math_ppc.o   \
    $(BUILD_DIR)/asm/MSL/MSL_C/MSL_Common_Embedded/Math/Double_precision/w_sqrt.o   \
    $(BUILD_DIR)/asm/runtime_libs/debugger/embedded/MetroTRK/Portable/mainloop.o    \
    $(BUILD_DIR)/asm/runtime_libs/debugger/embedded/MetroTRK/Portable/nubevent.o    \
    $(BUILD_DIR)/asm/runtime_libs/debugger/embedded/MetroTRK/Portable/nubinit.o    \
    $(BUILD_DIR)/asm/runtime_libs/debugger/embedded/MetroTRK/Portable/msg.o    \
    $(BUILD_DIR)/asm/runtime_libs/debugger/embedded/MetroTRK/Portable/msgbuf.o    \
    $(BUILD_DIR)/asm/runtime_libs/debugger/embedded/MetroTRK/Portable/serpoll.o    \
    $(BUILD_DIR)/src/runtime_libs/debugger/embedded/MetroTRK/Os/dolphin/usr_put.o \
    $(BUILD_DIR)/asm/runtime_libs/debugger/embedded/MetroTRK/Os/dolphin/usr_put.o \
    $(BUILD_DIR)/asm/runtime_libs/debugger/embedded/MetroTRK/Portable/dispatch.o    \
    $(BUILD_DIR)/asm/runtime_libs/debugger/embedded/MetroTRK/Portable/msghndlr.o    \
    $(BUILD_DIR)/asm/runtime_libs/debugger/embedded/MetroTRK/Portable/support.o    \
    $(BUILD_DIR)/asm/runtime_libs/debugger/embedded/MetroTRK/Portable/mutex_TRK.o  \
    $(BUILD_DIR)/asm/runtime_libs/debugger/embedded/MetroTRK/Portable/notify.o     \
    $(BUILD_DIR)/asm/runtime_libs/debugger/embedded/MetroTRK/Processor/ppc/Generic/flush_cache.o \
    $(BUILD_DIR)/asm/runtime_libs/debugger/embedded/MetroTRK/Portable/mem_TRK.o    \
    $(BUILD_DIR)/asm/runtime_libs/debugger/embedded/MetroTRK/Portable/string_TRK.o \
    $(BUILD_DIR)/asm/runtime_libs/debugger/embedded/MetroTRK/Processor/ppc/Generic/targimpl.o \
    $(BUILD_DIR)/asm/runtime_libs/debugger/embedded/MetroTRK/Processor/ppc/Generic/targsupp.o \
    $(BUILD_DIR)/asm/runtime_libs/debugger/embedded/MetroTRK/Processor/ppc/Generic/mpc_7xx_603e.o \
    $(BUILD_DIR)/asm/runtime_libs/debugger/embedded/MetroTRK/Export/mslsupp.o      \
    $(BUILD_DIR)/asm/runtime_libs/debugger/embedded/MetroTRK/Os/dolphin/dolphin_trk.o  \
    $(BUILD_DIR)/asm/runtime_libs/debugger/embedded/MetroTRK/Portable/main_TRK.o   \
    $(BUILD_DIR)/asm/runtime_libs/debugger/embedded/MetroTRK/Os/dolphin/dolphin_trk_glue.o \
    $(BUILD_DIR)/asm/runtime_libs/debugger/embedded/MetroTRK/Os/dolphin/targcont.o \
    $(BUILD_DIR)/asm/runtime_libs/debugger/embedded/MetroTRK/Os/dolphin/target_options.o   \
    $(BUILD_DIR)/asm/runtime_libs/debugger/embedded/MetroTRK/Os/dolphin/UDP_Stubs.o    \
    $(BUILD_DIR)/asm/runtime_libs/gamedev/cust_connection/cc/exi2/GCN/EXI2_GDEV_GCN/main.o   \
    $(BUILD_DIR)/asm/runtime_libs/gamedev/cust_connection/utils/common/CircleBuffer.o   \
    $(BUILD_DIR)/asm/runtime_libs/gamedev/cust_connection/utils/gc/MWCriticalSection_gc.o  \
    $(BUILD_DIR)/asm/ndev/DebuggerDriver.o  \
    $(BUILD_DIR)/asm/ndev/exi2.o  \

CTORS_O_FILES :=                                    \
    $(BUILD_DIR)/asm/ctors.o

DTORS_O_FILES :=                                    \
    $(BUILD_DIR)/asm/dtors.o

RODATA_O_FILES :=                                   \
    $(BUILD_DIR)/asm/rodata.o

DATA_O_FILES :=                                     \
    $(BUILD_DIR)/asm/data.o

BSS_O_FILES :=                                      \
    $(BUILD_DIR)/asm/bss.o

SDATA_O_FILES :=                                    \
    $(BUILD_DIR)/asm/sdata.o

SDATA2_O_FILES :=                                   \
    $(BUILD_DIR)/asm/sdata2.o

SBSS2_O_FILES :=                                   \
    $(BUILD_DIR)/asm/sbss2.o