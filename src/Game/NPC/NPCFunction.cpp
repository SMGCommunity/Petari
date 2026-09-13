#include "Game/NPC/NPCFunction.hpp"
#include "Game/NPC/NPCActorItem.hpp"
#include "Game/NPC/NPCDirector.hpp"
#include "Game/NPC/NPCParameter.hpp"
#include "Game/Scene/SceneObjHolder.hpp"
#include "Game/System/ResourceHolder.hpp"
#include "Game/Util/JMapInfo.hpp"
#include <cstdio>

void NPCFunction::createNPCData() {
}

void NPCFunction::deleteNPCData() {
}

bool NPCFunction::getNPCItemData(NPCActorItem* pItem, s32 idx) {
    NPCDirector* pDirector = MR::getSceneObj< NPCDirector >(SceneObj_NPCDirector);

    ResourceHolder* pHolder = pDirector->mDataResourceHolder;
    NPCItemParameterReader* pItemReader = pDirector->mItemParameterReader;

    char resName[256];
    snprintf(resName, sizeof(resName), "%sItem.bcsv", pItem->mActor);

    if (!pHolder->mFileInfoTable->isExistRes(resName)) {
        return false;
    }

    pItemReader->copy(pItem);

    JMapInfo mapInfo = JMapInfo();
    mapInfo.attach(pHolder->mFileInfoTable->getRes(resName));

    pItemReader->read(&mapInfo, idx);
    *pItem = pItemReader->mItem;

    return true;
}
