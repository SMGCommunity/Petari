#include "Game/Util/FootPrint.hpp"
#include "Game/Scene/SceneFunction.hpp"
#include "Game/Util/Color.hpp"
#include "Game/Util/DirectDraw.hpp"
#include "Game/Util/MathUtil.hpp"
#include "Game/Util/MemoryUtil.hpp"
#include "Game/Util/ObjUtil.hpp"
#include <JSystem/JUtility/JUTTexture.hpp>
#include <revolution/gx/GXEnum.h>
#include <revolution/gx/GXStruct.h>

namespace {
    static const s32 sFadeBufferCount = 10;
    static const s32 sFadeOutTime = 60;
};  // namespace

FootPrint::FootPrint(const char* pName, s32 amount, s32 drawType) : NameObj(pName) {
    initMember(amount, drawType);
}

FootPrint::FootPrint(const char* pName, s32 amount) : NameObj(pName) {
    initMember(amount, MR::DrawType_0x17);
}

void FootPrint::initMember(s32 amount, s32 drawType) {
    _C = nullptr;
    mPrints = nullptr;
    mPrintMaxNum = 0;
    mPrintNum = 0;
    mRotatingPrintIndex = 0;

    _2C = 20.0f;
    _30 = 20.0f;
    _34 = 20.0f;
    mMinPrintDistance = 80.0f;

    mLastPrintPos.set2(0.0f);

    mPrints = new FootPrintInfo[amount];
    mPrintMaxNum = amount;

    for (int i = 0; i < mPrintMaxNum; i++) {
        mPrints[i].mIsValid = false;
    }

    MR::connectToScene(this, MR::MovementType_MapObj, -1, -1, drawType);
}

FootPrintInfo::FootPrintInfo() {
}

void FootPrint::setTexture(ResTIMG* img) {
    MR::CurrentHeapRestorer heapRestorer = MR::CurrentHeapRestorer(reinterpret_cast< JKRHeap* >(MR::getSceneHeapGDDR3()));
    _C = new JUTTexture(img, 0);
}

void FootPrint::movement() {
    if (_3C) {
        for (int i = 0; i < mPrintNum; i++) {
            if (!mPrints[i].mIsValid) {
                continue;
            }

            mPrints[i].mTimeLeft--;

            if (mPrints[i].mTimeLeft <= 0) {
                mPrints[i].mIsValid = false;
            }
        }

        return;
    }

    for (s32 i = mRotatingPrintIndex; i < mRotatingPrintIndex + ::sFadeBufferCount; i++) {
        s32 rotatedIndex = i;
        if (i >= mPrintMaxNum) {
            rotatedIndex -= mPrintMaxNum;
        }

        if (!mPrints[rotatedIndex].mIsValid) {
            continue;
        }

        mPrints[rotatedIndex].mTimeLeft--;

        if (mPrints[rotatedIndex].mTimeLeft <= 0) {
            mPrints[rotatedIndex].mIsValid = false;
        }
    }
}

bool FootPrint::addPrint(const TVec3f& rPos, const TVec3f& rArg2, const TVec3f& rArg3, bool arg4) {
    _3C = false;

    if (mPrintNum > 0 && rPos.distance(mLastPrintPos) < mMinPrintDistance) {
        return false;
    }

    mLastPrintPos = rPos;
    mPrints[mRotatingPrintIndex].mPos = rPos;
    mPrints[mRotatingPrintIndex]._C = rArg2;
    mPrints[mRotatingPrintIndex]._18 = rArg3;
    mPrints[mRotatingPrintIndex].mTimeLeft = ::sFadeOutTime;
    mPrints[mRotatingPrintIndex].mIsValid = true;
    mPrints[mRotatingPrintIndex]._29 = arg4;

    mRotatingPrintIndex++;
    mPrintNum++;

    if (mRotatingPrintIndex >= mPrintMaxNum) {
        mRotatingPrintIndex -= mPrintMaxNum;
    }

    if (mPrintNum > mPrintMaxNum) {
        mPrintNum = mPrintMaxNum;
    }

    return true;
}

void FootPrint::draw() const {
    if (mPrintNum <= 0) {
        return;
    }

    TDDraw::setup(2, 1, 0);
    GXSetZMode(GX_TRUE, GX_LEQUAL, GX_FALSE);
    _C->load(GX_TEXMAP0);

    Color8 color(0, 0, 0, 255);

    for (int i = 0; i < mPrintNum; i++) {
        if (!mPrints[i].mIsValid) {
            continue;
        }

        color.a = 255.0f * MR::normalize(mPrints[i].mTimeLeft, 0.0f, ::sFadeOutTime);
        GXSetTevColor(GX_TEVREG0, color);

        TVec3f cross = mPrints[i]._C.cross(mPrints[i]._18);
        TVec3f v1;
        if ((i & 1) != 0) {
            v1 = mPrints[i].mPos + cross * _2C;
        } else {
            v1 = mPrints[i].mPos - cross * _2C;
        }

        v1 += mPrints[i]._18 * 5.0f;

        TDDraw::drawTexture3D(v1, mPrints[i]._18, mPrints[i]._C, _34, _30, nullptr, mPrints[i]._29 != false, false);
    }

    TDDraw::close();
}

void FootPrint::clear() {
    _3C = true;
}

void FootPrint::clearForce() {
    mPrintNum = 0;
    mRotatingPrintIndex = 0;
}

TVec3f* FootPrint::getPrintPos(u32 index) const {
    return &mPrints[index % mPrintMaxNum].mPos;
}

void FootPrint::invalidate(u32 index) {
    mPrints[index % mPrintMaxNum].mIsValid = false;
}

bool FootPrint::isValid(u32 index) const {
    return mPrints[index % mPrintMaxNum].mIsValid != false;
}
