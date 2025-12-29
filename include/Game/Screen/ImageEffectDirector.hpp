#pragma once

#include "Game/NameObj/NameObj.hpp"

class ImageEffectArea;
class ImageEffectBase;
class ImageEffectState;

namespace ImageEffectStateImpl {
    class StateBloomNormal;
    class StateBloomSimple;
    class StateDepthOfField;
    class StateNull;
    class StateScreenBlur;
};  // namespace ImageEffectStateImpl

class ImageEffectDirector : public NameObj {
public:
    /// @brief Creates a new `ImageEffectDirector`.
    /// @param pName A pointer to the null-terminated name of the object.
    ImageEffectDirector(const char* pName);

    virtual void movement();

    void setCurrentEffect(ImageEffectBase*);
    void turnOnNormal();
    void turnOnDepthOfField(bool);
    void turnOff();
    void forceOff();
    void setAuto();
    void turnOffDOFInSubjective();
    void turnOnDOFInSubjective();
    void setNormalBloomIntensity(u8);
    void setNormalBloomThreshold(u8);
    void setNormalBloomBlurIntensity1(u8);
    void setNormalBloomBlurIntensity2(u8);
    void setDepthOfFieldIntensity(f32);
    void updateManual();
    void setState(ImageEffectState*);
    void setBloomNormalParams(ImageEffectArea*);
    void setBloomSimpleParams(ImageEffectArea*);
    void setScreenBlurParams(ImageEffectArea*);
    void setDepthOfFieldParams(ImageEffectArea*);
    void initPlayerSyncIntensity(ImageEffectArea*, ImageEffectState*);
    void updatePlayerSyncIntensity(ImageEffectArea*, ImageEffectState*);
    void updateSyncCounter();
    void incPlayerSyncIntensity();
    void decPlayerSyncIntensity();
    void setPlayerSync(bool);

    /* 0x0C */ bool mIsAuto;
    /* 0x0D */ bool mIsPlayerSync;
    /* 0x0E */ bool _E;
    /* 0x0F */ bool _F;
    /* 0x10 */ s32 mPlayerSyncIntensity;
    /* 0x14 */ f32 mDepthOfFieldIntensity;
    /* 0x18 */ ImageEffectStateImpl::StateNull* _18;
    /* 0x1C */ ImageEffectStateImpl::StateBloomNormal* _1C;
    /* 0x20 */ ImageEffectStateImpl::StateBloomSimple* _20;
    /* 0x24 */ ImageEffectStateImpl::StateScreenBlur* _24;
    /* 0x28 */ ImageEffectStateImpl::StateDepthOfField* _28;
    /* 0x2C */ ImageEffectState* mState;
    /* 0x30 */ ImageEffectBase* mCurrentEffect;
};
