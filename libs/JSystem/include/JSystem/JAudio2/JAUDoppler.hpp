#pragma once

#include <JSystem/JAudio2/JASSoundParams.hpp>
#include <JSystem/JAudio2/JAUAudible.hpp>
#include <JSystem/JGeometry/TVec.hpp>

class JAUDopplerAudibleAbsPos {
public:
    void init(TVec3f* pAudiblePos, const TVec3f& rPos, const TVec3f* pPos) {
        pAudiblePos->set(rPos);

        if (pPos != nullptr) {
            mPos.set(*pPos);
            mVelocity.sub(rPos, mPos);
        } else {
            mPos.set(rPos);
            mVelocity.zero();
        }
    }

    void calc(const TVec3f& rPos) {
        mVelocity.sub(rPos, mPos);
        mPos.set(rPos);
    }

    const TVec3f& getPos() const {
        return mPos;
    }

    const TVec3f& getVelocity() const {
        return mVelocity;
    }

    /* 0x00 */ TVec3f mPos;
    /* 0x0C */ TVec3f mVelocity;
};

class JAUDopplerAudibleRelPos {
public:
    const TVec3f& getPos() const {
        return mPos;
    }

    const TVec3f& getDir() const {
        return mDir;
    }

    f32 getDist() const {
        return mDist;
    }

    /* 0x00 */ TVec3f mPos;
    /* 0x0C */ TVec3f mDir;
    /* 0x18 */ f32 mDist;
};

class JAUDopplerAudibleChannel {
public:
    const TVec3f& getDir() const {
        return mRelPos.getDir();
    }

    /* 0x00 */ JASSoundParams mParams;
    /* 0x14 */ JAUDopplerAudibleRelPos mRelPos;
    /* 0x30 */ f32 mPitch;
};
