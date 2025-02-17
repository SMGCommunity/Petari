#include "revolution/gx/GXTev.h"
#include "revolution/gx/GXRegs.h"
#include <revolution/gx.h>

static tev_color_env_t TEVCOpTableST0[] = {
    {
        0xC0,
        GX_TEVPREV,
        GX_CS_SCALE_1,
        GX_TRUE,
        GX_TEV_ADD,
        GX_TB_ZERO,
        GX_CC_ZERO,
        GX_CC_TEXC,
        GX_CC_RASC,
        GX_CC_ZERO  
    },
    {
        0xC0,
        GX_TEVPREV,
        GX_CS_SCALE_1,
        GX_TRUE,
        GX_TEV_ADD,
        GX_TB_ZERO,
        GX_CC_RASC,
        GX_CC_TEXC,
        GX_CC_TEXA,
        GX_CC_ZERO  
    },
    {
        0xC0,
        GX_TEVPREV,
        GX_CS_SCALE_1,
        GX_TRUE,
        GX_TEV_ADD,
        GX_TB_ZERO,
        GX_CC_RASC,
        GX_CC_ONE, 
        GX_CC_TEXC,
        GX_CC_ZERO  
    },
    {
        0xC0,
        GX_TEVPREV,
        GX_CS_SCALE_1,
        GX_TRUE,
        GX_TEV_ADD,
        GX_TB_ZERO,
        GX_CC_ZERO,
        GX_CC_ZERO,
        GX_CC_ZERO,
        GX_CC_TEXC  
    },
    {
        0xC0,
        GX_TEVPREV,
        GX_CS_SCALE_1,
        GX_TRUE,
        GX_TEV_ADD,
        GX_TB_ZERO,
        GX_CC_ZERO,
        GX_CC_ZERO,
        GX_CC_ZERO,
        GX_CC_RASC  
    },
};

static tev_color_env_t TEVCOpTableST1[] =
{
    {
        0xC0,
        GX_TEVPREV,
        GX_CS_SCALE_1,
        GX_TRUE,
        GX_TEV_ADD,
        GX_TB_ZERO,
        GX_CC_ZERO,
        GX_CC_TEXC,
        GX_CC_CPREV,
        GX_CC_ZERO  
    },
    {
        0xC0,
        GX_TEVPREV,
        GX_CS_SCALE_1,
        GX_TRUE,
        GX_TEV_ADD,
        GX_TB_ZERO,
        GX_CC_CPREV,
        GX_CC_TEXC,
        GX_CC_TEXA,
        GX_CC_ZERO  
    },
    {
        0xC0,
        GX_TEVPREV,
        GX_CS_SCALE_1,
        GX_TRUE,
        GX_TEV_ADD,
        GX_TB_ZERO,
        GX_CC_CPREV,
        GX_CC_ONE, 
        GX_CC_TEXC,
        GX_CC_ZERO  
    },
    {
        0xC0,
        GX_TEVPREV,
        GX_CS_SCALE_1,
        GX_TRUE,
        GX_TEV_ADD,
        GX_TB_ZERO,
        GX_CC_ZERO,
        GX_CC_ZERO,
        GX_CC_ZERO,
        GX_CC_TEXC  
    },
    {
        0xC0,
        GX_TEVPREV,
        GX_CS_SCALE_1,
        GX_TRUE,
        GX_TEV_ADD,
        GX_TB_ZERO,
        GX_CC_ZERO,
        GX_CC_ZERO,
        GX_CC_ZERO,
        GX_CC_CPREV 
    },
};

static tev_alpha_env_t TEVAOpTableST0[] =
{
    {
        0xC1,
        GX_TEVPREV,
        GX_CS_SCALE_1,
        GX_TRUE,
        GX_TEV_ADD,
        GX_TB_ZERO,
        GX_CA_ZERO,
        GX_CA_TEXA,
        GX_CA_RASA,
        GX_CA_ZERO, 
        GX_TEV_SWAP0,
        GX_TEV_SWAP0,
    },
    {
        0xC1,
        GX_TEVPREV,
        GX_CS_SCALE_1,
        GX_TRUE,
        GX_TEV_ADD,
        GX_TB_ZERO,
        GX_CA_ZERO,
        GX_CA_ZERO,
        GX_CA_ZERO,
        GX_CA_RASA, 
        GX_TEV_SWAP0,
        GX_TEV_SWAP0,
    },
    {
        0xC1,
        GX_TEVPREV,
        GX_CS_SCALE_1,
        GX_TRUE,
        GX_TEV_ADD,
        GX_TB_ZERO,
        GX_CA_ZERO,
        GX_CA_TEXA,
        GX_CA_RASA,
        GX_CA_ZERO, 
        GX_TEV_SWAP0,
        GX_TEV_SWAP0,
    },
    {
        0xC1,
        GX_TEVPREV,
        GX_CS_SCALE_1,
        GX_TRUE,
        GX_TEV_ADD,
        GX_TB_ZERO,
        GX_CA_ZERO,
        GX_CA_ZERO,
        GX_CA_ZERO,
        GX_CA_TEXA, 
        GX_TEV_SWAP0,
        GX_TEV_SWAP0,
    },
    {
        0xC1,
        GX_TEVPREV,
        GX_CS_SCALE_1,
        GX_TRUE,
        GX_TEV_ADD,
        GX_TB_ZERO,
        GX_CA_ZERO,
        GX_CA_ZERO,
        GX_CA_ZERO,
        GX_CA_RASA, 
        GX_TEV_SWAP0,
        GX_TEV_SWAP0,
    },
};

static tev_alpha_env_t TEVAOpTableST1[] =
{
    {
        0xC1,
        GX_TEVPREV,
        GX_CS_SCALE_1,
        GX_TRUE,
        GX_TEV_ADD,
        GX_TB_ZERO,
        GX_CA_ZERO,
        GX_CA_TEXA,
        GX_CA_APREV,
        GX_CA_ZERO, 
        GX_TEV_SWAP0,
        GX_TEV_SWAP0,
    },
    {
        0xC1,
        GX_TEVPREV,
        GX_CS_SCALE_1,
        GX_TRUE,
        GX_TEV_ADD,
        GX_TB_ZERO,
        GX_CA_ZERO,
        GX_CA_ZERO,
        GX_CA_ZERO,
        GX_CA_APREV,
        GX_TEV_SWAP0,
        GX_TEV_SWAP0,
    },
    {
        0xC1,
        GX_TEVPREV,
        GX_CS_SCALE_1,
        GX_TRUE,
        GX_TEV_ADD,
        GX_TB_ZERO,
        GX_CA_ZERO,
        GX_CA_TEXA,
        GX_CA_APREV,
        GX_CA_ZERO, 
        GX_TEV_SWAP0,
        GX_TEV_SWAP0,
    },
    {
        0xC1,
        GX_TEVPREV,
        GX_CS_SCALE_1,
        GX_TRUE,
        GX_TEV_ADD,
        GX_TB_ZERO,
        GX_CA_ZERO,
        GX_CA_ZERO,
        GX_CA_ZERO,
        GX_CA_TEXA, 
        GX_TEV_SWAP0,
        GX_TEV_SWAP0,
    },
    {
        0xC1,
        GX_TEVPREV,
        GX_CS_SCALE_1,
        GX_TRUE,
        GX_TEV_ADD,
        GX_TB_ZERO,
        GX_CA_ZERO,
        GX_CA_ZERO,
        GX_CA_ZERO,
        GX_CA_APREV,
        GX_TEV_SWAP0,
        GX_TEV_SWAP0,
    },
};

void GXSetTevOp(GXTevStageID id, GXTevMode mode) {
    u32 *ctmp, *atmp, tevReg;

    if (id == GX_TEVSTAGE0) {
        ctmp = (u32*)&TEVCOpTableST0[mode];
        atmp = (u32*)&TEVAOpTableST0[mode];
    }
    else {
        ctmp = (u32*)&TEVCOpTableST1[mode];
        atmp = (u32*)&TEVAOpTableST1[mode];
    }

    tevReg = gx->tevc[id];
    tevReg = ((*ctmp & ~0xFF000000) | (tevReg & 0xFF000000));
    GX_WRITE_RA_REG(tevReg);
    gx->tevc[id] = tevReg;

    tevReg = gx->teva[id];
    tevReg = ((*atmp & ~0xFF00000F) | (tevReg & 0xFF00000F));
    GX_WRITE_RA_REG(tevReg);
    gx->teva[id] = tevReg;
    gx->bpSentNot = GX_FALSE;
}
