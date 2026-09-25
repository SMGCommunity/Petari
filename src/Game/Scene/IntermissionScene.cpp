#include "Game/Scene/IntermissionScene.hpp"
#include "Game/Util/ActorSensorUtil.hpp"
#include "Game/Util/DrawUtil.hpp"
#include "Game/Util/ScreenUtil.hpp"
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
    JUtility::TColor color(0, 0, 0, 255);
    graph.setColor(color);

    f32 height = static_cast< s32 >(JUTVideo::getManager()->getRenderMode()->efbHeight);
    f32 width = MR::getScreenWidth();
    graph.fillBox(0.0f, 0.0f, width, height);
}

void IntermissionScene::setCurrentSceneControllerState(const char* pState, ...) {
    va_list list;
    va_start(list, pState);
    vsnprintf(mState, sizeof(mState), pState, list);
    va_end();
    _54 = 0;
}
