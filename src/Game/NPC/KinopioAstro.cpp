#include "Game/NPC/KinopioAstro.hpp"
#include "Game/NameObj/NameObjArchiveListCollector.hpp"
#include "Game/NPC/NPCActorItem.hpp"
#include "Game/Util.hpp"
#include "Game/Screen/LuigiLetter.hpp"
#include "Game/Screen/PeachLetter.hpp"
#include "Game/NPC/NPCActor.hpp"
#include "JSystem/JKernel/JKRArchive.hpp"

s32 JKRArchive::getExpandedResSize(const void* pResource) const {
    return getResSize(pResource);
}
void KinopioAstro::makeArchiveList(NameObjArchiveListCollector* pArchiveList, const JMapInfoIter& rIter) {
    NPCActorItem item("StaffFinalLetter_000");

    s32 arg1 = 0;
    MR::getJMapInfoArg1NoInit(rIter, &arg1);

    if (arg1 == 2) {
        if (!MR::isEqualStageName("PeachCastleFinalGalaxy")) {
            if (MR::isAnyPlayerLeftSupply()) {
                PeachLetter::makeArchiveList(pArchiveList, rIter);
            }

            if (MR::isLuigiLetterArrivalAtMessenger()) {
                LuigiLetter::makeArchiveListForNPC(pArchiveList, rIter);
            }
        }
        else {
            pArchiveList->addArchive("AllCompleteImage");
        }

        MR::getNPCItemData(&item, 3);
        NPCActor::addArchive(pArchiveList, item);
    }
    else {
        s32 arg7 = -1;
        MR::getJMapInfoArg7NoInit(rIter, &arg7);

        if (MR::getNPCItemData(&item, arg7)) {
            NPCActor::addArchive(pArchiveList, item);
        }
    }
}

KinopioAstro::KinopioAstro(const char* pName) : NameObj(pName) {}
KinopioAstro::~KinopioAstro() {}
