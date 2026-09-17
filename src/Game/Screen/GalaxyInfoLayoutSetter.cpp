#include "Game/Screen/GalaxyInfoLayoutSetter.hpp"
#include "Game/Screen/IconComet.hpp"
#include "Game/System/GalaxyStatusAccessor.hpp"
#include "Game/System/GameDataConst.hpp"
#include "Game/System/ResourceHolder.hpp"
#include "Game/Util/EventUtil.hpp"
#include "Game/Util/LayoutUtil.hpp"
#include "Game/Util/MessageUtil.hpp"
#include "Game/Util/ObjUtil.hpp"
#include "Game/Util/StringUtil.hpp"
#include <cstdio>

namespace {
    const char* const cTextureArcName = "GalaxyInfoTexture.arc";
};  // namespace

namespace {
    void addPictureFontStar(wchar_t** ppParam1, wchar_t** ppParam2, const char* pGalaxyName, s32 scenarioNo) {
        GalaxyStatusAccessor galaxyStatusAccessor = MR::makeGalaxyStatusAccessor(pGalaxyName);
        wchar_t fontCode = '7';

        if (GameDataConst::isGrandStar(pGalaxyName, scenarioNo)) {
            fontCode = 'K';
        } else if (GameDataConst::isPowerStarGreen(pGalaxyName, scenarioNo)) {
            fontCode = 'O';
        } else if (GameDataConst::isPowerStarRed(pGalaxyName, scenarioNo)) {
            fontCode = 'S';
        } else if (galaxyStatusAccessor.isCometStar(scenarioNo)) {
            fontCode = 'e';
        }

        *ppParam1 = MR::addPictureFontCode(*ppParam1, fontCode);
        *ppParam2 = MR::addPictureFontCode(*ppParam2, 'R');
    }
};  // namespace

GalaxyInfoLayoutSetter::GalaxyInfoLayoutSetter(LayoutActor* pHost) : mHost(pHost), mReplaceTexture(), mUnknownTexture(), mIconComet() {
    MR::setInfluencedAlphaToChild(pHost);
    initReplaceTexture();

    mIconComet = new IconComet();
    mIconComet->initWithoutIter();
}

void GalaxyInfoLayoutSetter::replaceTexture(const char* pGalaxyName) {
    char btiName[64];
    ResTable* fileInfoTable = MR::createAndAddResourceHolder(::cTextureArcName)->mFileInfoTable;
    nw4r::lyt::TexMap* texture = nullptr;

    for (s32 i = 0; i < fileInfoTable->mCount; i++) {
        snprintf(btiName, sizeof(btiName), "%s.bti", pGalaxyName);

        if (MR::isEqualStringCase(btiName, fileInfoTable->getResName(i))) {
            texture = mReplaceTexture[i];
            break;
        }
    }

    MR::replacePaneTexture(mHost, "PicDummy", texture, 0);
}

void GalaxyInfoLayoutSetter::updateInfo(const char* pGalaxyName, GalaxyState galaxyState) {
    updateInfoWithoutComet(pGalaxyName, galaxyState);

    if (mIconComet->appearIfLanding(pGalaxyName)) {
        showCometWindow(MR::getEncounterGalaxyCometNameId(pGalaxyName));
    } else {
        mIconComet->kill();
        hideCometWindow();
    }
}

void GalaxyInfoLayoutSetter::updateInfo(const char* pGalaxyName, GalaxyState galaxyState, int cometId) {
    updateInfoWithoutComet(pGalaxyName, galaxyState);

    if (cometId >= 0) {
        mIconComet->appearByCometNameId(cometId);
        showCometWindow(cometId);
    } else {
        mIconComet->kill();
        hideCometWindow();
    }
}

void GalaxyInfoLayoutSetter::updateCometPos() {
    MR::setLayoutScalePosAtPaneScaleTrans(mIconComet, mHost, "CometPosition");
    MR::setLayoutAlpha(mIconComet, MR::getPaneAlpha(mHost, "CometPosition"));
}

u32 GalaxyInfoLayoutSetter::getAnimLayerNum() {
    return 3;
}

void GalaxyInfoLayoutSetter::initReplaceTexture() {
    ResTable* fileInfoTable = MR::createAndAddResourceHolder(::cTextureArcName)->mFileInfoTable;

    mReplaceTexture = new nw4r::lyt::TexMap*[fileInfoTable->mCount];

    for (u32 i = 0; i < fileInfoTable->mCount; i++) {
        const char* resName = fileInfoTable->getResName(i);

        mReplaceTexture[i] = MR::createLytTexMap("GalaxyInfoTexture.arc", resName);

        if (MR::isEqualStringCase("GalaxyInfoTextureUnknown.bti", resName)) {
            mUnknownTexture = mReplaceTexture[i];
        }
    }
}

void GalaxyInfoLayoutSetter::updateInfoWithoutComet(const char* pGalaxyName, GalaxyState galaxyState) {
    bool isOpened = galaxyState == GalaxyState_Opened;

    switch (galaxyState) {
    case GalaxyState_Hatena:
        MR::setTextBoxGameMessageRecursive(mHost, "Unknown", "GalaxyInfo_Hatena");
        break;
    case GalaxyState_Unknown:
        MR::setTextBoxGameMessageRecursive(mHost, "Unknown", "GalaxyInfo_Unknown");
        break;
    case GalaxyState_New:
        MR::setTextBoxGameMessageRecursive(mHost, "Unknown", "GalaxyInfo_New");
        break;
    case GalaxyState_Opened:
        MR::setTextBoxMessageRecursive(mHost, "GalaxyName", MR::getGalaxyNameOnCurrentLanguage(pGalaxyName));
        break;
    }

    if (galaxyState == GalaxyState_Opened) {
        MR::clearTextBoxMessageRecursive(mHost, "Unknown");
    } else {
        MR::clearTextBoxMessageRecursive(mHost, "GalaxyName");
    }

    if (isOpened) {
        replaceTexture(pGalaxyName);
    } else {
        MR::replacePaneTexture(mHost, "PicDummy", mUnknownTexture, 0);
    }

    if (isOpened) {
        setTextBoxStarNum(pGalaxyName);
    } else {
        MR::clearTextBoxMessageRecursive(mHost, "StarIcon");
        MR::clearTextBoxMessageRecursive(mHost, "TxtStarBase");
        MR::hidePaneRecursive(mHost, "Complete");
    }
}

void GalaxyInfoLayoutSetter::setTextBoxStarNum(const char* pGalaxyName) {
    GalaxyStatusAccessor accessor = MR::makeGalaxyStatusAccessor(pGalaxyName);

    wchar_t starBaseBuffer[16];
    wchar_t starIconBuffer[16];
    
    wchar_t* pStarBase = starIconBuffer;
    wchar_t* pStarIcon = starBaseBuffer;

    starIconBuffer[0] = '\0';
    starBaseBuffer[0] = '\0';

    for (s32 i = 1; i <= accessor.getNormalScenarioNum(); i++) {
        if (MR::isOnGameEventFlagPowerStarSuccess(pGalaxyName, i)) {
            addPictureFontStar(&pStarBase, &pStarIcon, pGalaxyName, i);
        } else {
            pStarBase = MR::addPictureFontCode(pStarBase, 'R');
            pStarIcon = MR::addPictureFontCode(pStarIcon, '7');
        }
    }

    if (MR::isStarComplete(pGalaxyName)) {
        MR::showPaneRecursive(mHost, "Complete");

        for (s32 i = accessor.getNormalScenarioNum() + 1; i <= accessor.getPowerStarNum(); i++) {
            addPictureFontStar(&pStarBase, &pStarIcon, pGalaxyName, i);
        }
    } else {
        MR::hidePaneRecursive(mHost, "Complete");

        if (MR::isStarCompleteNormalScenario(pGalaxyName)) {
            for (s32 i = accessor.getNormalScenarioNum() + 1; i <= accessor.getPowerStarNum(); i++) {
                if (MR::isOnGameEventFlagPowerStarSuccess(pGalaxyName, i)) {
                    addPictureFontStar(&pStarBase, &pStarIcon, pGalaxyName, i);
                    continue;
                }

                if (GameDataConst::isPowerStarLuigiHas(pGalaxyName, i) && !MR::isLuigiHidingGalaxyAndScenario(pGalaxyName, i)) {
                    continue;
                }

                if (accessor.isValidNormalComet(i) && !MR::canAppearNormalComet(pGalaxyName)) {
                    continue;
                }

                if (accessor.isValidCoin100(i) && !MR::canAppearCoin100Comet(pGalaxyName)) {
                    continue;
                }

                if (accessor.isCometStar(i)) {
                    pStarBase = MR::addPictureFontCode(pStarBase, 'R');
                    pStarIcon = MR::addPictureFontCode(pStarIcon, 'e');
                } else {
                    pStarBase = MR::addPictureFontCode(pStarBase, MR::isPowerStarGreen(pGalaxyName, i) ? 'f' : 'Z');
                    pStarIcon = MR::addPictureFontCode(pStarIcon, '7');
                }
            }
        } else {
            for (s32 i = accessor.getNormalScenarioNum() + 1; i <= accessor.getPowerStarNum(); i++) {
                if (MR::isOnGameEventFlagPowerStarSuccess(pGalaxyName, i)) {
                    addPictureFontStar(&pStarBase, &pStarIcon, pGalaxyName, i);
                }
            }
        }
    }
    
    MR::setTextBoxMessageRecursive(mHost, "StarIcon", starIconBuffer);
    MR::setTextBoxMessageRecursive(mHost, "TxtStarBase", starBaseBuffer);
}

void GalaxyInfoLayoutSetter::showCometWindow(int cometId) {
    char messageId[256];

    MR::startAnim(mHost, "CometWindow", 1);
    MR::setAnimFrameAndStop(mHost, 1.0f, 1);
    MR::setCometAnimFromId(mHost, cometId, 2);
    MR::makeCometMessageID(messageId, sizeof(messageId), MR::getCometNameFromId(cometId));
    MR::setTextBoxGameMessageRecursive(mHost, "CometName", "GalaxyInfo_CometAppear");
    MR::setTextBoxArgGameMessageRecursive(mHost, "CometName", messageId, 0);
}

void GalaxyInfoLayoutSetter::hideCometWindow() {
    MR::startAnim(mHost, "CometWindow", 1);
    MR::setAnimFrameAndStop(mHost, 0.0f, 1);
    MR::clearTextBoxMessageRecursive(mHost, "CometName");
}
