#include "Game/Map/FileSelector.hpp"
#include "Game/Screen/BackButton.hpp"
#include "Game/Screen/BrosButton.hpp"
#include "Game/Screen/FileSelectButton.hpp"
#include "Game/Screen/FileSelectInfo.hpp"
#include "Game/Screen/InformationMessage.hpp"
#include "Game/Screen/Manual2P.hpp"
#include "Game/Screen/MiiConfirmIcon.hpp"
#include "Game/Screen/MiiSelect.hpp"
#include "Game/Screen/MiiSelectIcon.hpp"
#include "Game/Screen/SysInfoWindow.hpp"
#include "Game/Screen/TitleSequenceProduct.hpp"
#include "JSystem/JKernel/JKRMemArchive.hpp"

void FileSelector::createBackButton() {
    mBackButton = new BackButton("戻るボタン", false);
    mBackButton->initWithoutIter();
    MR::connectToScene(mBackButton, 0xE, 0xD, -1, 0x3D);
}

void FileSelector::createBrosButton() {
    mBrosButton = new BrosButton("ルイージ切り替えボタン");
    mBrosButton->initWithoutIter();
}

void FileSelector::createInfoMessage() {
    mInfoMessage = new InformationMessage();
    mInfoMessage->initWithoutIter();
}

void FileSelector::createSysInfoWindow() {
    mSysInfoWindow = MR::createSysInfoWindow();
    MR::connectToSceneLayout(mSysInfoWindow);
    mSysInfoWindowMini = MR::createSysInfoWindowMiniExecuteWithChildren();
    MR::connectToSceneLayout(mSysInfoWindowMini);
}

void FileSelector::createFileInfo() {
    mSelectInfo = new FileSelectInfo(0xB, "ファイル情報");
    mSelectInfo->initWithoutIter();
}

void FileSelector::createTitle() {
    mTitleSeq = new TitleSequenceProduct();
    mTitleSeq->kill();
}

void FileSelector::createMiiSelect() {
    mMiiSelect = new MiiSelect("MiiSelect");
    mMiiSelect->initWithoutIter();
}

void FileSelector::createMiiConfirmIcon() {
    mMiiConfirmIcon = new MiiConfirmIcon("Mii確認用アイコン");
    mMiiConfirmIcon->initWithoutIter();
    MR::connectToScene(mMiiConfirmIcon, 0xE, 0xD, -1, 0x3D);
}

void FileSelector::createMiiFont() {
    JKRMemArchive* archive = MR::receiveArchive("/LayoutData/MiiFont.arc");
    mFont = new nw4r::ut::ResFont();
    mFont->SetResource(archive->getResource("/MiiFont26.brfnt"));
    mFont->SetAlternateChar('?');
    MR::setTextBoxFontRecursive(mSelectInfo, "FileName", mFont);
    MR::setTextBoxFontRecursive(mMiiSelect, "TxtName", mFont);
    MR::setTextBoxFontRecursive(mMiiConfirmIcon, "MiiName", mFont);
}

void FileSelector::createManual() {
    mManual2P = new Manual2P("２Ｐマニュアル");
    mManual2P->initWithoutIter();
}
