#include "Game/Map/NamePosHolder.hpp"
#include "Game/Scene/SceneObjHolder.hpp"
#include "Game/Util/SceneUtil.hpp"

NamePosHolder::NamePosHolder() : NameObj("位置テーブル保持") {
    mPosNum = 0;
    mInfos = 0;
    mPosNum = MR::getGeneralPosNum();
    mInfos = new NamePosInfo[mPosNum];

    for (s32 i = 0; i < mPosNum; i++) {
        NamePosInfo* curInf = &mInfos[i];
        curInf->mLinkInfo = 0;
        curInf->_20 = 0;
        MR::getGeneralPosData(&curInf->mName, &curInf->mPosition, &curInf->mRotation, &curInf->mLinkInfo, i);
    }
}

NamePosInfo::NamePosInfo() {

}

namespace MR {
    NamePosHolder* getNamePosHolder() {
        return reinterpret_cast<NamePosHolder*>(MR::getSceneObjHolder()->getObj(SceneObj_NamePosHolder));
    }
};

NamePosHolder::~NamePosHolder() {

}