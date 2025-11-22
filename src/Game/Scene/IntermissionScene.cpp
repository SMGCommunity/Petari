#include "Game/Scene/IntermissionScene.hpp"
#include "Game/Util/ActorSensorUtil.hpp"
#include "Game/Util/DrawUtil.hpp"
#include <JSystem/JUtility/JUTVideo.hpp>
#include <cstdio>
#include <va_list.h>

IntermissionScene::IntermissionScene() : Scene("IntermissionScene") {
    _54 = 0;
    mState[0] = 0;
}

void IntermissionScene::update() {
    _54++;
}

// https://decomp.me/scratch/smCaD
void IntermissionScene::draw() const {
    J2DOrthoGraphSimple graph;
    graph.setPort();
    JUtility::TColor v1;
    v1.r = 0;
    v1.g = 0;
    v1.b = 0;
    v1.a = -1;
    JUtility::TColor v4;
    v4.r = 255;
    v4.g = 255;
    v4.b = 255;
    v4.a = 255;
    graph.setColor(v1, v1, v1, v4);

    f32 height = JUTVideo::sManager->mRenderModeObj->efbHeight;
    f32 width = MR::getScreenWidth();
    TBox2f box(0.0f, 0.0f, 0.0f + height, 0.0f + width);
    graph.fillBox(box);
}

void IntermissionScene::setCurrentSceneControllerState(const char* pState, ...) {
    va_list list;
    va_start(list, pState);
    vsnprintf(mState, sizeof(mState), pState, list);
    va_end();
    _54 = 0;
}
