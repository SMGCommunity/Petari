#include "Game/Screen/SimpleLayout.hpp"
#include "Game/Scene/SceneFunction.hpp"
#include "Game/Util/ObjUtil.hpp"

SimpleLayout::SimpleLayout(const char* pName, const char* pLayoutName, u32 a3, int drawType) : LayoutActor(pName, true) {
    int type = drawType >= 0 ? drawType : MR::DrawType_Layout;

    MR::connectToScene(this, MR::MovementType_Layout, MR::CalcAnimType_Layout, -1, type);
    initLayoutManager(pLayoutName, a3);
}

SimpleEffectLayout::SimpleEffectLayout(const char* pName, const char* pLayoutName, u32 a3, int a4) : SimpleLayout(pName, pLayoutName, a3, a4) {
    initEffectKeeper(0, nullptr, nullptr);
}
