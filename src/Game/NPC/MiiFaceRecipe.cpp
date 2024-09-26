#include "Game/NPC/MiiFaceRecipe.hpp"
#include <RVLFaceLib.h>

MiiFaceRecipe::MiiFaceRecipe(RFLDataSource dataSource, int index, RFLResolution resolution, u32 expressionFlags) {
    mDataSource = dataSource;
    mMiddleDB = 0;
    mIndex = index;
    mResolution = resolution;
    mExpressionFlag = expressionFlags;
}

void MiiFaceRecipe::setDatabase(RFLMiddleDB *pDb) {
    mMiddleDB = pDb;
}

bool MiiFaceRecipe::hasExpression(RFLExpression expr) const {
    return (mExpressionFlag & (1 << expr)) != 0;
}

u32 MiiFaceRecipe::getModelBufferSize() const {
    return RFLGetModelBufferSize(mResolution, mExpressionFlag);
}

void MiiFaceRecipe::initFaceModel(RFLCharModel *pModel, void *pData) const {
    RFLInitCharModel(pModel, mDataSource, mMiddleDB, mIndex, pData, mResolution, mExpressionFlag);
}

RFLErrcode MiiFaceRecipe::makeIcon(void *pData, int width, int height, _GXColor color) const {
    RFLIconSetting settings;
    settings.width = width;
    settings.height = height;
    settings.bgType = RFLIconBG_Direct;
    settings.bgColor.r = color.r;
    settings.bgColor.g = color.g;
    settings.bgColor.b = color.b;
    settings.bgColor.a = color.a;
    settings.drawXluOnly = false;
    return RFLMakeIcon(pData, mDataSource, mMiddleDB, mIndex, RFLExp_Normal, &settings);
}

RFLErrcode MiiFaceRecipe::makeIconWithFavoriteColor(void *pData, int width, int height) const {
    RFLIconSetting settings;
    settings.width = width;
    settings.height = height;
    settings.bgType = RFLIconBG_Favorite;
    settings.drawXluOnly = false;
    return RFLMakeIcon(pData, mDataSource, mMiddleDB, mIndex, RFLExp_Normal, &settings);
}
