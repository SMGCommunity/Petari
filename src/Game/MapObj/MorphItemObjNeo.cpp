#include "Game/MapObj/MorphItemObjNeo.hpp"
#include "Game/NameObj/NameObjArchiveListCollector.hpp"
#include "Game/Util.hpp"

MorphItemObjNeo::MorphItemObjNeo(const char* pName, long a2) : NameObj(pName) {}
MorphItemObjNeo::~MorphItemObjNeo() {}
void MorphItemObjNeo::exeWait() {}
void MorphItemObjNeo::exeAppear() {}
void MorphItemObjNeo::exeSwitchAppear() {}
void MorphItemObjNeo::exeWait2() {}
void MorphItemObjNeo::exeFly() {}
void MorphItemObjNeo::exeDemo() {}

void MorphItemObjNeo::makeArchiveList(NameObjArchiveListCollector* pArchiveList, const JMapInfoIter& rIter) {
    if (MR::isEqualObjectName(rIter, "MorphItemNeoHopper") || MR::isEqualObjectName(rIter, "BenefitItemInvincible")) {
        if (MR::isPlayerLuigi()) {
            pArchiveList->addArchive("HopperLuigi");
        }
        else {
            pArchiveList->addArchive("HopperMario");
        }
    }

    if (MR::isEqualObjectName(rIter, "MorphItemNeoBee")) {
        pArchiveList->addArchive("BeeMarioWing");

        if (MR::isPlayerLuigi()) {
            pArchiveList->addArchive("BeeLuigi");
        }
        else {
            pArchiveList->addArchive("BeeMario");
        }
    }

    if (MR::isEqualObjectName(rIter, "MorphItemNeoTeresa")) {
        if (MR::isPlayerLuigi()) {
            pArchiveList->addArchive("TeresaLuigi");
        }
        else {
            pArchiveList->addArchive("TeresaMario");
        }
    }

    if (MR::isEqualObjectName(rIter, "MorphItemNeoIce")) {
        pArchiveList->addArchive("IceMarioHandL");
        pArchiveList->addArchive("IceMarioHandR");

        if (MR::isPlayerLuigi()) {
            pArchiveList->addArchive("IceLuigi");
        }
        else {
            pArchiveList->addArchive("IceMario");
        }
    }

    if (MR::isEqualObjectName(rIter, "MorphItemNeoFire")) {
        pArchiveList->addArchive("InvincibleMarioHandL");
        pArchiveList->addArchive("InvincibleMarioHandR");

        if (MR::isPlayerLuigi()) {
            pArchiveList->addArchive("InvincibleLuigi");
        }
        else {
            pArchiveList->addArchive("InvincibleMario");
        }
    }

    s32 arg3 = -1;

    if (MR::isValidInfo(rIter)) {
        MR::getJMapInfoArg3WithInit(rIter, &arg3);
    }

    if (!MR::isEqualObjectName(rIter, "BenefitItemInvincible")) {
        if (arg3 == 0) {
            pArchiveList->addArchive("ItemBubble");
        }
        else if (arg3 == -1) {
            pArchiveList->addArchive("MorphItemNeoBee");
            pArchiveList->addArchive("MorphItemNeoHopper");
        }
    }
}
