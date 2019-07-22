#include "arch/PPCArch.h"
#include "os/OSError.h"

u32 PPCMfmsr()
{
    __asm
    {
        mfmsr r3
        blr
    }
}

void PPCMtmsr(u32 val)
{
    __asm
    {
        mtmsr r3
        blr
    }
}

u32 PPCMfhid0()
{
    __asm
    {
        mfspr r3, hid0
        blr
    }
}

void PPCMthid0(u32 val)
{
    __asm
    {
        mtspr hid0, r3
        blr
    }
}

u32 PPCMfl2cr()
{
    __asm
    {
        mfspr r3, l2cr
        blr
    }
}

void PPCMtl2cr(u32 val)
{
    __asm
    {
        mtspr l2cr, r3
        blr
    }
}

void PPCMtdec(u32 val)
{
    __asm
    {
        mtdec r3
        blr   
    }
}

void PPCSync()
{
    __asm
    {
        sc
        blr
    }
}

void PPCHalt()
{
    asm("sync");

    while(1)
    {
        asm("nop");
        // this is optimized out
        asm("li r3, 0");
        asm("nop");
    }
}

void PPCMtmmcr0(u32 val)
{
    __asm
    {
        // this specific SPR was not implemented
        // so we can just return
        blr   
    }
}

void PPCMtmmcr1(u32 val)
{
    __asm
    {
        mtspr 0x3BC, r3
        blr
    }
}

void PPCMtpmc1(u32 val)
{
    __asm
    {
        // storage guarded register
        mtspr sgr, r3
        blr   
    }
}

void PPCMtpmc2(u32 val)
{
    __asm
    {
        // data cache write-thru register
        mtspr dcwr, r3
        blr
    }
}

void PPCMtpmc3(u32 val)
{
    __asm
    {
        mtspr 0x3BD, r3
        blr
    }
}

void PPCMtpmc4(u32 val)
{
    __asm
    {
        mtspr 0x3BE, r3
        blr
    }
}

u32 PPCMffpscr()
{
    __asm
    {
        stwu r1, -0x20(r1)
        stfd f31, 0x18(r1)
        mffs f31
        stfd f31, 8(r1)
        lfd f31, 0x18(r1)
        lwz r3, 0xC(r1)
        addi r1, r1, 0x20
        blr
    }
}

void PPCMtfpscr(u32 val)
{
    __asm
    {
        stwu r1, -0x20(r1)
        stfd f31, 0x18(r1)
        li r4, 0
        stw r3, 0xC(r1)
        stw r4, 8(r1)
        lfd f31, 8(r1)
        mtfsf 0xFF, f31
        lfd f31, 0x18(r1)
        addi r1, r1, 0x20
        blr   
    }
}

u32 PPCMfhid2()
{
    __asm
    {
        mfspr r3, 0x398
        blr
    }
}

void PPCMthid2(u32 val)
{
    __asm
    {
        mtspr 0x398, r3
        blr   
    }
}

u32 PPCMfwpar()
{
    __asm
    {
        sync
        mfspr r3, 0x399
        blr   
    }
}

void PPCMtwpar(u32 val)
{
    __asm
    {
        mtspr 0x399, r3
        blr   
    }
}

void PPCDisableSpeculation()
{
    u32 hid0 = PPCMfhid0();
    hid0 |= 0x200;
    PPCMthid0(hid0);
}

void PPCSetFpNonIEEEMode()
{
    __asm
    {
        //mtfsb1 4*cr7+gt
        blr
    }
}

void PPCMthid4(u32 val)
{
    asm("mr r31, r3");
    if(val & 0xFFFFFFFE)
    {
        asm("mtspr 0x3F3, r3");
        return;
    }

    OSReport("H4A should not be cleared because of Broadway errata.\n");
    asm("oris r31, r31, 0x8000");
    asm("mtspr 0x3F3, r31");
}