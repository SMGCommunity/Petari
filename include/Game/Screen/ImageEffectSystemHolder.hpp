#pragma once

#include "Game/NameObj/NameObj.hpp"

class ImageEffectDirector;
class ImageEffectResource;

class ImageEffectSystemHolder : public NameObj {
public:
    /// @brief Creates a new `ImageEffectSystemHolder`.
    ImageEffectSystemHolder();

    void pauseOff();

    /* 0x0C */ ImageEffectResource* mResource;
    /* 0x10 */ ImageEffectDirector* mDirector;
};

namespace MR {
    void createImageEffectSystemHolder();
    ImageEffectSystemHolder* getImageEffectSystemHolder();
    bool isExistImageEffectDirector();
    ImageEffectDirector* getImageEffectDirector();
    ImageEffectResource* getImageEffectResource();
};  // namespace MR
