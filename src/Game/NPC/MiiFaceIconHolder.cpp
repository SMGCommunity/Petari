#include "Game/NPC/MiiFaceIcon.hpp"
#include "Game/NPC/MiiFaceIconHolder.hpp"
#include "Game/NameObj/NameObjAdaptor.hpp"
#include "Game/Scene/SceneObjHolder.hpp"
#include "Game/Util/Functor.hpp"
#include "Game/Util/ObjUtil.hpp"

MiiFaceIconHolder::MiiFaceIconHolder(u32 iconNum, const char* pName) :
    NameObj(pName),
    mIconNumMax(iconNum),
    mIconNum(0),
    mIcon(new MiiFaceIcon*[iconNum])
{
    MR::FunctorV0M<MiiFaceIconHolder*, void (MiiFaceIconHolder::*)()> drawFunc(
        this, &MiiFaceIconHolder::drawIcons);

    MR::connectToScene(MR::createDrawAdaptor("Miiアイコン生成", drawFunc), -1, -1, -1, 79);
}

void MiiFaceIconHolder::drawIcons() {
    for (int i = 0; i < mIconNum; i++) {
        if (mIcon[i]->mIsRequestMakeIcon) {
            mIcon[i]->drawIcon();
            break;
        }
    }
}

void MiiFaceIconHolder::registerIcon(MiiFaceIcon* pIcon) {
    mIcon[mIconNum++] = pIcon;
}

namespace MR {
    MiiFaceIconHolder* getMiiFaceIconHolder() {
        return MR::getSceneObj<MiiFaceIconHolder>(SceneObj_MiiFaceIconHolder);
    }

    void registerMiiFaceIcon(MiiFaceIcon* pIcon) {
        MR::createSceneObj(SceneObj_MiiFaceIconHolder);
        getMiiFaceIconHolder()->registerIcon(pIcon);
    }
};
