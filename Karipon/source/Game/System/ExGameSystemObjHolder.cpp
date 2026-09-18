#include "Game/System/ExGameSystemObjHolder.hpp"
#include "Game/System/GameSystem.hpp"
#include "Kamek.hpp"

namespace {
    static ExGameSystemObjHolder* ctorExGameSystemObjHolder(ExGameSystemObjHolder* pObjHolder) {
        pObjHolder->mSystemDebugMenu = new SystemDebugMenu();
        return pObjHolder;
    }

    static void drawGameSystem(GameSystem* pGameSystem) {
        pGameSystem->draw();

        ExGameSystemObjHolder* pSystemObjHolder = reinterpret_cast<ExGameSystemObjHolder*>(pGameSystem->mObjHolder);
        pSystemObjHolder->mSystemDebugMenu->update();
    }
} // namespace

extern kmSymbol init__10GameSystemFv;
kmWrite16(&init__10GameSystemFv + 0x2E, sizeof(ExGameSystemObjHolder));

extern kmSymbol __ct__19GameSystemObjHolderFv;
kmBranch(&__ct__19GameSystemObjHolderFv + 0x9C, ctorExGameSystemObjHolder);

extern kmSymbol frameLoop__10GameSystemFv;
kmCall(&frameLoop__10GameSystemFv + 0x20, drawGameSystem);
