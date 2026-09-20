#include "Game/Effect/AutoEffectInfo.hpp"
#include "Game/Util/JMapInfo.hpp"
#include "Game/Util/StringUtil.hpp"

void AutoEffectInfo_FORCE_MATCH_SDATA2() {
    (void)1.0f;
}

extern "C" u32 strtoul(const char*, char**, int);

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

    bool isValueOnT(const JMapInfoIter& rIter, const char* pName) NO_INLINE;

    const char* getStringValue(const JMapInfoIter& rIter, const char* pName) NO_INLINE {
        const char* value = "";
        rIter.getValue(pName, &value);

        if (MR::isEqualString(value, "")) {
            return nullptr;
        }

        return value;
    }
};  // namespace

namespace {
    struct DrawOrderData {
        /* 0x00 */ s32 _0;
        /* 0x04 */ const char* _4;
    };

    static const DrawOrderData sDrawOrderDataTable[] = {
        {0, "3D"}, {1, "PAUSE_IGNORE"},    {2, "INDIRECT"},     {3, "AFTER_INDIRECT"}, {4, "BLOOM_EFFECT"}, {5, "AFTER_IMAGE_EFFECT"},
        {6, "2D"}, {7, "2D_PAUSE_IGNORE"}, {8, "FOR_2D_MODEL"},
    };

    s32 getDrawOrderData(const char* pDrawOrder) {
        for (u32 i = 0; i < ARRAY_SIZE(sDrawOrderDataTable); i++) {
            if (MR::isEqualString(pDrawOrder, sDrawOrderDataTable[i]._4)) {
                return sDrawOrderDataTable[i]._0;
            }
        }

        return 0;
    }

};  // namespace

namespace {
    bool isValueOn(const JMapInfoIter& rIter, const char* pName) {
        const char* pValue = "";
        rIter.getValue(pName, &pValue);
        return MR::isEqualString(pValue, "on");
    }

}  // namespace

AutoEffectInfo::AutoEffectInfo()
    : mGroupName(), mAnimName(), mUniqueName(), mEffectName(), mParentName(), mJointName(), mFlag(), mOffset(), mStartFrame(), mEndFrame(-1),
      mScaleValue(1.0f), mRateValue(1.0f), mLightAffectValue(), mDrawOrder() {
    mPrmColor.mColor = 0;
    mIsValidPrmColor = false;
    mEnvColor.mColor = 0;
    mIsValidEnvColor = false;
}

void AutoEffectInfo::init(const JMapInfoIter& rIter) {
    mGroupName = ::getStringValue(rIter, "GroupName");
    mUniqueName = ::getStringValue(rIter, "UniqueName");
    mAnimName = ::getStringValue(rIter, "AnimName");

    if (::isValueOn(rIter, "ContinueAnimEnd")) {
        mFlag |= FLAG_CONTINUE_ANIM_END;
    } else {
        mFlag &= ~FLAG_CONTINUE_ANIM_END;
    }

    mJointName = ::getStringValue(rIter, "JointName");
    mEffectName = ::getStringValue(rIter, "EffectName");
    mParentName = ::getStringValue(rIter, "ParentName");
    rIter.getValue("OffsetX", &mOffset.x);
    rIter.getValue("OffsetY", &mOffset.y);
    rIter.getValue("OffsetZ", &mOffset.z);
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
        mPrmColor.set(Color8(::str2Color(prmColor)));
    }

    const char* envColor = "";
    rIter.getValue("EnvColor", &envColor);
    mIsValidEnvColor = !MR::isEqualString(envColor, "");

    if (mIsValidEnvColor) {
        mEnvColor.set(Color8(::str2Color(envColor)));
    }

    rIter.getValue("LightAffectValue", &mLightAffectValue);
    const char* pDrawOrder = "";
    rIter.getValue("DrawOrder", &pDrawOrder);
    mDrawOrder = ::getDrawOrderData(pDrawOrder);
}

const char* AutoEffectInfo::getName() const {
    if (mUniqueName != nullptr) {
        return mUniqueName;
    }

    return mEffectName;
}

namespace {
    bool isValueOnT(const JMapInfoIter& rIter, const char* pName) NO_INLINE {
        const char* value = "";
        rIter.getValue(pName, &value);

        return strchr(value, 'T') != nullptr;
    }

}  // namespace
