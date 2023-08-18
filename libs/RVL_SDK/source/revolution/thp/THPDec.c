#include <revolution/thp.h>

static THPHuffmanTab* Ydchuff __attribute__((align(32)));
static THPHuffmanTab* Udchuff __attribute__((align(32)));
static THPHuffmanTab* Vdchuff __attribute__((align(32)));
static THPHuffmanTab* Yachuff __attribute__((align(32)));
static THPHuffmanTab* Uachuff __attribute__((align(32)));
static THPHuffmanTab* Vachuff __attribute__((align(32)));
static f32 __THPIDCTWorkspace[64] __attribute__((align(32)));
static u8* __THPHuffmanBits;
static u8* __THPHuffmanSizeTab;
static u16* __THPHuffmanCodeTab;
static THPSample* Gbase __attribute__((align(32)));
static u32 Gwid __attribute__((align(32)));
static f32* Gq __attribute__((align(32)));
static u8 *__THPLCWork512[3];
static u8 *__THPLCWork672[3];
static u32 __THPOldGQR5;
static u32 __THPOldGQR6;
static u8* __THPWorkArea;
static THPCoeff* __THPMCUBuffer[6];
static THPFileInfo* __THPInfo;
static BOOL __THPInitFlag = FALSE;
 
 void __THPDecompressiMCURow640x480(void) {
    u8 cl_num;
    u32 x_pos;
    THPComponent* comp;

    LCQueueWait(3);

    {
        for (cl_num = 0; cl_num < __THPInfo->MCUsPerRow; cl_num++) {
            THPFileInfo* um = __THPInfo;
            __THPHuffDecodeDCTCompY(um, __THPMCUBuffer[0]);
            __THPHuffDecodeDCTCompY(__THPInfo, __THPMCUBuffer[1]);
            __THPHuffDecodeDCTCompY(__THPInfo, __THPMCUBuffer[2]);
            __THPHuffDecodeDCTCompY(__THPInfo, __THPMCUBuffer[3]);
            __THPHuffDecodeDCTCompU(__THPInfo, __THPMCUBuffer[4]);
            __THPHuffDecodeDCTCompV(__THPInfo, __THPMCUBuffer[5]); 

            comp = &__THPInfo->components[0];
            Gbase = __THPLCWork672[0];
            Gwid = 640;
            Gq = __THPInfo->quantTabs[comp->quantizationTableSelector];
            x_pos = (u32)(cl_num * 16);
            __THPInverseDCTNoYPos(__THPMCUBuffer[0], x_pos);
            __THPInverseDCTNoYPos(__THPMCUBuffer[1], x_pos + 8);
            __THPInverseDCTY8(__THPMCUBuffer[2], x_pos);
            __THPInverseDCTY8(__THPMCUBuffer[3], x_pos + 8);  

            comp = &__THPInfo->components[1];
            Gbase = __THPLCWork672[1];
            Gwid = 320;
            Gq = __THPInfo->quantTabs[comp->quantizationTableSelector];
            x_pos  /= 2;
            __THPInverseDCTNoYPos(__THPMCUBuffer[4], x_pos);

            comp = &__THPInfo->components[2];
            Gbase = __THPLCWork672[2];
            Gq = __THPInfo->quantTabs[comp->quantizationTableSelector];
            __THPInverseDCTNoYPos(__THPMCUBuffer[5], x_pos);

            if (__THPInfo->RST != 0) {
                __THPInfo->currMCU --;
                if (__THPInfo->currMCU == 0) {
                    __THPInfo->currMCU = __THPInfo->nMCU ;

                    __THPInfo->cnt =  1 + ((__THPInfo->cnt + 6) & 0xFFFFFFF8);

                    if (__THPInfo->cnt > 32) {
                        __THPInfo->cnt = 33;
                    }

                    __THPInfo->components[0].predDC = 0 ;
                    __THPInfo->components[1].predDC = 0 ;
                    __THPInfo->components[2].predDC = 0 ;
                }
            }
        }
    }

    LCStoreData(__THPInfo->dLC[0], __THPLCWork672[0], 0x2800);
    LCStoreData(__THPInfo->dLC[1], __THPLCWork672[1], 0xA00);
    LCStoreData(__THPInfo->dLC[2], __THPLCWork672[2], 0xA00);

    __THPInfo->dLC[0] += 0x2800;
    __THPInfo->dLC[1] += 0xA00;
    __THPInfo->dLC[2] += 0xA00;
}