#include "Game/AreaObj/AreaObjContainer.hpp"
#include "Game/AreaObj/AreaObj.hpp"
#include "Game/AreaObj/CubeCamera.hpp"
#include "Game/AreaObj/GlaringLightArea.hpp"
#include "Game/AreaObj/ImageEffectArea.hpp"
#include "Game/AreaObj/LightAreaHolder.hpp"
#include "Game/AreaObj/WarpCube.hpp"
#include "Game/AreaObj/WaterArea.hpp"
#include "Game/Scene/SceneObjHolder.hpp"
#include <cstring>

namespace {
    template < typename T >
    AreaObjMgr* createManager(s32 type, const char* pName) {
        return new T(type, pName);
    }

    const Name2CreateManagerFunc cCreateTable[] = {
        {
            "SwitchArea",
            0x40,
            createManager< AreaObjMgr >,
        },
        {
            "RaceJudgeCube",
            0x40,
            createManager< AreaObjMgr >,
        },
        {
            "NinForceWindCube",
            0x40,
            createManager< AreaObjMgr >,
        },
        {
            "NinAbyssCube",
            0x40,
            createManager< AreaObjMgr >,
        },
        {
            "CubeCamera",
            0xA0,
            createManager< CubeCameraMgr >,
        },
        {
            "BindEndCube",
            0x40,
            createManager< AreaObjMgr >,
        },
        {
            "EffectCylinder",
            0x40,
            createManager< AreaObjMgr >,
        },
        {
            "DeathArea",
            0x40,
            createManager< AreaObjMgr >,
        },
        {
            "WarpCube",
            0x40,
            createManager< WarpCubeMgr >,
        },
        {
            "TripodBossStepStart",
            0x40,
            createManager< AreaObjMgr >,
        },
        {
            "Water",
            0x40,
            createManager< WaterAreaMgr >,
        },
        {
            "PlaneModeCube",
            0x10,
            createManager< AreaObjMgr >,
        },
        {
            "PlaneCircularModeCube",
            0x10,
            createManager< AreaObjMgr >,
        },
        {
            "PipeModeCube",
            0x4,
            createManager< AreaObjMgr >,
        },
        {
            "TowerModeCylinder",
            0x4,
            createManager< AreaObjMgr >,
        },
        {
            "ShadeCube",
            0x40,
            createManager< AreaObjMgr >,
        },
        {
            "PullBackCube",
            0x40,
            createManager< AreaObjMgr >,
        },
        {
            "PullBackCylinder",
            0x40,
            createManager< AreaObjMgr >,
        },
        {
            "RestartCube",
            0x40,
            createManager< AreaObjMgr >,
        },
        {
            "ChangeBgmCube",
            0x20,
            createManager< AreaObjMgr >,
        },
        {
            "BgmProhibitArea",
            0x4,
            createManager< AreaObjMgr >,
        },
        {
            "SoundEmitterCube",
            0x10,
            createManager< AreaObjMgr >,
        },
        {
            "SoundEmitterSphere",
            0x8,
            createManager< AreaObjMgr >,
        },
        {
            "PlaneCollisionCube",
            0x10,
            createManager< AreaObjMgr >,
        },
        {
            "ForbidTriangleJumpCube",
            0x10,
            createManager< AreaObjMgr >,
        },
        {
            "ForbidWaterSearchCube",
            0x10,
            createManager< AreaObjMgr >,
        },
        {
            "QuakeEffectArea",
            0x10,
            createManager< AreaObjMgr >,
        },
        {
            "HazeCube",
            0x10,
            createManager< AreaObjMgr >,
        },
        {
            "AudioEffectArea",
            0x10,
            createManager< AreaObjMgr >,
        },
        {
            "BigBubbleGoalArea",
            0x10,
            createManager< AreaObjMgr >,
        },
        {
            "SunLightArea",
            0x10,
            createManager< AreaObjMgr >,
        },
        {
            "GlaringLightArea",
            0x40,
            createManager< GlaringLightAreaMgr >,
        },
        {
            "ViewGroupCtrlCube",
            0x40,
            createManager< AreaObjMgr >,
        },
        {
            "LensFlareArea",
            0x40,
            createManager< AreaObjMgr >,
        },
        {
            "CameraRepulsiveArea",
            0x80,
            createManager< AreaObjMgr >,
        },
        {
            "LightArea",
            0x80,
            createManager< LightAreaHolder >,
        },
        {
            "FallsCube",
            0x20,
            createManager< AreaObjMgr >,
        },
        {
            "MercatorCube",
            0x1,
            createManager< AreaObjMgr >,
        },
        {
            "AstroChangeStageCube",
            0x10,
            createManager< AreaObjMgr >,
        },
        {
            "ImageEffectArea",
            0x20,
            createManager< ImageEffectAreaMgr >,
        },
        {
            "BlueStarGuidanceCube",
            0x10,
            createManager< AreaObjMgr >,
        },
        {
            "TicoSeedGuidanceCube",
            0x10,
            createManager< AreaObjMgr >,
        },
        {
            "MessageArea",
            0x10,
            createManager< AreaObjMgr >,
        },
        {
            "SmokeEffectColorArea",
            0x10,
            createManager< AreaObjMgr >,
        },
        {
            "BeeWallShortDistArea",
            0x10,
            createManager< AreaObjMgr >,
        },
        {
            "ExtraWallCheckArea",
            0x10,
            createManager< AreaObjMgr >,
        },
        {
            "ExtraWallCheckCylinder",
            0x10,
            createManager< AreaObjMgr >,
        },
        {
            "SpinGuidanceCube",
            0x10,
            createManager< AreaObjMgr >,
        },
        {
            "HipDropGuidanceCube",
            0x10,
            createManager< AreaObjMgr >,
        },
        {
            "TamakoroMoveGuidanceCube",
            0x10,
            createManager< AreaObjMgr >,
        },
        {
            "TamakoroJumpGuidanceCube",
            0x10,
            createManager< AreaObjMgr >,
        },
        {
            "BigBubbleGuidanceCube",
            0x10,
            createManager< AreaObjMgr >,
        },
        {
            "HeavySteeringCube",
            0x10,
            createManager< AreaObjMgr >,
        },
        {
            "NonSleepCube",
            0x10,
            createManager< AreaObjMgr >,
        },
        {
            "AreaMoveSphere",
            0x10,
            createManager< AreaObjMgr >,
        },
        {
            "DodoryuClosedCylinder",
            0x8,
            createManager< AreaObjMgr >,
        },
        {
            "DashChargeCylinder",
            0x8,
            createManager< AreaObjMgr >,
        },
        {
            "PlayerSeArea",
            0x8,
            createManager< AreaObjMgr >,
        },
        {
            "RasterScrollCube",
            0x8,
            createManager< AreaObjMgr >,
        },
        {
            "OnimasuCube",
            0x20,
            createManager< AreaObjMgr >,
        },
        {
            "ForbidJumpCube",
            0x8,
            createManager< AreaObjMgr >,
        },
        {
            "CollisionArea",
            0x40,
            createManager< AreaObjMgr >,
        },
        {
            "AstroOverlookArea",
            0x8,
            createManager< AreaObjMgr >,
        },
        {
            "CelestrialSphere",
            0x4,
            createManager< AreaObjMgr >,
        },
        {
            "MirrorArea",
            0x10,
            createManager< AreaObjMgr >,
        },
        {
            "DarkMatterCube",
            0x40,
            createManager< AreaObjMgr >,
        },
        {
            "DarkMatterCylinder",
            0x20,
            createManager< AreaObjMgr >,
        },
    };
};  // namespace

AreaObjContainer::AreaObjContainer(const char* pName) : NameObj(pName) {
    mNumManagers = 0;
}

void AreaObjContainer::init(const JMapInfoIter& rIter) {
    for (s32 i = 0; i < sizeof(cCreateTable) / sizeof(*cCreateTable); i++) {
        const Name2CreateManagerFunc* pName2CreateManagerFunc = &cCreateTable[i];
        AreaObjMgr* mgr = pName2CreateManagerFunc->mCreateFunc(pName2CreateManagerFunc->_4, pName2CreateManagerFunc->mName);
        JMapInfoIter iter;
        iter.mInfo = nullptr;
        iter.mIndex = -1;
        mgr->init(iter);
        s32 num = mNumManagers;
        mNumManagers++;
        mManagerArray[num] = mgr;
    }
}

/* this function is nearly impossible to match without context behind the random stack storages */
AreaObjMgr* AreaObjContainer::getManager(const char* pName) const {
    const char* str;
    AreaObjMgr** first = (AreaObjMgr**)&mManagerArray[0];
    AreaObjMgr** last = (AreaObjMgr**)&mManagerArray[mNumManagers];

    while ((str && str == pName) == false) {
        if (++first == last)
            break;

        str = strstr(pName, (*first)->mName);
    }

    return *first;
}

AreaObj* AreaObjContainer::getAreaObj(const char* pName, const TVec3f& rVec) const {
    return getManager(pName)->find_in(rVec);
}

namespace MR {
    AreaObjContainer* getAreaObjContainer() {
        return MR::getSceneObj< AreaObjContainer >(SceneObj_AreaObjContainer);
    }
};  // namespace MR

AreaObjContainer::~AreaObjContainer() {}
