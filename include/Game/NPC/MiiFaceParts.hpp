#pragma once

#include "Game/LiveActor/LiveActor.hpp"
#include <JSystem/JGeometry/TMatrix.hpp>
#include <JSystem/JGeometry/TVec.hpp>

class FixedPosition;
class MiiFaceRecipe;
struct RFLCharModel;
struct RFLDrawCoreSetting;

class MiiFaceParts : public LiveActor {
public:
    MiiFaceParts(const char* pName, const MiiFaceRecipe& rRecipe);

    /* 0x08 */ virtual ~MiiFaceParts();
    /* 0x0C */ virtual void init(const JMapInfoIter& rIter);
    /* 0x1C */ virtual void calcAnim();
    /* 0x20 */ virtual void calcViewAndEntry();

    void drawOpa(const RFLDrawCoreSetting* pSetting) const;
    void drawXlu(const RFLDrawCoreSetting* pSetting) const;
    void initFixedPosition(MtxPtr, const TVec3f&, const TVec3f&);
    void changeExpressionNormal();
    void changeExpressionBlink();
    void changeFaceModel(const MiiFaceRecipe& rRecipe);
    void initFaceModel();
    bool tryChangeExpression(u32 expression);

    /* 0x8C */ RFLCharModel* mCharModel;
    /* 0x90 */ void* _90;
    /* 0x94 */ MiiFaceRecipe* mRecipe;
    /* 0x98 */ TPos3f _98;
    /* 0xC8 */ MtxPtr _C8;
    /* 0xCC */ FixedPosition* _CC;
    /* 0xD0 */ bool _D0;
    /* 0xD1 */ bool _D1;
};
