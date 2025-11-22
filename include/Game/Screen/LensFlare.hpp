#pragma once

#include "Game/LiveActor/LiveActor.hpp"
#include "Game/Util/Array.hpp"

class BrightCamInfo;
class BrightObjBase;
class TriggerChecker;

class LensFlareModel : public LiveActor {
public:
    /// @brief Creates a new `LensFlareModel`.
    /// @param pName The pointer to the null-terminated name of the object.
    /// @param pArcName The pointer to the null-terminated base name of the resource archive.
    LensFlareModel(const char* pName, const char* pArcName);

    virtual void appear();
    virtual void control();
    virtual void appearAnim();
    virtual void controlAnim();

    void update(bool, bool);
    void exeHide();
    void exeShow();
    void exeFadeIn();
    void exeFadeOut();
    void notifyInArea();

private:
    /* 0x8C */ TVec3f          _8C;
    /* 0x98 */ TriggerChecker* _98;
    /* 0x9C */ TriggerChecker* _9C;
};

class LensFlareRing : public LensFlareModel {
public:
    /// @brief Creates a new `LensFlareRing`.
    LensFlareRing();

    virtual void appearAnim();
    virtual void controlAnim();
};

class LensFlareGlow : public LensFlareModel {
public:
    /// @brief Creates a new `LensFlareGlow`.
    LensFlareGlow();

    virtual void appearAnim();
    virtual void controlAnim();
};

class LensFlareLine : public LensFlareModel {
public:
    /// @brief Creates a new `LensFlareLine`.
    LensFlareLine();

    virtual void appearAnim();
    virtual void controlAnim();
};

class LensFlareDirector : public NameObj {
public:
    /// @brief Creates a new `LensFlareDirector`.
    LensFlareDirector();

    /// @brief Intializes the `LensFlareDirector` while being placed into a scene.
    /// @param rIter The reference to an iterator over a `JMapInfo`.
    virtual void init(const JMapInfoIter& rIter);

    virtual void movement();
    virtual void drawSyncCallback(u16);

    void pauseOff();
    void setDrawSyncToken();
    s32  checkArea();
    bool checkBrightObj(bool);
    void controlFlare(s32, bool);

private:
    /* 0x0C */ void*                                              _C;
    /* 0x10 */ LensFlareRing*                                     mRing;
    /* 0x14 */ LensFlareGlow*                                     mGlow;
    /* 0x18 */ LensFlareLine*                                     mLine;
    /* 0x1C */ MR::Vector< MR::FixedArray< BrightObjBase*, 16 > > mBrightObjArray;
    /* 0x60 */ TVec2f                                             _60;
    /* 0x68 */ f32                                                _68;
    /* 0x6C */ TVec2f                                             _6C;
    /* 0x74 */ f32                                                _74;
    /* 0x78 */ f32                                                _78;
    /* 0x7C */ u16                                                _7C;
    /* 0x7E */ u16                                                mDrawSyncTokenIndex;
    /* 0x80 */ BrightCamInfo*                                     mBrightCamInfo;
};

namespace MR {
    void addBrightObj(BrightObjBase* pBrightObj);
    void setLensFlareDrawSyncToken();
    u16  getLensFlareDrawSyncTokenIndex();
}; // namespace MR
