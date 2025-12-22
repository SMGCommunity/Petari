#include "Game/System/FileLoader.hpp"
#include "Game/System/GameSystemFontHolder.hpp"
#include "Game/System/Language.hpp"
#include "Game/Util/FileUtil.hpp"
#include <JSystem/JKernel/JKRHeap.hpp>
#include <JSystem/JKernel/JKRMemArchive.hpp>
#include <nw4r/ut/ResFont.h>
#include <cstdio>

GameSystemFontHolder::GameSystemFontHolder() :
    _0(nullptr),
    _4(nullptr),
    mEmbeddedMessageFont(nullptr),
    mMessageFont(nullptr),
    mPictureFont(nullptr),
    mMenuFont(nullptr),
    mNumberFont(nullptr)
{
    
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
    char embeddedFontPath[256];

    MR::getMountedArchiveAndHeap("ErrorMessageArchive.arc", &pArchive, &pHeap);
    snprintf(embeddedFontPath, sizeof(embeddedFontPath), "/%s/LayoutData/EmbeddedFont.arc", MR::getCurrentLanguagePrefix());

    _0 = MR::decompressFileFromArchive(pArchive, embeddedFontPath, pHeap, -32);
    _4 = new(pHeap, 0) JKRMemArchive();
    _4->mountFixed(_0, JKR_MEM_BREAK_FLAG_0);
    mEmbeddedMessageFont = new(pHeap, -4) nw4r::ut::ResFont();
    mEmbeddedMessageFont->SetResource(_4->getResource("MessageFont26.brfnt"));
}

void GameSystemFontHolder::createFontFromFile() {
    nw4r::ut::ResFont* pFont;
    JKRArchive* pArchive = reinterpret_cast< JKRMemArchive* >(MR::receiveArchive("/LayoutData/Font.arc"));

    pFont = new nw4r::ut::ResFont();
    pFont->SetResource(pArchive->getResource("/MessageFont26.brfnt"));
    mMessageFont = pFont;

    pFont = new nw4r::ut::ResFont();
    pFont->SetResource(pArchive->getResource("/PictureFont.brfnt"));
    mPictureFont = pFont;

    pFont = new nw4r::ut::ResFont();
    pFont->SetResource(pArchive->getResource("/MenuFont64.brfnt"));
    mMenuFont = pFont;

    pFont = new nw4r::ut::ResFont();
    pFont->SetResource(pArchive->getResource("/NumberFont.brfnt"));
    mNumberFont = pFont;

    pFont = new nw4r::ut::ResFont();
    pFont->SetResource(pArchive->getResource("/CinemaFont26.brfnt"));
    mCinemaFont = pFont;

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
