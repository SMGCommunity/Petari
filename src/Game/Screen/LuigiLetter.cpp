#include "Game/NameObj/NameObjArchiveListCollector.hpp"
#include "Game/Screen/IconAButton.hpp"
#include "Game/Screen/LuigiLetter.hpp"
#include "Game/Util/EventUtil.hpp"
#include "Game/Util/GamePadUtil.hpp"
#include "Game/Util/LayoutUtil.hpp"
#include "Game/Util/ObjUtil.hpp"
#include "Game/Util/SoundUtil.hpp"
#include <cstdio>

namespace NrvLuigiLetter {
    NEW_NERVE(HostTypeNrvAppear, LuigiLetter, Appear);
    NEW_NERVE(HostTypeNrvWait, LuigiLetter, Wait);
    NEW_NERVE(HostTypeNrvEnd, LuigiLetter, End);
};

LuigiLetter::LuigiLetter(bool param1, const char* pParam2) :
    LayoutActor("ルイージの手紙", true),
    mTexture(NULL),
    mAButtonIcon(NULL),
    _28(pParam2),
    _2C(param1)
{
    
}

void LuigiLetter::init(const JMapInfoIter& rIter) {
    char archiveName[256];
    char textureName[256];

    initLayoutManager("LuigiLetter", 1);
    initNerve(&NrvLuigiLetter::HostTypeNrvAppear::sInstance);
    snprintf(archiveName, sizeof(archiveName), "LuigiPicture%s.arc", _28);
    snprintf(textureName, sizeof(textureName), "LuigiPicture%s.bti", _28);

    mTexture = MR::createLytTexMap(archiveName, textureName);

    MR::replacePaneTexture(this, "PicDummy", mTexture, 0);

    mAButtonIcon = new IconAButton(false, false);
    mAButtonIcon->initWithoutIter();
    mAButtonIcon->setFollowActorPane(this, "AButtonPosition");

    if (_2C) {
        MR::connectToSceneTalkLayoutNoMovement(this);
        MR::connectToSceneTalkLayoutNoMovement(mAButtonIcon);
    }
    else {
        MR::connectToSceneLayoutOnPauseMovementCalcAnim(this);
        MR::connectToSceneLayoutOnPauseNoMovement(mAButtonIcon);
    }

    kill();
}

void LuigiLetter::appear() {
    LayoutActor::appear();
    setNerve(&NrvLuigiLetter::HostTypeNrvAppear::sInstance);
}

void LuigiLetter::movement() {
    LayoutActor::movement();
    mAButtonIcon->movement();
}

void LuigiLetter::exeAppear() {
    if (MR::isFirstStep(this)) {
        MR::startAnim(this, "Appear", 0);
        MR::startSystemSE("SE_SY_LETTER_APPEAR", -1, -1);
    }

    if (MR::isAnimStopped(this, 0)) {
        MR::startSystemSE("SE_SV_LUIGI_MAIL", -1, -1);
        setNerve(&NrvLuigiLetter::HostTypeNrvWait::sInstance);
    }
}

void LuigiLetter::exeWait() {
    if (MR::isFirstStep(this)) {
        MR::startAnim(this, "Wait", 0);
        mAButtonIcon->openWithoutMessage();
    }

    if (mAButtonIcon->isWait() && MR::testCorePadTriggerA(0)) {
        MR::startSystemSE("SE_SY_TALK_OK", -1, -1);
        setNerve(&NrvLuigiLetter::HostTypeNrvEnd::sInstance);
    }
}

void LuigiLetter::exeEnd() {
    if (MR::isFirstStep(this)) {
        MR::startAnim(this, "End", 0);
        mAButtonIcon->term();
    }

    if (MR::isAnimStopped(this, 0)) {
        kill();
    }
}

void LuigiLetter::makeArchiveListForNPC(NameObjArchiveListCollector* pCollector, const JMapInfoIter& rIter) {
    LuigiLetter::makeArchiveListCommon(pCollector, MR::getLuigiLetterGalaxyNameForNPC());
}

void LuigiLetter::makeArchiveListForMenu(NameObjArchiveListCollector* pCollector, const JMapInfoIter& rIter) {
    LuigiLetter::makeArchiveListCommon(pCollector, MR::getLuigiLetterGalaxyName());
}

void LuigiLetter::makeArchiveListCommon(NameObjArchiveListCollector* pCollector, const char* pParam2) {
    char archiveName[256];

    if (pParam2 == NULL) {
        return;
    }

    snprintf(archiveName, sizeof(archiveName), "LuigiPicture%s", pParam2);
    pCollector->addArchive("LuigiLetter");
    pCollector->addArchive(archiveName);
}

namespace MR {
    LuigiLetter* createLuigiLetterForTalk() {
        return new LuigiLetter(true, MR::getLuigiLetterGalaxyNameForNPC());
    }

    LuigiLetter* createLuigiLetterForMenu() {
        return new LuigiLetter(false, MR::getLuigiLetterGalaxyName());
    }
};
