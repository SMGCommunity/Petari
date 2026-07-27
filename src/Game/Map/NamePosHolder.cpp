#include "Game/Map/NamePosHolder.hpp"
#include "Game/Scene/SceneObjHolder.hpp"
#include "Game/Util/JMapLinkInfo.hpp"
#include "Game/Util/SceneUtil.hpp"
#include "Game/Util/StringUtil.hpp"

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

// bool NamePosHolder::tryRegisterLinkObj(const NameObj* pObj, const JMapInfoIter& rIter) {}

bool NamePosHolder::find(const NameObj* pObj, const char* pName, TVec3f* pPos, TVec3f* pRot) const {
    for (s32 idx = 0; idx < mPosNum; idx++) {
        NamePosInfo* pInfo = &mInfos[idx];

        if (!MR::isEqualString(pName, mInfos[idx].mName)) {
            continue;
        }

        if (pObj != nullptr && pInfo->mLinkInfo->isValid() && (pObj != pInfo->_20)) {
            continue;
        }

        pPos->set(pInfo->mPosition);

        if (pRot != nullptr) {
            pRot->set(pInfo->mRotation);
        }

        return true;
    }

    return false;
}

namespace MR {
    NamePosHolder* getNamePosHolder() {
        return MR::getSceneObj< NamePosHolder >(SceneObj_NamePosHolder);
    }
};  // namespace MR

NamePosHolder::~NamePosHolder() {
}
