#include "Game/Effect/AutoEffectInfo.hpp"
#include "Game/Util/JMapInfo.hpp"
#include "Game/Util/StringUtil.hpp"

void AutoEffectInfo_FORCE_MATCH_SDATA2() {
    (void)1.0f;
}

extern "C" u32 strtoul(const char*, char**, int);

namespace {
    struct DrawOrderData {
        /* 0x00 */ s32 _0;
        /* 0x04 */ const char* _4;
    };

    static const DrawOrderData sDrawOrderDataTable[] = {
        {0, "3D"}, {1, "PAUSE_IGNORE"},    {2, "INDIRECT"},     {3, "AFTER_INDIRECT"}, {4, "BLOOM_EFFECT"}, {5, "AFTER_IMAGE_EFFECT"},
        {6, "2D"}, {7, "2D_PAUSE_IGNORE"}, {8, "FOR_2D_MODEL"},
    };

    const DrawOrderData* getDrawOrderData(const JMapInfoIter& rIter) {
        const char* drawOrder = "";
        rIter.getValue("DrawOrder", &drawOrder);

        for (u32 i = 0; i < ARRAY_SIZE(sDrawOrderDataTable); i++) {
            if (MR::isEqualString(drawOrder, sDrawOrderDataTable[i]._4)) {
                return &sDrawOrderDataTable[i];
            }
        }

        return nullptr;
    }
};  // namespace

namespace {
    u32 str2Color(const char* pStr) NO_INLINE {
        return strtoul(pStr + 1, nullptr, 16) * 256;
    }

    bool isValueOnR(const JMapInfoIter& rIter, const char* pName) NO_INLINE {
        const char* value = "";
        rIter.getValue(pName, &value);

        return strchr(value, 'R') != nullptr;
    }

    bool isValueOnS(const JMapInfoIter& rIter, const char* pName) NO_INLINE {
        const char* value = "";
        rIter.getValue(pName, &value);

        return strchr(value, 'S') != nullptr;
    }

    bool isValueOnT(const JMapInfoIter& rIter, const char* pName) NO_INLINE {
        const char* value = "";
        rIter.getValue(pName, &value);

        return strchr(value, 'T') != nullptr;
    }

    const char* getStringValue(const JMapInfoIter& rIter, const char* pName) NO_INLINE {
        const char* value = "";
        rIter.getValue(pName, &value);

        if (MR::isEqualString(value, "")) {
            return nullptr;
        }

        return value;
    }
};  // namespace

AutoEffectInfo::AutoEffectInfo()
    : mGroupName(), mAnimName(), mUniqueName(), mEffectName(), mParentName(), mJointName(), mFlag(), mPrmColor(-1), mIsValidPrmColor(), mEnvColor(),
      mIsValidEnvColor(), mOffsetX(), mOffsetY(), mOffsetZ(), mStartFrame(), mEndFrame(-1), mScaleValue(1.0f), mRateValue(1.0f), mLightAffectValue(),
      mDrawOrderData() {
}

void AutoEffectInfo::init(const JMapInfoIter& rIter) {
    mGroupName = ::getStringValue(rIter, "GroupName");
    mUniqueName = ::getStringValue(rIter, "UniqueName");
    mAnimName = ::getStringValue(rIter, "AnimName");

    const char* continueAnimEnd = "";
    rIter.getValue("ContinueAnimEnd", &continueAnimEnd);

    if (MR::isEqualString(continueAnimEnd, "on")) {
        mFlag |= FLAG_CONTINUE_ANIM_END;
    } else {
        mFlag &= ~FLAG_CONTINUE_ANIM_END;
    }

    mJointName = ::getStringValue(rIter, "JointName");
    mEffectName = ::getStringValue(rIter, "EffectName");
    mParentName = ::getStringValue(rIter, "ParentName");
    rIter.getValue("OffsetX", &mOffsetX);
    rIter.getValue("OffsetY", &mOffsetY);
    rIter.getValue("OffsetZ", &mOffsetZ);
    rIter.getValue("StartFrame", &mStartFrame);
    rIter.getValue("EndFrame", &mEndFrame);

    if (::isValueOnT(rIter, "Affect")) {
        mFlag |= FLAG_AFFECT_TRANS;
    } else {
        mFlag &= ~FLAG_AFFECT_TRANS;
    }

    if (::isValueOnR(rIter, "Affect")) {
        mFlag |= FLAG_AFFECT_ROTATE;
    } else {
        mFlag &= ~FLAG_AFFECT_ROTATE;
    }

    if (::isValueOnS(rIter, "Affect")) {
        mFlag |= FLAG_AFFECT_SCALE;
    } else {
        mFlag &= ~FLAG_AFFECT_SCALE;
    }

    if (::isValueOnT(rIter, "Follow")) {
        mFlag |= FLAG_FOLLOW_TRANS;
    } else {
        mFlag &= ~FLAG_FOLLOW_TRANS;
    }

    if (::isValueOnR(rIter, "Follow")) {
        mFlag |= FLAG_FOLLOW_ROTATE;
    } else {
        mFlag &= ~FLAG_FOLLOW_ROTATE;
    }

    if (::isValueOnS(rIter, "Follow")) {
        mFlag |= FLAG_FOLLOW_SCALE;
    } else {
        mFlag &= ~FLAG_FOLLOW_SCALE;
    }

    rIter.getValue("ScaleValue", &mScaleValue);
    rIter.getValue("RateValue", &mRateValue);

    const char* prmColor = "";
    rIter.getValue("PrmColor", &prmColor);
    mIsValidPrmColor = !MR::isEqualString(prmColor, "");

    if (mIsValidPrmColor) {
        u32 color = ::str2Color(prmColor);

        mPrmColor.set((GXColor){color >> 24, color >> 16, color >> 8, color >> 0});
    }

    const char* envColor = "";
    rIter.getValue("EnvColor", &envColor);
    mIsValidEnvColor = !MR::isEqualString(envColor, "");

    if (mIsValidEnvColor) {
        u32 color = ::str2Color(envColor);

        mEnvColor.set((GXColor){color >> 24, color >> 16, color >> 8, color >> 0});
    }

    rIter.getValue("LightAffectValue", &mLightAffectValue);
    mDrawOrderData = ::getDrawOrderData(rIter);
}

const char* AutoEffectInfo::getName() const {
    if (mUniqueName != nullptr) {
        return mUniqueName;
    }

    return mEffectName;
}
