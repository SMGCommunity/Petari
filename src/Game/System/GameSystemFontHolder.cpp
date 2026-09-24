#include "Game/System/GameSystemFontHolder.hpp"
#include "Game/System/FileLoader.hpp"
#include "Game/System/Language.hpp"
#include "Game/Util/FileUtil.hpp"
#include <JSystem/JKernel/JKRHeap.hpp>
#include <JSystem/JKernel/JKRMemArchive.hpp>
#include <cstdio>
#include <nw4r/ut/ResFont.h>

GameSystemFontHolder::GameSystemFontHolder() : _0(), _4(), mEmbeddedMessageFont(), mMessageFont(), mPictureFont(), mMenuFont(), mNumberFont() {
}

nw4r::ut::Font* GameSystemFontHolder::getMessageFont() const {
    if (mEmbeddedMessageFont != nullptr) {
        return mEmbeddedMessageFont;
    }

    return mMessageFont;
}

void GameSystemFontHolder::createFontFromEmbeddedData() {
    JKRArchive* pArchive = nullptr;
    JKRHeap* pHeap = nullptr;

    MR::getMountedArchiveAndHeap("ErrorMessageArchive.arc", &pArchive, &pHeap);

    char embeddedFontPath[256];
    snprintf(embeddedFontPath, sizeof(embeddedFontPath), "/%s/LayoutData/EmbeddedFont.arc", MR::getCurrentLanguagePrefix());

    _0 = static_cast< u8* >(MR::decompressFileFromArchive(pArchive, embeddedFontPath, pHeap, -32));
    _4 = new (pHeap, 0) JKRMemArchive();
    _4->mountFixed(_0, JKR_MEM_BREAK_FLAG_0);
    mEmbeddedMessageFont = new (pHeap, -4) nw4r::ut::ResFont();
    mEmbeddedMessageFont->SetResource(_4->getResource("MessageFont26.brfnt"));
}

namespace {
    inline nw4r::ut::ResFont* createFont(JKRArchive* pArchive, const char* pName) {
        nw4r::ut::ResFont* pFont = new nw4r::ut::ResFont();
        pFont->SetResource(pArchive->getResource(pName));
        return pFont;
    }
}  // namespace

void GameSystemFontHolder::createFontFromFile() {
    JKRMemArchive* pArchive = MR::receiveArchive("/LayoutData/Font.arc");

    mMessageFont = createFont(pArchive, "/MessageFont26.brfnt");

    mPictureFont = createFont(pArchive, "/PictureFont.brfnt");

    mMenuFont = createFont(pArchive, "/MenuFont64.brfnt");

    mNumberFont = createFont(pArchive, "/NumberFont.brfnt");

    mCinemaFont = createFont(pArchive, "/CinemaFont26.brfnt");

    mMessageFont->SetAlternateChar('?');
    mPictureFont->SetAlternateChar('?');
    mMenuFont->SetAlternateChar('?');
    mNumberFont->SetAlternateChar('?');
    mCinemaFont->SetAlternateChar('?');

    if (mEmbeddedMessageFont != nullptr) {
        mEmbeddedMessageFont->RemoveResource();

        delete mEmbeddedMessageFont;
        delete _4;
        delete[] _0;

        mEmbeddedMessageFont = nullptr;
        _0 = nullptr;
    }
}
