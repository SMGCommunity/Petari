#include "Game/MapObj/DynamicCollisionObj.hpp"
#include "Game/Map/CollisionParts.hpp"
#include "Game/Map/KCollision.hpp"
#include "Game/Scene/SceneFunction.hpp"

DynamicCollisionObj::DynamicCollisionObj(const char* pName) : LiveActor(pName) {
    _A4 = 0;
    _9C = 0;
    _11C = 0;
    _A8 = 0;
    _AC = 0;
}

void DynamicCollisionObj::init(const JMapInfoIter& rIter) {
    MR::initDefaultPos(this, rIter);
    MR::connectToScene(this, MR::MovementType_Enemy, MR::CalcAnimType_Enemy, -1, MR::DrawType_FlexibleSphere);

    mKCLFile = nullptr;
}

void DynamicCollisionObj::syncCollision() {
    updateCollisionHeader();
    updateTriangle();
    mParts->mServer->calcFarthestVertexDistance();
    mParts->updateBoundingSphereRange(mScale);
}

void DynamicCollisionObj::updateTriangle() {
    s32 nrmidxa, nrmidxb, nrmidxc;
    s32 prismidx;
    s32 normalidx = 0;

    for (u16 i = 0; i < _94; i++) {
        TVec3f Vertex1(mPositions[mIndices[i].mIndex[0]]);
        TVec3f Vertex2(mPositions[mIndices[i].mIndex[1]]);
        TVec3f Vertex3(mPositions[mIndices[i].mIndex[2]]);

        TVec3f a(Vertex3);
        a -= Vertex1;
        TVec3f SaveForLater(a);
        TVec3f b(Vertex2);
        b -= Vertex1;
        TVec3f c(Vertex3);
        c -= Vertex2;
        MR::normalizeOrZero(&a);
        MR::normalizeOrZero(&b);
        MR::normalizeOrZero(&c);
        TVec3f cross;
        PSVECCrossProduct(&b, &a, &cross);
        MR::normalizeOrZero(&cross);
        _9C[i] = cross;

        if (!mKCLFile) {
            continue;
        }

        TVec3f cross2;
        TVec3f cross3;
        TVec3f cross4;
        TVec3f OuterVec;
        JMathInlineVEC::PSVECNegate(&a, &OuterVec);
        PSVECCrossProduct(&OuterVec, &cross, &cross2);
        PSVECCrossProduct(&b, &cross, &cross3);
        PSVECCrossProduct(&c, &cross, &cross4);

        MR::normalizeOrZero(&cross2);
        MR::normalizeOrZero(&cross3);
        MR::normalizeOrZero(&cross4);

        mKCLFile->mPos[i] = Vertex1;
        mKCLFile->mNorms[normalidx + 0] = cross;
        nrmidxa = normalidx + 1;
        mKCLFile->mNorms[nrmidxa] = cross2;
        nrmidxb = normalidx + 2;
        mKCLFile->mNorms[nrmidxb] = cross3;
        nrmidxc = normalidx + 3;
        mKCLFile->mNorms[nrmidxc] = cross4;

        prismidx = i + 1;
        KC_PrismData* prism = &mKCLFile->mPrisms[prismidx];
        MR::vecKillElement(SaveForLater, c, &SaveForLater);

        prism->mHeight = PSVECMag(&SaveForLater);
        prism->mPositionIndex = i;
        u16 nrmidx = normalidx;
        prism->mNormalIndex = nrmidx;
        nrmidx += 4;
        prism->mEdgeIndices[0] = nrmidxa;
        prism->mEdgeIndices[1] = nrmidxb;
        prism->mEdgeIndices[2] = nrmidxc;
        prism->mAttribute = prismidx;
        normalidx = nrmidx;
    }
}

void DynamicCollisionObj::updateCollisionHeader() {
    // Compute bounding box and extent
    TVec3f min, max;
    MR::createBoundingBox(this->mPositions, this->mPositionNum, &min, &max);
    TVec3f extent(max);
    extent -= min;

    // Initialize masks with extent
    u32 masks[3];
    masks[0] = (s32)extent.x;
    masks[1] = (s32)extent.y;
    masks[2] = (s32)extent.z;
    if (!masks[0]) {
        masks[0] = 1;
    }
    if (!masks[1]) {
        masks[1] = 1;
    }
    if (!masks[2]) {
        masks[2] = 1;
    }

    // Find highest number of bits to shift
    u32 max_entropy = 0;
    for (u32 component = 0; component < 3; component++) {
        u32 bits_sel = 0x80000000;
        u32 mask_sel = 0xFFFFFFFF;
        u32 val = masks[component];
        u32 i = 0;

        for (u32 bit = 0; bit < 32; bit++) {
            if ((val & bits_sel)) {
                masks[component] = ~mask_sel;
                if (max_entropy < i) {
                    max_entropy = i;
                }
                break;
            }

            bits_sel >>= 1;
            mask_sel >>= 1;
            i++;
        }
    }

    // Update header fields
    u32 bit_shift = 33 - max_entropy;
    u32 area_y_width_mask = masks[1];
    this->mKCLFile->mXMask = masks[0];
    u32 area_z_width_mask = masks[2];
    this->mKCLFile->mYMask = area_y_width_mask;
    this->mKCLFile->mZMask = area_z_width_mask;
    this->mKCLFile->mBlockWidthShift = bit_shift;
    this->mKCLFile->mMin = min;
}

void DynamicCollisionObj::createCollision() {
    s32 v = _94;
    mKCLFile = new KCLFile();
    mKCLFile->mPos = new TVec3f[v];
    mKCLFile->mNorms = new TVec3f[v * 4];
    mKCLFile->mPrisms = new KC_PrismData[(v + 1) * 1];
    u16* u16array = new (4) u16[v + 3];
    mKCLFile->mOctree = u16array;

    u16array[0] = 0x8000;
    u16array[1] = 2;

    s32 count = v;
    for (s32 i = 0; i <= v; i++) {
        u16array[i + 2] = count--;
    }

    mKCLFile->mThickness = 40.f;
    mKCLFile->mBlockXShift = -1;
    mKCLFile->mBlockXYShift = -1;
    updateCollisionHeader();
    updateTriangle();

    mParts = new CollisionParts();
    TPos3f posmtx;
    PSMTXTrans(posmtx, mPosition.x, mPosition.y, mPosition.z);
    mParts->init(posmtx, getSensor("body"), mKCLFile, nullptr, 0, true);
    MR::validateCollisionParts(mParts);
    mParts->mServer->calcFarthestVertexDistance();
    mParts->updateBoundingSphereRange(TVec3f(mScale));
}