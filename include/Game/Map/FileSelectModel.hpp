#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class FileSelectModel : public LiveActor {
public:
    FileSelectModel(const char*, MtxPtr, const char*);

    /* 0x1C */ virtual void calcAnim();
    /* 0x4C */ virtual void calcAndSetBaseMtx();

    void open();
    void blinkOnce();
    void close();
    void blink();
    bool isOpen() const;
    void emitOpen();
    void emitVanish();
    void emitCopy();
    void emitCompleteEffect();
    void deleteCompleteEffect();

    void exeOpen();
    void exeBlinkOnce();
    void exeClose();
    void exeBlink();

    MtxPtr _8C;
};
