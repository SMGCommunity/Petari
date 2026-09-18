#include "Game/AreaObj/AreaObjContainer.hpp"
#include "Game/AreaObj/AreaObj.hpp"
#include "Kamek.hpp"
#include "Macros.hpp"

namespace {
    template <typename T>
    static AreaObjMgr* createManager(s32 count, const char* pName) {
        return new T(count, pName);
    }

    static const Name2CreateManagerFunc cCreateTable[] = {
        {nullptr, nullptr, nullptr},
    };

    // The vanilla `AreaObjMgr` creator table has 67 `AreaObjMgr` creator functions while `AreaObjContainer::mManagerArray`
    // supports 80 entries so we should be good to do this without expanding the array
    static void initContainer(AreaObjContainer* pContainer) {
        for (const Name2CreateManagerFunc* pEntry = &cCreateTable[1]; pEntry < ARRAY_END(cCreateTable); pEntry++) {
            AreaObjMgr* pMgr = pEntry->mCreateFunc(pEntry->_4, pEntry->mName);
            pMgr->initWithoutIter();

            pContainer->mManagerArray[pContainer->mNumManagers++] = pMgr;
        }
    }

    // clang-format off
#ifdef __MWERKS__
    static asm void saveRegister(void) {
        nofralloc
        addi r11, r1, 0x30
        mr   r3, r25 // AreaObjContainer* pContainer
        blr
    }
#else
    static void saveRegister() {}
#endif
    // clang-format on

    static AreaObj* findIn(AreaObjMgr* pManager, const TVec3f& rPos) {
        return pManager ? pManager->find_in(rPos) : nullptr;
    }
} // namespace

extern kmSymbol init__16AreaObjContainerFRC12JMapInfoIter;
kmCall(&init__16AreaObjContainerFRC12JMapInfoIter + 0x90, saveRegister);
kmBranch(&init__16AreaObjContainerFRC12JMapInfoIter + 0xA4, initContainer);

extern kmSymbol getAreaObj__16AreaObjContainerCFPCcRCQ29JGeometry8TVec3f;
kmCall(&getAreaObj__16AreaObjContainerCFPCcRCQ29JGeometry8TVec3f + 0x1C, findIn);