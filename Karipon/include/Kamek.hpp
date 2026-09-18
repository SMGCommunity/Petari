#pragma once

#include "revolution/types.h"

#ifdef __MWERKS__
#pragma section RX ".kamek"
#endif

#define kmIdWrite 1
#define kmIdConditionalWrite 2
#define kmIdInjectBranch 3
#define kmIdInjectCall 4
#define kmIdPatchExit 5

typedef const unsigned char kmSymbol;

#define kmConcat(a, b) a##b
#define kmIdentifier(key, counter) kmConcat(_k##key, counter)

#define kmHookStruct(size) static const u32 kmIdentifier(Hook, __COUNTER__)[size] __attribute__((section(".kamek"))) __attribute__((used))

#define kmHandleSymbol(symbol, offset) (reinterpret_cast<kmSymbol*>(symbol) + (offset))

#define kmHookStruct0(type) kmHookStruct(2) = {0, (type)}
#define kmHookStruct1(type, arg0) kmHookStruct(6) = {1, (type), (u32)(arg0)}
#define kmHookStruct2(type, arg0, arg1) kmHookStruct(6) = {2, (type), (u32)(arg0), (u32)(arg1)}
#define kmHookStruct3(type, arg0, arg1, arg2) kmHookStruct(6) = {3, (type), (u32)(arg0), (u32)(arg1), (u32)(arg2)}
#define kmHookStruct4(type, arg0, arg1, arg2, arg3) kmHookStruct(6) = {4, (type), (u32)(arg0), (u32)(arg1), (u32)(arg2), (u32)(arg3)}

#define kmWritePointer(addr, ptr) kmHookStruct3(kmIdWrite, 1, (addr), (ptr))
#define kmWrite32(addr, value) kmHookStruct3(kmIdWrite, 2, (addr), (value))
#define kmWrite16(addr, value) kmHookStruct3(kmIdWrite, 3, (addr), (value))
#define kmWrite8(addr, value) kmHookStruct3(kmIdWrite, 4, (addr), (value))

#define kmSymWritePointer(symbol, offset, value) kmWritePointer(kmHandleSymbol(symbol, offset), value);
#define kmSymWrite32(symbol, offset, value) kmWrite32(kmHandleSymbol(symbol, offset), value);
#define kmSymWrite16(symbol, offset, value) kmWrite16(kmHandleSymbol(symbol, offset), value);
#define kmSymWrite8(symbol, offset, value) kmWrite8(kmHandleSymbol(symbol, offset), value);

#define kmCondWritePointer(addr, original, value) kmHookStruct4(kmIdConditionalWrite, 1, (addr), (value), (original))
#define kmCondWrite32(addr, original, value) kmHookStruct4(kmIdConditionalWrite, 2, (addr), (value), (original))
#define kmCondWrite16(addr, original, value) kmHookStruct4(kmIdConditionalWrite, 3, (addr), (value), (original))
#define kmCondWrite8(addr, original, value) kmHookStruct4(kmIdConditionalWrite, 4, (addr), (value), (original))

#define kmSymCondWritePointer(symbol, offset, value, original) kmCondWritePointer(kmHandleSymbol(symbol, offset), value, original);
#define kmSymCondWrite32(symbol, offset, value, original) kmCondWrite32(kmHandleSymbol(symbol, offset), value, original);
#define kmSymCondWrite16(symbol, offset, value, original) kmCondWrite16(kmHandleSymbol(symbol, offset), value, original);
#define kmSymCondWrite8(symbol, offset, value, original) kmCondWrite8(kmHandleSymbol(symbol, offset), value, original);

#define kmBranch(addr, ptr) kmHookStruct2(kmIdInjectBranch, (addr), reinterpret_cast<u32*>(&ptr))
#define kmCall(addr, ptr) kmHookStruct2(kmIdInjectCall, (addr), reinterpret_cast<u32*>(&ptr))

#define kmSymBranch(symbol, offset, value) kmBranch(kmHandleSymbol(symbol, offset), value);
#define kmSymCall(symbol, offset, value) kmCall(kmHandleSymbol(symbol, offset), value);

#define PPC_B(offset) (0x48000000 | ((offset) & 0x03FFFFFC))
#define PPC_BL(offset) (PPC_B(offset) | 1)
#define PPC_BLR 0x4E800020
#define PPC_COND_GT 0x4181
#define PPC_COND_LT 0x4180
#define PPC_COND_EQ 0x4182
#define PPC_COND_GEQ 0x4080
#define PPC_COND_LEQ 0x4081
#define PPC_COND_NE 0x4082
#define PPC_COND_BRANCH(cond, off) ((cond << 16) | (off & 0xFFFC))

#define PPC_NOP 0x60000000

#define PPC_LI(reg, val) (0x38000000 + 0x200000 * reg + (val & 0xFFFF))
#define PPC_LIS(reg, val) (0x3C000000 + 0x200000 * reg + (val & 0xFFFF))

#define PPC_MR(dst, src) (0x7C000000 + (src << 21) + (dst << 16) + (src << 11) + (444 << 1))

#define PPC_ADDI(dst, src, val) (0x38000000 + (dst << 21) + (src << 16) + (val & 0xFFFF))

#define PPC_STW(src, off, dst) (0x90000000 + (src << 21) + (dst << 16) + (off & 0xFFFF))
#define PPC_STH(src, off, dst) PPC_STW(src, off, dst) | (1 << 29)
#define PPC_STB(src, off, dst) PPC_STW(src, off, dst) | (1 << 27)
#define PPC_STFS(src, off, dst) PPC_STW(src, off, dst) | (1 << 30)

#define PPC_LWZ(dst, off, src) (0x80000000 + (dst << 21) + (src << 16) + (off & 0xFFFF))
#define PPC_LHZ(dst, off, src) PPC_LWZ(dst, off, src) | (1 << 29)
#define PPC_LBZ(dst, off, src) PPC_LWZ(dst, off, src) | (1 << 27)
#define PPC_LFS(dst, off, src) PPC_LWZ(dst, off, src) | (1 << 30)

#define PPC_CMPWI(src, val) ((0x2C000000) + (src << 16) + (val & 0xFFFF))
#define PPC_CMPLWI(src, val) (PPC_CMPWI(src, val) - 0x04000000)
#define PPC_CMPW(src1, src2) ((0x7C000000) + (src1 << 16) + (src2 << 11))