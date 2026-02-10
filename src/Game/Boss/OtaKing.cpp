#include "Game/Boss/OtaKing.hpp"
#include "Game/NameObj/NameObjArchiveListCollector.hpp"
#include "Game/Util.hpp"
#include "Game/MapObj/CocoNut.hpp"

OtaKing::OtaKing(const char* pName) : NameObj(pName) {}
OtaKing::~OtaKing() {}

void OtaKing::makeArchiveList(NameObjArchiveListCollector* pArchiveList, const JMapInfoIter& rIter) {
    NameObjArchiveListCollector* pList = pArchiveList;
    bool isLv2 = false;
    MR::getJMapInfoArg1NoInit(rIter, &isLv2);
    const bool isLv2Flag = isLv2;

    pList->addArchive(CocoNut::getModelName());
    pList->addArchive("FireBall");
    pList->addArchive("FireBubble");
    pList->addArchive("OtaKingMagma");
    pList->addArchive("OtaKingMagmaBloom");

    if (!isLv2Flag) {
        pList->addArchive("OtaKing");
        pList->addArchive("OtaKingFoot");
        pList->addArchive("OtaKingLongFoot");
    }
    else {
        pList->addArchive("OtaKingLv2");
        pList->addArchive("OtaKingFootLv2");
    }
}
