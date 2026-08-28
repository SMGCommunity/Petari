#pragma once

#include <revolution/types.h>

class ImageEffectBase;
class ImageEffectDirector;

class ImageEffectState {
public:
    /// @brief Creates a new `ImageEffectState`.
    /// @param pHost A pointer to the owning `ImageEffectDirector` instance.
    ImageEffectState(ImageEffectDirector* pHost) : mHost(pHost) {
    }

    /// @brief Destroys the `ImageEffectState`.
    /* 0x08 */ virtual ~ImageEffectState() {
    }

    /* 0x0C */ virtual void update();
    /* 0x10 */ virtual bool doesEffectExist() const = 0;
    /* 0x14 */ virtual ImageEffectBase* getEffect() const = 0;
    /* 0x18 */ virtual void onChange() {
    }

    void forceOff();

private:
    /* 0x04 */ ImageEffectDirector* mHost;
};

namespace ImageEffectStateImpl {
    class StateNull : public ImageEffectState {
    public:
        /// @brief Creates a new `StateNull`.
        /// @param pHost A pointer to the owning `ImageEffectDirector` instance.
        StateNull(ImageEffectDirector* pHost) : ImageEffectState(pHost) {
        }

        /* 0x10 */ virtual bool doesEffectExist() const {
            return true;
        }

        /* 0x14 */ virtual ImageEffectBase* getEffect() const {
            return nullptr;
        }
    };

    class StateBloomNormal : public ImageEffectState {
    public:
        /// @brief Creates a new `StateBloomNormal`.
        /// @param pHost A pointer to the owning `ImageEffectDirector` instance.
        StateBloomNormal(ImageEffectDirector* pHost);

        /* 0x0C */ virtual void update();
        /* 0x10 */ virtual bool doesEffectExist() const;
        /* 0x14 */ virtual ImageEffectBase* getEffect() const;
        /* 0x18 */ virtual void onChange();

        void setBloomIntensity(u8 bloomIntensity);
        void setThreshold(u8 threshold);
        void setIntensity1(u8 intensity1);
        void setIntensity1Default();
        void setIntensity2(u8 intensity2);
        void setIntensity2Default();

    private:
        /* 0x08 */ bool _8;
        /* 0x0C */ u32 _C;
        /* 0x10 */ f32 _10;
        /* 0x14 */ u8 mBloomIntensity;
        /* 0x18 */ f32 _18;
        /* 0x1C */ u8 mThreshold;
        /* 0x20 */ f32 _20;
        /* 0x24 */ u32 mIntensity1;
        /* 0x28 */ f32 _28;
        /* 0x2C */ u32 mIntensity2;
    };

    class StateBloomSimple : public ImageEffectState {
    public:
        /// @brief Creates a new `StateBloomSimple`.
        /// @param pHost A pointer to the owning `ImageEffectDirector` instance.
        StateBloomSimple(ImageEffectDirector* pHost) : ImageEffectState(pHost) {
        }

        /* 0x10 */ virtual bool doesEffectExist() const;
        /* 0x14 */ virtual ImageEffectBase* getEffect() const;

        void setMaskFilterAll();
        void setMaskFilterRed();
        void setMaskFilterGreen();
        void setMaskFilterBlue();
        void setThreshold(u8 threshold);
        void setIntensity(f32 intensity);
    };

    class StateScreenBlur : public ImageEffectState {
    public:
        /// @brief Creates a new `StateScreenBlur`.
        /// @param pHost A pointer to the owning `ImageEffectDirector` instance.
        StateScreenBlur(ImageEffectDirector* pHost) : ImageEffectState(pHost) {
        }

        /* 0x10 */ virtual bool doesEffectExist() const;
        /* 0x14 */ virtual ImageEffectBase* getEffect() const;

        void setIntensity(f32 intensity);
    };

    class StateDepthOfField : public ImageEffectState {
    public:
        /// @brief Creates a new `StateDepthOfField`.
        /// @param pHost A pointer to the owning `ImageEffectDirector` instance.
        StateDepthOfField(ImageEffectDirector* pHost) : ImageEffectState(pHost) {
        }

        /* 0x10 */ virtual bool doesEffectExist() const;
        /* 0x14 */ virtual ImageEffectBase* getEffect() const;

        void setIntensity(f32 intensity);
        void setBlurMaxDist(s32 blurMaxDist);
        void setBlurMinDist(s32 blurMinDist);
    };
};  // namespace ImageEffectStateImpl
