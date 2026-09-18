#include "Game/System/GameSystem.hpp"
#include "Game/Util/ExSingletonHolder.hpp"
#include "Game/Util/FileUtil.hpp"
#include "Game/Util/MemoryUtil.hpp"
#include "JSystem/JKernel/JKRExpHeap.hpp"
#include "JSystem/JKernel/JKRHeap.hpp"
#include "Kamek.hpp"
#include "Karipon/Network/NetworkSystem.hpp"

namespace {
    static void onGameSystemInitAfterStationedResourceLoaded(GameSystem* pGameSystem) {
        pGameSystem->initAfterStationedResourceLoaded();

        NetworkSystem* pNetworkSystem = ExSingletonHolder<NetworkSystem>::init();
        pNetworkSystem->initSystem(true);

        /*
        OSReport("NetworkSystem %d %d %d %d.%d.%d.%d\n",
            pNetworkSystem->mFd,
            pNetworkSystem->mError,
            pNetworkSystem->mState,
            pNetworkSystem->mIP.mOctets[0],
            pNetworkSystem->mIP.mOctets[1],
            pNetworkSystem->mIP.mOctets[2],
            pNetworkSystem->mIP.mOctets[3]);
        */
    }

    static void requestResourceForInitializeAudio(const char* pFilePath, JKRHeap* pHeap) {
        MR::mountAsyncArchive(pFilePath, pHeap);
        MR::loadAsyncToMainRAM("/Debug/GameAudioWaveTable.byaml", nullptr, MR::getStationedHeapGDDR3(), JKRDvdRipper::ALLOC_DIRECTION_FORWARD);
    }
} // namespace

extern kmSymbol initAfterStationedResourceLoaded__18GameSystemFunctionFv;
kmBranch(&initAfterStationedResourceLoaded__18GameSystemFunctionFv + 0x4, onGameSystemInitAfterStationedResourceLoaded);

extern kmSymbol requestResourceForInitialize__16AudSystemWrapperFv;
kmCall(&requestResourceForInitialize__16AudSystemWrapperFv + 0x74, requestResourceForInitializeAudio);
