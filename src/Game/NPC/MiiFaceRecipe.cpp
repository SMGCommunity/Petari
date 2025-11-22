#include "Game/NPC/MiiFaceRecipe.hpp"

MiiFaceRecipe::MiiFaceRecipe(RFLDataSource dataSource, int index, RFLResolution resolution, u32 expressionFlag)
    : mDataSource(dataSource),
      mMiddleDB(nullptr),
      mIndex(index),
      mResolution(resolution),
      mExpressionFlag(expressionFlag) {}

void MiiFaceRecipe::setDatabase(RFLMiddleDB* pMiddleDB) {
    mMiddleDB = pMiddleDB;
}

bool MiiFaceRecipe::hasExpression(RFLExpression expression) const {
    return (mExpressionFlag & (1 << expression)) != 0;
}

u32 MiiFaceRecipe::getModelBufferSize() const {
    return RFLGetModelBufferSize(mResolution, mExpressionFlag);
}

void MiiFaceRecipe::initFaceModel(RFLCharModel* pCharModel, void* pBuffer) const {
    RFLInitCharModel(pCharModel, mDataSource, mMiddleDB, mIndex, pBuffer, mResolution, mExpressionFlag);
}

RFLErrcode MiiFaceRecipe::makeIcon(void* pBuffer, int width, int height, GXColor bgColor) const {
    RFLIconSetting setting;

    setting.width = width;
    setting.height = height;
    setting.bgType = RFLIconBG_Direct;
    setting.bgColor = bgColor;
    setting.drawXluOnly = false;

    return RFLMakeIcon(pBuffer, mDataSource, mMiddleDB, mIndex, RFLExp_Normal, &setting);
}

RFLErrcode MiiFaceRecipe::makeIconWithFavoriteColor(void* pBuffer, int width, int height) const {
    RFLIconSetting setting;

    setting.width = width;
    setting.height = height;
    setting.bgType = RFLIconBG_Favorite;
    setting.drawXluOnly = false;

    return RFLMakeIcon(pBuffer, mDataSource, mMiddleDB, mIndex, RFLExp_Normal, &setting);
}
