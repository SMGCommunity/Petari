#include "Game/System/DrawBuffer.hpp"
#include "Game/LiveActor/ActorLightCtrl.hpp"
#include "Game/System/ShapePacketUserData.hpp"
#include "JSystem/J3DGraphBase/J3DMaterial.hpp"
#include <cstring>

DrawBufferShapeDrawer::DrawBufferShapeDrawer(J3DMaterial* pMaterial, J3DMatPacket* pMatPacket)
    : mMaterial(pMaterial), mMatPacket(pMatPacket), _8(1), mMaxPackets(0), mNumPackets(0), mPackets(nullptr) {
}

void DrawBufferShapeDrawer::init(s32 a1) {
    mMaxPackets = a1 * _8;
    mPackets = new PacketInfo*[mMaxPackets];

    for (s32 idx = 0; idx < mMaxPackets; idx++) {
        PacketInfo* packet = new PacketInfo();
        mPackets[idx] = packet;
        mPackets[idx]->mShapePacket = nullptr;
        mPackets[idx]->mLightLoader = nullptr;
        mPackets[idx]->mLightCtrl = nullptr;
    }
}

void DrawBufferShapeDrawer::swap(DrawBufferShapeDrawer* pOther) {
    J3DMaterial* material = mMaterial;
    J3DMatPacket* matPacket = mMatPacket;
    s32 __8 = _8;

    mMaterial = pOther->mMaterial;
    mMatPacket = pOther->mMatPacket;
    _8 = pOther->_8;

    pOther->mMaterial = material;
    pOther->mMatPacket = matPacket;
    pOther->_8 = __8;
}

void DrawBufferShapeDrawer::draw() const {
    mMatPacket->mpDisplayListObj->callDL();

    J3DShapePacket* firstPacket = mPackets[0]->mShapePacket;
    firstPacket->mpShape->loadPreDrawSetting();
    if (MR::getJ3DShapePacketUserData(firstPacket) != nullptr) {
        MR::getJ3DShapePacketUserData(firstPacket)->callDL();
    }

    for (s32 idx = 0; idx < mNumPackets; idx++) {
        J3DShapePacket* packet = mPackets[idx]->mShapePacket;

        if (getLightLoader(idx) != nullptr) {
            getLightLoader(idx)->loadLight();
        }

        if ((packet->mpShape->mFlags & 1) == 0) {
            if (packet->mpDisplayListObj != nullptr) {
                packet->mpDisplayListObj->callDL();
            }
            packet->drawFast();
        }
    }
}

void DrawBufferShapeDrawer::add(const J3DShapePacket* pShapePacket, const ActorLightCtrl* pLightCtrl) {
    bool setLightLoader = false;
    s32 index = findLightSortIndex(pLightCtrl, &setLightLoader);

    PacketInfo* packet = mPackets[mNumPackets];
    packet->mShapePacket = const_cast< J3DShapePacket* >(pShapePacket);
    packet->mLightCtrl = const_cast< ActorLightCtrl* >(pLightCtrl);

    if (setLightLoader) {
        packet->mLightLoader = const_cast< ActorLightCtrl* >(pLightCtrl);
    } else {
        packet->mLightLoader = nullptr;
    }

    for (s32 idx = mNumPackets - 1; idx >= index; idx--) {
        mPackets[idx + 1] = mPackets[idx];
    }

    mPackets[index] = packet;
    mNumPackets++;
}

void DrawBufferShapeDrawer::remove(const J3DShapePacket* pShapePacket) {
    PacketInfo* packet;
    for (s32 removeIdx = 0; removeIdx < mNumPackets; removeIdx++) {
        packet = mPackets[removeIdx];
        if (packet->mShapePacket == pShapePacket) {
            for (s32 idx = removeIdx; idx < mNumPackets - 1; idx++) {
                mPackets[idx] = mPackets[idx + 1];
            }
            if (packet->mLightLoader != nullptr && removeIdx < mNumPackets - 1) {
                mPackets[removeIdx]->mLightLoader = mPackets[removeIdx]->mLightCtrl;
            }
            mNumPackets--;
            mPackets[mNumPackets] = packet;
            return;
        }
    }
}

void DrawBufferShapeDrawer::resetLightSort(const ActorLightCtrl* pLightCtrl) {
    PacketInfo* packet;
    for (s32 idx = 0; idx < mNumPackets; idx++) {
        packet = mPackets[idx];
        if (packet->mLightCtrl == pLightCtrl) {
            J3DShapePacket* shapePacket = packet->mShapePacket;
            remove(shapePacket);
            add(shapePacket, pLightCtrl);
            return;
        }
    }
}

s32 DrawBufferShapeDrawer::findLightSortIndex(const ActorLightCtrl* pLightCtrl, bool* pSetLightLoader) const {
    *pSetLightLoader = false;

    if (pLightCtrl == nullptr) {
        return mNumPackets;
    }

    if (pLightCtrl->_1C != 0) {
        *pSetLightLoader = true;
        return mNumPackets;
    }

    bool foundLight = false;
    for (s32 idx = 0; idx < mNumPackets; idx++) {
        if (mPackets[idx]->mLightLoader != nullptr) {
            if (foundLight) {
                return idx;
            }

            if (pLightCtrl->isSameLight(mPackets[idx]->mLightLoader)) {
                foundLight = true;
            }
        }
    }

    if (!foundLight) {
        *pSetLightLoader = true;
    }
    return mNumPackets;
}

DrawBuffer::DrawBuffer(J3DModel* pModel)
    : mModelData(pModel->mModelData), mModel(pModel), _8(0), mNumActors(0), mNumMaterials(0), mMaterialNos(nullptr), mNumShapeDrawers(0),
      mNumOpaShapeDrawers(0), mShapeDrawers(nullptr) {
}

void DrawBuffer::init(int a1) {
    _8 = a1;
    initTable();
}

void DrawBuffer::add(const LiveActor* pActor) {
    J3DModel* model = MR::getJ3DModel(pActor);
    ActorLightCtrl* lightCtrl = MR::getLightCtrl(pActor);

    for (s32 idx = 0; idx < mNumMaterials; idx++) {
        getShapeDrawerByMatNo(idx)->add(model->getShapePacket(mModelData->mMaterialTable.getMaterialNodePointer(idx)->getShape()->mIndex), lightCtrl);
    }
    mNumActors++;
}

void DrawBuffer::remove(const LiveActor* pActor) {
    J3DModel* model = MR::getJ3DModel(pActor);

    for (s32 idx = 0; idx < mNumMaterials; idx++) {
        getShapeDrawerByMatNo(idx)->remove(model->getShapePacket(mModelData->mMaterialTable.getMaterialNodePointer(idx)->getShape()->mIndex));
    }
    mNumActors--;
}

void DrawBuffer::resetLightSort(const ActorLightCtrl* pLightCtrl) {
    J3DModel* model = MR::getJ3DModel(pLightCtrl->mActor);

    for (s32 idx = 0; idx < mNumMaterials; idx++) {
        getShapeDrawerByMatNo(idx)->resetLightSort(pLightCtrl);
    }
}

void DrawBuffer::drawOpa() const {
    if (mNumActors > 0) {
        J3DShape::sOldVcdVatCmd = nullptr;
        for (s32 idx = 0; idx < mNumOpaShapeDrawers; idx++) {
            getShapeDrawerByIndex(idx)->draw();
        }
    }
}

void DrawBuffer::drawXlu() const {
    if (mNumActors > 0) {
        J3DShape::sOldVcdVatCmd = nullptr;
        for (s32 idx = mNumOpaShapeDrawers; idx < mNumShapeDrawers; idx++) {
            getShapeDrawerByIndex(idx)->draw();
        }
    }
}

namespace {
    s32 getSortedMaterialIndex(int materialIndex, J3DModel* pModel) {
        J3DMaterial* material;
        material = MR::getMaterial(pModel, materialIndex);
        for (s32 idx = 0; idx < materialIndex; idx++) {
            if (MR::getMaterial(pModel, idx)->getDiffFlag() == material->getDiffFlag()) {
                return idx;
            }
        }
        return -1;
    }
}  // namespace

void DrawBuffer::initTable() {
    mNumMaterials = MR::getMaterialNum(mModel);
    mNumShapeDrawers = mModelData->mMaterialTable.mUniqueMatNum;

    J3DModel* model = mModel;
    s32 idx, numOpaMaterials;
    numOpaMaterials = 0;
    for (idx = 0; idx < MR::getMaterialNum(model); idx++) {
        if (!MR::getMaterial(model, idx)->isDrawModeOpaTexEdge()) {
            if (getSortedMaterialIndex(idx, model) < 0) {
                numOpaMaterials++;
            }
        }
    }
    mNumOpaShapeDrawers = numOpaMaterials;

    mMaterialNos = new s32[mNumMaterials];
    for (s32 idx = 0; idx < mNumMaterials; idx++) {
        mMaterialNos[idx] = -1;
    }

    mShapeDrawers = new DrawBufferShapeDrawer*[mNumShapeDrawers];

    for (s32 idx = 0; idx < mNumShapeDrawers; idx++) {
        mShapeDrawers[idx] = nullptr;
    }

    s32 opaIndex = 0;
    s32 xluIndex = 0;
    for (s32 idx = 0; idx < mNumMaterials; idx++) {
        J3DMaterial* material = MR::getMaterial(mModel, idx);
        s32 index = getSortedMaterialIndex(idx, mModel);

        if (index >= 0) {
            s32 matNo = mMaterialNos[index];
            mMaterialNos[idx] = matNo;
            mShapeDrawers[matNo]->_8++;
            continue;
        }

        if (!material->isDrawModeOpaTexEdge()) {
            mMaterialNos[idx] = opaIndex;
            mShapeDrawers[opaIndex] = new DrawBufferShapeDrawer(material, mModel->getMatPacket(idx));
            opaIndex++;
        } else {
            mMaterialNos[idx] = xluIndex + mNumOpaShapeDrawers;
            mShapeDrawers[xluIndex + mNumOpaShapeDrawers] = new DrawBufferShapeDrawer(material, mModel->getMatPacket(idx));
            xluIndex++;
        }
    }

    // sort Opa
    for (s32 idx = 0; idx < mNumOpaShapeDrawers - 1; idx++) {
        for (s32 idx2 = idx + 1; idx2 < mNumOpaShapeDrawers; idx2++) {
            sortShapeDrawer(idx, idx2);
        }
    }

    // sort Xlu
    for (s32 idx = mNumOpaShapeDrawers; idx < mNumShapeDrawers - 1; idx++) {
        for (s32 idx2 = idx + 1; idx2 < mNumShapeDrawers; idx2++) {
            sortShapeDrawer(idx, idx2);
        }
    }

    for (s32 idx = 0; idx < mNumShapeDrawers; idx++) {
        mShapeDrawers[idx]->init(_8);
    }
}

void DrawBuffer::sortShapeDrawer(s32 matNo1, s32 matNo2) {
    DrawBufferShapeDrawer* drawerA = getShapeDrawerByIndex(matNo1);
    DrawBufferShapeDrawer* drawerB = getShapeDrawerByIndex(matNo2);
    const char* nameA = MR::getMaterialName(mModelData, drawerA->mMaterial->mIndex);
    const char* nameB = MR::getMaterialName(mModelData, drawerB->mMaterial->mIndex);

    if (strcasecmp(nameA, nameB) > 0) {
        drawerA->swap(drawerB);

        for (s32 idx = 0; idx < mNumMaterials; idx++) {
            if (matNo1 == mMaterialNos[idx]) {
                mMaterialNos[idx] = matNo2;
            } else if (matNo2 == mMaterialNos[idx]) {
                mMaterialNos[idx] = matNo1;
            }
        }
    }
}
