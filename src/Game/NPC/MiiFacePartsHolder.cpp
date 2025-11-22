#include "Game/LiveActor/LiveActor.hpp"
#include "Game/NPC/MiiDatabase.hpp"
#include "Game/NPC/MiiFaceParts.hpp"
#include "Game/NPC/MiiFacePartsHolder.hpp"
#include "Game/NPC/MiiFaceRecipe.hpp"
#include "Game/NameObj/NameObjAdaptor.hpp"
#include "Game/Scene/SceneObjHolder.hpp"
#include "Game/System/GameSequenceFunction.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/MemoryUtil.hpp"
#include "Game/Util/ObjUtil.hpp"
#include <JSystem/JKernel/JKRMemArchive.hpp>
#include <JSystem/JKernel/JKRSolidHeap.hpp>

MiiFacePartsHolder::MiiFacePartsHolder(int numParts)
    : LiveActorGroup("Mii顔モデル保持", numParts),
      JKRDisposer(),
      mRFLWorkBuffer(nullptr),
      _34(nullptr) {
}

MiiFacePartsHolder::~MiiFacePartsHolder() {
    RFLExit();
    delete[] mRFLWorkBuffer;
    delete _34;
}

void MiiFacePartsHolder::init(const JMapInfoIter& rIter) {
    mRFLWorkBuffer = new (MR::getSceneHeapGDDR3(), 32) u8[RFLGetWorkSize(false)];

    JKRMemArchive* pArchive = reinterpret_cast<JKRMemArchive*>(MR::receiveArchive("/ObjectData/MiiFaceDatabase.arc"));
    void*          pResBuffer = pArchive->getResource("/RFL_Res.dat");
    u32            resSize = pArchive->getResSize(pResBuffer);
    _38 = RFLInitResAsync(mRFLWorkBuffer, pResBuffer, resSize, false);

    MR::connectToScene(this, -1, 6, -1, 37);

    MR::FunctorV0M<MiiFacePartsHolder*, void (MiiFacePartsHolder::*)()> initFunc(
        this, &MiiFacePartsHolder::reinitCharModel);

    MR::connectToScene(MR::createDrawAdaptor("Miiモデル再作成", initFunc), -1, -1, -1, 80);
}

void MiiFacePartsHolder::calcAnim() {
    for (int i = 0; i < getObjectCount(); i++) {
        LiveActor* pActor = getActor(i);

        if (pActor == nullptr) {
            continue;
        }

        if (MR::isNoCalcAnim(pActor)) {
            continue;
        }

        pActor->calcAnim();
    }

    calcViewAndEntry();
}

void MiiFacePartsHolder::calcViewAndEntry() {
    for (int i = 0; i < getObjectCount(); i++) {
        LiveActor* pActor = getActor(i);

        if (pActor == nullptr) {
            continue;
        }

        if (MR::isNoCalcView(pActor)) {
            continue;
        }

        pActor->calcViewAndEntry();
    }
}

void MiiFacePartsHolder::draw() const {
    drawExtra();
}

void MiiFacePartsHolder::reinitCharModel() {
    MiiFaceParts* pParts;

    if (_38 == RFLErrcode_Busy) {
        RFLErrcode err = RFLGetAsyncStatus();

        if (err == RFLErrcode_Busy) {
            return;
        }

        _38 = err;

        if (err == RFLErrcode_NANDCommandfail) {
            _3C = RFLGetLastReason();

            GameSequenceFunction::tryNANDErrorSequence(_3C);
        }
    }

    if (_38 == RFLErrcode_Success) {
        for (int i = 0; i < getObjectCount(); i++) {
            pParts = static_cast<MiiFaceParts*>(getActor(i));

            if (pParts->_D0 || pParts->_D1) {
                pParts->initFaceModel();
            }
        }
    }
}

bool MiiFacePartsHolder::isInitEnd() const {
    MiiFaceParts* pParts;

    if (isError()) {
        return true;
    }

    for (int i = 0; i < getObjectCount(); i++) {
        pParts = static_cast<MiiFaceParts*>(getActor(i));

        if (pParts->_D1) {
            return false;
        }
    }

    return true;
}

bool MiiFacePartsHolder::isError() const {
    return _38 != RFLErrcode_Success && _38 != RFLErrcode_Busy;
}

MiiFaceParts* MiiFacePartsHolder::createPartsFromReceipe(const char* pName, const MiiFaceRecipe& rRecipe) {
    MiiFaceParts* pParts = new MiiFaceParts(pName, rRecipe);

    MR::getSceneObj<MiiFacePartsHolder>(SceneObj_MiiFacePartsHolder)->registerActor(pParts);

    MR::FunctorV0M<NameObj*, void (NameObj::*)()> initFunc(
        pParts, &NameObj::initWithoutIter);

    if (MR::startFunctionAsyncExecuteOnMainThread(initFunc, "initNameObjOnMainThread")) {
        MR::waitForEndFunctionAsyncExecute("initNameObjOnMainThread");
    }

    return pParts;
}

MiiFaceParts* MiiFacePartsHolder::createPartsFromDefault(const char* pName, u16 miiIndex) {
    return createPartsFromReceipe(
        pName,
        MiiFaceRecipe(
            RFLDataSource_Default,
            miiIndex,
            RFLResolution_256,
            RFLExpFlag_Normal | RFLExpFlag_Blink));
}

void MiiFacePartsHolder::drawEachActor(DrawPartsFuncPtr pDrawFunc, const RFLDrawCoreSetting* pSetting) const {
    MiiFaceParts* pParts;

    for (int i = 0; i < getObjectCount(); i++) {
        pParts = static_cast<MiiFaceParts*>(getActor(i));

        if (MR::isDead(pParts)) {
            continue;
        }

        (pParts->*pDrawFunc)(pSetting);
    }
}

// MiiFacePartsHolder::drawExtra

// FIXME: Enumerated GX types might be defined incorrectly?
void MiiFacePartsHolder::setTevOpa() const {
    GXColorS10 color1 = {0, 0, 0, -89};
    GXSetTevColorS10(GX_TEVREG1, color1);

    GXColorS10 color2 = {0, 0, 0, 246};
    GXSetTevColorS10(GX_TEVREG2, color2);

    GXSetTevDirect(GX_TEVSTAGE1);
    GXSetTevOrder(GX_TEVSTAGE1, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR0A0);
    GXSetTevKAlphaSel(GX_TEVSTAGE1, GX_TEV_KASEL_1_4);
    // GXSetTevColorIn(GX_TEVSTAGE1, GX_CC_A0, GX_CC_C0, GX_CC_TEXGGG, GX_CC_C2);
    GXSetTevColorOp(GX_TEVSTAGE1, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVREG0);
    GXSetTevAlphaIn(GX_TEVSTAGE1, GX_CA_A2, GX_CA_KONST, GX_CA_RASA, GX_CA_A1);
    GXSetTevAlphaOp(GX_TEVSTAGE1, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVREG0);
    GXSetTevDirect(GX_TEVSTAGE2);
    GXSetTevOrder(GX_TEVSTAGE2, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR0A0);
    GXSetTevKAlphaSel(GX_TEVSTAGE2, GX_TEV_KASEL_1_4);
    // GXSetTevColorIn(GX_TEVSTAGE2, GX_CC_ONE, GX_CC_TEXGGG, GX_CC_C1, GX_CC_C0);
    GXSetTevColorOp(GX_TEVSTAGE2, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_2, GX_TRUE, GX_TEVREG0);
    GXSetTevAlphaIn(GX_TEVSTAGE2, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_KONST);
    GXSetTevAlphaOp(GX_TEVSTAGE2, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_2, GX_TRUE, GX_TEVREG0);
    GXSetNumChans(1);
    GXSetChanCtrl(GX_COLOR0, GX_TRUE, GX_SRC_REG, GX_SRC_REG, GX_LIGHT3, GX_DF_CLAMP, GX_AF_NONE);
    GXSetChanCtrl(GX_ALPHA0, GX_TRUE, GX_SRC_REG, GX_SRC_REG, GX_LIGHT4, GX_DF_SIGN, GX_AF_NONE);

    GXColor ambColor = {128, 128, 128, 128};
    GXSetChanAmbColor(GX_COLOR0A0, ambColor);

    GXColor matColor = {165, 165, 165, 255};
    GXSetChanMatColor(GX_COLOR0A0, matColor);

    GXSetZMode(GX_TRUE, GX_EQUAL, GX_TRUE);
}

// FIXME: Enumerated GX types might be defined incorrectly?
void MiiFacePartsHolder::setTevXlu() const {
    GXColorS10 color1 = {0, 0, 0, -89};
    GXSetTevColorS10(GX_TEVREG1, color1);

    GXColorS10 color2 = {0, 0, 0, 246};
    GXSetTevColorS10(GX_TEVREG2, color2);

    GXSetTevDirect(GX_TEVSTAGE1);
    GXSetTevOrder(GX_TEVSTAGE1, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR0A0);
    // GXSetTevColorIn(GX_TEVSTAGE1, GX_CC_A0, GX_CC_C0, GX_CC_TEXGGG, GX_CC_C2);
    GXSetTevColorOp(GX_TEVSTAGE1, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVREG0);
    GXSetTevAlphaIn(GX_TEVSTAGE1, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_A0);
    GXSetTevAlphaOp(GX_TEVSTAGE1, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVREG0);
    GXSetTevDirect(GX_TEVSTAGE2);
    GXSetTevOrder(GX_TEVSTAGE2, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR_NULL);
    GXSetTevColorIn(GX_TEVSTAGE2, GX_CC_ONE, GX_CC_ONE, GX_CC_ONE, GX_CC_C0);
    GXSetTevColorOp(GX_TEVSTAGE2, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_2, GX_TRUE, GX_TEVREG0);
    GXSetTevAlphaIn(GX_TEVSTAGE2, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_A0);
    GXSetTevAlphaOp(GX_TEVSTAGE2, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVREG0);
    GXSetNumChans(1);
    GXSetNumChans(1);
    GXSetChanCtrl(GX_COLOR0, GX_TRUE, GX_SRC_REG, GX_SRC_REG, GX_LIGHT3, GX_DF_CLAMP, GX_AF_NONE);

    GXColor ambColor = {128, 128, 128, 128};
    GXSetChanAmbColor(GX_COLOR0A0, ambColor);

    GXColor matColor = {165, 165, 165, 255};
    GXSetChanMatColor(GX_COLOR0A0, matColor);

    GXSetZMode(GX_TRUE, GX_EQUAL, GX_TRUE);
}
