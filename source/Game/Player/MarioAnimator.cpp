#include "Game/Player/MarioAnimator.hpp"
#include "Game/Animation/XanimeResource.hpp"
#include "Game/Player/MarioActor.hpp"

#include "Game/Animation/XanimePlayer.hpp"
#include "Game/Player/MarioAnimatorData.hpp"

MarioAnimator::MarioAnimator(MarioActor *actor) : MarioModule(actor)
{
    init();
}

void MarioAnimator::init()
{
    XanimeSwapTable *luigiAnimations = nullptr;
    if (gIsLuigi) {
        luigiAnimations = luigiAnimeSwapTable;
    }
    _8 = new XanimeResourceTable(MR::getResourceHolder(mActor), marioAnimeTable, marioAnimeAuxTable, marioAnimeOfsTable, &singleAnimeTable[0].parent, &doubleAnimeTable[0].parent, &tripleAnimeTable[0].parent, &quadAnimeTable[0].parent, luigiAnimations);

    _14 = 0;
    _15 = 0;
    _16 = 1;

    _18 = 0.0f;
    _1C = 0.0f;
    _20 = 0.0f;
    _24 = 1.0f;

    PSMTXIdentity(_28.toMtxPtr());
    PSMTXIdentity(_7C.toMtxPtr());
    PSMTXIdentity(_AC.toMtxPtr());

    _58 = 0.0f;
    _5C = 0.0f;
    _60.zero();

    _6C = 0;
    _10C = 0;
    _10D = 0;
    _10E = 0;
    _114 = 0;
    _118 = 0.0f;
    _70 = 0.0f;
    _74 = 0;
    _78 = 0;
    _110 = 0.0f;

    initCallbackTable();

    _C = new XanimePlayer(MR::getJ3DModel(mActor), _8);

    f1("Šî–{");

    _C->getCore()->enableJointTransform(MR::getJ3DModelData(mActor));

    mActor->mModelManager->mXanimePlayer = _C;
    _10 = new XanimePlayer(MR::getJ3DModel(mActor), _8, _C);
    changeDefaultUpper("Šî–{");
    _10->changeAnimation("Šî–{");
    _10->mCore->shareJointTransform(_C->mCore);
    PSMTXCopy(MR::tmpMtxRotYRad(3.14159274101f), _DC.toMtxPtr());
}

bool MarioAnimator::isAnimationStop() const {
    return _C->_5C - _C->_60 == 0;
}
