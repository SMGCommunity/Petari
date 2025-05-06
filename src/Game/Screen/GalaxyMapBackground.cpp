#include "Game/Screen/GalaxyMapBackground.hpp"
#include "Game/Util/LayoutUtil.hpp"

GalaxyMapBackground::GalaxyMapBackground() :
    LayoutActor("背景", true)
{
    initLayoutManager("MapGalaxyBg", 1);
}

void GalaxyMapBackground::appear() {
    LayoutActor::appear();
    MR::startAnim(this, "Wait", 0);
}

GalaxyMapBackground::~GalaxyMapBackground() {
    
}
