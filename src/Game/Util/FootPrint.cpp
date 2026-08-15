#include "Game/Util/FootPrint.hpp"
#include "Game/NameObj/NameObj.hpp"
#include "Game/Scene/SceneFunction.hpp"
#include "Game/Util/Color.hpp"
#include "Game/Util/DirectDraw.hpp"
#include "Game/Util/MathUtil.hpp"
#include "Game/Util/MemoryUtil.hpp"
#include "Game/Util/ObjUtil.hpp"
#include "JSystem/JGeometry/TVec.hpp"
#include "JSystem/JUtility/JUTTexture.hpp"
#include "revolution/gx/GXEnum.h"
#include "revolution/gx/GXStruct.h"

namespace {
    static const f32 sMaxStepLifespan = 60.0f;
    static const f32 sMinPrintDistance = 80.0f;
}  // namespace

FootPrint::FootPrint(const char* pName, s32 arg2, s32 arg3) : NameObj(pName) {
    initMember(arg2, arg3);
}

FootPrint::FootPrint(const char* pName, s32 arg2) : NameObj(pName) {
    initMember(arg2, 23);
}

void FootPrint::initMember(s32 amount, s32 arg2) {
    _C = nullptr;
    mPrints = nullptr;
    mMaxAmountOfPrints = 0;
    mAmountOfPrints = 0;
    mRotatingPrintIndex = 0;

    _2C = 20.0f;
    _30 = 20.0f;
    _34 = 20.0f;
    mMinPrintDistance = sMinPrintDistance;

    mLastPrintPos.set2(0.0f);

    mPrints = new FootPrintInfo[amount];
    mMaxAmountOfPrints = amount;

    for (int i = 0; i < mMaxAmountOfPrints; i++) {
        mPrints[i].mValid = 0;
    }

    MR::connectToScene(this, MR::MovementType_MapObj, -1, -1, arg2);
}

FootPrintInfo::FootPrintInfo() {
}

void FootPrint::setTexture(ResTIMG* img) {
    MR::CurrentHeapRestorer heapRestorer = MR::CurrentHeapRestorer(reinterpret_cast< JKRHeap* >(MR::getSceneHeapGDDR3()));
    _C = new JUTTexture(img, 0);
}

void FootPrint::movement() {
    if (_3C != 0) {
        for (int i = 0; i < mAmountOfPrints; i++) {
            if (mPrints[i].mValid == 0) {
                continue;
            }

            mPrints[i].mTimeLeft--;

            if (mPrints[i].mTimeLeft <= 0) {
                mPrints[i].mValid = 0;
            }
        }

        return;
    }

    for (s32 i = mRotatingPrintIndex; i < mRotatingPrintIndex + 10; i++) {
        s32 rotatedIndex = i;
        if (i >= mMaxAmountOfPrints) {
            rotatedIndex -= mMaxAmountOfPrints;
        }

        if (mPrints[rotatedIndex].mValid == 0) {
            continue;
        }

        mPrints[rotatedIndex].mTimeLeft--;

        if (mPrints[rotatedIndex].mTimeLeft <= 0) {
            mPrints[rotatedIndex].mValid = 0;
        }
    }

    return;
}

bool FootPrint::addPrint(const TVec3f& rPos, const TVec3f& rArg2, const TVec3f& rArg3, bool arg4) {
    _3C = 0;

    if (mAmountOfPrints > 0 && rPos.distance(mLastPrintPos) < mMinPrintDistance) {
        return false;
    }

    mLastPrintPos = rPos;
    mPrints[mRotatingPrintIndex].mPos = rPos;
    mPrints[mRotatingPrintIndex]._C = rArg2;
    mPrints[mRotatingPrintIndex]._18 = rArg3;
    mPrints[mRotatingPrintIndex].mTimeLeft = sMaxStepLifespan;
    mPrints[mRotatingPrintIndex].mValid = 1;
    mPrints[mRotatingPrintIndex]._29 = arg4;

    mRotatingPrintIndex++;
    mAmountOfPrints++;

    if (mRotatingPrintIndex >= mMaxAmountOfPrints) {
        mRotatingPrintIndex -= mMaxAmountOfPrints;
    }

    if (mAmountOfPrints > mMaxAmountOfPrints) {
        mAmountOfPrints = mMaxAmountOfPrints;
    }

    return true;
}

void FootPrint::draw() const {
    if (mAmountOfPrints <= 0) {
        return;
    }

    TDDraw::setup(2, 1, 0);
    GXSetZMode(GX_TRUE, GX_LEQUAL, GX_FALSE);
    _C->load(GX_TEXMAP0);

    Color8 color(0, 0, 0, 255.0f);

    for (int i = 0; i < mAmountOfPrints; i++) {
        if (mPrints[i].mValid == 0) {
            continue;
        }

        color.a = 255.0f * MR::normalize(mPrints[i].mTimeLeft, 0.0f, sMaxStepLifespan);
        GXSetTevColor(GX_TEVREG0, color);

        TVec3f cross = mPrints[i]._C.cross(mPrints[i]._18);
        TVec3f v1;
        if ((i & 1) != 0) {
            v1 = mPrints[i].mPos + cross * _2C;
        } else {
            v1 = mPrints[i].mPos - cross * _2C;
        }

        v1 += mPrints[i]._18 * 5.0f;

        TDDraw::drawTexture3D(v1, mPrints[i]._18, mPrints[i]._C, _34, _30, nullptr, mPrints[i]._29, false);
    }

    TDDraw::close();
}

void FootPrint::clear() {
    _3C = 1;
}

void FootPrint::clearForce() {
    mAmountOfPrints = 0;
    mRotatingPrintIndex = 0;
}

TVec3f* FootPrint::getPrintPos(u32 index) const {
    return &mPrints[index % mMaxAmountOfPrints].mPos;
}

void FootPrint::invalidate(u32 index) {
    mPrints[index % mMaxAmountOfPrints].mValid = 0;
}

bool FootPrint::isValid(u32 index) const {
    return mPrints[index % mMaxAmountOfPrints].mValid != 0;
}

FootPrint::~FootPrint() {
}
