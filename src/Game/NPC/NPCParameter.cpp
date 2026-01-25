#include "Game/NPC/NPCParameter.hpp"
#include "Game/NPC/NPCActor.hpp"
#include "Game/NPC/NPCActorItem.hpp"
#include "Game/Util/JMapInfo.hpp"
#include "Game/Util/StringUtil.hpp"
#include "JSystem/JGeometry/TVec.hpp"
#include "revolution/types.h"

NPCParameterEdit::NPCParameterEdit(const char* pName, const char** ppChar, bool a3) : NPCParameterBase(pName), _8(ppChar), _C(a3) {}

void NPCParameterEdit::read(JMapInfo* pInfo, s32 a2) {
    pInfo->getValue(a2, mName, _8);
}

NPCParameterBool::NPCParameterBool(const char* pName, bool* pBool) : NPCParameterBase(pName), _8(pBool) {}

void NPCParameterBool::read(JMapInfo* pInfo, s32 a2) {
    // most likely inlined JmapInfo::getValue<bool>
    bool* temp = _8;
    s32 itemIndex = pInfo->searchItemInfo(mName);
    if (itemIndex < 0) {
        return;
    }
    const JMapItem* pItem = &pInfo->mData->mItems[itemIndex];
    const u32* pValue = reinterpret_cast< const u32* >(getEntryAddress(pInfo->mData, pInfo->mData->mDataOffset, a2) + pItem->mOffsData);
    *temp = (*pValue & pItem->mMask) != 0;
}

NPCParameterV3f::NPCParameterV3f(const char* pName, TVec3f* pVec, f32 f1, f32 f2) : NPCParameterBase(pName), _8(pVec), _C(f1), _10(f2) {}

void NPCParameterV3f::read(JMapInfo* pInfo, s32 a2) {
    char str[255];
    MR::copyString(str, mName, sizeof(str));
    *(str + strlen(mName) + 1) = 0x0;
    *(str + strlen(mName)) = 'X';  
    pInfo->getValue(a2, str, &_8->x);
    *(str + strlen(mName)) = 'Y';  
    pInfo->getValue(a2, str, &_8->y);
    *(str + strlen(mName)) = 'Z';  // Z in ASCII
    pInfo->getValue(a2, str, &_8->z);
}

NPCParameterJoint::NPCParameterJoint(const char* pName, const char* pChar, const char** ppChar)
    : NPCParameterBase(pName), _8(ppChar), _C(pChar), _10(-1) {}

NPCParameterReader::NPCParameterReader(const char* pName) : NPCParameterBase(pName), mVector() {}

void NPCParameterReader::read(JMapInfo* pInfo, s32 a2) {
    if (a2 <= -1 || a2 >= pInfo->getNumEntries()) {
        return;
    }

    for (NPCParameterBase** it = mVector.begin(); it != mVector.end(); ++it) {
        (*it)->read(pInfo, a2);
    }
}

NPCCapsParameterReader::NPCCapsParameterReader(const char* pName) : NPCParameterReader(pName), mActorCaps("") {
    mActorCaps.setDefault();
    mActorCaps.mObjectName = pName;
    mActorCaps._10 = pName;
    init();
}

void NPCItemParameterReader::copy(const NPCActorItem* pItem) {
    mItem.mActor = pItem->mActor;
    mItem.mGoods0 = pItem->mGoods0;
    mItem.mGoodsJoint0 = pItem->mGoodsJoint0;
    mItem.mGoods1 = pItem->mGoods1;
    mItem.mGoodsJoint1 = pItem->mGoodsJoint1;
}

template <>
NPCParameterRange< s32 >::NPCParameterRange(const char* pName, s32* pLong, s32 a3, s32 a4) : NPCParameterBase(pName), _8(pLong), _C(a3), _10(a4) {}

template <>
void NPCParameterRange< s32 >::read(JMapInfo* pInfo, s32 a2) {
    // inlined JmapInfo::getValue< s32 >
    s32 itemIndex = pInfo->searchItemInfo(mName);
    if (itemIndex < 0) {
        return;
    }
    pInfo->getValueFast(a2, itemIndex, _8);
}

template <>
NPCParameterRange< f32 >::NPCParameterRange(const char* pName, f32* pLong, f32 a3, f32 a4) : NPCParameterBase(pName), _8(pLong), _C(a3), _10(a4) {}

template <>
void NPCParameterRange< f32 >::read(JMapInfo* pInfo, s32 a2) {
    pInfo->getValue(a2, mName, _8);
}

void NPCParameterJoint::read(JMapInfo* pInfo, s32 a2) {
    pInfo->getValue(a2, mName, _8);

    if (strcmp(*_8, "") == 0) {
        mName = "";
    }
}

void NPCCapsParameterReader::init() {
    mVector.push_back(new NPCParameterBool("mModel", &mActorCaps.mModel));
    mVector.push_back(new NPCParameterBool("mMakeActor", &mActorCaps.mMakeActor));
    mVector.push_back(new NPCParameterBool("mHostIO", &mActorCaps.mHostIO));
    mVector.push_back(new NPCParameterBool("mMessage", &mActorCaps.mMessage));
    mVector.push_back(new NPCParameterV3f("mMessageOffset", &mActorCaps.mMessageOffset, 0.0f, 1000.0f));
    mVector.push_back(new NPCParameterBool("mInterpole", &mActorCaps.mInterpole));
    mVector.push_back(new NPCParameterBool("mConnectTo", &mActorCaps.mConnectTo));
    mVector.push_back(new NPCParameterBool("mLightCtrl", &mActorCaps.mLightCtrl));
    mVector.push_back(new NPCParameterBool("mEffect", &mActorCaps.mEffect));
    mVector.push_back(new NPCParameterBool("mSound", &mActorCaps.mSound));
    mVector.push_back(new NPCParameterRange< s32 >("mSoundSize", &mActorCaps.mSoundSize, 0, 16));
    mVector.push_back(new NPCParameterBool("mAttribute", &mActorCaps.mAttribute));
    mVector.push_back(new NPCParameterBool("mPosition", &mActorCaps.mPosition));
    mVector.push_back(new NPCParameterBool("mLodCtrl", &mActorCaps.mLodCtrl));
    mVector.push_back(new NPCParameterBool("mNerve", &mActorCaps.mNerve));
    mVector.push_back(new NPCParameterBool("mBinder", &mActorCaps.mBinder));
    mVector.push_back(new NPCParameterRange< f32 >("mBinderSize", &mActorCaps.mBinderSize, 0.0f, 1000.0f));
    mVector.push_back(new NPCParameterBool("mSensor", &mActorCaps.mSensor));
    mVector.push_back(new NPCParameterJoint("mSensorJoint", mActorCaps.mObjectName, &mActorCaps.mSensorJoint));
    mVector.push_back(new NPCParameterRange< f32 >("mSensorSize", &mActorCaps.mSensorSize, 0.0f, 1000.0f));
    mVector.push_back(new NPCParameterRange< s32 >("mSensorMax", &mActorCaps.mSensorMax, 0, 16));
    mVector.push_back(new NPCParameterBool("mShadow", &mActorCaps.mShadow));
    mVector.push_back(new NPCParameterRange< f32 >("mShadowSize", &mActorCaps.mShadowSize, 0.0f, 1000.0f));
    mVector.push_back(new NPCParameterBool("mRailRider", &mActorCaps.mRailRider));
    mVector.push_back(new NPCParameterBool("mSwitchDead", &mActorCaps.mSwitchDead));
    mVector.push_back(new NPCParameterBool("mSwitchAppear", &mActorCaps.mSwitchAppear));
    mVector.push_back(new NPCParameterBool("mPointer", &mActorCaps.mPointer));
    mVector.push_back(new NPCParameterRange< f32 >("mPointerSize", &mActorCaps.mPointerSize, 0.0f, 1000.0f));
}

NPCItemParameterReader::NPCItemParameterReader(const char* pName) : NPCParameterReader(pName), mItem("") {
    mVector.push_back(new NPCParameterEdit("mGoods0", &mItem.mGoods0, true));
    mVector.push_back(new NPCParameterEdit("mGoods1", &mItem.mGoods1, true));
    mVector.push_back(new NPCParameterJoint("mGoodsJoint0", pName, &mItem.mGoodsJoint0));
    mVector.push_back(new NPCParameterJoint("mGoodsJoint1", pName, &mItem.mGoodsJoint1));
}
